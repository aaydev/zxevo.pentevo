#pragma once

#ifndef UNREAL_EMUL_H
#define UNREAL_EMUL_H		//какаято NEDO хуита

#include "sysdefs.h"
#include "z80/defs.h"
#include "sndrender/sndrender.h"
#include "savevid.h"

#include <windows.h>	// нужно чтобы объявлять HWND-ы

//=============================================================================
#define EMUL_DEBUG
#define TRASH_PAGE
//=============================================================================
#define PAGE 0x4000
#define MAX_RAM_PAGES 256       // 4Mb RAM
#define MAX_CACHE_PAGES 2       // 32K cache
#define MAX_MISC_PAGES 1        // trash page
#define MAX_ROM_PAGES 64        // 1Mb
//=============================================================================
#define GS4MB //0.37.0

#ifdef MOD_GSZ80
    #define MAX_GSROM_PAGES  32U      // 512Kb
    
    #ifdef GS4MB
	#define MAX_GSRAM_PAGES 256U     // for gs4mb
    #else
	#define MAX_GSRAM_PAGES 30U      // for gs512 (last 32k unusable)
    #endif
#else
    #define MAX_GSROM_PAGES 0
    #define MAX_GSRAM_PAGES 0
#endif
//=============================================================================
#define MAX_PAGES (MAX_RAM_PAGES + MAX_CACHE_PAGES + MAX_MISC_PAGES + MAX_ROM_PAGES + MAX_GSROM_PAGES + MAX_GSRAM_PAGES)
//=============================================================================
#define RAM_BASE_M  memory
#define CACHE_M     (RAM_BASE_M + MAX_RAM_PAGES*PAGE)
#define MISC_BASE_M (CACHE_M + MAX_CACHE_PAGES*PAGE)
#define ROM_BASE_M  (MISC_BASE_M + MAX_MISC_PAGES*PAGE)
//=============================================================================
#ifdef MOD_GSZ80
#define ROM_GS_M    (ROM_BASE_M + MAX_ROM_PAGES*PAGE)
#define GSRAM_M     (ROM_GS_M + MAX_GSROM_PAGES*PAGE)
#endif
//=============================================================================
#define TRASH_M     (MISC_BASE_M+0*PAGE)




// extern HWND hwnd_bp_dialog;		// hwnd немодального окна бряков [NS]
// теперь в no_modal_windows.h


//=============================================================================
enum IDE_SCHEME
{
    IDE_NONE = 0,
    IDE_ATM,
    IDE_NEMO, IDE_NEMO_A8, IDE_NEMO_DIVIDE,
    IDE_SMUC,
    IDE_PROFI,
    IDE_DIVIDE
};
//=============================================================================
enum MOUSE_WHEEL_MODE
{
    MOUSE_WHEEL_NONE,
    MOUSE_WHEEL_KEYBOARD,
    MOUSE_WHEEL_KEMPSTON
}; //0.36.6 from 0.35b2
//=============================================================================
enum SAA_SCHEME							// NEDOREPO
{
    SAA_NONE = 0,
    SAA_ZXM,
    SAA_TFM_PRO
};
//=============================================================================
enum MEM_MODEL
{
    MM_PENTAGON = 0,
    MM_SCORP,
      MM_PROFSCORP,
    MM_PROFI,
    MM_ATM450,
      MM_ATM710,
	MM_ATM3,
    MM_KAY,
    MM_PLUS3,
    MM_QUORUM,
    N_MM_MODELS
};
//=============================================================================
enum ROM_MODE
{
    RM_NOCHANGE = 0,
    RM_SOS,
    RM_DOS,
    RM_SYS,
    RM_128,
    RM_CACHE
};
//=============================================================================
const int RAM_128 = 128;
const int RAM_256 = 256;
const int RAM_512 = 512;
const int RAM_1024 = 1024;
const int RAM_4096 = 4096;
//=============================================================================
struct TMemModel
{
    const char *fullname;
    const char *shortname;
    MEM_MODEL Model;
    unsigned defaultRAM;
    unsigned availRAMs;
};
//=============================================================================
typedef void (__fastcall *VOID_FUNC)(void);
typedef void (__fastcall *RENDER_FUNC)(unsigned char*,unsigned);

