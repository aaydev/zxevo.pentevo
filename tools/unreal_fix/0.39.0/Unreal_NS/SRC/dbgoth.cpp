#include "std.h"

#include "resource.h"
#include "emul.h"
#include "vars.h"
#include "debug.h"
#include "dbgpaint.h"
#include "dbgoth.h"
#include "gui.h"
#include "fdd.h"
#include "util.h"

//=============================================================================
namespace z80dbg
{
__int64 __cdecl delta();
__int64 __cdecl delta()
{
    return comp.t_states + cpu.t - cpu.debug_last_t;
}
}
//=============================================================================


#define DBG_ATTR_BCKGRND		0x00	//0	0
#define DBG_ATTR_BCKGRND_ACTIVE		0x10	//blue	0



#define DBG_ATTR_PORTS_48_LOCK		0x0A	//b	RED


#define DBG_ATTR_PAGES_ROM		0X07	//0	
#define DBG_ATTR_PAGES_RAM		0X0F	//0	WHITE
#define DBG_ATTR_PAGES_NMB		0x0C	//0	GREEN

#define DBG_ATTR_BETA128_VALUES		0X0F	//0	WHITE
#define DBG_ATTR_BETA128_NAMES		0x0C	//0	GREEN
#define DBG_ATTR_BETA128_OFF		0x09	//0	GREEN

#define DBG_ATTR_PORTS_VALUES		0X0F	//0	WHITE
#define DBG_ATTR_PORTS_NAMES		0x0C	//0	GREEN

#define DBG_ATTR_WATCH_VALUES		0X09//F	//0	
#define DBG_ATTR_WATCH_NAMES		0x0C	//0	

#define DBG_ATTR_STACK_VALUES		0x0F	//0	
#define DBG_ATTR_STACK_NAMES		0x0C	//0
#define DBG_ATTR_STACK_NAME_SP		0x0E	//0	

#define DBG_ATTR_AY_VALUES_IO		0x0D	//0	//выбранный/активный регистр
#define DBG_ATTR_AY_VALUES		0x0F	//0
	
#define DBG_ATTR_AY_NAMES		0x0C	//0

#define DBG_ATTR_DOS_PORTS_MARK		0x5A

#define DBG_ATTR_TITLES			0x5D//0x71	//white	0


//=============================================================================
void show_time()
{

//переместить в debug.h !!!!
#define	DBG_ATTR_TIME_DELTA		0x0F	//0x0F	//0x0F
#define	DBG_ATTR_TIME_DELTA_DARK	0x0C //0x0C	//0x0F

    unsigned char current_back_attr = (activedbg == WND_TIME_DELTA)  ?	DBG_ATTR_BCKGRND_ACTIVE : //(выделенное окно)
									DBG_ATTR_BCKGRND;					
//  unsigned char attr_values = (current_back_attr | DBG_ATTR_PORTS_VALUES);
//  unsigned char attr_names = (current_back_attr | DBG_ATTR_PORTS_NAMES);  
    
    

    Z80 &cpu = CpuMgr.Cpu();
    tprint(	time_x,
		time_y,
		"Time delta:",
		( DBG_ATTR_TIME_DELTA_DARK | current_back_attr )	//W_OTHEROFF
	  );
    char text[ 32];

    __int64 temp_cpu_delta = cpu.Delta();
    //-------------------------------------------------------------------------
    if ((temp_cpu_delta > 99999'99999'999 ) || ( temp_cpu_delta < 0 ))	//сокращаем до 8 дней в турбе?			
	temp_cpu_delta = 99999'99999'999;				//а было до 80 дней в турбе
    //-------------------------------------------------------------------------
    sprintf(	text,
		"%14I64d",
		temp_cpu_delta //cpu.Delta()
	   );
    tprint(	time_x + 10,	//11,
		time_y,
		text,
		( DBG_ATTR_TIME_DELTA | current_back_attr )	//W_OTHER
	  );
    tprint(	time_x + 24,	//25,
		time_y,
		".t",
		( DBG_ATTR_TIME_DELTA_DARK | current_back_attr ) //W_OTHEROFF
	  );
    frame(	time_x,
		time_y,
		26,
		1,
		FRAME
	);
}
//=============================================================================



