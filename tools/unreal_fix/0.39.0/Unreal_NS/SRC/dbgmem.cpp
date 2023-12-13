#include "std.h"

#include "emul.h"
#include "vars.h"
#include "debug.h"
#include "dbgpaint.h"
#include "dbgcmd.h"
#include "dbgmem.h"
#include "wd93crc.h"
#include "util.h"

static TRKCACHE edited_track;

static unsigned sector_offset, sector;

//=============================================================================
static void findsector(unsigned addr)
{
   for (sector_offset = sector = 0; sector < edited_track.s; sector++, sector_offset += edited_track.hdr[sector].datlen)
      if (addr >= sector_offset && addr < sector_offset + edited_track.hdr[sector].datlen)
         return;
   errexit("internal diskeditor error");
}
//=============================================================================


//=============================================================================
unsigned char *editam(unsigned addr)
{
   Z80 &cpu = CpuMgr.Cpu();
   if (editor == ED_CMOS)	return &cmos[addr & (sizeof(cmos)-1)];
   if (editor == ED_NVRAM)	return &nvram[addr & (sizeof(nvram)-1)];
   if (editor == ED_COMP_PAL)	return &comp.comp_pal[addr & (sizeof(comp.comp_pal) - 1)];
   if (editor == ED_MEM)	return cpu.DirectMem(addr);
   if (!edited_track.trkd)	return nullptr;
   if (editor == ED_PHYS)	return edited_track.trkd + addr;
   // editor == ED_LOG
	findsector(addr);
	return edited_track.hdr[sector].data + addr - sector_offset;
}
//=============================================================================


//=============================================================================
static void editwm(unsigned addr, unsigned char byte)
{
   if (editrm(addr) == byte) return;
   unsigned char *ptr = editam(addr);
   if (!ptr) return; 
   *ptr = byte;
   if (	(editor == ED_MEM)	||
	(editor == ED_CMOS)	||
	(editor == ED_NVRAM)	||
	(editor == ED_COMP_PAL)
     )
   {
       if (editor == ED_COMP_PAL)
       {
           temp.comp_pal_changed = 1;
       }
       return;
   }
   if (editor == ED_PHYS) { comp.fdd[mem_disk].optype |= 2;	return; }
   comp.fdd[mem_disk].optype |= 1;
   // recalc sector checksum
   findsector(addr);
   *(unsigned short*)(edited_track.hdr[sector].data + edited_track.hdr[sector].datlen) =
      u16(wd93_crc(edited_track.hdr[sector].data - 1, edited_track.hdr[sector].datlen + 1));
}
//=============================================================================


//=============================================================================
unsigned memadr(unsigned addr)
{
   if (editor == ED_MEM)	return (addr & 0xFFFF);
   if (editor == ED_CMOS) 	return (addr & (sizeof(cmos)-1));
   if (editor == ED_NVRAM)	return (addr & (sizeof(nvram)-1));
   if (editor == ED_COMP_PAL)	return (addr & (sizeof(comp.comp_pal) - 1));
   // else if (editor == ED_PHYS || editor == ED_LOG)
   if (!mem_max) return 0;
   while ((int)addr < 0) addr += mem_max;
   while (addr >= mem_max) addr -= mem_max;
   return addr;
}
//=============================================================================


