#include "std.h"

#include "emul.h"
#include "vars.h"
#include "draw.h"
#include "dx.h"
#include "debug.h"
#include "dbgpaint.h"
#include "dbgreg.h"
#include "dbgtrace.h"
#include "dbgmem.h"
#include "dbgoth.h"
#include "dbglabls.h"
#include "dbgbpx.h"

#include "util.h"

#include "resource.h"	// [NS] для менюшек	
#include "dbgrwdlg.h"	// [NS] для вызова block load/save/fill
#include "dbgcmd.h"	// [NS] memory dump toggle - конфликтовало!
#include "z80asm.h"	// [NS] для смены стиля дизасма
#include "emulkeys.h"	// [NS] для отключения захвата мыши

#ifdef MOD_MONITOR

unsigned char trace_labels;

unsigned show_scrshot;
unsigned user_watches[3] = { 0x4000, 0x8000, 0xC000 };

unsigned mem_sz = 8;

unsigned mem_disk;
unsigned mem_track;
unsigned mem_max;

unsigned char mem_ascii;
unsigned char mem_dump;
unsigned char editor = ED_MEM;

unsigned regs_curs;
unsigned dbg_extport;
unsigned char dbg_extval;	// extended memory port like 1FFD or DFFD

unsigned ripper;		// ripper mode (none/read/write)

int dbg_force_exit;		// для костыльного выхода из дебагера	[NS]
				// без дополнительного аникея
			
DBGWND activedbg = WNDTRACE;

void debugscr();

//unsigned find1dlg(unsigned start);	//наверно	- удалено тк конфликтует
//unsigned find2dlg(unsigned start);	//рудименты	- [NS]

/*
#include "dbgpaint.cpp"
#include "dbglabls.cpp"
#include "z80asm.cpp"
#include "dbgreg.cpp"
#include "dbgmem.cpp"
#include "dbgtrace.cpp"
#include "dbgrwdlg.cpp"
#include "dbgcmd.cpp"
#include "dbgbpx.cpp"
#include "dbgoth.cpp"
*/

//		paper ink
//		Bgrb Bgrb
//=============================================================================
void debugscr()
{

#define	DBG_ATTR_PANEL	0x50	


   //заливка фона шахматкой
   memset(	txtscr,
		' ',			//BACKGR_CH,	//символ шахматки на фоне дебагера
		sizeof txtscr/2
	  );
   memset(	txtscr+sizeof txtscr/2,
		DBG_ATTR_PANEL,		//BACKGR,	//атрибут шахматки
		sizeof txtscr/2
	  );
   nfr = 0;

    showregs();		// окно регистров	chkd
    showtrace();	// окно дизасма		chkd
    showmem();		// окно мемори виевера	chkd
    showwatch();
    showstack();
    show_ay();
    showbanks();
    showports();
    showdos();

    show_time();	//time delta внизу экрана
    
//#if 0
   
//#else	// доисторическая рамочка с названием ебулятора вместо
//   tprint(copy_x, copy_y, "\x1A", 0x9C);
//   tprint(copy_x+1, copy_y, "UnrealSpeccy " VERS_STRING, 0x9E);
//   tprint(copy_x+20, copy_y, "by SMT", 0x9D);
//   tprint(copy_x+26, copy_y, "\x1B", 0x9C);
//   frame(copy_x, copy_y, 27, 1, 0x0A);
//#endif
}
//=============================================================================