//=============================================================================
// отрисовка 1-й линии вачесов
static void wtline(const char *name, unsigned ptr, unsigned y)
{
    unsigned char current_back_attr = (activedbg == WND_WATCHES)   ?	DBG_ATTR_BCKGRND_ACTIVE : //(выделенное окно)
									DBG_ATTR_BCKGRND;								
    char line[ 40];
    //-------------------------------------------------------------------------
    if (name)
	sprintf( line, "%3s: ", name);
    //-------------------------------------------------------------------------
    else
	sprintf( line, "%04X ", ptr);
    //-------------------------------------------------------------------------
    tprint(	wat_x,
		wat_y + y,
		line,
		( DBG_ATTR_WATCH_NAMES | current_back_attr )	//W_OTHER
	   );
	  
    Z80 &cpu = CpuMgr.Cpu();
    //-------------------------------------------------------------------------
    for (unsigned dx = 0;    dx < 8;    dx++)
    {
	unsigned char c = cpu.DirectRm( ptr++);
	sprintf( line + 5 + (3 * dx), "%02X", c);
//	sprintf(line+3*dx, "%02X", c);
	line[ 7 + (3 * dx)] = ' ';
	line[ 29 + dx] = char( c  ?  c   :
				    '.' );
    }
    //-------------------------------------------------------------------------
    line[ 37] = 0;
    tprint(	wat_x + 5,
		wat_y + y,
		line + 5,
		( DBG_ATTR_WATCH_VALUES | current_back_attr )	//W_OTHER
	   );
}
//=============================================================================


//=============================================================================
void showwatch()
{
    //-------------------------------------------------------------------------
    // вместо того чтобы рисовать превиев поверх
    // в унриале НАХУЯТО рисовали превиев первым
    // а в месте превиева естьквадрат с "прозрачным" атрибутом...
    // :face_palm:
    // 
    // НО так у нас 481 1байт записей + 2,3К проверок
    //
    // а без прозрачного кубика будет
    // +15392 чтений таблицы +15392 записей 32битов
    // что возможно будет медленее
    if (show_scrshot)
    {
	for (unsigned y = 0;    y < wat_sz;    y++)
	    for (unsigned x = 0;    x < 37;    x++)
		txtscr[ s80 * s30 +  (wat_y + y) * s80 + (wat_x + x)] = 0xFF;
    }
    //-------------------------------------------------------------------------
    else 
    {
	Z80 &cpu = CpuMgr.Cpu();
	/*
	wtline( "PC",	cpu.pc,		0 );
	wtline( "SP",	cpu.sp,		1 );
	wtline( "BC",	cpu.bc,		2 );
	wtline( "DE",	cpu.de,		3 );
	wtline( "HL",	cpu.hl,		4 );
	wtline( "IX",	cpu.ix,		5 );
	wtline( "IY",	cpu.iy,		6 );
	wtline( "BC'",	cpu.alt.bc,	7 );
	wtline( "DE'",	cpu.alt.de,	8 );
	wtline( "HL'",	cpu.alt.hl,	9 );
	*/

	wtline( "BC ",	cpu.bc,		0 );
	wtline( "DE ",	cpu.de,		1 );
	wtline( "HL ",	cpu.hl,		2 );
	wtline( "BC'",	cpu.alt.bc,	3 );
	wtline( "DE'",	cpu.alt.de,	4 );
	wtline( "HL'",	cpu.alt.hl,	5 );
	wtline( "IX ",	cpu.ix,		6 );
	wtline( "IY ",	cpu.iy,		7 );
	wtline( "SP ",	cpu.sp,		8 );
	wtline( "PC ",	cpu.pc,		9 );

	wtline( nullptr, user_watches[0], 10 );
	wtline( nullptr, user_watches[1], 11 );
	wtline( nullptr, user_watches[2], 12 );
    }
    //-------------------------------------------------------------------------
    const char *text = "Watches";
    //-------------------------------------------------------------------------
    switch (show_scrshot)
    {
	case 1: text = "Screen Memory";		break;
	case 2: text = "Shadow Screen Memory";	break;
	case 3: text = "Ray-Painted";		break;
    }
    //-------------------------------------------------------------------------
    tprint(	wat_x,
		wat_y - 1,
		text,
		DBG_ATTR_TITLES		//W_TITLE
	  );
    //-------------------------------------------------------------------------
    if (comp.flags & CF_DOSPORTS)
    {
	tprint(		wat_x + 34,
			wat_y - 1,
			"DOS",
			DBG_ATTR_DOS_PORTS_MARK	//W_DOS
	      );
    }
    frame(	wat_x,
		wat_y,
		37,
		wat_sz,
		FRAME
	  );
}
//=============================================================================



//=============================================================================
// set user-defined watch address
// 4000 8000 С000 в окне вачесов			// "mon.setwatch" хоткей
void mon_setwatch()				
{
    //-------------------------------------------------------------------------
    if (show_scrshot)
	show_scrshot = 0;
    //-------------------------------------------------------------------------
    for (unsigned i = 0;    i < 3;    i++)
    {
	debugscr();
	int addr = input4(	wat_x,
				wat_y + wat_sz - 3 + i,
				user_watches[ i]
			  );
	//---------------------------------------------------------------------
	if (addr == -1)
	    return;
	//---------------------------------------------------------------------
	user_watches[ i] = unsigned( addr);
    }
    //-------------------------------------------------------------------------
}
//=============================================================================



