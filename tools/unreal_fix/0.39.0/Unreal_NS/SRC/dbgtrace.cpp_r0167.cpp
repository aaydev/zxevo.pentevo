#include "std.h"

#include "emul.h"
#include "vars.h"
#include "debug.h"
#include "dbgtrace.h"
#include "dbglabls.h"
#include "dbgpaint.h"
#include "dbgcmd.h"
#include "memory.h"
#include "z80asm.h"
#include "z80/op_system.h"
#include "util.h"


// добавить в мейк !!!1
#include "gsz80.h"		// для z80gs::GSCPUFQ	[NS]


//unsigned trace_PC_in_view;	//флаг что PC находится в полезрения	 //[NS]

unsigned trace_follow_regs_in_view;	//флаг что PC находится в полезрения	 //[NS]


//unsigned trace_follow_request;	// .conf флаг что нужно перевести фокус на regs //[NS]

//unsigned trace_follow_regs;	// теперь conf.trace_follow_regs






//=============================================================================
bool get_labels( unsigned addr, char *line)				// [NS]
{
    Z80 &cpu = CpuMgr.Cpu();

    sprintf( line, "   > "); 
    int ptr = 5;
    //-------------------------------------------------------------------------
    // .... LABELS xxx xx,xx
    //if (trace_labels)
    {
	char *virtlbl = mon_labels.find(((unsigned char *)NULL) + addr);	//NEDOREPO
	char *lbl = mon_labels.find( am_r( addr));
	//---------------------------------------------------------------------
	if (virtlbl)
	    lbl = virtlbl;		//NEDOREPO
	//---------------------------------------------------------------------
//    	if (lbl) for (int k = 0;  k < 10  && lbl[k];  line[ptr++] = lbl[k++]); //Alone Coder	//ПОЛНОСТЬЮ УДАЛЕНО В NEDOREPO
	if (lbl)
	{
	    for (int k = 0;    (k < 10) && lbl[k];    )
		line[ptr++] = lbl[k++]; //Alone Coder
	}
	else
	{
	    return FALSE;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    // заливка строки от опкода до мнемоники
    while (ptr < 32)
	line[ ptr++] = ' ';
    //-------------------------------------------------------------------------
    return TRUE;
}
//=============================================================================



//=============================================================================
int disasm_line( unsigned addr, char *line)
{
    Z80 &cpu = CpuMgr.Cpu();
    unsigned char dbuf[ 16 + 129];			/*Alone Code 0.36.7*/
    ptrdiff_t i; 					//Alone Coder 0.36.7
    //-------------------------------------------------------------------------
    for (/*int*/ i = 0;    i < 16;    i++)
	dbuf[i] = cpu.DirectRm( addr + unsigned(i));
    //-------------------------------------------------------------------------
    sprintf( line, "%04X ", addr); 
    int ptr = 5;
    ptrdiff_t len = disasm( dbuf, addr, char( trace_labels)) - dbuf;
    //8000 ..DDCB0106 rr (ix+1)
    //-------------------------------------------------------------------------
    // .... LABELS xxx xx,xx
    if (trace_labels)
    {
	char *virtlbl = mon_labels.find(((unsigned char *)NULL) + addr);	//NEDOREPO
	char *lbl = mon_labels.find( am_r(addr));
	//---------------------------------------------------------------------
	if (virtlbl)
	    lbl = virtlbl;		//NEDOREPO
	//---------------------------------------------------------------------
//    	if (lbl) for (int k = 0;  k < 10  && lbl[k];  line[ptr++] = lbl[k++]); //Alone Coder	//ПОЛНОСТЬЮ УДАЛЕНО В NEDOREPO
	if (lbl)
	    for (int k = 0;    (k < 10) && lbl[k];    )
		line[ptr++] = lbl[k++]; //Alone Coder
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    // .... OPCODES xx,xx
    else
    {
	ptrdiff_t len1 = len;
	//---------------------------------------------------------------------
	if (len > 4)
	{
	    len1 = 4;
	    *(short*)(line + ptr) = WORD2('.', '.');
	    ptr += 2;
	}
	//---------------------------------------------------------------------
	for (i = len - len1;    i < len;    i++)
	{
	    sprintf(line + ptr, "%02X", dbuf[i]);
	    ptr += 2;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    // заливка строки от опкода до мнемоники
    while (ptr < 16)
	line[ ptr++] = ' ';
    //-------------------------------------------------------------------------
    strcpy( line + ptr, asmbuf);
    return int( len);
}
//=============================================================================
#define TWF_BRANCH  0x010000U
#define TWF_BRADDR  0x020000U
#define TWF_LOOPCMD 0x040000U
#define TWF_CALLCMD 0x080000U
#define TWF_BLKCMD  0x100000U
#define TWF_HALTCMD 0x200000U
// Младшие 16бит - адрес z80
// старшие 16бит - флаги TWF_xxxx
//=============================================================================


//=============================================================================
static unsigned tracewndflags()						//????
{
    Z80 &cpu = CpuMgr.Cpu();
    unsigned readptr = cpu.pc, base = cpu.hl;
    unsigned char opcode = 0; unsigned char ed = 0;
    //-------------------------------------------------------------------------
    for (;;)
    {
	opcode = cpu.DirectRm(readptr++);
	if (opcode == 0xDD)
	    base = cpu.ix;
	else if (opcode == 0xFD)
	    base = cpu.iy;
	else if (opcode == 0xED)
	    ed = 1;
	else
	    break;
    }
    //-------------------------------------------------------------------------
    unsigned fl = 0;
    if (opcode == 0x76) // halt
    {
	u32 addr;
	if (cpu.im < 2)		//!!!! точно ли для всех?????
	{
	    addr = 0x38;
	}
	else // im2
	{
	    unsigned vec = unsigned(cpu.i << 8U) | cpu.IntVec();
	    addr = u32((cpu.DirectRm(vec+1) << 8U) | cpu.DirectRm(vec));
	}
	return TWF_HALTCMD | addr;
    }
//-----------------------------------------------------------------------------
    if (ed)
    {
	//---------------------------------------------------------------------
	if ((opcode & 0xF4) == 0xB0) // ldir/lddr | cpir/cpdr | inir/indr | otir/otdr
	    return TWF_BLKCMD;
	//---------------------------------------------------------------------
	if ((opcode & 0xC7) != 0x45)
	    return 0; // reti/retn
	//---------------------------------------------------------------------
ret:
	return (cpu.DirectRm(cpu.sp) | unsigned(cpu.DirectRm(cpu.sp+1) << 8U)) | TWF_BRANCH | TWF_BRADDR;
    }
    //-------------------------------------------------------------------------
    if (opcode == 0xC9)						// ret
	goto ret;
    //-------------------------------------------------------------------------
    if (opcode == 0xC3)						// jp
    {
jp:	
	return (cpu.DirectRm(readptr) | unsigned(cpu.DirectRm(readptr+1) << 8U)) | TWF_BRANCH | fl;
    }
    //-------------------------------------------------------------------------
    if (opcode == 0xCD)						// call
    {
	fl = TWF_CALLCMD;
	goto jp;
    }
    //-------------------------------------------------------------------------
    static const unsigned char flags[] = { ZF,CF,PV,SF };

    if ((opcode & 0xC1) == 0xC0)
    {
	unsigned char flag = flags[(opcode >> 4) & 3];
	unsigned char res = cpu.f & flag;
	//---------------------------------------------------------------------
	if (!(opcode & 0x08))
	    res ^= flag;
	if (!res)
	    return 0;
	//---------------------------------------------------------------------
	if ((opcode & 0xC7) == 0xC0)				// ret cc
	    goto ret;
	//---------------------------------------------------------------------
	if ((opcode & 0xC7) == 0xC4)				// call cc
	{
	    fl = TWF_CALLCMD;
	    goto jp;
	}
	//---------------------------------------------------------------------
	if ((opcode & 0xC7) == 0xC2)				// jp cc
	{
	    fl = TWF_LOOPCMD;
	    goto jp;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    if (opcode == 0xE9)
	return base | TWF_BRANCH | TWF_BRADDR;		// jp (hl/ix/iy)
    //-------------------------------------------------------------------------
    if ((opcode & 0xC7) == 0xC7)
	return (opcode & 0x38) | TWF_CALLCMD | TWF_BRANCH;	// rst #xx
    //-------------------------------------------------------------------------
    if ((opcode & 0xC7) == 0x00)
    {
	//---------------------------------------------------------------------
	if (!opcode || opcode == 0x08)
	    return 0;
	//---------------------------------------------------------------------
	int offs = (signed char)cpu.DirectRm(readptr++);
	unsigned addr = unsigned(offs + int(readptr)) | TWF_BRANCH;
	if (opcode == 0x18)
	    return addr;					// jr
	//---------------------------------------------------------------------
	if (opcode == 0x10)
	    return (cpu.b==1)? 0 : addr | TWF_LOOPCMD;		// djnz
	//---------------------------------------------------------------------
	unsigned char flag = flags[(opcode >> 4) & 1];		// jr cc
	unsigned char res = cpu.f & flag;
	if (!(opcode & 0x08))
	    res ^= flag;
	//---------------------------------------------------------------------
	return res  ?	addr | TWF_LOOPCMD : 
			0;
	//---------------------------------------------------------------------
    }
    return 0;
}
//=============================================================================


//=============================================================================
static unsigned trcurs_y;
unsigned asmii;
static char asmpc[64], dumppc[12];
const unsigned cs[3][2] = { {0,4}, {5,10}, {16,16} };

//=============================================================================
// обновление окна дизасма в дебагере

void showtrace()
{

#define DBG_ATTR_TITLES		0x5D//0x71	//white	blue


	char trace_follow_regs_text[10];

	    switch (conf.trace_follow_regs)
	    {
		case REG_AF: 	sprintf( trace_follow_regs_text, "(AF)");	break;
		case REG_BC: 	sprintf( trace_follow_regs_text, "(BC)");	break;
		case REG_DE: 	sprintf( trace_follow_regs_text, "(DE)");	break;
		case REG_HL: 	sprintf( trace_follow_regs_text, "(HL)");	break;
		case REG_AF1:	sprintf( trace_follow_regs_text, "(AF\')");	break;
		case REG_BC1:	sprintf( trace_follow_regs_text, "(BC\')");	break;
		case REG_DE1:	sprintf( trace_follow_regs_text, "(DE\')");	break;
		case REG_HL1:	sprintf( trace_follow_regs_text, "(HL\')");	break;
		case REG_IX: 	sprintf( trace_follow_regs_text, "(IX)");		break;
		case REG_IY: 	sprintf( trace_follow_regs_text, "(IY)");		break;
		case REG_SP: 	sprintf( trace_follow_regs_text, "(SP)");		break;
		case REG_PC:	sprintf( trace_follow_regs_text, "(PC)");		break;
		default:	sprintf( trace_follow_regs_text, "(None)");	break;
	    }

	   		    tprint(	trace_x + 26,
					trace_y - 1,
					trace_follow_regs_text,
					DBG_ATTR_TITLES
			      );
	        
    //printf("showtrace()\n");

//переместить в debug.h !!!!
#define DBG_ATTR_BCKGRND		0x00	//0	0
#define DBG_ATTR_BCKGRND_ACTIVE		0x10	//blue	0
#define DBG_ATTR_BCKGRND_BRIGHT		0X80	//br+	0

#define DBG_ATTR_TRACE_LINES		0x0F	//0		br+WHITE
#define DBG_ATTR_TRACE_LINES_ROM	0x07	//0		WHITE


#define DBG_ATTR_TRACE_BREAK_EX		0x0A	//0		red


#define DBG_ATTR_TRACE_BREAK_R_BACK	0x40	//red		0
#define DBG_ATTR_TRACE_BREAK_W_BACK	0x20	//green		0
#define DBG_ATTR_TRACE_BREAK_RW_BACK	0x60	//yellow	0
#define DBG_ATTR_TRACE_REG_PC_BACK	0xD0	//br+WHITE	0
#define DBG_ATTR_TRACE_REG_FOLLOW_BACK	0xF0	//br+WHITE	0



#define DBG_ATTR_TRACE_SELECTED		0xB0	//br+MAGENTAA	0


#define DBG_ATTR_TRACE_BRANCH_DIRECTION		0x0A	//0	br+RED
#define DBG_ATTR_TRACE_BRANCH_DIRECTION_INV	0x0D	//0	br+CYAN

#define DBG_ATTR_TRACE_BRANCH_DESTINATION	0x0D	//0	br+CYAN

#define DBG_ATTR_TRACE_CURRENT_Z80		0x5D//0x70	//white	0
#define DBG_ATTR_TRACE_LAST_BRANCH		0x5D//0x70	//white	0



/*
масив хранит адреса всех отображаемых строк в дебагере
cpu.trpc[00] - 00E5
cpu.trpc[01] - 00E7
cpu.trpc[02] - 00E8

cpu.trpc[18] - 0109
cpu.trpc[19] - 010C
cpu.trpc[20] - 010F

+ еще строка с адресом начала следующей страницы
cpu.trpc[21] - 0112
*/


//    trace_follow_regs = 4;


restart_showtrace:


    
    //trace_PC_in_view = 0;
    trace_follow_regs_in_view = 0;

    Z80 &cpu = CpuMgr.Cpu();
//  char line[ 40];		//Alone Coder 0.36.7
    char line[ 16 + 129];	//Alone Coder 0.36.7

    cpu.trace_curs &= 0xFFFF;
    cpu.trace_top &= 0xFFFF;
    cpu.pc &= 0xFFFF;
    cpu.trace_mode = (cpu.trace_mode + 3) % 3;

    cpu.pc_trflags = tracewndflags();
    cpu.nextpc = (cpu.pc_trflags & TWF_HALTCMD)  ?  (cpu.pc_trflags & 0xFFFF):
 						    ((cpu.pc + unsigned( disasm_line( cpu.pc, line))) & 0xFFFF );
    unsigned pc = cpu.trace_top;
    asmii = -1U;	//курсор в не поля зрения
    
//  unsigned char atr0 = (activedbg == WNDTRACE)   ?	W_SEL :	//0x17
//							W_NORM;	//0x07

    unsigned char attr1;	//temp attr
    unsigned char current_back_attr = (activedbg == WNDTRACE)  ?  DBG_ATTR_BCKGRND_ACTIVE :	//(выделенное окно)
								  DBG_ATTR_BCKGRND;
								
    unsigned char attr_lines = (current_back_attr | DBG_ATTR_TRACE_LINES);
    unsigned char attr_lines_rom = (current_back_attr | DBG_ATTR_TRACE_LINES_ROM);
    //-------------------------------------------------------------------------
    unsigned ii; //Alone Coder 0.36.7
    
    //-------------------------------------------------------------------------

    for (ii = 0;    ii < trace_size;    ii++)
    {
	pc &= 0xFFFF;
	cpu.trpc[ ii] = pc;
	int len = disasm_line( pc, line);
      
	//---------------------------------------------------------------------
	char *ptr = line + strlen( line);	//видимо зачистка строки
	//---------------------------------------------------------------------
	while (ptr < line + 32)
	    *ptr++ = ' ';
	//---------------------------------------------------------------------
	line[ 32] = 0;
	//---------------------------------------------------------------------


	
#define	FLAG_FOLLOW	0x01
#define	FLAG_BP_X	0x02
#define	FLAG_BP_R	0x04
#define	FLAG_BP_W	0x08

#define	FLAG_PC		0x10	//+FLAG_FOLLOW

	//---------------------------------------------------------------------
	//attr1 = attr_lines;

	attr1 = (bankr[ (pc >> 14) & 3] != bankw[ (pc >> 14) & 3])  ?  attr_lines_rom	:
								       attr_lines ;

	
	int temp_line_flags = 0;
	//int temp_line_pc_flag = 0;
	
	//---------------------------------------------------------------------
	if (pc == (cpu.pc & 0xFFFF))
	{
	    temp_line_flags |= FLAG_PC;
	}
	//---------------------------------------------------------------------
	int follow_regs_value;
	
	if (conf.trace_follow_regs == REG_AF)	follow_regs_value = cpu.af;
	if (conf.trace_follow_regs == REG_BC)	follow_regs_value = cpu.bc;
	if (conf.trace_follow_regs == REG_DE)	follow_regs_value = cpu.de;
	if (conf.trace_follow_regs == REG_HL)	follow_regs_value = cpu.hl;
                                      
	if (conf.trace_follow_regs == REG_AF1)	follow_regs_value = cpu.alt.af;
	if (conf.trace_follow_regs == REG_BC1)	follow_regs_value = cpu.alt.bc;
	if (conf.trace_follow_regs == REG_DE1)	follow_regs_value = cpu.alt.de;
	if (conf.trace_follow_regs == REG_HL1)	follow_regs_value = cpu.alt.hl;
                                      
	if (conf.trace_follow_regs == REG_IX)	follow_regs_value = cpu.ix;
	if (conf.trace_follow_regs == REG_IY)	follow_regs_value = cpu.iy;
	if (conf.trace_follow_regs == REG_SP)	follow_regs_value = cpu.sp;
	if (conf.trace_follow_regs == REG_PC)	follow_regs_value = cpu.pc;

	follow_regs_value &= 0xFFFF;	//тк в старших битах может быть левый мусор РЕАЛЬНО!!!
    //-------------------------------------------------------------------------
    // Если следим за каким то регистром
    if (conf.trace_follow_regs)
    {
	if (pc == follow_regs_value)
	{
	    temp_line_flags |= FLAG_FOLLOW;
	    trace_follow_regs_in_view = 1;	//trace_PC_in_view = 1; }
	} 
    }
    //-------------------------------------------------------------------------
    // Если не следим
    else
    {
	trace_follow_regs_in_view = 1;
	conf.trace_follow_request = 0;
    }
    //-------------------------------------------------------------------------
    
	
//	if (pc == cpu.pc) { temp_line_flags |= FLAG_PC;	trace_follow_regs_in_view = 1; } //trace_PC_in_view = 1; }
	
	
	if (cpu.membits[ pc] & MEMBITS_BPX) 	temp_line_flags |= FLAG_BP_X;
	if (cpu.membits[ pc] & MEMBITS_BPR) 	temp_line_flags |= FLAG_BP_R;
	if (cpu.membits[ pc] & MEMBITS_BPW) 	temp_line_flags |= FLAG_BP_W;
	//---------------------------------------------------------------------
	switch ((temp_line_flags & (FLAG_BP_X | FLAG_BP_R | FLAG_BP_W)))
	{
	    //X only ----------------------------------------------------------
	    case (FLAG_BP_X):				//...O
		attr1 = (attr1 & 0xF0) | DBG_ATTR_TRACE_BREAK_EX;	break;	//+ back
	    //RW --------------------------------------------------------------
	    case (FLAG_BP_R):				//..O.
		attr1 = (attr1 & 0x0F) | DBG_ATTR_TRACE_BREAK_R_BACK;	break;	//+ ink
	    case (FLAG_BP_W):				//.O..
		attr1 = (attr1 & 0x0F) | DBG_ATTR_TRACE_BREAK_W_BACK;	break;	//+ ink
	    case (FLAG_BP_R | FLAG_BP_W):		//.OO.
		attr1 = (attr1 & 0x0F) | DBG_ATTR_TRACE_BREAK_RW_BACK;	break;	//+ ink
	    //RW + X ----------------------------------------------------------
	    case (FLAG_BP_X | FLAG_BP_R):		//..OO
		attr1 = DBG_ATTR_TRACE_BREAK_EX | DBG_ATTR_TRACE_BREAK_R_BACK;	break;
	    case (FLAG_BP_X | FLAG_BP_W):		//.O.O
		attr1 = DBG_ATTR_TRACE_BREAK_EX | DBG_ATTR_TRACE_BREAK_W_BACK;	break;
	    case (FLAG_BP_X | FLAG_BP_R | FLAG_BP_W):	//.OOO
		attr1 = DBG_ATTR_TRACE_BREAK_EX | DBG_ATTR_TRACE_BREAK_RW_BACK;	break;
	}
	//---------------------------------------------------------------------
	switch ((temp_line_flags & (FLAG_BP_X | FLAG_PC | FLAG_FOLLOW)))
	{	
	    //Follow поверx PC  -----------------------------------------------
	    case (FLAG_FOLLOW):				
	    case (FLAG_FOLLOW | FLAG_PC):		
		attr1 = DBG_ATTR_TRACE_REG_FOLLOW_BACK;
		break;
	    //Follow поверх PC + X --------------------------------------------
	    case (FLAG_BP_X | FLAG_FOLLOW):				
	    case (FLAG_BP_X | FLAG_FOLLOW | FLAG_PC):		
		attr1 = DBG_ATTR_TRACE_REG_FOLLOW_BACK | DBG_ATTR_TRACE_BREAK_EX;
		break;
	    //PC --------------------------------------------------------------
	    case (FLAG_PC):	
		attr1 = DBG_ATTR_TRACE_REG_PC_BACK;
		break;		
	    //PC + X ----------------------------------------------------------
	    case (FLAG_PC | FLAG_BP_X):		
		attr1 = DBG_ATTR_TRACE_REG_PC_BACK | DBG_ATTR_TRACE_BREAK_EX;
		break;
	    //-----------------------------------------------------------------
	}
	    

		

//	//---------------------------------------------------------------------
//	// атрибут строки с (текущим PC) / (обычная строка)
//	attr1 = (pc == cpu.pc)  ?	DBG_ATTR_TRACE_REG_PC_BACK :	//W_TRACEPOS :	
//					attr_lines;			//atr0;
//	//---------------------------------------------------------------------


//	// атрибут всей строки с бряком
//	//					так же потом можно будет отображать разные виды бряков
//	//					разными цветами
//
//	if (cpu.membits[pc] & MEMBITS_BPX)	//EX
//	{
//		attr1 = (attr1 & 0xF0) | DBG_ATTR_TRACE_BREAK_EX;	// + back
//	}

	
		// TEZT !!!!!!!!!!!!!1
		char labels_line[ 16 + 129];
		
		if (get_labels( pc, labels_line))
		{
		    tprint(	trace_x,
				(trace_y + ii),
				labels_line,
				((current_back_attr & 0xF0) | 0x0E)	//F)
				);
				
		    //trace_y_displace++;
		    //trace_size_1--;
		    ii++;
		    cpu.trpc[ ii] = pc;
		}
		// TEZT !!!!!!!!!!!!!1
		
		

	//---------------------------------------------------------------------
	tprint(		trace_x,
			(trace_y + ii),		//trace_y + ii,
			line,
			attr1
	       );
	       
	//---------------------------------------------------------------------
	// выделение положения курсора в окне дизасма
	// НЕ НА ВСЮ СТРОКУ! а только в активном столбце
	
	
	if (pc == cpu.trace_curs)
	{
	    asmii = ii;				// установка "курсора по реальному положению
	    cpu.graph_trace_cursor_pos = ii;	// [NS]
	    //-----------------------------------------------------------------
	    if (activedbg == WNDTRACE)
	    {
		for (unsigned q = 0;    q < cs[ cpu.trace_mode][ 1];    q++)
		{
		    txtscr[	s80 * s30		+ 
				(trace_y + ii) * s80	+
				trace_x			+
				cs[ cpu.trace_mode][ 0]	+ 
				q
			   ] = (DBG_ATTR_TRACE_SELECTED);	//W_CURS;
		}
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	if (cpu.pc_trflags & TWF_BRANCH)
	{
	    //-----------------------------------------------------------------
	    if (pc == cpu.pc)
	    {
		unsigned addr = cpu.pc_trflags & 0xFFFF;
		//-------------------------------------------------------------
		// символ стрелочки направления перехода
		unsigned arr = (addr <= cpu.pc)  ?  0x18 :
						    0x19;	// up/down arrow
		attr1 = (	(pc == cpu.trace_curs)	&&
				(activedbg == WNDTRACE)	&&
				(cpu.trace_mode == 2)
			 )  
			    ?	// стрелочка направления перехода под курсором
				DBG_ATTR_TRACE_BRANCH_DIRECTION_INV :	//W_TRACE_JINFO_CURS_FG :  // 0x0D 0 br+CYAN
				// стрелочка направления
				DBG_ATTR_TRACE_BRANCH_DIRECTION;	//W_TRACE_JINFO_NOCURS_FG; // 0x02 0 red	
		//-------------------------------------------------------------
		// отрисовка стрелочка направления перехода вверх/вниз	    
		if (cpu.pc_trflags & TWF_BRADDR)
		{
		    sprintf(	line,
				"%04X%c",
				addr,
				int( arr)
			    );
		// адрес + стрелочка при RET	(5B13 C9 ret   0038^)
		    tprint_fg(	trace_x + 32 - 5,
				trace_y + ii,
				line,
				attr1	//ink only	//color
			      );
		}
		else 
		{
		// стрелочка при JP CALL	(0043 2003 jr nz,0048   v)
		    tprint_fg(	trace_x + 32 - 1,
				trace_y + ii,
				(char*) &arr,
				attr1	//ink only	//color
			      );
		}
             }
	    //-----------------------------------------------------------------
	    // стрелочка возле адреса куда происходит переход	(5B00 F5 push af   <)
	    if (pc == (cpu.pc_trflags & 0xFFFF))
	    {
		unsigned arr = 0x11; // left arrow
		tprint_fg(	trace_x + 32 - 1,
				trace_y + ii,
				(char*) &arr,
				DBG_ATTR_TRACE_BRANCH_DESTINATION	//W_TRACE_JARROW_FOREGR	//0x0D	br+CYAN
			  );
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	pc += unsigned( len);
    }
    //-------------------------------------------------------------------------
    cpu.trpc[ ii] = pc;		// обновляем масив адресов
    //-------------------------------------------------------------------------
    // на всякий
    cpu.trace_top &= 0xFFFF;	// [NS]
    cpu.trace_curs &= 0xFFFF;
					
    if (conf.trace_follow_request)					// [NS]
    {
	//printf("trace_follow_request \n");
	//if (!trace_PC_in_view)  //???
	if ((!trace_follow_regs_in_view) && (conf.trace_follow_regs))
	{
	    //printf("(!trace_PC_in_view)\n");
	    switch (conf.trace_follow_regs)
	    {
		case REG_AF: cpu.trace_top = cpu.trace_curs = cpu.af;	break;
		case REG_BC: cpu.trace_top = cpu.trace_curs = cpu.bc;	break;
		case REG_DE: cpu.trace_top = cpu.trace_curs = cpu.de;	break;
		case REG_HL: cpu.trace_top = cpu.trace_curs = cpu.hl;	break;
		case REG_AF1: cpu.trace_top = cpu.trace_curs = cpu.alt.af;	break;
		case REG_BC1: cpu.trace_top = cpu.trace_curs = cpu.alt.bc;	break;
		case REG_DE1: cpu.trace_top = cpu.trace_curs = cpu.alt.de;	break;
		case REG_HL1: cpu.trace_top = cpu.trace_curs = cpu.alt.hl;	break;
		case REG_IX: cpu.trace_top = cpu.trace_curs = cpu.ix;	break;
		case REG_IY: cpu.trace_top = cpu.trace_curs = cpu.iy;	break;
		case REG_SP: cpu.trace_top = cpu.trace_curs = cpu.sp;	break;
		case REG_PC: cpu.trace_top = cpu.trace_curs = cpu.pc;	break;
	    }
	    //cpu.trace_top = cpu.trace_curs = cpu.pc;
	    //printf("restart_showtrace ");
	    goto restart_showtrace;
	}
	else
	{
	    //printf("trace_follow_request = 0;");
	    conf.trace_follow_request = 0;
	}
    }
  
    //-------------------------------------------------------------------------
    // Kурсор посреди команды !!!			// [NS]
    // или page down
    if (asmii == -1U)				
    {
	// printf("(asmii == -1U) ");
	// переставляем верх на положение курсора и дизасмим еще раз
	
	//---------------------------------------------------------------------
	// Page down
	if (cpu.graph_trace_cursor_pos == (trace_size - 1))
	{
	    cpu.trace_curs = cpu.trpc[ cpu.graph_trace_cursor_pos];
	}
	//---------------------------------------------------------------------
	// Курсор посреди команды
	// текущими средствами можем отдизасмить только когда оно в trace_top
	//	нужно править дизасмер для обработки курсора посреди команды
	else
	{
	    cpu.graph_trace_cursor_pos = 0;
	    cpu.trace_top = cpu.trace_curs;	//cpu.trpc[0];	
	    //cpu.trace_curs = cpu.trpc[0];
	}
	//---------------------------------------------------------------------
	//printf("restart_showtrace 2");
	goto restart_showtrace;
    }
    //-------------------------------------------------------------------------
    


    unsigned char dbuf[16];
   //-------------------------------------------------------------------------
    unsigned i; //Alone Coder
    for (/*int*/ i = 0;    i < 16;    i++)
	dbuf[i] = cpu.DirectRm( cpu.trace_curs + i);
    //-------------------------------------------------------------------------
    ptrdiff_t len = disasm(	dbuf,
				cpu.trace_curs, 
				0
			   ) - dbuf;
    strcpy( asmpc, asmbuf);

    //-------------------------------------------------------------------------
    // опкоды команд в режиме редактирования
    for (/*int*/ i = 0;    i < len && i < 5;    i++)
	sprintf(	dumppc + i * 2,
			"%02X",
			cpu.DirectRm(cpu.trace_curs + i)
		);

    //-------------------------------------------------------------------------
    // печать текущего z80
    char cpu_num[ 10];
//    _snprintf(	cpu_num,
//			sizeof(cpu_num),
//			"Z80(%u)",
//			CpuMgr.GetCurrentCpu()
//	        );
    if ( (CpuMgr.GetCurrentCpu()) == 0 )	//(NS)
    {
	sprintf( cpu_num, "ZX-CPU" );//"ZX-Z80" );	//И зачем нам нужно было угадывать где мы щас?
    }
    else
    {
	sprintf( cpu_num, "GS-CPU" );//"GS-Z80" );
    }
	     
    tprint(	trace_x,
		trace_y-1,
		cpu_num,
		DBG_ATTR_TRACE_CURRENT_Z80	//W_TITLE
	   );
    //-------------------------------------------------------------------------
    // печать адреса последнего перехода
    char lbr[18];	//lbr[5];
    _snprintf(	lbr,
		sizeof(lbr),
		"Last branch(%04hX)",	//"%04hX",
		cpu.last_branch
	      );
    tprint(	trace_x+8,
		trace_y-1,
		lbr,
		DBG_ATTR_TRACE_LAST_BRANCH	//W_TITLE
	  );
    //-------------------------------------------------------------------------
    // рамочка
    frame(	trace_x,
		trace_y,
		32,
		trace_size,
		FRAME
	 );
    //-------------------------------------------------------------------------
    
} //void showtrace()
//=============================================================================


//=============================================================================
void c_lbl_import() //menu for importing labels from XAS/ALASM ???	// в меню WNDTRACE дебагера
{									// и "cpu.importl" хоткей
   mon_labels.import_menu();						// скорей всего уже сломан? 
}
//=============================================================================

      /* ------------------------------------------------------------- */
static unsigned save_pos[8] = { -1U,-1U,-1U,-1U,-1U,-1U,-1U,-1U };
static unsigned save_cur[8] = { -1U,-1U,-1U,-1U,-1U,-1U,-1U,-1U };
static unsigned stack_pos[32] = { -1U }, stack_cur[32] = { -1U };

//=============================================================================
void push_pos();							//????
void push_pos()
{
    Z80 &cpu = CpuMgr.Cpu();
    memmove(&stack_pos[1], &stack_pos[0], sizeof stack_pos - sizeof *stack_pos);
    memmove(&stack_cur[1], &stack_cur[0], sizeof stack_cur - sizeof *stack_cur);
    stack_pos[0] = cpu.trace_top; stack_cur[0] = cpu.trace_curs;
}
//=============================================================================


//=============================================================================
// Получение адреса предыдущей команды
//    хз как оно работает и на сколько точно
static unsigned cpu_up(unsigned ip)
{
    Z80 &cpu = CpuMgr.Cpu();
    //printf("ip = %X\n",ip);
    unsigned char buf1[0x10];
    unsigned p1 = (ip > sizeof buf1)  ?	ip - sizeof buf1 :
					0;
    //-------------------------------------------------------------------------
    for (unsigned i = 0;    i < sizeof buf1;    i++)
	buf1[i] = cpu.DirectRm(p1 + i);
    //-------------------------------------------------------------------------
    const unsigned char *dispos = buf1;
    const unsigned char *prev;
    //-------------------------------------------------------------------------
    do {
	prev = dispos;
	dispos = disasm(dispos, 0, 0);
    } while ((unsigned)(dispos - buf1 + p1) < ip);
    //-------------------------------------------------------------------------
    return unsigned(prev - buf1 + p1);
}
//=============================================================================




//=============================================================================
// устаревший goto adress в окне дизасма		// "cpu.goto" хотекей
// такой же как в окне мемори виевера
// жмем кнопку, вводим адрес ручками
void cgoto()						
{
    Z80 &cpu = CpuMgr.Cpu();
    int v = input4(trace_x, trace_y, cpu.trace_top);
    if (v != -1)
	cpu.trace_top = cpu.trace_curs = unsigned(v);
}
//=============================================================================


//=============================================================================
// SET_PC_TO_CURSOR					// в меню WNDTRACE дебагера
void csetpc()						// и "cpu.setpc" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    cpu.pc = cpu.trace_curs;
}
//=============================================================================


//=============================================================================
void center()	// edit instruction ???
{							//"cpu.asm" хоткей
    //-------------------------------------------------------------------------
    // нельзя редактировать когда курсор не найден !!!!
    // (посреди команды) нужно редизасмить
    if (asmii == -1U)
    {
	printf("asmii/trace cursor error\n");
	return;
    }
    //-------------------------------------------------------------------------
    Z80 &cpu = CpuMgr.Cpu();
    //-------------------------------------------------------------------------
    // addr
    if (!cpu.trace_mode) // = 0
	sprintf(	str,
			"%04X",
			cpu.trace_curs
		);
    //-------------------------------------------------------------------------
    // dump
    else if (cpu.trace_mode == 1)
	strcpy( str, dumppc);
    //-------------------------------------------------------------------------
    // disasm
    else
	strcpy( str, asmpc);
    //-------------------------------------------------------------------------
    // в случае входа в режим редактирования началом ввода текста
    // идет очистка всей строки
    //    НО center может быть не только на Enter-e !!!!!
    //    нужна проверка именно на center hotkey
    //    иначе нельзя переназначить center !!!!! [NS]
    if (input.lastkey != VK_RETURN)
    {
	//---------------------------------------------------------------------
	// прямой ввод значений в столбце адреса не очищает строку адреса
	if (cpu.trace_mode != 0) // addr
	{
	    *str = 0;
	}
	//---------------------------------------------------------------------
	// шляпа - ущербный ввод значения которым мы вошли в режим редактирования
	PostThreadMessage(	GetCurrentThreadId(),
				WM_KEYDOWN,
				input.lastkey,
				1
			);
    }
    //-------------------------------------------------------------------------
    for (    ;    ;    )
    {
	//---------------------------------------------------------------------
	// asmii как раз типо текущий курсор
	
	if (!inputhex(	(trace_x + cs[ cpu.trace_mode][ 0]),
			(trace_y + trcurs_y + asmii),		//trcurs_y вообще не используетсо
			(cs[ cpu.trace_mode][ 1]),
			(cpu.trace_mode < 2),		//0...1 - hex=TRUE 2 - hex=FALSE
			(cpu.trace_mode == 2) ?	FALSE :	//людской способ ввода текста
						TRUE	//hex-ы же удобно вводить insert-ом
		      )
	 )
	{
	    break;
	}
	//---------------------------------------------------------------------
	// addr
	if (!cpu.trace_mode)
	{
	    push_pos();
	    sscanf(	str,
			"%X",
			&cpu.trace_top
		   );
	    cpu.trace_curs = cpu.trace_top;
	    //-----------------------------------------------------------------
	    printf("asmii %d ",asmii);
	    for (unsigned i = 0;    i < asmii;    i++)
	    {
		printf("** ");
		cpu.trace_top = cpu_up(cpu.trace_top);
	    }
	    printf("\n");
	    //-----------------------------------------------------------------
	    break;
	}
	//---------------------------------------------------------------------
	// dump
	else if (cpu.trace_mode == 1)
	{
	    char *p; //Alone Coder 0.36.7
	    //-----------------------------------------------------------------
	    for (/*char * */p = str + strlen(str) - 1;    p >= str && *p == ' ';    *p-- = 0);
	    //-----------------------------------------------------------------
	    unsigned char dump[8];
	    unsigned i;
	    //-----------------------------------------------------------------
	    for (p = str, i = 0;    ishex(*p) && ishex(p[1]);    p += 2)
		dump[i++] = hex(p);
	    //-----------------------------------------------------------------
	    if (*p)
		continue;
	    //-----------------------------------------------------------------
	    for (unsigned j = 0;    j < i;    j++)
		cpu.DirectWm(cpu.trace_curs+j, dump[j]);
	    //-----------------------------------------------------------------
	    break;
	}
	//---------------------------------------------------------------------
	// disasm
	else
	{
	    unsigned sz = assemble_cmd((unsigned char*)str, cpu.trace_curs);
	    //-----------------------------------------------------------------
	    if (sz)
	    {
		//-------------------------------------------------------------
		for (unsigned i = 0;    i < sz;    i++)
		    cpu.DirectWm(cpu.trace_curs + i, asmresult[i]);
		//-------------------------------------------------------------
		showtrace();
		void cdown();
		cdown();
		break;
	    }
	    //-----------------------------------------------------------------
	}
    }
}
//=============================================================================


//=============================================================================
char dispatch_trace()				//????
{
// if (input.lastkey >= 'A' && input.lastkey < 'Z')
// {
	//printf("dispatch_trace %c\n",input.lastkey);
// (NS) ввод цифр жо тожы нужен в окне дизасма
   if ((input.lastkey >= '0' && input.lastkey <= '9') || (input.lastkey >= 'A' && input.lastkey <= 'Z'))
   {
       center();
       return 1;
   }
   return 0;
}
//=============================================================================
// FIND TEXT в окне дизасма			// в меню WNDTRACE дебагера
void cfindtext()				// и "cpu.findtext" хоткей
{
   Z80 &cpu = CpuMgr.Cpu();
   unsigned char oldmode = editor; editor = ED_MEM;
   int rs = find1dlg(cpu.trace_curs);
   editor = oldmode;
   if (rs != -1)
       cpu.trace_top = cpu.trace_curs = unsigned(rs);
}
//=============================================================================
// FIND CODE в окне дизасма			// в меню WNDTRACE дебагера
void cfindcode()				// и "cpu.findcode" хоткей
{
   Z80 &cpu = CpuMgr.Cpu();
   unsigned char oldmode = editor; editor = ED_MEM;
   int rs = find2dlg(cpu.trace_curs);
   editor = oldmode;
   if (rs != -1)
       cpu.trace_top = cpu.trace_curs = unsigned(rs);
}
//=============================================================================
// set breakpoint				// в меню WNDTRACE дебагера
void cbpx()					// и "cpu.bpx" хоткей
{			
    Z80 &cpu = CpuMgr.Cpu();
    cpu.membits[ cpu.trace_curs] ^= MEMBITS_BPX;
}
//=============================================================================v
void cbpr()					// в меню WNDTRACE дебагера
{						// [NS]
    Z80 &cpu = CpuMgr.Cpu();			
    cpu.membits[ cpu.trace_curs] ^= MEMBITS_BPR;
}
//=============================================================================
void cbpw()					// в меню WNDTRACE дебагера
{						//(NS)
    Z80 &cpu = CpuMgr.Cpu();			
    cpu.membits[cpu.trace_curs] ^= MEMBITS_BPW;
}
//=============================================================================






//=============================================================================
// GOTO PC					// в меню WNDTRACE дебагера
void cfindpc()					// и "cpu.findpc" хоткей
{
    //printf("cfindpc\n");
    Z80 &cpu = CpuMgr.Cpu();
    cpu.trace_top = cpu.trace_curs = cpu.pc;
    
//    cup();	и даже такая конструкция не работатет
//    cup();	хотя не работает только на цепочке разночитаемых опкодов?
//    cup();
//    cup();
//    cup();
//    cpu.trace_curs = cpu.pc;
					//нормально отступить с запасом не получаетсо
					//начальный адрес может попасть на пол команды
					//и и тогда адрес текущего PC не отобразитсо
}
//=============================================================================



//=============================================================================
// переход к адресу в окне дизасма					// [NS]
//    из окна дизасма

void mon_goto_disasm_addr( unsigned addr)				
{
//  unsigned addr = cpu.af;
    addr &= 0xFFFF;			// обрезка тк может быть фигня вышо
    Z80 &cpu = CpuMgr.Cpu();
    cpu.trace_top = cpu.trace_curs = addr;
}

void mon_goto_disasm_rAF() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.af);	}
void mon_goto_disasm_rBC() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.bc);	}
void mon_goto_disasm_rDE() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.de);	}
void mon_goto_disasm_rHL() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.hl);	}
                                  
void mon_goto_disasm_rAF1() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.alt.af);	}
void mon_goto_disasm_rBC1() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.alt.bc);	}
void mon_goto_disasm_rDE1() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.alt.de);	}
void mon_goto_disasm_rHL1() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.alt.hl);	}
                                  