//=============================================================================
static void handle_mouse()
{
    Z80 &cpu = CpuMgr.Cpu();
		//printf("mousepos %x\n",mousepos);
		//printf("temp.gx %x\n",temp.gx);
		//printf("temp.gy %x\n",temp.gy);
                //
		//printf("temp.ox %d\n",temp.ox);
		//printf("temp.oy %d\n",temp.oy);

// unsigned	mx = ((mousepos & 0xFFFF) - temp.gx) / 8;		//orig
// unsigned	my = (((mousepos >> 16) & 0x7FFF) - temp.gy) / 16;


// temp.ox НЕ ФАКТИЧЕСКИЙ РАЗМЕР !!!!

    RECT current_mainwnd_size;
    GetClientRect( wnd, &current_mainwnd_size);
    int real_dbg_wnd_size_x = current_mainwnd_size.right - current_mainwnd_size.left;
    int real_dbg_wnd_size_y = current_mainwnd_size.bottom - current_mainwnd_size.top;
    // real_dbg_wnd_size_y вынести в отдельную переменную
    // чтобы не делать GetClientRect !!!
    // new_wnd_size_x другое имя
    
//	printf("temp.gx %d\n",temp.gx);
//	printf("temp.gy %d\n",temp.gy);
//    	printf("real_dbg_wnd_size_x %d\n", real_dbg_wnd_size_x);
//	printf("real_dbg_wnd_size_y %d\n", real_dbg_wnd_size_y);
//коректируем координаты любого окна до дебагерных 640х480		[NS]
//    unsigned	mx = (float) (( mousepos & 0xFFFF) - temp.gx) *		 ((float) (s640) / (float) (real_dbg_wnd_size_x)) ;
//		mx /= 8;	//размер шрифта
//    unsigned	my = (float) (((( mousepos >> 16) & 0x7FFF) - temp.gy)) * ((float) (s480) / (float) (real_dbg_wnd_size_y)) ;
//		my /= 16;	//размер шрифта

// temp.gx наверное уже не нужен (мешает вычислять в растянтомм режиме)
// это наверно осталось от центрования дебагера поо центру
    unsigned	mx = (float) (( mousepos & 0xFFFF)) *		((float) (s640) / (float) (real_dbg_wnd_size_x)) ;
		mx /= 8;	//размер шрифта
    unsigned	my = (float) (((( mousepos >> 16) & 0x7FFF))) * ((float) (s480) / (float) (real_dbg_wnd_size_y)) ;
		my /= 16;	//размер шрифта

//	printf("mx %d\n",mx);
//	printf("my %d\n",my);




// переписать старые адреса координат на дефайны из debug.h

    // может лучшо вообще организовать стенсил буфер?
    // и в месте с отрисовкой графики (в том числе двигабелльной)
    // его заполнять
    //-------------------------------------------------------------------------
    // Ports window					// [NS]
    if (	(my >= ports_y)			&&
		(my < (ports_y + ports_cy) )	&&
		(mx >= ports_x)			&&
		(mx < (ports_x + ports_cx) )
     )
    {
	activedbg = WND_PORTS;
	needclr++;
	//printf("ports window detect\n");
    }
    //-------------------------------------------------------------------------
    // Stack window					// [NS]
    if (	(my >= stack_y)			&&
		(my < (stack_y + stack_cy) )	&&
		(mx >= stack_x)			&&
		(mx < (stack_x + stack_cx) )
      )
    {
	activedbg = WND_STACK;
	needclr++;
	//printf("WND_STACK window detect\n");
    }
    //-------------------------------------------------------------------------
    // AY window					// [NS]
    if (	(my >= ay_y)		&&
		(my < (ay_y + ay_cy) )	&&
		(mx >= ay_x)		&&
		(mx < (ay_x + ay_cx) )
     )
    {
	activedbg = WND_AY;
	needclr++; 
	//printf("WND_AY window detect\n");
    }
    //-------------------------------------------------------------------------
    // Time Delta					// [NS]
    if (	(my >= time_y)			&&
		(my < (time_y + time_cy) )	&&
		(mx >= time_x)			&&
		(mx < (time_x + time_cx) )
      )
    {
	activedbg = WND_TIME_DELTA;
	needclr++;
	//printf("WND_TIME_DELTA window detect\n");
    }
    //-------------------------------------------------------------------------
    // Pages						// [NS]
    if (	(my >= banks_y)			&&
		(my < (banks_y + banks_cy) )	&&
		(mx >= banks_x)			&&
		(mx < (banks_x + banks_cx) )
     )
    {
	activedbg = WND_PAGES;
	needclr++;
	//printf("WND_PAGES window detect\n");
    }
    //-------------------------------------------------------------------------
    // Beta 128						// [NS]
    if (	(my >= dos_y)			&&
		(my < (dos_y + dos_cy) )	&&
		(mx >= dos_x)			&&
		(mx < (dos_x + dos_cx) )
     )
    {
	activedbg = WND_BETA128;
	needclr++;
	//printf("WND_BETA128 window detect\n");
    }
    //-------------------------------------------------------------------------
    // Watches						// [NS]
    if (	(my >= wat_y)			&&
		(my < (wat_y + wat_cy) )	&&
		(mx >= wat_x)			&&
		(mx < (wat_x + wat_cx) )
     )
    {
	activedbg = WND_WATCHES;
	needclr++;
	//printf("WND_WATCHES window detect\n");
    }
    //-------------------------------------------------------------------------
    //  Trace window
    if (	(my >= trace_y)			&&
		(my < (trace_y + trace_size))	&&
		(mx >= trace_x)			&&
		(mx < trace_x + 32)			// magic number !!!!!
     )
    {
        activedbg = WNDTRACE;
	needclr++; 
	//printf("trace window detect\n");
	
        cpu.trace_curs = cpu.trpc[ my - trace_y];
	//printf("cpu.trace_curs %X\n",cpu.trace_curs);
        //---------------------------------------------------------------------
	if	(mx - trace_x < cs[1][0])	cpu.trace_mode = 0;	// addr
        //---------------------------------------------------------------------
	else if	(mx - trace_x < cs[2][0])	cpu.trace_mode = 1;	// dump/labels
        //---------------------------------------------------------------------
	else					cpu.trace_mode = 2;	// disasm
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    //  Memory window
    if (	(my >= mem_y)			&&
		(my < (mem_y + mem_size))	&&
		(mx >= mem_x)			&&
		(mx < (mem_x + 37))			// magic number !!!!
     )
    {
        activedbg = WNDMEM;
	needclr++; 
	//printf("mem window detect\n");	

        unsigned dx = mx - mem_x;
	
	//printf("my %d  mx %d  dx %d\n",my,mx,dx);
	//---------------------------------------------------------------------
	//тыкание по адресу
	if (dx < 5)
	{
	    //-----------------------------------------------------------------
	    if (mem_dump)
	    {
		cpu.mem_curs = cpu.mem_top + (my - mem_y) * 32;
	    }
	    //-----------------------------------------------------------------
	    else
	    {
		cpu.mem_curs = cpu.mem_top + (my - mem_y) * 8;
	    }
	    //-----------------------------------------------------------------
	    cpu.mem_addr_edit_mode = 1;
	}
	//---------------------------------------------------------------------
	// тыкание по дампу
	// выдает конкретный адрес куда ткнуто курсором
	//
	// ascii dump  
	if (mem_dump)	
	{ 
            //-----------------------------------------------------------------
	    if (dx >= 5)
	    {
                cpu.mem_curs = cpu.mem_top + (dx - 5) + (my - mem_y) * 32;
		cpu.mem_addr_edit_mode = 0;
	    }
	    //-----------------------------------------------------------------
        }
	//---------------------------------------------------------------------
	// hex+ascii dump
	else	
        {
            unsigned mem_se = (dx - 5) % 3;
            //-----------------------------------------------------------------
	    // ascii
	    if (dx >= 29)
            {
                cpu.mem_curs = cpu.mem_top + (dx - 29) + (my - mem_y) * 8;
                mem_ascii = 1;
		cpu.mem_addr_edit_mode = 0;
            }
            //-----------------------------------------------------------------
	    // hex
	    if (	(dx >= 5)	&&
			(mem_se != 2)	&&
			(dx < 29)
	     )
            {
                cpu.mem_curs = cpu.mem_top + (dx - 5) / 3 + (my - mem_y) * 8;
                cpu.mem_second = mem_se;
                mem_ascii = 0;
		cpu.mem_addr_edit_mode = 0;
            }
	    //-----------------------------------------------------------------
        }
	//---------------------------------------------------------------------
	//printf("cpu.mem_curs %x\n",cpu.mem_curs);
    }
    //-------------------------------------------------------------------------
    // Regs window
    if (	(mx >= regs_x)		&&
		(my >= regs_y)		&&
		(mx < (regs_x + 32))	&&
		(my < (regs_y + 4))
     )
    {
        activedbg = WNDREGS;
	needclr++; 
	//printf("regs window detect\n");
        //---------------------------------------------------------------------
	// определение по какому regs_layout номеру регистра тыкнуто
	for (unsigned i = 0;    i < regs_layout_count;    i++)
        {
            unsigned delta = 1;	// дефолтная ширины регов для детекции
	    //-----------------------------------------------------------------
	    if (regs_layout[i].width == DEC_6T)	delta = 6;	// [NS] для тактов
            //-----------------------------------------------------------------
	    if (regs_layout[i].width == HEX_16)	delta = 4;
	    //-----------------------------------------------------------------
            if (regs_layout[i].width == HEX_8)	delta = 2;
	    //-----------------------------------------------------------------
            if (   (my - regs_y == regs_layout[i].y)		&&
		   ((mx - regs_x - regs_layout[i].x) < delta)
	     )
	    {
		regs_curs = i;
	    }
	    //-----------------------------------------------------------------
        }
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    //printf("regs_curs %d\n",regs_curs);



    




    //=========================================================================
    // Right click
    if (mousepos & 0x80000000)
    {
	//---------------------------------------------------------------------
	// костыль [NS]			// форсируем перерисовку дебагера
	debugscr();	 //рисовать	// чтобы активное окно выделилось раньше
	debugflip(); //перекидывать	// чем всплывет контексное меню иначе
	//needclr--;			// выделение только после закрытия контексного меню!!!	
	//---------------------------------------------------------------------

	// enum { IDM_MON_BPX = 1, IDM_SOME_OTHER };	// ШОЗАХНЯ??? О_о  !!!!!!!

	int temp_mf_flag;
	
	//=====================================================================
	// WNDTRACE - View - sub menu
	//=====================================================================
	HMENU wnd_trace_view_sub_menu = CreatePopupMenu();
	
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_AF	 ,	"AF"	);
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_BC	 ,	"BC"	);
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_DE	 ,	"DE"	);
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_HL	 ,	"HL"	);
	AppendMenu( wnd_trace_view_sub_menu, MF_SEPARATOR, 0, nullptr); //----------------------
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_AF1 ,	"AF\'"	);
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_BC1 ,	"BC\'"	);
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_DE1 ,	"DE\'"	);	
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_HL1 ,	"HL\'"	);
	AppendMenu( wnd_trace_view_sub_menu, MF_SEPARATOR, 0, nullptr); //----------------------
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_IX	 ,	"IX"	);
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_IY	 ,	"IY"	);
	AppendMenu( wnd_trace_view_sub_menu, MF_SEPARATOR, 0, nullptr); //----------------------
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_SP	 ,	"SP"	);
	AppendMenu( wnd_trace_view_sub_menu, MF_STRING, IDM_MON_VIEW_PC	 ,	"PC"	);
	//=====================================================================
	// WNDTRACE - Goto - sub menu
	//=====================================================================
	HMENU wnd_trace_goto_sub_menu = CreatePopupMenu();
	
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_AF	 ,	"AF"	);
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_BC	 ,	"BC"	);
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_DE	 ,	"DE"	);
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_HL	 ,	"HL"	);
	AppendMenu( wnd_trace_goto_sub_menu, MF_SEPARATOR, 0, nullptr); //----------------------
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_AF1 ,	"AF\'"	);
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_BC1 ,	"BC\'"	);
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_DE1 ,	"DE\'"	);	
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_HL1 ,	"HL\'"	);
	AppendMenu( wnd_trace_goto_sub_menu, MF_SEPARATOR, 0, nullptr); //----------------------
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_IX	 ,	"IX"	);
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_IY	 ,	"IY"	);
	AppendMenu( wnd_trace_goto_sub_menu, MF_SEPARATOR, 0, nullptr); //----------------------
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_SP	 ,	"SP"	);
	AppendMenu( wnd_trace_goto_sub_menu, MF_STRING, IDM_MON_GOTO_PC	 ,	"PC"	);
	//=====================================================================
	// WNDTRACE - Follow - sub menu
	//=====================================================================
	HMENU wnd_trace_follow_sub_menu = CreatePopupMenu();
	
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_AF) ? MF_CHECKED : MF_UNCHECKED);		
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_AF  ,	"AF"	);
	
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_BC) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_BC  ,	"BC"	);
		
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_DE) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_DE  ,	"DE"	);
		
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_HL) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_HL  ,	"HL"	);
	
	AppendMenu( wnd_trace_follow_sub_menu, MF_SEPARATOR, 0, nullptr); //----------------------

	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_AF1) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_AF1 ,	"AF\'"	);
		
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_BC1) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_BC1 ,	"BC\'"	);
		
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_DE1) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_DE1 ,	"DE\'"	);	
		
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_HL1) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_HL1 ,	"HL\'"	);

	AppendMenu( wnd_trace_follow_sub_menu, MF_SEPARATOR, 0, nullptr); //----------------------
		
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_IX) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_IX  ,	"IX"	);
		
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_IY) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_IY  ,	"IY"	);
		
	AppendMenu( wnd_trace_follow_sub_menu, MF_SEPARATOR, 0, nullptr); //----------------------
	
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_SP) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_SP  ,	"SP"	);
		
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_PC) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_PC  ,	"PC"	);
	
	AppendMenu( wnd_trace_follow_sub_menu, MF_SEPARATOR, 0, nullptr); //----------------------
	
	temp_mf_flag = MF_STRING | ((conf.trace_follow_regs == REG_NONE) ? MF_CHECKED : MF_UNCHECKED);	
	AppendMenu( wnd_trace_follow_sub_menu, temp_mf_flag, IDM_MON_FOLLOW_NONE,	"Off"	);
	//=====================================================================


	//---------------------------------------------------------------------
	

	//---------------------------------------------------------------------
	// WNDMEM - Switch dump mode - sub menu
	HMENU wnd_mem_switch_dump_mode_sub_menu = CreatePopupMenu();
	
	AppendMenu( wnd_mem_switch_dump_mode_sub_menu, MF_STRING, IDM_MON_MEM_DUMP_MEM	,	"CPU Memory"	);
	AppendMenu( wnd_mem_switch_dump_mode_sub_menu, MF_STRING, IDM_MON_MEM_DUMP_PHYS	,	"Disk PHYS"	);
	AppendMenu( wnd_mem_switch_dump_mode_sub_menu, MF_STRING, IDM_MON_MEM_DUMP_LOG	,	"Disk LOG"		);
	AppendMenu( wnd_mem_switch_dump_mode_sub_menu, MF_STRING, IDM_MON_MEM_DUMP_CMOS	,	"CMOS"		);
	AppendMenu( wnd_mem_switch_dump_mode_sub_menu, MF_STRING, IDM_MON_MEM_DUMP_NVRAM ,	"NVRAM"		);
	AppendMenu( wnd_mem_switch_dump_mode_sub_menu, MF_STRING, IDM_MON_MEM_DUMP_COMP_PAL ,	"Comp_Pal"	);
	//---------------------------------------------------------------------
	


	HMENU menu = CreatePopupMenu();