//=============================================================================
void showstack()
{
    unsigned char current_back_attr = (activedbg == WND_STACK)   ?	DBG_ATTR_BCKGRND_ACTIVE : //(выделенное окно)
									DBG_ATTR_BCKGRND;	
    Z80 &cpu = CpuMgr.Cpu();
    int sp_pos;
    //-------------------------------------------------------------------------
    for (unsigned i = 0;    i < stack_size;    i++)
    {
	sp_pos = i - 3;
    
	char xx[ 10]; 	//-2:1234
			//SP:1234
			//+2:
	//---------------------------------------------------------------------
	if (sp_pos < 0) //(!i)
	{
	    //*(unsigned*)xx = WORD2('-','2');
	    //sprintf(	xx,"-2:");
	    sprintf(	xx,
			"-%X:",( (abs( sp_pos)) * 2)
		    );
	}
	//---------------------------------------------------------------------
	else if	(sp_pos == 0) //(i == 1)
	{
	    //*(unsigned*)xx = WORD2('S','P');
	    sprintf(	xx,"SP:");
	}
	//---------------------------------------------------------------------
	else
	{
	    sprintf(	xx,
			(sp_pos > 8) ?	"%X:" :
					"+%X:",
			((sp_pos) * 2)
		    );
	}
	//---------------------------------------------------------------------
	tprint(		stack_x,
			stack_y + i,
			xx,
			(sp_pos == 0) ? (DBG_ATTR_STACK_NAME_SP | current_back_attr) :
					(DBG_ATTR_STACK_NAMES   | current_back_attr)	//W_OTHER
		);
	//-------------------------------------------------------------------------	
	
	sprintf(	xx,
			"%02X%02X",
			cpu.DirectRm( cpu.sp + (sp_pos) * 2 + 1),
			cpu.DirectRm( cpu.sp + (sp_pos) * 2)
		);
	tprint(		stack_x + 3,
			stack_y + i,
			xx,
			(DBG_ATTR_STACK_VALUES | current_back_attr)	//W_OTHER
		);
    }
    //-------------------------------------------------------------------------
    tprint(	stack_x,
		stack_y - 1,
		"Stack",
		DBG_ATTR_TITLES		//W_TITLE
	   );
    //-------------------------------------------------------------------------
    frame( stack_x, stack_y, 7, stack_size, FRAME);
    //-------------------------------------------------------------------------
}
//=============================================================================



//=============================================================================
void show_ay()
{
    unsigned char current_back_attr = (activedbg == WND_AY)  ?	DBG_ATTR_BCKGRND_ACTIVE : //(выделенное окно)
								DBG_ATTR_BCKGRND;
									
    //-------------------------------------------------------------------------
    // if (!conf.sound.ay_scheme)	// так в дебагере дырка !!!
    //	   return;
    //-------------------------------------------------------------------------
	//	"0123456789_123456789_123456789_123456789_12345678",
	//	"A:xxxx:xx B:xxxx:xx C:xxxx:xx N:xx Mxx E:xxxx S:xx i:xx o:xx
	//	"A:xxx:xx B:xxx:xx C:xxx:xx N:xx Mxx E:xxxx S:xx i:xx o:xx
	//	"A****:** B****:** C****:** N** M** E****:** ****",

		
//					x,	y,	reg
    int ay_map[16][3] = {	{	1,	0,	0x01	},	//A
				{	3,	0,	0x00	},
				{	6,	0,	0x08	},
					               
				{	10,	0,	0x03	},	//B
				{	12,	0,	0x02	},
				{	15,	0,	0x09	},
					               
				{	19,	0,	0x05	},	//C
				{	21,	0,	0x04	},
				{	24,	0,	0x0A	},
					               
				{	28,	0,	0x06	},	//noise
					               
				{	32,	0,	0x07	},	//mixer
					               
				{	36,	0,	0x0C	},	//en
				{	38,	0,	0x0B	},
				{	41,	0,	0x0D	},	//shape
					               
				{	44,	0,	0x0E	},	//port A	
				{	46,	0,	0x0F	}	//port B

			};
	
    const char *ayn = comp.active_ay ?	"Ay1" :
					"Ay0";

    //-------------------------------------------------------------------------
    if (conf.sound.ay_scheme < AY_SCHEME_QUADRO)
    {
	ayn = "Ay:";
	comp.active_ay = 0;
    }
    //-------------------------------------------------------------------------
    tprint(	ay_x - 3,
		ay_y,
		ayn,
		DBG_ATTR_TITLES	//W_TITLE
	   );
    
    SNDCHIP *chip = &ay[ comp.active_ay];
    char line[ 32];
    
    
    tprint(	ay_x,
		ay_y,
	//	"123456789_123456789_123456789_123456789_12345678",
	//	"A:xxxx:xx B:xxxx:xx C:xxxx:xx N:xx Mxx E:xxxx S:xx i:xx o:xx
	//	"A:xxx:xx B:xxx:xx C:xxx:xx N:xx Mxx E:xxxx S:xx i:xx o:xx
	//	"A****:** B****:** C****:** N** M** E****:** ****",
		"A----:-- B----:-- C----:-- N-- M-- E----:-- ----",
	//	"a****=** b****=** c****=** n** m** e****=** ****",
	//	"A****#** B****#** C****#** N** M** E****#** ****",
		( DBG_ATTR_AY_NAMES | current_back_attr )	//W_AYNUM
	   );
		
    //-------------------------------------------------------------------------
    if (conf.sound.ay_scheme)
    {
    for (unsigned i = 0;    i < 16;    i++)
    {
	sprintf(	line,
			"%02X",
			chip->get_reg(ay_map[i][2])
		);
	tprint(		( ay_x + ay_map[i][0] ),
			( ay_y + ay_map[i][1] ),
			line,
			( ay_map[i][2] == ( chip->get_activereg() ) ) ?	( DBG_ATTR_AY_VALUES_IO | current_back_attr ):	//W_AYON :
									( DBG_ATTR_AY_VALUES | current_back_attr )	//W_AYOFF
		);
    }
    }
    //-------------------------------------------------------------------------
    
/*




    for (unsigned i = 0; i < 16; i++)
    {
	line[0] = "0123456789ABCDEF"[i];
	line[1] = 0;
	tprint(		ay_x + i*3,
			ay_y,
			line,
			DBG_ATTR_AY_NAMES	//W_AYNUM
		);
	sprintf(	line,
			"%02X",
			chip->get_reg(i)
		);
	tprint(		unsigned(ay_x + i*3 + 1),
			unsigned(ay_y),
			line,
			(i == (chip->get_activereg() ))    ? 	DBG_ATTR_AY_VALUES_IO :		//W_AYON :
								DBG_ATTR_AY_VALUES		//W_AYOFF
		);
    }
*/
    /*
    рамочки ненужны
    frame(	ay_x,
		ay_y,
		48,
		1,
		FRAME
	 );
    */
}
//=============================================================================



