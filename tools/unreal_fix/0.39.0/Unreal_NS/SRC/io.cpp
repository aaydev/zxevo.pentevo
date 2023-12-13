#include "std.h"

#include "emul.h"
#include "funcs.h"
#include "vars.h"
#include "draw.h"
#include "memory.h"
#include "atm.h"
#include "profi.h"
#include "sndrender/sndcounter.h"
#include "sound.h"
#include "gs.h"
#include "sdcard.h"
#include "zc.h"
#include "tape.h"
#include "zxevo.h"	//NEDOREPO
#include "upd765.h"
#include "zxusbnet.h"	//NEDOREPO





#define	NO_KEY	0xFF

// [NS] скан коды найдены методом научного тыка в готовых програмах
// и не есть тошно тошными
unsigned char ps2_2_atm620_xt_keyb_code[256] = 
{ 
	
	NO_KEY,		// 00		// nope
	NO_KEY,		// 0x0001,	// DIK_F9              
	NO_KEY,		// 02
	NO_KEY,		// 0x0003,	// DIK_F5   
			
	NO_KEY,		// 0x0004,	// DIK_F3              
	NO_KEY,		// 0x0005,	// DIK_F1              
	NO_KEY,		// 0x0006,	// DIK_F2              
	NO_KEY,		// 0x0007,	// DIK_F12
			
	NO_KEY,		// 08
	NO_KEY,		// 0x0009,	// DIK_F10             
	NO_KEY,		// 0x000A,	// DIK_F8              
	NO_KEY,		// 0x000B,	// DIK_F6     
			
	NO_KEY,		// 0x000C,	// DIK_F4              
	0xF0, //tab	// 0x000D,	// DIK_TAB             
	0x94, //`	// 0x000E,	// DIK_GRAVE	` возле 1 !!!
	NO_KEY,		// 0F
			
	NO_KEY,		// 10
	NO_KEY,		// 0x0011,	// DIK_LMENU	(left Alt) !!!
	NO_KEY,		// 0x0012,	// DIK_LSHIFT        
	NO_KEY,		// 13
			
	NO_KEY,		// 0x0014,	// DIK_LCONTROL	!!!   
	0x08, //Q q	// 0x0015,	// DIK_Q               
	0x40, //1	// 0x0016,	// DIK_1  
	NO_KEY,		// 17
			
	NO_KEY,		// 18
	NO_KEY,		// 19
	0x34, //Z z	// 0x001A,	// DIK_Z               
	0xF8, //S s	// 0x001B,	// DIK_S
			
	0x78, //A a	// 0x001C,	// DIK_A               
	0x88, //W w	// 0x001D,	// DIK_W               
	0xC0, //2	// 0x001E,	// DIK_2  
	NO_KEY,		// 1f
			
	NO_KEY,		// 20
	0x74, //C c	// 0x0021,	// DIK_C               
	0xB4, //X x	// 0x0022,	// DIK_X               
	0x04, //D d	// 0x0023,	// DIK_D    
			
	0x48, //E e	// 0x0024,	// DIK_E               
	0xA0, //4	// 0x0025,	// DIK_4               
	0x20, //3	// 0x0026,	// DIK_3    
	NO_KEY,		// 27
			
	NO_KEY,		// 28
	0x9C, //space	// 0x0029,	// DIK_SPACE           
	0xF4, //V v	// 0x002A,	// DIK_V               
	0x84, //F f	// 0x002B,	// DIK_F   
			
	0x28, //T t	// 0x002C,	// DIK_T               
	0xC8, //R r	// 0x002D,	// DIK_R               
	0x60, //5	// 0x002E,	// DIK_5    
	NO_KEY,		// 2f
			
	NO_KEY,		// 30
	0x8C, //N n	// 0x0031,	// DIK_N               
	0x0C, //B b	// 0x0032,	// DIK_B               
	0xC4, //H h	// 0x0033,	// DIK_H       
			
	0x44, //G g	// 0x0034,	// DIK_G               
	0xA8, //Y y	// 0x0035,	// DIK_Y               
	0xE0, //6	// 0x0036,	// DIK_6      
	NO_KEY,		// 37
			
	NO_KEY,		// 38
	NO_KEY,		// 39
	0x4C, //M m	// 0x003A,	// DIK_M
	0x24, //J j	// 0x003B,	// DIK_J               
			
	0x68, //U u	// 0x003C,	// DIK_U               
	0x10, //7	// 0x003D,	// DIK_7   
	0x90, //8	// 0x003E,	// DIK_8
	NO_KEY,		// 3f
			
	NO_KEY,		// 40
	0xCC, //,	// 0x0041,	// DIK_COMMA           
	0xA4, //K k	// 0x0042,	// DIK_K               
	0xE8, //I i	// 0x0043,	// DIK_I 
			
	0x18, //O o	// 0x0044,	// DIK_O char???           
	0xD0, //0	// 0x0045,	// DIK_0 zero???
	0x50, //9	// 0x0046,	// DIK_9  
	NO_KEY,		// 47
			
	NO_KEY,		// 48
	0x2C, //.	// 0x0049,	// DIK_PERIOD		/* . on main keyboard */
	0xAC, // /	// 0x004A,	// DIK_SLASH		/* / on main keyboard */
	0x64, //L l	// 0x004B,	// DIK_L
			
	0xE4, //; ????	// 0x004C,	// DIK_SEMICOLON       
	0x98, //P p	// 0x004D,	// DIK_P
	0x30, //- _	// 0x004E,	// DIK_MINUS
	NO_KEY,		// 4f
			
	NO_KEY,		// 50
	NO_KEY,		// 51
	0x14, //' ????	// 0x0052,	// DIK_APOSTROPHE    
	NO_KEY,		// 53
			
	0x58,	//[ {	// 0x0054,	// DIK_LBRACKET        	[ {
	0xB0,	//= +	// 0x0055,	// DIK_EQUALS    
	NO_KEY,		// 56
	NO_KEY,		// 57
			
	0x00,	//????	// 0x0058,	// DIK_CAPITAL         
	NO_KEY,		// 0x0059,	// DIK_RSHIFT          
	0x38,	//ENT??	// 0x005A,	// DIK_RETURN		MAIN ENTER !!!
	0xD8,	//] }	// 0x005B,	// DIK_RBRACKET    	] } !!!
			
	NO_KEY,		// 5c
	0xD4, // \ ???	// 0x005D,	// DIK_BACKSLASH 
	NO_KEY,		// 5e
	NO_KEY,		// 5f

	NO_KEY,		// 60
	NO_KEY,		// 61
	NO_KEY,		// 62
	NO_KEY,		// 63
			
	NO_KEY,		// 64
	NO_KEY,		// 65
	0x70, //bcksp??	// 0x0066,	// DIK_BACK
	NO_KEY,		// 67
			
	NO_KEY,		// 68
	NO_KEY,		// 0x0069,	// DIK_NUMPAD1   
	NO_KEY,		// 6a			
	NO_KEY,		// 0x006B,	// DIK_NUMPAD4    
			
	NO_KEY,		// 0x006C,	// DIK_NUMPAD7   
	NO_KEY,		// 6d
	NO_KEY,		// 6e
	NO_KEY,		// 6f
			
	NO_KEY,		// 0x0070,	// DIK_NUMPAD0         
	NO_KEY,		// 0x0071,	// DIK_DECIMAL		. on numeric keypad
	NO_KEY,		// 0x0072,	// DIK_NUMPAD2         
	NO_KEY,		// 0x0073,	// DIK_NUMPAD5    
			
	NO_KEY,		// 0x0074,	// DIK_NUMPAD6         
	NO_KEY,		// 0x0075,	// DIK_NUMPAD8         
	NO_KEY,		// 0x0076,	// DIK_ESCAPE
	NO_KEY,		// 0x0077,	// DIK_NUMLOCK    
			
	NO_KEY,		// 0x0078,	// DIK_F11             
	0x72, //+(num)	// 0x0079,	// DIK_ADD		+ on numeric keypad !!!
	NO_KEY,		// 0x007A,	// DIK_NUMPAD3         
	0x52, //-(num)	// 0x007B,	// DIK_SUBTRACT		- on numeric keypad
			
	0xEC, //*(num)	// 0x007C,	// DIK_MULTIPLY		* on numeric keypad
	NO_KEY,		// 0x007D,	// DIK_NUMPAD9         
	NO_KEY,		// 0x007E,	// DIK_SCROLL		/* Scroll Lock */
	NO_KEY,		// 7f

	NO_KEY,		// 80
	NO_KEY,		// 81
	NO_KEY,		// 82
	NO_KEY		// 0x0083,	// DIK_F7   

	
			// 0x0111,	// DIK_RMENU		/* right Alt */
			// 0x0112,	// DIK_SYSRQ           
			// 0x0114,	// DIK_RCONTROL        
			// 0x011F,	// DIK_LWIN		/* Left Windows key */
			// 0x0127,	// DIK_RWIN		/* Right Windows key */
			// 0x012F,	// DIK_APPS		/* AppMenu key */
			// 0x0137,	// DIK_POWER		/* System Power */
			// 0x013F,	// DIK_SLEEP		/* System Sleep */
			// 0x014A,	// DIK_DIVIDE		/* / on numeric keypad */
			// 0x015A,	// DIK_NUMPADENTER	/* Enter on numeric keypad */
			// 0x015E,	// DIK_WAKE		/* System Wake */
			// 0x0169,	// DIK_END		/* End on arrow keypad */
			// 0x016B,	// DIK_LEFT		/* LeftArrow on arrow keypad */
			// 0x016C,	// DIK_HOME		/* Home on arrow keypad */
			// 0x0170,	// DIK_INSERT		/* Insert on arrow keypad */
			// 0x0171,	// DIK_DELETE		/* Delete on arrow keypad */
			// 0x0172,	// DIK_DOWN		/* DownArrow on arrow keypad */
			// 0x0174,	// DIK_RIGHT		/* RightArrow on arrow keypad */
			// 0x0175,	// DIK_UP		/* UpArrow on arrow keypad */
			// 0x017A,	// DIK_NEXT		/* PgDn on arrow keypad */
			// 0x017D,	// DIK_PRIOR		/* PgUp on arrow keypad */
    
};   
    

// 2DO
//
// - 	можот всякие goto ***_hdd вынести inline функцией?





//=============================================================================
// MEMORY
//=============================================================================

//=============================================================================
// Cache FB - IN					
//=============================================================================
inline int dc_in__Cache_FB( unsigned port)
{
//  кеш включаетсо именно чтением
//  и на пентагоне это вообще включение ПЗУ принтера
//  которое надо включать блджд!

//  if ((port & 0x7F) == 0x7B) { // FB/7B

    if ((port & 0x04) == 0x00)
    {
	// пока оставлено в оригинальном виде
	//	не ясно что это там за кеш в атм1
	//---------------------------------------------------------------------
	// FB/7B //Alone Coder 0.36.6 (for MODPLAYi)
	if (conf.mem_model == MM_ATM450)
	{
	    comp.aFB = (unsigned char)port;
	    set_banks();
	}
	//---------------------------------------------------------------------
	else if (conf.cache)
	{
	    comp.flags &= ~CF_CACHEON;
	    if (port & 0x80)
	        comp.flags |= CF_CACHEON;
	    set_banks();
	}//---------------------------------------------------------------------
	return 0xFF;	// удолить !!! ибо это не на всех машинах и мешает декодировать следующие порты!!!
	//---------------------------------------------------------------------
   }
    return -1;	// провверять другие порты
}
//=============================================================================


//=============================================================================
// VIDEO
//=============================================================================




//=============================================================================
// ULA Plus - Out

inline int dc_out__Ula_Plus( unsigned port, unsigned char val )
{
    if (conf.ula_plus)
    {
	//---------------------------------------------------------------------
	if (port == 0xBF3B)
	{
	    comp.ula_plus_group = val >> 6;
	    if (comp.ula_plus_group == 0)
	    {
		comp.ula_plus_pal_idx = val & 0x3F;
	    }
	    return 1;	//out end
	}
	//---------------------------------------------------------------------
	if (port == 0xFF3B)
	{
	    //-----------------------------------------------------------------
	    if (comp.ula_plus_group == 0)
	    {
		comp.comp_pal[comp.ula_plus_pal_idx] = val;
		temp.comp_pal_changed = 1;
		return 1;	//out end
	    }
	    //-----------------------------------------------------------------
	    if (comp.ula_plus_group == 1)
	    {
		bool en = (val & 1) != 0;
		if (comp.ula_plus_en != en)
		{
		    comp.ula_plus_en = en;
		    if (comp.ula_plus_en)
		    {
			temp.rflags |= RF_COMPPAL | RF_PALB;
		    }
		    else
		    {
			temp.rflags &= unsigned(~(RF_COMPPAL | RF_PALB));
		    }
		    video_color_tables();
		    temp.comp_pal_changed = 1;
		}
		return 1;	//out end
	    }
	    //-----------------------------------------------------------------
	   return 1;	//out end
	}
	//---------------------------------------------------------------------
    }
    return 0;	// провверять другие порты
}
//=============================================================================
// ULA Plus - In

inline int dc_in__Ula_Plus( unsigned port )
{
    if (conf.ula_plus)
    {
	if (port == 0xFF3B)
	{
	    //-----------------------------------------------------------------
	    if (comp.ula_plus_group == 0)
	    {
		return comp.comp_pal[comp.ula_plus_pal_idx];
	    }
	    //-----------------------------------------------------------------
	    if (comp.ula_plus_group == 1)
	    {
		u8 val = comp.ula_plus_en ? 1 : 0;
		return val;
	    }
	    //-----------------------------------------------------------------
	    return 0xFF;
	}
    }
    return -1;	// провверять другие порты
}
//=============================================================================













//=============================================================================
// SOUND
//=============================================================================



//=============================================================================
// NGS - Out
//=============================================================================
inline int dc_out__NGS( unsigned port, u8 p1, unsigned char val )
{
//-----------------------------------------------------------------------------
#ifdef MOD_GS
    if (conf.gs_type)		//0 - nope
    {				//1 - Z80
				//2 - BASS  
    // 10111011 | BB
    // 10110011 | B3
    // 00110011 | 33
   
	// mask 11111111 ?
	if ((port & 0xFF) == 0x33)	// 33
	{
	    out_gs(p1, val);
	    return 1;	//out end
	}
	// mask 11110111
	if ((port & 0xF7) == 0xB3)	// BB, B3
	{
	    out_gs(p1, val);
	    return 1;	//out end
	}
	
    }
#endif
//-----------------------------------------------------------------------------

    return 0;	// провверять другие порты
}
//=============================================================================
// NGS - In
//=============================================================================
inline int dc_in__NGS( unsigned port, u8 p1 )
{

#ifdef MOD_GS
    if (conf.gs_type)		//0 - nope
    {				//1 - Z80
				//2 - BASS 
	// ngs
	if ( (port & 0xF7) == 0xB3)
	    return in_gs(p1);	
    }		
#endif
    return -1;	// провверять другие порты
}
//=============================================================================