//	AppendMenu( menu, MF_STRING | MF_POPUP ,	(UINT) sub_menu, "444"			);
	
	
	
//	WNDNO,		WNDMEM,		WNDTRACE,	WNDREGS,
//	WND_PORTS,	WND_BETA128,	WND_STACK,	WND_PAGES,
//	WND_WATCHES,	WND_AY,		WND_TIME_DELTA
    
	//=====================================================================
	//  Right click at Trace window
	//=====================================================================
        if (activedbg == WNDTRACE)
        {
	    Z80 &cpu = CpuMgr.Cpu();
	    //====================================================================================================
	    temp_mf_flag = (cpu.membits[ cpu.trace_curs] & MEMBITS_BPX)  ?  MF_STRING | MF_CHECKED:
									    MF_STRING;
	    AppendMenu( menu, temp_mf_flag, IDM_MON_BPX,			"Breakpoint"			);
	    //----------------------------------------------------------------------------------------------------
	    temp_mf_flag = (cpu.membits[ cpu.trace_curs] & MEMBITS_BPR)  ?  MF_STRING | MF_CHECKED:
									    MF_STRING;
	    AppendMenu( menu, temp_mf_flag, IDM_MON_BPR,			"Breakpoint Read"		);
	    //----------------------------------------------------------------------------------------------------
	    temp_mf_flag = (cpu.membits[ cpu.trace_curs] & MEMBITS_BPW)  ?  MF_STRING | MF_CHECKED:
									    MF_STRING;
	    AppendMenu( menu, temp_mf_flag, IDM_MON_BPW,			"Breakpoint Write"		);
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr); //=======================================================
	    AppendMenu( menu, MF_STRING, IDM_MON_GOTO_PC,			"Goto PC"			);
	    //----------------------------------------------------------------------------------------------------
	    AppendMenuA( menu, (MF_STRING | MF_POPUP),
				(UINT_PTR) wnd_trace_goto_sub_menu, 		"Goto Register"	 ); // -->
	    //----------------------------------------------------------------------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_GOTO_OPERAND_ADDR,	   	"Goto Operand Address"		);

	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr); //=======================================================
	    AppendMenuA( menu, (MF_STRING | MF_POPUP),
				(UINT_PTR) wnd_trace_view_sub_menu, 		"View Register"	 ); // -->
	    //----------------------------------------------------------------------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_VIEW_OPERAND_ADDR,		"View Operand Address"		);
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr); //=======================================================
	    AppendMenuA( menu, (MF_STRING | MF_POPUP),
				(UINT_PTR) wnd_trace_follow_sub_menu, 		"Follow Register" ); // -->
	    //----------------------------------------------------------------------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_RETURN_TO_PREV_ADDR,		"Return to Previous Address"	);
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr); //=======================================================
	    AppendMenu( menu, MF_STRING, IDM_MON_SET_PC_TO_CURSOR,		"Set PC to Cursor"		);
	    //----------------------------------------------------------------------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_TRACE_TO_CURSOR,		"Trace to Cursor"		);
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr); //=======================================================
	    AppendMenu( menu, MF_STRING, IDM_MON_TOGGLE_LABELS,			"Toggle Labels"			);
	    //----------------------------------------------------------------------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_IMPORT_LABELS_FROM_ALASM,	"Import Labels From XAS/ALASM"	);
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr); //=======================================================
	    AppendMenu( menu, MF_STRING, IDM_MON_FIND_CODE,			"Find Code"			);
	    //----------------------------------------------------------------------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_FIND_TEXT,			"Find Text"			);
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr); //=======================================================
	    AppendMenu( menu, MF_STRING, IDM_MON_SWITCH_CPU,			"Switch CPU ZX/GS"		);
	    //====================================================================================================
        }
	//=====================================================================
	
	
	//=====================================================================
	//  Right click at Regs window
        //=====================================================================
	else if (activedbg == WNDREGS)
        {
            AppendMenu(menu, MF_STRING, IDM_MON_REG_GOTO_ADDR, "Goto address");
            AppendMenu(menu, MF_STRING, IDM_MON_REG_VIEW_ADDR, "View address");
	    //AppendMenu(menu, MF_STRING, 0, "I don't know");	//оригинальная муть...
            //AppendMenu(menu, MF_STRING, 0, "what to place");
            //AppendMenu(menu, MF_STRING, 0, "to menu, so");
            //AppendMenu(menu, MF_STRING, 0, "No Stuff Here");
        }
	//=====================================================================
	
	
	
	//=====================================================================
	//  Right click at Memory window
        //=====================================================================
	else if (activedbg == WNDMEM)
        {
	    AppendMenu( menu, MF_STRING, IDM_MON_MEM_GOTO_PC,			"Goto PC"		);
	    AppendMenu( menu, MF_STRING, IDM_MON_MEM_GOTO_SP,			"Goto SP"		);
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr);	//----------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_MEM_BPX,			"Breakpoint"		);
	    AppendMenu( menu, MF_STRING, IDM_MON_MEM_BPR,			"Breakpoint Read"	);
	    AppendMenu( menu, MF_STRING, IDM_MON_MEM_BPW,			"Breakpoint Write"	);
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr);	//----------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_MEM_TOGGLE_DUMP_HEX_TEXT,	"Toggle hex/text"	);
	    AppendMenuA( menu, (MF_STRING | MF_POPUP),
				(UINT_PTR) wnd_mem_switch_dump_mode_sub_menu, 	"Switch dump mode"	); // -->
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr);	//----------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_MEM_FIND_CODE,			"Find code"		);
	    AppendMenu( menu, MF_STRING, IDM_MON_MEM_FIND_TEXT,			"Find text"		);	    
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr);	//----------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_LOADBLOCK,			"Load block"		);
	    AppendMenu( menu, MF_STRING, IDM_MON_SAVEBLOCK,			"Save block"		);
	    AppendMenu( menu, MF_STRING, IDM_MON_FILLBLOCK,			"Fill block"		);
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr);	//----------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_RIPPER,			"Ripper's tool"		);
	    // AppendMenu( menu, MF_STRING, IDM_MON_MEM_GOTO_ADDRESS,		"Goto address"); //        deprecated	    
	    // AppendMenu( menu, MF_STRING, IDM_MON_MEM_SWITCH_DUMP_MODE,	"Switch dump mode"	);
        }
	//=====================================================================
	
	
	//=====================================================================
	//  Right click at watches window
	//=====================================================================
        else if (activedbg == WND_WATCHES)
        {
	    AppendMenu( menu, MF_STRING, IDM_MON_WATCHES_WATCHES,		"Watches"		);
	    AppendMenu( menu, MF_STRING, IDM_MON_WATCHES_SCREEN_MEMORY,		"Screen Memory"		);
	    AppendMenu( menu, MF_STRING, IDM_MON_WATCHES_SHADOW_SCREEN_MEMORY,	"Shadow Screen Memory"	);
	    AppendMenu( menu, MF_STRING, IDM_MON_WATCHES_RAY_PAINTED,		"Ray-Painted"		);
	    AppendMenu( menu, MF_SEPARATOR, 0, nullptr);	//----------------------------------------
	    AppendMenu( menu, MF_STRING, IDM_MON_WATCHES_SET_WATCH_ADDRESS,	"Set Watch Address"	);
	}
	
	//---------------------------------------------------------------------
	else
        {
            AppendMenu( menu, MF_STRING, 0, "HZ MENU");
        }