//=============================================================================
// switch active AY (turbo-sound)			//"mon.switchay" хоткей
void mon_switchay()					
{				// вместо перекючения отображаемого чипа
    comp.active_ay ^= 1;	// походу клацаетсо весь чип навсегда? :rofl:
}
//=============================================================================



//=============================================================================
void __cdecl BankNames( int i, char *Name)
{
    unsigned rom_bank;
    unsigned ram_bank;

    bool IsRam = (RAM_BASE_M <= bankr[i]) && (bankr[i] < RAM_BASE_M + PAGE * MAX_RAM_PAGES);
    bool IsRom = (ROM_BASE_M <= bankr[i]) && (bankr[i] < ROM_BASE_M + PAGE * MAX_ROM_PAGES);

    if (IsRam)
	ram_bank = ULONG(( bankr[i] - RAM_BASE_M) / PAGE);

    if (IsRom)
	rom_bank = ULONG(( bankr[i] - ROM_BASE_M) / PAGE);
	
    //-------------------------------------------------------------------------
    if (IsRam)				sprintf( Name,	"RAM%2X", ram_bank	);
    //-------------------------------------------------------------------------
    if (IsRom)				sprintf( Name,	"ROM%2X", rom_bank	);
    //-------------------------------------------------------------------------
    if (bankr[ i] == base_sos_rom)	strcpy( Name,	"BASIC"			);
    //-------------------------------------------------------------------------
    if (bankr[ i] == base_dos_rom)	strcpy( Name,	"TRDOS"			);
    //-------------------------------------------------------------------------
    if (bankr[ i] == base_128_rom)	strcpy( Name,	"B128K"			);
    //-------------------------------------------------------------------------
    if	(
	    (bankr[i] == base_sys_rom)
	     &&
	    (	
		(conf.mem_model == MM_PROFSCORP) ||
		(conf.mem_model == MM_SCORP)
	    )
	)				strcpy( Name,	"SVM  "			);
    //-------------------------------------------------------------------------
    if (    (
		(conf.mem_model == MM_PROFSCORP) ||
		(conf.mem_model == MM_SCORP)
	    )
	    && 
	    (IsRom)
	    && 
	    (rom_bank > 3)
	)				sprintf( Name,	"ROM%2X", rom_bank	);
    //-------------------------------------------------------------------------
    if (bankr[i] == CACHE_M)		strcpy( Name, (conf.cache != 32)  ?  "CACHE":
									     "CACH0");
    //-------------------------------------------------------------------------
    if (bankr[i] == CACHE_M + PAGE)	strcpy( Name,	"CACH1"			);
    //-------------------------------------------------------------------------
}
//=============================================================================


