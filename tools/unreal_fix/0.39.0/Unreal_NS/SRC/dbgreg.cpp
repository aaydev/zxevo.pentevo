#include "std.h"

#include "emul.h"
#include "vars.h"
#include "debug.h"
#include "dbgpaint.h"
#include "dbgreg.h"




//#include "dbgtrace.h"	//только для теста !!!! удалть



//=============================================================================

//struct TRegLayout
//{
//   size_t offs;
//   unsigned char width;
//   unsigned char x,y;
//   unsigned char lf,rt,up,dn;
//};

//в dbgreg.h
/*
#define		HEX_1		0x01
#define		HEX_2		0x02

#define		HEX_8		0x08
#define		HEX_16		0x10

#define		HEX_IM2		0x12
#define		DEC_6T		0x76

#define		BIT_0		0xB0
#define		BIT_1		0xB1
#define		BIT_2		0xB2
#define		BIT_3		0xB3
#define		BIT_4		0xB4
#define		BIT_5		0xB5
#define		BIT_6		0xB6
#define		BIT_7		0xB7
*/


//=============================================================================
// REG WINDOW DESIGN	[NS]
//
// POS_A POS_F	POS_A1 POS_F1	POS_SP						POS_I		POS_R
// XX    XX	XX     XX	XX XX						XX		XX
// POS_BC	POS_BC1		POS_PC	-->	-->	-->	-->	-->	-->	-->	POS_T
// XX    XX	XX     XX	XX XX								X
// POS_DE	POS_DE1		POS_IX	-->	-->	POS_IM	<--	-->	-->	POS_IF1 POS_IF2
// XX    XX	XX     XX	XX XX			X				X	X
// POS_HL	POS_HL1		POS_IY	POS_FSF POS_FZF POS_FF5 POS_FHF POS_FF3 POS_FPV POS_FNF POS_FCF
// XX    XX	XX     XX	XX XX	X	X	X	X	X	X	X	X
//
//=============================================================================

//		название	порядковый номер в regs_layout[] !!! (ниже)
//		позиции		
#define		POS_A		0
#define		POS_F		1

#define		POS_BC		2
#define		POS_DE		3
#define		POS_HL		4

//#define	POS_AF1		5	//NS
#define		POS_A1		5
	
#define		POS_BC1		6
#define		POS_DE1		7
#define		POS_HL1		8

#define		POS_SP		9
#define		POS_PC		10
	
#define		POS_IX		11
#define		POS_IY		12
	
#define		POS_I		13
#define		POS_R		14
	
#define		POS_IM		15
#define		POS_IF1		16	//IFF1
#define		POS_IF2		17	//IFF2

#define		POS_FSF		18
#define		POS_FZF		19
#define		POS_FF5		20
#define		POS_FHF		21
#define		POS_FF3		22
#define		POS_FPV		23
#define		POS_FNF		24
#define		POS_FCF		25

#define		POS_T		26	//NS	
#define		POS_F1		27	//NS

// regs_layout[]
// с целью обратной совместимости с возможно незамеченым гофнокодом
// добавление новых значений идет в конец