struct RENDER
{
    const char *name;
    RENDER_FUNC func;
    const char *nick;
    unsigned flags;
};
//=============================================================================
struct IDE_CONFIG
{
    char image[512];
    unsigned c, h, s;
    u64 lba;
    unsigned char readonly;
    u8 cd;
};
//=============================================================================
enum RSM_MODE
{
    RSM_SIMPLE,
    RSM_FIR0,
    RSM_FIR1,
    RSM_FIR2
};
//=============================================================================
enum SYNC_MODE
{
    SM_SOUND,
    SM_TSC,
    SM_VIDEO
};
//=============================================================================
struct zxkeymap ;
//=============================================================================
struct CONFIG
{
    unsigned paper;	// start of paper
    unsigned t_line;	// t-states per line
    unsigned frame;	// t-states per frame
    unsigned intfq;	// usually 50Hz
    unsigned intlen;	// length of INT signal (for Z80)
    unsigned nopaper;	// hide paper
//-----------------------------------------------------------------------------
    unsigned render;
    unsigned driver;
    unsigned fontsize;
//-----------------------------------------------------------------------------
    unsigned soundbuffer;
    unsigned refresh;
//-----------------------------------------------------------------------------
    unsigned char flashcolor;
    unsigned char noflic;
    unsigned char fast_sl;
    unsigned char alt_nf;   
    
    unsigned char frameskip;
    unsigned char frameskipmax;
    
    unsigned char flip;
    unsigned char fullscr;
    //-------------------------------------------------------------------------
    // для нормального общего размера для ебулятора и дебагера	[NS]
    // ибо изначально там одни костыли...
    unsigned char win_resize_request;
    unsigned int win_static_size_x;
    unsigned int win_static_size_y;
    //-------------------------------------------------------------------------
    unsigned char lockmouse;
    unsigned char detect_video;
    unsigned char tape_traps;
    unsigned char tape_autostart;
    int scrshot;
    char scrshot_dir[FILENAME_MAX];
//-----------------------------------------------------------------------------
    unsigned char ch_size;
    unsigned char EFF7_mask;
    unsigned char reset_rom;
    unsigned char use_romset;
//-----------------------------------------------------------------------------
    unsigned char updateb;
    unsigned char bordersize;
    
    unsigned char even_M1;
    unsigned char border_4T;
    
    unsigned char floatbus;
    unsigned char floatdos;
    bool portff;
//-----------------------------------------------------------------------------
// border small
    unsigned mcx_small;		// Горизонтальный размер видимого экрана (pix)
    unsigned mcy_small;		// Вертикальный размер видимого экрана (lines)
    unsigned b_top_small;	// Число видимых строк верхнего бордюра
    unsigned b_left_small;	// Число видимых пикселей левого бордюра
//-----------------------------------------------------------------------------
// border full
    unsigned mcx_full;		// Горизонтальный размер видимого экрана pix
    unsigned mcy_full;		// Вертикальный размер видимого экрана lines
    unsigned b_top_full;	// Число видимых строк верхнего бордюра
    unsigned b_left_full;	// Число видимых пикселей левого бордюра
//-----------------------------------------------------------------------------

    int modem_port;		//, modem_scheme;
    unsigned char fdd_noise;

    unsigned char trdos_present;
    unsigned char trdos_interleave;
    
    unsigned char trdos_traps;   
    unsigned char wd93_nodelay;			// очепятка?
    unsigned char trdos_wp[4];

    unsigned char trdos_IORam;	// временный параметр, потом надо удалить [недописюканские костыли]
	unsigned char wiznet;
	char cold_ram_pat[17];	// NEDOREPO
	
    unsigned char cache;
    unsigned char cmos;
    unsigned char smuc;
    unsigned char ula_preset;
    
    unsigned char gs_type;
    unsigned char pixelscroll;
    unsigned char sleepidle;
    unsigned char rsrvd1_;
    unsigned char ConfirmExit;
//-----------------------------------------------------------------------------
    unsigned char highpriority;
    SYNC_MODE SyncMode;
    bool HighResolutionTimer;
    unsigned char videoscale;
//-----------------------------------------------------------------------------
    MEM_MODEL mem_model;
    unsigned ramsize;
    unsigned romsize;
    bool Sna128Lock;
//-----------------------------------------------------------------------------
    IDE_SCHEME ide_scheme;
    IDE_CONFIG ide[2];
    unsigned char ide_skip_real;
    unsigned char cd_aspi;
//-----------------------------------------------------------------------------
    unsigned char soundfilter;	//Alone Coder (IDC_SOUNDFILTER)
    unsigned char RejectDC;
//-----------------------------------------------------------------------------
struct
{
    unsigned fq;
    unsigned ayfq;
    unsigned saa1099fq;
		