//=============================================================================
void showmem()
{

//переместить в debug.h !!!!
#define DBG_ATTR_BCKGRND		0x00	//0	0
#define DBG_ATTR_BCKGRND_ACTIVE		0x10	//blue	0
#define DBG_ATTR_BCKGRND_BRIGHT		0X80	//br+	0

#define DBG_ATTR_MEM_VALUES		0x0F	//0	br+WHITE
#define DBG_ATTR_MEM_SELECTED		0xB0	//br+MAGENTAA	0

#define DBG_ATTR_MEM_DUMP_TYPE		0x5D//0x70	//white	0
//#define DBG_ATTR_MEM_NGS_DMA		0x70	//white	0


#define DBG_ATTR_MEM_BREAK_EX		0x0A	//0		red

#define DBG_ATTR_MEM_BREAK_R		0x04	//0		red
#define DBG_ATTR_MEM_BREAK_W		0x02	//0		green
#define DBG_ATTR_MEM_BREAK_RW		0x06	//0		yellow
#define DBG_ATTR_MEM_BREAK_R_BACK	0x40	//red		0
#define DBG_ATTR_MEM_BREAK_W_BACK	0x20	//green		0
#define DBG_ATTR_MEM_BREAK_RW_BACK	0x60	//yellow	0


#define DBG_ATTR_MEM_REG_PC		0x0D	//0		br+CYAN
#define DBG_ATTR_MEM_TRACE_CURSOR	0x0B	//0		br+MAGENTA	0x0E	//0		br+YELLOW



// #define DBG_ATTR_REGS_NAMES	0x07	//0	br+WHITE

// #define DBG_ATTR_REGS_SELECTED	0xB0	//br+MAGENTAA	0
// #define DBG_ATTR_REGS_CHANGED	0X0E	//0	br+YELLOW
// #define DBG_ATTR_REGS_DIHALT	0x0A	//0	br+RED
// 
// #define DBG_ATTR_REGS_TITLE	0x51	//cyan blue






    Z80 &cpu = CpuMgr.Cpu();
 
//  char line[80];
    char line[ 80] = "                                                    ";
    
    char value[ 16] = "    ";	//NS	(чтобы красить каждый символ по отдельности)
    
    char char_1[ 16] = " "; 
    
    unsigned ii;
    unsigned cursor_found = 0;
    
    unsigned char current_back_attr = (activedbg == WNDMEM) ?	DBG_ATTR_BCKGRND_ACTIVE :	//(выделенное окно)
								DBG_ATTR_BCKGRND;

    unsigned char attr_values = (current_back_attr | DBG_ATTR_MEM_VALUES);

    filledframe(	mem_x,			// тк не все строки могут отрисовыватсо
			mem_y,
			mem_cx,
			mem_cy,
			current_back_attr );
			
			
    if (mem_dump)
    {
	mem_ascii = 1;
	mem_sz = 32;
    }
    else 
    {
	mem_sz = 8;
    }
    //-------------------------------------------------------------------------
    if (editor == ED_LOG || editor == ED_PHYS)
    {
	edited_track.clear();
	edited_track.seek(comp.fdd+mem_disk, mem_track/2, mem_track & 1, (editor == ED_LOG)? LOAD_SECTORS : JUST_SEEK);
	
	//---------------------------------------------------------------------
	if (!edited_track.trkd)		// no track
	{ 
	    for (ii = 0; ii < mem_size; ii++) 
	    {
			sprintf(line, (ii == mem_size/2)   ?	"          track not found            "	:
								"                                     "	);
		// error
		tprint(		mem_x,
				mem_y+ii,
				line,
				attr_values	//(activedbg == WNDMEM) ?	W_SEL	:
						//				W_NORM
		       );
	    }
	    mem_max = 0;	//| title
	    goto title;		//v down
	}
	//---------------------------------------------------------------------
	mem_max = edited_track.trklen;
	if (editor == ED_LOG)
	    for (mem_max = ii = 0; ii < edited_track.s; ii++)
		mem_max += edited_track.hdr[ii].datlen;
    }
    //-------------------------------------------------------------------------
    else if (editor == ED_MEM)
	mem_max = 0x10000;
    //-------------------------------------------------------------------------
    else if (editor == ED_CMOS)
	mem_max = sizeof(cmos);
    //-------------------------------------------------------------------------
    else if (editor == ED_NVRAM)
	mem_max = sizeof(nvram);
    //-------------------------------------------------------------------------
    else if (editor == ED_COMP_PAL)
	mem_max = sizeof(comp.comp_pal);
    //-------------------------------------------------------------------------
    unsigned div;
    div = mem_dump  ?	32	:
			8	;
    unsigned dx;
    dx = (mem_max + div - 1) / div;
    unsigned mem_lines;
    mem_lines = min(dx, unsigned(mem_size));
    
redraw:

    cpu.mem_curs = memadr(cpu.mem_curs);
    cpu.mem_top = memadr(cpu.mem_top);

    //-------------------------------------------------------------------------
    for (ii = 0; ii < mem_lines; ii++)
    {
	//current addr
	unsigned ptr = memadr( cpu.mem_top + ii * mem_sz);
//	sprintf(	line,
//			"%04X ",
//			ptr
//		);
	
	sprintf(	value,		//NS
			"%04X",
			ptr
		);
		
	int attr_values_1 = attr_values;
	// ROM костылем
	if (bankr[ (ptr >> 14) & 3] != bankw[ (ptr >> 14) & 3]) 
	{
	    attr_values_1 &= 0xF7;
	}
		    
	tprint(		mem_x,		//NS
			mem_y+ii,
			value,
			(	((cpu.mem_addr_edit_mode == 1)	&&
				(activedbg == WNDMEM)		&&
				(cpu.mem_curs == ptr))    ?	DBG_ATTR_MEM_SELECTED :
								attr_values_1 )
		);
	tprint(		mem_x+4,	//NS
			mem_y+ii,
			" ",
			attr_values
		);
		
	unsigned cx = 0;
	
#define	FLAG_PC		0x01
#define	FLAG_BP_X	0x02
#define	FLAG_BP_R	0x04
#define	FLAG_BP_W	0x08



	// сбрасываемые каждую строку переменные	// DF 58<5A>F6 C7 A5 17 40
	// для маркера типа бряка вокруг hex значения		
	// для лучшей видимости типа бряка
	//
	// тип бряка R/W/RW
	int old_break_mark = 0;
	int new_break_mark = 0;
	// тк ink в след раз может использоваться как paper
	int old_break_mark_attr = current_back_attr | (current_back_attr >> 4);
	int new_break_mark_attr = current_back_attr | (current_back_attr >> 4);
	// выходные атрибуты для маркера бряка
	int break_mark_1_attr = current_back_attr;
	int break_mark_2_attr = current_back_attr;
	// изображение бряка
	char break_mark_1[2] = " ";	// ' ' / '>'
	char break_mark_2[2] = " ";	// ' ' / '<'
	
	//---------------------------------------------------------------------
	if (mem_dump)
	{  // 0000 0123456789ABCDEF0123456789ABCDEF
	    for (unsigned dx = 0;    dx < 32;    dx++)
	    {
	    		//подсветка бряков и PC и курсора		//NS
			unsigned char res = cpu.membits[ ptr];		
			unsigned char mem_color = attr_values;
			if (editor == ED_MEM)
			{
			    if		(ptr == cpu.pc)		mem_color = (current_back_attr | 0x0D);	//5
			    else if	(ptr == cpu.trace_curs)	mem_color = (current_back_attr | 0x0E);	//6
			    else if	(res & MEMBITS_BPX)	mem_color = (current_back_attr | 0x0A);	//2
			    else if	(res & MEMBITS_BPR)	mem_color = (current_back_attr | 0x0C);	//4
			    else if	(res & MEMBITS_BPW)	mem_color = (current_back_attr | 0x0B);	//3
			    else if 	(bankr[ (ptr >> 14) & 3] != bankw[ (ptr >> 14) & 3])
								mem_color = (current_back_attr | 0x07);	//7 ROM
			    
			    
			}
		if (ptr == cpu.mem_curs) 
		{
		    cx = dx + 5;
		}
		unsigned char c = editrm( ptr);
		ptr = memadr( ptr + 1);
		//line[dx+5] = char(c ? c : '.' );
					
			*char_1 = char(	c ? c : '.' );
			tprint(		(mem_x + dx + 5),	// [NS]
					mem_y+ii,
					char_1,
					mem_color	//attr_values
				);
				
	    }
	} // тип 0000 0123456789ABCDEF0123456789ABCDEF
	//---------------------------------------------------------------------
	else 
	{  // 0000 11 22 33 44 55 66 77 88 abcdefgh
	    for (unsigned dx = 0;    dx < 8;    dx++)
	    {
	    	//подсветка бряков и PC и курсора		// [NS]
		unsigned char res = cpu.membits[ ptr];;		
		unsigned char attr1 = attr_values;

		*break_mark_1 = ' ';	//nope / >
		*break_mark_2 = ' ';	//nope / <
		    
		// редактор CPU memory
		if (editor == ED_MEM)
		{
		    int temp_line_flags = 0;
		    old_break_mark = new_break_mark;
		    old_break_mark_attr = new_break_mark_attr;
		    
		    new_break_mark = 0;
			
		    //-------------------------------------------------
		    // сборка флагов для выбора цвета и типа маркера
		    if (res & MEMBITS_BPX) { temp_line_flags |= FLAG_BP_X; }	//NO! new_break_mark
		    if (res & MEMBITS_BPR) { temp_line_flags |= FLAG_BP_R;	new_break_mark |= FLAG_BP_R; }
		    if (res & MEMBITS_BPW) { temp_line_flags |= FLAG_BP_W;	new_break_mark |= FLAG_BP_W; }	
		    //-------------------------------------------------
		    // выбор атрибута для текущего значения
		    // и атрибута для маркера типа бряка
		    switch (temp_line_flags)
		    {
			//X only --------------------------------------
			case (FLAG_BP_X):				//...O
			    attr1 = (attr1 & 0xF0) | DBG_ATTR_MEM_BREAK_EX;	
			    break;	//+ back
			//RW ------------------------------------------
			case (FLAG_BP_R):				//..O.
			    attr1 = (attr1 & 0x0F) | DBG_ATTR_MEM_BREAK_R_BACK;	//+ ink
			    new_break_mark_attr = current_back_attr | DBG_ATTR_MEM_BREAK_R;
			    break;	
			case (FLAG_BP_W):				//.O..
			    attr1 = (attr1 & 0x0F) | DBG_ATTR_MEM_BREAK_W_BACK;	//+ ink
			    new_break_mark_attr = current_back_attr | DBG_ATTR_MEM_BREAK_W;
			    break;	
			case (FLAG_BP_R | FLAG_BP_W):		//.OO.
			    attr1 = (attr1 & 0x0F) | DBG_ATTR_MEM_BREAK_RW_BACK;	//+ ink
			    new_break_mark_attr = current_back_attr | DBG_ATTR_MEM_BREAK_RW;
			    break;	
			//RW + X --------------------------------------
			case (FLAG_BP_X | FLAG_BP_R):		//..OO
			    attr1 = DBG_ATTR_MEM_BREAK_EX | DBG_ATTR_MEM_BREAK_R_BACK;
			    new_break_mark_attr = current_back_attr | DBG_ATTR_MEM_BREAK_R;
			    break;	
			case (FLAG_BP_X | FLAG_BP_W):		//.O.O
			    attr1 = DBG_ATTR_MEM_BREAK_EX | DBG_ATTR_MEM_BREAK_W_BACK;
			    new_break_mark_attr = current_back_attr | DBG_ATTR_MEM_BREAK_W;
		    break;	
			case (FLAG_BP_X | FLAG_BP_R | FLAG_BP_W):	//.OOO
			    attr1 = DBG_ATTR_MEM_BREAK_EX | DBG_ATTR_MEM_BREAK_RW_BACK;
			    new_break_mark_attr = current_back_attr | DBG_ATTR_MEM_BREAK_RW;
			    break;
			default:
			    new_break_mark_attr = current_back_attr | (current_back_attr >> 4);
		    }
		    //-------------------------------------------------
		    // атрибуты для курсора и PC
		    if (ptr == cpu.trace_curs)	attr1 = (attr1 & 0XF0) | DBG_ATTR_MEM_TRACE_CURSOR;
		    if (ptr == cpu.pc) 		attr1 = (attr1 & 0XF0) | DBG_ATTR_MEM_REG_PC;
		    //-------------------------------------------------
		    // ROM костылем
		    if ( (bankr[ (ptr >> 14) & 3] != bankw[ (ptr >> 14) & 3]) &&
			 ((attr1 & 0X0F) == 0x0F)
		     )
		    {
			attr1 &= 0xF7;
		    }
		    //-------------------------------------------------
		    // разные изображения для единичного и серии маркеров
		    // и разные атрибуты для них
		    //
		    if (new_break_mark)	//начало нового типа маркера
		    {
			if (old_break_mark != new_break_mark)
			{
			    //new mode
			    *break_mark_1 = 0x11;	//	'<';	
			    *break_mark_2 = 0x10;	//	'>';
			    //paper = предыдущий ink
			    break_mark_1_attr =	 (0x0F & new_break_mark_attr) |
						((0x0F & old_break_mark_attr) << 4);
			}
			else
			{
			    //previous mode
			    *break_mark_1 = 0xDB;	//	'O';	
			    *break_mark_2 = 0x10;	//	'>';
			    break_mark_1_attr = new_break_mark_attr;
			}
			
			break_mark_2_attr = new_break_mark_attr;
		    }
		    else		//конец маркера или его отсутствие
		    {
			if (old_break_mark)
			{
			    //previous mode ends
			    *break_mark_1 = 0x10;	//	'>';	
			    *break_mark_2 = 0x20;	//	' ';
			}
			else
			{
			    //nope
			    *break_mark_1 = 0x20;	//	' ';	
			    *break_mark_2 = 0x20;	//	' ';
			}
				
				break_mark_1_attr = old_break_mark_attr;
				break_mark_2_attr = old_break_mark_attr;
		    }
		    //-------------------------------------------------

		// MEMBITS_R | MEMBITS_W | MEMBITS_X флаги не ставятсо в режиме дебагера
		// (хотя возможно они ставятсо при выходе из дебагера и сразу все или раз в фрейм)
		// (или как то перекидываютсо из другого масива)
		// а включенный меморри бенд еще и затирает их постоянно
		// так что нужно больше костылей
			
		} //(editor == ED_MEM)
		//-----------------------------------------------------		

			
		//-----------------------------------------------------
		// отрисовка
		
		if (ptr == cpu.mem_curs)
		{
		    cx = (mem_ascii) ?	dx + 29				:
					dx * 3 + 5 + cpu.mem_second	;
		}
		unsigned char c = editrm(ptr);
		ptr = memadr(ptr+1);
	//	sprintf(	line + 5 + (3*dx),
	//			"%02X",c
	//		);
	//	line[7+3*dx] = ' ';				//:rofl: 
	//	line[29+dx] = char(	c   ?	c	:
	//					'.'	);
		
		//-------------------------------------------------------------
		// hex
		sprintf(	value,				// [NS]
				"%02X",c
			);
		tprint(		(mem_x + 5 + (3 * dx)),		// [NS]
				mem_y + ii,
				value,
				attr1	//mem_color	//attr_values
			);
		//-------------------------------------------------------------
		// маркеры типа бряка
		tprint(		(mem_x + 4 + (3*dx)),		//NS
				mem_y+ii,
				break_mark_1,	//" ","<"
				break_mark_1_attr	//attr_values
			);			
		tprint(		(mem_x + 7 + (3*dx)),		//NS
				mem_y+ii,
				break_mark_2,	//" ","<"
				break_mark_2_attr	//attr_values
			);
		//-----------------------------------------------------
		//ascii превиев в конце
		*char_1 = char(	c   ?	c	:	//замена 0 пустоты на видимый '.'
					'.'	);
		tprint(		(mem_x + 29+dx),		//NS
				mem_y+ii,
				char_1,
				attr1	//attr_values
			);
		//-----------------------------------------------------
						
	    }
	} // тип 0000 11 22 33 44 55 66 77 88 abcdefgh
	//---------------------------------------------------------------------
	
	
	//---------------------------------------------------------------------
	line[37] = 0;
	
	// отрисовка сразу всей, блджд, строки
//	tprint(		mem_x,
//			mem_y+ii,
//			line,
//			(activedbg == WNDMEM) ?	W_SEL	:
//						W_NORM
//		);
	//---------------------------------------------------------------------
	// разукрашка курсора
	cursor_found |= cx;

	if (	cx			&&
		(activedbg == WNDMEM)	&&
		(!cpu.mem_addr_edit_mode)
	  )
	{
	    txtscr[ ((mem_y + ii) * s80) + mem_x + cx + (s80 * s30) ] = DBG_ATTR_MEM_SELECTED;	//W_CURS;
	}    
    }
    //-------------------------------------------------------------------------

    //слежение за курсором улетающим вверх
    if (!cursor_found) {	cursor_found = 1;
				cpu.mem_top = cpu.mem_curs & ~(mem_sz-1);
				goto redraw;
			}
    //-------------------------------------------------------------------------

title:
    const char *MemName = nullptr;
    //-------------------------------------------------------------------------
    if (editor == ED_MEM)
    {
	//---------------------------------------------------------------------
	//MemName = "Memory";
	if ( (CpuMgr.GetCurrentCpu()) == 0 )	// [NS]
	{
	    MemName = "ZX-CPU";	//"ZX-Z80";	//"ZX_CPU";
	}
	//---------------------------------------------------------------------
	else
	{
	    MemName = "GS-CPU";	//"GS-Z80";	//"GS_CPU";
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    else if (editor == ED_CMOS)
	MemName = "CMOS";
    //-------------------------------------------------------------------------
    else if (editor == ED_NVRAM)
	MemName = "NVRAM";
    //-------------------------------------------------------------------------
    else if(editor == ED_COMP_PAL)
	MemName = "CompPal";
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    if (	editor == ED_MEM	||
		editor == ED_CMOS	||
		editor == ED_NVRAM	||
		editor == ED_COMP_PAL
      )
    {
	sprintf(	line,
			"%s: %04X    NGS_DMA: %06X",
			MemName,
			cpu.mem_curs & 0xFFFF,
			temp.gsdmaaddr
		);
    }
    //-------------------------------------------------------------------------
    else if (editor == ED_PHYS)
	sprintf(	line,
			"Disk %c, Trk %02X, Offs %04X",
			int(mem_disk+'A'),
			mem_track,
			cpu.mem_curs
		);
    //-------------------------------------------------------------------------
    else		// ED_LOG
    { 
	if (mem_max)
	    findsector(cpu.mem_curs);
	    
	sprintf(	line,
			"Disk %c, Trk %02X, Sec %02X[%02X], Offs %04X",
			int(mem_disk+'A'),
			mem_track,
			sector,
			edited_track.hdr[sector].n,
			cpu.mem_curs-sector_offset
		);
    }
    //-------------------------------------------------------------------------
    tprint(	mem_x,
		mem_y-1,
		line,
		DBG_ATTR_MEM_DUMP_TYPE	//W_TITLE
	   );
    //-------------------------------------------------------------------------
    frame(	mem_x,
		mem_y,
		37,
		mem_size,
		FRAME
	 );
    //-------------------------------------------------------------------------
}
//=============================================================================


      /* ------------------------------------------------------------- */


//=============================================================================
// start of line
void mstl()						//"mem.stline" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    if (mem_max)
    {
        cpu.mem_curs &= ~(mem_sz - 1);
        cpu.mem_second = 0;
    }
}
//=============================================================================
// end of line						//"mem.endline" хоткей
void mendl()						
{
    Z80 &cpu = CpuMgr.Cpu();
    if(mem_max)
    {
        cpu.mem_curs |= (mem_sz - 1);
        cpu.mem_second = 1;
    }
}
//=============================================================================
void mup()						//"mem.up" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();

	if (mem_max) cpu.mem_curs -= mem_sz;    
}
//=============================================================================
void mpgdn()						//"mem.pgdn" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    if (mem_max)
    {
        cpu.mem_curs += mem_size * mem_sz;
        cpu.mem_top += mem_size * mem_sz;
    }
}
//=============================================================================
void mpgup()						// "mem.pgup" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    if (mem_max)
    {
        cpu.mem_curs -= mem_size * mem_sz;
        cpu.mem_top -= mem_size * mem_sz;
    }
}
//=============================================================================
void mdown()						// "mem.down" хоткей
{
    if (!mem_max) return;

    Z80 &cpu = CpuMgr.Cpu();
	//dump
	cpu.mem_curs += mem_sz;
	if (((cpu.mem_curs - cpu.mem_top + mem_max) % mem_max) / mem_sz >= mem_size) cpu.mem_top += mem_sz;
}
//=============================================================================
void mleft()						// "mem.left" хоткей
{
    if (!mem_max) return;
    
    if (!cpu.mem_addr_edit_mode)
    {
	Z80 &cpu = CpuMgr.Cpu();
	if (mem_ascii || !cpu.mem_second) cpu.mem_curs--;
	if (!mem_ascii) cpu.mem_second ^= 1;
    }
}
//=============================================================================
void mright()						// "mem.right" хоткей
{
   Z80 &cpu = CpuMgr.Cpu();
   if (!mem_max) return;
   
    if (!cpu.mem_addr_edit_mode)
    {
	if (mem_ascii || cpu.mem_second) cpu.mem_curs++;
	if (!mem_ascii) cpu.mem_second ^= 1;
	if (((cpu.mem_curs - cpu.mem_top + mem_max) % mem_max) / mem_sz >= mem_size) cpu.mem_top += mem_sz;
    }
    else
    {
	cpu.mem_addr_edit_mode = 0;
	cpu.mem_second = 0;
    }
}
//=============================================================================