const TRegLayout regs_layout[] =
{	//							порядковый номер соседних позиций для переходов
	//		,	,  разрядность,	X,	Y,	left,	right,	up,	down
	//			   (так же для									
	//			   сдвигов!!!
	//			   в оригинале)																
    { offsetof(TZ80State, a)	,	HEX_8,	3,	0,	POS_A,	POS_F,	POS_A,	POS_BC	}, //  0 a
    { offsetof(TZ80State, f)	,	HEX_8,	5,	0,	POS_A,	POS_A1,	POS_F,	POS_BC	}, //  1 f
    { offsetof(TZ80State, bc)	,	HEX_16,	3,	1,	POS_BC,	POS_BC1,POS_A,	POS_DE	}, //  2 bc
    { offsetof(TZ80State, de)	,	HEX_16,	3,	2,	POS_DE,	POS_DE1,POS_BC,	POS_HL	}, //  3 de
    { offsetof(TZ80State, hl)	,	HEX_16,	3,	3,	POS_HL,	POS_HL1,POS_DE,	POS_HL	}, //  4 hl
													
//  { offsetof(TZ80State, alt.af),	HEX_16,	11,	0,	1,	9,	5,	6	}, //  5 af'
    { offsetof(TZ80State, alt.a),	HEX_8,	11,	0,	POS_F,	POS_F1,	POS_A1,	POS_BC1	}, //  5 a'	//NS
													
    { offsetof(TZ80State, alt.bc),	HEX_16,	11,	1,	POS_BC,	POS_PC,	POS_A1,	POS_DE1	}, //  6 bc'
    { offsetof(TZ80State, alt.de),	HEX_16,	11,	2,	POS_DE,	POS_IX,	POS_BC1,POS_HL1	}, //  7 de'
    { offsetof(TZ80State, alt.hl),	HEX_16,	11,	3,	POS_HL,	POS_IY,	POS_DE1,POS_HL1	}, //  8 hl'
    { offsetof(TZ80State, sp)	,	HEX_16,	19,	0,	POS_F1,	POS_I,	POS_SP,	POS_PC	}, //  9 sp
    { offsetof(TZ80State, pc)	,	HEX_16,	19,	1,	POS_BC1,POS_T,	POS_SP,	POS_IX	}, // 10 pc
    { offsetof(TZ80State, ix)	,	HEX_16,	19,	2,	POS_DE1,POS_IM,	POS_PC,	POS_IY	}, // 11 ix
    { offsetof(TZ80State, iy)	,	HEX_16,	19,	3,	POS_HL1,POS_FSF,POS_IX,	POS_IY	}, // 12 iy
    { offsetof(TZ80State, i)	,	HEX_8,	27,	0,	POS_SP,	POS_R,	POS_I,	POS_T	}, // 13 i
    { offsetof(TZ80State, r_low) ,	HEX_8,	30,	0,	POS_I,	POS_R,	POS_R,	POS_T	}, // 14 r
    { offsetof(TZ80State, im)	,	HEX_IM2,26,	2,	POS_IX,	POS_IF1,POS_T,	POS_FF5	}, // 15 im
    { offsetof(TZ80State, iff1)	,	HEX_1,	30,	2,	POS_IM,	POS_IF2,POS_T,	POS_FNF	}, // 16 iff1
    { offsetof(TZ80State, iff2)	,	HEX_1,	31,	2,	POS_IF1,POS_IF2,POS_T,	POS_FCF	}, // 17 iff2
    { offsetof(TZ80State, f)     ,	BIT_7,	24,	3,	POS_IY,	POS_FZF,POS_IM,	POS_FSF	}, // 18 SF
    { offsetof(TZ80State, f)     ,	BIT_6,	25,	3,	POS_FSF,POS_FF5,POS_IM,	POS_FZF	}, // 19 ZF
    { offsetof(TZ80State, f)     ,	BIT_5,	26,	3,	POS_FZF,POS_FHF,POS_IM,	POS_FF5	}, // 20 F5
    { offsetof(TZ80State, f)     ,	BIT_4,	27,	3,	POS_FF5,POS_FF3,POS_IM,	POS_FHF	}, // 21 HF
    { offsetof(TZ80State, f)     ,	BIT_3,	28,	3,	POS_FHF,POS_FPV,POS_IF1,POS_FF3	}, // 22 F3
    { offsetof(TZ80State, f)     ,	BIT_2,	29,	3,	POS_FF3,POS_FNF,POS_IF1,POS_FPV	}, // 23 PV
    { offsetof(TZ80State, f)     ,	BIT_1,	30,	3,	POS_FPV,POS_FCF,POS_IF1,POS_FNF	}, // 24 NF
    { offsetof(TZ80State, f)     ,	BIT_0,	31,	3,	POS_FNF,POS_FCF,POS_IF2,POS_FCF	}, // 25 CF
													
    { offsetof(TZ80State, t)     ,	DEC_6T,	26,	1,	POS_PC,	POS_T,	POS_R,	POS_IF1	}, // 26 t	//NS
    { offsetof(TZ80State, alt.f),	HEX_8,	13,	0,	POS_A1,	POS_SP,	POS_F1,	POS_BC1	}, // 27 f'	//NS
	//		,	,  разрядность,	X,	Y,	left,	right,	up,	down			
															
															
};

// [NS] раньше тут был один не читаемый набор цифор...


//=============================================================================

const size_t regs_layout_count = _countof(regs_layout);