//=============================================================================
// AY, TS, TSFM, TSFM Pro, pseudo, quadro, POS сборная недосолянка - OUT				
//=============================================================================
inline int dc_out__AY_TS_TSFM_nedoSAA_etc( unsigned port, unsigned char val )
{

//  // #xD		//сейчас еще под таким колпаком во время вызова
//  if (!(port & 2))
//  {
	// разбить на n типов отдельных БЛДЖД ay
	if (conf.sound.ay_scheme >= AY_SCHEME_SINGLE)
	{
	    if ((port & 0xC0FF) == 0xC0FD )	//оптимизаторный ебанизм
	    {
		//-------------------------------------------------------------
		// A15=A14=1, FxFD - AY select register
	//	if ((conf.sound.ay_scheme == AY_SCHEME_CHRV) && ((val & 0xF8) == 0xF8)) //Alone Coder	0.39.0
		if ((conf.sound.ay_scheme == AY_SCHEME_CHRV) && ((val & 0xF0) == 0xF0)) //Alone Coder	NEDOREPO
		{
		    //---------------------------------------------------------
		    if (conf.sound.ay_chip == (SNDCHIP::CHIP_YM2203))
		    {
			comp.tfmstat = val;	//NEDOREPO
			fmsoundon0 = val & 4;
			tfmstatuson0 = val & 2;
		    } //Alone Coder 0.36.6
		    //---------------------------------------------------------
		    comp.active_ay = val & 1;
		    return 1;	//out end		//ретурн в NEDOREPO
		};
		//-------------------------------------------------------------
		if ((conf.sound.saa1099 == SAA_TFM_PRO) && ((comp.tfmstat & CF_TFM_SAA) == 0))
		{
		    Saa1099.WrCtl(val);
		}
		//-------------------------------------------------------------
		else
		{
		    unsigned n_ay = (conf.sound.ay_scheme == AY_SCHEME_QUADRO)? (port >> 12) & 1 : comp.active_ay;
		    ay[n_ay].select(val);
		}
		//-------------------------------------------------------------
		return 1;	//out end
	    }
	}
	//---------------------------------------------------------------------
	// if ((port & 0xC000)==0x8000 && conf.sound.ay_scheme >= AY_SCHEME_SINGLE)
	//---------------------------------------------------------------------
	// TSFM PRO SAA PART?
	
	if (conf.sound.ay_scheme)
	{
	    if ((port & 0xC000) == 0x8000)	//NEDOREPO
	    {  
		// BFFD - AY data register
		if (	(conf.sound.saa1099 == SAA_TFM_PRO) &&
			( (comp.tfmstat & CF_TFM_SAA) == 0)
		 )
		{
		    Saa1099.WrData(temp.sndblock ? 0 : cpu.t, val);
		}
		else
		{
		    unsigned n_ay = (conf.sound.ay_scheme == AY_SCHEME_QUADRO) ? (port >> 12) & 1 : comp.active_ay;
		    ay[n_ay].write(temp.sndblock ? 0 : cpu.t, val);
		    
		    if ( (conf.input.mouse == 2) && (ay[n_ay].get_activereg() == 14) )
			input.aymouse_wr(val);
		}
		return 1;	//out end
	    }
	}
	
	
//  }
    return 0;	// OUT - провверять другие порты
}
//=============================================================================
// AY, TS, TSFM, TSFM Pro, pseudo, quadro, POS сборная недосолянка - IN					
//=============================================================================
inline int dc_in__AY_TS_TSFM_etc( unsigned port)
{
    if (conf.sound.ay_scheme >= AY_SCHEME_SINGLE)
    {
	if ((unsigned char)port == 0xFD)
	{
	    //-----------------------------------------------------------------
	    if (	(conf.sound.ay_scheme == AY_SCHEME_CHRV)	&&
			(conf.sound.ay_chip == (SNDCHIP::CHIP_YM2203))	&&
			(tfmstatuson0 == 0)
	      )
	    {
		return 0x7F; //always ready 		//Alone Coder 0.36.6
	    }
	    //-----------------------------------------------------------------
	    if ((port & 0xC0FF) != 0xC0FD)
	        return 0xFF;
	    //-----------------------------------------------------------------
	    unsigned n_ay = (conf.sound.ay_scheme == AY_SCHEME_QUADRO)    ?	(port >> 12) & 1 :
										comp.active_ay;
	    //-----------------------------------------------------------------
	    // AY Mouse
	    if (conf.input.mouse == 2)
	    {
		if (ay[n_ay].get_activereg() == 14)
		{
		    input.mouse_joy_led |= 1;
		    return input.aymouse_rd();
		}
	    }
	    //-----------------------------------------------------------------
	    // else FxFD - read selected AY register
	    return ay[n_ay].read();
	    //-----------------------------------------------------------------
	}
    }
    return -1;	// IN - провверять другие порты
}
//=============================================================================








//=============================================================================
// Fuller AY							

inline int dc_out__Fuller_AY( u8 p1, unsigned char val )
{
    if (conf.sound.ay_scheme == AY_SCHEME_FULLER)
    {   
	//---------------------------------------------------------------------
	// fuller AY register select
	if (p1 == 0x3F)		
	{
	    ay[0].select(val);
	    return 1;	//out end
	}
	//---------------------------------------------------------------------
	// fuller AY data
	if (p1 == 0x5F)		
	{
	    ay[0].write( temp.sndblock ? 0 : cpu.t, val );
	    return 1;	//out end
	}
	//---------------------------------------------------------------------
    }
    return 0;	// провверять другие порты
}
//=============================================================================


//=============================================================================
// POS AY							

inline int dc_out__POS_AY( unsigned port, unsigned char val )
{
    if (conf.sound.ay_scheme == AY_SCHEME_POS)
    {
	if ( (unsigned char) port == 0x1F )
	{
	    comp.active_ay = val & 1;
	    return 1;	//out end
	}
    }
    return 0;	// провверять другие порты
}
//=============================================================================


//=============================================================================
// ZXI TSFM - OUT							//[NS]
//=============================================================================
inline int dc_out__ZXI_TSFM_Mirror( unsigned port, unsigned char val )
{
    if (conf.ZXI_TSFM_Mirror != 0)
    {
	//---------------------------------------------------------------------
	if (port == 0x7F3B)	//select reg	аналог FFFB
	{
	    if (((val & 0xF0) == 0xF0))
	    {
		if (conf.sound.ay_chip == (SNDCHIP::CHIP_YM2203))
		{
		    comp.tfmstat = val;
		    fmsoundon0 = val & 4;
		    tfmstatuson0 = val & 2;
		}
		comp.active_ay = val & 1;
	    }
	    else
	    {
		unsigned n_ay = comp.active_ay;
		ay[n_ay].select(val);
	    }
	    return 1;	//out end
	}
	//---------------------------------------------------------------------
	if (port == 0x7E3B)	//data reg	аналог BFFB
	{
	    unsigned n_ay = comp.active_ay;
	    ay[n_ay].write(temp.sndblock? 0 : cpu.t, val);
	    return 1;	//out end
	}
	//---------------------------------------------------------------------
    }
    return 0;	// OUT - провверять другие порты
}
//=============================================================================
// ZXI TSFM - IN							//[NS]
//=============================================================================
inline int dc_in__ZXI_TSFM_Mirror( unsigned port )
{
    if (conf.ZXI_TSFM_Mirror)
    {
	if (port == 0x7F3B)
	{
	    //-----------------------------------------------------------------
	    if ((conf.sound.ay_chip == (SNDCHIP::CHIP_YM2203)) && (tfmstatuson0 == 0))
		return 0x7f; //always ready
	    //-----------------------------------------------------------------
	    unsigned n_ay = comp.active_ay;
	    // else FxFD - read selected AY register
	    return ay[n_ay].read();
	    //-----------------------------------------------------------------
	}   
    }
    return -1;	// IN - провверять другие порты
}
//=============================================================================



//=============================================================================
// ZXM saa1099						

inline int dc_out__ZXM_saa1099( unsigned port, unsigned char val )
{
    if (conf.sound.saa1099 == SAA_ZXM)		//NEDOREPO
    {
	// saa1099
	if (((port & 0xFF) == 0xFF))
	{
	    if (port & 0x100)
		Saa1099.WrCtl(val);
	    else
		Saa1099.WrData( temp.sndblock ? 0 : cpu.t, val );
	    return 1;	//out endv
	}
    }
    return 0;	// провверять другие порты
}
//=============================================================================



//=============================================================================
// Covox DD							

inline int dc_out__Covox_DD( unsigned port, unsigned char val )
{
//  if (!(port & 2))	//сейчас под
//  {
	if (conf.sound.covoxDD)
	{
	    // port DD - covox
	    if ((unsigned char)port == 0xDD)
	    { 
		//  __debugbreak();
		//  flush_dig_snd();		//ОБНОВЛЕНИЕ ДО ЗАПИСИ
		covDD_vol = val * conf.sound.covoxDD_vol / 0x100;
		flush_dig_snd();		//[NS] FIX?
		return 1;	//out end
	    }
	}
//  }
    return 0;	// провверять другие порты
}
//=============================================================================


//=============================================================================
// Covox FB						

inline int dc_out__Covox_FB( unsigned port, unsigned char val )
{
    if (conf.sound.covoxFB)
    {
	// port FB - covox
	if (!(port & 4))
	{ 
	    // __debugbreak();
	    // flush_dig_snd();		//ОБНОВЛЕНИЕ ДО ЗАПИСИ
	    covFB_vol = val * conf.sound.covoxFB_vol / 0x100;
	    flush_dig_snd();			//[NS] FIX
	    return 1;	//out end
	}
    }
    return 0;	// провверять другие порты
}
//=============================================================================


//=============================================================================
// Soundrive							

inline int dc_out__Soundrive( unsigned port, unsigned char val )
{
    if (conf.sound.sd)
    {
	// soundrive
	// маска	1.1.1111 - AF ?
	//
	//		....1111 - 0F
	//		...11111 - 1F
	//		.1..1111 - 4F
	//		.1.11111 - 5F
	if ((port & 0xAF) == 0x0F)
	{ 
	    // __debugbreak();
	    // и вся неполная дешифрация выше идет лесом?
	    if ( (unsigned char) port == 0x0F ) comp.p0F = val;
	    if ( (unsigned char) port == 0x1F ) comp.p1F = val;
	    if ( (unsigned char) port == 0x4F ) comp.p4F = val;
	    if ( (unsigned char) port == 0x5F ) comp.p5F = val;
	    // flush_dig_snd();			//ОБНОВЛЕНИЕ ДО ЗАПИСИ !!!
	    sd_l = (conf.sound.sd_vol * (comp.p0F+comp.p1F)) >> 8;
	    sd_r = (conf.sound.sd_vol * (comp.p4F+comp.p5F)) >> 8;
	    flush_dig_snd();			//[NS] FIX
	    return 1;	//out end
	}
    }
    return 0;	// провверять другие порты
}
//=============================================================================








//=============================================================================
// IDE
//=============================================================================