// mem_max - размер всей памяти (для 0...FFFF = 10000)
// mem_sz - размер строки
// cpu.mem_top - адрес начала самой верхней видимой строки
// cpu.mem_curs - адрес курсора

//=============================================================================
// по аналогии с center()
void menter()	// edit addr pos	"mem.addr_edit" хоткей		// [NS]
{
    //-------------------------------------------------------------------------
    // работает только когда курсор над адресом?
    if (cpu.mem_addr_edit_mode)
    {
	Z80 &cpu = CpuMgr.Cpu();
	//---------------------------------------------------------------------
	if (!mem_sz)	//на всякий случай защита от /0
	    return;
	//---------------------------------------------------------------------
	int y_coord;
	//---------------------------------------------------------------------
	if (cpu.mem_curs >= cpu.mem_top)
	{
	    y_coord = (mem_y + ((cpu.mem_curs - cpu.mem_top) / mem_sz));
	//			(0030 - 0000) / 10 = 3
	//			0 0000 top
	//			1 0010
	//			2 0020
	//			3 0030 <- cursor
	}
	//---------------------------------------------------------------------
	else
	{
	    y_coord = (mem_y + ((cpu.mem_curs - (cpu.mem_top - mem_max)) / mem_sz));
	//			(0030 - (FFF0 - 10000)) / 10
	//			(0030 - (-10)) / 10 = 4
	//			0 FFF0 top
	//			1 0000
	//			2 0010
	//			3 0020
	//			4 0030 <- cursor
	}
	//---------------------------------------------------------------------
	//тк возможны падения при неправильной координате то ограничение
	if (y_coord < mem_y)		y_coord = mem_y;
	//---------------------------------------------------------------------
	if (y_coord > (mem_y + 11) )	y_coord = (mem_y + 11);
	//---------------------------------------------------------------------
//	printf("mem_y %d ",mem_y);
//	printf("cpu.mem_curs %d ",cpu.mem_curs);
//	printf("cpu.mem_top %d ",cpu.mem_top);
//	printf("mem_sz %d ",mem_sz);	
//	printf("y_coord %d ",y_coord);
//	printf("mem_max %d ",mem_max);
	//y_coord 536862737 !!! crash

//crsh03
//mem_y 15
//cpu.mem_curs 8
//cpu.mem_top 65496
//mem_sz 8
//y_coord 536862741

//	int y_coord = (15 + ((8 - 65496) / 8)); = -8171
	
// mem_top может быть F000 когда курсор 0010 !!!!!!!!

	//---------------------------------------------------------------------
	// если мы не зашли в режим редактирования по enter (нужно проверять menter hotkey !!!!!)
	// значет мы зашли туда началом ввода
	// и надо сразу ввести что мы вводли
	if (input.lastkey != VK_RETURN)
	{
	    // шляпа - ущербный ввод значения которым мы вошли в режим редактирования
	    PostThreadMessage(	GetCurrentThreadId(),	//????????
				WM_KEYDOWN,
				input.lastkey,
				1
			  );
	}
	//---------------------------------------------------------------------
			  

	int v = input4(	mem_x,
			y_coord,//mem_y,
			cpu.mem_curs
		       );
	//---------------------------------------------------------------------
	if (v != -1)
	{
	    cpu.mem_top = (unsigned(v) & ~(mem_sz - 1));
	    cpu.mem_curs = (unsigned(v) & ~(mem_sz - 1));	//unsigned(v);
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------

}
//=============================================================================


//=============================================================================
char dispatch_mem()				//правка в окне мемори виевера
{
   Z80 &cpu = CpuMgr.Cpu();
   if (!mem_max)
       return 0;


    if (!cpu.mem_addr_edit_mode)
    {
	u8 Kbd[256];
	GetKeyboardState(Kbd);
	unsigned short k = 0;
	if (ToAscii(input.lastkey,0,Kbd,&k,0) != 1)
	    return 0;

	if (mem_ascii)
	{
	    k &= 0xFF;
	    if (k < 0x20 || k >= 0x80)
		return 0;
	    editwm(cpu.mem_curs, (unsigned char) k);
	    mright();
	    return 1;
	}
	else
	{
	    u8 u = u8(toupper(k));
	    if ((u >= '0' && u <= '9') || (u >= 'A' && u <= 'F'))
	    {
		unsigned char k = (u >= 'A') ? u - 'A'+10 : u - '0';
		unsigned char c = editrm(cpu.mem_curs);
		if (cpu.mem_second)
		    editwm(cpu.mem_curs, (c & 0xF0) | k);
		else
		    editwm(cpu.mem_curs, u8(c & 0x0F) | u8(k << 4));
		mright();
		return 1;
	    }
	}
    } //if (!cpu.mem_addr_edit_mode)
    else
    {
	if ((input.lastkey >= '0' && input.lastkey <= '9') || (input.lastkey >= 'A' && input.lastkey <= 'F'))
	{
	    menter();	    
	    return 1;
	}
    }
   
   return 0;
}
//=============================================================================





//=============================================================================
// find text in memory / disk track			// меню в WNDMEM окне
void mtext()						// и "mem.findtext" хоткей
{
   Z80 &cpu = CpuMgr.Cpu();
   int rs = find1dlg(cpu.mem_curs);
   if (rs != -1) cpu.mem_curs = unsigned(rs);
}
//=============================================================================
// find code with mask in memory / disk track
void mcode()						// и "mem.findcode" хоткей
{
   Z80 &cpu = CpuMgr.Cpu();
   int rs = find2dlg(cpu.mem_curs);
   if (rs != -1) cpu.mem_curs = unsigned(rs);
}
//=============================================================================


//=============================================================================
// goto address						// меню в WNDMEM окне
void mgoto()						// и "mem.goto" хоткей
{
   Z80 &cpu = CpuMgr.Cpu();
   int v = input4(mem_x, mem_y, cpu.mem_top);
   if(v != -1)
   {
       cpu.mem_top = (unsigned(v) & ~(mem_sz - 1));
       cpu.mem_curs = unsigned(v);
   }
}
//=============================================================================
// move to hex/ascii					// "mem.switch" хоткей
// переход мехду столбцемм hex и ascii (щас не актуален) 
void mswitch() { mem_ascii ^= 1; }			
//=============================================================================
// goto SP в окне мемори виевира			// меню в WNDMEM окне
void msp()						// и "mem.sp" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    cpu.mem_curs = cpu.sp;
}
//=============================================================================
// goto PC в окне мемори виевира
void mpc()						// и "mem.pc" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    cpu.mem_curs = cpu.pc;
}
//=============================================================================
// goto BC в окне мемори виевира
void mbc()						// "mem.bc" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    cpu.mem_curs = cpu.bc;
}
//=============================================================================
// goto DE в окне мемори виевира
void mde()						// "mem.de" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    cpu.mem_curs = cpu.de;
}
//=============================================================================
// goto HL (в окне мемори виевира)
void mhl()						// "mem.hl" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    cpu.mem_curs = cpu.hl;
}
//=============================================================================
// goto IX (в окне мемори виевира)
void mix()						// "mem.ix" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    cpu.mem_curs = cpu.ix;
}
//=============================================================================
// goto IY (в окне мемори виевира)
void miy()						// "mem.iy" хоткей
{
    Z80 &cpu = CpuMgr.Cpu();
    cpu.mem_curs = cpu.iy;
}
//=============================================================================
// select memory editor
void mmodemem() { editor = ED_MEM; }			// "mem.mem" хоткей
//=============================================================================
// select disk editor (physical track)
void mmodephys() { editor = ED_PHYS; }			// "mem.diskphys" хоткей
//=============================================================================
// select disk editor (logical sectors)
void mmodelog() { editor = ED_LOG; }			// "mem.disklog" хоткей
//=============================================================================
// select drive/track/sector in diskeditor
void mdiskgo()						// "mem.diskgo" хоткей
{
   Z80 &cpu = CpuMgr.Cpu();
   if (editor == ED_MEM) return;
   //дописать еще !!!!!
   // точнее режиммы ппри каких работать
   // а не ппри кааких выходитть !!!

// bug !!!!!   
// "mem.diskgo" хоткей сррабатывает во всех режимах меморриввиевераа крроме мем
// в тех которых трижды не должен !!!!

   for (;;) {
      *(unsigned*)str = mem_disk + 'A';
      if (!inputhex(mem_x+5, mem_y-1, 1, true)) return;
      if (*str >= 'A' && *str <= 'D') break;
   }
   mem_disk = unsigned(*str-'A'); showmem();
   int x = input2(mem_x+12, mem_y-1, mem_track);
   if (x == -1) return;
   mem_track = unsigned(x);
   if (editor == ED_PHYS) return;
   showmem();
   // enter sector
   for (;;) {
      findsector(cpu.mem_curs); x = input2(mem_x+20, mem_y-1, sector);
      if (x == -1) return; if (unsigned(x) < edited_track.s) break;
   }
   for (cpu.mem_curs = 0; x; x--) cpu.mem_curs += edited_track.hdr[x-1].datlen;
}
//=============================================================================

//=============================================================================
// set breakpoint				// в меню WNDTRACE дебагера
void mbpx()					// [NS]
{			
    Z80 &cpu = CpuMgr.Cpu();
    cpu.membits[ cpu.mem_curs] ^= MEMBITS_BPX;
}
//=============================================================================v
void mbpr()					// в меню WNDTRACE дебагера
{						// [NS]
    Z80 &cpu = CpuMgr.Cpu();			
    cpu.membits[ cpu.mem_curs] ^= MEMBITS_BPR;
}
//=============================================================================
void mbpw()					// в меню WNDTRACE дебагера
{						// [NS]
    Z80 &cpu = CpuMgr.Cpu();			
    cpu.membits[cpu.mem_curs] ^= MEMBITS_BPW;
}
//=============================================================================