void mon_goto_disasm_rIX() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.ix);	}
void mon_goto_disasm_rIY() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.iy);	}

void mon_goto_disasm_rSP() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.sp);	}
void mon_goto_disasm_rPC() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_goto_disasm_addr( cpu.pc);	}

//=============================================================================


//=============================================================================
// move cursor up (видимо в окне дизасма)			//"cpu.up" хоткей
void cup()					
{
    Z80 &cpu = CpuMgr.Cpu();
    //-------------------------------------------------------------------------
    // простой переход в середине экрана
    if (cpu.graph_trace_cursor_pos > 0)
    {
	cpu.graph_trace_cursor_pos--;
	cpu.trace_curs = cpu.trpc[ cpu.graph_trace_cursor_pos];
    }  
    //-------------------------------------------------------------------------
    // скролл экрана только с самой верхней позиции
    else
    {
	//---------------------------------------------------------------------
	// Простой переход
	if (cpu.trace_top > 0x0000)
	{
	    cpu.trace_top = cpu.trace_curs = cpu_up(cpu.trace_top);
	}
	//---------------------------------------------------------------------
	// Переход с 0x0000 к 0xFFFF
	else
	{
	//  cpu.trace_top = cpu.trace_curs = 0xFFFF;
	    cpu.trace_top = cpu.trace_curs = cpu_up(0x10000);
	}
	//---------------------------------------------------------------------
	cpu.graph_trace_cursor_pos = 0;
    }
    //-------------------------------------------------------------------------
    
    
  // перевод курсора в самый верх в любом случае
 //   cpu.trace_curs = cpu.trace_top;
 //   cpu.graph_trace_cursor_pos = 0;
    //-------------------------------------------------------------------------
//    printf("cpu.trpc -");
//    for (int tmp = 0; tmp <= trace_size; tmp++)
//    {
//        //printf("tmp,cpu.trpc[%04d] - %04X\n",tmp,cpu.trpc[tmp]);
//        printf(" %04X",cpu.trpc[tmp]);
//    }
//    printf("\n");
    //-------------------------------------------------------------------------
    
    cpu.trace_top &= 0xFFFF;
    cpu.trace_curs &= 0xFFFF; 
    
    //-------------------------------------------------------------------------
    return;
    //-------------------------------------------------------------------------








/*

   Z80 &cpu = CpuMgr.Cpu();








//   printf("cpu.trace_curs %d\n",cpu.trace_curs);
//   printf("cpu.trace_top %d\n",cpu.trace_top);   

    // printf("cup()\n");   


// может вообще не трогать cpu.trace_curs, cpu.trace_top
// а сразу вызывать follow PC
// хотя это наверно будет более ресурсоемко
//	а так можно будет лазить по дизасму с зажатым степом без потери курсора
//	или там аутостеп/трейс
//		наверное такое лучшо вынести на отдельную кнопку?

 
    signed int trace_top_1;
    //-------------------------------------------------------------------------
    if (cpu.trace_curs < 500)
    {
	//---------------------------------------------------------------------
	if (cpu.trace_top > 0xF000)
	{
	    printf("cpu.trace_top > 0xF000\n");
	    trace_top_1 = (cpu.trace_top - 0xFFFF) - 1;
	}
	//---------------------------------------------------------------------
	else
	{
	    printf("cpu.trace_top = normal\n");
	    trace_top_1 = cpu.trace_top;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    
    
    // когда cpu.trace_top = FFFF а cpu.trace_curs = 0000
    // условие не срабатывает
    // и топ-ом становитсо курсор со страшным дергосмыком
    
    //printf("cpu.trace_curs %x  cpu.trace_top %x trace_top_1 %d\n",cpu.trace_curs,cpu.trace_top,trace_top_1);

    signed int trace_cursor_1 = cpu.trace_curs;
    // почему то (cpu.trace_curs > trace_top_1) не дает true в случае (10 > -10) o_O
    // даже при -O0
    // но вынесено в отдельную переменную рабоотает 
    
    //-------------------------------------------------------------------------
//  if (cpu.trace_curs > cpu.trace_top)
    if (	(trace_cursor_1 > trace_top_1) &&
		(cpu.trace_curs != cpu.trace_top)	 //для переходаа вверх с верхней строки
      )    
    {
	//printf("(cpu.trace_curs > trace_top_1) \n");
	//---------------------------------------------------------------------
	
	
	for (unsigned i = 1; i < trace_size; i++)	//trace_size = 21
	{
	    //printf(" i %d\n",i);
	    if (cpu.trpc[i] == cpu.trace_curs)
	    {
		// printf(" cpu.trpc[i] == cpu.trace_curs\n",i);
		cpu.trace_curs = cpu.trpc[i - 1];
		cpu.graph_trace_cursor_pos = i - 1;
		//printf ("cpu.trpc[i-1] %d\n",cpu.trpc[i-1]);
	    }
	    //printf("i %d\n",i);
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    else
    {
	//переход выше самого верхнего положения
	//---------------------------------------------------------------------
	if (cpu.trace_curs != 0) // [NS]
	{
	    cpu.trace_top = cpu.trace_curs = cpu_up(cpu.trace_curs);
	    //printf ("1\n");
	}
	//---------------------------------------------------------------------
	else
	{
	    //переход с 0000 к FFFF (раньше упиралось и это дико бесило)
	    cpu.trace_top = cpu.trace_curs = cpu_up(0x10000);
	    //printf ("2\n");
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
	cpu.trace_top &= 0xFFFF;
	cpu.trace_curs &= 0xFFFF; 
 
 
 
 */
 
 
}
//=============================================================================
void cdown()						//"cpu.down" хоткей
{
    // printf("cdown()\n");
	
    Z80 &cpu = CpuMgr.Cpu();
    //-------------------------------------------------------------------------
    for (unsigned i = 0;    i < trace_size;    i++)
    {
	// printf(" i %d\n",i);
	//---------------------------------------------------------------------
	if (cpu.trpc[ i] == cpu.trace_curs)
	{
	    cpu.trace_curs = cpu.trpc[ i + 1];
	    cpu.graph_trace_cursor_pos = i + 1;
	    //-----------------------------------------------------------------
	    if (i + 1 == trace_size)
	    {
		// printf(" (i+1 == trace_size)\n",i);
		cpu.trace_top = cpu.trpc[ 1];
	    }
	    //-----------------------------------------------------------------
	    break;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
}
//=============================================================================
// видимо trace_mode 0,1,2 это адрес, опкод, дизасм ?
// что из них кто надо утошнять
//=============================================================================
void cleft()  { CpuMgr.Cpu().trace_mode--; }		// "cpu.left" хоткей
//=============================================================================
void cright() { CpuMgr.Cpu().trace_mode++; }		// "cpu.right" хоткей
//=============================================================================
void chere()	//trace to cursor????			// в меню WNDTRACE дебагера
{							// и "cpu.here" хоткей
    //printf("chere()\n");
    Z80 &cpu = CpuMgr.Cpu();
    cpu.dbgbreak = 0;
    dbgbreak = 0;
    cpu.dbgchk = 1;
 
    cpu.dbg_stophere = cpu.trace_curs;
}
//=============================================================================


//=============================================================================
void cpgdn()	//next page				// "cpu.pgdn" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
   
    
    // вначале переход к последней строке
    //-------------------------------------------------------------------------
    if (cpu.graph_trace_cursor_pos < (trace_size - 1))
    {
	cpu.trace_curs = cpu.trpc[ trace_size - 1];
	cpu.graph_trace_cursor_pos = trace_size - 1;
    }
    //-------------------------------------------------------------------------
    // Переход на след страницу только когда курсор в самом низу
    // по типу explorer.exe
    else	//if (cpu.graph_trace_cursor_pos == (trace_size - 1))
    {
        cpu.graph_trace_cursor_pos = trace_size - 1;
	//---------------------------------------------------------------------
	// упираемся в 0xFFFF
	if (cpu.trace_curs == 0xFFFF)
	{
	    cpu.trace_top = 0xFFFF - (trace_size - 1);
	    cpu.trace_curs = 0xFFFF;
	    return;
	}
	//---------------------------------------------------------------------
	// последние шаги уменьшенный и упираютсо в FFFF
	if ((cpu.trace_top < (0xFFFF - (trace_size - 1))) && (cpu.trace_top > (0xFFFF - (2 * (trace_size - 1)) - 1)))
	{
	    //
	    //FFEA_FFFE first -> FFEB_FFFF
	    //FFD8_FFEC
	    //FFD7_FFEB last  -> FFEB_FFFF
	    cpu.trace_top = 0xFFFF - (trace_size - 1);
	    cpu.trace_curs = 0xFFFF;
	}
	//---------------------------------------------------------------------
	// FFEC...FFFF нормальный pddown
	else
	{
	    cpu.trace_top = cpu.trpc[ trace_size];
	}
    }
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------    



	

	//	//-------------------------------------------------------------
	//	printf("cpu.trpc -");
	//	for (int tmp = 0; tmp <= trace_size; tmp++)
	//	{
	//	    //printf("tmp,cpu.trpc[%04d] - %04X\n",tmp,cpu.trpc[tmp]);
	//	    printf(" %04X",cpu.trpc[tmp]);
	//	}
	//	printf("\n");
	//	//-------------------------------------------------------------
   return;
   
   
   
   
   
   
   
   
   
/*		
    Z80 &cpu = CpuMgr.Cpu();
    unsigned curs = 0;
    //-------------------------------------------------------------------------
    for (unsigned i = 0;    i < trace_size;    i++)
	if (cpu.trace_curs == cpu.trpc[i])
	    curs = i;
    //-------------------------------------------------------------------------
    cpu.trace_top = cpu.trpc[trace_size];
    showtrace();
    cpu.trace_curs = cpu.trpc[curs];
  */  
    
}
//=============================================================================


//=============================================================================
void cpgup()		//prev page			// "cpu.pgup" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    //-------------------------------------------------------------------------
    // Переход на пред страницу только когда курсор в самом верху
    // по типу explorer.exe
    if (cpu.graph_trace_cursor_pos == 0)
    {
	unsigned i; 		//Alone Coder 0.36.7
	//---------------------------------------------------------------------
	// тупо повторяем 20 раз :rofl:
	for (i = 0;    i < trace_size;    i++)
	    cpu.trace_top = cpu_up( cpu.trace_top);
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    // перевод курсора в самый верх в любом случае
    cpu.trace_curs = cpu.trace_top;
    cpu.graph_trace_cursor_pos = 0;
    //-------------------------------------------------------------------------
//    printf("cpu.trpc -");
//    for (int tmp = 0; tmp <= trace_size; tmp++)
//    {
//        //printf("tmp,cpu.trpc[%04d] - %04X\n",tmp,cpu.trpc[tmp]);
//        printf(" %04X",cpu.trpc[tmp]);
//    }
//    printf("\n");
    //-------------------------------------------------------------------------
    return;
    //-------------------------------------------------------------------------



//    оригинальный код и ущербный и туго отлаживаемый
//    unsigned curs = 0;
//    unsigned i; 		//Alone Coder 0.36.7
//    //-------------------------------------------------------------------------
//    for (/*unsigned*/ i = 0;    i < trace_size;    i++)
//	if (cpu.trace_curs == cpu.trpc[i])
//	    curs = i;
//    //-------------------------------------------------------------------------
//    for (i = 0;    i < trace_size;    i++)
//	cpu.trace_top = cpu_up(cpu.trace_top);
//    //-------------------------------------------------------------------------
//    showtrace();
//    cpu.trace_curs = cpu.trpc[curs];
//		//-------------------------------------------------------------
//		printf("cpu.trpc -");
//		for (int tmp = 0; tmp <= trace_size; tmp++)
//		{
//		    //printf("tmp,cpu.trpc[%04d] - %04X\n",tmp,cpu.trpc[tmp]);
//		    printf(" %04X",cpu.trpc[tmp]);
//		}
//		printf("\n");
//		//-------------------------------------------------------------


}
//=============================================================================


//=============================================================================
// pop cursor position from jumpstack			// в меню WNDTRACE дебагера
void pop_pos()	//IDA mode				// и "cpu.back" хоткей
{		// back from "goto to branch destination"
		// на самом деле это Return to previous possition
		// тк переход по адресу при вводе кидает адрес в стек
   Z80 &cpu = CpuMgr.Cpu();
   if (stack_pos[0] == -1U)
       return;
   cpu.trace_curs = stack_cur[0];
   cpu.trace_top = stack_pos[0];
   memcpy(&stack_pos[0], &stack_pos[1], sizeof stack_pos - sizeof *stack_pos);
   memcpy(&stack_cur[0], &stack_cur[1], sizeof stack_cur - sizeof *stack_cur);
   stack_pos[(sizeof stack_pos / sizeof *stack_pos)-1] = -1U;
}
//=============================================================================



//=============================================================================
// Goto Operand Address				ZX/GS - ok
// push cursor position and goto instruction operand	// в меню WNDTRACE дебагера
void cjump() 	// IDA mode				// "cpu.context" хоткей
{		// переход на адрес операнда !!!
    Z80 &cpu = CpuMgr.Cpu();
    char *ptr = nullptr;
    //-------------------------------------------------------------------------
    for (char *p = asmpc;    *p;    p++)
      if (ishex(p[0]) & ishex(p[1]) & ishex(p[2]) & ishex(p[3])) ptr = p;
    //-------------------------------------------------------------------------
    if (!ptr) 
	return;
    //-------------------------------------------------------------------------
    push_pos();
    unsigned addr;
    sscanf( ptr, "%04X", &addr);
    cpu.trace_curs = cpu.trace_top = addr;
}
//=============================================================================



//=============================================================================
// View Operand Address				ZX/GS - ok
// jump to instruction operand in data window		// в меню WNDTRACE дебагера
void cdjump()						// и "cpu.datajump" хоткей
{							
    char *ptr = nullptr;
    //-------------------------------------------------------------------------
    for (char *p = asmpc;    *p;    p++)
	if (ishex( p[0]) & ishex( p[1]) & ishex( p[2]) & ishex( p[3]))
	    ptr = p;
    //-------------------------------------------------------------------------
    if (!ptr) 
	return;
    //-------------------------------------------------------------------------
    unsigned addr;
    sscanf( ptr, "%04X", &addr);
    Z80 &cpu = CpuMgr.Cpu();
    cpu.mem_curs = addr;
    activedbg = WNDMEM;
    editor = ED_MEM;
}
//=============================================================================



		
		
//=============================================================================
// переход к адресу в окне мемори виевера	ZX/GS - ok		// [NS]
// из окна дизасма
void mon_view_mem_addr( unsigned addr)					
{
//  unsigned addr = cpu.af;
    addr &= 0xFFFF;	// обрезка тк может быть фигня
    Z80 &cpu = CpuMgr.Cpu();
    cpu.mem_curs = addr;
    activedbg = WNDMEM;
    editor = ED_MEM;
}

void mon_view_mem_rAF() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.af);		}
void mon_view_mem_rBC() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.bc);		}
void mon_view_mem_rDE() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.de);		}
void mon_view_mem_rHL() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.hl);		}