//=============================================================================
// NEMO IDE - OUT
//=============================================================================
inline int dc_out__NEMO_IDE( unsigned port, unsigned char val )
{
    if (conf.ide_scheme == IDE_NEMO || conf.ide_scheme == IDE_NEMO_A8)
    {
	if (!(port & 6))
	{
	    unsigned hi_byte = (conf.ide_scheme == IDE_NEMO) ? (port & 1) : (port & 0x100);
	    //-----------------------------------------------------------------
	    if (hi_byte)
	    {
		comp.ide_write = val;
		return 1;	//out end
	    }
	    //-----------------------------------------------------------------
	    if ((port & 0x18) == 0x08)
	    {
		if ((port & 0xE0) == 0xC0)
		    hdd.write(8, val);
		return 1;	//out end
	    } // CS1=0,CS0=1,reg=6
	    //-----------------------------------------------------------------
	    if ((port & 0x18) != 0x10)
		return 1;	//out end 	// invalid CS0,CS1
	    //-----------------------------------------------------------------
	    //goto write_hdd_5;
	    {
		//write_hdd_5:
		port >>= 5;
		//write_hdd:
		port &= 7;
		if (port)
		    hdd.write(port, val);
		else
		    hdd.write_data(unsigned(val | (comp.ide_write << 8)));
		return 1;	//out end
	    }
	}
    }
    return 0;	// OUT - провверять другие порты	
}
//=============================================================================
// NEMO IDE - IN
//=============================================================================
inline int dc_in__NEMO_IDE( unsigned port )			//NO SHADOW
{
    if (conf.ide_scheme == IDE_NEMO || conf.ide_scheme == IDE_NEMO_A8)
    {
	if (!(port & 6))
	{
	    //-----------------------------------------------------------------
	    unsigned hi_byte = (conf.ide_scheme == IDE_NEMO) ?	(port & 1) :
								(port & 0x100);
	    if (hi_byte)
		return comp.ide_read;
	    //-----------------------------------------------------------------
	    comp.ide_read = 0xFF;
	    //-----------------------------------------------------------------
	    if ((port & 0x18) == 0x08)
		return ((port & 0xE0) == 0xC0)    ?	hdd.read(8) :
							0xFF;	// CS1=0,CS0=1,reg=6
	    //-----------------------------------------------------------------
	    if ((port & 0x18) != 0x10)
		return 0xFF;	// invalid CS0,CS1
	    //-----------------------------------------------------------------
	    //goto read_hdd_5;
	    //read_hdd_5:
	    {
		port >>= 5;
	    //read_hdd:
		port &= 7;
		if (port)
		    return hdd.read(port);
		unsigned v = hdd.read_data();
		comp.ide_read = (unsigned char)(v >> 8);
		return (unsigned char) v;
	    }
	} 
    }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================



//=============================================================================
// DivIDE на nemo портах - OUT
//=============================================================================
inline int dc_out__NEMO_DivIDE( unsigned port, unsigned char val )
{
    if (conf.ide_scheme == IDE_NEMO_DIVIDE)
    {
	//-------------------------------------------------------------------------
	// 00011110 mask?
	if ((port & 0x1E) == 0x10) 	// rrr1000x
	{
	    //-----------------------------------------------------------------
	    if ((port & 0xFF) == 0x11)
	    {
		comp.ide_write = val;
		comp.ide_hi_byte_w = 0;
		comp.ide_hi_byte_w1 = 1;
		return 1;	//out end
	    }
	    //-----------------------------------------------------------------
	    if ((port & 0xFE) == 0x10)
	    {
		comp.ide_hi_byte_w ^= 1;

		// Была запись в порт 0x11 (старший байт уже запомнен)
		if (comp.ide_hi_byte_w1) 
		{
		    comp.ide_hi_byte_w1 = 0;
		}
		// не была запись ???
		else
		{
		    if (comp.ide_hi_byte_w) 
		    {
			// Запоминаем младший байт
			comp.ide_write = val;
			return 1;	//out end
		    }
		    else 
		    {
			// Меняем старший и младший байты местами (как этого ожидает write_hdd_5)
			u8 tmp = comp.ide_write;
			comp.ide_write = val;
			val = tmp;
		    }
		}
	    }
	    //-----------------------------------------------------------------
	    else
	    {
		comp.ide_hi_byte_w = 0;
	    }
	    //goto write_hdd_5;
	    {
		//write_hdd_5:
		port >>= 5;
		//write_hdd:
		port &= 7;
		if (port)
		    hdd.write(port, val);
		else
		    hdd.write_data(unsigned(val | (comp.ide_write << 8)));
		return 1;	//out end
	    }
	}
	//-------------------------------------------------------------------------
	else if ((port & 0xFF) == 0xC8)
	{
	    hdd.write(8, val);
	    return 1;	//out end
	}
	//-------------------------------------------------------------------------
    }
    return 0;	// OUT провверять другие порты	
}
//=============================================================================
// DivIDE на nemo портах - IN
//=============================================================================
inline int dc_in__NEMO_DivIDE( unsigned port )
{
    if (conf.ide_scheme == IDE_NEMO_DIVIDE)
    {
	//---------------------------------------------------------------------
	// divide на nemo портах
	if (((port & 0x1E) == 0x10)) // rrr1000x
	{
	    //-----------------------------------------------------------------
	    if ((port & 0xFF) == 0x11)
	    {
		comp.ide_hi_byte_r = 0;
		return comp.ide_read;
	    }
	    //-----------------------------------------------------------------
	    if ((port & 0xFE) == 0x10)
	    {
		comp.ide_hi_byte_r ^= 1;
		
		if (!comp.ide_hi_byte_r)
		{
		    return comp.ide_read;
		}
	    }
	    //-----------------------------------------------------------------
	    else
	    {
		comp.ide_hi_byte_r = 0;
	    }
	    //-----------------------------------------------------------------
	    //goto read_hdd_5;
	    //read_hdd_5:
	    {
		port >>= 5;
	    //read_hdd:
		port &= 7;
		if (port)
		    return hdd.read(port);
		unsigned v = hdd.read_data();
		comp.ide_read = (unsigned char)(v >> 8);
		return (unsigned char) v;
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	else if ((port & 0xFF) == 0xC8)
	{
	    return hdd.read(8);
	}
	//---------------------------------------------------------------------
    }
    return -1;	// IN провверять другие порты	
}
//=============================================================================



//=============================================================================
// ATM IDE - OUT
//=============================================================================
inline int dc_out__ATM_IDE( unsigned port, unsigned char val )
{
    if (conf.ide_scheme == IDE_ATM && (port & 0x1F) == 0x0F)
    {
	if (port & 0x100)
	{
	    comp.ide_write = val;
	    return 1;	//out end
	}
	//write_hdd_5:
	port >>= 5;
	//write_hdd:
	port &= 7;
	
	if (port)
	    hdd.write(port, val);
	else
	    hdd.write_data(unsigned(val | (comp.ide_write << 8)));
	return 1;	//out end
    }
    return 0;	// OUT - провверять другие порты	
}
//=============================================================================
// ATM IDE - IN
//=============================================================================
inline int dc_in__ATM_IDE( unsigned port )
{
//  if (comp.flags & CF_DOSPORTS)	// щас shadow проверка до вызова
//  {
	if (conf.ide_scheme == IDE_ATM)
	{
	    if ((port & 0x1F) == 0x0F)
	    {
		//-------------------------------------------------------------
		if (port & 0x100)
		    return comp.ide_read;
		//-------------------------------------------------------------
		//read_hdd_5:
		port >>= 5;
		//read_hdd:
		port &= 7;
		if (port)
		    return hdd.read(port);
		//-------------------------------------------------------------
		unsigned v = hdd.read_data();
		comp.ide_read = (unsigned char)(v >> 8);
		return (unsigned char) v;
		//-------------------------------------------------------------
	    }
	}
//  }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================
// ATM IDE - IN							//NO SHADOW?
//=============================================================================
inline int dc_in__ATM_IDE_intrq( unsigned port )
{
    if ((conf.mem_model == MM_ATM710) || (conf.ide_scheme == IDE_ATM))
    {
	if ((port & 0x8202) == (0x7FFD & 0x8202))
	{ 
	    //-----------------------------------------------------------------
	    // ATM-2 IDE+DAC/ADC
	    unsigned char irq = 0x40;
	    //-----------------------------------------------------------------
	    if (conf.ide_scheme == IDE_ATM)
		irq = (hdd.read_intrq() & 0x40);
	    //-----------------------------------------------------------------
	    // MSX DOS
	    // in #7FFD (выборка: %0nnnnn1n nnnnnn0n)
	    // для 6.40 - или данные с XT-клавиатуры, или (D7=0) - сигнал готовности АЦП
	    if (conf.atm620_force_xt_keyb_ready)
		irq |= 0x80;						// [NS]
	    //-----------------------------------------------------------------
	    return irq + 0x3F;
	    //-----------------------------------------------------------------
	}
    }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================



//=============================================================================
// Profi IDE - OUT
//=============================================================================
inline int dc_out__Profi_IDE( unsigned port, u8 p1, unsigned char val )
{
    // вынесено из профи портов
    // раньше надо было включать и MM_PROFI и IDE_PROFI одновременно
    if (conf.ide_scheme == IDE_PROFI)
    {
	// было под таким нечтом в оригинале
	if ((comp.p7FFD & 0x10) && (comp.pDFFD & 0x20)) // modified ports
	{
	    // IDE (AB=10101011, CB=11001011, EB=11101011)
	    if ((p1 & 0x9F) == 0x8B )
	    {
		//-------------------------------------------------------------
		if (p1 & 0x40)
		{    
		    // cs1
		    if (!(p1 & 0x20))
		    {
			comp.ide_write = val;
			return 1;	//out end
		    }
		    //---------------------------------------------------------
		    port >>= 8;
		    //goto write_hdd;
		    {
			//write_hdd:
			port &= 7;
			if (port)
			    hdd.write(port, val);
			else
			    hdd.write_data(unsigned(val | (comp.ide_write << 8)));
			return 1;	//out end
		    }
		}
		//-------------------------------------------------------------
		// cs3
		if (p1 & 0x20)
		{
		    if (((port>>8) & 7) == 6)
			hdd.write(8, val);
		    return 1;	//out end
		}
		//-------------------------------------------------------------
	    }	
	}
    }
    return 0;	// OUT - провверять другие порты	
}
//=============================================================================
// Profi IDE - IN
//=============================================================================
inline int dc_in__Profi_IDE( unsigned port, u8 p1 )
{
    if (conf.ide_scheme == IDE_PROFI)
    {
//  if (comp.flags & CF_DOSPORTS)	//щас вокруг
//  {
	// modified ports
	if ((comp.p7FFD & 0x10) && (comp.pDFFD & 0x20))
	{ 
	    // IDE
	    if ((p1 & 0x9F) == 0x8B)	//&& (conf.ide_scheme == IDE_PROFI)
	    {
		if (p1 & 0x40)		// cs1
		{
		    if (p1 & 0x20)
			return comp.ide_read;
		    port >>= 8;
		    //goto read_hdd;
		    //read_hdd:
		    {
			port &= 7;
			if (port)
			    return hdd.read(port);
			//-----------------------------------------------------
			unsigned v = hdd.read_data();
			comp.ide_read = (unsigned char)(v >> 8);
			return (unsigned char) v;
			//-----------------------------------------------------
		    }
		}
	    }  
	}
//  }
    }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================



//=============================================================================
// DivIDE - OUT
//=============================================================================
inline int dc_out__DivIDE( unsigned port, unsigned char val )
{
    if (conf.ide_scheme == IDE_DIVIDE)
    {
	//---------------------------------------------------------------------
	if ((port & 0xA3) == 0xA3)	//шозанах?
	{
	    //-----------------------------------------------------------------
	    if ((port & 0xFF) == 0xA3)
	    {
		comp.ide_hi_byte_w ^= 1;
		if (comp.ide_hi_byte_w)
		{
		    comp.ide_write = val;
		    return 1;	//out end
		}
		u8 tmp = comp.ide_write;
		comp.ide_write = val;
		val = tmp;
	    }
	    //-----------------------------------------------------------------
	    else
	    {
		comp.ide_hi_byte_w = 0;
	    }
	    //-----------------------------------------------------------------
	    port >>= 2;
	    //goto write_hdd;
	    {
		//write_hdd:
		port &= 7;
		if (port)
		    hdd.write(port, val);
		else
		    hdd.write_data(unsigned(val | (comp.ide_write << 8)));
		return 1;	//out end
	    }
	}
	//---------------------------------------------------------------------
    }
    return 0;	// провверять другие порты	
}
//=============================================================================
// DivIDE - IN
//=============================================================================
inline int dc_in__DivIDE( unsigned port)	// NO SHADOW
{
    if (conf.ide_scheme == IDE_DIVIDE)
    {
	if (((port & 0xA3) == 0xA3))
	{
	    //-----------------------------------------------------------------
	    if((port & 0xFF) == 0xA3)
	    {
		comp.ide_hi_byte_r ^= 1;
		if (!comp.ide_hi_byte_r)
		{
		    return comp.ide_read;
		}
	    }
	    //-----------------------------------------------------------------
	    else
	    {
		comp.ide_hi_byte_r = 0;
	    }
	    //-----------------------------------------------------------------
	    port >>= 2;
	    //goto read_hdd;
	    //read_hdd:
	    {
	        //-------------------------------------------------------------
		port &= 7;
		if (port)
		    return hdd.read(port);
		//-------------------------------------------------------------
		unsigned v = hdd.read_data();
		comp.ide_read = (unsigned char)(v >> 8);
		return (unsigned char) v;
		//-------------------------------------------------------------
	    }
	}
    }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================




//=============================================================================
// SMUC	- OUT		(+ SMUC IDE чтобы не разгребать эту муть)
//=============================================================================
inline int dc_out__SMUC( unsigned port, unsigned char val )
{
//  if (comp.flags & CF_DOSPORTS)	//щас вокруг
//  {
    if ( (port & 0x18A3) == (0xFFFE & 0x18A3) )	//чозанах? о_О
    { 
	//---------------------------------------------------------------------
	// SMUC
	if (conf.smuc)
	{
	    //-----------------------------------------------------------------
	    if ( (port & 0xA044) == (0xDFBA & 0xA044) )
            { 
		// clock
		if (comp.pFFBA & 0x80)
		    cmos_write(val);
		else
		    comp.cmos_addr = val;
		return 1;	//out end
	    }
	    //-----------------------------------------------------------------
            // SMUC system port
	    if ( (port & 0xA044) == (0xFFBA & 0xA044) )
	    { 
		if ( (val & 1) && (conf.ide_scheme == IDE_SMUC) )
		    hdd.reset();
		comp.nvram.write(val);
		comp.pFFBA = val;
		return 1;	//out end
	    }
	    //-----------------------------------------------------------------
	    if ( (port & 0xA044) == (0x7FBA & 0xA044) )
	    {
		comp.p7FBA = val;
		return 1;	//out end
            }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	// SMUC IDE	  
	if ( ((port & 0x8044) == (0xFFBE & 0x8044)) && (conf.ide_scheme == IDE_SMUC) )
	{ 
	    // FFBE, FEBE
	    if (comp.pFFBA & 0x80)
	    {
		if(!(port & 0x100))
		    hdd.write(8, val);		// control register
		return 1;	//out end
	    }
	    //-----------------------------------------------------------------
	    if (!(port & 0x2000))
	    {
		comp.ide_write = val;
		return 1;	//out end
	    }
	    port >>= 8;
	    //goto write_hdd;
	    {
		//write_hdd:
		port &= 7;
		if (port)
		    hdd.write(port, val);
		else
		    hdd.write_data(unsigned(val | (comp.ide_write << 8)));
		return 1;	//out end
	    }
	}
	//--------------------------------------------------------------------- 
    }
//  }   
    return 0;	// провверять другие порты	
}
//=============================================================================
// SMUC	- IN		(+ SMUC IDE чтобы не разгребать эту муть)
//=============================================================================
inline int dc_in__SMUC( unsigned port )
{
//  if (comp.flags & CF_DOSPORTS)
//  {
	if ((port & 0x18A3) == (0xFFFE & 0x18A3))
	{ 
	    //-----------------------------------------------------------------
	    // SMUC
	    if (conf.smuc)
	    {
		if ((port & 0xA044) == (0xDFBA & 0xA044)) return cmos_read();		// clock
		if ((port & 0xA044) == (0xFFBA & 0xA044)) return comp.nvram.out;	// SMUC system port
		if ((port & 0xA044) == (0x7FBA & 0xA044)) return comp.p7FBA | 0x3F;
		if ((port & 0xA044) == (0x5FBA & 0xA044)) return 0x3F;
		if ((port & 0xA044) == (0x5FBE & 0xA044)) return 0x57;
		if ((port & 0xA044) == (0x7FBE & 0xA044)) return 0x57;
	    }
	    //-----------------------------------------------------------------
	    if (conf.ide_scheme == IDE_SMUC)
	    {
		if ((port & 0x8044) == (0xFFBE & 0x8044))
		{ // FFBE, FEBE
		    //---------------------------------------------------------
		    if (comp.pFFBA & 0x80)
		    {
			if (!(port & 0x100))
			    return hdd.read(8);		// alternate status
			return 0xFF; 			// obsolete register
		    }
		    //---------------------------------------------------------
		    if (!(port & 0x2000))
			return comp.ide_read;
		    //---------------------------------------------------------
		    port >>= 8;
		    //goto read_hdd;
		    //read_hdd:
		    {
			port &= 7;
			if (port)
			    return hdd.read(port);
			//-----------------------------------------------------
			unsigned v = hdd.read_data();
			comp.ide_read = (unsigned char)(v >> 8);
			return (unsigned char) v;
			//-----------------------------------------------------
		    }
		    //---------------------------------------------------------
		}
	    }
	    //-----------------------------------------------------------------
      }
//  }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================







//=============================================================================
// SD 
//=============================================================================

//=============================================================================
// Z-Controller - Out
//=============================================================================
inline int dc_out__zc( unsigned port, unsigned char val )
{
    if (conf.zc)								
    {
	//-------------------------------------------------------------------------
	if ((port & 0xFF) == 0x57)
	{		
	    //port 0x57
	    Zc.Wr(port, val);
	    return 1;	//out end
	}
	//-------------------------------------------------------------------------
//	if ((port & 0xFF) == 0x77)	// [NS]
//	{				// тут должно быть наверно так?
//	    //port 0x77	 		// тк реализации этого порта нет 
//	    return 1;	//out end	// А блокировка записи в другие порты в IRL есть
//	}				// но из коробки это конфликтует? с АТМ2
					// 	уточнить в доке на оригинальный ZC !!!!
	//-------------------------------------------------------------------------
    }
    return 0;	// провверять другие порты	
}
//=============================================================================
// Z-Controller in ATM3 (ZX-Evo) - Out
//=============================================================================
inline int dc_out__zc__ATM3( unsigned port, unsigned char val )
{
    if (conf.zc)								
    {
	//-------------------------------------------------------------------------
	// Shadow mode
	if (comp.flags & CF_DOSPORTS)
	{
	    //port 0x57 - 0x77 mode		// в ZX-Evo при (a15 = 1) + shadow mode $57 выполняет роль порта $77
	    if ((port & 0x80FF) == 0x8057)	// (тк $77 занят в shadow mode ATM-а !!!1)
	    {					// который не эмулируетсо?
		return 1;	//out end			
	    }
	    //port 0x57 - 0x57 mode
	    if ((port & 0x80FF) == 0x0057)
	    {
		Zc.Wr(port, val);
		return 1;	//out end
	    }
	}
	//-------------------------------------------------------------------------
	// NO shadow mode
	else
	{
	    //---------------------------------------------------------------------
	    // отсылка байта в SD-карту по SPI
	    if ((port & 0xFF) == 0x57)
	    {		
		//port 0x57
		Zc.Wr(port, val);
		return 1;	//out end
	    }
	    //---------------------------------------------------------------------
	    // управление сигналом CS		// [NS] тут должно быть наверно так?
	    if ((port & 0xFF) == 0x77)		// тк реализации этого порта нет 
	    {					// а блокировку записи в другие порты таки никто не отменял 
		//port 0x77	 		// d7 = 0 - для совместимости
		return 1;	//out end	// d6 = 0 - для совместимости
						// d5 = 0 - для совместимости
						// d4 = 0 - для совместимости
						// d3 = 0 - для совместимости
						// d2 = 0 - для совместимости
						// d1 = X - сигнал CS	(1 - после сброса)
						//			(0 - для выбора SD карты)
						// d0 = 1 - для совместимости
	    }					//
	    //---------------------------------------------------------------------
	}
	//-------------------------------------------------------------------------
    }
    return 0;	// провверять другие порты	
}
//-----------------------------------------------------------------------------

// zxevo_base_configuration.pdf
// АХТУНХ: в цикле обмена по SPI, инициируемом записью или! чтением порта #xx57,
// происходит одновременно отсылка байта в SD-карту и приём байта от неё.
// если цикл обмена инициирован записью - Отсылаемый байт тот же, что записан в этот порт
// если цикл обмена инициирован чтением порта - Отсылаемый байт #FF !!!
// Принятый байт запоминается во внутреннем буфере и доступен для последующего чтения из этого же порта.
// Данное чтение вновь инициирует цикл обмена и т. д. ,
// Допускается читать/писать порт #xx57 командами INIR и OTIR.

//=============================================================================
// Z-Controller - In
//=============================================================================
inline int dc_in__zc( unsigned port )
{
    if (conf.zc)
    {
	// z-controller
//	if ( (port & 0xDF) == 0x57 )	//0.39.0
	if ( (port & 0xFF) == 0x57 )	//NEDOREPO	опять недописюканы сделали везде? как в банкоматах
	{
	    // no shadow-mode ZXEVO patch here since 0x57 port in read mode is the same
	    // as in noshadow-mode, i.e. no A15 is used to decode port.
	    return Zc.Rd(port);
	}
    }
    return -1;	// провверять другие порты	
}
//=============================================================================






//=============================================================================
// NET
//=============================================================================

//=============================================================================
// Wiznet - OUT
//=============================================================================
inline int dc_out__wiznet( unsigned port, unsigned char val )
{
    if (conf.wiznet)								
    {
	if ((port & 0xFF) == 0xAB)	//какаято недоприблуда
	{ 
	    pXXAB_Write(port,val); 
	    return 1;	//out end
	} 
    }
    return 0;	// провверять другие порты	
}
//=============================================================================
// Wiznet - IN
//=============================================================================
inline int dc_in__wiznet( unsigned port )
{
    if (conf.wiznet)
    {
	if ((port & 0xFF) == 0xAB)
	{ 
	    return pXXAB_Read(port); 
	} 
    }
    return -1;	// провверять другие порты	
}
//=============================================================================



//=============================================================================
// какой то Modem - OUT
//=============================================================================
inline int dc_out__Modem_HZ( unsigned port, unsigned char val )
{
    if (modem.open_port)
    {
	if ((port & 0xF8FF) == 0xF8EF)
	    modem.write((port >> 8) & 7, val);
	 return 1;	//out end
    }
    return 0;	// OUT - провверять другие порты	
}
//=============================================================================
// какой то Modem - IN
//=============================================================================
inline int dc_in__Modem_HZ( unsigned port)
{
    if (modem.open_port)
    {
	if ((port & 0xF8FF) == 0xF8EF)
	    return modem.read((port >> 8) & 7);
    }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================


//=============================================================================
// INPUT
//=============================================================================


//=============================================================================
// Kemston Mouse - IN
//=============================================================================
inline int dc_in__Kempston_Mouse( unsigned port )
{	
    if (conf.input.mouse == 1)	//Kempston Mouse
    {
	if (!(port & 0x20))		//NEDOREPO
	{ 
	    // kempstons
	    port = (port & 0xFFFF) | 0xFA00; // A13,A15 not used in decoding
	    // mouse
	    if ((port == 0xFADF || port == 0xFBDF || port == 0xFFDF))
	    { 
		input.mouse_joy_led |= 1;
		//-----------------------------------------------------
		if (port == 0xFBDF)
		    return input.kempston_mx();
		//-----------------------------------------------------
		if (port == 0xFFDF)
		    return input.kempston_my();
		//-----------------------------------------------------
		return input.mbuttons;
	    }
	}
    }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================


//=============================================================================
// Kemston Joystick - IN
//=============================================================================
inline int dc_in__Kempston_Joystick( unsigned port )
{
    if (!(port & 0x20))					//NEDOREPO
    { 
	//-------------------------------------------------------------
	input.mouse_joy_led |= 2;
	unsigned char res = (conf.input.kjoy) ?	input.kjoy :	//kempston on
							0xFF;	//kempston off
	return res;
	//-------------------------------------------------------------
    }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================
// Kemston Joystick Scorp - IN
//=============================================================================
inline int dc_in__Kempston_Joystick_Scorp( unsigned port )
{
    if (!(port & 0x20))					//NEDOREPO
    { 
	//-------------------------------------------------------------
	input.mouse_joy_led |= 2;
	unsigned char res = (conf.input.kjoy) ?	input.kjoy :
							0xFF;
	// if (conf.mem_model == MM_SCORP || conf.mem_model == MM_PROFSCORP)
	// в скорпе старшие биты кемпстона заюзали под какуюто хуйню для чтения ВГ-шки
	res = (res & 0x1F) | (comp.wd.in(0xFF) & 0xE0);
	return res;
	//-------------------------------------------------------------
    }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================


//=============================================================================
// Fuller Joystick - IN
//=============================================================================
inline int dc_in__Fuller_Joystick( u8 p1 )
{
    if (conf.input.fjoy)
    {
	// fuller joystick
	if (p1 == 0x7F)
	{
	    input.mouse_joy_led |= 2;
	    return input.fjoy;
	}
    }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================

//=============================================================================
// ATM 6.20 fake XT Keyboard - IN
//=============================================================================
inline int dc_in__atm620_xt_keyb( unsigned port )			// [NS]
{						
// вроде как в АТМ620 нет буфера для XT скан кодов
// есть только готовность/принятость байта
// и буфер для имитации zx клавы с заполнением по NMI
// да и еще САМИ XT КОДЫ ПЕРЕВЕРНУТЫ!

// тут же сделан такой же буфер как у ps/2 клавы
// тк так проще...
// ...хотя в случае прекращения опроса буфер заполнитсо
// а потом выплюнетсо когда не надо...
// хотя тут хз как работает клава
// мож там свой буфер и чтение по запросу? 



		//	//перевернуто (так не работает)
		//	return	(
		//		((atm620_xt_keyb_code & 0b00000001) << 7) |	//X... ....
		//		((atm620_xt_keyb_code & 0b00000010) << 5) |	//.X.. ....
		//		((atm620_xt_keyb_code & 0b00000100) << 3) |	//..X. ....
		//		((atm620_xt_keyb_code & 0b00001000) << 1) |	//...X ....
		//		((atm620_xt_keyb_code & 0b00010000) >> 1) |	//.... X...
		//		((atm620_xt_keyb_code & 0b00100000) >> 3) |	//.... .X..
		//		((atm620_xt_keyb_code & 0b01000000) >> 5) |	//.... ..X.
		//		((atm620_xt_keyb_code & 0b10000000) >> 7)	//.... ...X
		//		);
		
    if (conf.atm620_xt_keyb)
    {
	//---------------------------------------------------------------------
	if (port == 0x7DFD)
	{
	    //int return_val = input.atm620_xt_keyb_buffer.Pop();
	    //if (return_val) printf("%02X\n",return_val);
	    //return return_val;
	    return input.atm620_xt_keyb_buffer.Pop();
	}
	//---------------------------------------------------------------------
    }	   
    return -1;	// IN - провверять другие порты	
}
//=============================================================================



//=============================================================================
// MISC
//=============================================================================


//=============================================================================
// Port FF - IN
//=============================================================================
inline int dc_in__Port_FF( unsigned port )
{
    if (conf.portff)
    { 
	if (((port & 0xFF) == 0xFF))
	{
	    update_screen();
	    //-----------------------------------------------------------------
	    if (vmode != 2)
		return 0xFF;	// ray is not in paper
	    //-----------------------------------------------------------------
	    unsigned ula_t = (cpu.t+temp.border_add) & temp.border_and;
	    return temp.base[vcurr->atr_offs + (ula_t - vcurr[-1].next_t) / 4];
	}
    }
    return -1;	// IN - провверять другие порты	
}
//=============================================================================















// заготовка
//
//	//список в emul.h
//	switch (conf.mem_model)
//	{
//	//---------------------------------------------------------------------
//	case MM_PENTAGON:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_SCORP:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PROFSCORP:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PROFI:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM450:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM710:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM3:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_KAY:
//	    break;
//	//-----------------------------------------------------------------
//	case MM_PLUS3:
//	    break;
//	//-----------------------------------------------------------------
//	case MM_QUORUM:
//	    break; 
//	//---------------------------------------------------------------------
//	}






















//=============================================================================
void out(unsigned port, unsigned char val)
{
   port &= 0xFFFF;
   u8 p1 = (port & 0xFF);
   brk_port_out = port;
   brk_port_val = val;


//-----------------------------------------------------------------------------
// В начале дешифрация портов по полным 8бит
//-----------------------------------------------------------------------------


//  dc_* значит что inline функция
// сама определяет включенность и декодирует по Port/P1


// ущербность сяпаскалей категорически не дает сделать нам компактно без лишней хуиты
// НО тк для разных клоУнов нужна еще и разная последовательность подключения портов
// и без сотен if *** && (!ATM3 == Ы) || (ATM2 == 0) по всему коду наделанных всякими там алониями
// то делаем хоть как нибудь...

//-----------------------------------------------------------------------------
// NO SHADOW + SHADOW
//-----------------------------------------------------------------------------
		if ( dc_out__ZXI_TSFM_Mirror( port, val )) return;	// ZXI TSFM
//-----------------------------------------------------------------------------
		if ( dc_out__Ula_Plus( port, val )) return;		// ULA Plus
//-----------------------------------------------------------------------------
		if ( dc_out__NGS( port, p1, val )) return;		// NGS
//-----------------------------------------------------------------------------
 if (conf.mem_model == MM_ATM3)						// Z-Controller
		if ( dc_out__zc__ATM3( port, val )) return;	// в ATM3 недоступен $77 в Shadow mode
 else
		if ( dc_out__zc( port, val )) return;		// как в других реализациях ХЗ !!!!!	
//-----------------------------------------------------------------------------
		if ( dc_out__wiznet( port, val )) return;		// Wiznet
//-----------------------------------------------------------------------------
		if ( dc_out__NEMO_DivIDE( port, val )) return;		// DivIDE на NEMO портах
//-----------------------------------------------------------------------------



   
   
   
   
   
   
   
    //список в emul.h
    switch (conf.mem_model)
    {
//	//---------------------------------------------------------------------
//	case MM_PENTAGON:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_SCORP:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PROFSCORP:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PROFI:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM450:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM710:
//	    break;

	//---------------------------------------------------------------------
	case MM_ATM3:
	
//      // Порт расширений АТМ3		0.39.0
//      if((port & 0xFF) == 0xBF)
//      {
//          if((comp.pBF ^ val) & comp.pBF & 8) // D3: 1->0
//              nmi_pending  = 1;
//          comp.pBF = val;
//          set_banks();
//          return;

//       // Порт разблокировки RAM0 АТМ3
//       if((port & 0xFF) == 0xBE)
//       {
//           comp.pBE = 2; // счетчик для выхода из nmi
//           return;
//       }
//   }
	   
	switch (port & 0xFF)
	{
	    //-----------------------------------------------------------------
	    // Порт расширений пентевы
	    case 0xBF:	
		//d3 - (Перевод из 1 в 0) - force NMI 
		if ((comp.pBF ^ val) & comp.pBF & 8)	// D3: 1->0
		{
		    nmi_pending = 1;
		    trdos_in_nmi = (comp.flags & CF_TRDOS);
		}
		comp.pBF = val;
		set_banks();
		return;
	    //-----------------------------------------------------------------
	    // Порт разблокировки RAM0 АТМ3
	    case 0xBE:	
		if (cpu.nmi_in_progress && (cpu.nmi_in_progress == conf.trdos_IORam) )
		{
		    if (trdos_in_nmi)
			comp.flags |= (CF_SETDOSROM | CF_TRDOS);
		    cpu.nmi_in_progress = false;
		    set_banks();
		    return;
		}
		comp.pBE = 2; // счетчик для выхода из nmi
		return;
	    //-----------------------------------------------------------------
	    // порт адреса брякпоинта и маски перехвата портов FDD
	    case 0xBD:	
		switch(port & 0xEFFF)
		{
		//-------------------------------------------------------------
		// hardware break LOW
		case 0x00BD:
		    comp.brk_addr &= 0xFF00;
		    comp.brk_addr |= ((u16)val) & 0x00FF;
		    break;
		//-------------------------------------------------------------
		// hardware break HIGH
		case 0x01BD:
		    comp.brk_addr &= 0x00FF;
		    comp.brk_addr |= ( ((u16)val) << 8 ) & 0xFF00;
		    break;
		//-------------------------------------------------------------
		case 0x03BD:
		    comp.fddIO2Ram_mask = val;
		    break;	
		//-------------------------------------------------------------
		}
		return;
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------

	    break; 
	//---------------------------------------------------------------------
//	case MM_KAY:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PLUS3:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_QUORUM:
//	    break;
//	//---------------------------------------------------------------------
    }
    
   
   





//-----------------------------------------------------------------------------
// SHADOW PORTS		местами NO RETURN !!!!???
//-----------------------------------------------------------------------------
   if (comp.flags & CF_DOSPORTS)
   {
   
	//список в emul.h
	switch (conf.mem_model)
	{
//	    //-----------------------------------------------------------------
//	    case MM_PENTAGON:
//		break;
//	    //-----------------------------------------------------------------
//	    case MM_SCORP:
//		break;
//	    //-----------------------------------------------------------------
//	    case MM_PROFSCORP:
//		break;
	    //=================================================================
	    case MM_PROFI:
		//-------------------------------------------------------------
		if ((comp.p7FFD & 0x10) && (comp.pDFFD & 0x20)) // modified ports
		{
		    //---------------------------------------------------------
		    // BDI ports
		    if ((p1 & 0x9F) == 0x83)	// WD93 ports
		    {
			comp.wd.out((p1 & 0x60) | 0x1F, val);
			return;
		    }
		    //---------------------------------------------------------
		    if ((p1 & 0xE3) == 0x23)	// port FF
		    {
			comp.wd.out(0xFF, val);
			return;
		    }
		    //---------------------------------------------------------
		    // RTC
		    if ( ((port & 0x9F) == 0x9F) && conf.cmos)
		    {
			if (port & 0x20)
			{
			    comp.cmos_addr = val;
			    return;
			}
			cmos_write(val);
			return;
		    }
		    //---------------------------------------------------------
		    // IDE_PROFI 
		    //		Profi IDE был тут
		    //		но вынесен в общие порты dc_out__Profi_IDE
		    //		для возможности создания суперспецтрумов

		    //---------------------------------------------------------
		}
		else
		{
		    //---------------------------------------------------------
		    // BDI ports
		    if ((p1 & 0x83) == 0x03)	// WD93 ports 1F, 3F, 5F, 7F
		    {
			comp.wd.out((p1 & 0x60) | 0x1F,val);
			return;
		    }
		    //---------------------------------------------------------
		    if ((p1 & 0xE3) == ((comp.pDFFD & 0x20) ? 0xA3 : 0xE3)) // port FF
		    {
			comp.wd.out(0xFF,val);
			return;
		    }
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		break;
	    //-----------------------------------------------------------------
//	    case MM_ATM450:
//		break;
	    //=================================================================
	    case MM_ATM710: //SHADOW
	    {
		//-------------------------------------------------------------
		if (p1 == 0x77) // xx77
		{
		    set_atm_FF77(port, val);
		    return;
		}
		//-------------------------------------------------------------
		//0.39.0
		// u32 mask = (conf.mem_model == MM_ATM3) ? 0x3FFF : 0x00FF;
		//NEDOREPO
		// lvd fix: pentevo hardware decodes fully only low byte,
		u32 mask = (conf.mem_model == MM_ATM3) ? /*0x3FFF*/ 0x0FFF : 0x00FF; 
		// so using eff7 in shadow mode lead to outting to fff7,
		// unlike this was in unreal!
		//-------------------------------------------------------------
		if ((port & mask) == (0x3FF7 & mask)) // xff7
		{
		    comp.pFFF7[((comp.p7FFD & 0x10) >> 2) | ((port >> 14) & 3)] = unsigned(((val & 0xC0) << 2) | (val & 0x3F)) ^ 0x33FU;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
		if ((p1 & 0x9F) == 0x9F && !(comp.aFF77 & 0x4000))
		{
		//  atm_writepal(val);		// don't return - write to TR-DOS system port
		    atm_writepal(port, val);	// don't return - write to TR-DOS system port	//NEDOREPO
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		break;
	    }
	    //=================================================================
	    case MM_ATM3: //SHADOW
	    {
	        //-------------------------------------------------------------
		// NO OUT RETURN ???????
	    //	if ( ((p1 & 0x1F) == 0x0F) &&  (((p1 >> 5) - 1) < 5) )	//0.39.0
		if ( ((p1 & 0x1F) == 0x0F) && !(((p1 >> 5) - 1) & 4) )	//NEDOREPO
		{
		    // 2F = 001|01111b
		    // 4F = 010|01111b
		    // 6F = 011|01111b
		    // 8F = 100|01111b
		    //  		AF = 101|01111b	//AF УБРАНО В NEDOREPO !!!
		    //			нет в zxevo_base_configuration.pdf 2015 года
		    comp.wd_shadow[(p1 >> 5) - 1] = val;
		// NO OUT RETURN ???????
		}
		//-------------------------------------------------------------
		if ((port & 0x3FFF) == 0x37F7) 	// x7f7 ATM3 4Mb memory manager
		{
		    unsigned idx = ((comp.p7FFD & 0x10) >> 2) | ((port >> 14) & 3);
		    comp.pFFF7[idx] = (comp.pFFF7[idx] & ~0x1FFU) | (val ^ 0xFF); // always ram
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
		if (p1 == 0x77) // xx77		// такой же в MM_ATM710
		{
		    set_atm_FF77(port, val);
		    return;
		}
		//-------------------------------------------------------------
		//0.39.0
		// u32 mask = (conf.mem_model == MM_ATM3) ? 0x3FFF : 0x00FF;
		//NEDOREPO
		// lvd fix: pentevo hardware decodes fully only low byte, [NS] может low nibble?
		u32 mask = (conf.mem_model == MM_ATM3) ? /*0x3FFF*/ 0x0FFF : 0x00FF; 
		// so using eff7 in shadow mode lead to outting to fff7,
		// unlike this was in unreal!
		//-------------------------------------------------------------
		if ((port & mask) == (0x3FF7 & mask)) // xff7		// такой же в MM_ATM710
		{
		    comp.pFFF7[((comp.p7FFD & 0x10) >> 2) | ((port >> 14) & 3)] = unsigned(((val & 0xC0) << 2) | (val & 0x3F)) ^ 0x33FU;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
		if ((p1 & 0x9F) == 0x9F && !(comp.aFF77 & 0x4000))	// такой же в MM_ATM710
		{
		//  atm_writepal(val);		// don't return - write to TR-DOS system port
		    atm_writepal(port, val);	// don't return - write to TR-DOS system port	//NEDOREPO
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		break; 
	    }
//	    //=================================================================
//	    case MM_KAY:
//		break;
//	    //-----------------------------------------------------------------
//	    case MM_PLUS3:
//		break;
	    //-----------------------------------------------------------------
	    case MM_QUORUM:

		// if(conf.mem_model == MM_QUORUM /* && !(comp.p00 & Q_TR_DOS)*/) // cpm ports
		// какой дурак убрал quorum trdos? поебалуему!
		// хоть один недописюкан можЫт подумать на перед про дальнейшую расширяемость*
		
		//-------------------------------------------------------------
		if ((p1 & 0xFC) == 0x80) // 80, 81, 82, 83
		{
		    p1 = u8(((p1 & 3) << 5) | 0x1F);
		    comp.wd.out(p1, val);
		    return;
		}
		//-------------------------------------------------------------
		if (p1 == 0x85) // 85
		{
		//	 01 -> 00 A
		//	 10 -> 01 B
		//	 00 -> 11 D (unused)
		//	 11 -> 11 D (unused)
		    static const u8 drv_decode[] = { 3, 0, 1, 3 };
		    u8 drv = drv_decode[val & 3];
		    comp.wd.out(0xFF, ((val & ~3) ^ 0x10) | 0xCC | 8 | drv);
		    return;
		}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		break;
	}
	//---------------------------------------------------------------------
	// общие shadow порты
	//---------------------------------------------------------------------
		if ( dc_out__ATM_IDE( port, val )) return;		//ATM IDE
	//---------------------------------------------------------------------
		if ( dc_out__SMUC( port, val )) return;			//SMUC
	//---------------------------------------------------------------------
		if ( dc_out__Profi_IDE( port, p1, val )) return; 	// Profi IDE (вынесен из профи)
	//---------------------------------------------------------------------
	
	// нужно выкинуть всю АТМщину и раскидать по машинам
	//	точнее закинуть атмщину в АТМ-ы
	if (conf.mem_model != MM_QUORUM)
	{
	    // NOT IN QUORUM !!!
	    if ((p1 & 0x1F) == 0x1F) // 1F, 3F, 5F, 7F, FF
	    {
		//0.39.0
		//comp.wd.out(p1, val);	//у десусофта тут была 1 строка
		//return;
	    
		// АТМщина?
		// потом окажотсо что и тырдырдос отвалилсо в других клонах...
		// с таким недоподходом
		//-------------------------------------------------------------
		if (p1 & 0x80) 
		{
		    comp.trdos_last_ff = val & 0x1f;
		}
		//-------------------------------------------------------------
		if (		(comp.flags & CF_TRDOS)	    &&
				conf.trdos_IORam	    &&
				(bankr[0] == base_dos_rom)  &&
				(p1 & 0x80)
		  )
		{
		    comp.wd.out(p1, val);
		    
		    if ((1<<comp.wd.drive)&comp.fddIO2Ram_mask)
		    {
			trdos_in_nmi = (comp.flags & CF_TRDOS);
			cpu.nmi_in_progress=conf.trdos_IORam;
			set_banks();
		    }
		}
		//-------------------------------------------------------------
		else if (	(comp.flags & CF_TRDOS)			    &&
				conf.trdos_IORam			    &&
				(bankr[0] == base_dos_rom)		    &&
				((1<<comp.wd.drive) & comp.fddIO2Ram_mask)    
				
		    )
		{
		trdos_in_nmi = comp.flags&CF_TRDOS;
		    cpu.nmi_in_progress=conf.trdos_IORam;
		    set_banks();
		}
		//-------------------------------------------------------------
		else
		{
		    comp.wd.out(p1, val);
		}
		//-------------------------------------------------------------
		return;  
	    }	
	}
	//---------------------------------------------------------------------

	// оригинальный комент р_О
	// don't return - out to port #FE works in trdos!
    }
    else
//-----------------------------------------------------------------------------
// NOT shadow ports		
//-----------------------------------------------------------------------------   
    {
	//---------------------------------------------------------------------
		if ( dc_out__Fuller_AY( p1, val )) return;		// Fuller AY
	//---------------------------------------------------------------------
		if ( dc_out__DivIDE( port, val )) return;		// DivIDE
	//---------------------------------------------------------------------
		if ( dc_out__POS_AY( port, val )) return;		// POS AY
	//---------------------------------------------------------------------
		if ( dc_out__NEMO_IDE( port, val )) return;		// NEMO IDE
	//---------------------------------------------------------------------
    }
//-----------------------------------------------------------------------------





//-----------------------------------------------------------------------------
// ЕЩЕ КАКИЕТО NO shadow Порты
//-----------------------------------------------------------------------------




    //список в emul.h
    switch (conf.mem_model)
    {
//	//---------------------------------------------------------------------
//	case MM_PENTAGON:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_SCORP:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PROFSCORP: 
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PROFI:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM450:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM710:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM3:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_KAY:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PLUS3:
//	    break;
	//---------------------------------------------------------------------
	case MM_QUORUM:
	    //-----------------------------------------------------------------
	    if ((port & 0xFF) == 0x00)
	    {
		comp.p00 = val;
		set_banks();
		return;
	    }
	    //-----------------------------------------------------------------
	    break; 
	//---------------------------------------------------------------------
	    
    }
    
//-----------------------------------------------------------------------------
// ЕЩЕ КАКИЕТО NO shadow ОБЩИЕ Порты
//-----------------------------------------------------------------------------
 
    
//-----------------------------------------------------------------------------
// Video Drive by SAM style
 #ifdef MOD_VID_VD
    if ((unsigned char)port == 0xDF)
    {
	comp.pVD = val;
	comp.vdbase = (comp.pVD & 4) ?	vdmem[comp.pVD & 3] :
					0;
	return;
    }
 #endif
//-----------------------------------------------------------------------------
    
    
//-----------------------------------------------------------------------------
// PORT FE - OUT
//-----------------------------------------------------------------------------
 
    // port #FE
    bool pFE;
 
    //список в emul.h
    switch (conf.mem_model)
    {
//	//---------------------------------------------------------------------
//	case MM_PENTAGON:
//	    break;
	//=====================================================================
	case MM_SCORP:
	    // scorp  xx1xxx10 /dos=1 (sc16 green)
	    // if ((conf.mem_model == MM_SCORP || conf.mem_model == MM_PROFSCORP))
	    pFE = ((port & 0x23) == (0xFE & 0x23)) && !(comp.flags & CF_DOSPORTS);
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
	    //[vv]      assert(!(val & 0x08));
	        //-------------------------------------------------------------
		spkr_dig = (val & 0x10) ? conf.sound.beeper_vol : 0;
		mic_dig = (val & 0x08) ? conf.sound.micout_vol : 0;
		//-------------------------------------------------------------
		// speaker & mic
		if ((comp.pFE ^ val) & 0x18)
		{
		//  __debugbreak();
		    flush_dig_snd();
		}
		//-------------------------------------------------------------
		unsigned char new_border = (val & 7);
		if (comp.border_attr ^ new_border)
		    update_screen();
		comp.border_attr = new_border;
		//-------------------------------------------------------------
		comp.pFE = val;
		// do not return! intro to navy seals (by rst7) uses out #FC for to both FE and 7FFD
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break;
	//=====================================================================
	case MM_PROFSCORP:
	    // scorp  xx1xxx10 /dos=1 (sc16 green)
	    // if ((conf.mem_model == MM_SCORP || conf.mem_model == MM_PROFSCORP))
	    pFE = ((port & 0x23) == (0xFE & 0x23)) && !(comp.flags & CF_DOSPORTS);
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
	    //[vv]      assert(!(val & 0x08));
		//-------------------------------------------------------------
		spkr_dig = (val & 0x10) ? conf.sound.beeper_vol : 0;
		mic_dig = (val & 0x08) ? conf.sound.micout_vol : 0;
		//-------------------------------------------------------------
		// speaker & mic
		if ((comp.pFE ^ val) & 0x18)
		{
		//  __debugbreak();
		    flush_dig_snd();
		}
		//-------------------------------------------------------------
		unsigned char new_border = (val & 7);
		if (comp.border_attr ^ new_border)
		    update_screen();
		comp.border_attr = new_border;
		//-------------------------------------------------------------
		comp.pFE = val;
		// do not return! intro to navy seals (by rst7) uses out #FC for to both FE and 7FFD
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break;
	//=====================================================================
	case MM_PROFI:
	    //-----------------------------------------------------------------
	    pFE = !(port & 1);	//DEFAULT FE
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
	    //[vv]      assert(!(val & 0x08));
		//-------------------------------------------------------------
		spkr_dig = (val & 0x10) ? conf.sound.beeper_vol : 0;
		mic_dig = (val & 0x08) ? conf.sound.micout_vol : 0;
		//-------------------------------------------------------------
		// speaker & mic
		if ((comp.pFE ^ val) & 0x18)
		{
		//  __debugbreak();
		    flush_dig_snd();
		}
		//-------------------------------------------------------------
		unsigned char new_border = (val & 7);
		if (comp.border_attr ^ new_border)
		    update_screen();
		comp.border_attr = new_border;
		//-------------------------------------------------------------
		if (!(port & 0x80) && (comp.pDFFD & 0x80))
		{
		    profi_writepal(u8(~(port >> 8)));
		}
		//-------------------------------------------------------------
		comp.pFE = val;
		// do not return! intro to navy seals (by rst7) uses out #FC for to both FE and 7FFD
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break;
	//=====================================================================
	case MM_ATM450:
	    //-----------------------------------------------------------------
	    pFE = !(port & 1);	//DEFAULT FE
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
		//[vv]      assert(!(val & 0x08));
		//-------------------------------------------------------------
		spkr_dig = (val & 0x10) ? conf.sound.beeper_vol : 0;
		mic_dig = (val & 0x08) ? conf.sound.micout_vol : 0;
		//-------------------------------------------------------------
		// speaker & mic
		if ((comp.pFE ^ val) & 0x18)
		{
		// __debugbreak();
		    flush_dig_snd();
		}
		//-------------------------------------------------------------
		unsigned char new_border = (val & 7);
		//if (conf.mem_model == MM_ATM710 || conf.mem_model == MM_ATM3 || conf.mem_model == MM_ATM450)
		new_border |= ((port & 8) ^ 8); // port F6, bright border
		if (comp.border_attr ^ new_border)
		    update_screen();
		comp.border_attr = new_border;
		//-------------------------------------------------------------
		//if (conf.mem_model == MM_ATM450)
		set_atm_aFE((unsigned char)port);
		//-------------------------------------------------------------
		comp.pFE = val;
		// do not return! intro to navy seals (by rst7) uses out #FC for to both FE and 7FFD
	    }
	    //-----------------------------------------------------------------
	    break;
	//=====================================================================
	case MM_ATM710:
	    //-----------------------------------------------------------------
	    pFE = !(port & 1);	//DEFAULT FE
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
	    //[vv]      assert(!(val & 0x08));
		//-------------------------------------------------------------
		spkr_dig = (val & 0x10) ? conf.sound.beeper_vol : 0;
		mic_dig = (val & 0x08) ? conf.sound.micout_vol : 0;
		//-------------------------------------------------------------
		// speaker & mic
		if ((comp.pFE ^ val) & 0x18)
		{
		//  __debugbreak();
		    flush_dig_snd();
		}
		//-------------------------------------------------------------
		unsigned char new_border = (val & 7);
		//if (conf.mem_model == MM_ATM710 || conf.mem_model == MM_ATM3 || conf.mem_model == MM_ATM450)
		new_border |= ((port & 8) ^ 8); // port F6, bright border
		if (comp.border_attr ^ new_border)
		    update_screen();
		comp.border_attr = new_border;
		//-------------------------------------------------------------
		comp.pFE = val;
		// do not return! intro to navy seals (by rst7) uses out #FC for to both FE and 7FFD
	    }
	    //-----------------------------------------------------------------
	    break;
	//=====================================================================
	case MM_ATM3:
	    //-----------------------------------------------------------------
	    pFE = !(port & 1);	//DEFAULT FE
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
	    //[vv]      assert(!(val & 0x08));
		//-------------------------------------------------------------
		spkr_dig = (val & 0x10) ? conf.sound.beeper_vol : 0;
		mic_dig = (val & 0x08) ? conf.sound.micout_vol : 0;
		//-------------------------------------------------------------
		// speaker & mic
		if ((comp.pFE ^ val) & 0x18)
		{
		//  __debugbreak();
		    flush_dig_snd();
		}
		//-------------------------------------------------------------
		unsigned char new_border = (val & 7);
		//if (conf.mem_model == MM_ATM710 || conf.mem_model == MM_ATM3 || conf.mem_model == MM_ATM450)
		new_border |= ((port & 8) ^ 8); // port F6, bright border
		if (comp.border_attr ^ new_border)
		    update_screen();
		comp.border_attr = new_border;
		//-------------------------------------------------------------
		comp.pFE = val;
		// do not return! intro to navy seals (by rst7) uses out #FC for to both FE and 7FFD
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break;
//	//=====================================================================
//	case MM_KAY:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PLUS3:
//	    break;
//	//=====================================================================
	case MM_QUORUM:
	    // if (conf.mem_model == MM_QUORUM) // 1xx11xx0
	    pFE = ((port & 0x99) == (0xFE & 0x99));
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
	    //[vv]      assert(!(val & 0x08));
		//-------------------------------------------------------------
		spkr_dig = (val & 0x10) ? conf.sound.beeper_vol : 0;
		mic_dig = (val & 0x08) ? conf.sound.micout_vol : 0;
		//-------------------------------------------------------------
		// speaker & mic
		if ((comp.pFE ^ val) & 0x18)
		{
		//  __debugbreak();
		    flush_dig_snd();
		}
		//-------------------------------------------------------------
		unsigned char new_border = (val & 7);
		if (comp.border_attr ^ new_border)
		    update_screen();
		comp.border_attr = new_border;
		//-------------------------------------------------------------
		comp.pFE = val;
		// do not return! intro to navy seals (by rst7) uses out #FC for to both FE and 7FFD
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break; 
	//=====================================================================
	default:
	    // else // others xxxxxxx0		//ДОБАВЛЯТТЬ ВЫШЕ ПРИ РАСКОМЕНТЕНЬИ МАШИН
	    pFE = !(port & 1);
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
	    //[vv]      assert(!(val & 0x08));
		//-------------------------------------------------------------
		spkr_dig = (val & 0x10) ? conf.sound.beeper_vol : 0;
		mic_dig = (val & 0x08) ? conf.sound.micout_vol : 0;
		//-------------------------------------------------------------
		// speaker & mic
		if ((comp.pFE ^ val) & 0x18)
		{
		//  __debugbreak();
		    flush_dig_snd();
		}
		//-------------------------------------------------------------
		unsigned char new_border = (val & 7);
		if (comp.border_attr ^ new_border)
		    update_screen();
		comp.border_attr = new_border;
		//-------------------------------------------------------------
		comp.pFE = val;
		// do not return! intro to navy seals (by rst7) uses out #FC for to both FE and 7FFD
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    // break; 
	//=====================================================================
    } //FE
    
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// PORT xD
//-----------------------------------------------------------------------------
 

// вынести звук из каждой машины !!!

    //список в emul.h
    switch (conf.mem_model)
    {
	//=====================================================================
	case MM_PENTAGON:
	    // #xD
	    if (!(port & 2))
	    {
		//-------------------------------------------------------------
			if ( dc_out__Covox_DD( port, val )) return;		// Covox_DD
		//-------------------------------------------------------------
		if (!(port & 0x8000)) // zx128 port
		{
		// 0001xxxxxxxxxx0x (bcig4) // 1FFD
		// 0010xxxxxxxxxx0x (bcig4) // 2FFD
		// 0011xxxxxxxxxx0x (bcig4) // 3FFD
		    //---------------------------------------------------------
		    // 7FFD
		    if (comp.p7FFD & 0x20)
		    { // 48k lock
			//-----------------------------------------------------
			// #EFF7.2 forces lock
			if (	(comp.pEFF7 & EFF7_LOCKMEM) &&	//&& conf.mem_model == MM_PENTAGON
				(conf.ramsize == 1024)
			    )
			    return;
			//-----------------------------------------------------
			//0.39.0
			// if not pentagon-1024 or profi with #DFFD.4 set, apply lock
			//NEDOREPO
			// if not pentagon-1024 or pentevo (atm3) --(added by lvd)-- or profi with #DFFD.4 set, apply lock

			//0.39.0
			//    if (!((conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
			//         (conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10)))) 	 molodcov_alex
			if (!(	(conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
				(conf.mem_model == MM_ATM3)				||
				(conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10))
			    ))								// molodcov_alex
			    return;
		    }
		    //---------------------------------------------------------
		    if ((comp.p7FFD ^ val) & 0x08)
			update_screen();
		    //---------------------------------------------------------
		    comp.p7FFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
			if ( dc_out__AY_TS_TSFM_nedoSAA_etc( port, val )) return;	// AY_TS_TSFM_nedoSAA_etc
		//-------------------------------------------------------------
	
		// а зачем выход?
		// никуда ж не записали о_О
		return;
	    }
	    break;
	//=====================================================================
	case MM_SCORP:
	    // #xD
	    if (!(port & 2))
	    {
		//-------------------------------------------------------------
			if ( dc_out__Covox_DD( port, val )) return;		// Covox_DD
		//-------------------------------------------------------------
		if (!(port & 0x8000)) // zx128 port
		{
		// 0001xxxxxxxxxx0x (bcig4) // 1FFD
		// 0010xxxxxxxxxx0x (bcig4) // 2FFD
		// 0011xxxxxxxxxx0x (bcig4) // 3FFD
		    //-----------------------------------------------------
		    // 00xxxxxxxx1xxx01 (sc16 green)
		    if ((port & 0xC023) == (0x1FFD & 0xC023))	//&& (conf.mem_model == MM_SCORP, MM_PROFSCORP
		    {
			//set1FFD:
			comp.p1FFD = val;
			set_banks();
			return;
		    }
		    //---------------------------------------------------------
		    // 01xxxxxxxx1xxx01 (sc16 green)
		    if ((port & 0xC023) != (0x7FFD & 0xC023))	//&& (conf.mem_model == MM_SCORP, MM_PROFSCORP
			return;
		    //---------------------------------------------------------
		    // 7FFD
		    if (comp.p7FFD & 0x20)
		    { // 48k lock
			//0.39.0
			// if not pentagon-1024 or profi with #DFFD.4 set, apply lock
			//NEDOREPO
			// if not pentagon-1024 or pentevo (atm3) --(added by lvd)-- or profi with #DFFD.4 set, apply lock
			//0.39.0
			//    if (!((conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
			//         (conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10)))) 	 molodcov_alex
			if (!(	(conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
				(conf.mem_model == MM_ATM3)				||
				(conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10))
			    ))								// molodcov_alex
			    return;
		    }
		    //---------------------------------------------------------
		    if ((comp.p7FFD ^ val) & 0x08)
			update_screen();
		    //---------------------------------------------------------
		    comp.p7FFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
			if ( dc_out__AY_TS_TSFM_nedoSAA_etc( port, val )) return;	// AY_TS_TSFM_nedoSAA_etc
		//-------------------------------------------------------------
	
		// а зачем выход?
		// никуда ж не записали о_О	
		return;
	    }
	    break;
	//=====================================================================
	case MM_PROFSCORP:
	    // #xD
	    if (!(port & 2))
	    {
		//-------------------------------------------------------------
			if ( dc_out__Covox_DD( port, val )) return;		// Covox_DD
		//-------------------------------------------------------------
		if (!(port & 0x8000)) // zx128 port
		{
		// 0001xxxxxxxxxx0x (bcig4) // 1FFD
		// 0010xxxxxxxxxx0x (bcig4) // 2FFD
		// 0011xxxxxxxxxx0x (bcig4) // 3FFD
		    //---------------------------------------------------------
		    // 00xxxxxxxx1xxx01 (sc16 green)
		    if ((port & 0xC023) == (0x1FFD & 0xC023))	//&& (conf.mem_model == MM_SCORP, MM_PROFSCORP
		    {
			//set1FFD:
			comp.p1FFD = val;
			set_banks();
			return;
		    }
		    //---------------------------------------------------------
		    // gmx
		    if (port == 0x7EFD)	// && conf.mem_model == MM_PROFSCORP
		    {
			comp.p7EFD = val;
			set_banks();
			return;
		    }
		    //---------------------------------------------------------
		    // 01xxxxxxxx1xxx01 (sc16 green)
		    if ((port & 0xC023) != (0x7FFD & 0xC023))	// && (conf.mem_model == MM_SCORP, MM_PROFSCORP
			return;
		    //---------------------------------------------------------
		    // 7FFD
		    if (comp.p7FFD & 0x20)
		    { // 48k lock
			//0.39.0
			// if not pentagon-1024 or profi with #DFFD.4 set, apply lock
			//NEDOREPO
			// if not pentagon-1024 or pentevo (atm3) --(added by lvd)-- or profi with #DFFD.4 set, apply lock
			//0.39.0
			//    if (!((conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
			//         (conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10)))) 	 molodcov_alex
			if (!(	(conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
				(conf.mem_model == MM_ATM3)				||
				(conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10))
			    ))								// molodcov_alex
			    return;
		    }
		    //---------------------------------------------------------
		    if ((comp.p7FFD ^ val) & 0x08)
			update_screen();
		    //---------------------------------------------------------
		    comp.p7FFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
			if ( dc_out__AY_TS_TSFM_nedoSAA_etc( port, val )) return;	// AY_TS_TSFM_nedoSAA_etc
		//-------------------------------------------------------------

		// а зачем выход?
		// никуда ж не записали о_О
		return;
	    }
	    break;
	//=====================================================================
	case MM_PROFI:
	    // #xD
	    if (!(port & 2))
	    {
		//-------------------------------------------------------------
			if ( dc_out__Covox_DD( port, val )) return;		// Covox_DD
		//-------------------------------------------------------------
		if (!(port & 0x8000)) // zx128 port
		{
		// 0001xxxxxxxxxx0x (bcig4) // 1FFD
		// 0010xxxxxxxxxx0x (bcig4) // 2FFD
		// 0011xxxxxxxxxx0x (bcig4) // 3FFD
		    //---------------------------------------------------------
		    // 7FFD
		    if (comp.p7FFD & 0x20)
		    { // 48k lock
	
			//  0.39.0
			// if not pentagon-1024 or profi with #DFFD.4 set, apply lock
			//  NEDOREPO
			// if not pentagon-1024 or pentevo (atm3) --(added by lvd)-- or profi with #DFFD.4 set, apply lock
			//0.39.0
			//    if (!((conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
			//         (conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10)))) 	 molodcov_alex
			if (!(	(conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
				(conf.mem_model == MM_ATM3)				||
				(conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10))
			    ))								// molodcov_alex
			    return;
		    }
		    //---------------------------------------------------------
		    if ((comp.p7FFD ^ val) & 0x08)
			update_screen();
		    //---------------------------------------------------------
		    comp.p7FFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
		// xx0xxxxxxxxxxx0x (3.2) [vv]
		if ((port & 0x2002) == (0xDFFD & 0x2002))	// && conf.mem_model == MM_PROFI)
		{
		    comp.pDFFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
			if ( dc_out__AY_TS_TSFM_nedoSAA_etc( port, val )) return;	// AY_TS_TSFM_nedoSAA_etc
		//-------------------------------------------------------------
		// а зачем выход?
		// никуда ж не записали о_О
		return;
	    }
	    break;
	//=====================================================================
	case MM_ATM450:
	    // #xD
	    if (!(port & 2))
	    {
		//-------------------------------------------------------------
			if ( dc_out__Covox_DD( port, val )) return;		// Covox_DD
		//-------------------------------------------------------------
		if (!(port & 0x8000)) // zx128 port
		{
		// 0001xxxxxxxxxx0x (bcig4) // 1FFD
		// 0010xxxxxxxxxx0x (bcig4) // 2FFD
		// 0011xxxxxxxxxx0x (bcig4) // 3FFD
		    //---------------------------------------------------------
		    if ((port & 0x8202) == (0x7DFD & 0x8202))	//&& conf.mem_model == MM_ATM450 
		    {
			//atm_writepal(val);
			atm_writepal(port, val);	//NEDOREPO
			return;
		    }
		    //---------------------------------------------------------
		    // 7FFD
		    if (comp.p7FFD & 0x20)
		    { // 48k lock
			//  0.39.0
			// if not pentagon-1024 or profi with #DFFD.4 set, apply lock
			//  NEDOREPO
			// if not pentagon-1024 or pentevo (atm3) --(added by lvd)-- or profi with #DFFD.4 set, apply lock
			//0.39.0
			//    if (!((conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
			//         (conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10)))) 	 molodcov_alex
			if (!(	(conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
				(conf.mem_model == MM_ATM3)				||
				(conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10))
			    ))								// molodcov_alex
			    return;
		    }
		    //---------------------------------------------------------
		    if ((comp.p7FFD ^ val) & 0x08)
			update_screen();
		    //---------------------------------------------------------
		    comp.p7FFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
		if ((port & 0x8202) == (0xFDFD & 0x8202))	//&& conf.mem_model == MM_ATM450 
		{
		    comp.pFDFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
			if ( dc_out__AY_TS_TSFM_nedoSAA_etc( port, val )) return;	// AY_TS_TSFM_nedoSAA_etc
		//-------------------------------------------------------------
		// а зачем выход?
		// никуда ж не записали о_О
		return;
	    }
	    break;
	//=====================================================================
	case MM_ATM710:
	    // #xD
	    if (!(port & 2))
	    {
		//-------------------------------------------------------------
			if ( dc_out__Covox_DD( port, val )) return;		// Covox_DD
		//-------------------------------------------------------------
		if (!(port & 0x8000)) // zx128 port
		{
		// 0001xxxxxxxxxx0x (bcig4) // 1FFD
		// 0010xxxxxxxxxx0x (bcig4) // 2FFD
		// 0011xxxxxxxxxx0x (bcig4) // 3FFD
		//-------------------------------------------------------------
		    // ХЗ что это (но скорей всего недопыцы об этом лжет с целью повышения продаж!!!)
		    //
		    // if (conf.mem_model == MM_ATM710 && (port & 0x8202) != (0x7FFD & 0x8202)) return; // strict 7FFD decoding on ATM-2
		    //---------------------------------------------------------
		    // 7FFD
		    if (comp.p7FFD & 0x20)
		    { // 48k lock
			//  0.39.0
			// if not pentagon-1024 or profi with #DFFD.4 set, apply lock
			//  NEDOREPO
			// if not pentagon-1024 or pentevo (atm3) --(added by lvd)-- or profi with #DFFD.4 set, apply lock
			// 0.39.0
			//    if (!((conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
			//         (conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10)))) 	 molodcov_alex
			if (!(	(conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
				(conf.mem_model == MM_ATM3)				||
				(conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10))
			    ))								// molodcov_alex
			    return;
		    }
		    //---------------------------------------------------------
		    if ((comp.p7FFD ^ val) & 0x08)
			update_screen();
		    //---------------------------------------------------------
		    comp.p7FFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
			if ( dc_out__AY_TS_TSFM_nedoSAA_etc( port, val )) return;	// AY_TS_TSFM_nedoSAA_etc
		//-------------------------------------------------------------
		// а зачем выход?
		// никуда ж не записали о_О
		return;
	    }
	    break;
	//=====================================================================
	case MM_ATM3:
	    // #xD
	    if (!(port & 2))
	    {
		//-------------------------------------------------------------
			if ( dc_out__Covox_DD( port, val )) return;		// Covox_DD
		//-------------------------------------------------------------
		if (!(port & 0x8000)) // zx128 port
		{
		// 0001xxxxxxxxxx0x (bcig4) // 1FFD
		// 0010xxxxxxxxxx0x (bcig4) // 2FFD
		// 0011xxxxxxxxxx0x (bcig4) // 3FFD
		    //---------------------------------------------------------
		    // 7FFD
		    if (comp.p7FFD & 0x20)
		    { // 48k lock
			if ((comp.pEFF7 & EFF7_LOCKMEM) ) // lvd added eff7 to atm3	//&& conf.mem_model == MM_ATM3
			    return;

			//  0.39.0
			// if not pentagon-1024 or profi with #DFFD.4 set, apply lock
			//  NEDOREPO
			// if not pentagon-1024 or pentevo (atm3) --(added by lvd)-- or profi with #DFFD.4 set, apply lock
			//0.39.0
			//    if (!((conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
			//         (conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10)))) 	 molodcov_alex
			if (!(	(conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
				(conf.mem_model == MM_ATM3)				||
				(conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10))
			    ))								// molodcov_alex
			    return;
		    }
		    //---------------------------------------------------------
		    if ((comp.p7FFD ^ val) & 0x08)
			update_screen();
		    //---------------------------------------------------------
		    comp.p7FFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
			if ( dc_out__AY_TS_TSFM_nedoSAA_etc( port, val )) return;	// AY_TS_TSFM_nedoSAA_etc
		//-------------------------------------------------------------
		// а зачем выход?
		// никуда ж не записали о_О
		return;
	    }
	    break;
	//=====================================================================
	case MM_KAY:
	    // #xD
	    if (!(port & 2))
	    {
		//-------------------------------------------------------------
			if ( dc_out__Covox_DD( port, val )) return;		// Covox_DD
		//-------------------------------------------------------------
		if (!(port & 0x8000)) // zx128 port
		{
		// 0001xxxxxxxxxx0x (bcig4) // 1FFD
		// 0010xxxxxxxxxx0x (bcig4) // 2FFD
		// 0011xxxxxxxxxx0x (bcig4) // 3FFD
		    //---------------------------------------------------------
		    if ((port & 0xC003) == (0x1FFD & 0xC003))	//&& conf.mem_model == MM_KAY
		    {
			//goto set1FFD;
			//set1FFD:
			comp.p1FFD = val;
			set_banks();
			return;
		    }
		    //---------------------------------------------------------
		    // 7FFD
		    if (comp.p7FFD & 0x20)
		    { // 48k lock
			// 0.39.0
			//   if not pentagon-1024 or profi with #DFFD.4 set, apply lock
			// NEDOREPO
			//   if not pentagon-1024 or pentevo (atm3) --(added by lvd)-- or profi with #DFFD.4 set, apply lock
			// 0.39.0
			//    if (!((conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
			//         (conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10)))) 	 molodcov_alex
			if (!(	(conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
				(conf.mem_model == MM_ATM3)				||
				(conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10))
			    ))								// molodcov_alex
			    return;
		    }
		    //---------------------------------------------------------
		    if ((comp.p7FFD ^ val) & 0x08)
			update_screen();
		    //---------------------------------------------------------
		    comp.p7FFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
			if ( dc_out__AY_TS_TSFM_nedoSAA_etc( port, val )) return;	// AY_TS_TSFM_nedoSAA_etc
		//-------------------------------------------------------------
		// а зачем выход?
		// никуда ж не записали о_О
		return;
	    }
	    break;
	//=====================================================================
	case MM_PLUS3:
	    // #xD
	    if (!(port & 2))
	    {
		//-------------------------------------------------------------
			if ( dc_out__Covox_DD( port, val )) return;		// Covox_DD
		//-------------------------------------------------------------
		if (!(port & 0x8000)) // zx128 port
		{
		// 0001xxxxxxxxxx0x (bcig4) // 1FFD
		// 0010xxxxxxxxxx0x (bcig4) // 2FFD
		// 0011xxxxxxxxxx0x (bcig4) // 3FFD
		    //---------------------------------------------------------
		    if ((port & (3 << 14)) == 0)	// && conf.mem_model == MM_PLUS3
		    {
			unsigned Idx = (port >> 12) & 3;
			switch(Idx)
			{
			    case 1: // 1FFD
				//goto set1FFD;
				//set1FFD:
				comp.p1FFD = val;
				set_banks();
				return;
			    case 3: // 3FFD
				Upd765.out(val);
				return;
			}
		    }
		    //---------------------------------------------------------
		    // 7FFD
		    if (comp.p7FFD & 0x20)
		    { // 48k lock
			// 0.39.0
			//   if not pentagon-1024 or profi with #DFFD.4 set, apply lock
			// NEDOREPO
			//   if not pentagon-1024 or pentevo (atm3) --(added by lvd)-- or profi with #DFFD.4 set, apply lock
			// 0.39.0
			//    if (!((conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
			//         (conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10)))) 	 molodcov_alex
			if (!(	(conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
				(conf.mem_model == MM_ATM3)				||
				(conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10))
			    ))								// molodcov_alex
			    return;
		    }
		    //---------------------------------------------------------
		    if ((comp.p7FFD ^ val) & 0x08)
			update_screen();
		    //---------------------------------------------------------
		    comp.p7FFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
			if ( dc_out__AY_TS_TSFM_nedoSAA_etc( port, val )) return;	// AY_TS_TSFM_nedoSAA_etc
		//-------------------------------------------------------------
		// а зачем выход?
		// никуда ж не записали о_О
		return;
	    }
	    break;
	//=====================================================================
	case MM_QUORUM:
	    // #xD
	    if (!(port & 2))
	    {
		//-------------------------------------------------------------
			if ( dc_out__Covox_DD( port, val )) return;		// Covox_DD
		//-------------------------------------------------------------
		if (!(port & 0x8000)) // zx128 port
		{
		// 0001xxxxxxxxxx0x (bcig4) // 1FFD
		// 0010xxxxxxxxxx0x (bcig4) // 2FFD
		// 0011xxxxxxxxxx0x (bcig4) // 3FFD
		    //---------------------------------------------------------
		    // 0xxxxxxxxxx11x0x
		    if ((port & 0x801A) != (0x7FFD & 0x801A))	// && (conf.mem_model == MM_QUORUM
			return;
		    //---------------------------------------------------------
		    // 7FFD
		    if (comp.p7FFD & 0x20)
		    { // 48k lock
			// 0.39.0
			//   if not pentagon-1024 or profi with #DFFD.4 set, apply lock
			// NEDOREPO
			//   if not pentagon-1024 or pentevo (atm3) --(added by lvd)-- or profi with #DFFD.4 set, apply lock
			// 0.39.0
			//    if (!((conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
			//         (conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10)))) 	 molodcov_alex
			if (!(	(conf.ramsize == 1024 && conf.mem_model == MM_PENTAGON)	||
				(conf.mem_model == MM_ATM3)				||
				(conf.mem_model == MM_PROFI && (comp.pDFFD & 0x10))
			    ))								// molodcov_alex
			    return;
		    }
		    //---------------------------------------------------------
		    if ((comp.p7FFD ^ val) & 0x08)
			update_screen();
		    //---------------------------------------------------------
		    comp.p7FFD = val;
		    set_banks();
		    return;
		}
		//-------------------------------------------------------------
		// 1x0xxxxxxxx11x0x
		if ((port & 0xA01A) == (0x80FD & 0xA01A))	// && conf.mem_model == MM_QUORUM
		{
		    comp.p80FD = val;
		    set_banks();
		    return;
		}	
		//-------------------------------------------------------------
			if ( dc_out__AY_TS_TSFM_nedoSAA_etc( port, val )) return;	// AY_TS_TSFM_nedoSAA_etc
		//-------------------------------------------------------------
		// а зачем выход?
		// никуда ж не записали о_О
		return;
	    }
	    break;
	//=====================================================================
    }
    
//-----------------------------------------------------------------------------
// снова общие порты
//-----------------------------------------------------------------------------
    
		//-------------------------------------------------------------
			if ( dc_out__Soundrive( port, val )) return;		// Soundrive
		//-------------------------------------------------------------
			if ( dc_out__Covox_FB( port, val )) return;		// Covox FB
		//-------------------------------------------------------------
			if ( dc_out__ZXM_saa1099( port, val )) return;		// ZXM saa1099
		//-------------------------------------------------------------
   


//-----------------------------------------------------------------------------
// снова платформозависимые
//-----------------------------------------------------------------------------
    


    //список в emul.h
    switch (conf.mem_model)
    {
	//=====================================================================
	case MM_PENTAGON:
	    //---------------------------------------------------------------------
	    if (port == 0xEFF7) 	// && conf.mem_model == MM_PENTAGON, MM_ATM3, MM_ATM710 
	    {
		unsigned char oldpEFF7 = comp.pEFF7; //Alone Coder 0.36.4
		comp.pEFF7 = (comp.pEFF7 & conf.EFF7_mask) | (val & ~conf.EFF7_mask);
		//-------------------------------------------------------------
		comp.pEFF7 |= EFF7_GIGASCREEN; // [vv] disable turbo
		// if ((comp.pEFF7 ^ oldpEFF7) & EFF7_GIGASCREEN) 
		// {
		//	conf.frame = frametime;
		//	if ((conf.mem_model == MM_PENTAGON)&&(comp.pEFF7 & EFF7_GIGASCREEN))conf.frame = 71680;
		//	apply_sound();
		// } //Alone Coder removed 0.37.1
		//-------------------------------------------------------------
		if (!(comp.pEFF7 & EFF7_4BPP))
		{
		    temp.offset_vscroll = 0;
		    temp.offset_vscroll_prev = 0;
		    temp.offset_hscroll = 0;
		    temp.offset_hscroll_prev = 0;
		}
		//-------------------------------------------------------------
		if ((comp.pEFF7 ^ oldpEFF7) & (EFF7_ROCACHE | EFF7_LOCKMEM))
		    set_banks();	//Alone Coder 0.36.4
		//-------------------------------------------------------------
		return;
	    }
	    //-----------------------------------------------------------------
	    if (conf.cmos && (comp.pEFF7 & EFF7_CMOS))	// &&  conf.mem_model == MM_PENTAGON
	    {	//для пентагона только с (comp.pEFF7 & EFF7_CMOS)
 
		unsigned mask = (conf.mem_model == MM_ATM3 && (comp.flags & CF_DOSPORTS)) ? ~0x100U : 0xFFFF;

		//-------------------------------------------------------------
		if (port == (0xDFF7 & mask))
		{
		    comp.cmos_addr = val;
		    return;
		}
		//-------------------------------------------------------------
		if (port == (0xBFF7 & mask))
		{
		    cmos_write(val);	//для MM_PENTAGON как в 0.39.0
		    return; 
		}
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break;
	//=====================================================================
//	case MM_SCORP:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PROFSCORP:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PROFI:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM450:
//	    break;
	//=====================================================================
	case MM_ATM710:
	    //-----------------------------------------------------------------
	    if (port == 0xEFF7) 	// && conf.mem_model == MM_PENTAGON, MM_ATM3, MM_ATM710
	    {				// lvd added eff7 to atm3	NEDOREPO
					// [NS] лвдо то добавил, но к чему тут АТМ2 ??? o_O
					// скорей всего рукожопства????
		unsigned char oldpEFF7 = comp.pEFF7; //Alone Coder 0.36.4
		comp.pEFF7 = (comp.pEFF7 & conf.EFF7_mask) | (val & ~conf.EFF7_mask);
		//-------------------------------------------------------------
		if (conf.mem_model == MM_ATM710) 	//NEDOREPO
		    return;
		//-------------------------------------------------------------
		// МЕРТВОКОД !!!
		comp.pEFF7 |= EFF7_GIGASCREEN; // [vv] disable turbo
		// if ((comp.pEFF7 ^ oldpEFF7) & EFF7_GIGASCREEN)
		// {
		//	conf.frame = frametime;
		//	if ((conf.mem_model == MM_PENTAGON)&&(comp.pEFF7 & EFF7_GIGASCREEN))conf.frame = 71680;
		//	apply_sound();
		// } //Alone Coder removed 0.37.1
		//-------------------------------------------------------------
		// МЕРТВОКОД !!!
		if (!(comp.pEFF7 & EFF7_4BPP))
		{
		    temp.offset_vscroll = 0;
		    temp.offset_vscroll_prev = 0;
		    temp.offset_hscroll = 0;
		    temp.offset_hscroll_prev = 0;
		}
		//-------------------------------------------------------------
		// МЕРТВОКОД !!!
		if ((comp.pEFF7 ^ oldpEFF7) & (EFF7_ROCACHE | EFF7_LOCKMEM))
		    set_banks(); //Alone Coder 0.36.4
		//-------------------------------------------------------------
		return;
	    }
	    //-----------------------------------------------------------------
	    if (conf.cmos && (comp.pEFF7 & EFF7_CMOS))	//conf.mem_model == MM_ATM710
	    {	//для MM_ATM710 только с (comp.pEFF7 & EFF7_CMOS) !!! (раньше вообще небыло для MM_ATM710)

		unsigned mask = (conf.mem_model == MM_ATM3 && (comp.flags & CF_DOSPORTS)) ? ~0x100U : 0xFFFF;

		//-------------------------------------------------------------
		if (port == (0xDFF7 & mask))
		{
		    comp.cmos_addr = val;
		    return;
		}
		//-------------------------------------------------------------
		if (port == (0xBFF7 & mask))
		{
		    cmos_write(val);	////для MM_ATM710 как в 0.39.0
		    return;
		}
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break;
	//=====================================================================
	case MM_ATM3:
	    //-----------------------------------------------------------------------------
	    if (port == 0xEFF7)		// && conf.mem_model == MM_PENTAGON, MM_ATM3, MM_ATM710
	    {				// lvd added eff7 to atm3	NEDOREPO
		unsigned char oldpEFF7 = comp.pEFF7; //Alone Coder 0.36.4
		comp.pEFF7 = (comp.pEFF7 & conf.EFF7_mask) | (val & ~conf.EFF7_mask);
		//-------------------------------------------------------------
		comp.pEFF7 |= EFF7_GIGASCREEN; // [vv] disable turbo
		// if ((comp.pEFF7 ^ oldpEFF7) & EFF7_GIGASCREEN)
		// {
		//	conf.frame = frametime;
		//	if ((conf.mem_model == MM_PENTAGON)&&(comp.pEFF7 & EFF7_GIGASCREEN))conf.frame = 71680;
		//	apply_sound();
		// } //Alone Coder removed 0.37.1
		//-------------------------------------------------------------
		if (!(comp.pEFF7 & EFF7_4BPP))
		{
		    temp.offset_vscroll = 0;
		    temp.offset_vscroll_prev = 0;
		    temp.offset_hscroll = 0;
		    temp.offset_hscroll_prev = 0;
		}
		//-------------------------------------------------------------
		if ((comp.pEFF7 ^ oldpEFF7) & (EFF7_ROCACHE | EFF7_LOCKMEM))
		    set_banks();	//Alone Coder 0.36.4
		//-------------------------------------------------------------
		return;
	    }
	    //-----------------------------------------------------------------  
	    if (conf.cmos)			//&& conf.mem_model == MM_ATM3
	    {	//для MM_ATM3 вообще небыло проверки (comp.pEFF7 & EFF7_CMOS) !!!!
   
		unsigned mask = ((conf.mem_model == MM_ATM3) && (comp.flags & CF_DOSPORTS)) ? ~0x100U : 0xFFFF;

		//-------------------------------------------------------------
		if (port == (0xDFF7 & mask))
		{
		    comp.cmos_addr = val;
		    return;
		}
		//-------------------------------------------------------------
		if (port == (0xBFF7 & mask))
		{
		    //cmos_write(val);	//было в 0.39.0
		    //return;
	  
		    // было дето закоменчено (наверно в NEDOREPO)
		    //	if (comp.cmos_addr >= 0xF0 && (val & 0xf0) == 0x10 && conf.mem_model == MM_ATM3)
		    // 	{
		    //	    comp.fddIO2Ram_mask=val;
		    // 	}
		    //	else
		    if (comp.cmos_addr >= 0xF0 && val <= 2)	// && conf.mem_model == MM_ATM3
		    {//thims added
			if (val < 2)
			{
			    input.buffer_enabled = false;
			    static unsigned version = 0;
			    
			    if (!version)
			    {
				unsigned day, year;
				char month[8];
				static const char months[] = "JanFebMarAprMayJunJulAugSepOctNovDec"; 
				sscanf(__DATE__, "%s %d %d", month, &day, &year);
				version = day | ((strstr( months, month) - months) / 3 + 1) << 5 | (year - 2000) << 9;
			    }
               
			    //strcpy((char*)cmos + 0xF0, "UnrealSpeccy");
			    strcpy( (char*) cmos + 0xF0, "Unreal_NS");		//[NS]
			    *(unsigned*) (cmos + 0xFC) = version;
			}
			else
			{
			    input.buffer_enabled = true;
			}
		    }
		    else
		    {
			cmos_write( val);
		    }
		return;
		}
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break;
	//=====================================================================
//	case MM_KAY:
//	    break;
//	//-----------------------------------------------------------------
//	case MM_PLUS3:
//	    break;
//	//-----------------------------------------------------------------
//	case MM_QUORUM:
//	    break; 
//	//---------------------------------------------------------------------
    }

   
//-----------------------------------------------------------------------------
// опять и снова опять общие порты
//-----------------------------------------------------------------------------
    
		//-------------------------------------------------------------
    			if ( dc_out__Modem_HZ( port, val )) return;		// Modem_HZ
		//-------------------------------------------------------------
 
}	//out end
//=============================================================================



















				
				
				




















//=============================================================================
__inline unsigned char in1(unsigned port)
{
    port &= 0xFFFF;
    brk_port_in = port;

    u8 p1 = (port & 0xFF);

/*
    if((port & 0xFF) == 0xF0)
	__debugbreak();

    if((comp.flags & CF_DOSPORTS) && port == 0xFADF)
	__debugbreak();
*/

    // В начале дешифрация портов по полным 8бит



	    
//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__atm620_xt_keyb( port );	if (in_v >= 0) return in_v; }	// ATM620 XT Keyb
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__ZXI_TSFM_Mirror( port );	if (in_v >= 0) return in_v; }	// ZXI TSFM
//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__Ula_Plus( port ); 		if (in_v >= 0) return in_v; }	// ULA PLUS
//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__NGS( port, p1 ); 		if (in_v >= 0) return in_v; }	// NGS
//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__zc( port ); 		if (in_v >= 0) return in_v; }	// Z-Controller
//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__wiznet( port ); 		if (in_v >= 0) return in_v; }	// Wiznet
//-----------------------------------------------------------------------------




    //-------------------------------------------------------------------------
    if (conf.mem_model == MM_ATM3)
    {
	//---------------------------------------------------------------------
	// Порт расширений АТМ3
	if ((port & 0xFF) == 0xBF)
	    return comp.pBF;
	//---------------------------------------------------------------------
	// xxBE - было раньше				(15.05.2014)
	// xxBD - чтения конфигурации компьютера	zxevo_base_configuration.pdf (07.01.2022)
//	if ( (port & 0xFF) == 0xBE)
	if (((port & 0xFF) == 0xBE) || ((port & 0xFF) == 0xBD))	//NEDOREPO
	{
	    u8 port_hi = (port >> 8) & 0xFF;
	    //-----------------------------------------------------------------
	    // 00BE...07BE Чтение не инвертированного номера страницы
	    if ((port_hi & ~7) == 0)	
	    {
		unsigned PgIdx = port_hi & 7;
		return (comp.pFFF7[PgIdx] & 0xFF) ^ 0xFF;
	    }
	    //-----------------------------------------------------------------
	    switch (port_hi)
	    {
		//-------------------------------------------------------------
		// ram/rom
		case 0x08: 
		{
		    u8 RamRomMask = 0;
		    for(unsigned i = 0; i < 8; i++)
			RamRomMask |= ((comp.pFFF7[i] >> 8) & 1) << i;
		    return ~RamRomMask;
		}
		//-------------------------------------------------------------
		// dos7ffd
		case 0x09: 
		{
		    u8 RamRomMask = 0;
		    for(unsigned i = 0; i < 8; i++)
			RamRomMask |= ((comp.pFFF7[i] >> 9) & 1) << i;
		    return ~RamRomMask;
		}
		//-------------------------------------------------------------
		// Последнее записанное значение в порт #7FFD
		case 0x0A:
		    return comp.p7FFD;
		//-------------------------------------------------------------
		// Последнее записанное значение в порт #EFF7
//        	case 0x0B:;	//ЗАКОМЕНЧЕНО В 0.39.0
		case 0x0B:	//NEDOREPO
		    return comp.pEFF7; // lvd - added EFF7 reading in pentevo (atm3)		   
		//-------------------------------------------------------------
		// Последнее записанное значение в порт #xx77.
		case 0x0C:
		    // lvd: fixed bug with no-anding bits from aFF77, added CF_TRDOS to bit 4
		    // lvd: changed bit 4 to dos state, remembered during nmi
		    //
	//	    return u8(	((( comp.aFF77 >> 14) & 1) << 7		) |	// 0.39.0
	//			((( comp.aFF77 >> 9)  & 1) << 6		) |
	//			((( comp.aFF77 >> 8)  & 1) << 5		) |
	//			(  (comp.flags & CF_TRDOS)  ?	0x10 :
	//							0	) |
	//			( comp.pFF77 & 0xF)
	//			);			
		    return (	((( comp.aFF77 >> 14) << 7) & 0x0080	) |	// NEDOREPO
				((( comp.aFF77 >> 9 ) << 6) & 0x0040	) |
				((( comp.aFF77 >> 8 ) << 5) & 0x0020	) |
				(    trdos_in_nmi   ?	0x0010 :
							0		) |
				( comp.pFF77 & 0x0F			)
			   );
		//-------------------------------------------------------------
		// Чтение текущего отображаемого цвета (для чтения палитры)
		case 0x0D:
		    return atm_readpal();
		//-------------------------------------------------------------
		// Чтение текущего отображаемого байта в текстовом режиме (для считывания шрифта)
		case 0x0E:
		    return zxevo_readfont();
		//-------------------------------------------------------------
		// чтение последнего установленного цвета бордюра
	//	case 0x0F:   
		//-------------------------------------------------------------
		// breakpoint address readback	LOW
		case 0x10:
		    return comp.brk_addr & 0x00FF;
		//-------------------------------------------------------------
		// breakpoint address readback	HIGH
		case 0x11:
		    return (comp.brk_addr >> 8) & 0x00FF;
		//-------------------------------------------------------------
		// Биты защиты от записи
	//	case 0x12:   
		//-------------------------------------------------------------
		// read fddIO2Ram_mask
		case 0x13:
		    return comp.fddIO2Ram_mask;
		//-------------------------------------------------------------
		// read scanline (UNDOC) !!!
		case 0x14:
		    return ((cpu.t / 224) >> 1) & 0x00FF;
		//-------------------------------------------------------------
	    }
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    
    
    
//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__NEMO_DivIDE( port ); 	if (in_v >= 0) return in_v; }	// NEMO DivIDE
//-----------------------------------------------------------------------------


    //-------------------------------------------------------------------------
    // quorum additional keyboard port
    if (conf.mem_model == MM_QUORUM)
    {
	if ((port & 0xFF) == 0x7E)
	{
	    u8 val = input.read_quorum( u8 (port >> 8));
	    return val;
	}
    }
    //-------------------------------------------------------------------------
    
    
    
    
    
    
    

//-----------------------------------------------------------------------------
// SHADOW PORTS - Платформозависимые
//-----------------------------------------------------------------------------
    if (comp.flags & CF_DOSPORTS)
    {
   


	//список в emul.h
	switch (conf.mem_model)
	{
	//---------------------------------------------------------------------
//	case MM_PENTAGON:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_SCORP:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PROFSCORP:
//	    break;
	//=====================================================================
	case MM_PROFI:
	//  if (conf.mem_model == MM_PROFI) // molodcov_alex
	    //-----------------------------------------------------------------
	    // modified ports
	    if ((comp.p7FFD & 0x10) && (comp.pDFFD & 0x20))
	    {
		//-------------------------------------------------------------
		// BDI ports
		if ((p1 & 0x9F) == 0x83)
		    return comp.wd.in((p1 & 0x60) | 0x1F);	 // WD93 ports (1F, 3F, 7F)
		//-------------------------------------------------------------
		// port FF
		if ((p1 & 0xE3) == 0x23)
		    return comp.wd.in(0xFF);                
		//-------------------------------------------------------------
		// RTC
		if (conf.cmos)
		{
		    if ((port & 0x9F) == 0x9F)
		    {
			if (!(port & 0x20))
			    return cmos_read();
		    }
		}
		//-------------------------------------------------------------
		//  
		// IDE_PROFI 
		//		Profi IDE был тут
		//		но вынесен в общие порты dc_in__Profi_IDE
		//		для возможности создания суперспецтрумов
		//
		//-------------------------------------------------------------
	    } 
	    //-----------------------------------------------------------------
	    // not? modified ports
	    else
	    {
		//-------------------------------------------------------------
		// BDI ports
		if ((p1 & 0x83) == 0x03)
		    return comp.wd.in((p1 & 0x60) | 0x1F);  // WD93 ports
		//-------------------------------------------------------------
		// port FF
		if ((p1 & 0xE3) == ((comp.pDFFD & 0x20) ? 0xA3 :
							  0xE3
		    ))
		    return comp.wd.in(0xFF);    
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------

	    break;
	//=====================================================================
//	case MM_ATM450:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM710:
//	    break;
	//=====================================================================
	case MM_ATM3:
	    //-----------------------------------------------------------------
	//  if (((p1 & 0x1F) == 0x0F) &&  (((p1 >> 5) - 1) < 5))				// 0.39.0
	    if (((p1 & 0x1F) == 0x0F) && !(((p1 >> 5) - 1) & 4)) //&& conf.mem_model == MM_ATM3	// NEDOREPO
	    {
		// 2F = 001|01111b
		// 4F = 010|01111b
		// 6F = 011|01111b
		// 8F = 100|01111b
		// 		AF = 101|01111b	//НЕТУ В NEDOREPO
		return comp.wd_shadow[(p1 >> 5) - 1];
	    }
	    //-----------------------------------------------------------------
	    break;
	//=====================================================================
//	case MM_KAY:
//	    break;
//	//-----------------------------------------------------------------
//	case MM_PLUS3:
//	    break;
	//-----------------------------------------------------------------
	case MM_QUORUM:
	    //-----------------------------------------------------------------
	    //conf.mem_model == MM_QUORUM /* && !(comp.p00 & Q_TR_DOS) */) // cpm ports
	    if ((p1 & 0xFC) == 0x80) // 80, 81, 82, 83
	    {
		p1 = u8(((p1 & 3) << 5) | 0x1F);
		return comp.wd.in(p1);
	    }
	    //-----------------------------------------------------------------
	    break; 
	//---------------------------------------------------------------------
	}
	
//-----------------------------------------------------------------------------
// SHADOW PORTS - Общие
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__ATM_IDE( port ); 	if (in_v >= 0) return in_v; }	// ATM IDE
//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__SMUC( port ); 	if (in_v >= 0) return in_v; }	// SMUC
//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__Profi_IDE( port, p1 ); if (in_v >= 0) return in_v; } // Profi IDE
//-----------------------------------------------------------------------------

	//---------------------------------------------------------------------
	// 
	// 1F = 0001|1111b
	// 3F = 0011|1111b
	// 5F = 0101|1111b
	// 7F = 0111|1111b
	// DF = 1101|1111b порт мыши
	// FF = 1111|1111b
	//
	// not quorum	
	// как было?
	// if ((p1 & 0x9F) == 0x1F || p1 == 0xFF) // 1F, 3F, 5F, 7F, FF
	//    return comp.wd.in(p1);	0.39.0
	//
	// опять какаято недописюкатница добавленная тупо поверх всего вместо
	if ((p1 & 0x9F) == 0x1F || p1 == 0xFF) 
	{// 1F, 3F, 5F, 7F, FF
	    if (    (comp.flags & CF_TRDOS)			&&
		    conf.trdos_IORam				&& 
		    ((1<<comp.wd.drive)&comp.fddIO2Ram_mask)	&&
		    (bankr[0] == base_dos_rom)
	      )
	    {
		comp.fddIO2Ram_wr_disable = true;
		cpu.nmi_in_progress=conf.trdos_IORam;
		trdos_in_nmi = comp.flags & CF_TRDOS;
		set_banks(); 
		return 0xff;
	    }
	    else
	    {
		if (conf.trdos_IORam && (p1&0x80))
		    return (comp.wd.in(p1) & 0xE0) | comp.trdos_last_ff;
		
		return comp.wd.in(p1);
	    }
	}
	//---------------------------------------------------------------------


      
      
   
    }
//-----------------------------------------------------------------------------
// NO shadow ports (общие)
//-----------------------------------------------------------------------------
    else
    {
   
//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__DivIDE( port ); if (in_v >= 0) return in_v; }	// DivIDE
//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__NEMO_IDE( port ); if (in_v >= 0) return in_v; }	// NEMO IDE
//-----------------------------------------------------------------------------

    }
   
//-----------------------------------------------------------------------------
// опять NO shadow ports (общие)
//-----------------------------------------------------------------------------
   
//-----------------------------------------------------------------------------
	{ register int in_v = dc_in__Kempston_Mouse( port ); if (in_v >= 0) return in_v; } // Kempston Mouse
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

//   if (((conf.mem_model != MM_ATM3) && !(port & 0x20)) ||		// какойто остаток от кемпстона
//       ((conf.mem_model == MM_ATM3) && (p1 == 0x1F || p1 == 0xDF)))	// над которым надо поразмышлять

    if ((conf.mem_model == MM_SCORP) || (conf.mem_model == MM_PROFSCORP))
    {
	// у скорпа свой левый кемпстон
	{ register int in_v = dc_in__Kempston_Joystick_Scorp( port ); if (in_v >= 0) return in_v; } // Kempston Joystick Scorp
    }
    else
    {
	{ register int in_v = dc_in__Kempston_Joystick( port ); if (in_v >= 0) return in_v; } // Kempston Joystick
    }
//-----------------------------------------------------------------------------
   	{ register int in_v = dc_in__Fuller_Joystick( p1 ); if (in_v >= 0) return in_v; } // Fuller Joystick
//-----------------------------------------------------------------------------
    
    


//-----------------------------------------------------------------------------
// Port FE - IN
//-----------------------------------------------------------------------------v

	// port #FE
	bool pFE;    
    
    //	//список в emul.h
	switch (conf.mem_model)
	{
//	//=====================================================================
//	case MM_PENTAGON:
//		DEFAULT
//	    break;
//	//=====================================================================
	case MM_SCORP:
	    //-----------------------------------------------------------------
	    // scorp  xx1xxx10 (sc16)		// conf.mem_model == MM_SCORP, MM_PROFSCORP
	    pFE = ((port & 0x23) == (0xFE & 0x23)) && !(comp.flags & CF_DOSPORTS);
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
		//-------------------------------------------------------------
		if (	((cpu.pc & 0xFFFF) == 0x0564)	&&
			(bankr[0][0x0564] == 0x1F)	&&
			conf.tape_autostart		&&
			comp.tape.stopped
		 )
		{
		    start_tape();
		}
		//-------------------------------------------------------------
		u8 val = input.read(u8(port >> 8));
		return val;
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break;
//	//=====================================================================
	case MM_PROFSCORP:
	    //-----------------------------------------------------------------
	    // scorp  xx1xxx10 (sc16)		// conf.mem_model == MM_SCORP, MM_PROFSCORP
	    pFE = ((port & 0x23) == (0xFE & 0x23)) && !(comp.flags & CF_DOSPORTS);
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
		//-------------------------------------------------------------
		if (	((cpu.pc & 0xFFFF) == 0x0564)	&&
			(bankr[0][0x0564] == 0x1F)	&&
			conf.tape_autostart		&&
			comp.tape.stopped
		 )
		{
		    start_tape();
		}
		//-------------------------------------------------------------
		u8 val = input.read(u8(port >> 8));
		return val;
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break;
//	//=====================================================================
//	case MM_PROFI:
//		DEFAULT
//	    break;
//	//=====================================================================
	case MM_ATM450:
	    //-----------------------------------------------------------------
	    // others xxxxxxx0		//DEFAULT
	    pFE = !(port & 1);
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
		//-------------------------------------------------------------
		if (	((cpu.pc & 0xFFFF) == 0x0564)	&&
			(bankr[0][0x0564] == 0x1F)	&&
			conf.tape_autostart		&&
			comp.tape.stopped
		 )
		{
		    start_tape();
		}
		//-------------------------------------------------------------
		u8 val = input.read(u8(port >> 8));
		val = (val & 0x7F) | atm450_z(cpu.t);	// conf.mem_model == MM_ATM450
		return val;
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break;
//	//=====================================================================
	case MM_ATM710:
	    //-----------------------------------------------------------------
	    // others xxxxxxx0		//DEFAULT
	    pFE = !(port & 1);
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
		//-------------------------------------------------------------
		if (	((cpu.pc & 0xFFFF) == 0x0564)	&&
			(bankr[0][0x0564] == 0x1F)	&&
			conf.tape_autostart		&&
			comp.tape.stopped
		 )
		{
		    start_tape();
		}
		//-------------------------------------------------------------
		u8 val = input.read(u8(port >> 8));
		if (conf.atm620_hl8_z)	// копирастия для запуска MSX-DOS [NS]
		    val = (val & 0xDF) | atm620_z(cpu.t);		
		return val;
		//-------------------------------------------------------------
	    }
	    break;
//	//=====================================================================
//	case MM_ATM3:
//		DEFAULT
//	    break;
//	//=====================================================================
//	case MM_KAY:
//		DEFAULT
//	    break;
//	//=====================================================================
//	case MM_PLUS3:
//		DEFAULT
//	    break;
//	//=====================================================================
	case MM_QUORUM:
	    //-----------------------------------------------------------------
	    // 1xx11xx0		//conf.mem_model == MM_QUORUM)
	    pFE = ((port & 0x99) == (0xFE & 0x99));
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
		//-------------------------------------------------------------
		if (	((cpu.pc & 0xFFFF) == 0x0564)	&&
			(bankr[0][0x0564] == 0x1F)	&&
			conf.tape_autostart		&&
			comp.tape.stopped
		 )
		{
		    start_tape();
		}
		//-------------------------------------------------------------
		u8 val = input.read(u8(port >> 8));
		return val;
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break; 
	//=====================================================================
	default:
	    //-----------------------------------------------------------------
	    // others xxxxxxx0
	    pFE = !(port & 1);
	    //-----------------------------------------------------------------
	    if (pFE)
	    {
		//-------------------------------------------------------------
		if (	((cpu.pc & 0xFFFF) == 0x0564)	&&
			(bankr[0][0x0564] == 0x1F)	&&
			conf.tape_autostart		&&
			comp.tape.stopped
		 )
		{
		    start_tape();
		}
		//-------------------------------------------------------------
		u8 val = input.read(u8(port >> 8));
		return val;
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	//=====================================================================
	}

    
    
     
//-----------------------------------------------------------------------------
   	{ register int in_v = dc_in__ATM_IDE_intrq( port ); if (in_v >= 0) return in_v; } // ATM IDE intrq
//-----------------------------------------------------------------------------

 





	//список в emul.h
	switch (conf.mem_model)
	{
//	//---------------------------------------------------------------------
//	case MM_PENTAGON:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_SCORP:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PROFSCORP:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_PROFI:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM450:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM710:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_ATM3:
//	    break;
//	//---------------------------------------------------------------------
//	case MM_KAY:
//	    break;
	//=====================================================================
	case MM_PLUS3:
	    //-----------------------------------------------------------------
	    // 0001xxxxxxxxxx0x (bcig4) // 1FFD
	    // 0010xxxxxxxxxx0x (bcig4) // 2FFD
	    // 0011xxxxxxxxxx0x (bcig4) // 3FFD
	    if ( (port & ((3 << 14) | 2)) == 0 )	//&& conf.mem_model == MM_PLUS3
	    {
		unsigned Idx = (port >> 12) & 3;
		switch (Idx)
		{
		    //---------------------------------------------------------
		    // 2FFD
		    case 2: 
			return Upd765.in(Idx);
		    //---------------------------------------------------------
		    // 3FFD
		    case 3: 
			return Upd765.in(Idx);	//одинаковое??? о_О
		    //---------------------------------------------------------
		}
	    }
	    //-----------------------------------------------------------------
	    break;
	//=====================================================================
//	case MM_QUORUM:
//	    break; 
//	//---------------------------------------------------------------------
	}


 
     
//-----------------------------------------------------------------------------
   	{ register int in_v = dc_in__AY_TS_TSFM_etc( port ); if (in_v >= 0) return in_v; } // AY TS TSFM etc
//-----------------------------------------------------------------------------
   	{ register int in_v = dc_in__Cache_FB( port ); if (in_v >= 0) return in_v; }	// Cache FB
//-----------------------------------------------------------------------------

    // пока оставлено в оригинальном виде
    //-------------------------------------------------------------------------
//  if (conf.cmos && ((comp.pEFF7 & EFF7_CMOS) || conf.mem_model == MM_ATM3))
    //
    if (conf.cmos && (	(comp.pEFF7 & EFF7_CMOS)	||	    // NEDOREPO
			(conf.mem_model == MM_ATM3)	|| 
			(conf.mem_model == MM_ATM710)
		      )
      )
    {
	unsigned mask = ((conf.mem_model == MM_ATM3) && (comp.flags & CF_DOSPORTS))  ?	~0x100U :
											0xFFFF;
	if (port == (0xBFF7 & mask))
	    return cmos_read();
    }
    //-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
   	{ register int in_v = dc_in__Modem_HZ( port ); if (in_v >= 0) return in_v; }	// Modem HZ
//-----------------------------------------------------------------------------






// Самый последний порт FF !!!
//-----------------------------------------------------------------------------
   	{ register int in_v = dc_in__Port_FF( port ); if (in_v >= 0) return in_v; }	// Port FF
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
    return 0xFF;	// чтение пустоты в самом конце
//-----------------------------------------------------------------------------
}
//=============================================================================


//=============================================================================
unsigned char in(unsigned port)
{
    brk_port_val = in1(port);		// и зочем оно так? О_о
    return brk_port_val;		// для бряков?
}
//=============================================================================

#undef in_trdos
#undef out_trdos