    int covoxFB;
    int covoxDD;
    int sd;
    int saa1099;
		
    int beeper_vol;
    int micout_vol;
    int micin_vol;
    int ay_vol;
    int aydig_vol;
    int covoxFB_vol;
    int covoxDD_vol;
    int sd_vol;
    int gs_vol;
    int bass_vol;
		
    VOID_FUNC do_sound;
    unsigned char enabled;
    unsigned char gsreset;
    unsigned char dsprimary;
	
    unsigned char ay_chip;
    unsigned char ay_scheme;
    unsigned char ay_stereo;
    unsigned char ay_vols;
    unsigned char ay_samples;
    
    unsigned ay_stereo_tab[6];
    unsigned ay_voltab[32];
	
} sound;
//-----------------------------------------------------------------------------
struct {
    unsigned firenum;
    
    unsigned char altlock;
    unsigned char fire;
    unsigned char firedelay;
    
    unsigned char paste_hold;
    unsigned char paste_release;
    unsigned char paste_newline;    
    
    unsigned char mouse;
    unsigned char mouseswap;
    unsigned char kjoy;
    unsigned char keymatrix;
    unsigned char joymouse;    
    
    bool fjoy;			// fuller joystick
    
    unsigned char keybpcmode;
    signed char mousescale;
    unsigned char mousewheel;	// enum MOUSE_WHEEL_MODE //0.36.6 from 0.35b2
    zxkeymap *active_zxk;
    unsigned JoyId;
} input;
//-----------------------------------------------------------------------------
struct
{
    unsigned char enabled;
    unsigned char flash_ay_kbd;
    unsigned char perf_t;
    unsigned char reserved1;
    unsigned bandBpp;
    #define NUM_LEDS 8	// добавляя новые !!! ВПИСЫВАТЬ РУЧКАМИ !!!
    unsigned ay;		//1
    unsigned perf;		//2
    unsigned load;		//3
    unsigned input;		//4
    unsigned time;		//5
    unsigned osw;		//6
    unsigned memband;		//7
    unsigned memband_256;	//8 - [NS]
} led;
//-----------------------------------------------------------------------------
struct
{
    unsigned char mem_swap;
    unsigned char xt_kbd;
    unsigned char reserved1;
} atm;
//-----------------------------------------------------------------------------
    unsigned char use_comp_pal;
    unsigned pal;			// selected palette
    unsigned num_pals;			// total number of pals