//=============================================================================
void showregs()
{

//переместить в debug.h !!!!
#define DBG_ATTR_BCKGRND	0x00	//0	0
#define DBG_ATTR_BCKGRND_ACTIVE	0x10	//blue	0
#define DBG_ATTR_BCKGRND_BRIGHT	0X80	//br+	0

#define DBG_ATTR_REGS_NAMES	0x0C	//0		br+GREEN
#define DBG_ATTR_REGS_VALUES	0x0F	//0		br+WHITE
#define DBG_ATTR_REGS_CHANGED	0X0E	//0		br+YELLOW
#define DBG_ATTR_REGS_SELECTED	0xB0	//br+MAGENTA	0

#define DBG_ATTR_REGS_DIHALT	0x0A	//0	br+RED

//#define DBG_ATTR_REGS_TITLE	0x51	//cyan blue

#define DBG_ATTR_TITLES		0x5D//0x71	//white	blue

    Z80 &cpu = CpuMgr.Cpu();
    const TZ80State &prevcpu = CpuMgr.PrevCpu();

//   unsigned char atr = (activedbg == WNDREGS) ? W_SEL :	//0x17 blue  white	(выделенное окно)
//						  W_NORM;	//0x07 black white
    unsigned char current_back_attr = (activedbg == WNDREGS) ?	DBG_ATTR_BCKGRND_ACTIVE :	//(выделенное окно)
								DBG_ATTR_BCKGRND;

    unsigned char attr1 = current_back_attr | DBG_ATTR_REGS_NAMES;
    char line[40];
//  tprint(regs_x,regs_y+0, "af:**** af'**** sp:**** ir: ****", attr1);
//  tprint(regs_x,regs_y+1, "bc:**** bc'**** pc:**** t:******", attr1);
//  tprint(regs_x,regs_y+2, "de:**** de'**** ix:**** im?,i:**", attr1);
//  tprint(regs_x,regs_y+3, "hl:**** hl'**** iy:**** ########", attr1);
    
    tprint(regs_x,regs_y+0, "af:**** af'**** sp:**** ir:** **", attr1);
    tprint(regs_x,regs_y+1, "bc:**** bc'**** pc:**** t:******", attr1);
    tprint(regs_x,regs_y+2, "de:**** de'**** ix:**** im? i:**", attr1);
    tprint(regs_x,regs_y+3, "hl:**** hl'**** iy:**** ########", attr1);
    
//-----------------------------------------------------------------------------
unsigned char attr_values = (current_back_attr | DBG_ATTR_REGS_VALUES);
//-----------------------------------------------------------------------------
// Отрисовка числа тактов
    if (cpu.halted && !cpu.iff1)
    {
	//если di:halt такты не рисуютсо
//	tprint(	regs_x+26,
//		regs_y+1,
//		"DiHALT",
//		(activedbg == WNDREGS) ?	W_DIHALT1 :	//0x1A red
//						W_DIHALT2	//0x0A
//	       );
	tprint(	regs_x+26,		//NS
		regs_y+1,
		"DiHALT",
		( current_back_attr | DBG_ATTR_REGS_DIHALT )
	       );
    }
    else
    {
	sprintf(	line,
			"%6u",
			cpu.t
		);
	tprint(	   regs_x+26,
		   regs_y+1,
		   line,
		   ((regs_curs == POS_T) && (activedbg == WNDREGS))  ?	(current_back_attr | DBG_ATTR_REGS_SELECTED | DBG_ATTR_BCKGRND_BRIGHT) :
									attr_values
		);
    }
	
//-----------------------------------------------------------------------------
// Отрисовка значений регистров
    cpu.r_low = (cpu.r_low & 0x7F) + cpu.r_hi;
   
    for (unsigned i = 0; i < regs_layout_count; i++)
    {
	unsigned mask;	// = (1 << regs_layout[i].width) - 1;
	switch (regs_layout[i].width)
	{
	    case HEX_1:		mask = 0x01;	 break;
	    case HEX_IM2:	mask = 0x03;	 break;
	    case HEX_8:		mask = 0xFF;	 break;
	    case HEX_16:	mask = 0xFFFF;	 break;
	    default: 		mask = 0;
		
	}
	//текущее значение регистра
	unsigned val =	mask
			&
			*(unsigned*) ( PCHAR((TZ80State*)&cpu) + regs_layout[i].offs );

	//---------------------------------------------------------------------	
	// генерируем атрибут для текущего регистра
	//---------------------------------------------------------------------	
	// подсветка изменившегося значения регистров
	if (	val != (	mask
				& 
				*(unsigned*) ( PCHAR(&prevcpu) + regs_layout[i].offs ) // пред значение регистра
			)
	  )
	{
//	    atr1 |= 0x08;				
	    attr1 = (current_back_attr | DBG_ATTR_REGS_CHANGED);
	}
	//---------------------------------------------------------------------
	// регистр без изменений
	else
	{
	    attr1 = attr_values;
	}
	//---------------------------------------------------------------------
	// подсветка/выделение ткнутых курсором регистров
	if (	(activedbg == WNDREGS) &&
		(i == regs_curs)
	  )
	{
//	    atr1 = W_CURS; // 0x30		
	    attr1 = (current_back_attr | DBG_ATTR_REGS_SELECTED | DBG_ATTR_BCKGRND_BRIGHT);
	}
	//---------------------------------------------------------------------
	//printf("regs_curs %d\n",regs_curs);

    //-------------------------------------------------------------------------
    // генерируем строку разной ширины для 8/16 битных итд значений
    char bf[16];
    switch (regs_layout[i].width)
    {
         case HEX_1:
         case HEX_IM2:	sprintf(bf,	"%01X",	val); break;
         case HEX_8:	sprintf(bf,	"%02X",	val); break;
         case HEX_16:	sprintf(bf,	"%04X",	val); break;
	//такты рисуютсо отдельно !!
         default:	*bf = 0;
    }
    // рисуем строку
    tprint(	regs_x + regs_layout[i].x,
		regs_y + regs_layout[i].y,
		bf,
		attr1	//атрибут в зависимости от состояния регистра
	   );
   }
//-----------------------------------------------------------------------------
// отрисовка флагов
//  static const char flg[] = "SZ5H3PNCsz.h.pnc";	//зачем то объявляетсо каждый раз :)
    static const char flg[] = "SZ5H3PNCsz_h_pnc";	//зачем то объявляетсо каждый раз :)
    for (unsigned char q = 0; q < 8; q++)
    {	   
	//---------------------------------------------------------------------	
	// генерируем атрибут для текущего флага
	//---------------------------------------------------------------------
	unsigned ln;
	ln = unsigned ( flg	[ q + ( (cpu.af & (0x80>>q)) ?	0 :
								8
					)
				 ]
			);
	//---------------------------------------------------------------------	
	// подсветка изменившегося значения регистров			
	if ( (0x80 >> q) & (cpu.f ^ prevcpu.f) )
	{
	    //atr1 |= 0x08;	//подсветка изменившихся флагов
	    attr1 = (current_back_attr | DBG_ATTR_REGS_CHANGED);
	}
	//---------------------------------------------------------------------
	// дефолтный атрибут для флагов
	else
	{
	    attr1 = attr_values;
	}
	//---------------------------------------------------------------------
	// подсветка выделения курсором ткнутых флагов
	if (	(activedbg == WNDREGS)			&&
		(regs_curs == (unsigned)(q+18))
	  ) 
	{
	    //atr1 = W_CURS;	//выделение флагов курсором
	    attr1 = (current_back_attr | DBG_ATTR_REGS_SELECTED | DBG_ATTR_BCKGRND_BRIGHT);
	}
	//---------------------------------------------------------------------
	
	// рисуем флаг	
	tprint(		regs_x + 24 + q,
			regs_y + 3,
			(char*) &ln,
			attr1
	       );
    } //for
//-----------------------------------------------------------------------------
    //заголовок окна
    tprint(	regs_x,
		regs_y-1,
		"Regs",
		DBG_ATTR_TITLES	//DBG_ATTR_REGS_TITLE	//W_TITLE		//0x59	cyan brght+blue
	   );
//-----------------------------------------------------------------------------
    //рамочка вокруг окна
    frame(	regs_x,
		regs_y,
		32,
		4,
		FRAME		//0x01
	  );
} //void showregs()
//=============================================================================


