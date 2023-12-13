#include "std.h"

#include "emul.h"
#include "vars.h"
#include "draw.h"
#include "dx.h"
#include "debug.h"
#include "dbgpaint.h"
#include "dbgmem.h"
#include "dbgrwdlg.h"
#include "dbgcmd.h"
#include "memory.h"
#include "gui.h"
#include "util.h"

//=============================================================================
void out(unsigned port, unsigned char val);
//=============================================================================


#define DBG_ATTR_BCKGRND		0x00	//0	0
#define DBG_ATTR_BCKGRND_ACTIVE		0x10	//blue


#define DBG_ATTR_WINDOW_TITLE		0xF0//0x1F	//blue	br+WHITE

#define DBG_ATTR_WINDOW_BACK_COLOR	0x70//0x70//white	0
#define DBG_ATTR_WINDOW_TEXT		0x70
#define DBG_ATTR_WINDOW_TEXT_ERROR	0x72		//white	red


//=============================================================================
int find1dlg(unsigned start)
{
    static char ftext[12] = "";
    strcpy(	str,
		ftext
	   );
    filledframe(	10,
			10,
			16,
			4,
			DBG_ATTR_WINDOW_BACK_COLOR
		);
    tprint(	10,
		10,
		"  find string   ",
		DBG_ATTR_WINDOW_TITLE	//FRM_HEADER
		);
    tprint(	11,
		12,
		"text:", 
		DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
	   );
    if (!inputhex( 17, 12, 8, false)) return -1;

    strcpy(	ftext,
		str
	   );

    size_t len = strlen(ftext);
    unsigned i;		//Alone Coder 0.36.7
    
    for (unsigned ptr = memadr(start+1); ptr != start; ptr = memadr(ptr+1))
    {
	for (/*unsigned*/ i = 0; i < len; i++)
	    if (editrm(memadr(ptr+i)) != ftext[i]) break;
	    
	if (i == len) return int(ptr);
    }
    tprint(	11,
		12,
		"  not found   ",
		DBG_ATTR_WINDOW_TEXT_ERROR	//FFRAME_ERROR
	   );
    debugflip();
    while (!process_msgs());
    return -1;
}
//=============================================================================


//=============================================================================
int find2dlg(unsigned start)
{
    static unsigned code = 0xF3, mask = 0xFF; char ln[64];
    //-------------------------------------------------------------------------
    filledframe(	10,
			10,
			16,
			5,
			DBG_ATTR_WINDOW_BACK_COLOR
		);
    //-------------------------------------------------------------------------
    tprint(	10,
		10,
		"   find data    ",
		DBG_ATTR_WINDOW_TITLE	//FRM_HEADER
	   );
    //-------------------------------------------------------------------------
    sprintf(	ln,
		"code: %08lX",
		_byteswap_ulong(code)
	    );
    tprint(	11,
		12,
		ln,
		DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
	   );
    //-------------------------------------------------------------------------
    sprintf(	ln,
		"mask: %08lX",
		_byteswap_ulong(mask)
	    );
    tprint(	11,
		13,
		ln,
		DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
	   );
    //-------------------------------------------------------------------------
    // ввод code:
    sprintf(	str,
		"%08lX",
		_byteswap_ulong(code)
	    );
    if (!inputhex(17,12,8,true)) return -1;
    sscanf(	str,
		"%x",
		&code
	    );
    code = _byteswap_ulong(code);
    tprint(	17,	//введенное code: xxxxxxxx
		12,
		str,
		DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
	   );
    //-------------------------------------------------------------------------
    // ввод mask:
    sprintf(	str,
		"%08lX",
		_byteswap_ulong(mask)
	    );
    if (!inputhex( 17, 13, 8, true )) return -1;
    sscanf(	str,
		"%x",
		&mask
	   );
    mask = _byteswap_ulong(mask);
    //-------------------------------------------------------------------------
    unsigned i; //Alone Coder 0.36.7
    for (unsigned ptr = memadr( start + 1);    ptr != start;    ptr = memadr( ptr + 1))
    {
	unsigned char *cd = (unsigned char*) &code, *ms = (unsigned char*) &mask;
	for (/*unsigned*/ i = 0;    i < 4;    i++)
	    if ((editrm(memadr(ptr+i)) & ms[i]) != (cd[i] & ms[i]))
		break;
	if (i == 4) return int(ptr);
    }
    tprint(	11,
		12,
		"  not found   ",
		DBG_ATTR_WINDOW_TEXT_ERROR	//FFRAME_ERROR
	   );
    tprint(	11,
		13,
		"              ",
		DBG_ATTR_WINDOW_TEXT_ERROR	//FFRAME_ERROR
	   );
    debugflip();
    while (!process_msgs());
    return -1;
}
//=============================================================================


