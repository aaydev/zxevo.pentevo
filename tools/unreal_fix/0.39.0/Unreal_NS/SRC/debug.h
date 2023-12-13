#pragma once
//#ifndef DEBUG_H
//#define DEBUG_H


//-----------------------------------------------------------------------------
// Regs				// WNDREGS
#define regs_x 1
#define regs_y 1

#define regs_cx 32		// size
#define regs_cy 4
//-----------------------------------------------------------------------------
// Stack			// WND_STACK
#define stack_x 72
#define stack_y 12
#define stack_size 10

#define stack_cx 7		// size
#define stack_cy 10
//-----------------------------------------------------------------------------
// AY				// WND_AY
#define ay_x  31
#define ay_y  28

#define ay_cx  48		// size
#define ay_cy  1
//-----------------------------------------------------------------------------
// Time Delta			// WND_TIME_DELTA
#define time_x 1
#define time_y 28

#define time_cx 26		// size
#define time_cy 1
//-----------------------------------------------------------------------------
// deprecated copyrights (был на месте time delta)
//#define copy_x 1
//#define copy_y 28
//-----------------------------------------------------------------------------
// Pages			// WND_PAGES - маленькое окошко с страницами
#define banks_x 72
#define banks_y 23

#define banks_cx 7		// size
#define banks_cy 4
//-----------------------------------------------------------------------------
// Ports			// WND_PORTS - маленькое окошко с портами
#define ports_x 72		// FE 7FFD cmos EFF7
#define ports_y 1

#define ports_cx 7		// Size
#define ports_cy 4
//-----------------------------------------------------------------------------
// Beta 128			// WND_BETA128 - маленькое окошко
#define dos_x 72
#define dos_y 6

#define dos_cx 7		// size
#define dos_cy 5
//-----------------------------------------------------------------------------
// Trace window			// WNDTRACE
#define trace_size 21		
#define trace_x 1
#define trace_y 6

#define trace_cx 32		// size
#define trace_cy 21
//-----------------------------------------------------------------------------
// Watches			// WND_WATCHES - большое окно с превиевом памяти
#define wat_x 34		// по адресам в регистрах
#define wat_y 1
#define wat_sz 13

#define wat_cx 37		// size
#define wat_cy 13
//-----------------------------------------------------------------------------
// Memory window		// WNDMEM
#define mem_size 12
#define mem_x 34
#define mem_y 15

#define mem_cx 37		// size
#define mem_cy 12
//-----------------------------------------------------------------------------





//NS
//#define DBG_REGS		0x38	//0011 1000 bright white?
//#define DBG_REGS_HGLGHT	0x17	//0001 0111

#define W_SEL      0x17
#define W_NORM     0x07
#define W_CURS     0x30		//	00110000
#define BACKGR     0x50
#define FRAME_CURS 0x02
#define W_TITLE    0x59
#define W_OTHER    0x40
#define W_OTHEROFF 0x47
#define BACKGR_CH  0xB1
#define W_AYNUM    0x4F
#define W_AYON     0x41
#define W_AYOFF    0x40
#define W_BANK     0x40
#define W_BANKRO   0x41
#define W_DIHALT1  0x1A
#define W_DIHALT2  0x0A
#define W_TRACEPOS 0x70
#define W_INPUTCUR 0x60
#define W_INPUTBG  0x40
#define W_48K 0x20
#define W_DOS 0x20

#define W_TRACE_JINFO_CURS_FG   0x0D
#define W_TRACE_JINFO_NOCURS_FG 0x02
#define W_TRACE_JARROW_FOREGR   0x0D

#define FRAME         0x01
#define FFRAME_FRAME  0x04

#define FFRAME_INSIDE 0x50
#define FFRAME_ERROR  0x52
#define FRM_HEADER    0xD0

#define MENU_INSIDE   0x70
#define MENU_HEADER   0xF0

#define MENU_CURSOR   0xE0
#define MENU_ITEM     MENU_INSIDE
#define MENU_ITEM_DIS 0x7A



//=============================================================================
// Типы текущего "активного" окна дебагера
enum DBGWND
{
    WNDNO,
    WNDMEM,
    WNDTRACE,
    WNDREGS,
    WND_PORTS,		// [NS]
    WND_BETA128,	// [NS]
    WND_STACK,		// [NS]
    WND_PAGES,		// [NS]
    WND_WATCHES,	// [NS]
    WND_AY,		// [NS]
    WND_TIME_DELTA	// [NS]
};
//=============================================================================
enum 
{ 
    ED_MEM, 
    ED_PHYS, 
    ED_LOG, 
    ED_CMOS, 
    ED_NVRAM, 
    ED_COMP_PAL, 
    ED_MAX 
};
//=============================================================================



//=============================================================================
class TCpuMgr
{
    static const unsigned Count;
    static Z80* Cpus[];
    static TZ80State PrevCpus[];
    static unsigned CurrentCpu;
public:
    //-------------------------------------------------------------------------
    static Z80 &Cpu() 
    { 
	return *Cpus[ CurrentCpu]; 
    }
    //-------------------------------------------------------------------------
    static Z80 &Cpu( u32 Idx) 
    { 
	return *Cpus[ Idx]; 
    }
    //-------------------------------------------------------------------------
    static TZ80State &PrevCpu( u32 Idx) 
    { 
	return PrevCpus[ Idx]; 
    }
    //-------------------------------------------------------------------------
    static TZ80State &PrevCpu() 
    { 
	return PrevCpus[ CurrentCpu]; 
    }
    //-------------------------------------------------------------------------
    static void SwitchCpu();
    //-------------------------------------------------------------------------
    static unsigned GetCurrentCpu() 
    { 
	return CurrentCpu; 
    }
    //-------------------------------------------------------------------------
    static void SetCurrentCpu( u32 Idx) 
    { 
	CurrentCpu = Idx; 
    }
    //-------------------------------------------------------------------------
    static void CopyToPrev();
    //-------------------------------------------------------------------------
    static unsigned GetCount() 
    { 
	return Count; 
    }
    //-------------------------------------------------------------------------
};
//=============================================================================

extern TCpuMgr CpuMgr;
extern DBGWND activedbg;
extern unsigned dbg_extport;
extern unsigned char dbg_extval; // extended memory port like 1FFD or DFFD

extern unsigned mem_sz;
extern unsigned mem_disk;
extern unsigned mem_track;
extern unsigned mem_max;

extern unsigned char mem_dump;
extern unsigned show_scrshot;
extern unsigned char editor;
extern unsigned char mem_ascii;

extern unsigned char mem_dump;

extern unsigned ripper; // ripper mode (none/read/write)

extern int dbg_force_exit;	// для костыльного выхода из дебагера	[NS]
				// без дополнительного аникея

extern unsigned user_watches[ 3];
extern unsigned regs_curs;

extern unsigned char trace_labels;
//-----------------------------------------------------------------------------
// is there breakpoints active or any other reason to use debug z80 loop?
unsigned char isbrk( const Z80 &cpu); 
//-----------------------------------------------------------------------------
void debugscr();
//-----------------------------------------------------------------------------
void debug_events( Z80 *cpu);
//-----------------------------------------------------------------------------
//#endif