//=============================================================================
void showbanks()
{
    unsigned char current_back_attr = (activedbg == WND_PAGES)  ?  DBG_ATTR_BCKGRND_ACTIVE : //(выделенное окно)
								   DBG_ATTR_BCKGRND;
    Z80 &cpu = CpuMgr.Cpu();
    //-------------------------------------------------------------------------
    for (int i = 0;    i < 4;    i++)
    {
	char ln[ 64];
	sprintf(	ln,
			"%d:", i
	        );
	tprint(		unsigned( banks_x),
			unsigned( banks_y + i),
			ln,
			( DBG_ATTR_PAGES_NMB | current_back_attr)	//W_OTHEROFF
		);
	strcpy( ln, "?????");
	cpu.BankNames( i, ln);
	tprint(		unsigned( banks_x + 2),
			unsigned( banks_y + i),
			ln,
			bankr[ i] != bankw[ i]  ?  ( DBG_ATTR_PAGES_ROM | current_back_attr) :	//W_BANKRO : 
						   ( DBG_ATTR_PAGES_RAM | current_back_attr)	//W_BANK
		);
    }
    //-------------------------------------------------------------------------

    frame(	banks_x,
		banks_y,
		7,
		4,
		FRAME
	  );
    tprint(	banks_x,
		banks_y - 1,
		"Pages",
		DBG_ATTR_TITLES//W_TITLE
	   );
}

//=============================================================================
void showports()
{

    unsigned char current_back_attr = (activedbg == WND_PORTS)  ?  DBG_ATTR_BCKGRND_ACTIVE :	//(выделенное окно)
								   DBG_ATTR_BCKGRND;					
    unsigned char attr_values = (current_back_attr | DBG_ATTR_PORTS_VALUES);
    unsigned char attr_names =  (current_back_attr | DBG_ATTR_PORTS_NAMES);    
    
    char ln[ 64];
    //-------------------------------------------------------------------------
    // FE
    sprintf(	ln,
		"%02X", comp.pFE
	    );
    tprint(	ports_x,
		ports_y,
		"  FE:",
		attr_names	//W_OTHER
	   );
    tprint(	ports_x + 5,
		ports_y,
		ln,
		attr_values	//W_OTHER
	   );
    //-------------------------------------------------------------------------
    // 7FFD
    sprintf(	ln,
		"%02X", comp.p7FFD
	    );
    tprint(	ports_x,
		ports_y + 1,
		"7FFD:",
		attr_names
	   );
    tprint(	ports_x + 5,
		ports_y + 1,
		ln,
		(comp.p7FFD & 0x20) &&	!(  
					    ( 
						(conf.mem_model == MM_PENTAGON)	&& (conf.ramsize == 1024)
					    )
					    ||
					    (
						(conf.mem_model == MM_PROFI) && (comp.pDFFD & 0x10)
					    )
					)   ?	(current_back_attr | DBG_ATTR_PORTS_48_LOCK) :	//W_48K :
						attr_values	//W_OTHER
	   );
    //-------------------------------------------------------------------------
    switch (conf.mem_model)
    {
	case MM_KAY:
	case MM_SCORP:
	case MM_PROFSCORP:
	case MM_PLUS3:
	    dbg_extport = 0x1FFD;
	    dbg_extval = comp.p1FFD;
	    break;
	case MM_PROFI:
	    dbg_extport = 0xDFFD;
	    dbg_extval = comp.pDFFD;
	    break;
	case MM_ATM450:
	    dbg_extport = 0xFDFD;
	    dbg_extval = comp.pFDFD;
	    break;
	case MM_ATM710:
	case MM_ATM3:
	    dbg_extport = (comp.aFF77 & 0xFFFF);
	    dbg_extval = comp.pFF77;
	    break;
	case MM_QUORUM:
	    dbg_extport = 0x0000;
	    dbg_extval = comp.p00;
	    break;
	default:
	    dbg_extport = -1U;
    }
    //-------------------------------------------------------------------------
    // печать порта выбранного выше
    if (dbg_extport != -1U)
    {
	sprintf(	ln,
			"%04X:", dbg_extport
		);
	tprint(		ports_x,
			ports_y + 2,
			ln,
			attr_names	//W_OTHER
		);
	sprintf(	ln,
			"%02X", dbg_extval
		);
	tprint(		ports_x + 5,
			ports_y + 2,
			ln,
			attr_values	//W_OTHER
		);
		
    }
    //-------------------------------------------------------------------------
    // печать вместо cmos-а
    else
    {		
	sprintf(	ln,
			"%02X", comp.cmos_addr
		);
	tprint(		ports_x,
			ports_y + 2,
			"CMOS:",
			attr_names	//W_OTHER
		);
	tprint(		ports_x + 5,
			ports_y + 2,
			ln,
			attr_values	//W_OTHER
		);
    }	
    //-------------------------------------------------------------------------
    // EFF7
    sprintf(	ln,
		"%02X", comp.pEFF7
	    );    
    tprint(	ports_x,
		ports_y + 3,
		"EFF7:",
		attr_names	//W_OTHER
	   );   
    tprint(	ports_x + 5,
		ports_y + 3,
		ln,
		attr_values	//W_OTHER
	   );
    //-------------------------------------------------------------------------
    frame(	ports_x,
		ports_y,
		7,
		4,
		FRAME
	  );
	  
    tprint(	ports_x,
		ports_y - 1,
		"Ports",
		DBG_ATTR_TITLES	//W_TITLE
	   );
}
//=============================================================================