//=============================================================================
// fill memory block with pattern		// в WNDMEM меню
void mon_fill()					//и "mon.fillblock" хоткей
{
    filledframe(	6,
			10,
			26,
			5,
			DBG_ATTR_WINDOW_BACK_COLOR
		);
    char ln[64];
    sprintf(	ln,
		"start: %04X end: %04X",
		addr,
		end
	   );
    tprint(	6,
		10,
		"    fill memory block     ",
		DBG_ATTR_WINDOW_TITLE	//FRM_HEADER
	   );
    tprint(	7,
		12,
		"pattern (hex):",
		DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
	   );
    tprint(	7,
		13,
		ln,
		DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
	   );

    static char fillpattern[10] = "00";

    unsigned char pattern[4];
    unsigned fillsize = 0;

    strcpy(	str,
		fillpattern
	   );
    if (!inputhex( 22, 12, 8,true)) return;

    strcpy(	fillpattern,
		str
	   );

    if (!fillpattern[0])
	strcpy(	    fillpattern,
		    "00"
	       );

    for (fillsize = 0; fillpattern[2*fillsize]; fillsize++)
    {
	if (!fillpattern[2 * fillsize + 1])
	{
	    fillpattern[2 * fillsize + 1] = '0';
	    fillpattern[2 * fillsize + 2] = 0;
	}
	pattern[fillsize] = hex(fillpattern + 2*fillsize);
    }
    tprint(	22,
		12,
		"        ",
		DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
	   );
    tprint(	22,
		12,
		fillpattern,
		DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
	   );

    int a1 = input4(	14,
			13,
			addr
		    );
    if (a1 == -1) return;
   
    addr = unsigned(a1);
    tprint(	14,
		13,
		str,
		DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
	   );
    a1 = input4(	24,
			13,
			end
		);
    if (a1 == -1) return;
    end = unsigned(a1);

    unsigned pos = 0;
    for (a1 = int(addr); a1 <= int(end); a1++)
    {
	cpu.DirectWm(unsigned(a1), pattern[pos]);
	if (++pos == fillsize) pos = 0;
    }
}
//=============================================================================


//=============================================================================
// Выход из монитора					//"mon.emul" хоткей
void mon_emul()
{
    for (unsigned i = 0;    i < CpuMgr.GetCount();    i++)
    {
	Z80 &cpu = CpuMgr.Cpu( i);
	cpu.dbgchk = isbrk( cpu);
	cpu.dbgbreak = 0;
    }
    dbgbreak = 0;
}
//=============================================================================


//=============================================================================
// смотрениие экрана из дебагера по хоткею
static void mon_scr(char alt)
{
//  printf("mon_scr()\n");
    temp.scale = temp.mon_scale;
    apply_video();

    memcpy(	save_buf,
		rbuf,
		rb2_offs
	   );
    paint_scr(alt);
    flip();
    if (conf.noflic) flip();
    memcpy(	rbuf,
		save_buf,
		rb2_offs
	   );

    while (!process_msgs()) Sleep(20);
   
    temp.rflags = RF_MONITOR;
    temp.mon_scale = temp.scale;
    temp.scale = 1;		
    set_video();
}
//=============================================================================