    unsigned minres;			// min. screen x-resolution
    unsigned scanbright;		// scanlines intensity
//-----------------------------------------------------------------------------
struct
{
    unsigned char mix_frames;
    unsigned char mode;			// RSM_MODE
} rsm;
//-----------------------------------------------------------------------------
    char sos_rom_path[FILENAME_MAX];
    char dos_rom_path[FILENAME_MAX];
    char zx128_rom_path[FILENAME_MAX];
    char sys_rom_path[FILENAME_MAX];
    char atm1_rom_path[FILENAME_MAX];
    char atm2_rom_path[FILENAME_MAX];
    char atm3_rom_path[FILENAME_MAX];
    char scorp_rom_path[FILENAME_MAX];
    char prof_rom_path[FILENAME_MAX];
    char profi_rom_path[FILENAME_MAX];
//[vv]   char kay_rom_path[FILENAME_MAX];
    char plus3_rom_path[FILENAME_MAX];
    char quorum_rom_path[FILENAME_MAX];
//-----------------------------------------------------------------------------
#ifdef MOD_GSZ80
    unsigned gs_ramsize;
    char gs_rom_path[FILENAME_MAX];
#endif
//-----------------------------------------------------------------------------
#ifdef MOD_MONITOR
    char sos_labels_path[FILENAME_MAX];
#endif
//-----------------------------------------------------------------------------
    char ngs_sd_card_path[FILENAME_MAX];
//-----------------------------------------------------------------------------
    unsigned char zc;
    char zc_sd_card_path[FILENAME_MAX];
//-----------------------------------------------------------------------------
    unsigned int sd_delay;	// NEDOREPO
//-----------------------------------------------------------------------------
    char atariset[64];		// preset for atari mode
    char zxkeymap[64];		// name of ZX keys map
    char keyset[64];		// short name of keyboard layout
    char appendboot[FILENAME_MAX];
    char workdir[FILENAME_MAX];
    u8 profi_monochrome;
//-----------------------------------------------------------------------------
struct
{
    char exec[VS_MAX_FFPATH];	// ffmpeg path/name
    char parm[VS_MAX_FFPARM];	// enc. parameters for ffmpeg
    char vout[VS_MAX_FFVOUT];	// output video file name
    char newcons;		// open new console for ffmpeg
} ffmpeg;
//-----------------------------------------------------------------------------
    bool ula_plus;
//-----------------------------------------------------------------------------
   
// НОВЫЕ ПАРАМЕТРЫ И СОРТИРОВКА ПО ТИПУ	[NS]
//	желательно в config.cpp и unreal.ini ложить в той же последовательносте
   
   
   
   
//-----------------------------------------------------------------------------
// MISC
    unsigned char All_Screen_Drag;	// Перетаскивание окна за почти любую его часть

//-----------------------------------------------------------------------------
// DEBUG
    unsigned char ZXGS_Step;	// одновременное Step для ZX и GS [NS]
    
    unsigned char Disasm_Tabulation;	// отключение мерского ld            a,b	// [NS]
    
    unsigned char Disasm_A_Mnemonics_Style;	//0 - full  (xxx a,a)			// [NS]
						//1 - short (xxx a)    
						//2 - Combined / Classic Unreal
						
    unsigned char Disasm_Index_H_L_Style;	//0 - IXH IXL IYH IYL			// [NS]
						//1 - XH XL YH YL
   
    unsigned char Disasm_New_Labels;		//0
						//1 - метки с новой строки
						//	недоделано!!!
   
    unsigned char trace_follow_request;		// флаг необходимости переместить вид	// [NS] r0134
						// на отслеживаемый объект

    unsigned char trace_follow_regs;		// [NS]
    
#define REG_NOPE	0			// 0 - nope
#define REG_AF		1			// 1 - AF
#define REG_BC		2			// 2 - BC
#define REG_DE		3			// 3 - DE
#define REG_HL		4			// 4 - HL
#define REG_AF1		5			// 5 - AF'
#define REG_BC1		6			// 6 - BC'
#define REG_DE1		7			// 7 - DE'
#define REG_HL1		8			// 8 - HL'
#define REG_IX		9			// 9 - IX
#define REG_IY		10			// 10 - IY
#define REG_SP		11			// 11 - SP
#define REG_PC		12			// 12 - PC

    unsigned char debug_unlock_mouse;	// 1 - разблокировать мышу при входе в дебагер
    
//-----------------------------------------------------------------------------
// VIDEO_MODES
    unsigned char ATM_DDp_4K_Palette;

//-----------------------------------------------------------------------------
// MSX DOS				// Костыли для запуска MSX DOS для ATM620 на ATM710
    unsigned char atm620_hl8_z;		// копирастическая защита
					// предположительно с пулянием в первых 1024 тактах нулей в d5 порта FE
    unsigned char atm620_force_xt_keyb_ready;	//in 7FFD d7 = 1	!!!7FFD это ATM IDE !!!
    unsigned char atm620_xt_keyb; 	// atm 6.20 fake xt keyboard
//-----------------------------------------------------------------------------
// SOUND
    unsigned char ZXI_TSFM_Mirror;   	// [NS]
    
    unsigned char MIDI_128_Out;   	// [NS]
};
//=============================================================================