void mon_view_mem_rAF1()	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.alt.af);	}
void mon_view_mem_rBC1()	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.alt.bc);	}
void mon_view_mem_rDE1()	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.alt.de);	}
void mon_view_mem_rHL1()	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.alt.hl);	}

void mon_view_mem_rIX() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.ix);		}
void mon_view_mem_rIY() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.iy);		}

void mon_view_mem_rSP() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.sp);		}
void mon_view_mem_rPC() 	{ Z80 &cpu = CpuMgr.Cpu();	mon_view_mem_addr( cpu.pc);		}

//=============================================================================


//=============================================================================
// показать/скрыть labels
void cfliplabels()				// в меню WNDTRACE дебагера
{						// "cpu.labels" хоткей
   trace_labels = !trace_labels; showtrace();
}
//=============================================================================
// save cursor position to slot n
static void csave(unsigned n)		//"cpu.save1"..."cpu.save8" хоткеи
{
   Z80 &cpu = CpuMgr.Cpu();
   save_pos[n] = cpu.trace_top;
   save_cur[n] = cpu.trace_curs;
}
//=============================================================================
// save cursor position to jumpstack, load from slot 1
static void crest(unsigned n)		//"cpu.rest1"..."cpu.rest8" хоткеи
{
   Z80 &cpu = CpuMgr.Cpu();
   if (save_pos[n] == -1U)
       return;
   push_pos();
   cpu.trace_top = save_pos[n];
   cpu.trace_curs = save_cur[n];
}
//=============================================================================
void csave1() { csave(0); }
void csave2() { csave(1); }
void csave3() { csave(2); }
void csave4() { csave(3); }
void csave5() { csave(4); }
void csave6() { csave(5); }
void csave7() { csave(6); }
void csave8() { csave(7); }
//=============================================================================
void crest1() { crest(0); }
void crest2() { crest(1); }
void crest3() { crest(2); }
void crest4() { crest(3); }
void crest5() { crest(4); }
void crest6() { crest(5); }
void crest7() { crest(6); }
void crest8() { crest(7); }
//=============================================================================