//=============================================================================
void rleft() { regs_curs = regs_layout[regs_curs].lf; }		// "reg.left" хоткей
//=============================================================================
void rright() { regs_curs = regs_layout[regs_curs].rt; }	// "reg.right" хоткей
//=============================================================================
void rup() { regs_curs = regs_layout[regs_curs].up; }		// "reg.up" хоткей
//=============================================================================
void rdown() { regs_curs = regs_layout[regs_curs].dn; }		// "reg.down" хоткей
//=============================================================================


//=============================================================================
void renter()						// "reg.edit" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    
    debugscr();
    debugflip();
    
    unsigned char sz = regs_layout[ regs_curs].width;

    
    unsigned mask;					// [NS]
    //-------------------------------------------------------------------------
    switch (sz)
    {
	case HEX_1:		mask = 0x01;		break;
	case HEX_IM2:		mask = 0x03;		break;
	case HEX_8:		mask = 0xFF;		break;
	case HEX_16:		mask = 0xFFFF;		break;
	case DEC_6T:		mask = 0xFFFFFFFF;	break;
	default: 		mask = 0;	
    }
    //-------------------------------------------------------------------------
    
    unsigned val =	mask	//( (1 << sz) - 1)	// подрязка исходных значений
			&				// перед отображением при редактировании
			*(unsigned*)( PCHAR(( TZ80State*)&cpu) + regs_layout[ regs_curs].offs);
			
    unsigned char *ptr = PUCHAR(( TZ80State*)&cpu) + regs_layout[ regs_curs].offs;

    u8 Kbd[ 256];
    GetKeyboardState( Kbd);
    unsigned short k = 0;
    //-------------------------------------------------------------------------
    if ( ToAscii(	input.lastkey,
			0,
			Kbd,
			&k,
			0   )	!= 1
      )
    {
	//printf("return ");
	return;    
    }
    //-------------------------------------------------------------------------
    u8 u = u8( toupper( k));
    //-------------------------------------------------------------------------
    // послать повторно код клавиши
    // чтобы сразу пошел ввод этой клавиши
    // а не вход в режим ввода а только по след кнопке ввод
    if (   (	(sz == HEX_8)	||
		(sz == HEX_16)	||
		(sz == DEC_6T)	||
		(sz == HEX_IM2)
	    )
	   &&
	   (	(u >= '0' && u <= '9')	||
		(u >= 'A' && u <= 'F')
	    )
     )
    {
	PostThreadMessage(	GetCurrentThreadId(),	//????????
				WM_KEYDOWN,
				input.lastkey,
				1
			  );
    }
    //-------------------------------------------------------------------------
    switch (sz)
    {
	//---------------------------------------------------------------------
	case HEX_8:
	    val = unsigned(input2(regs_x + regs_layout[regs_curs].x, regs_y + regs_layout[regs_curs].y, val));
	    //-----------------------------------------------------------------
	    if (int(val) != -1)
		*ptr = u8(val);
	    //-----------------------------------------------------------------
	    break;
	//---------------------------------------------------------------------
	case HEX_16:
	    val = unsigned(input4(regs_x + regs_layout[regs_curs].x, regs_y + regs_layout[regs_curs].y, val));
	    //-----------------------------------------------------------------
	    if (int(val) != -1)
		*(unsigned short*)ptr = u16(val);
	    //-----------------------------------------------------------------
	    break;
	//---------------------------------------------------------------------
	case DEC_6T:	//24:	//редактор тактов	//NS
	    val = unsigned(input6dec(regs_x + regs_layout[regs_curs].x, regs_y + regs_layout[regs_curs].y, val));
	    //printf ("val1 %x\n",val);
	    //-----------------------------------------------------------------
	    if (int(val) != -1)
	    {
		// *(unsigned short*)ptr = u32(val);
		if (int(val) < cpu.tpi) //frametime)	//cpu.tpi только ZX но не GS!!!!!!
		{
		    *(int*)ptr = int(val);
		}
		//-------------------------------------------------------------
		else
		{
		    *(int*)ptr = (cpu.tpi-1);	//(frametime-1);	
						// если вписать число тактов больше чем есть
						// то происходит какаято херня
						// с считанием тактов назад ~ до frametime
						// так же не ясно точно за frametime-1
						// вроде бы 0-й такт есть
		}
	    }
	    //-----------------------------------------------------------------
	    //printf ("val2 %x\n",val);
	    //printf ("frametime %d\n",frametime);
	    break;
	//--------------------------------------------------------------------- 
	case HEX_1:
	    *ptr ^= 1; break;			//тупой ксор
	//--------------------------------------------------------------------- 
	case HEX_IM2:	//im2
	    //*ptr = (*ptr + 1) % 3; break;	//тупое ущербное гоняние по кругу 0,1,2,0,1...
	 
	    // нормальный ввод ручками [NS]
	    val = unsigned(input1(regs_x + regs_layout[regs_curs].x, regs_y + regs_layout[regs_curs].y, val));
	    //-----------------------------------------------------------------
	    if (int(val) != -1)
	    {
		if (int(val) < 3)
		    *ptr = u8(val);
	    }
	    //-----------------------------------------------------------------
	    break;
	//---------------------------------------------------------------------
	case BIT_0:				      //default: // flags
	case BIT_1:
	case BIT_2:
	case BIT_3:
	case BIT_4:
	case BIT_5:
	case BIT_6:
	case BIT_7:
	    *ptr ^= (1 << (sz - BIT_0));
	//---------------------------------------------------------------------
    }
    cpu.r_hi = cpu.r_low & 0x80;
    
    
    // если редактируем то зачем следим то включаем слежение
    
    switch (conf.trace_follow_regs)
    {
	case 1:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, af)))
		conf.trace_follow_request = 1;
	    break;
	case 2:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, bc)))
		conf.trace_follow_request = 1;
	    break;
	case 3:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, de)))
		conf.trace_follow_request = 1;
	    break;
	case 4:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, hl)))
		conf.trace_follow_request = 1;
	    break;
	    
	case 5:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, alt.af)))
		conf.trace_follow_request = 1;
	    break;
	case 6:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, alt.bc)))
		conf.trace_follow_request = 1;
	    break;
	case 7:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, alt.de)))
		conf.trace_follow_request = 1;
	    break;
	case 8:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, alt.hl)))
		conf.trace_follow_request = 1;
	    break;
	    
	case 9:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, ix)))
		conf.trace_follow_request = 1;
	    break;
	case 10:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, iy)))
		conf.trace_follow_request = 1;
	    break;
	    
	case 11:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, sp)))
		conf.trace_follow_request = 1;
	    break;
	case 12:
	    if ((regs_layout[ regs_curs].offs) == (offsetof( TZ80State, pc)))
		conf.trace_follow_request = 1;
	    break;
    }







 //   conf.trace_follow_request = 1;	//следовать за чем нибудь	// [NS]
	//printf("showtrace\n");
	//showtrace();
       //debugflip();	//tst
}
//=============================================================================


