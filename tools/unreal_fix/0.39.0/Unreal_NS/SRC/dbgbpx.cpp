#include "std.h"

#include "resource.h"
#include "emul.h"
#include "vars.h"
#include "debug.h"
#include "dbgbpx.h"
#include "dbgcmd.h"
#include "config.h"
#include "util.h"

#include "emul_no_modal_windows.h"	// hwnd-ы немодальных окон [NS]
#include "dx.h"				// OnEnterGui [NS]
#include "draw.h"			// apply_video() [NS]

//=============================================================================
bool group_bpx_list = TRUE;						// [NS]
bool group_bpr_list = TRUE;
bool group_bpw_list = TRUE;
//=============================================================================
enum
{
    DB_STOP = 0,
    DB_CHAR,
    DB_SHORT,
    DB_PCHAR,
    DB_PSHORT,
    DB_PINT,
    DB_PFUNC
};
//=============================================================================
typedef bool (__cdecl *func_t)();

static unsigned calcerr;

//=============================================================================
unsigned calc( const Z80 *cpu, uintptr_t *script)
{
    unsigned stack[ 64];
    unsigned *sp = stack-1, x;
    //-------------------------------------------------------------------------
    while (*script) 
    {
	switch (*script++)
	{
	    case 'M':			*sp = cpu->DirectRm(*sp);		break;
	    case '!':			*sp = ! *sp;				break;
	    case '~':			*sp = ~ *sp;				break;
	    case '+':			*(sp-1) += *sp;				goto arith;
	    case '-':			*(sp-1) -= *sp;				goto arith;
	    case '*':			*(sp-1) *= *sp;				goto arith;
	    case '/':	if (*sp)	*(sp-1) /= *sp;				goto arith;
	    case '%':	if (*sp)	*(sp-1) %= *sp;				goto arith;
	    case '&':			*(sp-1) &= *sp;				goto arith;
	    case '|':			*(sp-1) |= *sp;				goto arith;
	    case '^':			*(sp-1) ^= *sp;				goto arith;
	    case WORD2('-','>'):	*(sp-1) = cpu->DirectRm(*sp + sp[-1]);	goto arith;
	    case WORD2('>','>'):	*(sp-1) >>= *sp;			goto arith;
	    case WORD2('<','<'):	*(sp-1) <<= *sp;			goto arith;
	    case WORD2('!','='):	*(sp-1) = (sp[-1] != *sp);		goto arith;
	    case '=':
	    case WORD2('=','='):	*(sp-1) = (sp[-1] == *sp);		goto arith;
	    case WORD2('<','='):	*(sp-1) = (sp[-1] <= *sp);		goto arith;
	    case WORD2('>','='):	*(sp-1) = (sp[-1] >= *sp);		goto arith;
	    case WORD2('|','|'):	*(sp-1) = (sp[-1] || *sp);		goto arith;
	    case WORD2('&','&'):	*(sp-1) = (sp[-1] && *sp);		goto arith;
	    case '<':			*(sp-1) = (sp[-1] < *sp);		goto arith;
	    case '>':			*(sp-1) = (sp[-1] > *sp);		goto arith;
arith:                sp--;  break;
	    case DB_CHAR:
	    case DB_SHORT:		x = unsigned(*script++);		goto push;
	    case DB_PCHAR:		x = *(unsigned char*)*script++;		goto push;
	    case DB_PSHORT:		x = 0xFFFF & *(unsigned*)*script++;	goto push;
	    case DB_PINT:		x = *(unsigned*)*script++;		goto push;
	    case DB_PFUNC:		x = ((func_t)*script++)();		goto push;
push:                 *++sp = x; break;
	} // switch (*script)
    } // while
    //-------------------------------------------------------------------------
    if (sp != stack)
	calcerr = 1;
    //-------------------------------------------------------------------------
    return *sp;
}
//=============================================================================


//=============================================================================
static bool __cdecl get_dos_flag()
{
    return (comp.flags & CF_DOSPORTS) != 0;
}
//=============================================================================

//=============================================================================
// АХТУНХ! В ЭТОЙ КОНСТРУКЦИИ НЕЛЬЗЯ НОРМАЛЬНО КОМЕНТИТЬ !!!
//
// соблюдать неповторяемость начал обратной елочкой !!!
// ABCD
// AB2
// AB
// A
// ибо сработает только самое первое похожее
// A
// ABCD = AB
// AB2
// AB


#define DECL_REGS(var, cpu)							\
    static struct								\
    {										\
	unsigned reg;								\
	const void *ptr;							\
	unsigned char size;							\
    } var[] =									\
    {										\
										\
	{ WORD4('I','N','T', 0 ),	&cpu.iff1,   1	},	 /* INT [NS] */	\
	{ WORD4('I','F','2', 0 ),	&cpu.iff2,   1	},	 /* IF2 [NS] */	\
	{ WORD4('H','L','T', 0 ),	&cpu.halted, 1	},	 /* HLT [NS] */	\
	{ WORD2('I','M'),		&cpu.im, 1 },	      /* IM = 0 [NS] */	\
							      /* IM = 1 [NS] */	\
							      /* IM = 2 [NS] */	\
										\
	{ WORD4('D','O','S', 0 ), (const void *)get_dos_flag, 0 }, /* DOS */	\
	{ WORD4('O','U','T', 0 ), 	&brk_port_out,	4  },	   /* OUT */	\
	{ WORD2('I','N'),		&brk_port_in,	4  },	   /* IN  */	\
	{ WORD4('V','A','L', 0 ),	&brk_port_val,	1  },	   /* VAL */	\
	{ WORD2('F','D'),		&comp.p7FFD,	1  },	   /* FD  */	\
										\
	{ WORD4('A','F','\'', 0 ),	&cpu.alt.af, 2	},			\
	{ WORD4('B','C','\'', 0 ),	&cpu.alt.bc, 2	},			\
	{ WORD4('D','E','\'', 0 ),	&cpu.alt.de, 2	},			\
	{ WORD4('H','L','\'', 0 ),	&cpu.alt.hl, 2	},			\
	{ WORD2('A','\''),		&cpu.alt.a,  1	},			\
	{ WORD2('F','\''),		&cpu.alt.f,  1	},			\
	{ WORD2('B','\''),		&cpu.alt.b,  1	},			\
	{ WORD2('C','\''),		&cpu.alt.c,  1	},			\
	{ WORD2('D','\''),		&cpu.alt.d,  1	},			\
	{ WORD2('E','\''),		&cpu.alt.e,  1	},			\
	{ WORD2('H','\''),		&cpu.alt.h,  1	},			\
	{ WORD2('L','\''),		&cpu.alt.l,  1	},			\
										\
	{ WORD4('a','f','\'', 0 ),	&cpu.alt.af, 2	},	/* [NS] */	\
	{ WORD4('b','c','\'', 0 ),	&cpu.alt.bc, 2	},	/* [NS] */	\
	{ WORD4('d','e','\'', 0 ),	&cpu.alt.de, 2	},	/* [NS] */	\
	{ WORD4('h','l','\'', 0 ),	&cpu.alt.hl, 2	},	/* [NS] */	\
	{ WORD4('a','F','\'', 0 ),	&cpu.alt.af, 2	},	/* [NS] */	\
	{ WORD4('b','C','\'', 0 ),	&cpu.alt.bc, 2	},	/* [NS] */	\
	{ WORD4('d','E','\'', 0 ),	&cpu.alt.de, 2	},	/* [NS] */	\
	{ WORD4('h','L','\'', 0 ),	&cpu.alt.hl, 2	},	/* [NS] */	\
	{ WORD4('A','f','\'', 0 ),	&cpu.alt.af, 2	},	/* [NS] */	\
	{ WORD4('B','c','\'', 0 ),	&cpu.alt.bc, 2	},	/* [NS] */	\
	{ WORD4('D','e','\'', 0 ),	&cpu.alt.de, 2	},	/* [NS] */	\
	{ WORD4('H','l','\'', 0 ),	&cpu.alt.hl, 2	},	/* [NS] */	\
	{ WORD2('a','\''),		&cpu.alt.a,  1	},	/* [NS] */	\
	{ WORD2('f','\''),		&cpu.alt.f,  1	},	/* [NS] */	\
	{ WORD2('b','\''),		&cpu.alt.b,  1	},	/* [NS] */	\
	{ WORD2('c','\''),		&cpu.alt.c,  1	},	/* [NS] */	\
	{ WORD2('d','\''),		&cpu.alt.d,  1	},	/* [NS] */	\
	{ WORD2('e','\''),		&cpu.alt.e,  1	},	/* [NS] */	\
	{ WORD2('h','\''),		&cpu.alt.h,  1	},	/* [NS] */	\
	{ WORD2('l','\''),		&cpu.alt.l,  1	},	/* [NS] */	\
										\
	{ WORD2('A','F'),	&cpu.af, 2	},		/* AF */	\
	{ WORD2('B','C'),	&cpu.bc, 2	},		/* BC */	\
	{ WORD2('D','E'),	&cpu.de, 2	},		/* DE */	\
	{ WORD2('H','L'),	&cpu.hl, 2	},		/* HL */	\
	{ 'A',			&cpu.a,  1	},				\
	{ 'F',			&cpu.f,  1	},				\
	{ 'B',			&cpu.b,  1	},				\
	{ 'C',			&cpu.c,  1	},				\
	{ 'D',			&cpu.d,  1	},				\
	{ 'E',			&cpu.e,  1	},				\
	{ 'H',			&cpu.h,  1	},				\
	{ 'L',			&cpu.l,  1	},				\
										\
	{ WORD2('P','C'),		&cpu.pc, 2 },		/* PC */	\
	{ WORD2('S','P'),		&cpu.sp, 2 },		/* SP */	\
										\
	{ WORD4('I','X','H', 0 ),	&cpu.xh, 1 },		/* IXH	[NS] */	\
	{ WORD4('I','X','L', 0 ),	&cpu.xl, 1 },		/* IXL	[NS] */	\
	{ WORD4('I','Y','H', 0 ),	&cpu.yh, 1 },		/* IYH	[NS] */	\
	{ WORD4('I','Y','L', 0 ),	&cpu.yl, 1 },		/* IYL	[NS] */	\
										\
	{ WORD2('X','H'),		&cpu.xh, 1 },		/* XH	[NS] */	\
	{ WORD2('X','L'),		&cpu.xl, 1 },		/* XL	[NS] */	\
	{ WORD2('Y','H'),		&cpu.yh, 1 },		/* YH	[NS] */	\
	{ WORD2('Y','L'),		&cpu.yl, 1 },		/* YL	[NS] */	\
										\
	{ WORD2('I','X'),		&cpu.ix, 2 },		/* IX */	\
	{ WORD2('I','Y'),		&cpu.iy, 2 },		/* IY */	\
										\
	{ 'I',			&cpu.i,     1	},				\
	{ 'R',			&cpu.r_low, 1	},  /* вроде бы d7 работает */	\
										\
	{ 'T',			&cpu.t,     4	},		/* T	[NS] */	\
    }
// АХТУНХ! В ЭТОЙ КОНСТРУКЦИИ НЕЛЬЗЯ НОРМАЛЬНО КОМЕНТИТЬ !!!

// 	{ WORD4('N','P','C', 0 ),	&cpu.nextpc, 2 }, - не работатет а жаль
//=============================================================================