//=============================================================================
void mon_scr0() { mon_scr(0); }		// "mon.screen",	mon_scr0
//=============================================================================
void mon_scr1() { mon_scr(1); }		// "mon.altscreen",	mon_scr1
//=============================================================================
void mon_scray() { mon_scr(2); }	// "mon.rayscreen",	mon_scray
//=============================================================================


//=============================================================================
void mon_exitsub()	//trace until (sp)		//"mon.exitsub" хоткей
{
    //printf("mon_exitsub()\n");
    Z80 &cpu = CpuMgr.Cpu();
    cpu.dbgchk = 1;
    cpu.dbgbreak = 0;
    dbgbreak = 0;
    cpu.dbg_stophere = cpu.DirectRm( cpu.sp) + 0x100 * cpu.DirectRm( cpu.sp + 1);
    //printf("mon_exitsub\n");
}
//=============================================================================


//=============================================================================
void editbank()		//write to #7FFD		//"mon.setbank" хоткей
{
    //printf("editbank()\n");
    int x = input2(	ports_x+5,
			ports_y+1,
			comp.p7FFD
			);
    if (x != -1)
    {
	comp.p7FFD = u8(x);
	set_banks();
    }
}
//=============================================================================


//=============================================================================
// write to model-specific extended memory port
void editextbank()			//"mon.sethimem" хоткей
{
    if (dbg_extport == -1U)
	return;
    int x = input2(ports_x+5, ports_y+2, dbg_extval);
    if (x != -1)
	out(dbg_extport, (unsigned char)x);
}
//=============================================================================


//=============================================================================
// next window
void mon_nxt()						//"mon.next" хоткей
{
    if		(activedbg == WNDREGS)	activedbg = WNDTRACE;
    else if	(activedbg == WNDTRACE)	activedbg = WNDMEM;
    else if	(activedbg == WNDMEM)	activedbg = WNDREGS;
}
//=============================================================================


//=============================================================================
// prev window...					//"mon.prev" хоткей
void mon_prv()
{
    mon_nxt();
    mon_nxt();
}	
//=============================================================================
// toggle dump	(HEX/Ascii)				//в WNDMEM меню
void mon_dump()						//и "mon.dump" хоткей
{
    mem_dump ^= 1;
    mem_sz = mem_dump ?	32:
			8 ;
}
//=============================================================================


//=============================================================================
//							//в WNDMEM меню
void mon_switch_dump()					//и "mon.switchdump" хоткей
{
    static const u8 DumpModes[] = {	ED_MEM,
					ED_PHYS,
					ED_LOG,
					ED_CMOS,
					ED_NVRAM,
					ED_COMP_PAL
				   };
    static unsigned Idx = 0;
    ++Idx;
    Idx %= ED_MAX;
    editor = DumpModes[ Idx];
}
//=============================================================================

#define tool_x 18
#define tool_y 12