//=============================================================================
namespace z80dbg						//????
{
    void __cdecl SetLastT()
    {
	cpu.debug_last_t = comp.t_states + cpu.t;
    }
}
//=============================================================================



float zx_step_dither = 0.0;
float gs_step_dither = 0.0;

float zx_tact_dither = 0.0;
float gs_tact_dither = 0.0;


//=============================================================================
//бывший void mon_step()
void mon_step_single_cpu()
{
    Z80 &cpu = CpuMgr.Cpu();
    TZ80State &prevcpu = CpuMgr.PrevCpu();
   
    cpu.SetLastT();
    prevcpu = cpu;
    // CpuMgr.CopyToPrev();
    
    //-------------------------------------------------------------------------
    if (cpu.t >= conf.intlen)
       cpu.int_pend = false;
    //-------------------------------------------------------------------------
    cpu.Step();
    //-------------------------------------------------------------------------
    if (  (cpu.int_pend)	&& 
	  (cpu.iff1)		&& 
	  (cpu.t != cpu.eipos)	&&	// int enabled in CPU not issued after EI
	  (cpu.int_gate) 		// int enabled by ATM hardware
     ) 
    {
	handle_int( &cpu, cpu.IntVec());
    }
    //-------------------------------------------------------------------------
    cpu.CheckNextFrame();
    //-------------------------------------------------------------------------
    if (conf.trace_follow_regs)						// [NS]
    {
	//---------------------------------------------------------------------
	switch (conf.trace_follow_regs)
	{
	    case REG_AF:	cpu.trace_curs = cpu.af;	break;
	    case REG_BC:	cpu.trace_curs = cpu.bc;	break;
	    case REG_DE:	cpu.trace_curs = cpu.de;	break;
	    case REG_HL:	cpu.trace_curs = cpu.hl;	break;
	    case REG_AF1:	cpu.trace_curs = cpu.alt.af;	break;
	    case REG_BC1:	cpu.trace_curs = cpu.alt.bc;	break;
	    case REG_DE1:	cpu.trace_curs = cpu.alt.de;	break;
	    case REG_HL1:	cpu.trace_curs = cpu.alt.hl;	break;
	    case REG_IX:	cpu.trace_curs = cpu.ix;	break;
	    case REG_IY:	cpu.trace_curs = cpu.iy;	break;
	    case REG_SP:	cpu.trace_curs = cpu.sp;	break;
	    case REG_PC:	cpu.trace_curs = cpu.pc;	break;
	}
	//---------------------------------------------------------------------	
	conf.trace_follow_request = 1;	// следовать за чем нибудь	
    }
    //-------------------------------------------------------------------------
    // тут нужно несколько раз вызывать step для GS
    // или может даже брать такты
    // и щитать сколько каждого должно быть вызвано
    
}
//=============================================================================
// ZX+GS Step								// [NS]
void mon_step()								
{
    //=========================================================================
    if ( (conf.gs_type == 1)	&&	// GS Z80
	 (conf.ZXGS_Step == 1)		// включенность совместного степа
     )
    {
	Z80 &cpu = CpuMgr.Cpu();

	// NO STATIC !!!!
	int current_cpu_back = CpuMgr.GetCurrentCpu();
    
	float zx_cpu_frq = frametime * conf.intfq;
	//printf("ZX CPU %f\n",zx_cpu_frq);
    
	//=====================================================================
	// ZX Step
	if (current_cpu_back == 0)
	{
	    //printf("ZX_STEP\n");
	    float gs_tacts_per_1_zx = (float) z80gs::GSCPUFQ / zx_cpu_frq;
	    
	    mon_step_single_cpu();	// 1 ZX Step
	    //printf("cpu.Delta() %d\n",cpu.Delta());
	    gs_tact_dither += gs_tacts_per_1_zx * (float) cpu.Delta();
	    //printf("gs_tact_dither %f\n",gs_tact_dither);
	    //-----------------------------------------------------------------
	    // Возможна раздача GS ошибки
	    if (gs_tact_dither > 4.0)	// пушо меньше 4 тактов невозможно
	    {
		//CpuMgr.SwitchCpu();
		CpuMgr.SetCurrentCpu( 1);	// GS CPU
		Z80 &cpu = CpuMgr.Cpu();
		//-------------------------------------------------------------
		// GS Step
		while (gs_tact_dither > 4.0)
		{
		    //printf("	gs_step\n");
		    mon_step_single_cpu();
		    //printf("	cpu.Delta() %d\n",cpu.Delta());
		    gs_tact_dither -= (float) cpu.Delta();
		    //printf("	gs_tact_dither %f\n",gs_tact_dither);
		}
		//-------------------------------------------------------------
		//CpuMgr.SwitchCpu();
	    }
	    //-----------------------------------------------------------------
	} // ZX Step
	//=====================================================================
	// GS Step
	else
	{
	    //printf("GS_STEP\n");
	    float zx_tacts_per_1_gs = zx_cpu_frq / (float) z80gs::GSCPUFQ; 
	    
	    mon_step_single_cpu();	// 1 GS Step
	    //printf("cpu.Delta() %d\n",cpu.Delta());
	    zx_tact_dither += zx_tacts_per_1_gs * (float) cpu.Delta();
	    //printf("zx_tact_dither %f\n",zx_tact_dither);
	    //-----------------------------------------------------------------
	    // Возможна раздача ZX ошибки
	    if (zx_tact_dither > 4.0)	// пушо меньше 4 тактов невозможно
	    {
		//CpuMgr.SwitchCpu();
		CpuMgr.SetCurrentCpu( 0);	// ZX CPU
		Z80 &cpu = CpuMgr.Cpu();
		//-------------------------------------------------------------
		// ZX Step
		while (zx_tact_dither > 4.0)
		{
		    //printf("	zx_step\n");
		    mon_step_single_cpu();
		    //printf("	zx cpu.Delta() %d\n",cpu.Delta());
		    zx_tact_dither -= (float) cpu.Delta();
		    //printf("	zx_tact_dither %f\n",zx_tact_dither);
		}
		//-------------------------------------------------------------
		//CpuMgr.SwitchCpu();
	    }
	    //-----------------------------------------------------------------
	} // GS Step
	//=====================================================================
	// Востановление исходного CPU
	CpuMgr.SetCurrentCpu( current_cpu_back);
    }
    //=========================================================================
    // не шагать "одновременно"
    else
    {
	mon_step_single_cpu();
    }
    //=========================================================================
    
/*



    // NO STATIC !!!!
    int current_cpu_back = CpuMgr.GetCurrentCpu();
    //printf("cpu %x\n",current_cpu_back);
    
   //     const int GSCPUFQ = 24000000;		// hz //12
   // extern const unsigned GSCPUINT;
    // GSINTFQ
    
	//conf.frame
	//cpu.tpi //zx?
	//printf("GSINTFQ %d\n",GSINTFQ);
	//printf("GSCPUINT %d\n",GSCPUINT);
	//printf("GSCPUFQ %d\n",z80gs::GSCPUFQ);
	//printf("frametime %d\n",frametime);
	//printf("conf.intfq %d\n",conf.intfq);
	float zx_cpu_frq = frametime * conf.intfq;
	//printf("ZX CPU %f\n",zx_cpu_frq);
	float gs_steps_per_1_zx = (float) z80gs::GSCPUFQ / zx_cpu_frq;
	float zx_steps_per_1_gs = zx_cpu_frq / (float) z80gs::GSCPUFQ;
	//printf("gs_steps_per_1_zx %f\n",gs_steps_per_1_zx);
	//printf("zx_steps_per_1_gs %f\n",zx_steps_per_1_gs);
	
    // STEP-ы не учитывают такты !!!
    // нужно делать с учетом тактов за последний степ
    //=========================================================================
    // ZX Step
    if (current_cpu_back == 0)
    {
	printf("ZX_STEP\n");
	mon_step_single_cpu();	// 1 ZX Step

	gs_step_dither += gs_steps_per_1_zx;
	//printf("gs_step_dither %f\n",gs_step_dither);
	//---------------------------------------------------------------------
	
	
	if (gs_step_dither > 1.0)
	{
	    CpuMgr.SwitchCpu();
	    while (gs_step_dither > 1.0)
	    {
		gs_step_dither -= 1.0;
		printf("gs_step\n");
		mon_step_single_cpu();
	    }
	    CpuMgr.SwitchCpu();
	}
	//---------------------------------------------------------------------
	
    }
    //=========================================================================
    // GS Step
    else
    {
	printf("GS_STEP\n");
	mon_step_single_cpu();	// 1 GS Step
	
	zx_step_dither += zx_steps_per_1_gs;
	//printf("zx_step_dither %f\n",zx_step_dither);
	//---------------------------------------------------------------------
	if (zx_step_dither > 1.0)
	{
	    CpuMgr.SwitchCpu();
	    while (zx_step_dither > 1.0)
	    {
		zx_step_dither -= 1.0;
		printf("zx_step\n");
		mon_step_single_cpu();
	    }
	    CpuMgr.SwitchCpu();
	}
	//---------------------------------------------------------------------
	
    }
    //=========================================================================
    
    
    
    CpuMgr.SetCurrentCpu( current_cpu_back);
    
    */
}
//=============================================================================