//=============================================================================
void showdos()
{
//    CD:802E
//    STAT:24
//    SECT:00
//    T:00/01
//    S:00/00
//[vv]   if (conf.trdos_present) comp.wd.process();

    unsigned char current_back_attr = (activedbg == WND_BETA128)  ?  DBG_ATTR_BCKGRND_ACTIVE :	//(выделенное окно)
								     DBG_ATTR_BCKGRND;					
    unsigned char attr_values = conf.trdos_present  ?  (current_back_attr | DBG_ATTR_BETA128_VALUES)	:
						       (current_back_attr | DBG_ATTR_BETA128_OFF)	;

    unsigned char attr_names = (current_back_attr | DBG_ATTR_BETA128_NAMES); 


    char ln[ 64];
    unsigned char atr = conf.trdos_present  ?  W_OTHER :
					       W_OTHEROFF;
    //-------------------------------------------------------------------------
    sprintf( ln, "%02X%02X", comp.wd.cmd, comp.wd.data);
    tprint(	dos_x,
 		dos_y,
 		"CD:",
 		attr_names	//atr
 	);
    tprint(	dos_x + 3,
 		dos_y,
 		ln,
 		attr_values	//atr
 	);
    //-------------------------------------------------------------------------
    sprintf( ln, "%02X", comp.wd.RdStatus() );
    tprint(	dos_x,
		dos_y + 1,
		"STAT:",
		attr_names	//atr
	   );
    tprint(	dos_x + 5,
		dos_y + 1,
		ln,
		attr_values	//atr
	   );
 
    //-------------------------------------------------------------------------
    sprintf( ln, "%02X", comp.wd.sector );
    tprint(	dos_x,
		dos_y + 2,
		"SECT:",
		attr_names	//atr
	   );
    tprint(	dos_x + 5,
		dos_y + 2,
		ln,
		attr_values	//atr
	   );
    //-------------------------------------------------------------------------
    sprintf( ln, "%02X/%02X", comp.wd.seldrive->track, comp.wd.track );
    tprint(	dos_x,
		dos_y + 3,
		"T:",
		attr_names	//atr
	   );
    tprint(	dos_x + 2,
		dos_y + 3,
		ln,
		attr_values	//atr
	   );
    //-------------------------------------------------------------------------
    sprintf( ln, "%02X/%02X", comp.wd.system, comp.wd.rqs );
    tprint(	dos_x,
		dos_y + 4,
		"S:",
		attr_names	//atr
	   );
    tprint(	dos_x + 2,
		dos_y + 4,
		ln,
		attr_values	//atr
	   );
    //-------------------------------------------------------------------------
    frame( dos_x, dos_y, 7, 5, FRAME);
    //-------------------------------------------------------------------------
#if 1
    tprint(	dos_x,
		dos_y-1,
		"Beta128",
		DBG_ATTR_TITLES//W_TITLE
	   );
#else
    sprintf( ln, "%X-%X %d", comp.wd.state, comp.wd.state2, comp.wd.seldrive->track );
    tprint(	dos_x,
		dos_y-1,
		ln,
		atr
	   );
#endif
/*
// уже не компилируемый код

//    STAT:00101010
//    CMD:80,STA:2A
//    DAT:22,SYS:EE
//    TRK:31,SEC:01
//    DISK:A,SIDE:0

   char ln[64]; unsigned char atr = conf.trdos_present ? 0x20 : 0x27;
   sprintf(ln, "STAT:00000000"); unsigned char stat = in_trdos(0x1F);
   for (int i = 0; i < 7; i++) ln[i+5] = (stat & (0x80 >> i)) ? '1':'0';
   tprint(dos_x, dos_y+1, ln, atr);
   sprintf(ln, "CMD:%02X,STA:%02X", comp.trdos.cmd, stat);
   tprint(dos_x, dos_y+2, ln, atr);
   sprintf(ln, "DAT:%02X,SYS:%02X", comp.trdos.data, in_trdos(0xFF));
   tprint(dos_x, dos_y+3, ln, atr);
   sprintf(ln, "TRK:%02X,SEC:%02X", comp.trdos.track, comp.trdos.sector);
   tprint(dos_x, dos_y+4, ln, atr);
   sprintf(ln, "DISK:%c,SIDE:%c", 'A'+(comp.trdos.system & 3), (comp.trdos.system & 0x10) ? '0':'1');
   tprint(dos_x, dos_y+5, ln, atr);
   frame(dos_x, dos_y+1, 13, 5, FRAME);
   tprint(dos_x, dos_y, "beta128", 0x83);
*/
}
//=============================================================================