//-----------------------------------------------------------------------------

	//DrawMenuBar(wnd);



        int cmd = TrackPopupMenu(	menu,
					TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTALIGN | TPM_TOPALIGN,
					int(mousepos & 0xFFFF) + temp.client.left,
					int((mousepos >> 16) & 0x7FFF) + temp.client.top,
					0,
					wnd,
					nullptr
				 );

        //---------------------------------------------------------------------
	// зачем то удаляем все менюшки
	// наверное пушо их 16384 по все одновременно запущенные процессы и может не хватить...
	DestroyMenu( menu);
	
	DestroyMenu( wnd_trace_view_sub_menu);
	DestroyMenu( wnd_trace_goto_sub_menu);
	DestroyMenu( wnd_trace_follow_sub_menu);	
	
	DestroyMenu( wnd_mem_switch_dump_mode_sub_menu);
	
	
	
	
// если не править mousepos ТО при выкидывании менюшки
// нажатие up/down приведет к выходу по if (mousepos) return 0; в handle_menu()
	
	mousepos = 0;		// может нужно прям сюда?
	    
	debug_mouse_keys = 0;	// пока полный сброс нажатых клавишь
	
	// можно делать
	//    debugscr();
	//	debugflip();
	// вместо needclr
	
	
	// переписать под switch !!!
	//=====================================================================
	// Обработка событий контексного меню дебагера
	//---------------------------------------------------------------------
	
	//---------------------------------------------------------------------
	// WNDTRACE	(в окне дизасма)
	//---------------------------------------------------------------------
	if (cmd == IDM_MON_BPX)			{ cbpx();				needclr++;	}
	if (cmd == IDM_MON_BPR)			{ cbpr();				needclr++;	}
	if (cmd == IDM_MON_BPW)			{ cbpw();				needclr++;	}	
	// -----
	if (cmd == IDM_MON_GOTO_PC)		{ cfindpc();		showtrace();	debugflip();	}
	// goto register -->
		if (cmd == IDM_MON_GOTO_AF)	{ mon_goto_disasm_rAF();	needclr++; }
		if (cmd == IDM_MON_GOTO_BC)	{ mon_goto_disasm_rBC();	needclr++; }
		if (cmd == IDM_MON_GOTO_DE)	{ mon_goto_disasm_rDE();	needclr++; }
		if (cmd == IDM_MON_GOTO_HL)	{ mon_goto_disasm_rHL();	needclr++; }
		// -----
		if (cmd == IDM_MON_GOTO_AF1)	{ mon_goto_disasm_rAF1();	needclr++; }
		if (cmd == IDM_MON_GOTO_BC1)	{ mon_goto_disasm_rBC1();	needclr++; }
		if (cmd == IDM_MON_GOTO_DE1)	{ mon_goto_disasm_rDE1();	needclr++; }
		if (cmd == IDM_MON_GOTO_HL1)	{ mon_goto_disasm_rHL1();	needclr++; }
		// -----
		if (cmd == IDM_MON_GOTO_IX)	{ mon_goto_disasm_rIX();	needclr++; }
		if (cmd == IDM_MON_GOTO_IY)	{ mon_goto_disasm_rIY();	needclr++; }
		if (cmd == IDM_MON_GOTO_SP)	{ mon_goto_disasm_rSP();	needclr++; }
		if (cmd == IDM_MON_GOTO_PC)	{ mon_goto_disasm_rPC();	needclr++; }
	if (cmd == IDM_MON_GOTO_OPERAND_ADDR)	{ cjump();		showtrace();	debugflip();	}
	// -----	
	// view register -->
		if (cmd == IDM_MON_VIEW_AF)	{ mon_view_mem_rAF();	needclr++; }
		if (cmd == IDM_MON_VIEW_BC)	{ mon_view_mem_rBC();	needclr++; }
		if (cmd == IDM_MON_VIEW_DE)	{ mon_view_mem_rDE();	needclr++; }
		if (cmd == IDM_MON_VIEW_HL)	{ mon_view_mem_rHL();	needclr++; }
		// -----
		if (cmd == IDM_MON_VIEW_AF1)	{ mon_view_mem_rAF1();	needclr++; }
		if (cmd == IDM_MON_VIEW_BC1)	{ mon_view_mem_rBC1();	needclr++; }
		if (cmd == IDM_MON_VIEW_DE1)	{ mon_view_mem_rDE1();	needclr++; }
		if (cmd == IDM_MON_VIEW_HL1)	{ mon_view_mem_rHL1();	needclr++; }
		// -----
		if (cmd == IDM_MON_VIEW_IX)	{ mon_view_mem_rIX();	needclr++; }
		if (cmd == IDM_MON_VIEW_IY)	{ mon_view_mem_rIY();	needclr++; }
		if (cmd == IDM_MON_VIEW_SP)	{ mon_view_mem_rSP();	needclr++; }
		if (cmd == IDM_MON_VIEW_PC)	{ mon_view_mem_rPC();	needclr++; }
		
	if (cmd == IDM_MON_VIEW_OPERAND_ADDR)	{ cdjump();				needclr++;	}
	// -----
	// follow register -->
		if (cmd == IDM_MON_FOLLOW_AF)	{ conf.trace_follow_regs = REG_AF;	needclr++; }
		if (cmd == IDM_MON_FOLLOW_BC)	{ conf.trace_follow_regs = REG_BC;	needclr++; }
		if (cmd == IDM_MON_FOLLOW_DE)	{ conf.trace_follow_regs = REG_DE;	needclr++; }
		if (cmd == IDM_MON_FOLLOW_HL)	{ conf.trace_follow_regs = REG_HL;	needclr++; }
		// -----
		if (cmd == IDM_MON_FOLLOW_AF1)	{ conf.trace_follow_regs = REG_AF1;	needclr++; }
		if (cmd == IDM_MON_FOLLOW_BC1)	{ conf.trace_follow_regs = REG_BC1;	needclr++; }
		if (cmd == IDM_MON_FOLLOW_DE1)	{ conf.trace_follow_regs = REG_DE1;	needclr++; }
		if (cmd == IDM_MON_FOLLOW_HL1)	{ conf.trace_follow_regs = REG_HL1;	needclr++; }
		// -----
		if (cmd == IDM_MON_FOLLOW_IX)	{ conf.trace_follow_regs = REG_IX;	needclr++; }
		if (cmd == IDM_MON_FOLLOW_IY)	{ conf.trace_follow_regs = REG_IY;	needclr++; }
		if (cmd == IDM_MON_FOLLOW_SP)	{ conf.trace_follow_regs = REG_SP;	needclr++; }
		if (cmd == IDM_MON_FOLLOW_PC)	{ conf.trace_follow_regs = REG_PC;	needclr++; }
		// -----
		if (cmd == IDM_MON_FOLLOW_NONE)	{ conf.trace_follow_regs = REG_NONE;	needclr++; }		
	if (cmd == IDM_MON_RETURN_TO_PREV_ADDR) { pop_pos() ;		showtrace();	debugflip();	}
	// -----
	if (cmd == IDM_MON_SET_PC_TO_CURSOR)	{ csetpc();		showtrace();	debugflip();	}
	if (cmd == IDM_MON_TRACE_TO_CURSOR)	{ chere(); 		dbg_force_exit = 1; 		}
	// chere() работает только после пинка !
	// -----
	if (cmd == IDM_MON_TOGGLE_LABELS)	{ cfliplabels();	/*showtrace()*/	debugflip();	}
	if (cmd == IDM_MON_IMPORT_LABELS_FROM_ALASM) { c_lbl_import();			needclr++;	}
	// -----
	if (cmd == IDM_MON_FIND_CODE)		{ cfindcode() ;				needclr++;	}
	if (cmd == IDM_MON_FIND_TEXT)		{ cfindtext() ;				needclr++;	}
	// -----
	if (cmd == IDM_MON_SWITCH_CPU)		  mon_switch_cpu();
	//---------------------------------------------------------------------
	
	
	//=====================================================================
	// WNDMEM	(в окне мемори виевера) 
	//=====================================================================
	if (cmd == IDM_MON_MEM_GOTO_PC)			{ mpc();	showmem();	debugflip();	}
	if (cmd == IDM_MON_MEM_GOTO_SP)			{ msp();	showmem();	debugflip();	}
	//---------------------------------------------------------------------
	if (cmd == IDM_MON_MEM_BPX)			{ mbpx();			needclr++;	}
	if (cmd == IDM_MON_MEM_BPR)			{ mbpr();			needclr++;	}
	if (cmd == IDM_MON_MEM_BPW)			{ mbpw();			needclr++;	}
	//---------------------------------------------------------------------	
	if (cmd == IDM_MON_MEM_TOGGLE_DUMP_HEX_TEXT)	{ mon_dump();			needclr++;	}
	// Switch dump mode - Sub Menu
		if (cmd == IDM_MON_MEM_DUMP_MEM)	{ editor = 0;			needclr++;};
		if (cmd == IDM_MON_MEM_DUMP_PHYS)	{ editor = 1;			needclr++;};
		if (cmd == IDM_MON_MEM_DUMP_LOG)	{ editor = 2;			needclr++;};
		if (cmd == IDM_MON_MEM_DUMP_CMOS)	{ editor = 3;			needclr++;};
		if (cmd == IDM_MON_MEM_DUMP_NVRAM)	{ editor = 4;			needclr++;};
		if (cmd == IDM_MON_MEM_DUMP_COMP_PAL)	{ editor = 5;			needclr++;};
	//---------------------------------------------------------------------
	if (cmd == IDM_MON_MEM_FIND_CODE)		{ mcode();			needclr++;	}
	if (cmd == IDM_MON_MEM_FIND_TEXT)		{ mtext();			needclr++;	}
	// -----	
	if (cmd == IDM_MON_LOADBLOCK)			{ mon_load();			needclr++;	}
	if (cmd == IDM_MON_SAVEBLOCK)			{ mon_save();			needclr++;	}
	if (cmd == IDM_MON_FILLBLOCK)			{ mon_fill();			needclr++;	}
	// -----
	if (cmd == IDM_MON_RIPPER)			{ mon_tool();			needclr++;	}
	//---------------------------------------------------------------------