// в us0.38.0 и ниже это было переход и редактирование
// с us0.38.1 это работает как только выделение
// в Unreal TS это тоже работает как переход и редактирование
//=============================================================================
void ra() { regs_curs = 0; input.lastkey = 0; renter(); }	// "reg.a" хоткей
//=============================================================================
void rf() { regs_curs = 1; input.lastkey = 0; renter(); }	// "reg.f" хоткей
//=============================================================================
void rbc() { regs_curs = 2; input.lastkey = 0; renter(); }	// "reg.bc" хоткей
//=============================================================================
void rde() { regs_curs = 3; input.lastkey = 0; renter(); }	// "reg.de" хоткей
//=============================================================================
void rhl() { regs_curs = 4; input.lastkey = 0; renter(); }	// "reg.hl" хоткей
//=============================================================================
void rsp() { regs_curs = 9; input.lastkey = 0; renter(); }	// "reg.sp" хоткей
//=============================================================================
void rpc() { regs_curs = 10; input.lastkey = 0; renter(); }	// "reg.pc" хоткей
//=============================================================================
void rix() { regs_curs = 11; input.lastkey = 0; renter(); }	// "reg.ix" хоткей
//=============================================================================
void riy() { regs_curs = 12; input.lastkey = 0; renter(); }	// "reg.iy" хоткей
//=============================================================================
void ri() { regs_curs = 13; input.lastkey = 0; renter(); }	// "reg.i" хоткей
//=============================================================================
void rr() { regs_curs = 14; input.lastkey = 0; renter(); }	// "reg.r" хоткей
//=============================================================================
void rm() { regs_curs = 15; renter(); }				// "reg.im" хоткей
//=============================================================================
void r_1() { regs_curs = 16; renter(); }			// "reg.iff1" хоткей
//=============================================================================
void r_2() { regs_curs = 17; renter(); }			// "reg.iff2" хоткей
//=============================================================================
void rSF() { regs_curs = 18; renter(); }			// "reg.SF" хоткей
//=============================================================================
void rZF() { regs_curs = 19; renter(); }			// "reg.ZF" хоткей
//=============================================================================
void rF5() { regs_curs = 20; renter(); }			// "reg.F5" хоткей
//=============================================================================
void rHF() { regs_curs = 21; renter(); }			// "reg.HF" хоткей
//=============================================================================
void rF3() { regs_curs = 22; renter(); }			// "reg.F3" хоткей
//=============================================================================
void rPF() { regs_curs = 23; renter(); }			// "reg.PF" хоткей
//=============================================================================
void rNF() { regs_curs = 24; renter(); }			// "reg.NF" хоткей
//=============================================================================
void rCF() { regs_curs = 25; renter(); }			// "reg.CF" хоткей
//=============================================================================