//=============================================================================
// ripper's tool						//в WNDMEM меню
void mon_tool()						//и "mon.rip" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    static unsigned char unref = 0xCF;	//RST $8
    
    //-------------------------------------------------------------------------
    // уже рипнуто?
    if (ripper) 
    {
	OPENFILENAME ofn = {  };
	char savename[0x200]; *savename = 0;
	ofn.lStructSize = (WinVerMajor < 5)  ?	OPENFILENAME_SIZE_VERSION_400 :
						sizeof(OPENFILENAME);
	ofn.lpstrFilter = "Memory dump\0*.bin\0";
	ofn.lpstrFile = savename;
	ofn.nMaxFile = sizeof savename;
	ofn.lpstrTitle = "Save ripped data";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.hwndOwner = wnd;
	ofn.lpstrDefExt = "bin";
	ofn.nFilterIndex = 1;
	
	if (GetSaveFileName(&ofn))
	{
	    for (unsigned i = 0; i < 0x10000; i++)
		snbuf[i] = (cpu.membits[i] & ripper) ?	cpu.DirectRm(i) :
							unref;
	    FILE *ff = fopen(savename, "wb");
	    if (ff)
	    {
		fwrite(	    snbuf,
			    1,
			    0x10000,
			    ff
		       );
		fclose(ff);
	    }
	}
	ripper = 0;
    }
    //-------------------------------------------------------------------------
    // первый запуск?
    else
    {
	//---------------------------------------------------------------------
	filledframe(	tool_x,
			tool_y,
			17,
			6,
			DBG_ATTR_WINDOW_BACK_COLOR
		    );
	tprint(	    	tool_x,
			tool_y,
			"  ripper's tool  ",
			DBG_ATTR_WINDOW_TITLE	//FRM_HEADER
		);
	//---------------------------------------------------------------------
	// trace reads
	tprint(		tool_x+1,
			tool_y+2,
			"trace reads:",
			DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
		);
	*(unsigned*)str = 'Y';
	if (!inputhex( tool_x+15, tool_y+2, 1, false )) return;
	tprint(		tool_x+15,
			tool_y+2,
			str,
			DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
		);
	if (*str == 'Y' || *str == 'y' || *str == '1')	ripper |= MEMBITS_R;
	//---------------------------------------------------------------------
	// trace writes
	*(unsigned*)str = 'N';
	tprint(		tool_x+1,
			tool_y+3,
			"trace writes:",
			DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
		);
	if (!inputhex( tool_x+15, tool_y+3, 1, false )) { ripper = 0; return; }
	tprint(		tool_x+15,
			tool_y+3,
			str,
			DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
		);
	if (*str == 'Y' || *str == 'y' || *str == '1') ripper |= MEMBITS_W;
	//---------------------------------------------------------------------
	// unref. byte
	tprint(		tool_x+1,
			tool_y+4,
			"unref. byte:",
			DBG_ATTR_WINDOW_TEXT	//FFRAME_INSIDE
		);
	int ub;
	if ((ub = input2( tool_x+14, tool_y+4, unref)) == -1) { ripper = 0; return; }
	unref = (unsigned char)ub;
	//---------------------------------------------------------------------
	if (ripper)
	    for (unsigned i = 0; i < 0x10000; i++)
		cpu.membits[i] &= ~(MEMBITS_R | MEMBITS_W);
    }
}
//=============================================================================


//=============================================================================
// emulation settings (GUI)				//"mon.settings" хоткей
void mon_setup_dlg()				
{
#ifdef MOD_SETTINGS			// заход в настройки
    setup_dlg();			// с дополнителььными костылями
    temp.rflags = RF_MONITOR;		// для нормального возврата в дебагер
    set_video();
#endif
}
//=============================================================================


//=============================================================================
// toggle screenshot/scrdump/watches			//"mon.scrshot" хоткей
void mon_scrshot()
{
    show_scrshot++;
    show_scrshot %= 4;
}
//=============================================================================


//=============================================================================
// ыswitch to next cpu					// в WNDTRACE меню
void mon_switch_cpu()					// и "mon.cpu" хоткей
{
//    CpuMgr.CopyToPrev();
    Z80 &cpu0 = CpuMgr.Cpu();
    cpu0.dbgbreak = 0;
    CpuMgr.SwitchCpu();
    Z80 &cpu1 = CpuMgr.Cpu();

    //если не инициализировано?
    if (cpu1.trace_curs == -1U)		cpu1.trace_curs = cpu1.pc;
    if (cpu1.trace_top == -1U)		cpu1.trace_top = cpu1.pc;
    if (cpu1.trace_mode == -1U)		cpu1.trace_mode = 0;

    debugscr();
    debugflip();
}
//=============================================================================