//	if (cmd == IDM_MON_MEM_GOTO_ADDRESS)		{ mgoto();	showmem();	debugflip();	}
//	if (cmd == IDM_MON_MEM_SWITCH_DUMP_MODE)	{ mon_switch_dump(); 		needclr++;	}	
	//=====================================================================
	
	
	//=====================================================================
	// WNDREGS	(в окне регистров) 	
	//=====================================================================
	if (cmd == IDM_MON_REG_GOTO_ADDR)		{ rcodejump();			needclr++;	}
	if (cmd == IDM_MON_REG_VIEW_ADDR)		{ rdatajump();			needclr++;	}
	//=====================================================================
	
	
	
	//---------------------------------------------------------------------
	// WND_WATCHES	(в окне вачесов) 	
	//---------------------------------------------------------------------
	if (cmd == IDM_MON_WATCHES_WATCHES)		{ show_scrshot = 0;	needclr++;	}
	if (cmd == IDM_MON_WATCHES_SCREEN_MEMORY)	{ show_scrshot = 1;	needclr++;	}
	if (cmd == IDM_MON_WATCHES_SHADOW_SCREEN_MEMORY){ show_scrshot = 2;	needclr++;	}
	if (cmd == IDM_MON_WATCHES_RAY_PAINTED)		{ show_scrshot = 3;	needclr++;	}
	if (cmd == IDM_MON_WATCHES_SET_WATCH_ADDRESS)	{ mon_setwatch();	needclr++;	}
	//---------------------------------------------------------------------



	//if (cmd == IDM_SOME_OTHER) some_other();
	//needclr++;
    }
    // Right click
    //=========================================================================
    
    mousepos = 0;	// [NS] перенесено вверх
			// если закоментить тут то курсор/выделение опкода в дебагере застряет в месте клика


    // printf("debug_mouse_keys %8X\n",debug_mouse_keys);
    
    //=========================================================================
    // Double Left Click
    if (debug_mouse_keys & DEBUG_MOUSE_LEFT_DBL)
    {
	//---------------------------------------------------------------------
	// Double Left в окне дизасма -> режим редактирования
	if (activedbg == WNDTRACE)
	{
	    // костыль			// иначе будет режим редактироания вводом
	    input.lastkey = VK_RETURN;	// с очисткой содержимого
					// когда нам надо без очистки
	    center();
	   
	    // костыль	 // без перерисовки !!!!
	    showtrace(); // будет двойное сохранение с затиранием следующих команд!!
	    debugflip(); // видимо вход в редактирования след команды не видно
			 // а данные берутсо из старого буфера
	}
	//---------------------------------------------------------------------
	// Double Left в окне мемори виевера -> режим редактирования
	if (activedbg == WNDMEM)
	{
	    menter();	// сам работает только когда курсор над адресом
			// другие проверки не нужны
	    showmem();		// нужна перерисовка
	    debugflip();	// иначе переход видно только потом
	}
	//---------------------------------------------------------------------
	// Double Left в окне мемори виевера -> режим редактирования
	if (activedbg == WNDREGS)
	{
	    // костыль			// иначе срабатывает выход в renter()
	    input.lastkey = VK_RETURN;	// в if ( ToAscii(
	    
	    renter();	// сам работает только когда курсор над адресом
			// другие проверки не нужны
	    showregs();		// нужна перерисовка
	    debugflip();	// иначе изменения видно только потом
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    debug_mouse_keys = 0;	// на всякий случай
    //=========================================================================
    
}
//=============================================================================


//=============================================================================
void TCpuMgr::CopyToPrev()
{
    for(unsigned i = 0; i < Count; i++)
        PrevCpus[i] = *Cpus[i];
}
//=============================================================================

/* ------------------------------------------------------------- */



//=============================================================================
static void debug( Z80 *cpu)
{
//вход сюда только при входе в дебагер
// и при step over когда пропускаютсо call-ы
//  printf("debug entry");

    //-------------------------------------------------------------------------
    if (conf.debug_unlock_mouse)	// [NS]
    {
	main_unlock_mouse_silent();
    }
    //-------------------------------------------------------------------------
    dbg_force_exit = 0;			// [NS]
	
    conf.trace_follow_request = 1;	// перевести фокус на PC при входе	[NS] 
    
    cpu->graph_trace_cursor_pos = 0;	// чисто графическое положение курсора	[NS]

    OnEnterGui();

    update_disasm_style();		// [NS]
   
    temp.mon_scale = temp.scale;	
    temp.scale = 1;		
   
   //#define RF_MONITOR (RF_MON | RF_GDI | RF_2X)
//   temp.rflags = RF_MONITOR;


//   RECT current_mainwnd_size;
//   GetClientRect( wnd, &current_mainwnd_size);
//   int real_dbg_wnd_size_x = current_mainwnd_size.right - current_mainwnd_size.left;
//   int real_dbg_wnd_size_y = current_mainwnd_size.bottom - current_mainwnd_size.top;




    if		(temp.rflags & RF_4X)			//NS
    {
	temp.rflags = (RF_MON | RF_GDI | RF_4X);
    }
    else if	(temp.rflags & RF_3X)
    {
	temp.rflags = (RF_MON | RF_GDI | RF_3X);
    }
    else if	(temp.rflags & RF_2X)
    {
	temp.rflags = (RF_MON | RF_GDI | RF_2X);
    }
    else if	(temp.rflags & RF_1X)
    {
	temp.rflags = (RF_MON | RF_GDI | RF_1X);
    }

   
    needclr = 1;
    dbgbreak = 1;

    //conf.win_resize_request = 0;
    set_video( TRUE);	//надо включать gdi режим!!
			// preserve_size = true
			
    CpuMgr.SetCurrentCpu( cpu->GetIdx());
    TZ80State *prevcpu = &CpuMgr.PrevCpu( cpu->GetIdx());
    
    // По моему это куда переводить курсор при входе в дебагер 
    //cpu->trace_curs = cpu->pc;
    if (conf.trace_follow_regs == REG_AF) cpu->trace_curs = cpu->af;
    if (conf.trace_follow_regs == REG_BC) cpu->trace_curs = cpu->bc;
    if (conf.trace_follow_regs == REG_DE) cpu->trace_curs = cpu->de;
    if (conf.trace_follow_regs == REG_HL) cpu->trace_curs = cpu->hl;
    
    if (conf.trace_follow_regs == REG_AF1) cpu->trace_curs = cpu->alt.af;
    if (conf.trace_follow_regs == REG_BC1) cpu->trace_curs = cpu->alt.bc;
    if (conf.trace_follow_regs == REG_DE1) cpu->trace_curs = cpu->alt.de;
    if (conf.trace_follow_regs == REG_HL1) cpu->trace_curs = cpu->alt.hl;
    
    if (conf.trace_follow_regs == REG_IX) cpu->trace_curs = cpu->ix;
    if (conf.trace_follow_regs == REG_IY) cpu->trace_curs = cpu->iy;
    if (conf.trace_follow_regs == REG_SP) cpu->trace_curs = cpu->sp;
    if (conf.trace_follow_regs == REG_PC) cpu->trace_curs = cpu->pc;
	    
    cpu->dbg_stopsp = cpu->dbg_stophere = -1U;
    cpu->dbg_loop_r1 = 0;
    cpu->dbg_loop_r2 = 0xFFFF;
    mousepos = 0;

    //-------------------------------------------------------------------------
    // dbg_mainloop
    while (dbgbreak)	// debugger event loop
    {			// ВХОД/ЦИКЛ ТОЛЬКО ПРИ ТЫКАНИИ КНОПОК КЛАВЫ!!!!
			// для мышы и менюшок есть еще цикл
dbg_mainloop:
   //printf("while(dbgbreak)\n");
   
	if (trace_labels)
	    mon_labels.notify_user_labels();

	cpu = &CpuMgr.Cpu();
	prevcpu = &CpuMgr.PrevCpu( cpu->GetIdx());
	
repaint_dbg:
//printf("repaint_dbg\n !!!!!!!!!!!!");
	//вход при	тягании окно
	//		нажатии кнопок мыши и клавы

	cpu->trace_top &= 0xFFFF;
	cpu->trace_curs &= 0xFFFF;

	debugscr();	//обновление всех окон в буфере
	debugflip();	//отрисовка экрана из буфера




//-----------------------------------------------------------------------------
sleep:
//printf("*");
	while (!dispatch(nullptr))
	{
	    //цикл ожидания
	    //printf ("sleep\n");
	    
	    //printf("needclr %d\n",needclr);
      
	    if (mousepos)
		handle_mouse();
	     
	    if (needclr)	//unsigned char needclr
	    {
		//needclr--;
		needclr = 0;	// тест [NS]
				// ибо нахуя перерисовывать 15 раз?
		goto repaint_dbg;
	    }
	 
	    if (dbg_force_exit)
	    {
		dbg_force_exit = 0;
		goto force_exit_from_debug;
	    }
	 
	    Sleep(20);	//20 !!
	} //sleep loop
//-----------------------------------------------------------------------------
// Опрос клавы
//-----------------------------------------------------------------------------
	
// делать dispatch_more после проверки актиности окна!!!
// может это поможот победить конфликты хоткеев
//	хотя конфликт повторить ЩАС не удаетсо
//	а сам глобальный набор хоткеев ac_mon
//	подключен вниз к каждому набору хоткеев ac_regs, ac_trace, ac_mem
//		уже отдельно

	
	//---------------------------------------------------------------------
	// hotkeyz (с большим приоритетом)
	//---------------------------------------------------------------------
	if (activedbg == WNDREGS && dispatch_more(ac_regs))	// кнопки в окне регистров
	{
	    //printf("(activedbg == WNDREGS && dispatch_more(ac_regs))\n");
	    continue;	//goto -> while (dbgbreak)
	}
	//---------------------------------------------------------------------
	if (activedbg == WNDTRACE && dispatch_more(ac_trace))	// кнопки в окне дизасма
	{
	    //printf("(activedbg == WNDTRACE && dispatch_more(ac_trace))\n");
	    continue;	//goto -> while (dbgbreak)
	}
	//---------------------------------------------------------------------
	if (activedbg == WNDMEM && dispatch_more(ac_mem))	// кнопки в окне мемори виевера
	{
	    //printf("(activedbg == WNDMEM && dispatch_more(ac_mem))\n");
	    continue;	//goto -> while (dbgbreak)
	}
	//---------------------------------------------------------------------
	if (dispatch_more(ac_mon))				// глобальные кнопки в дебагере
	{
	    continue;	//goto -> while (dbgbreak)
	}
	
	
	//---------------------------------------------------------------------
	// Начало прямой правки значений (с меньшим приоритетом)
	//---------------------------------------------------------------------
	if (activedbg == WNDREGS && dispatch_regs())	// правка в окне регистрров
	{
	    //printf("(activedbg == WNDREGS && dispatch_regs())\n");
	    continue;	//goto -> while (dbgbreak)
	}
	//---------------------------------------------------------------------
	if (activedbg == WNDTRACE && dispatch_trace()) // правка в окне дизасма
	{
	    //printf("(activedbg == WNDTRACE && dispatch_trace())\n");
	    continue;	//goto -> while (dbgbreak)
	}
	//---------------------------------------------------------------------
	if (activedbg == WNDMEM && dispatch_mem()) // правка в окне мемори виевера
	{
	    //printf("(activedbg == WNDMEM && dispatch_mem())\n");
	    continue;	//goto -> while (dbgbreak)
	}
	//---------------------------------------------------------------------

	
	
//	if (needclr)	//????? [NS] нахрена это?
//	{
//	    needclr--;
//	    continue;
//	}
      
	goto sleep;
	//printf("exit 1\n");
    
    } //while (dbgbreak)
    //-------------------------------------------------------------------------

force_exit_from_debug:
// Выход из дебагера  
    //printf("exit from debug\n");

    *prevcpu = *cpu;
//   CpuMgr.CopyToPrev();
    cpu->SetLastT();
    temp.scale = temp.mon_scale;	
    
    // сильно тяжелый выход из дебагера
    // пересчеты таблиц итд...
    apply_video( TRUE);		// preserve_size = true
    OnExitGui( false);
}
//=============================================================================


//=============================================================================
void debug_events( Z80 *cpu)
{
    unsigned pc = cpu->pc & 0xFFFF;
    unsigned char *membit =         cpu->membits         + pc;
    unsigned char *bp_disable_bit = cpu->bp_disable_bits + pc;	// [NS]
    
    *membit |= MEMBITS_X;	// memoryband
    
    // нужно заменить на
    // dbgbreak |= ((*membit & MEMBITS_BPR) && (*bp_disable_bit & BP_DISABLE_BPR));
    // и инверсировать назначение битов в BP_DISABLE_XXX !!!!!
    //-------------------------------------------------------------------------
    if ((*bp_disable_bit & BP_DISABLE_BPX) == 0)	// не оптимально !!!!! [NS]
    {
	cpu->dbgbreak |= (*membit & MEMBITS_BPX);
	dbgbreak      |= (*membit & MEMBITS_BPX);
    }
    //-------------------------------------------------------------------------
    if (pc == cpu->dbg_stophere)
    {
	//printf("dbg_stophere\n");
	cpu->dbgbreak = 1;
	dbgbreak = 1;
    }
    //-------------------------------------------------------------------------
    if ((cpu->sp & 0xFFFF) == cpu->dbg_stopsp)
    {
	//---------------------------------------------------------------------
	if (pc > cpu->dbg_stophere && pc < cpu->dbg_stophere + 0x100)
	{
	    //printf("dbg_stophere\n");
	    cpu->dbgbreak = 1;
	    dbgbreak = 1;
	}
	//---------------------------------------------------------------------
	if (pc < cpu->dbg_loop_r1 || pc > cpu->dbg_loop_r2)
	{
	    cpu->dbgbreak = 1;
	    dbgbreak = 1;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    if (cpu->cbpn)
    {
	cpu->r_low = (cpu->r_low & 0x7F) + cpu->r_hi;
	//---------------------------------------------------------------------
	for (unsigned i = 0; i < cpu->cbpn; i++)
	{
	    //-----------------------------------------------------------------
	    if (calc( cpu, cpu->cbp[ i]))
	    {
		cpu->dbgbreak = 1;
		dbgbreak = 1;
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------

    brk_port_in = brk_port_out = -1U; // reset only when breakpoints active
    //-------------------------------------------------------------------------
    if (cpu->dbgbreak)
        debug( cpu);
    //-------------------------------------------------------------------------
}
//=============================================================================


#endif // MOD_MONITOR


//=============================================================================
// is there breakpoints active or any other reason to use debug z80 loop?
unsigned char isbrk( const Z80 &cpu) 	//!!!!!!!!
{

// важная функция при модернизации дебагера !!!
// printf("isbrk()");

    //-------------------------------------------------------------------------
    #ifndef MOD_DEBUGCORE
	return 0;
    #else
    //-------------------------------------------------------------------------
	#ifdef MOD_MEMBAND_LED
	    //-----------------------------------------------------------------
	    if (conf.led.memband & 0x80000000)
		return 1;
	    //-----------------------------------------------------------------
	    if (conf.led.memband_256 & 0x80000000)	// [NS]
		return 1;
	    //-----------------------------------------------------------------
	#endif
	//---------------------------------------------------------------------
	// breakpoint on read ROM switches ROM bank
	if (conf.mem_model == MM_PROFSCORP)		// и зачем????
	    return 1; 
	//---------------------------------------------------------------------
	#ifdef MOD_MONITOR
	    //-----------------------------------------------------------------
	    if (cpu.cbpn)
		return 1;
	    //-----------------------------------------------------------------
// проверка стоят ли бряки!!!
// если тут будет 4 метра под раздельные страницы
// надо будет блджд прочесывать 4+ метра !!!!!
// хотя это видимо не так часто вызываетсо
    
// по хорошему нужно не сканировать по 16М если бряки никогда не ставились
// или если их больше нет...
// 
// ...хотя теперь же бряки и из файла...
// а так надежней :)

	    unsigned char res = 0;
	    //-----------------------------------------------------------------
	    for (int i = 0;    i < 0x10000;    i++)
		res |= cpu.membits[ i];
	    //-----------------------------------------------------------------
	    return (res & (MEMBITS_BPR | MEMBITS_BPW | MEMBITS_BPX));
	#endif //MOD_MONITOR
	//---------------------------------------------------------------------
    #endif
    //-------------------------------------------------------------------------
}
//=============================================================================