//=============================================================================
// goto address in code window under cursor		// в меню WNDREGS
void rcodejump()					// и "reg.codejump" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    
    if (	(regs_layout[regs_curs].width == HEX_16)	||
		(regs_curs == POS_A)			||	//a, f по своему правилу
		(regs_curs == POS_F)			||
		(regs_curs == POS_A1)			||
		(regs_curs == POS_F1)
      )
    {
		// НЕ ЛОЖИТ В СТЕК ПЕРЕХОДОВ !!!!!!
		// тут ничего не объявлено надо подключать из dbgtrace и править makefile
		// push_pos();
		// memmove(&stack_pos[1], &stack_pos[0], sizeof stack_pos - sizeof *stack_pos);
		// memmove(&stack_cur[1], &stack_cur[0], sizeof stack_cur - sizeof *stack_cur);
		// stack_pos[0] = cpu.trace_top; stack_cur[0] = cpu.trace_curs;

         activedbg = WNDTRACE;
	 
	if (	(regs_curs == POS_A)	||	//половнки регов трактуем как весь AF	[NS]
		(regs_curs == POS_F)		//думаю чаще надо будет смотреть (AF) чем (A) или (F)
	  )
	{
	    cpu.trace_curs = cpu.trace_top = *(unsigned short*)(PCHAR((TZ80State*)&cpu) + offsetof(TZ80State, af));
	}
	else if ( (regs_curs == POS_A1)	||
		  (regs_curs == POS_F1)
	  )
	{
	    cpu.trace_curs = cpu.trace_top = *(unsigned short*)(PCHAR((TZ80State*)&cpu) + offsetof(TZ80State, alt.af));
	}
	else
	{
	    cpu.trace_curs = cpu.trace_top = *(unsigned short*)(PCHAR((TZ80State*)&cpu) + regs_layout[regs_curs].offs);
	}
         
    }
    // НУЖНА ОБРАБОТКА 8 БИТНЫХ AF
    //		влоб работает как goto (00Aa) и goto (AaFf)
    //		а не как goto (00Aa) и goto (00Ff)
    //		и не как goto (AaFf) в обоих случаях
    //		что не есть предсказуеммо
    
    // НЕ ЗАКИДЫВАЕТ АДРЕС В СТЕК!!!!
}
//=============================================================================