//=============================================================================М
struct TEMP
{
    //-------------------------------------------------------------------------
    unsigned char win9x;	// if we have old OS //Dexus
    unsigned rflags;		// render_func flags
    //-------------------------------------------------------------------------
    unsigned border_add;	// for scorpion 4T border update
    unsigned border_and;	
    //-------------------------------------------------------------------------
    unsigned char *base; 	// pointers to Spectrum screen memory
    unsigned char *base_2;		
    //-------------------------------------------------------------------------
    unsigned char rom_mask;
    unsigned char ram_mask;
    unsigned char evenM1_C0;		// C0 for scorpion, 00 for pentagon
    unsigned char hi15;			// 0 - 16bit color, 
					// 1 - 15bit color,
					// 2 - YUY2 colorspace
    unsigned snd_frame_samples;		// samples / frame
    unsigned snd_frame_ticks;		// sound ticks / frame
    unsigned cpu_t_at_frame_start;
    //-------------------------------------------------------------------------
    unsigned gx; 	// updating rectangle (used by GDI renderer)
    unsigned gy;
    unsigned gdx;
    unsigned gdy;		
    //-------------------------------------------------------------------------
    RECT client;	// updating rectangle (used by DD_blt renderer)
    bool Minimized;	// window is minimized
    HDC gdidc;
    //-------------------------------------------------------------------------
    unsigned ox;	// destination video format
    unsigned oy; 
    unsigned obpp; 
    unsigned ofq;		
    //-------------------------------------------------------------------------
    unsigned scx; 	// multicolor area (320x240 or 384x300), used in MCR renderer
    unsigned scy;	
    //-------------------------------------------------------------------------
    unsigned odx; 	// offset to border in surface, used only in flip()
    unsigned ody;
    //-------------------------------------------------------------------------
    unsigned rsx; 	// screen resolution
    unsigned rsy;
    //-------------------------------------------------------------------------
    unsigned b_top;	// border frame used to setup MCR
    unsigned b_left;
    unsigned b_right;
    unsigned b_bottom; 
    //-------------------------------------------------------------------------
    unsigned scale;			// window scale (x1, x2, x3, x4)
    unsigned mon_scale;			// window scale in monitor mode(debugger)
					//    щас юзаетсо для временного хранения scale в дебагере !!!
//-----------------------------------------------------------------------------
    unsigned ataricolors[ 0x100];
    unsigned shift_mask;		// for 16/32 bit modes masks low bits of color components
//-----------------------------------------------------------------------------
// led coords
struct 
{
    unsigned char *ay;
    unsigned char *perf;
    unsigned char *load;
    unsigned char *input;
    unsigned char *time;
    unsigned char *osw;
    unsigned char *memband;
    unsigned char *memband_256;		// [NS]
    unsigned char *fdd;

    __int64 tape_started;
} led;
//-----------------------------------------------------------------------------
    unsigned char profrom_mask;
    unsigned char comp_pal_changed;
//-----------------------------------------------------------------------------
// CPU features
    unsigned char mmx;
    unsigned char sse; 
    unsigned char sse2;
    
    u64 cpufq;			// x86 t-states per second
    unsigned ticks_frame;	// x86 t-states in frame
//-----------------------------------------------------------------------------
    unsigned char vidblock;
    unsigned char sndblock;
    unsigned char inputblock;
    unsigned char frameskip;
    bool PngSupport;
    bool ZlibSupport;
    unsigned gsdmaaddr;
    u8 gsdmaon;
    u8 gs_ram_mask;
//-----------------------------------------------------------------------------
    u8 offset_vscroll;
    u8 offset_vscroll_prev;
    u8 offset_hscroll;
    u8 offset_hscroll_prev;
//-----------------------------------------------------------------------------
    char RomDir[ FILENAME_MAX];
    char SnapDir[ FILENAME_MAX];
    char HddDir[ FILENAME_MAX];
    char SdDir[ FILENAME_MAX];
    char LastSnapName[ FILENAME_MAX]; // Имя последнего загруженного файла без расширения
};
//=============================================================================


//=============================================================================
extern TEMP temp;
extern unsigned sb_start_frame;