//=============================================================================
void mon_step_x( int count)
{
    mon_step();
    //-------------------------------------------------------------------------
    for (int temp_cnt = 0;    temp_cnt < count;    temp_cnt++)
    {
	debugscr();		//а почему не после?
	debugflip();
	
	mon_step();
    }
    //-------------------------------------------------------------------------
}
//=============================================================================
void mon_step_x2() {	mon_step_x(   1); }
void mon_step_x4() {	mon_step_x(   3); }
void mon_step_x8() {	mon_step_x(   7); }
void mon_step_x16() {	mon_step_x(  15); }
void mon_step_x32() {	mon_step_x(  31); }
void mon_step_x64() {	mon_step_x(  63); }
void mon_step_x128() {	mon_step_x( 127); }
void mon_step_x256() {	mon_step_x( 255); }
//=============================================================================


//#define TWF_BRANCH  0x010000U
//#define TWF_BRADDR  0x020000U
//#define TWF_LOOPCMD 0x040000U
//#define TWF_CALLCMD 0x080000U
//#define TWF_BLKCMD  0x100000U
//#define TWF_HALTCMD 0x200000U

//=============================================================================
void mon_stepover_flags( int flags)
{
    Z80 &cpu = CpuMgr.Cpu();
    unsigned char trace = 1;
    //-------------------------------------------------------------------------
    // call,rst
    if (cpu.pc_trflags & TWF_CALLCMD)
    {
	cpu.dbg_stopsp = cpu.sp & 0xFFFF;	//??????
	cpu.dbg_stophere = cpu.nextpc;
	trace = 0;
    }
    //-------------------------------------------------------------------------
//  else if (cpu.pc_trflags & (TWF_BLKCMD | TWF_HALTCMD)) 
    else if (cpu.pc_trflags & (flags))					// [NS]	     
    {
	trace = 0;
	cpu.dbg_stophere = cpu.nextpc;
    }
    //-------------------------------------------------------------------------  
/* [vv]
    // jr cc,$-xx, jp cc,$-xx
    else if ((cpu.pc_trflags & TWF_LOOPCMD) && (cpu.pc_trflags & 0xFFFF) < (cpu.pc & 0xFFFF))
    {
	cpu.dbg_stopsp = cpu.sp & 0xFFFF;
	cpu.dbg_stophere = cpu.nextpc,
	cpu.dbg_loop_r1 = cpu.pc_trflags & 0xFFFF;
	cpu.dbg_loop_r2 = cpu.pc & 0xFFFF;
	trace = 0;
    }
*/
    //-------------------------------------------------------------------------
/*  [vv]
    else if (cpu.pc_trflags & TWF_BRANCH)
	trace = 1;
    else
    {
	trace = 1;
	cpu.dbg_stophere = cpu.nextpc;
    }
*/
    //-------------------------------------------------------------------------
    //step over (step) ?
    if (trace)
    {
	mon_step();
    }
    //-------------------------------------------------------------------------
    //step over (call skip)
    else
    {
	cpu.dbgbreak = 0;
	dbgbreak = 0;
	cpu.dbgchk = 1;
    }
    //-------------------------------------------------------------------------
}
//=============================================================================
void mon_stepover()							// [NS]
{
// ldir/lddr|cpir/cpdr|otir/otdr|inir/indr|halt

    mon_stepover_flags(		TWF_BLKCMD	|
				TWF_HALTCMD
			);
}
//=============================================================================
void mon_stepover_jump()						// [NS]
{
// так же проскакивает jr cc jp cc djnz
// НО некоторые условия очень редкие
// и trace over может ВНЕЗАПНО превратитсо в run
// поэтому отдельной кнопкой

    mon_stepover_flags(		TWF_BLKCMD	|
				TWF_HALTCMD	|
				TWF_LOOPCMD
			);
}
//=============================================================================