//=============================================================================
// goto address in data window under cursor			// в меню WNDREGS
void rdatajump()						// "reg.datajump" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    if (	(regs_layout[regs_curs].width == HEX_16)	||
		(regs_curs == POS_A)				||	//a, f по своему правилу
		(regs_curs == POS_F)				||
		(regs_curs == POS_A1)				||
		(regs_curs == POS_F1)
      )
    {
	activedbg = WNDMEM;
	editor = ED_MEM;
	if (	(regs_curs == POS_A)	||	//половнки регов трактуем как весь AF	[NS]
		(regs_curs == POS_F)		//думаю чаще надо будет смотреть (AF) чем (A) или (F)
	  )
	{
	    cpu.mem_curs = *(unsigned short*)(PCHAR((TZ80State*)&cpu) + offsetof(TZ80State, af));
	}
	else if ( (regs_curs == POS_A1)	||
		  (regs_curs == POS_F1)
	  )
	{
	    cpu.mem_curs = *(unsigned short*)(PCHAR((TZ80State*)&cpu) + offsetof(TZ80State, alt.af));
	}
	else
	{
	    cpu.mem_curs = *(unsigned short*)(PCHAR((TZ80State*)&cpu) + regs_layout[regs_curs].offs);
	}
	
	
    }
}
//=============================================================================


//=============================================================================
// начало правки в окне регистров
char dispatch_regs()
{
    if ((input.lastkey >= '0' && input.lastkey <= '9') || (input.lastkey >= 'A' && input.lastkey <= 'F'))
    {
	renter();
	return 1;
    }
    return 0;
}
//=============================================================================