//=============================================================================
static unsigned char toscript( char *script, uintptr_t *dst)
{
    uintptr_t *d1 = dst;
    //-------------------------------------------------------------------------
    static struct 
    {
	unsigned short op;
	unsigned char prior;
    } prio[] = 
    {
	{ 	'(',	  10	},
	{ 	')',	  0	},
	{ 	'!',	  1	},
	{ 	'~',	  1	},
	{ 	'M',	  1	},
	{ WORD2('-','>'), 1	},
	{ 	'*',	  2	},
	{ 	'%',	  2	},
	{ 	'/',	  2	},
	{ 	'+',	  3	},
	{ 	'-',	  3	},
	{ WORD2('>','>'), 4	},
	{ WORD2('<','<'), 4	},
	{ 	'>', 	  5	},
	{ 	'<', 	  5	},
	{ 	'=', 	  5	},
	{ WORD2('>','='), 5	},
	{ WORD2('<','='), 5	},
	{ WORD2('=','='), 5	},
	{ WORD2('!','='), 5	},
	{ 	'&',	  6	},
	{ 	'^',	  7	},
	{ 	'|',	  8	},
	{ WORD2('&','&'), 9	},
	{ WORD2('|','|'), 10	}
    };
    //-------------------------------------------------------------------------
    
    const Z80 &cpu = CpuMgr.Cpu();

    DECL_REGS( regs, cpu);

    unsigned sp = 0;
    uintptr_t stack[ 128];
    //-------------------------------------------------------------------------
    for (char *p = script;    *p;    p++)
	if (p[1] != 0x27)
	    *p = char( toupper( *p));
    //-------------------------------------------------------------------------
    while (*script)
    {
	//---------------------------------------------------------------------
	if (*(unsigned char*) script <= ' ')
	{
	    script++;
	    continue;
	}
	//---------------------------------------------------------------------
	if (*script == '\'')
	{ // char
	    *dst++ = DB_CHAR;
	    *dst++ = uintptr_t( script[1]);
	    //-----------------------------------------------------------------
	    if (script[2] != '\'')
		return 0;
	    //-----------------------------------------------------------------
	    script += 3;
	    continue;
	}
	//---------------------------------------------------------------------
	if (isalnum( *script) && *script != 'M')
	{
	    unsigned r = -1U;
	    unsigned p = *(unsigned*) script;
	    unsigned ln = 0;
	    //-----------------------------------------------------------------
	    for (unsigned i = 0;    i < _countof( regs);    i++)
	    {
		unsigned mask = 0xFF;
		ln = 1;
		//-------------------------------------------------------------
		if (regs[i].reg & 0xFF00)
		{
		    mask = 0xFFFF;
		    ln = 2;
		}
		//-------------------------------------------------------------
		if (regs[i].reg & 0xFF0000)
		{
		    mask = 0xFFFFFF;
		    ln = 3;
		}
		//-------------------------------------------------------------
		if (regs[i].reg == (p & mask))
		{
		    r = i;
		    break;
		}
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    if (r != -1U)
	    {
		script += ln;
		//-------------------------------------------------------------
		switch (regs[r].size)
		{
		    case 0:	*dst++ = DB_PFUNC;	break;
		    case 1:	*dst++ = DB_PCHAR;	break;
		    case 2:	*dst++ = DB_PSHORT;	break;
		    case 4:	*dst++ = DB_PINT;	break;
		    default:	errexit("BUG01");
		}
		//-------------------------------------------------------------
		*dst++ = (uintptr_t)regs[r].ptr;
	    }
	    //-----------------------------------------------------------------
	    else
	    { // number
		//-------------------------------------------------------------
		if (*script > 'F') return 0;
		//-------------------------------------------------------------
		for (r = 0;    isalnum( *script) && *script <= 'F';    script++)
		{
		    r = r * 0x10 + unsigned( (*script >= 'A') ?	*script-'A' + 10 :
								*script-'0');
		}
		//-------------------------------------------------------------
		*dst++ = DB_SHORT;
		*dst++ = r;
	    }
	    //-----------------------------------------------------------------
	    continue;
	}
	//---------------------------------------------------------------------
	// find operation
	unsigned char pr = 0xFF;
	unsigned r = (unsigned)*script++;
	//---------------------------------------------------------------------
	if (strchr("<>=&|-!", (char)r) && strchr("<>=&|", *script))
	    r += unsigned( 0x100 * (*script++));
	//---------------------------------------------------------------------
	for (unsigned i = 0; i < _countof( prio); i++)
	{
	    if (prio[i].op == r)
	    {
		pr = prio[i].prior;
		break;
	    }
	}
	//---------------------------------------------------------------------
	if (pr == 0xFF)
	    return 0;
	//---------------------------------------------------------------------
	if (r != '(')
	{
	    while (sp && ((stack[ sp] >> 16 <= pr) || (r == ')' && (stack[ sp] & 0xFF) != '(')))
	    { // get from stack
		*dst++ = stack[sp--] & 0xFFFF;
	    }
	}
	//---------------------------------------------------------------------
	if (r == ')')
	{
	    sp--;			// del opening bracket
	}
	//---------------------------------------------------------------------
	else
	{
	    stack[ ++sp] = r + 0x10000 * pr;	// put to stack
	}
	//---------------------------------------------------------------------
	if ((int) sp < 0)
	    return 0;			// no opening bracket
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    // empty stack
    while (sp)
    {
	//---------------------------------------------------------------------
	if ((stack[ sp] & 0xFF) == '(')
	    return 0; // no closing bracket
	//---------------------------------------------------------------------
	*dst++ = stack[ sp--] & 0xFFFF;
    }
    //-------------------------------------------------------------------------
    *dst = DB_STOP;

    calcerr = 0;
    calc( &cpu, d1);
    return u8( 1 - calcerr);
}
//=============================================================================

// тк дальше глюки npp подсветки
// такой костыль
	int npp_higlight_shit_costil = (')');	// [NS]

//=============================================================================
static void script2text( char *dst, const uintptr_t *src)
{
    char stack[ 64][ 0x200];
    char tmp[ 0x200];
    unsigned sp = 0;
    uintptr_t r;

    const Z80 &cpu = CpuMgr.Cpu();

    DECL_REGS( regs, cpu);
    //-------------------------------------------------------------------------
    while ((r = *src++))
    {
	//---------------------------------------------------------------------
	if (r == DB_CHAR)
	{
	    sprintf( stack[ sp++], "'%c'", int( *src++));
	    continue;
	}
	//---------------------------------------------------------------------
	if (r == DB_SHORT)
	{
	    sprintf( stack[ sp], "0%X", unsigned( *src++));
	    //-----------------------------------------------------------------
	    if (isdigit( stack[ sp][ 1]))
		strcpy( stack[ sp], stack[ sp] + 1);
	    //-----------------------------------------------------------------
	    sp++;
	    continue;
	}
	//---------------------------------------------------------------------
	if ((r >= DB_PCHAR) && (r <= DB_PFUNC))
	{
	    unsigned sz = 0;
	    //-----------------------------------------------------------------
	    switch (r)
	    {
		case DB_PCHAR:	sz = 1;	break;
		case DB_PSHORT:	sz = 2;	break;
		case DB_PINT:	sz = 4;	break;
		case DB_PFUNC:	sz = 0;	break;
	    }
	    //-----------------------------------------------------------------
	    unsigned i;
	    for (i = 0;    i < _countof( regs);    i++)
	    {
		if ((*src == (uintptr_t) regs[ i].ptr) && (sz == regs[ i].size))
		    break;
	    }
	    //-----------------------------------------------------------------
	    *(unsigned*) &(stack[ sp++]) = regs[ i].reg;
	    src++;
	    continue;
	}
	//---------------------------------------------------------------------
	if ((r == 'M') || (r == '~') || (r == '!'))
	{ // unary operators
	    sprintf( tmp, "%c(%s)", int( r), stack[ sp - 1]);
	    strcpy( stack[ sp - 1], tmp);
	    continue;
	}
	//---------------------------------------------------------------------
	// else binary operators
	sprintf( tmp, "(%s%s%s)", stack[ sp - 2], (char*) &r, stack[ sp - 1]);
	sp--; 
	strcpy( stack[ sp - 1], tmp);
    }
    //-------------------------------------------------------------------------
    if (!sp)
    {
	*dst = 0;
    }
    //-------------------------------------------------------------------------
    else
    {
	strcpy( dst, stack[ sp - 1]);
    }
    //-------------------------------------------------------------------------
}
//=============================================================================


//=============================================================================
static void SetBpxButtons( HWND dlg)
{
    int focus = -1;
    int text = 0;
    int box = 0;
    
    HWND focusedWnd = GetFocus();
    //-------------------------------------------------------------------------
    // Перевод фокуса и засерение груп при клацании
    //-------------------------------------------------------------------------
    // Condition BP - EDITTEXT / LISTBOX
    if ( (focusedWnd == GetDlgItem( dlg, IDE_CBP))	|| 
	 (focusedWnd == GetDlgItem( dlg, IDC_CBP))
     )
    {
	focus = 0;
	text = IDE_CBP;
	box = IDC_CBP;
    }
    //-------------------------------------------------------------------------
    // Execute BP - EDITTEXT / LISTBOX
    if ( (focusedWnd == GetDlgItem( dlg, IDE_BPX))	|| 
	 (focusedWnd == GetDlgItem( dlg, IDC_BPX))
     )
    {
	focus = 1;
	text = IDE_BPX;
	box = IDC_BPX;
    }
    //-------------------------------------------------------------------------
    // Read BP - EDITTEXT / LISTBOX
    if ( (focusedWnd == GetDlgItem( dlg, IDE_MEM_RD))	||
         (focusedWnd == GetDlgItem( dlg, IDC_MEM_RD))
     )
    {
	focus = 2;
	text = IDE_MEM_RD;
	box = IDC_MEM_RD;
    }
    //-------------------------------------------------------------------------
    // Write BP - EDITTEXT / LISTBOX
    if ( (focusedWnd == GetDlgItem( dlg, IDE_MEM_WR))	||
         (focusedWnd == GetDlgItem( dlg, IDC_MEM_WR))
     )
    {
	focus = 3;
	text = IDE_MEM_WR;
	box = IDC_MEM_WR;
    }
    //-------------------------------------------------------------------------
    /*
	    // DEPRECATED WR/RD
	    if (	(focusedWnd == GetDlgItem( dlg, IDE_MEM))	||
			(focusedWnd == GetDlgItem( dlg, IDC_MEM))	||
			(focusedWnd == GetDlgItem( dlg, IDC_MEM_R))	||
			(focusedWnd == GetDlgItem( dlg, IDC_MEM_W))
	     )
	    {
		focus = 2;
		text = IDE_MEM;
		box = IDC_MEM;
	    }
    */
    //-------------------------------------------------------------------------
    SendDlgItemMessage( dlg, IDE_CBP,    EM_SETREADONLY, (BOOL) (focus != 0), 0);
    SendDlgItemMessage( dlg, IDE_BPX,    EM_SETREADONLY, (BOOL) (focus != 1), 0);
    SendDlgItemMessage( dlg, IDE_MEM_RD, EM_SETREADONLY, (BOOL) (focus != 2), 0); // [NS]
    SendDlgItemMessage( dlg, IDE_MEM_WR, EM_SETREADONLY, (BOOL) (focus != 3), 0); // [NS]
    //-------------------------------------------------------------------------
    int del0 = 0;
    int add0 = 0;
    int del1 = 0;
    int add1 = 0;
    int del2 = 0;
    int add2 = 0;
    int del3 = 0; // [NS]
    int add3 = 0; // [NS]
    //-------------------------------------------------------------------------
    unsigned max = unsigned( SendDlgItemMessage( dlg, box,  LB_GETCOUNT,      0, 0));
    unsigned cur = unsigned( SendDlgItemMessage( dlg, box,  LB_GETCURSEL,     0, 0));
    unsigned len = unsigned( SendDlgItemMessage( dlg, text, WM_GETTEXTLENGTH, 0, 0));
    //-------------------------------------------------------------------------
    if (max && cur >= max)
	SendDlgItemMessage( dlg, box, LB_SETCURSEL, cur = 0, 0);
    //-------------------------------------------------------------------------
    // Condition BP
    if (focus == 0)
    {
	if (len && max < MAX_CBP) add0 = 1;
	if (cur < max) del0 = 1;
    }
    //-------------------------------------------------------------------------
    // Execute BP 
    if (focus == 1)
    {
	if (len) add1 = 1;
	if (cur < max) del1 = 1;
    }
    //-------------------------------------------------------------------------
    // Read BP
    if (focus == 2)
    {
	/*
	//---------------------------------------------------------------------
	if (	(IsDlgButtonChecked( dlg, IDC_MEM_R) == BST_UNCHECKED) &&
		(IsDlgButtonChecked( dlg, IDC_MEM_W) == BST_UNCHECKED)
	 )
	{
	    len = 0;
	}
	//---------------------------------------------------------------------
	*/
	if (len) add2 = 1;
	if (cur < max) del2 = 1;
    }
    //-------------------------------------------------------------------------
    // Write BP
    if (focus == 3)
    {
	if (len) add3 = 1;
	if (cur < max) del3 = 1;
    }
    //-------------------------------------------------------------------------

    EnableWindow( GetDlgItem( dlg, IDB_CBP_ADD),  add0);
    EnableWindow( GetDlgItem( dlg, IDB_CBP_DEL),  del0);
    EnableWindow( GetDlgItem( dlg, IDB_CBP_EDIT), del0); // совпадает с del
    
    EnableWindow( GetDlgItem( dlg, IDB_BPX_ADD),  add1);
    EnableWindow( GetDlgItem( dlg, IDB_BPX_DEL),  del1);
    EnableWindow( GetDlgItem( dlg, IDB_BPX_EDIT), del1); // совпадает с del
    
    EnableWindow( GetDlgItem( dlg, IDB_MEM_RD_ADD),  add2); // [NS]
    EnableWindow( GetDlgItem( dlg, IDB_MEM_RD_DEL),  del2);
    EnableWindow( GetDlgItem( dlg, IDB_MEM_RD_EDIT), del2); // совпадает с del
    
    EnableWindow( GetDlgItem( dlg, IDB_MEM_WR_ADD),  add3); // [NS]
    EnableWindow( GetDlgItem( dlg, IDB_MEM_WR_DEL),  del3);
    EnableWindow( GetDlgItem( dlg, IDB_MEM_WR_EDIT), del3); // совпадает с del
    
    
    unsigned defid = 0;
    //-------------------------------------------------------------------------
    if (add0) defid = IDB_CBP_ADD;
    if (add1) defid = IDB_BPX_ADD;
    if (add2) defid = IDB_MEM_RD_ADD;
    if (add3) defid = IDB_MEM_WR_ADD;
    //-------------------------------------------------------------------------
    if (defid)
	SendMessage( dlg, DM_SETDEFID, defid, 0);
    //-------------------------------------------------------------------------
}
//=============================================================================


//=============================================================================
/*
// лютейший говнокод неизвестного аффтара
// даже не знание про LB_DELETESTRING не оправдание килотоннам тоннам LB_GETCOUNT
static void ClearListBox( HWND box)
{
    //-------------------------------------------------------------------------
    while (SendMessage( box, LB_GETCOUNT, 0, 0))
    {
	SendMessage( box, LB_DELETESTRING, 0, 0);
    }
    //-------------------------------------------------------------------------
}
*/
//=============================================================================


//=============================================================================
static void FillCondBox( HWND dlg, unsigned cursor)
{
    HWND box = GetDlgItem( dlg, IDC_CBP);
    
    SendMessage( box, WM_SETREDRAW, FALSE, 0);	// отключаем перерисовку списка [NS]
    
    //ClearListBox( box);
    SendMessage( box, LB_RESETCONTENT, 0, 0);	// [NS]

    Z80 &cpu = CpuMgr.Cpu();
    //-------------------------------------------------------------------------
    for (unsigned i = 0;    i < cpu.cbpn;    i++)
    {
	char tmp[ 0x200];
	script2text( tmp, cpu.cbp[ i]);
	SendMessage( box, LB_ADDSTRING, 0, (LPARAM) tmp);
    }
    //-------------------------------------------------------------------------
    SendMessage( box, LB_SETCURSEL, cursor, 0);
    
    SendMessage( box, WM_SETREDRAW, TRUE, 0);	// включаем перерисовку списка [NS]
}
//=============================================================================


//=============================================================================
static void FillBpxBox( HWND dlg, unsigned address)
{
    //printf("FillBpxBox ");
    HWND box = GetDlgItem( dlg, IDC_BPX);

    SendMessage( box, WM_SETREDRAW, FALSE, 0);	// отключаем перерисовку списка [NS]

    //ClearListBox( box);	// очистка тоже долго с перерисовкой!!!
				// напорядки больше чем создание !!!
    SendMessage( box, LB_RESETCONTENT, 0, 0);	// [NS]
    
    unsigned selection = 0;

    Z80 &cpu = CpuMgr.Cpu();
    unsigned end; 		// Alone Coder 0.36.7
    //-------------------------------------------------------------------------
    for (unsigned start = 0;    start < 0x10000;    )
    {
	//---------------------------------------------------------------------
	if (!(cpu.membits[ start] & MEMBITS_BPX))
	{
	    start++;
	    continue;
	}
	//---------------------------------------------------------------------
	for (	/*unsigned*/ end = start
		;
		(end < 0xFFFF) 				&&	// диапазон поиска
		(group_bpx_list)			&&	// отключение групировки
		(cpu.membits[ end + 1] & MEMBITS_BPX)	&&
		// + У блока должна быть такая же отключенность
		// иначе это разные бряки
		((cpu.bp_disable_bits[ end + 1] & BP_DISABLE_BPX) == (cpu.bp_disable_bits[ end] & BP_DISABLE_BPX))	
		;
		end++ );
	//---------------------------------------------------------------------
	char tmp[ 16];
	//---------------------------------------------------------------------
	if (start == end)
	{
	    if (cpu.bp_disable_bits[ start] & BP_DISABLE_BPX)
	    {
		sprintf( tmp, "%04X <-- DISABLED", start);
	    }
	    else
	    {
		sprintf( tmp, "%04X", start);
	    }
	}
	//---------------------------------------------------------------------
	else
	{
	    // "-" между цифр нельзя менять !!!!
	    // тк диапазон для заливки\удаления\отключения
	    // ВНЕЗАПНО читаетсо прямо из этого текста
	    //
	    // по идеи весь блок с одинаковым режимом
	    if (cpu.bp_disable_bits[ start] & BP_DISABLE_BPX)
	    {
		sprintf( tmp, "%04X-%04X <-- DISABLED", start, end);
	    }
	    else
	    {
		sprintf( tmp, "%04X-%04X", start, end);
	    }
	}
	//---------------------------------------------------------------------
	SendMessage( box, LB_ADDSTRING, 0, (LPARAM) tmp);
	
		//	int i_temp = SendMessage( box, LB_ADDSTRING, 0, (LPARAM) tmp);
		//	printf("%x\n",i_temp);
		//TZT
			//SendDlgItemMessage(hDlg, id, iMsg, wParam, lParam);
			//SendMessage(hwnd, message, wParam, lParam);
		//	i_temp = SendMessage( box, LB_GETITEMDATA, i_temp, 0);
		//	printf("  %x\n",i_temp);

	//---------------------------------------------------------------------
	if ((start <= address) && (address <= end))
	    selection = unsigned( SendMessage( box, LB_GETCOUNT, 0, 0));
	//---------------------------------------------------------------------
	start = end + 1;
    }
    //-------------------------------------------------------------------------
    if (selection)
	SendMessage( box, LB_SETCURSEL, selection - 1, 0);
    //-------------------------------------------------------------------------
    SendMessage( box, WM_SETREDRAW, TRUE, 0);	// включаем перерисовку списка [NS]

    needclr++;	// обновить окно дебагера
}
//=============================================================================





//=============================================================================
static void Fill_Mem_RD_Box( HWND dlg, unsigned address)		// [NS]
{
    //printf("Fill_Mem_RD_Box ");
    HWND box = GetDlgItem( dlg, IDC_MEM_RD);

    SendMessage( box, WM_SETREDRAW, FALSE, 0);	// отключаем перерисовку списка [NS]
    
    //ClearListBox( box);
    SendMessage( box, LB_RESETCONTENT, 0, 0);	// [NS]

    unsigned selection = 0;

    Z80 &cpu = CpuMgr.Cpu();
    unsigned end; 		// Alone Coder 0.36.7
    //-------------------------------------------------------------------------
    for (unsigned start = 0;    start < 0x10000;    )
    {
	//---------------------------------------------------------------------
	if (!(cpu.membits[ start] & MEMBITS_BPR))
	{
	    start++;
	    continue;
	}
	//---------------------------------------------------------------------
	for (	/*unsigned*/ end = start
		;
		(end < 0xFFFF) 				&&
		(group_bpr_list)			&&	// отключение групировки
		(cpu.membits[ end + 1] & MEMBITS_BPR)	&&
		// + У блока должна быть такая же отключенность
		// иначе это разные бряки
		((cpu.bp_disable_bits[ end + 1] & BP_DISABLE_BPR) == (cpu.bp_disable_bits[ end] & BP_DISABLE_BPR))	
		;
		end++ );
	//---------------------------------------------------------------------
	char tmp[ 16];
	//---------------------------------------------------------------------
	if (start == end)
	{
	    if (cpu.bp_disable_bits[ start] & BP_DISABLE_BPR)
	    {
		sprintf( tmp, "%04X <-- DISABLED", start);
	    }
	    else
	    {
		sprintf( tmp, "%04X", start);
	    }
	}
	//---------------------------------------------------------------------
	else
	{
	    // "-" между цифр нельзя менять !!!!
	    // тк диапазон для заливки\удаления\отключения
	    // ВНЕЗАПНО читаетсо прямо из этого текста
	    //
	    // по идеи весь блок с одинаковым режимом
	    if (cpu.bp_disable_bits[ start] & BP_DISABLE_BPR)
	    {
		sprintf( tmp, "%04X-%04X <-- DISABLED", start, end);
	    }
	    else
	    {
		sprintf( tmp, "%04X-%04X", start, end);
	    }
	}
	//---------------------------------------------------------------------
	SendMessage( box, LB_ADDSTRING, 0, (LPARAM) tmp);
	//---------------------------------------------------------------------
	if ((start <= address) && (address <= end))
	    selection = unsigned( SendMessage( box, LB_GETCOUNT, 0, 0));
	//---------------------------------------------------------------------
	start = end + 1;
    }
    //-------------------------------------------------------------------------
    if (selection)
	SendMessage( box, LB_SETCURSEL, selection - 1, 0);
    //-------------------------------------------------------------------------
    SendMessage( box, WM_SETREDRAW, TRUE, 0);	// включаем перерисовку списка [NS]
    
    needclr++;	// обновить окно дебагера
}
//=============================================================================


//=============================================================================
static void Fill_Mem_WR_Box( HWND dlg, unsigned address)		// [NS]
{
    HWND box = GetDlgItem( dlg, IDC_MEM_WR);

    SendMessage( box, WM_SETREDRAW, FALSE, 0);	// отключаем перерисовку списка [NS]

    //ClearListBox( box);			// очистка тоже долго с перерисовкой!!!
    SendMessage( box, LB_RESETCONTENT, 0, 0);	// [NS]

    unsigned selection = 0;

    SendMessage( box, WM_SETREDRAW, FALSE, 0);	// отключаем перерисовку списка [NS]
    
    Z80 &cpu = CpuMgr.Cpu();
    unsigned end; 		// Alone Coder 0.36.7
    //-------------------------------------------------------------------------
    for (unsigned start = 0;    start < 0x10000;    )
    {
	//---------------------------------------------------------------------
	if (!(cpu.membits[ start] & MEMBITS_BPW))
	{
	    start++;
	    continue;
	}
	//---------------------------------------------------------------------
	for (	/*unsigned*/ end = start
		;
		(end < 0xFFFF)				&&
		(group_bpw_list)			&&	// отключение групировки
		(cpu.membits[ end + 1] & MEMBITS_BPW)	&&
		// + У блока должна быть такая же отключенность
		// иначе это разные бряки
		((cpu.bp_disable_bits[ end + 1] & BP_DISABLE_BPW) == (cpu.bp_disable_bits[ end] & BP_DISABLE_BPW))
		;
		end++ );
	//---------------------------------------------------------------------
	char tmp[ 16];
	//---------------------------------------------------------------------
	if (start == end)
	{
	    if (cpu.bp_disable_bits[ start] & BP_DISABLE_BPW)
	    {
		sprintf( tmp, "%04X <-- DISABLED", start);
	    }
	    else
	    {
		sprintf( tmp, "%04X", start);
	    }
	}
	//---------------------------------------------------------------------
	else
	{
	    // "-" между цифр нельзя менять !!!!
	    // тк диапазон для заливки\удаления\отключения
	    // ВНЕЗАПНО читаетсо прямо из этого текста
	    //
	    // по идеи весь блок с одинаковым режимом
	    if (cpu.bp_disable_bits[ start] & BP_DISABLE_BPW)
	    {
		sprintf( tmp, "%04X-%04X <-- DISABLED", start, end);
	    }
	    else
	    {
		sprintf( tmp, "%04X-%04X", start, end);
	    }
	}
	//---------------------------------------------------------------------
	SendMessage( box, LB_ADDSTRING, 0, (LPARAM) tmp);
	//---------------------------------------------------------------------
	if ((start <= address) && (address <= end))
	    selection = unsigned( SendMessage( box, LB_GETCOUNT, 0, 0));
	//---------------------------------------------------------------------
	start = end + 1;
    }
    //-------------------------------------------------------------------------
    if (selection)
	SendMessage( box, LB_SETCURSEL, selection - 1, 0);
    //-------------------------------------------------------------------------
    SendMessage( box, WM_SETREDRAW, TRUE, 0);	// включаем перерисовку списка [NS]

    needclr++;	// обновить окно дебагера
}
//=============================================================================



//=============================================================================
static char MoveBpxFromBoxToEdit( HWND dlg, unsigned box, unsigned edit)
{
    HWND hBox = GetDlgItem( dlg, int( box));
    unsigned max = unsigned( SendDlgItemMessage( dlg, box, LB_GETCOUNT,  0, 0));
    unsigned cur = unsigned( SendDlgItemMessage( dlg, box, LB_GETCURSEL, 0, 0));
    //-------------------------------------------------------------------------
    if (cur >= max)
	return 0;
    //-------------------------------------------------------------------------
    char tmp[ 0x200];
    SendMessage( hBox, LB_GETTEXT, cur, (LPARAM) tmp);
    
    char *temp_addr = strstr( tmp, " <-");
    //-------------------------------------------------------------------------
    // если строка не найдена то strstr возвращает 0 !!!!
    if ((temp_addr) != 0)
    {
	//типо затираем " <-- DISABLED"
	*temp_addr = 0;
    }
    //-------------------------------------------------------------------------
    /*
    if ((box == IDC_MEM) && *tmp)
    {
	char *last = tmp + strlen( tmp);
	unsigned r = BST_UNCHECKED;
	unsigned w = BST_UNCHECKED;
	//---------------------------------------------------------------------
	if (last[ -1] == 'W')
	{
	    w = BST_CHECKED;
	    last--;
	}
	//---------------------------------------------------------------------
	if (last[ -1] == 'R')
	{
	    r = BST_CHECKED;
	    last--;
	}
	//---------------------------------------------------------------------
	if (last[ -1] == ' ')
	    last--;
	//---------------------------------------------------------------------
	*last = 0;
	CheckDlgButton( dlg, IDC_MEM_R, r);
	CheckDlgButton( dlg, IDC_MEM_W, w);
    }
    */
    //-------------------------------------------------------------------------
    SetDlgItemText( dlg, edit, tmp);
    return 1;
}
//=============================================================================


//=============================================================================
struct MEM_RANGE
{
    unsigned start;
    unsigned end;
};
//=============================================================================


//=============================================================================
static int GetMemRamge( char *str, MEM_RANGE &range)
{
    //-------------------------------------------------------------------------
    while (*str == ' ')
	str++;
    //-------------------------------------------------------------------------
    for (range.start = 0;    ishex( *str);    str++)
	range.start = range.start * 0x10 + hex( *str);
    //-------------------------------------------------------------------------
    if (*str == '-')
    {
	//---------------------------------------------------------------------
	for (range.end = 0, str++;    ishex( *str);    str++)
	    range.end = range.end * 0x10 + hex( *str);
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    else
    {
	range.end = range.start;
    }
    //-------------------------------------------------------------------------
    while (*str == ' ')
	str++;
    //-------------------------------------------------------------------------
    if (*str)
	return 0;
    //-------------------------------------------------------------------------
    if (range.start > range.end)		// [NS]
    {
	unsigned temp = range.end;
	range.end = range.start;
	range.start = temp;
    }
    //-------------------------------------------------------------------------
    if ((range.start > 0xFFFF) || (range.end > 0xFFFF))
	return 0;
    //-------------------------------------------------------------------------
    return 1;
}
//=============================================================================









//=============================================================================
void conddlg_update_texts( HWND dlg)					// [NS]
{
    LPCSTR temp_CPU_name;
    //-------------------------------------------------------------------------
    if ( (CpuMgr.GetCurrentCpu()) == 0 )
    {
        temp_CPU_name = "Current CPU: ZX-CPU";
    }
    //-------------------------------------------------------------------------
    else
    {
        temp_CPU_name = "Current CPU: GS-CPU";
    }
    //-------------------------------------------------------------------------
    //SetWindowText( GetDlgItem( dlg, IDT_CURRENT_CPU_INDICATOR), temp_CPU_name);
    SetWindowText( GetDlgItem( dlg, IDB_SWITCH_CPU), temp_CPU_name);
}
//=============================================================================























//=============================================================================


    static int conddlg_current_visible_cpu = 0;	
    
//=============================================================================
// обработчик сообщений диалога breakpoint-ов

static INT_PTR CALLBACK conddlg( HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
    (void) lp;
        
    // бекапим текущий CPU
    // тк conddlg ввызываетсо рекурсивно !!!!
    // нужно возвращать всегда предыдущее состояние !!
    // NOT STATIC !!!
    int current_cpu_back = CpuMgr.GetCurrentCpu();	// NOT STATIC !!!
    
    // меняем текущий CPU
    CpuMgr.SetCurrentCpu( conddlg_current_visible_cpu);
    
    Z80 &cpu = CpuMgr.Cpu();
    
    // НА ВСЕХ ВЫХОДАХ ИЗ conddlg()
    // НУЖНО ВОСТАНАВЛИВАТЬ CPU на место!!!!
    // это проще чем поправить все записи
    
    // printf("msg %x wp %x lp %x\n",msg,wp,lp);
    
    //-------------------------------------------------------------------------
    // цвета для listbox-ов
    static HBRUSH hBrushStatic;
    static COLORREF color_cbp = { RGB(   0, 100, 200) };
    static COLORREF color_bpx = { RGB(   0,   0, 255) };
    static COLORREF color_bpw = { RGB( 160,  16,  16) };
    static COLORREF color_bpr = { RGB(  16, 140,  16) };
    //-------------------------------------------------------------------------
 
    unsigned id = LOWORD( wp);
    unsigned code = HIWORD( wp);
    unsigned char mask = 0;
    unsigned char disable_bp_mask = 0;

    //=========================================================================
    if (msg == WM_INITDIALOG)						// [NS]
    {	    
	// для разукрашки listbox-ов
	hBrushStatic = CreateSolidBrush( GetSysColor( COLOR_BTNHIGHLIGHT));
	
	// текущий видимый проц на момент входа
	// нужно при входе из дебагера когда активен GS по хоткею
	conddlg_current_visible_cpu = current_cpu_back;
	CpuMgr.SetCurrentCpu( conddlg_current_visible_cpu);
	Z80 &cpu = CpuMgr.Cpu();
	//printf("current_visible_cpu %x\n",current_visible_cpu);
    }
    //=========================================================================					
    if (msg == WM_DESTROY)						// [NS]
    {
	DeleteObject( hBrushStatic);
    }
    //=========================================================================

    // ODT_LISTBOX
    // LB_GETLISTBOXINFO 0x01B2
    

    //=========================================================================
    // разукрашка listbox-ов методом научного тыка			// [NS]
    // к сожалению мы не знаем как красить отдельные строки
    // так покрасим хоть шо нибудь (так лучше различаютсо колонки)
    if (msg == WM_CTLCOLORLISTBOX)
    {
	// int i;
	// i = GetWindowLong ((HWND) lp, GWL_ID);	
	//---------------------------------------------------------------------
	if	(lp == (WPARAM) GetDlgItem( dlg, IDC_CBP))
	{
	    SetTextColor ((HDC) wp, color_cbp);
	}
	//---------------------------------------------------------------------
	else if	(lp == (WPARAM) GetDlgItem( dlg, IDC_BPX))
	{
	    SetTextColor ((HDC) wp, color_bpx);
	}
	//---------------------------------------------------------------------
	else if	(lp == (WPARAM) GetDlgItem( dlg, IDC_MEM_RD))
	{
	    SetTextColor ((HDC) wp, color_bpr);
	}
	//---------------------------------------------------------------------
	else if	(lp == (WPARAM) GetDlgItem( dlg, IDC_MEM_WR))
	{
	    SetTextColor ((HDC) wp, color_bpw);
	}
	//---------------------------------------------------------------------
	//SetBkColor ((HDC) wp, GetSysColor( COLOR_BTNHIGHLIGHT));
	
	// возвращаем CPU на место перед выходом
	CpuMgr.SetCurrentCpu( current_cpu_back);
	
	return (LRESULT) hBrushStatic;
    }
    //=========================================================================
    

    //=========================================================================
    // Контексные Менюшки
    //     а шо так можно мы узнали при помощи winuser.h
    //     и метода научного тыка приплясывая с бубном
    //     всякие там Педзольды молчат как партизанэн
    //     а правые клики мышой тупо сюдава недоходят о_О
    //     даже как noclient
    //
    if (msg == WM_CONTEXTMENU)	// [NS]
    {
	// wp - HWND того куда мы ткнули
	// lp - координаты куда мы ткнули в screen формате
	bool display_listbox_menu;
	int current_listbox_menu;
	//=====================================================================
	// Conditional BP
	if	(wp == (WPARAM) GetDlgItem( dlg, IDC_CBP))
	{
	    display_listbox_menu = TRUE;
	    current_listbox_menu = IDC_CBP;
	}
	//---------------------------------------------------------------------
	// Execute BP
	else if	(wp == (WPARAM) GetDlgItem( dlg, IDC_BPX))
	{
	    display_listbox_menu = TRUE;
	    current_listbox_menu = IDC_BPX;
	}
	//---------------------------------------------------------------------
	// Read BP
	else if	(wp == (WPARAM) GetDlgItem( dlg, IDC_MEM_RD))
	{
	    display_listbox_menu = TRUE;
	    current_listbox_menu = IDC_MEM_RD;
	}
	//---------------------------------------------------------------------
	// Write BP
	else if	(wp == (WPARAM) GetDlgItem( dlg, IDC_MEM_WR))
	{
	    display_listbox_menu = TRUE;
	    current_listbox_menu = IDC_MEM_WR;
	}
	//---------------------------------------------------------------------
	else
	{
	    display_listbox_menu = FALSE;	// никуда не попали
	}
	//=====================================================================
	if (display_listbox_menu)
	{
	    int xPos = LOWORD( lp);	// в screen формате
	    int yPos = HIWORD( lp);
	    // printf("old x %d y %d\n",xPos,yPos);
	    POINT click_pos;
	    click_pos.x = xPos;
	    click_pos.y = yPos;
	    
	    // переводим в формат координат listbox-а
	    ScreenToClient( (HWND) wp, &click_pos);
	    //printf("new x %d y %d\n",click_pos.x,click_pos.y);
	    // имитируем клик чтобы переместить гребаный фокус
	    // почему то отправка на на dlg на работает
	    // работает только на hwnd listbox-а
	    SendMessage( (HWND) wp, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG( click_pos.x, click_pos.y));

	//MF_END
	// MF_MENUBARBREAK
	// MF_MENUBREAK
	// MF_STRING | MF_SYSMENU смещает надпись вправо
	
	    //-----------------------------------------------------------------
	    // Выбираем что писать Enable или Disable
	    char brk_disable_enable_text[ 16] = "Disable";
	    bool brk_disable_possible = TRUE;
	    bool brk_edit_possible = TRUE;
	    bool brk_delete_possible = TRUE;
	    bool brk_xxx_all_possible = TRUE;
	    //-----------------------------------------------------------------
	    if (current_listbox_menu != IDC_CBP)
	    {
		HWND hBox = GetDlgItem( dlg, int( current_listbox_menu));	//IDC_PPX
		unsigned cur = unsigned( SendDlgItemMessage( dlg, current_listbox_menu, LB_GETCURSEL, 0, 0));
		unsigned max = unsigned( SendDlgItemMessage( dlg, current_listbox_menu, LB_GETCOUNT,  0, 0));
		//-------------------------------------------------------------
		if (cur >= max)
		{
		    brk_disable_possible = FALSE;
		    brk_edit_possible = FALSE;
		    brk_delete_possible = FALSE;
		    brk_xxx_all_possible = FALSE;
		    goto skip_disable_enable_text_printing;
		    // ДАЛЬШЕ НЕЛЬЗЯ РАБОТАТЬ С НЕВАЛИДНЫМИ ДАННЫМИ !!!
		    // или можно словить access denied с падением !!!
		}
		//-------------------------------------------------------------
		char tmp_strng[ 0x200];
		SendMessage( hBox, LB_GETTEXT, cur, (LPARAM) tmp_strng);
		char *temp_addr = strstr( tmp_strng, "-");
		//-------------------------------------------------------------
		// если строка не найдена то strstr возвращает 0 !!!!
		if ((temp_addr) != 0)
		{
		    *temp_addr = 0;	//типо затираем "-xxxx <--..."
		}
		//-------------------------------------------------------------
		unsigned cursor_addr;
		sscanf( tmp_strng, "%X", &cursor_addr);
		//Z80 &cpu = CpuMgr.Cpu();	// cpu.xxx = текущий CPU
		int mask = 0;
		//-------------------------------------------------------------
		switch (current_listbox_menu)
		{
		    case IDC_BPX:	mask = u8( BP_DISABLE_BPX);	break;
		    case IDC_MEM_RD:	mask = u8( BP_DISABLE_BPR);	break;
		    case IDC_MEM_WR:	mask = u8( BP_DISABLE_BPW);	break;
		}
		//-------------------------------------------------------------
		if (cpu.bp_disable_bits[ cursor_addr] & mask)
		{
		    sprintf( brk_disable_enable_text, "Enable");
		}
		else
		{
		    sprintf( brk_disable_enable_text, "Disable");
		}
		//-------------------------------------------------------------
	    } // not IDC_CBP
	    //-----------------------------------------------------------------
	    // IDC_CBP
	    else
	    {
	    	unsigned cur = unsigned( SendDlgItemMessage( dlg, current_listbox_menu, LB_GETCURSEL, 0, 0));
		unsigned max = unsigned( SendDlgItemMessage( dlg, current_listbox_menu, LB_GETCOUNT,  0, 0));
		//-------------------------------------------------------------
		if (cur >= max)
		{
		    brk_disable_possible = FALSE;
		    brk_edit_possible = FALSE;
		    brk_delete_possible = FALSE;
		    brk_xxx_all_possible = FALSE;
		    goto skip_disable_enable_text_printing;
		}
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
skip_disable_enable_text_printing:
	    //-----------------------------------------------------------------
	    
	    
	    //-----------------------------------------------------------------
	    HMENU context_menu = CreatePopupMenu();
								
	    int mf_flag;
								//=================================
	    mf_flag = ( (current_listbox_menu != IDC_CBP) &&
		        (brk_disable_possible)	
						)  ?  MF_STRING :
						      MF_STRING | MF_GRAYED;
								  
	    AppendMenu( context_menu, mf_flag, IDM_BRK_DISABLE, brk_disable_enable_text );
	    //								"Disable/Enable"
	    AppendMenu( context_menu, MF_SEPARATOR, 0, nullptr); //================================
	    mf_flag = (brk_edit_possible)  ?  MF_STRING :
					      MF_STRING | MF_GRAYED;
	    AppendMenu( context_menu, mf_flag, IDM_BRK_EDIT,		"Edit"         );
	    //-------------------------------------------------------------------------------------
	    mf_flag = (brk_delete_possible)  ?	MF_STRING :
						MF_STRING | MF_GRAYED;
	    AppendMenu( context_menu, mf_flag, IDM_BRK_DELETE,		"Delete"       );
	    AppendMenu( context_menu, MF_SEPARATOR, 0, nullptr); //================================
	    mf_flag = (	(current_listbox_menu != IDC_CBP) &&
			(brk_xxx_all_possible)	
						)  ?  MF_STRING:
						      MF_STRING | MF_GRAYED;
	    AppendMenu( context_menu, mf_flag, IDM_BRK_ENABLE_ALL,	"Enable All"   );
	    //-------------------------------------------------------------------------------------
	    AppendMenu( context_menu, mf_flag, IDM_BRK_DISABLE_ALL,	"Disable All"  );
	    AppendMenu( context_menu, MF_SEPARATOR, 0, nullptr); //================================
	    AppendMenu( context_menu, mf_flag, IDM_BRK_DELETE_ALL,	"Delete All"   );
								//=================================
	    //-----------------------------------------------------------------
	    // TPM_LAYOUTRTL выравниввниие по правому краю
	    int cmd = TrackPopupMenu(	context_menu,			// handle of shortcut menu
					TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTALIGN | TPM_TOPALIGN, //uFlags
					xPos,				// x
					yPos,				// y
					0,				// nReserved
					dlg, 	//	или (HWND) wp	// hWnd
					nullptr		//points to RECT that specifies no-dismissal area
				 );
	    //-----------------------------------------------------------------
	    DestroyMenu( context_menu);
	    //-----------------------------------------------------------------
	    Z80 &cpu = CpuMgr.Cpu();	// cpu.xxx = текущий CPU
	    //-----------------------------------------------------------------
	    switch (cmd)
	    {
		//=============================================================
		// Disable
		case IDM_BRK_DISABLE:
		{
		    //---------------------------------------------------------
		    switch (current_listbox_menu)
		    {
			//case IDC_CBP:		goto;
			case IDC_BPX:		goto disable_bpx;
			case IDC_MEM_RD:	goto disable_bpr;
			case IDC_MEM_WR:	goto disable_bpw;
		    }
		    //---------------------------------------------------------
		    break;
		}
		//=============================================================
		// Edit
		case IDM_BRK_EDIT:
		{
		    //---------------------------------------------------------
		    switch (current_listbox_menu)
		    {
			case IDC_CBP:		goto cbp_edit_label;
			case IDC_BPX:		goto bpx_edit_label;
			case IDC_MEM_RD:	goto bpr_edit_label;
			case IDC_MEM_WR:	goto bpw_edit_label;
		    }
		    //---------------------------------------------------------
		    break;
		}
		//=============================================================
		// Delete
		case IDM_BRK_DELETE:
		{
		    //---------------------------------------------------------
		    switch (current_listbox_menu)
		    {
			case IDC_CBP:		goto del_cond;
			case IDC_BPX:		goto del_bpx;
			case IDC_MEM_RD:	goto del_mem_rd;
			case IDC_MEM_WR:	goto del_mem_wr;
		    }
		    //---------------------------------------------------------
		    break;
		}
		//=============================================================
		// Enable All
		case IDM_BRK_ENABLE_ALL:
		{
		    //---------------------------------------------------------
		    switch (current_listbox_menu)
		    {
			//-----------------------------------------------------
			case IDC_BPX: // Enable All
			{
			    SetFocus( GetDlgItem( dlg, IDC_BPX));
			    //-------------------------------------------------
			    for (unsigned i = 0x0000;    i <= 0xFFFF;    i++)
			    {	
				if (cpu.membits[i] & MEMBITS_BPX)
				{
				    // ставим флаг выключености
				    cpu.bp_disable_bits[ i] &= u8( ~BP_DISABLE_BPX); 
				}
			    }
			    //-------------------------------------------------
			    FillBpxBox(  dlg, 0);	// [NS]
			    // обновить окно дебагера
			    needclr++;
			    goto set_buttons_and_return;
			}
			//-----------------------------------------------------
			case IDC_MEM_RD: // Enable All
			{
			    SetFocus( GetDlgItem( dlg, IDE_MEM_RD));
			    //-------------------------------------------------
			    for (unsigned i = 0x0000;    i <= 0xFFFF;    i++)
			    {
				if (cpu.membits[i] & MEMBITS_BPR)
				{
				    // ставим флаг выключености
				    cpu.bp_disable_bits[ i] &= u8( ~BP_DISABLE_BPR); 
				}
			    }
			    //-------------------------------------------------
			    Fill_Mem_RD_Box(  dlg, 0);	// [NS]
			    // обновить окно дебагера
			    needclr++;
			    goto set_buttons_and_return;
			}
			//-----------------------------------------------------
			case IDC_MEM_WR: // Enable All
			{
			    SetFocus( GetDlgItem( dlg, IDE_MEM_WR));
			    //-------------------------------------------------
			    for (unsigned i = 0x0000;    i <= 0xFFFF;    i++)
			    {
				if (cpu.membits[i] & MEMBITS_BPW)
				{
				    // ставим флаг выключености
				    cpu.bp_disable_bits[ i] &= u8( ~BP_DISABLE_BPW);
				}
			    }
			    //-------------------------------------------------
			    Fill_Mem_WR_Box(  dlg, 0);	// [NS]
			    // обновить окно дебагера
			    needclr++;
			    goto set_buttons_and_return;
			}
			//-----------------------------------------------------	
		    } // switch (current_listbox_menu)
		    //---------------------------------------------------------
		    break;
		} // case IDM_BRK_ENABLE_ALL: 
		//=============================================================	
		
		
		
		//=============================================================
		// Disable All
		case IDM_BRK_DISABLE_ALL:
		{
		    // отключать НЕЛЬЗЯ простым циклом 0-FFFF !!!!!
		    // тк потом будут создаватсо отключенные бряки!!!
		    // 
		    // нужно проверять стоит ли бряк
		    // и если он есть тогда только отключать !!!!
		    //---------------------------------------------------------
		    switch (current_listbox_menu)
		    {
			//-----------------------------------------------------
			case IDC_BPX:
			{
			    SetFocus( GetDlgItem( dlg, IDC_BPX));
			    //-------------------------------------------------
			    for (unsigned i = 0x0000;    i <= 0xFFFF;    i++)
			    {	
				if (cpu.membits[i] & MEMBITS_BPX)
				{
				    // ставим флаг выключености
				    cpu.bp_disable_bits[ i] |= u8( BP_DISABLE_BPX); 
				}
			    }
			    //-------------------------------------------------
			    FillBpxBox(  dlg, 0);	// [NS]
			    // обновить окно дебагера
			    needclr++;
			    goto set_buttons_and_return;
			}
			//-----------------------------------------------------
			case IDC_MEM_RD:
			{
			    SetFocus( GetDlgItem( dlg, IDE_MEM_RD));
			    //-------------------------------------------------
			    for (unsigned i = 0x0000;    i <= 0xFFFF;    i++)
			    {
				if (cpu.membits[i] & MEMBITS_BPR)
				{
				    // ставим флаг выключености
				    cpu.bp_disable_bits[ i] |= u8( BP_DISABLE_BPR); 
				}
			    }
			    //-------------------------------------------------
			    Fill_Mem_RD_Box(  dlg, 0);	// [NS]
			    // обновить окно дебагера
			    needclr++;
			    goto set_buttons_and_return;
			}
			//-----------------------------------------------------
			case IDC_MEM_WR:
			{
			    SetFocus( GetDlgItem( dlg, IDE_MEM_WR));
			    //-------------------------------------------------
			    for (unsigned i = 0x0000;    i <= 0xFFFF;    i++)
			    {
				if (cpu.membits[i] & MEMBITS_BPW)
				{
				    // ставим флаг выключености
				    cpu.bp_disable_bits[ i] |= u8( BP_DISABLE_BPW);
				}
			    }
			    //-------------------------------------------------
			    Fill_Mem_WR_Box(  dlg, 0);	// [NS]
			    // обновить окно дебагера
			    needclr++;
			    goto set_buttons_and_return;
			}
			//-----------------------------------------------------	
		    } // switch (current_listbox_menu)
		    //---------------------------------------------------------
		    break;
		} // case IDM_BRK_DISABLE_ALL: 
		//=============================================================	
		
		
		
		//=============================================================
		// Delete All
		case IDM_BRK_DELETE_ALL:
		{
		    //---------------------------------------------------------
		    switch (current_listbox_menu)
		    {
			//-----------------------------------------------------
			case IDC_BPX:
			{
			    SetFocus( GetDlgItem( dlg, IDC_BPX));
			    //-------------------------------------------------
			    for (unsigned i = 0x0000;    i <= 0xFFFF;    i++)
			    {
				// удаляем бряк
				cpu.membits[ i] &= u8( ~(MEMBITS_BPX));		
				// удаляем флаг выключености
				cpu.bp_disable_bits[ i] &= u8( ~(BP_DISABLE_BPX)); 
			    }
			    //-------------------------------------------------
			    FillBpxBox(  dlg, 0);	// [NS]
			    // обновить окно дебагера
			    needclr++;
			    goto set_buttons_and_return;
			}
			//-----------------------------------------------------
			case IDC_MEM_RD:
			{
			    SetFocus( GetDlgItem( dlg, IDE_MEM_RD));
			    //-------------------------------------------------
			    for (unsigned i = 0x0000;    i <= 0xFFFF;    i++)
			    {
				// удаляем бряк
				cpu.membits[ i] &= u8( ~(MEMBITS_BPR));		
				// удаляем флаг выключености
				cpu.bp_disable_bits[ i] &= u8( ~(BP_DISABLE_BPR)); 
			    }
			    //-------------------------------------------------
			    Fill_Mem_RD_Box(  dlg, 0);	// [NS]
			    // обновить окно дебагера
			    needclr++;
			    goto set_buttons_and_return;
			}
			//-----------------------------------------------------
			case IDC_MEM_WR:
			{
			    SetFocus( GetDlgItem( dlg, IDE_MEM_WR));
			    //-------------------------------------------------
			    for (unsigned i = 0x0000;    i <= 0xFFFF;    i++)
			    {
				// удаляем бряк
				cpu.membits[ i] &= u8( ~(MEMBITS_BPW));		
				// удаляем флаг выключености
				cpu.bp_disable_bits[ i] &= u8( ~(BP_DISABLE_BPW)); 
			    }
			    //-------------------------------------------------
			    Fill_Mem_WR_Box(  dlg, 0);	// [NS]
			    // обновить окно дебагера
			    needclr++;
			    goto set_buttons_and_return;
			}
			//-----------------------------------------------------	
		    } // switch (current_listbox_menu)
		    //---------------------------------------------------------
		    break;
		} // case IDM_BRK_DELETE_ALL: 
		//=============================================================	
		
		
		
		
	    } // switch (cmd)
	    //-----------------------------------------------------------------
	} // (display_listbox_menu)
	//=====================================================================
    } //if (msg == WM_CONTEXTMENU)
    //=========================================================================


    //-------------------------------------------------------------------------
    // if (msg == WM_DESTROY)
    // {
    //     printf("conddlg WM_DESTROY\n");
    // }
    //=========================================================================
    if (msg == WM_CLOSE)	// [NS]
    {
	//printf("conddlg WM_CLOSE\n");
	DestroyWindow( dlg);
	hwnd_bp_dialog = NULL;	// теперь окно можно снова вызвать
    }
    //=========================================================================
    // глушим звук (иначе буфер зацикленно играет по кругу)
    // тк дефолтный виндовый обработчик WM_MOVE
    // не возвращает управления на время перемещения
    if (msg == WM_MOVE)		// [NS]
    {
	OnEnterGui();
    }
    //=========================================================================
    // включаем обратно
    if (msg == WM_EXITSIZEMOVE)	// [NS]
    {
	OnExitGui();
    }
    //=========================================================================
    if (msg == WM_INITDIALOG)
    {
	FillCondBox( dlg, 0);
	FillBpxBox(  dlg, 0);
	Fill_Mem_RD_Box(  dlg, 0);	// [NS]
	Fill_Mem_WR_Box(  dlg, 0);	// [NS]
	//FillMemBox(  dlg, 0);		// Deprecated
	conddlg_update_texts( dlg);	// [NS]

	// [NS]								
	CheckDlgButton( dlg, IDC_BPX_GROUP,    group_bpx_list  ?  BST_CHECKED :
								  BST_UNCHECKED);
	CheckDlgButton( dlg, IDC_MEM_RD_GROUP, group_bpr_list  ?  BST_CHECKED :
								  BST_UNCHECKED);
	CheckDlgButton( dlg, IDC_MEM_WR_GROUP, group_bpw_list  ?  BST_CHECKED :
								  BST_UNCHECKED);
	
	SetFocus( GetDlgItem( dlg, IDE_CBP));

set_buttons_and_return:		// вызываетсо отовсюду
	SetBpxButtons( dlg);
	
	// возвращаем CPU на место перед выходом
	CpuMgr.SetCurrentCpu( current_cpu_back);
	
	return 1;
    }
    //=========================================================================
    if ((msg == WM_SYSCOMMAND) && ((wp & 0xFFF0) == SC_CLOSE))
    {
	//printf("conddlg SC_CLOSE \n");
	EndDialog( dlg, 0);
    }
    //=========================================================================
    if (msg != WM_COMMAND)
    {
	// возвращаем CPU на место перед выходом
	CpuMgr.SetCurrentCpu( current_cpu_back);
	
	return 0;
    }
    //=========================================================================
    
    
    //=========================================================================

    //-------------------------------------------------------------------------
    // нет никакого OK и Cancel у этого окна [NS]
    // но эта хуита срабатывает если при входе нажать enter
    //  if (id == IDCANCEL || id == IDOK)
    //  {
    //	   printf("conddlg not existed IDCANCEL IDOK \n");
    //	   EndDialog( dlg, 0);
    //  }
    //    
    //-------------------------------------------------------------------------
    char tmp[ 0x200];
    
    
    //=========================================================================
    if (   (	((id == IDE_BPX) || (id == IDE_CBP) || (id == IDE_MEM_RD) || (id == IDE_MEM_WR))	&&
		((code == EN_SETFOCUS) || (code == EN_CHANGE))
	   )
	   ||
	   (	((id == IDC_BPX) || (id == IDC_CBP) || (id == IDC_MEM_RD) || (id == IDC_MEM_WR))	&&
		(code == LBN_SETFOCUS)
	   )
      )
    {
	goto set_buttons_and_return;
    }
    //=========================================================================
    /*
    // больше нету
    if ((id == IDC_MEM_R) || (id == IDC_MEM_W))
	goto set_buttons_and_return;
    */
    //=========================================================================
    
    
       					// [NZ]
    
   
   
    
    //=========================================================================
    if (id == IDC_BPX_GROUP)
    {
	group_bpx_list = FALSE;	
	//---------------------------------------------------------------------
	if (IsDlgButtonChecked( dlg, IDC_BPX_GROUP) == BST_CHECKED)
	{
	    group_bpx_list = TRUE;
	}
	//---------------------------------------------------------------------
	FillBpxBox( dlg, 0);
	goto set_buttons_and_return;
    }
    //-------------------------------------------------------------------------
    if (id == IDC_MEM_RD_GROUP)
    {
	group_bpr_list = FALSE;
	//---------------------------------------------------------------------
	if (IsDlgButtonChecked( dlg, IDC_MEM_RD_GROUP) == BST_CHECKED)
	{
	    group_bpr_list = TRUE;
	}
	//---------------------------------------------------------------------
	Fill_Mem_RD_Box( dlg, 0);
	goto set_buttons_and_return;
    }
    //-------------------------------------------------------------------------
    if (id == IDC_MEM_WR_GROUP)
    {
	group_bpw_list = FALSE;
	//---------------------------------------------------------------------
	if (IsDlgButtonChecked( dlg, IDC_MEM_WR_GROUP) == BST_CHECKED)
	{
	    group_bpw_list = TRUE;
	}
	//---------------------------------------------------------------------
	Fill_Mem_WR_Box( dlg, 0);
	goto set_buttons_and_return;
    }
    //=========================================================================
    
    		
		
		
    //=========================================================================
    if (id == IDB_CBP_EDIT)						// [NS]
    {
cbp_edit_label:
	if (MoveBpxFromBoxToEdit( dlg, IDC_CBP, IDE_CBP))
	{
	    goto del_cond;
	}
    }
    //-------------------------------------------------------------------------
    if (id == IDB_BPX_EDIT)						// [NS]
    {
bpx_edit_label:
	if (MoveBpxFromBoxToEdit( dlg, IDC_BPX, IDE_BPX))
	{
	    goto del_bpx;
	}
    }
    //-------------------------------------------------------------------------
    if (id == IDB_MEM_RD_EDIT)						// [NS]
    {
bpr_edit_label:
	if (MoveBpxFromBoxToEdit( dlg, IDC_MEM_RD, IDE_MEM_RD))
	{
	    goto del_mem_rd;
	}
    }
    //-------------------------------------------------------------------------
    if (id == IDB_MEM_WR_EDIT)						// [NS]
    {
bpw_edit_label:
	if (MoveBpxFromBoxToEdit( dlg, IDC_MEM_WR, IDE_MEM_WR))
	{
	    goto del_mem_wr;
	}
    }
    //=========================================================================
    
    
    
    
    
    
    //=========================================================================
    // отключение бряка
    if (code == LBN_DBLCLK)
    {
	//if (id == IDC_CBP)
	//{
	//	;
	//}
	//---------------------------------------------------------------------
	// дабл клик в execute break listbox-е
	if (id == IDC_BPX)
	{
	    //printf("LBN_DBLCLK IDC_BPX\n");
disable_bpx:
	    SetFocus( GetDlgItem( dlg, IDE_BPX));
	    id = IDC_BPX;
	    disable_bp_mask = u8( BP_DISABLE_BPX);
disable_range:
	    unsigned cur = unsigned( SendDlgItemMessage( dlg, id, LB_GETCURSEL, 0, 0));
	    unsigned max = unsigned( SendDlgItemMessage( dlg, id, LB_GETCOUNT,  0, 0));
	    //-----------------------------------------------------------------
	    if (cur >= max)
	    {
		// возвращаем CPU на место перед выходом
		CpuMgr.SetCurrentCpu( current_cpu_back);
	
		return 0;
	    }
	    //-----------------------------------------------------------------
	    SendDlgItemMessage( dlg, id, LB_GETTEXT, cur, (LPARAM) tmp);
	    unsigned start, end;
	    sscanf( tmp, "%X", &start);
	    //-----------------------------------------------------------------
	    if (tmp[ 4] == '-')
	    {
		sscanf( tmp + 5, "%X", &end);
	    }
	    //-----------------------------------------------------------------
	    else
	    {
		end = start;
	    }
	    //-----------------------------------------------------------------
	    Z80 &cpu = CpuMgr.Cpu();
	    //-----------------------------------------------------------------
	    for (unsigned i = start;    i <= end;    i++)
		cpu.bp_disable_bits[ i] ^= disable_bp_mask;
	    //---------------------------------------------------------------------
	  
	  
		
		// НЕ ТОЛЬКО ДЛЯ (id == IDC_BPX) !!!!
		// ниже сюда goto
		//---------------------------------------------------------------------
		// Exe BP
		if (id == IDC_BPX)
		{
		    FillBpxBox( dlg, 0);
		}
		//---------------------------------------------------------------------
		// Read BP
		else if (id == IDC_MEM_RD)
		{
		    Fill_Mem_RD_Box( dlg, 0);
		}
		//---------------------------------------------------------------------
		// Write BP
		else if (id == IDC_MEM_WR)
		{
		    Fill_Mem_WR_Box( dlg, 0);
		}
		//---------------------------------------------------------------------
		
		
		/*
		// DEBUG !!!!!!!!!!!!1
		FillBpxBox( dlg, 0);
		Fill_Mem_RD_Box( dlg, 0);
		Fill_Mem_WR_Box( dlg, 0);
		*/
		
	    //---------------------------------------------------------------------
	    if (cur && (cur == max))
		cur--;
	    //---------------------------------------------------------------------
	    SendDlgItemMessage( dlg, id, LB_SETCURSEL, cur, 0);
	    cpu.dbgchk = isbrk( cpu);
	    goto set_buttons_and_return;
	}
	//---------------------------------------------------------------------
	// дабл клик в memory read break listbox-е
	if (id == IDC_MEM_RD)
	{
disable_bpr:
	    //printf("LBN_DBLCLK IDC_MEM_RD\n");
	    SetFocus( GetDlgItem( dlg, IDE_MEM_RD));
	    id = IDC_MEM_RD;
	    disable_bp_mask = u8( BP_DISABLE_BPR);
	    goto disable_range;
	}
	//---------------------------------------------------------------------
	// дабл клик в memory write listbox-е
	if (id == IDC_MEM_WR)
	{
disable_bpw:
	    //printf("LBN_DBLCLK IDC_MEM_WR\n");
	    SetFocus( GetDlgItem( dlg, IDE_MEM_WR));
	    id = IDC_MEM_WR;
	    disable_bp_mask = u8( BP_DISABLE_BPW);
	    goto disable_range;
	}
	//---------------------------------------------------------------------
    }
    //=========================================================================
    //
    if (id == IDB_SWITCH_CPU)
    {
	// бекапим текущий CPU
	///current_cpu_back = CpuMgr.GetCurrentCpu();	
	conddlg_current_visible_cpu ^= 1; 		
	// меняем текущий CPU
	CpuMgr.SetCurrentCpu( conddlg_current_visible_cpu);	

	//Z80 &cpu = CpuMgr.Cpu();
	
	/*
	Z80 &cpu0 = CpuMgr.Cpu();
	cpu0.dbgbreak = 0;
	CpuMgr.SwitchCpu();
	Z80 &cpu1 = CpuMgr.Cpu();
	*/
	
	FillCondBox( dlg, 0);
	FillBpxBox(  dlg, 0);
	Fill_Mem_RD_Box(  dlg, 0);	// [NS]
	Fill_Mem_WR_Box(  dlg, 0);	// [NS]
	
	
	conddlg_update_texts( dlg);
	
	// возвращаем CPU на место
	//CpuMgr.SetCurrentCpu( current_cpu_back);	

	//needclr++;	// нужно обновить окно дебагера
			// теперь не нужно тк не синхронно
	goto set_buttons_and_return;
    }
    //-------------------------------------------------------------------------






    /*
    // редактирование по двойному клику
    if (code == LBN_DBLCLK)
    {
	if ((id == IDC_CBP) && MoveBpxFromBoxToEdit( dlg, IDC_CBP, IDE_CBP))	goto del_cond;
	if ((id == IDC_BPX) && MoveBpxFromBoxToEdit( dlg, IDC_BPX, IDE_BPX))	goto del_bpx;
	if ((id == IDC_MEM) && MoveBpxFromBoxToEdit( dlg, IDC_MEM, IDE_MEM))	goto del_mem;
    }
    */
    //-------------------------------------------------------------------------
    if (id == IDB_CBP_ADD)
    {
	SendDlgItemMessage( dlg, IDE_CBP, WM_GETTEXT, sizeof tmp, (LPARAM) tmp);
	SetFocus( GetDlgItem( dlg, IDE_CBP));
	Z80 &cpu = CpuMgr.Cpu();
	//---------------------------------------------------------------------
	if (!toscript( tmp, cpu.cbp[ cpu.cbpn]))
	{
	    MessageBox(	dlg,
			"Error in expression\nPlease do RTFM",
			nullptr,
			MB_ICONERROR
		     );
	    // возвращаем CPU на место перед выходом
	    CpuMgr.SetCurrentCpu( current_cpu_back);
	    
	    return 1;
	}
	//---------------------------------------------------------------------
	SendDlgItemMessage( dlg, IDE_CBP, WM_SETTEXT, 0, 0);
	FillCondBox( dlg, cpu.cbpn++);
	cpu.dbgchk = isbrk( cpu);
	goto set_buttons_and_return;
    }
    //-------------------------------------------------------------------------
    if (id == IDB_BPX_ADD)
    {
	SendDlgItemMessage( dlg, IDE_BPX, WM_GETTEXT, sizeof tmp, (LPARAM) tmp);
	SetFocus( GetDlgItem( dlg, IDE_BPX));
	MEM_RANGE range;
	//---------------------------------------------------------------------
	if (!GetMemRamge( tmp, range))
	{
	    MessageBox(	dlg,
			"Invalid breakpoint address / range",
			nullptr,
			MB_ICONERROR
		     );
		     
	    // возвращаем CPU на место перед выходом
	    CpuMgr.SetCurrentCpu( current_cpu_back);
	    
	    return 1;
	}
	//---------------------------------------------------------------------
	Z80 &cpu = CpuMgr.Cpu();
	//---------------------------------------------------------------------
	for (unsigned i = range.start;    i <= range.end;    i++)
	{
	    cpu.membits[i] |= MEMBITS_BPX;
	}
	//---------------------------------------------------------------------
	SendDlgItemMessage( dlg, IDE_BPX, WM_SETTEXT, 0, 0);
	FillBpxBox( dlg, range.start);
	cpu.dbgchk = isbrk( cpu);
	goto set_buttons_and_return;
    }
    //-------------------------------------------------------------------------
    
    
    //-------------------------------------------------------------------------
    if (id == IDB_MEM_RD_ADD)	// [NS]
    {
	SendDlgItemMessage( dlg, IDE_MEM_RD, WM_GETTEXT, sizeof tmp, (LPARAM) tmp);
	SetFocus( GetDlgItem( dlg, IDE_MEM_RD));
	MEM_RANGE range;
	//---------------------------------------------------------------------
	if (!GetMemRamge( tmp, range))
	{
	    MessageBox(	dlg,
			"Invalid breakpoint address / range",
			nullptr,
			MB_ICONERROR
		     );
		     
	    // возвращаем CPU на место перед выходом
	    CpuMgr.SetCurrentCpu( current_cpu_back);
	    
	    return 1;
	}
	//---------------------------------------------------------------------
	Z80 &cpu = CpuMgr.Cpu();
	//---------------------------------------------------------------------
	for (unsigned i = range.start;    i <= range.end;    i++)
	{
	    cpu.membits[i] |= MEMBITS_BPR;
	}
	//---------------------------------------------------------------------
	SendDlgItemMessage( dlg, IDE_MEM_RD, WM_SETTEXT, 0, 0);
	Fill_Mem_RD_Box( dlg, range.start);
	cpu.dbgchk = isbrk( cpu);
	goto set_buttons_and_return;
    }
    //-------------------------------------------------------------------------
    
    

    //-------------------------------------------------------------------------
    if (id == IDB_MEM_WR_ADD)
    {
	SendDlgItemMessage( dlg, IDE_MEM_WR, WM_GETTEXT, sizeof tmp, (LPARAM) tmp);
	SetFocus( GetDlgItem( dlg, IDE_MEM_WR));
	MEM_RANGE range;
	//---------------------------------------------------------------------
	if (!GetMemRamge( tmp, range))
	{
	    MessageBox(	dlg,
			"Invalid breakpoint address / range",
			nullptr,
			MB_ICONERROR
		     );
		     
	    // возвращаем CPU на место перед выходом
	    CpuMgr.SetCurrentCpu( current_cpu_back);
	    
	    return 1;
	}
	//---------------------------------------------------------------------
	Z80 &cpu = CpuMgr.Cpu();
	//---------------------------------------------------------------------
	for (unsigned i = range.start;    i <= range.end;    i++)
	{
	    cpu.membits[i] |= MEMBITS_BPW;
	}
	//---------------------------------------------------------------------
	SendDlgItemMessage( dlg, IDE_MEM_WR, WM_SETTEXT, 0, 0);
	Fill_Mem_WR_Box( dlg, range.start);
	cpu.dbgchk = isbrk( cpu);
	goto set_buttons_and_return;
    }
    //-------------------------------------------------------------------------
    
    
    
    //=========================================================================
    if (id == IDB_CBP_DEL)
    {
del_cond:
	SetFocus( GetDlgItem( dlg, IDE_CBP));
	unsigned cur = unsigned( SendDlgItemMessage( dlg, IDC_CBP, LB_GETCURSEL, 0, 0));
	Z80 &cpu = CpuMgr.Cpu();
	//---------------------------------------------------------------------
	if (cur >= cpu.cbpn)
	{
	    // возвращаем CPU на место перед выходом
	    CpuMgr.SetCurrentCpu( current_cpu_back);
	    
	    return 0;
	}
	//---------------------------------------------------------------------
	cpu.cbpn--;
	memcpy(	cpu.cbp[ cur],
		cpu.cbp[ cur + 1],
		sizeof( cpu.cbp[ 0]) * (cpu.cbpn - cur)
	    );
	//---------------------------------------------------------------------
	if (cur && cur == cpu.cbpn)
	    cur--;
	//-------------------------------------------------------------------------
	FillCondBox( dlg, cur);
	cpu.dbgchk = isbrk( cpu);
	goto set_buttons_and_return;
    }
    //-------------------------------------------------------------------------
    
    
    //-------------------------------------------------------------------------
    // Удаление BPX (и за одноо BPW, BPR)
    if (id == IDB_BPX_DEL)
    {
del_bpx:
	SetFocus( GetDlgItem( dlg, IDE_BPX));
	id = IDC_BPX;
	mask = u8( ~MEMBITS_BPX);
	disable_bp_mask = u8( ~BP_DISABLE_BPX);
del_range:
	unsigned cur = unsigned( SendDlgItemMessage( dlg, id, LB_GETCURSEL, 0, 0));
	//printf("cur %d\n",cur);
	unsigned max = unsigned( SendDlgItemMessage( dlg, id, LB_GETCOUNT,  0, 0));
	//printf("max %d\n",max);
	//---------------------------------------------------------------------
	if (cur >= max)
	{
	    // возвращаем CPU на место перед выходом
	    CpuMgr.SetCurrentCpu( current_cpu_back);
	    
	    return 0;
	}
	//---------------------------------------------------------------------
	SendDlgItemMessage( dlg, id, LB_GETTEXT, cur, (LPARAM) tmp);
	unsigned start, end;
	sscanf( tmp, "%X", &start);
	//---------------------------------------------------------------------
	if (tmp[ 4] == '-')
	{
	    sscanf( tmp + 5, "%X", &end);
	}
	//---------------------------------------------------------------------
	else
	{
	    end = start;
	}
	//---------------------------------------------------------------------
	Z80 &cpu = CpuMgr.Cpu();
	//---------------------------------------------------------------------
	for (unsigned i = start;    i <= end;    i++)
	{
	    cpu.membits[ i] &= mask;			// удаляем бряк
	    cpu.bp_disable_bits[ i] &= disable_bp_mask;	// удаляем флаг выключености
	}
	//---------------------------------------------------------------------
	
	    
	    // не только для (id == IDB_BPX_DEL)
	    // ниже goto
	    //-----------------------------------------------------------------
	    if (id == IDC_BPX)
	    {
		FillBpxBox( dlg, 0);
	    }
	    //-----------------------------------------------------------------
	    else if (id == IDC_MEM_RD)					// [NS]
	    {
		Fill_Mem_RD_Box( dlg, 0);
	    }
	    //-----------------------------------------------------------------
	    else if (id == IDC_MEM_WR)					// [NS]
	    {
		Fill_Mem_WR_Box( dlg, 0);
	    }
	    //-----------------------------------------------------------------
	    
	    
	    /*
	    // DEBUG !!!!!!!!!!!!!!!!!!
	    FillBpxBox( dlg, 0);
	    Fill_Mem_RD_Box( dlg, 0);
	    Fill_Mem_WR_Box( dlg, 0);
	    */
	    
	//---------------------------------------------------------------------
	if (cur && (cur == max))
	    cur--;
	//---------------------------------------------------------------------
	SendDlgItemMessage( dlg, id, LB_SETCURSEL, cur, 0);
	cpu.dbgchk = isbrk( cpu);
	goto set_buttons_and_return;
    }
    //-------------------------------------------------------------------------
    if (id == IDB_MEM_RD_DEL)
    {
del_mem_rd:
	SetFocus( GetDlgItem( dlg, IDE_MEM_RD));
	id = IDC_MEM_RD;
	mask = u8( ~(MEMBITS_BPR));
	disable_bp_mask = u8( ~BP_DISABLE_BPR);
	goto del_range;
    }
    //-------------------------------------------------------------------------
    if (id == IDB_MEM_WR_DEL)
    {
del_mem_wr:
	SetFocus( GetDlgItem( dlg, IDE_MEM_WR));
	id = IDC_MEM_WR;
	mask = u8( ~(MEMBITS_BPW));
	disable_bp_mask = u8( ~BP_DISABLE_BPW);
	goto del_range;
    }

    // возвращаем CPU на место перед выходом
    CpuMgr.SetCurrentCpu( current_cpu_back);
    
    return 0;
}
//=============================================================================
// создание диалогового окна бряков
void mon_bpdialog()
{
    // DialogBox( hIn, MAKEINTRESOURCE( IDD_COND), wnd, conddlg);	// orig

    //-------------------------------------------------------------------------
    // создаем окно							// [NS]
    // только если оно еще не создано
    // иначе у нас обрабатываютсо сообщения только от одного окна
    // и нужен массив hwnd-овов
    if (hwnd_bp_dialog == NULL)
    {
	hwnd_bp_dialog = CreateDialog( hIn, MAKEINTRESOURCE( IDD_COND), wnd, conddlg);	// [NS]
    }
    //-------------------------------------------------------------------------
    // щас окно поверх всех унриальных окон почему то
    // а так тут нужно переводить фокус на окно
    // которрое могло потерятсо
    /*
    else
    {
        ShowWindow( hwnd_bp_dialog, SW_SHOWNORMAL);	//SW_SHOWMINNOACTIVE 
    }*/
}
//=============================================================================
































//=============================================================================
// обработка сообщений окна вачесов
static INT_PTR CALLBACK watchdlg( HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
    (void) lp;

    char tmp[ 0x200];
    unsigned i;
    //-------------------------------------------------------------------------
    static const int ids1[] =
    {
	IDC_W1_ON,
	IDC_W2_ON,
	IDC_W3_ON,
	IDC_W4_ON
    };
    //-------------------------------------------------------------------------
    static const int ids2[] =
    {
	IDE_W1,
	IDE_W2,
	IDE_W3,
	IDE_W4
    };
    //-------------------------------------------------------------------------
    if (msg == WM_CLOSE)	// [NS]
    {
	DestroyWindow( dlg);
	hwnd_watchdlg = NULL;	// теперь окно можно снова вызвать
    }
    //-------------------------------------------------------------------------
    if (msg == WM_INITDIALOG)
    {
	//---------------------------------------------------------------------
	for (i = 0;    i < 4;    i++)
	{
	    CheckDlgButton( dlg, ids1[i], watch_enabled[i]  ?	BST_CHECKED :
								BST_UNCHECKED);
	    script2text( tmp, watch_script[i]);
	    SetWindowText( GetDlgItem( dlg, ids2[i]), tmp);
	}
	//---------------------------------------------------------------------
	CheckDlgButton( dlg, IDC_TR_RAM,    trace_ram  ?  BST_CHECKED :
							  BST_UNCHECKED);
	//---------------------------------------------------------------------
	CheckDlgButton( dlg, IDC_TR_ROM,    trace_rom  ?  BST_CHECKED :	// r0146 fixed было trace_ram [NS]
							  BST_UNCHECKED);
	//---------------------------------------------------------------------
	CheckDlgButton( dlg, IDC_LED_DEBUG, (conf.led.osw & 0x80000000)  ?  BST_CHECKED :	// [NS]
									    BST_UNCHECKED);
	//---------------------------------------------------------------------
reinit:
	//---------------------------------------------------------------------
	for (i = 0;    i < 4;    i++)
	    EnableWindow( GetDlgItem( dlg, ids2[ i]), watch_enabled[ i]);
	//---------------------------------------------------------------------
	return 1;
    }
    //-------------------------------------------------------------------------
    // Клацание птичек [Trace RAM banks] и [Trace ROM banks]
    if ((msg == WM_COMMAND)  &&	 (	(LOWORD( wp) == IDC_TR_RAM)	||
					(LOWORD( wp) == IDC_TR_ROM) 
				)
     )
    {
	trace_ram = IsDlgButtonChecked( dlg, IDC_TR_RAM) == BST_CHECKED;
	trace_rom = IsDlgButtonChecked( dlg, IDC_TR_ROM) == BST_CHECKED;
    }
    //-------------------------------------------------------------------------
    // Клацание птички включения вачесов [NS]
    if ((msg == WM_COMMAND)  &&	 (	(LOWORD( wp) == IDC_LED_DEBUG) )
     )
    {
	conf.led.osw =  (conf.led.osw & 0x7FFFFFFF)
			|
		((IsDlgButtonChecked( dlg, IDC_LED_DEBUG) == BST_CHECKED)  ?  0x80000000 :
									      0  );
	apply_video();	// без него не включаетсо ? [NS]
    }
    //-------------------------------------------------------------------------    
    // тк как отследить enter в поле редактирования мы не знаем
    // делаем кнопочку применить
    if ((msg == WM_COMMAND)  &&	 (	(LOWORD( wp) == ID_APPLY) )
     )
    {
	//---------------------------------------------------------------------
	for (i = 0;    i < 4;    i++)
	{
	    if (watch_enabled[ i]) 
	    {
		SendDlgItemMessage( dlg, ids2[i], WM_GETTEXT, sizeof tmp, (LPARAM)tmp);
		//-------------------------------------------------------------
		if (!toscript( tmp, watch_script[i]))
		{
		    sprintf( tmp, "Watch %u: error in expression\nPlease do RTFM", i+1);
		    MessageBox( dlg, tmp, nullptr, MB_ICONERROR); 
		    watch_enabled[ i] = 0;
		    SetFocus( GetDlgItem( dlg, ids2[i]));
		    return 0;
		}
		//-------------------------------------------------------------
	    }
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    
    // видимо обработка 4-х кнопок вклюючения вачеса
    if ((msg == WM_COMMAND)  &&	 (	(LOWORD( wp) == ids1[0]) ||
					(LOWORD( wp) == ids1[1]) || 
					(LOWORD( wp) == ids1[2]) || 
					(LOWORD( wp) == ids1[3])
				  )
     )
    {
	//---------------------------------------------------------------------
	for (i = 0;    i < 4;    i++)
	    watch_enabled[i] = IsDlgButtonChecked( dlg, ids1[ i]) == BST_CHECKED;
	//---------------------------------------------------------------------
	goto reinit;
    }    
    //-------------------------------------------------------------------------
    if (	((msg == WM_SYSCOMMAND) && ((wp & 0xFFF0) == SC_CLOSE))	||
		((msg == WM_COMMAND) && (LOWORD( wp) == IDCANCEL))
     )
    {
	trace_ram = IsDlgButtonChecked( dlg, IDC_TR_RAM) == BST_CHECKED;
	trace_rom = IsDlgButtonChecked( dlg, IDC_TR_ROM) == BST_CHECKED;
	
	// *(&conf.led.ay + 5) = ((*(&conf.led.ay + 5)) & 0x7FFFFFFF)
	conf.led.osw =  (conf.led.osw & 0x7FFFFFFF)
			|
		((IsDlgButtonChecked( dlg, IDC_LED_DEBUG) == BST_CHECKED)  ?  0x80000000 :
									      0  );
	apply_video();	// без него не включаетсо ? [NS]
	//---------------------------------------------------------------------
	for (i = 0;    i < 4;    i++)
	{
	    if (watch_enabled[ i]) 
	    {
		SendDlgItemMessage( dlg, ids2[i], WM_GETTEXT, sizeof tmp, (LPARAM)tmp);
		//-------------------------------------------------------------
		if (!toscript( tmp, watch_script[i]))
		{
		    sprintf( tmp, "Watch %u: error in expression\nPlease do RTFM", i+1);
		    MessageBox( dlg, tmp, nullptr, MB_ICONERROR); 
		    watch_enabled[ i] = 0;
		    SetFocus( GetDlgItem( dlg, ids2[i]));
		    return 0;
		}
		//-------------------------------------------------------------
	    }
	}
	//---------------------------------------------------------------------
	EndDialog( dlg, 0);
    }
    //-------------------------------------------------------------------------
    return 0;
}
//=============================================================================
void mon_watchdialog()
{
    //DialogBox( hIn, MAKEINTRESOURCE( IDD_OSW), wnd, watchdlg);
    //-------------------------------------------------------------------------
    // создаем окно							// [NS]
    // только если оно еще не создано
    // иначе у нас обрабатываютсо сообщения только от одного окна
    // и нужен массив hwnd-овов
    if (!hwnd_watchdlg)
    {
	hwnd_watchdlg = CreateDialog( hIn, MAKEINTRESOURCE( IDD_OSW), wnd, watchdlg);	// [NS]
    }
    //-------------------------------------------------------------------------
}
//=============================================================================






//=============================================================================
static void LoadBpx()
{
    char Line[ 100];
    char BpxFileName[ FILENAME_MAX];

    addpath( BpxFileName, "bpx.ini");

    FILE *BpxFile = fopen( BpxFileName, "rt");
    //-------------------------------------------------------------------------
    if (!BpxFile)
        return;
    //-------------------------------------------------------------------------
    while (!feof( BpxFile))
    {
	fgets( Line, sizeof( Line), BpxFile);
	Line[ sizeof( Line) - 1] = 0;
	char Type = -1;
	int Start = -1;
	int End = -1;
	int CpuIdx = -1;
	int n = sscanf( Line, "%c%1d=%i-%i", &Type, &CpuIdx, &Start, &End);
	//---------------------------------------------------------------------
	if (n < 3 || CpuIdx < 0 || CpuIdx >= (int) CpuMgr.GetCount() || Start < 0)
	    continue;
	//---------------------------------------------------------------------
	if (End < 0)
	    End = Start;
	//---------------------------------------------------------------------
	unsigned mask = 0;
	//---------------------------------------------------------------------
	switch (Type)
	{
	    case 'r':	mask |= MEMBITS_BPR;	break;
	    case 'w':	mask |= MEMBITS_BPW;	break;
	    case 'x':	mask |= MEMBITS_BPX;	break;
	    default:	continue;
	}
	//---------------------------------------------------------------------
	Z80 &cpu = CpuMgr.Cpu( u32( CpuIdx));
	//---------------------------------------------------------------------
	for (unsigned i = unsigned( Start);    i <= unsigned( End);    i++)
            cpu.membits[ i] |= mask;
	//---------------------------------------------------------------------
	cpu.dbgchk = isbrk( cpu);
    }
    fclose( BpxFile);
}
//=============================================================================




//=============================================================================
static void SaveBpx()
{
    char BpxFileName[ FILENAME_MAX];

    addpath( BpxFileName, "bpx.ini");

    FILE *BpxFile = fopen( BpxFileName, "wt");
    //-------------------------------------------------------------------------
    if (!BpxFile)
        return;
    //-------------------------------------------------------------------------
    for (unsigned CpuIdx = 0;    CpuIdx < CpuMgr.GetCount();    CpuIdx++)
    {
	Z80 &cpu = CpuMgr.Cpu( CpuIdx);
	//---------------------------------------------------------------------
	for (int i = 0;    i < 3;    i++)
	{
	    //-----------------------------------------------------------------
	    for (unsigned Start = 0;    Start < 0x10000;    )
            {
		static const unsigned Mask[] =
		{
		    MEMBITS_BPR,
		    MEMBITS_BPW,
		    MEMBITS_BPX
		};
		//-------------------------------------------------------------
		if (!(cpu.membits[ Start] & Mask[ i]))
		{
		    Start++;
		    continue;
		}
		//-------------------------------------------------------------
		unsigned active = cpu.membits[ Start];
		unsigned End;
		//-------------------------------------------------------------
		for (End = Start;    End < 0xFFFF && !((active ^ cpu.membits[ End + 1]) & Mask[i]);    End++);
		//-------------------------------------------------------------
		static const char Type[] = { 'r', 'w', 'x' };
		//-------------------------------------------------------------
		if (active & Mask[i])
		{
		    //---------------------------------------------------------
		    if (Start == End)
		    {
			fprintf( BpxFile, "%c%1u=0x%04X\n", Type[ i], CpuIdx, Start);
		    }
		    //---------------------------------------------------------
		    else
		    {
			fprintf( BpxFile, "%c%1u=0x%04X-0x%04X\n", Type[ i], CpuIdx, Start, End);
		    }
		    //---------------------------------------------------------
		}
		//-------------------------------------------------------------
		Start = End + 1;
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    fclose( BpxFile);
}
//=============================================================================




//=============================================================================
void init_bpx()
{
    LoadBpx();
}
//=============================================================================


//=============================================================================
void done_bpx()
{
    SaveBpx();
}
//=============================================================================