//=============================================================================
#ifdef MOD_GSBASS

// менюшка BASS mod ripper-а
static INT_PTR CALLBACK gsdlg( HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
    (void)lp;

    char tmp[ 0x200];
    unsigned i;					// Alone Coder 0.36.7
    HWND lv = GetDlgItem( dlg, IDC_GSLIST);
    
    //-------------------------------------------------------------------------
    if (msg == WM_INITDIALOG)
    {
	ListView_SetExtendedListViewStyleEx(	lv,
						LVS_EX_FULLROWSELECT,
						LVS_EX_FULLROWSELECT
					    );
	LVCOLUMN Col = { };

	static const char *Cols[] = { "", "smp", "v", "n", "p", "f", "l" };
	static const int Width[] =  { 30,   100,  40,  40,  40,  70,  70 };

	Col.mask = LVCF_TEXT | LVCF_WIDTH;
	//---------------------------------------------------------------------
	for (i = 0;    i < _countof( Cols);    i++)
	{
	    Col.pszText = LPSTR( Cols[ i]);
	    Col.cx = Width[ i];
	    ListView_InsertColumn(	lv,
					i,
					&Col
				  );
	}
	//---------------------------------------------------------------------
	if (gs.modsize)
	{
	    sprintf(	tmp,
			"(%s)", gs.mod_playing  ?  "P" :
						   "S"
		    );
	    LVITEM Item = { };
	    Item.mask = LVIF_TEXT;
	    Item.iItem = 0;
	    Item.iSubItem = 0;
	    Item.pszText = tmp;
	    ListView_InsertItem(  lv,
				  &Item
				);

	    sprintf(	tmp,
			"%.20s", gs.mod
		    );
	    tmp[ 20] = 0;
	    Item.iSubItem++;
	    Item.pszText = tmp;
	    ListView_SetItem(	lv,
				&Item
			     );
	}
	//---------------------------------------------------------------------
	for (i = 1;    i < gs.total_fx;    i++)
	{
	    sprintf(	tmp,
			"%s%s", (gs.cur_fx == i)  ?  "*" :
						     ""  ,
			gs.sample[ i].loop < gs.sample[i].end  ?  "(L)" :
								  ""
		    );
	    LVITEM Item = { };
	    Item.mask = LVIF_TEXT;
	    Item.iItem = int(i);
	    Item.iSubItem = 0;
	    Item.pszText = tmp;
	    ListView_InsertItem(	lv,
					&Item
				);
	    sprintf(	tmp,
			"%u", i
		    );
	    Item.iSubItem++;
	    ListView_SetItem( lv, &Item);

	    sprintf( tmp, "%u", unsigned( gs.sample[ i].volume));
	    Item.iSubItem++;
	    ListView_SetItem( lv, &Item);

	    sprintf( tmp, "%u", unsigned( gs.sample[ i].note));
	    Item.iSubItem++;
	    ListView_SetItem(lv, &Item);
	    
	    sprintf( tmp, "%u", unsigned( gs.sample[ i].Priority));
	    Item.iSubItem++;
	    ListView_SetItem(lv, &Item);
	    
	    sprintf( tmp, "%.2f", double( gs.note2rate[ gs.sample[ i].note]));
	    Item.iSubItem++;
	    ListView_SetItem( lv, &Item);

	    sprintf(tmp, "%u", gs.sample[ i].end);
	    Item.iSubItem++;
	    ListView_SetItem( lv, &Item);
        }
	//---------------------------------------------------------------------
        int Sel = ListView_GetItemCount( lv);
        //---------------------------------------------------------------------
	if (Sel > 0)
	{
	    LVITEM Item = { };
	    Item.stateMask = LVIS_SELECTED;
	    Item.state = LVIS_SELECTED;
	    SendMessage( lv, LVM_SETITEMSTATE, WPARAM( 0), LPARAM( &Item));
	    SetFocus( lv);
	}
	//---------------------------------------------------------------------
        *tmp = 0;
	//---------------------------------------------------------------------
	for (i = 0;    i < 0x100;    i++)
	{
	    if (gs.badgs[ i])
	    {
		sprintf( tmp + strlen( tmp), "%02X ", i);
	    }
	}
	//---------------------------------------------------------------------
	Edit_SetText( GetDlgItem( dlg, IDE_GS), tmp);

	return Sel  ?  FALSE :
		       TRUE;
    }
    //-------------------------------------------------------------------------
    if (msg == WM_SYSCOMMAND && (wp & 0xFFF0) == SC_CLOSE)
    {
	EndDialog( dlg, 0);
    }
    //-------------------------------------------------------------------------
    unsigned id;
    unsigned nc;
    //-------------------------------------------------------------------------
    switch (msg)
    {
	//---------------------------------------------------------------------
	case WM_NOTIFY:
	{
	    LPNMHDR Hdr = LPNMHDR(lp);
	    id = unsigned( Hdr->idFrom);
	    nc = Hdr->code;
	}
	break;
	//---------------------------------------------------------------------
	case WM_COMMAND:
	    id = LOWORD( wp);
	    nc = HIWORD( wp);
	    break;
	//---------------------------------------------------------------------
	default:
	    return FALSE;
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    if ((id == IDCANCEL || id == IDOK) && (nc == BN_CLICKED))
    {
	EndDialog( dlg, 0);
    }
    //-------------------------------------------------------------------------
    if ((id == IDB_GS_CLEAR) && (nc == BN_CLICKED))
    {
	memset( gs.badgs, 0, sizeof gs.badgs);
	Edit_SetText( GetDlgItem( dlg, IDE_GS), "");
	return TRUE;
    }
    //-------------------------------------------------------------------------
    if ((id == IDB_GS_RESET) && (nc == BN_CLICKED))
    {
	gs.reset();
	ListView_DeleteAllItems( lv);
	return TRUE;
    }
    //-------------------------------------------------------------------------
    if (((id == IDB_GS_PLAY) && (nc == BN_CLICKED)) || ((id == IDC_GSLIST) && (nc == NM_DBLCLK)))
    {
	unsigned i = unsigned( ListView_GetSelectionMark( lv));
	//---------------------------------------------------------------------
	if (i > 0x100)
	{
	    return TRUE;
	}
	//---------------------------------------------------------------------
	if (!i && gs.modsize)
	{
	    gs.mod_playing ^= 1;
	    //-----------------------------------------------------------------
	    if (gs.mod_playing)
	    {
		gs.restart_mod( 0, 0);
	    }
	    //-----------------------------------------------------------------
	    else
	    {
		gs.stop_mod();
	    }
	    //-----------------------------------------------------------------

	    sprintf( tmp, "(%s)", gs.mod_playing  ?  "P" :
						     "S");
	    LVITEM Item = { };
	    Item.mask = LVIF_TEXT;
	    Item.pszText = tmp;
	    ListView_SetItem( lv, &Item);

	    return TRUE;
	}
	//---------------------------------------------------------------------
	if (!gs.modsize)
	{
	    i++;
	}
	//---------------------------------------------------------------------
        gs.debug_note( i);
        return TRUE;
    }
    //-------------------------------------------------------------------------
    if ((id == IDB_GS_SAVE) && (nc == BN_CLICKED))
    {
	unsigned i = unsigned( ListView_GetSelectionMark( lv));
	//---------------------------------------------------------------------
	if ((i == 0) && (gs.modsize != 0))
	{
	    SaveModDlg( wnd);
	    return TRUE;
	}
	//---------------------------------------------------------------------
	if (i > 0x100)
	{
	    return TRUE;
	}
	//---------------------------------------------------------------------
	OPENFILENAME ofn = { };
	char sndsavename[ MAX_PATH];
	*sndsavename = 0;
	ListView_GetItemText( lv, i, 1, sndsavename, _countof( sndsavename));
	strcat( sndsavename, ".pcm");

	ofn.lStructSize = (WinVerMajor < 5)  ?	OPENFILENAME_SIZE_VERSION_400 :
						sizeof( OPENFILENAME);
	ofn.lpstrFilter = "PCM sample (.pcm)\0*.pcm\0";
	ofn.lpstrFile = sndsavename;
	ofn.lpstrDefExt = "pcm";
	ofn.nMaxFile = _countof( sndsavename);
	ofn.lpstrTitle = "Save Sample";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.hwndOwner = wnd;
	ofn.nFilterIndex = 1;
	//---------------------------------------------------------------------
	if (GetSaveFileName( &ofn))
	{
	    gs.debug_save_note( i, sndsavename);
	}
	//---------------------------------------------------------------------
	return TRUE;
    }
    //-------------------------------------------------------------------------
    return FALSE;
}
//=============================================================================



//=============================================================================
void mon_gsdialog()						//"mon.gs" хоткей
{
    //-------------------------------------------------------------------------
    if (conf.gs_type == 2)
    {
	DialogBox( hIn, MAKEINTRESOURCE( IDD_GS), wnd, gsdlg);
    }
    //-------------------------------------------------------------------------
    else
    {
	MessageBox( wnd, "high-level GS emulation\nis not initialized", nullptr, MB_OK | MB_ICONERROR);
    }
    //-------------------------------------------------------------------------
}
#else
void mon_gsdialog() {}
#endif
//=============================================================================