//=============================================================================
// От этого enum'а завивит порядок строк в массиве ay_schemes
// (тут ищо недописюканы наоптимизировали блджд!)
enum AY_SCHEME
{
    AY_SCHEME_NONE = 0,
    AY_SCHEME_FULLER,	// fuller должен идти обязательно до single
    AY_SCHEME_SINGLE,
    AY_SCHEME_PSEUDO,
    AY_SCHEME_QUADRO,
    AY_SCHEME_POS,
    AY_SCHEME_CHRV,
//  AY_SCHEME_ZXI,	// [NS]
    AY_SCHEME_MAX	// видимо просто для определения последнего
};
//=============================================================================
#include "wd93.h"
#include "fdd.h"
#include "hddio.h"
#include "hdd.h"
#include "input.h"
#include "modem.h"
//=============================================================================
#if defined(MOD_GSZ80) || defined(MOD_GSBASS)
#include "bass.h"
#include "snd_bass.h"
#endif
//=============================================================================
#ifdef MOD_GSBASS
#include "gshlbass.h"
#include "gshle.h"
#endif
//=============================================================================
#define EFF7_4BPP       0x01
#define EFF7_512        0x02
#define EFF7_LOCKMEM    0x04
#define EFF7_ROCACHE    0x08
#define EFF7_GIGASCREEN 0x10
#define EFF7_HWMC       0x20
#define EFF7_384        0x40
#define EFF7_CMOS       0x80
//=============================================================================


//=============================================================================
// Биты порта 00 для кворума
    static const u8 Q_F_RAM = 0x01;
    static const u8 Q_RAM_8 = 0x08;
    static const u8 Q_B_ROM = 0x20;
    static const u8 Q_BLK_WR = 0x40;
    static const u8 Q_TR_DOS = 0x80;
//=============================================================================


//=============================================================================
enum SNAP
{
    snNOFILE,
    snUNKNOWN,
    snTOOLARGE,
    //
    snSP,
    snZ80,
    snSNA_48,
    snSNA_128,
    //
    snTAP,
    snTZX,
    snCSW,
    //
    snPAL,
    // Дисковые форматы в конце, для них отдельная проверка по наличию дисковой подсистемы
    snHOB,
    snSCL,
    snTRD,
    snFDI,
    snTD0,
    snUDI,
    snISD,
    snPRO,
    snDSK,
    snIPF
};
//=============================================================================


//=============================================================================
struct NVRAM
{
    enum EEPROM_STATE
    {
	IDLE = 0,
	RCV_CMD,
	RCV_ADDR,
	RCV_DATA,
	SEND_DATA,
	RD_ACK 
    };
    unsigned address;
    
    unsigned char datain;
    unsigned char dataout;
    unsigned char bitsin;
    unsigned char bitsout;
    
    unsigned char state;
    unsigned char prev;
    unsigned char out;
    unsigned char out_z;
    
    void write(unsigned char val);
};
//=============================================================================


//=============================================================================
struct COMPUTER
{
    unsigned char p7FFD;
    unsigned char pFE;
    unsigned char pEFF7;
    unsigned char pXXXX;    
    
    unsigned char pDFFD;
    unsigned char pFDFD;
    unsigned char p1FFD;
    unsigned char pFF77;    
    
    //-------------------------------------------------------------------------
    // gmx
    u8 p7EFD;		
    //-------------------------------------------------------------------------    
    // Quorum
    u8 p00;		
    u8 p80FD;
    //-------------------------------------------------------------------------
    
    __int64 t_states;		// inc with conf.frame by each frame
    unsigned frame_counter;	// inc each frame
    
    unsigned char aFE;		// ATM 4.50 system port
    unsigned char aFB;		// ATM 4.50 system port
    unsigned pFFF7[8];		// ATM 7.10 / ATM3(4Mb) memory map
    // |7ffd|rom|b7b6|b5..b0| b7b6 = 0 for atm2

    u8 wd_shadow[5];		// 2F, 4F, 6F, 8F, AF

    unsigned aFF77;
    unsigned active_ay;
   
    unsigned char tfmstat;	// NEDOREPO
   
    u8 pBF;			// ATM3
    u8 pBE;			// ATM3
//-----------------------------------------------------------------------------
    u16 brk_addr; // pentevo	// [NEDOREPO]
    u8 fddIO2Ram_mask;
    u8 fddIO2Ram_wr_disable;
    u8 trdos_last_ff;	//временный параметр, потом надо удалить
//-----------------------------------------------------------------------------
struct			// NEDOREPO
{ 
    u8 p83;		//
    u8 p82;		//
    u8 p81;		// 
    u8 memEna; 		//
} wiznet; 		
//-----------------------------------------------------------------------------
    unsigned char flags;

    // Цвет бордюра (то, что выводится в порт бордюра, значение 0..7, или 0..F (для ATM, PROFI))
    // значение всегда должно быть < 0x10, т.к. для размножения цвета используется умножение на константу 0x11001100
    unsigned char border_attr;
    unsigned char cmos_addr;
    unsigned char pVD;
//-----------------------------------------------------------------------------
#ifdef MOD_VID_VD				// Video Drive by SAM style
    unsigned char *vdbase;
#endif
//-----------------------------------------------------------------------------
    unsigned char pFFBA;		// SMUC
    unsigned char p7FBA;		// SMUC
    unsigned char res1;
    unsigned char res2;
//-----------------------------------------------------------------------------
// soundrive
    unsigned char p0F;	
    unsigned char p1F;
    unsigned char p4F;
    unsigned char p5F;
//-----------------------------------------------------------------------------
    struct WD1793 wd;
    FDD fdd[4];
//-----------------------------------------------------------------------------
    struct NVRAM nvram;
//-----------------------------------------------------------------------------
struct
{
    __int64 edge_change;
    unsigned char *play_pointer;	// or NULL if tape stopped
    unsigned char *end_of_tape;		// where to stop tape
    unsigned index;			// current tape block
    unsigned tape_bit;
    bool stopped;
    SNDRENDER sound;
} tape;
//-----------------------------------------------------------------------------
    unsigned char comp_pal[0x40]; // Формат палитры GggRrrBb (формат ULA+)
//-----------------------------------------------------------------------------
    // NEDOREPO
    u16 atm3_pal[0x40];	// Формат палитры %grbG11RB(low in HSB), %grbG11RB(high in LSB), inverted
//-----------------------------------------------------------------------------
    unsigned char ide_hi_byte_r;	// high byte in IDE i/o
    unsigned char ide_hi_byte_w;
    unsigned char ide_hi_byte_w1;
    unsigned char ide_read;
    unsigned char ide_write;

    unsigned char profrom_bank;
//-----------------------------------------------------------------------------
    u8 ula_plus_group;
    u8 ula_plus_pal_idx;
    bool ula_plus_en;
//-----------------------------------------------------------------------------
    COMPUTER();
};
//=============================================================================


//=============================================================================
// bits for COMPUTER::flags
#define CF_DOSPORTS	0x01	// tr-dos ports are accessible
#define CF_TRDOS	0x02	// DOSEN trigger
#define CF_SETDOSROM	0x04	// tr-dos ROM become active at #3Dxx
#define CF_LEAVEDOSRAM	0x08	// DOS ROM will be closed at executing RAM
#define CF_LEAVEDOSADR	0x10	// DOS ROM will be closed at pc>#4000
#define CF_CACHEON	0x20	// cache active
#define CF_Z80FBUS	0x40	// unstable data bus
#define CF_PROFROM	0x80	// PROF-ROM active
//=============================================================================
// bits for COMPUTER::tfmstat	//			NEDOREPO
#define CF_TFM_REG	0x02	// YM stat reg select 
				//	1 - read register
				//	0 - read status )
#define CF_TFM_FM	0x04	// YM fm part disable 
				//	0 - enable
				//	1 - disable )
#define CF_TFM_SAA	0x08	// SAA enable
				//	0 - enable
				//	1 - disable )
//=============================================================================



//=============================================================================
#define TAPE_QUANTUM 64

struct tzx_block
{
    unsigned char *data;
    unsigned datasize;    // data size, in bytes
    unsigned pause;
    union
    {
	struct
	{
	    unsigned pilot_t;
	    unsigned s1_t;
	    unsigned s2_t;
	    unsigned zero_t;
	    unsigned one_t;
	    unsigned pilot_len;
	};
	struct
	{
	    unsigned numblocks;
	    unsigned numpulses;
	};
	unsigned param;
    };
    unsigned char type;	// 0-playable, 1-pulses, 10-20 - info, etc...
    unsigned char crc;	// xor of all bytes
    char desc[128];
};
//=============================================================================
struct SNDVAL
{
    union
    {
	unsigned data;
	struct
	{
	    short left;
	    short right;
	};
    };
};
//=============================================================================
struct virtkeyt
{
    const char *name;
    unsigned short virtkey;
};
//=============================================================================
struct keyports
{
    volatile unsigned char *port1;
    volatile unsigned char *port2;
    unsigned char mask1;
    unsigned char mask2;
};
//=============================================================================
struct zxkey
{
    const char *name;
    volatile unsigned char * /*const*/ port; //Alone Coder
    /*const*/ unsigned char mask; //Alone Coder
};
//=============================================================================
struct zxkeymap
{
    const char *name;
    zxkey *zxk;
    unsigned zxk_size;
};
//=============================================================================
struct action
{
    const char *name;
    void (*func)();
    unsigned short k1;
    unsigned short k2;
    unsigned short k3;
    unsigned short k4;
};
//=============================================================================
typedef	void (*TColorConverter)	(u8 *dst, u8 *scrbuf, int dx);
	void ConvBgr32ToBgr24	(u8 *dst, u8 *scrbuf, int dx);
	void ConvYuy2ToBgr24	(u8 *dst, u8 *scrbuf, int dx);
	void ConvRgb16ToBgr24	(u8 *dst, u8 *scrbuf, int dx);
	void ConvRgb15ToBgr24	(u8 *dst, u8 *scrbuf, int dx);
	void ConvPal8ToBgr24	(u8 *dst, u8 *scrbuf, int dx);
extern TColorConverter ConvBgr24;
//=============================================================================
// flags for video filters
							// misc options
#define RF_BORDER	0x00000001	//0x00000002	// no multicolor painter, read directly from spectrum memory
#define RF_MON		0x00000002	//0x00000004	// not-flippable surface, show mouse cursor
#define RF_DRIVER	0x00000004	//0x00000008	// use options from driver
//#define RF_VSYNC	0x00000008  	//0x00000010	// force VSYNC

#define RF_D3D		0x00000008	//0x00000010	// use d3d for windowed mode
#define RF_GDI		0x00000010	//0x00000020	// output to window
#define RF_CLIP		0x00000020	//0x00000040	// use DirectDraw clipper for windowed mode
#define RF_OVR		0x00000040	//0x00000080	// output to overlay
#define RF_D3DE		0x00000080	//0x00400000	// use d3d for full screen mode

							// main area size
#define RF_1X		0x00000100	//0x00000000	// 256x192,320x240 with border (default) какой тупой мудак тут сделал 0 ?
#define RF_2X		0x00000200	//0x00000100	// default x2
#define RF_3X		0x00000400	//0x00000001	// default x3
#define RF_4X		0x00000800	//0x00000200	// default x4
#define RF_64x48	0x00001000	//0x00000400U	// 64x48  (for chunky 4x4)
#define RF_128x96	0x00002000	//0x00000800U	// 128x96 (for chunky 2x2)
//			0x00004000
//			0x00008000

#define RF_8		0x00010000	//0x00000000	// 8 bit (default) (сейчас не проверяетсо)
#define RF_8BPCH	0x00020000	//0x00001000	// 8-bit per color channel. GDI mode => 32-bit surface. 8-bit mode => grayscale palette
#define RF_YUY2		0x00040000	//0x00002000	// pixel format: 16bit, YUY2
#define RF_16		0x00080000	//0x00004000	// hicolor mode (RGB555/RGB565)
#define RF_32		0x00100000	//0x00008000	// true color
//			0x00200000
//			0x00400000
//			0x00800000

#define RF_USEC32	0x01000000	//0x00010000	// use c32tab
#define RF_USE32AS16	0x02000000	//0x0020000	// c32tab contain hi-color WORDS
#define RF_USEFONT	0x04000000	//0x00040000	// use font_tables
#define RF_PALB		0x08000000	//0x00080000	// palette for bilinear filter
#define RF_COMPPAL	0x10000000	//0x00100000	// use palette from ATM palette registers
#define RF_GRAY		0x20000000	//0x00200000	// grayscale palette
//			0x40000000
//			0x80000000

#define RF_MONITOR (RF_MON | RF_GDI | RF_2X)
//=============================================================================
extern unsigned frametime;
extern unsigned nmi_pending;
//=============================================================================
extern int main_pause_flag;	// [NS]
//=============================================================================
bool ConfirmExit();
BOOL WINAPI ConsoleHandler(DWORD CtrlType);
void showhelp(const char *anchor = nullptr);

#endif //UNREAL_EMUL_H		//какаято NEDO хуита
//=============================================================================


