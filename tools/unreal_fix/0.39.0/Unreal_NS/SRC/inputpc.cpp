#include "std.h"

#include "emul.h"
#include "vars.h"
#include "inputpc.h"


//=============================================================================
// Таблица для конвертации обратно из виндового XT -> в ps/2 AT 	thims zxevo_ps/2
// [NS] сверка с педевикией + коменты + мультимедиа кнопки

#define NO_KEY 0x0000

const unsigned short dik_scan[256] =		// NEDOREPO		
{   						//			
//  & x1xx - добавляет к сканкоду E0 xx

		// XT		AT		key		key
// $00
    NO_KEY,	// 00
    0x0076,	// 01 81	76 F0,76	DIK_ESCAPE	Esc
    0x0016,	// 02 82	16 F0,16 	DIK_1		1 !
    0x001E,	// 03 83	1E F0,1E	DIK_2		2 @  

    0x0026,	// 04 84 	26 F0,26 	DIK_3		3 #        
    0x0025,	// 05 85 	25 F0,25	DIK_4		4 $      
    0x002E,	// 06 86 	2E F0,2E 	DIK_5		5 %           
    0x0036,	// 07 87 	36 F0,36	DIK_6		6 ^
// $08
    0x003D,	// 08 88 	3D F0,3D 	DIK_7		7 &           
    0x003E,	// 09 89 	3E F0,3E 	DIK_8		8 *         
    0x0046,	// 0A 8A 	46 F0,46 	DIK_9		9 (    
    0x0045,	// 0B 8B 	45 F0,45 	DIK_0		0 )
    
    0x004E,	// 0C 8C 	4E F0,4E 	DIK_MINUS	- _
    0x0055,	// 0D 82 	55 F0,55 	DIK_EQUALS	= +     
    0x0066,	// 0E 8E 	66 F0,66 	DIK_BACK	Backspace
    0x000D,	// 0F 8F 	0D F0,0D 	DIK_TAB		Tab     
// $10
    0x0015,	// 10 90 	15 F0,15 	DIK_Q		q Q      
    0x001D,	// 11 91 	1D F0,1D 	DIK_W		w W
    0x0024,	// 12 92 	24 F0,24 	DIK_E		e E    
    0x002D,	// 13 93 	2D F0,2D 	DIK_R		r R
    
    0x002C,	// 14 94 	2C F0,2C 	DIK_T		t T       
    0x0035,	// 15 95 	35 F0,35 	DIK_Y		y Y            
    0x003C,	// 16 96 	3C F0,3C 	DIK_U		u U
    0x0043,	// 17 97 	43 F0,43 	DIK_I		i I
// $18
    0x0044,	// 18 98 	44 F0,44 	DIK_O		o O
    0x004D,	// 19 99 	4D F0,4D	DIK_P		p P
    0x0054,	// 1A 9A 	54 F0,54	DIK_LBRACKET	[ {
    0x005B,	// 1B 9B 	5B F0,5B	DIK_RBRACKET	] }
    
    0x005A,	// 1C 9C 	5A F0,5A	DIK_RETURN	Enter
    0x0014,	// 1D 9D 	14 F0,14 	DIK_LCONTROL	lCtrl
    0x001C,	// 1E 9E 	1C F0,1C 	DIK_A		a A
    0x001B,	// 1F 9F 	1B F0,1B 	DIK_S		s S
// $20
    0x0023,	// 20 A0 	23 F0,23 	DIK_D		d D
    0x002B,	// 21 A1 	2B F0,2B 	DIK_F		f F       
    0x0034,	// 22 A2 	34 F0,34 	DIK_G		g G
    0x0033,	// 23 A3 	33 F0,33 	DIK_H		h H
    
    0x003B,	// 24 A4 	3B F0,3B 	DIK_J		j J
    0x0042,	// 25 A5 	42 F0,42 	DIK_K		k K
    0x004B,	// 26 A6 	4B F0,4B 	DIK_L		l L
    0x004C,	// 27 A7 	4C F0,4C 	DIK_SEMICOLON	; :
// $28
    0x0052,	// 28 A8 	52 F0,52 	DIK_APOSTROPHE	' "
    0x000E,	// 29 89 	0E F0,0E 	DIK_GRAVE	` ~
    0x0012,	// 2A AA 	12 F0,12 	DIK_LSHIFT	lShift
    0x005D,	// 2B AB 	5D F0,5D 	DIK_BACKSLASH	\ |
    
    0x001A,	// 2C AC 	1A F0,1A 	DIK_Z		z Z
    0x0022,	// 2D AD 	22 F0,22 	DIK_X		x X
    0x0021,	// 2E AE 	21 F0,21	DIK_C		c C
    0x002A,	// 2F AF 	2A F0,2A 	DIK_V		v V
// $30
    0x0032,	// 30 B0 	32 F0,32 	DIK_B		b B
    0x0031,	// 31 B1 	31 F0,31	DIK_N		n N
    0x003A,	// 32 B2 	3A F0,3A 	DIK_M		m M
    0x0041,	// 33 B3 	41 F0,41 	DIK_COMMA	, <
    
    0x0049,	// 34 B4 	49 F0,49 	DIK_PERIOD	. >
    0x004A,	// 35 B5 	4A F0,4A 	DIK_SLASH	/ ?
    0x0059,	// 36 B6 	59 F0,59 	DIK_RSHIFT	rShift
    0x007C,	// 37 B7 	7C F0,7C 	DIK_MULTIPLY	Num *
// $38
    0x0011,	// 38 B8 	11 F0,11 	DIK_LMENU	lAlt
    0x0029,	// 39 B9 	29 F0,29 	DIK_SPACE	Space
    0x0058,	// 3A BA 	58 F0,58 	DIK_CAPITAL	CapsLock
    0x0005,	// 3B BB 	05 F0,05 	DIK_F1		F1
    
    0x0006,	// 3C BC 	06 F0,06 	DIK_F2		F2
    0x0004,	// 3D BD 	04 F0,04 	DIK_F3		F3
    0x000C,	// 3E BE 	0C F0,0C	DIK_F4		F4
    0x0003,	// 3F BF 	03 F0,03 	DIK_F5		F5
// $40
    0x000B,	// 40 C0 	0B F0,0B	DIK_F6		F6
    0x0083,	// 41 C1 	83 F0,83	DIK_F7		F7
    0x000A,	// 42 C2 	0A F0,0A 	DIK_F8		F8
    0x0001,	// 43 C3 	01 F0,01 	DIK_F9		F9
    
    0x0009,	// 44 C4 	09 F0,09 	DIK_F10		F10
    0x0077,	// 45 C5 	77 F0,77	DIK_NUMLOCK	NumLock
    0x007E,	// 46 C6 	7E F0,7E 	DIK_SCROLL	ScrollLock
    0x006C,	// 47 C7 	6C F0,6C	DIK_NUMPAD7	Num 7
// $48
    0x0075,	// 48 C8 	75 F0,75 	DIK_NUMPAD8	Num 8
    0x007D,	// 49 C9 	7D F0,7D 	DIK_NUMPAD9     Num 9    
    0x007B,	// 4A CA 	7B F0,7B 	DIK_SUBTRACT	Num -
    0x006B,	// 4B CB 	6B F0,6B 	DIK_NUMPAD4	Num 4
    
    0x0073,	// 4C CC 	73 F0,73 	DIK_NUMPAD5	Num 5
    0x0074,	// 4D CD 	74 F0,74	DIK_NUMPAD6	Num 6
    0x0079,	// 4E CE 	79 F0,79 	DIK_ADD		Num +
    0x0069,	// 4F CF 	69 F0,69 	DIK_NUMPAD1     Num 1
// $50
    0x0072,	// 50 D0 	72 F0,72	DIK_NUMPAD2	Num 2
    0x007A,	// 51 D1 	7A F0,7A 	DIK_NUMPAD3	Num 3
    0x0070,	// 52 D2 	70 F0,70 	DIK_NUMPAD0	Num 0
    0x0071,	// 53 D3 	71 F0,71 	DIK_DECIMAL	Num .
    
    NO_KEY,	// 54 - ALT + PrintScreen (код для клавиши Alt генерируется отдельно)
    NO_KEY,	// 55 - hz
    NO_KEY,	// 56 - hz - DIK_OEM_102	/* <> or \| on RT 102-key keyboard (Non-U.S.) */
    0x0078,	// 57 D7	78 F0,78 	DIK_F11		F11
// $58
    0x0007,	// 58 D8 	07 F0,07	DIK_F12		F12
    NO_KEY,	// 59 - hz
    NO_KEY,	// 5A - hz
    NO_KEY,	// 5B - hz		есть E0,5B E0,DB - lWin
    
    NO_KEY,	// 5C - hz		есть E0,5C E0,DC - rWin
    NO_KEY,	// 5D - hz		есть E0,5D E0,DD - menu
    NO_KEY,	// 5E - hz		есть E0,5E E0,DE - ACPI Power
    NO_KEY,	// 5F - hz		есть E0,5F E0,DF - ACPI Sleep
// $60
    NO_KEY,	// 60 - hz
    NO_KEY,	// 61 - hz
    NO_KEY,	// 62 - hz
    NO_KEY,	// 63 - hz		есть E0,63 E0,E3 - ACPI Wake
    
    NO_KEY,	// 64 - hz - DIK_F13	/* (NEC PC98) */
    NO_KEY,	// 65 - hz - DIK_F14	/* (NEC PC98) */	есть E0,65 E0,E5 - Brwsr Search
    NO_KEY,	// 66 - hz - DIK_F15	/* (NEC PC98) */	есть E0,66 E0,E6 - Brwsr Bookmarks
    NO_KEY,	// 67 - hz 					есть E0,67 E0,E7 - Brwsr Reflesh
// $68
    NO_KEY,	// 68 - hz		есть E0,68 E0,E8 - Brwsr Stop
    NO_KEY,	// 69 - hz		есть E0,69 E0,E9 - Brwsr Forward
    NO_KEY,	// 6A - hz		есть E0,6A E0,EA - Brwsr Backward
    NO_KEY,	// 6B - hz		есть E0,6B E0,EB - Apps MyComputer
    
    NO_KEY,	// 6C - hz		есть E0,6C E0,EC - Apps EMail
    NO_KEY,	// 6D - hz		есть E0,6D E0,ED - Apps Media Select
    NO_KEY,	// 6E - hz
    NO_KEY,	// 6F - hz
// $70
    NO_KEY,	// 70 - hz - DIK_KANA		/* (Japanese keyboard) */
    NO_KEY,	// 71 - hz
    NO_KEY,	// 72 - hz
    NO_KEY,	// 73 - hz - DIK_ABNT_C1	/* /? on Brazilian keyboard */
    
    NO_KEY,	// 74 - hz
    NO_KEY,	// 75 - hz
    NO_KEY,	// 76 - hz
    NO_KEY,	// 77 - hz
// $78
    NO_KEY,	// 78 - hz
    NO_KEY,	// 79 - hz - DIK_CONVERT	/* (Japanese keyboard) */
    NO_KEY,	// 7A - hz
    NO_KEY,	// 7B - hz - DIK_NOCONVERT	/* (Japanese keyboard) */
    
    NO_KEY,	// 7C - hz
    NO_KEY,	// 7D - hz - DIK_YEN		/* (Japanese keyboard) */
    NO_KEY,	// 7E - hz - DIK_ABNT_C2	/* Numpad . on Brazilian keyboard */
    NO_KEY,	// 7F - hz
    
// для XT это key off набор всех этих же клавишъ
// а тут, судя по всему, это кнопки с E0 префиксом
 
// $80
    NO_KEY,	// E0,00 - hz
    NO_KEY,	// E0,01 - hz
    NO_KEY,	// E0,02 - hz
    NO_KEY,	// E0,03 - hz
    
    NO_KEY,	// E0,04 - hz
    NO_KEY,	// E0,05 - hz
    NO_KEY,	// E0,06 - hz
    NO_KEY,	// E0,07 - hz
// $88
    NO_KEY,	// E0,08 - hz
    NO_KEY,	// E0,09 - hz
    NO_KEY,	// E0,0A - hz
    NO_KEY,	// E0,0B - hz
    
    NO_KEY,	// E0,0C - hz
    NO_KEY,	// E0,0D - hz - DIK_NUMPADEQUALS	/* = on numeric keypad (NEC PC98) */
    NO_KEY,	// E0,0E - hz
    NO_KEY,	// E0,0F - hz
// $90
    0x0115,//NS	// E0,10 E0,90		E0,15 E0,F0,15		DIK_PREVTRACK		Player Previous Track	(DIK_CIRCUMFLEX on Japanese keyboard)
    NO_KEY,	// E0,11 - hz - DIK_AT		/* (NEC PC98) */
    NO_KEY,	// E0,12 - hz - DIK_COLON	/* (NEC PC98) */
    NO_KEY,	// E0,13 - hz - DIK_UNDERLINE	/* (NEC PC98) */
    
    NO_KEY,	// E0,14 - hz - DIK_KANJI	/* (Japanese keyboard) */
    NO_KEY,	// E0,15 - hz - DIK_STOP	/* (NEC PC98) */
    NO_KEY,	// E0,16 - hz - DIK_AX		/* (Japan AX) */
    NO_KEY,	// E0,17 - hz - DIK_UNLABELED	/* (J3100) */
// $98
    NO_KEY,	// E0,18 - hz
    0x014D,//NS	// E0,19 E0,99		E0,4D E0,F0,4D		DIK_NEXTTRACK		Player Next Track
    NO_KEY,	// E0,1A - hz
    NO_KEY,	// E0,1B - hz
    
    0x015A,	// E0,1C E0,9C		E0,5A E0,F0,5A		DIK_NUMPADENTER		Num Enter
    0x0114,	// E0,1D E0,9D 		E0,14 E0,F0,14 		DIK_RCONTROL    	rCtrl    
    NO_KEY,	// E0,1E - hz
    NO_KEY,	// E0,1F - hz
// $A0
    0x0123,//NS	// E0,20 E0,A0		E0,23 E0,F0,23 		DIK_MUTE		Player Mute
    0x012B,//NS	// E0,21 E0,A1		E0,2B E0,F0,2B		DIK_CALCULATOR		App Calculator
    0x0134,//NS	// E0,22 E0,A2		E0,34 E0,F0,34		DIK_PLAYPAUSE		Player Pause
    NO_KEY,	// E0,23 - hz
    
    0x013B,//NS	// E0,24 E0,A4		E0,3B E0,F0,3B		DIK_MEDIASTOP		Player Stop
    NO_KEY,	// E0,25 - hz
    NO_KEY,	// E0,26 - hz
    NO_KEY,	// E0,27 - hz
// $A8
    NO_KEY,	// E0,28 - hz
    NO_KEY,	// E0,29 - hz
    NO_KEY,	// E0,2A,E0,37 E0,B7,E0,AA	E0,12,E0,7C E0,F0,7C,E0,F0,12		PrintScreen	!!!
    NO_KEY,	// E0,2B - hz
    
    NO_KEY,	// E0,2C - hz
    NO_KEY,	// E0,2D - hz
    0x0121,//NS	// E0,2E E0,AE		E0,21 E0,F0,21		DIK_VOLUMEDOWN		Player Volume -
    NO_KEY,	// E0,2F - hz
// $B0
    0x0132,//NS	// E0,30 E0,B0		E0,32 E0,F0,32		DIK_VOLUMEUP		Player Volume +
    NO_KEY,	// E0,31 - hz
    0x013A,//NS	// E0,32 E0,B2		E0,3A E0,F0,3A		DIK_WEBHOME		Web Home
    NO_KEY,	// E0,33 - hz - DIK_NUMPADCOMMA	/* , on numeric keypad (NEC PC98) */
    
    NO_KEY,	// E0,34 - hz
    0x014A,	// E0,35 E0,B5		E0,4A E0,F0,4A		DIK_DIVIDE		Num /
    NO_KEY,	// E0,36 - hz
    0x0112,//??	// E0,37 E0,B7 - DIK_SYSRQ - неизвестно что генерирует неизвестно что !!!! [NS]  
// $B8
    0x0111,	// E0,38 E0,B8		E0,11 E0,F0,11		DIK_RMENU		rAlt
    NO_KEY,	// E0,39 - hz
    NO_KEY,	// E0,3A - hz
    NO_KEY,	// E0,3B - hz
    
    NO_KEY,	// E0,3C - hz
    NO_KEY,	// E0,3D - hz
    NO_KEY,	// E0,3E - hz
    NO_KEY,	// E0,3F - hz
// $C0
    NO_KEY,	// E0,40 - hz
    NO_KEY,	// E0,41 - hz
    NO_KEY,	// E0,42 - hz
    NO_KEY,	// E0,43 - hz
    
    NO_KEY,	// E0,44 - hz
    NO_KEY,	// E1,1D,45, E1,9D,C5	E1,14,77, E1,F0,14,F0,77  DIK_PAUSE		Pause	!!!! [NS]
    NO_KEY,	// E0,46 E0,C6		ctrl+break ctrl+pause
    0x016C,	// E0,47 E0,C7		E0,6C E0,F0,6C 		DIK_HOME		Home on arrow keypad ???
// $C8
    0x0175,	// E0,48 E0,C8		E0,75 E0,F0,75		DIK_UP			Up on arrow keypad
    0x017D,	// E0,49 E0,C9		E0,7D E0,F0,7D 		DIK_PRIOR		PgUp on arrow keypad
    NO_KEY,	// E0,4A - hz
    0x016B,	// E0,4B E0,CB		E0,6B E0,F0,6B		DIK_LEFT		Left on arrow keypad
    
    NO_KEY,	// E0,4C - hz
    0x0174,	// E0,4D E0,CD		E0,74 E0,F0,74		DIK_RIGHT		Right on arrow keypad
    NO_KEY,	// E0,4E - hz
    0x0169,	// E0,4F E0,CF		E0,69 E0,F0,69		DIK_END			End on arrow keypad
// $D0
    0x0172,	// E0,50 E0,D0		E0,72 E0,F0,72		DIK_DOWN		Down on arrow keypad
    0x017A,	// E0,51 E0,D1		E0,7A E0,F0,7A		DIK_NEXT		PgDn on arrow keypad
    0x0170,	// E0,52 E0,D2		E0,70 E0,F0,70		DIK_INSERT		Insert on arrow keypad
    0x0171,	// E0,53 E0,D3 		E0,71 E0,F0,71		DIK_DELETE		Delete on arrow keypad
    
    NO_KEY,	// E0,54 - hz
    NO_KEY,	// E0,55 - hz
    NO_KEY,	// E0,56 - hz
    NO_KEY,	// E0,57 - hz
// $D8
    NO_KEY,	// E0,58 - hz
    NO_KEY,	// E0,59 - hz
    NO_KEY,	// E0,5A - hz
    0x011F,	// E0,5B E0,DB		E0,1F E0,F0,1F		DIK_LWIN		Left Windows key
    
    0x0127,	// E0,5C E0,DC		E0,27 E0,F0,27		DIK_RWIN		Right Windows key
    0x012F,	// E0,5D E0,DD		E0,2F E0,F0,2F		DIK_APPS		AppMenu key
    0x0137,	// E0,5E E0,DE		E0,37 E0,F0,37 		DIK_POWER		ACPI Power
    0x013F,	// E0,5F E0,DF		E0,3F E0,F0,3F		DIK_SLEEP		ACPI Sleep
// $E0
    NO_KEY,	// E0,60 - hz
    NO_KEY,	// E0,61 - hz
    NO_KEY,	// E0,62 - hz
    0x015E,	// E0,63 E0,E3		E0,5E E0,F0,5E		DIK_WAKE		ACPI Wake
    
    NO_KEY,	// E0,64 - hz
    0x0110,//NS	// E0,65 E0,E5		E0,10 E0,F0,10		DIK_WEBSEARCH		Web Search
    0x0118,//NS	// E0,66 E0,E6 		E0,18 E0,F0,18		DIK_WEBFAVORITES	Web Favorites bookmarks?
    0x0120,//NS	// E0,67 E0,E7		E0,20 E0,F0,20 		DIK_WEBREFRESH		Web Refresh
// $E8
    0x0128,//NS	// E0,68 E0,E8	 	E0,28 E0,F0,28 		DIK_WEBSTOP		Web Stop
    0x0130,//NS	// E0,69 E0,E9		E0,30 E0,F0,30		DIK_WEBFORWARD		Web Forward
    0x0138,//NS	// E0,6A E0,EA 		E0,38 E0,F0,38		DIK_WEBBACK		Web Backward
    0x0140,//NS	// E0,6B E0,EB		E0,40 E0,F0,40		DIK_MYCOMPUTER		App My Computer
    
    0x0148,//NS	// E0,6C E0,EC		E0,48 E0,F0,48		DIK_MAIL		App Mail
    0x0150,//NS	// E0,6D E0,ED		E0,50 E0,F0,50		DIK_MEDIASELECT		App Media Select
    NO_KEY,	// E0,6E - hz
    NO_KEY,	// E0,6F - hz
// $F0
    NO_KEY,	// E0,70 - hz
    NO_KEY,	// E0,71 - hz
    NO_KEY,	// E0,72 - hz
    NO_KEY,	// E0,73 - hz
    
    NO_KEY,	// E0,74 - hz
    NO_KEY,	// E0,75 - hz
    NO_KEY,	// E0,76 - hz
    NO_KEY,	// E0,77 - hz
// $F8
    NO_KEY,	// E0,78 - hz
    NO_KEY,	// E0,79 - hz
    NO_KEY,	// E0,7A - hz
    NO_KEY,	// E0,7B - hz
    
    NO_KEY,	// E0,7C - hz
    NO_KEY,	// E0,7D - hz
    NO_KEY,	// E0,7E - hz
    NO_KEY	// E0,7F - hz
};			//NEDOREPO
//=============================================================================



//=============================================================================
// Таблица для конвертации из виндового XT -> ATM 6.20 XT 		[NS]
// В АТМ зачем то чтение через Ж (задом на перед)
// d7...d0 -> d0...d7
// в итоге скан коды вывернуты на изнанку

const unsigned short atm620_xt_keyb_dik_scan[256] =		
{   						//			
//  & x1xx - добавляет к сканкоду E0 xx

		// XT		key		key
// $00
    NO_KEY,//0x0000,	// 00
    0x0080,		// 01 81	DIK_ESCAPE	Esc
    0x0040,		// 02 82	DIK_1		1 !
    0x00C0,		// 03 83	DIK_2		2 @  
    
    0x0020,		// 04 84 	DIK_3		3 #        
    0x00A0,		// 05 85 	DIK_4		4 $      
    0x0060,		// 06 86 	DIK_5		5 %           
    0x00E0,		// 07 87 	DIK_6		6 ^
// $08	
    0x0010,		// 08 88 	DIK_7		7 &           
    0x0090,		// 09 89 	DIK_8		8 *         
    0x0050,		// 0A 8A 	DIK_9		9 (    
    0x00D0,		// 0B 8B 	DIK_0		0 )
	
    0x0030,		// 0C 8C 	DIK_MINUS	- _
    0x00B0,		// 0D 82 	DIK_EQUALS	= +     
    0x0070,		// 0E 8E 	DIK_BACK	Backspace
    0x00F0,		// 0F 8F 	DIK_TAB		Tab     
// $10	
    0x0008,		// 10 90 	DIK_Q		q Q      
    0x0088,		// 11 91 	DIK_W		w W
    0x0048,		// 12 92 	DIK_E		e E    
    0x00C8,		// 13 93 	DIK_R		r R
	
    0x0028,		// 14 94 	DIK_T		t T       
    0x00A8,		// 15 95 	DIK_Y		y Y            
    0x0068,		// 16 96 	DIK_U		u U
    0x00E8,		// 17 97 	DIK_I		i I
// $18	
    0x0018,		// 18 98 	DIK_O		o O
    0x0098,		// 19 99 	DIK_P		p P
    0x0058,		// 1A 9A 	DIK_LBRACKET	[ {
    0x00D8,		// 1B 9B 	DIK_RBRACKET	] }
    
    0x0038,		// 1C 9C 	DIK_RETURN	Enter
    0x00B8,		// 1D 9D 	DIK_LCONTROL	lCtrl
    0x0078,		// 1E 9E 	DIK_A		a A
    0x00F8,		// 1F 9F 	DIK_S		s S
// $20	
    0x0004,		// 20 A0 	DIK_D		d D
    0x0084,		// 21 A1 	DIK_F		f F       
    0x0044,		// 22 A2 	DIK_G		g G
    0x00C4,		// 23 A3 	DIK_H		h H
	
    0x0024,		// 24 A4 	DIK_J		j J
    0x00A4,		// 25 A5 	DIK_K		k K
    0x0064,		// 26 A6 	DIK_L		l L
    0x00E4,		// 27 A7 	DIK_SEMICOLON	; :
// $28	
    0x0014,		// 28 A8 	DIK_APOSTROPHE	' "
    0x0094,		// 29 89 	DIK_GRAVE	` ~
    0x0054,		// 2A AA 	DIK_LSHIFT	lShift
    0x00D4,		// 2B AB 	DIK_BACKSLASH	\ |
	
    0x0034,		// 2C AC 	DIK_Z		z Z
    0x00B4,		// 2D AD 	DIK_X		x X
    0x0074,		// 2E AE 	DIK_C		c C
    0x00F4,		// 2F AF 	DIK_V		v V
// $30	
    0x000C,		// 30 B0 	DIK_B		b B
    0x008C,		// 31 B1 	DIK_N		n N
    0x004C,		// 32 B2 	DIK_M		m M
    0x00CC,		// 33 B3 	DIK_COMMA	, <
    
    0x002C,		// 34 B4 	DIK_PERIOD	. >
    0x00AC,		// 35 B5 	DIK_SLASH	/ ?
    0x006C,		// 36 B6 	DIK_RSHIFT	rShift
    0x00EC,		// 37 B7 	DIK_MULTIPLY	Num *
// $38	
    0x001C,		// 38 B8 	DIK_LMENU	lAlt
    0x009C,		// 39 B9 	DIK_SPACE	Space
    0x005C,		// 3A BA 	DIK_CAPITAL	CapsLock
    0x00DC,		// 3B BB 	DIK_F1		F1
	
    0x003C,		// 3C BC 	DIK_F2		F2
    0x00BC,		// 3D BD 	DIK_F3		F3
    0x007C,		// 3E BE 	DIK_F4		F4
    0x00FC,		// 3F BF 	DIK_F5		F5
// $40	
    0x0002,		// 40 C0 	DIK_F6		F6
    0x0082,		// 41 C1 	DIK_F7		F7
    0x0042,		// 42 C2 	DIK_F8		F8
    0x00C2,		// 43 C3 	DIK_F9		F9
	
    0x0022,		// 44 C4 	DIK_F10		F10
    0x00A2,		// 45 C5 	DIK_NUMLOCK	NumLock
    0x0062,		// 46 C6 	DIK_SCROLL	ScrollLock
    0x00E2,		// 47 C7 	DIK_NUMPAD7	Num 7
// $48	
    0x0012,		// 48 C8 	DIK_NUMPAD8	Num 8
    0x0092,		// 49 C9 	DIK_NUMPAD9     Num 9    
    0x0052,		// 4A CA 	DIK_SUBTRACT	Num -
    0x00D2,		// 4B CB 	DIK_NUMPAD4	Num 4
	
    0x0032,		// 4C CC 	73 F0,73 	DIK_NUMPAD5	Num 5
    0x00B2,		// 4D CD 	74 F0,74	DIK_NUMPAD6	Num 6
    0x0072,		// 4E CE 	79 F0,79 	DIK_ADD		Num +
    0x00F2,		// 4F CF 	69 F0,69 	DIK_NUMPAD1     Num 1
// $50
    0x000A,		// 50 D0 	72 F0,72	DIK_NUMPAD2	Num 2
    0x008A,		// 51 D1 	7A F0,7A 	DIK_NUMPAD3	Num 3
    0x004A,		// 52 D2 	70 F0,70 	DIK_NUMPAD0	Num 0
    0x00CA,		// 53 D3 	71 F0,71 	DIK_DECIMAL	Num .
	
    NO_KEY,//0x002A,	// 54 - ALT + PrintScreen (код для клавиши Alt генерируется отдельно)
    NO_KEY,//0x00AA,	// 55 - hz
    NO_KEY,//0x006A,	// 56 - hz - DIK_OEM_102	/* <> or \| on RT 102-key keyboard (Non-U.S.) */
    0x00EA,		// 57 D7	78 F0,78 	DIK_F11		F11
// $58	
    0x001A,		// 58 D8 	07 F0,07	DIK_F12		F12
    NO_KEY,//0x009A,	// 59 - hz
    NO_KEY,//0x005A,	// 5A - hz
    NO_KEY,//0x00DA,	// 5B - hz
	
    NO_KEY,//0x003A,	// 5C - hz
    NO_KEY,//0x00BA,	// 5D - hz
    NO_KEY,//0x007A,	// 5E - hz
    NO_KEY,//0x00FA,	// 5F - hz
// $60	
    NO_KEY,//0x0006,	// 60 - hz
    NO_KEY,//0x0086,	// 61 - hz
    NO_KEY,//0x0046,	// 62 - hz
    NO_KEY,//0x00C6,	// 63 - hz
	
    NO_KEY,//0x0026,	// 64 - hz - DIK_F13	/* (NEC PC98) */
    NO_KEY,//0x00A6,	// 65 - hz - DIK_F14	/* (NEC PC98) */	есть E0,65 E0,E5 - Brwsr Search
    NO_KEY,//0x0066,	// 66 - hz - DIK_F15	/* (NEC PC98) */	есть E0,66 E0,E6 - Brwsr Bookmarks
    NO_KEY,//0x00E6,	// 67 - hz 					есть E0,67 E0,E7 - Brwsr Reflesh
// $68	
    NO_KEY,//0x0016,	// 68 - hz
    NO_KEY,//0x0096,	// 69 - hz
    NO_KEY,//0x0056,	// 6A - hz
    NO_KEY,//0x00D6,	// 6B - hz
    
    NO_KEY,//0x0036,	// 6C - hz
    NO_KEY,//0x00B6,	// 6D - hz
    NO_KEY,//0x0076,	// 6E - hz
    NO_KEY,//0x00F6,	// 6F - hz
// $70	
    NO_KEY,//0x000E,	// 70 - hz - DIK_KANA		/* (Japanese keyboard) */
    NO_KEY,//0x008E,	// 71 - hz
    NO_KEY,//0x004E,	// 72 - hz
    NO_KEY,//0x00CE,	// 73 - hz - DIK_ABNT_C1	/* /? on Brazilian keyboard */
	
    NO_KEY,//0x002E,	// 74 - hz
    NO_KEY,//0x00AE,	// 75 - hz
    NO_KEY,//0x006E,	// 76 - hz
    NO_KEY,//0x00EE,	// 77 - hz
// $78	
    NO_KEY,//0x001E,	// 78 - hz
    NO_KEY,//0x009E,	// 79 - hz - DIK_CONVERT	/* (Japanese keyboard) */
    NO_KEY,//0x005E,	// 7A - hz
    NO_KEY,//0x00DE,	// 7B - hz - DIK_NOCONVERT	/* (Japanese keyboard) */

    NO_KEY,//0x003E,	// 7C - hz
    NO_KEY,//0x00BE,	// 7D - hz - DIK_YEN		/* (Japanese keyboard) */
    NO_KEY,//0x007E,	// 7E - hz - DIK_ABNT_C2	/* Numpad . on Brazilian keyboard */
    NO_KEY,//0x00FE,	// 7F - hz
	
// для XT это key off набор всех этих же клавишъ
// а тут, судя по всему, это кнопки с E0 префиксом
 
// $80
    NO_KEY,//0x0100,	// E0,00 - hz
    NO_KEY,//0x0180,	// E0,01 - hz
    NO_KEY,//0x0140,	// E0,02 - hz
    NO_KEY,//0x01C0,	// E0,03 - hz

    NO_KEY,//0x0120,	// E0,04 - hz
    NO_KEY,//0x01A0,	// E0,05 - hz
    NO_KEY,//0x0160,	// E0,06 - hz
    NO_KEY,//0x01E0,	// E0,07 - hz
// $88	
    NO_KEY,//0x0110,	// E0,08 - hz
    NO_KEY,//0x0190,	// E0,09 - hz
    NO_KEY,//0x0150,	// E0,0A - hz
    NO_KEY,//0x01D0,	// E0,0B - hz
	
    NO_KEY,//0x0130,	// E0,0C - hz
    NO_KEY,//0x01B0,	// E0,0D - hz - DIK_NUMPADEQUALS	/* = on numeric keypad (NEC PC98) */
    NO_KEY,//0x0170,	// E0,0E - hz
    NO_KEY,//0x01F0,	// E0,0F - hz
// $90	
    0x0108,//NS		// E0,10 E0,90		E0,15 E0,F0,15		DIK_PREVTRACK		Player Previous Track	(DIK_CIRCUMFLEX on Japanese keyboard)
    NO_KEY,//0x0188,	// E0,11 - hz - DIK_AT		/* (NEC PC98) */
    NO_KEY,//0x0148,	// E0,12 - hz - DIK_COLON	/* (NEC PC98) */
    NO_KEY,//0x01C8,	// E0,13 - hz - DIK_UNDERLINE	/* (NEC PC98) */
	
    NO_KEY,//0x0128,	// E0,14 - hz - DIK_KANJI	/* (Japanese keyboard) */
    NO_KEY,//0x01A8,	// E0,15 - hz - DIK_STOP	/* (NEC PC98) */
    NO_KEY,//0x0168,	// E0,16 - hz - DIK_AX		/* (Japan AX) */
    NO_KEY,//0x01E8,	// E0,17 - hz - DIK_UNLABELED	/* (J3100) */
// $98
    NO_KEY,//0x0118,	// E0,18 - hz
    0x0198,//NS		// E0,19 E0,99		E0,4D E0,F0,4D		DIK_NEXTTRACK		Player Next Track
    NO_KEY,//0x0158,	// E0,1A - hz
    NO_KEY,//0x01D8,	// E0,1B - hz
	
    0x0138,		// E0,1C E0,9C		E0,5A E0,F0,5A		DIK_NUMPADENTER		Num Enter
    0x01B8,		// E0,1D E0,9D 		E0,14 E0,F0,14 		DIK_RCONTROL    	rCtrl    
    NO_KEY,//0x0178,	// E0,1E - hz
    NO_KEY,//0x01F8,	// E0,1F - hz
// $A0	
    0x0104,//NS		// E0,20 E0,A0		E0,23 E0,F0,23 		DIK_MUTE		Player Mute
    0x0184,//NS		// E0,21 E0,A1		E0,2B E0,F0,2B		DIK_CALCULATOR		App Calculator
    0x0144,//NS		// E0,22 E0,A2		E0,34 E0,F0,34		DIK_PLAYPAUSE		Player Pause
    NO_KEY,//0x01C4,	// E0,23 - hz
	
    0x0124,//NS		// E0,24 E0,A4		E0,3B E0,F0,3B		DIK_MEDIASTOP		Player Stop
    NO_KEY,//0x01A4,	// E0,25 - hz
    NO_KEY,//0x0164,	// E0,26 - hz
    NO_KEY,//0x01E4,	// E0,27 - hz
// $A8	
    NO_KEY,//0x0114,	// E0,28 - hz
    NO_KEY,//0x0194,	// E0,29 - hz
    0x0154,		// E0,2A,E0,37 E0,B7,E0,AA	E0,12,E0,7C E0,F0,7C,E0,F0,12		PrintScreen	!!!
    NO_KEY,//0x01D4,	// E0,2B - hz
	
    NO_KEY,//0x0134,	// E0,2C - hz
    NO_KEY,//0x01B4,	// E0,2D - hz
    0x0174,//NS		// E0,2E E0,AE		E0,21 E0,F0,21		DIK_VOLUMEDOWN		Player Volume -
    NO_KEY,//0x01F4,	// E0,2F - hz
// $B0	
    0x010C,//NS		// E0,30 E0,B0		E0,32 E0,F0,32		DIK_VOLUMEUP		Player Volume +
    NO_KEY,//0x018C,	// E0,31 - hz
    0x014C,//NS		// E0,32 E0,B2		E0,3A E0,F0,3A		DIK_WEBHOME		Web Home
    NO_KEY,//0x01CC,	// E0,33 - hz - DIK_NUMPADCOMMA	/* , on numeric keypad (NEC PC98) */
	
    NO_KEY,//0x012C,	// E0,34 - hz
    0x01AC,		// E0,35 E0,B5		E0,4A E0,F0,4A		DIK_DIVIDE		Num /
    NO_KEY,//0x016C,	// E0,36 - hz
    0x01EC,//??		// E0,37 E0,B7 - DIK_SYSRQ - неизвестно что генерирует неизвестно что !!!! [NS]  
// $B8
    0x011C,		// E0,38 E0,B8		E0,11 E0,F0,11		DIK_RMENU		rAlt
    NO_KEY,//0x019C,	// E0,39 - hz
    NO_KEY,//0x015C,	// E0,3A - hz
    NO_KEY,//0x01DC,	// E0,3B - hz
	
    NO_KEY,//0x013C,	// E0,3C - hz
    NO_KEY,//0x01BC,	// E0,3D - hz
    NO_KEY,//0x017C,	// E0,3E - hz
    NO_KEY,//0x01FC,	// E0,3F - hz
// $C0	
    NO_KEY,//0x0102,	// E0,40 - hz
    NO_KEY,//0x0182,	// E0,41 - hz
    NO_KEY,//0x0142,	// E0,42 - hz
    NO_KEY,//0x01C2,	// E0,43 - hz
	
    NO_KEY,//0x0122,	// E0,44 - hz
    0x01A2,		// E1,1D,45, E1,9D,C5	E1,14,77, E1,F0,14,F0,77  DIK_PAUSE		Pause	!!!! [NS]
    0x0162,		// E0,46 E0,C6		ctrl+break ctrl+pause
    0x01E2,		// E0,47 E0,C7		E0,6C E0,F0,6C 		DIK_HOME		Home on arrow keypad ???
// $C8	
    0x0112,		// E0,48 E0,C8		E0,75 E0,F0,75		DIK_UP			Up on arrow keypad
    0x0192,		// E0,49 E0,C9		E0,7D E0,F0,7D 		DIK_PRIOR		PgUp on arrow keypad
    NO_KEY,//0x0152,	// E0,4A - hz
    0x01D2,		// E0,4B E0,CB		E0,6B E0,F0,6B		DIK_LEFT		Left on arrow keypad
	
    NO_KEY,//0x0132,	// E0,4C - hz
    0x01B2,		// E0,4D E0,CD		E0,74 E0,F0,74		DIK_RIGHT		Right on arrow keypad
    NO_KEY,//0x0172,	// E0,4E - hz
    0x01F2,		// E0,4F E0,CF		E0,69 E0,F0,69		DIK_END			End on arrow keypad
// $D0	
    0x010A,		// E0,50 E0,D0		E0,72 E0,F0,72		DIK_DOWN		Down on arrow keypad
    0x018A,		// E0,51 E0,D1		E0,7A E0,F0,7A		DIK_NEXT		PgDn on arrow keypad
    0x014A,		// E0,52 E0,D2		E0,70 E0,F0,70		DIK_INSERT		Insert on arrow keypad
    0x01CA,		// E0,53 E0,D3 		E0,71 E0,F0,71		DIK_DELETE		Delete on arrow keypad
	
    NO_KEY,//0x012A,	// E0,54 - hz
    NO_KEY,//0x01AA,	// E0,55 - hz
    NO_KEY,//0x016A,	// E0,56 - hz
    NO_KEY,//0x01EA,	// E0,57 - hz
// $D8	
    NO_KEY,//0x011A,	// E0,58 - hz
    NO_KEY,//0x019A,	// E0,59 - hz
    NO_KEY,//0x015A,	// E0,5A - hz
    0x01DA,		// E0,5B E0,DB		E0,1F E0,F0,1F		DIK_LWIN		Left Windows key
    
    0x013A,		// E0,5C E0,DC		E0,27 E0,F0,27		DIK_RWIN		Right Windows key
    0x01BA,		// E0,5D E0,DD		E0,2F E0,F0,2F		DIK_APPS		AppMenu key
    0x017A,		// E0,5E E0,DE		E0,37 E0,F0,37 		DIK_POWER		ACPI Power
    0x01FA,		// E0,5F E0,DF		E0,3F E0,F0,3F		DIK_SLEEP		ACPI Sleep
// $E0	
    NO_KEY,//0x0106,	// E0,60 - hz
    NO_KEY,//0x0186,	// E0,61 - hz
    NO_KEY,//0x0146,	// E0,62 - hz
    0x01C6,		// E0,63 E0,E3		E0,5E E0,F0,5E		DIK_WAKE		ACPI Wake
	
    NO_KEY,//0x0126,	// E0,64 - hz
    0x01A6,//NS		// E0,65 E0,E5		E0,10 E0,F0,10		DIK_WEBSEARCH		Web Search
    0x0166,//NS		// E0,66 E0,E6 		E0,18 E0,F0,18		DIK_WEBFAVORITES	Web Favorites bookmarks?
    0x01E6,//NS		// E0,67 E0,E7		E0,20 E0,F0,20 		DIK_WEBREFRESH		Web Refresh
// $E8	
    0x0116,//NS		// E0,68 E0,E8	 	E0,28 E0,F0,28 		DIK_WEBSTOP		Web Stop
    0x0196,//NS		// E0,69 E0,E9		E0,30 E0,F0,30		DIK_WEBFORWARD		Web Forward
    0x0156,//NS		// E0,6A E0,EA 		E0,38 E0,F0,38		DIK_WEBBACK		Web Backward
    0x01D6,//NS		// E0,6B E0,EB		E0,40 E0,F0,40		DIK_MYCOMPUTER		App My Computer
	
    0x0136,//NS		// E0,6C E0,EC		E0,48 E0,F0,48		DIK_MAIL		App Mail
    0x01B6,//NS		// E0,6D E0,ED		E0,50 E0,F0,50		DIK_MEDIASELECT		App Media Select
    NO_KEY,//0x0176,	// E0,6E - hz
    NO_KEY,//0x01F6,	// E0,6F - hz
// $F0	
    NO_KEY,//0x010E,	// E0,70 - hz
    NO_KEY,//0x018E,	// E0,71 - hz
    NO_KEY,//0x014E,	// E0,72 - hz
    NO_KEY,//0x01CE,	// E0,73 - hz
    
    NO_KEY,//0x012E,	// E0,74 - hz
    NO_KEY,//0x01AE,	// E0,75 - hz
    NO_KEY,//0x016E,	// E0,76 - hz
    NO_KEY,//0x01EE,	// E0,77 - hz
// $F8	
    NO_KEY,//0x011E,	// E0,78 - hz
    NO_KEY,//0x019E,	// E0,79 - hz
    NO_KEY,//0x015E,	// E0,7A - hz
    NO_KEY,//0x01DE,	// E0,7B - hz

    NO_KEY,//0x013E,	// E0,7C - hz
    NO_KEY,//0x01BE,	// E0,7D - hz
    NO_KEY,//0x017E,	// E0,7E - hz
    NO_KEY,//0x01FE	// E0,7F - hz
};			//NEDOREPO
//=============================================================================



//=============================================================================
const PC_KEY pc_layout[] =
{
    { DIK_1,		0x31,	0xB1 },
    { DIK_2,		0x32,	0xB2 },
    { DIK_3,		0x33,	0xB3 },
    { DIK_4,		0x34,	0xB4 },
    { DIK_5,		0x35,	0xB5 },
    { DIK_6,		0x45,	0xE5 },
    { DIK_7,		0x44,	0xC5 },
    { DIK_8,		0x43,	0xF5 },
    { DIK_9,		0x42,	0xC3 },
    { DIK_0,		0x41,	0xC2 },
    { DIK_MINUS, 	0xE4,	0xC1 }, // -_
    { DIK_EQUALS,	0xE2,	0xE3 }, // =+

    { DIK_Q,		0x21,	0x29 },
    { DIK_W,		0x22,	0x2A },
    { DIK_E,		0x23,	0x2B },
    { DIK_R,		0x24,	0x2C },
    { DIK_T,		0x25,	0x2D },
    { DIK_Y,		0x55,	0x5D },
    { DIK_U,		0x54,	0x5C },
    { DIK_I,		0x53,	0x5B },
    { DIK_O,		0x52,	0x5A },
    { DIK_P,		0x51,	0x59 },
//  { DIK_LBRACKET,	0xD5,	0x94 }, // [{
//  { DIK_RBRACKET,	0xD4,	0x95 }, // ]}

    { DIK_A,		0x11,	0x19 },
    { DIK_S,		0x12,	0x1A },
    { DIK_D,		0x13,	0x1B },
    { DIK_F,		0x14,	0x1C },
    { DIK_G,		0x15,	0x1D },
    { DIK_H,		0x65,	0x6D },
    { DIK_J,		0x64,	0x6C },
    { DIK_K,		0x63,	0x6B },
    { DIK_L,		0x62,	0x6A },
    { DIK_SEMICOLON,	0xD2,	0x82 }, // ;:
    { DIK_APOSTROPHE,	0xC4,	0xD1 }, // '"

    { DIK_Z,		0x02,	0x0A },
    { DIK_X,		0x03,	0x0B },
    { DIK_C,		0x04,	0x0C },
    { DIK_V,		0x05,	0x0D },
    { DIK_B,		0x75,	0x7D },
    { DIK_N,		0x74,	0x7C },
    { DIK_M,		0x73,	0x7B },
    { DIK_COMMA,	0xF4,	0xA4 }, // ,<
    { DIK_PERIOD,	0xF3,	0xA5 }, // .>
    { DIK_SLASH,	0x85,	0x84 }, // /?
    { DIK_BACKSLASH,	0x93,	0x92 }, // \|
};
//=============================================================================


const size_t pc_layout_count = _countof( pc_layout);



//=============================================================================
// таблица конвертации из вендовых VK_KEY-ев в вендовые? типо XT сканкоды
// Win32.hlp + педивикия + метод научного тыка в унриале		// [NS]
// не знаем есть ли конвертация средствами венды
// будем делать ручками
//
//	НЕ АКТУАЛЬНО
//	ТЕПЕРЬ КОНВЕРТИРУЕТСО НА ХОДУ ИЗ lParam
//	ЧТО ПО ИДЕИ БУДЕТ ПРОПУСКАТЬ И КНОПКИ КОТОРЫЕ НЕ УДАЛОСЬ ПРОВЕРИТЬ
/*
const unsigned char vk_key_2_xt_key_tab[ 256] =		
{
//printscreen и sysreq ннежмутсо вообще
// ПРИ ЭТОМ ЭВО ТЕСТ ВИДИТ ПРИНТСКРИН КАК E0 12
// при этом эвотест не видит паузу и бреак

	NO_KEY,		//		00
	NO_KEY,		// VK_LBUTTON	01	Left mouse button 
	NO_KEY,		// VK_RBUTTON	02	Right mouse button 
	0xC6, //unreal	// VK_CANCEL	03	Control-break processing 

	NO_KEY,		// VK_MBUTTON	04	Middle mouse button (three-button mouse) 
	NO_KEY,		//  ѕ 		05-07	Undefined 
	NO_KEY,		// 		06
	NO_KEY,		// 		07
	
	0x0E,		// VK_BACK	08	BACKSPACE key 
	0x0F,		// VK_TAB	09	TAB key 
	NO_KEY,		//  ѕ	 	0A-0B	Undefined 
	NO_KEY,		//		0B
	
	0x4C, //unreal "5"		// VK_CLEAR	0C	CLEAR key 
 NO_KEY, // cnflct 0x1C,		// VK_RETURN	0D	ENTER key 
	NO_KEY,		//  ѕ 		0E-0F	Undefined 
	NO_KEY,		//		0F
	
 NO_KEY, //0x2A, //LEFT	// VK_SHIFT	10	SHIFT key 
NO_KEY, //	0x1D, //LEFT	// VK_CONTROL	11	CTRL key 
 NO_KEY,//	0x38, //LEFT	// VK_MENU	12	ALT key 
	0xC5, //unreal	// VK_PAUSE	13	PAUSE key 
	
	0x3A,		// VK_CAPITAL	14	CAPS LOCK key 
	NO_KEY,		//  ѕ 		15-19	Reserved for Kanji systems 
	NO_KEY,		//		16
	NO_KEY,		//		17
	
	NO_KEY,		//		18
	NO_KEY,		//		19
	NO_KEY,		//  ѕ 		1A	Undefined 
	0x01,		// VK_ESCAPE	1B	ESC key 
	
	NO_KEY,		//  ѕ 		1C-1F	Reserved for Kanji systems 
	NO_KEY,		//		1D
	NO_KEY,		//		1E
	NO_KEY,		//		1F
	
	0x39,		// VK_SPACE	20	SPACEBAR 
	0xC9, //unreal	// VK_PRIOR	21	PAGE UP key 
	0xD1, //unreal	// VK_NEXT	22	PAGE DOWN key 
	0xCF, //unreal	// VK_END	23	END key 
	
	0xC7, //unreal	// VK_HOME	24	HOME key 
	0xCB, //unreal	// VK_LEFT	25	LEFT ARROW key 
	0xC8, //unreal	// VK_UP	26	UP ARROW key 
	0xCD, //unreal	// VK_RIGHT	27	RIGHT ARROW key 
	
	0xD0, //unreal	// VK_DOWN	28	DOWN ARROW key 
	NO_KEY,		// VK_SELECT	29	SELECT key 
	NO_KEY,		//  ѕ 	2A	Original equipment manufacturer (OEM) specific 
	NO_KEY,		// VK_EXECUTE	2B	EXECUTE key 
	
	0x2A, //?????	// VK_SNAPSHOT	2C	PRINT SCREEN key for Windows 3.0 and later 
 NO_KEY, //cnflct	// VK_INSERT	2D	INS key 
 NO_KEY,// 2E 6E cnflct	// VK_DELETE	2E	DEL key 
	NO_KEY,		// VK_HELP	2F	HELP key 
	
	0x0B,		// VK_0		30	0 key 
	0x02,		// VK_1		31	1 key 
	0x03,		// VK_2		32	2 key 
	0x04,		// VK_3		33	3 key 
	
	0x05,		// VK_4		34	4 key 
	0x06,		// VK_5		35	5 key 
	0x07,		// VK_6		36	6 key 
	0x08,		// VK_7		37	7 key 
	
	0x09,		// VK_8		38	8 key 
	0x0A,		// VK_9		39	9 key 
	NO_KEY,		//  ѕ 		3A-40	Undefined 
	NO_KEY,		//		3B
	
	NO_KEY,		//		3C
	NO_KEY,		//		3D
	NO_KEY,		//		3E
	NO_KEY,		//		3F
	
	NO_KEY,		//		40
	0x1E,		// VK_A		41	A key 
	0x30,		// VK_B		42	B key 
	0x2E,		// VK_C		43	C key 
	
	0x20,		// VK_D		44	D key 
	0x12,		// VK_E		45	E key 
	0x21,		// VK_F		46	F key 
	0x22,		// VK_G		47	G key
	
	0x23,		// VK_H		48	H key 
	0x17,		// VK_I		49	I key 
	0x24,		// VK_J		4A	J key 
	0x25,		// VK_K		4B	K key 
	
	0x26,		// VK_L		4C	L key 
	0x32,		// VK_M		4D	M key 
	0x31,		// VK_N		4E	N key 
	0x18,		// VK_O		4F	O key 
	
	0x19,		// VK_P		50	P key 
	0x10,		// VK_Q		51	Q key 
	0x13,		// VK_R		52	R key 
	0x1F,		// VK_S		53	S key 
	
	0x14,		// VK_T		54	T key 
	0x16,		// VK_U		55	U key 
	0x2F,		// VK_V		56	V key 
	0x11,		// VK_W		57	W key 
	
	0x2D,		// VK_X		58	X key 
	0x15,		// VK_Y		59	Y key 
	0x2C,		// VK_Z		5A	Z key 
	0xDB, //unreal	// VK_LWIN	5B	Left Windows key (Microsoft Natural Keyboard) 
	
	0x5C, //?????	// VK_RWIN	5C	Right Windows key (Microsoft Natural Keyboard) 
	0xDD, //unreal	// VK_APPS	5D	Applications key (Microsoft Natural Keyboard) 
	NO_KEY,		//  ѕ 		5E-5F	Undefined 
	NO_KEY,		//		5F
	
	0x52,		// VK_NUMPAD0	60	Numeric keypad 0 key 
	0x4F,		// VK_NUMPAD1	61	Numeric keypad 1 key 
	0x50,		// VK_NUMPAD2	62	Numeric keypad 2 key 
	0x51,		// VK_NUMPAD3	63	Numeric keypad 3 key 
	
	0x4B,		// VK_NUMPAD4	64	Numeric keypad 4 key 
	0x4C,		// VK_NUMPAD5	65	Numeric keypad 5 key 
	0x4D,		// VK_NUMPAD6	66	Numeric keypad 6 key 
	0x47,		// VK_NUMPAD7	67	Numeric keypad 7 key 
	
	0x48,		// VK_NUMPAD8	68	Numeric keypad 8 key 
	0x49,		// VK_NUMPAD9	69	Numeric keypad 9 key 
	0x37, //unreal	// VK_MULTIPLY	6A	Multiply key numpad
	0x4E, //unreal	// VK_ADD	6B	Add key 
	
	NO_KEY,		// VK_SEPARATOR	6C	Separator key 
	0x4A, //unreal	// VK_SUBTRACT	6D	Subtract key 
 NO_KEY, //2e 6e cnflct	// VK_DECIMAL	6E	Decimal key 
	0xB5, //unreal	// VK_DIVIDE	6F	Divide key   numpad
	
	0x3B,		// VK_F1	70	F1 key 
	0x3C,		// VK_F2	71	F2 key 
	0x3D,		// VK_F3	72	F3 key 
	0x3E,		// VK_F4	73	F4 key 
	
	0x3F,		// VK_F5	74	F5 key 
	0x40,		// VK_F6	75	F6 key 
	0x41,		// VK_F7	76	F7 key 
	0x42,		// VK_F8	77	F8 key 
	
	0x43,		// VK_F9	78	F9 key 
	0x44,		// VK_F10	79	F10 key 
	0x57,		// VK_F11	7A	F11 key 
	0x58,		// VK_F12	7B	F12 key 
	
	NO_KEY,		// VK_F13	7C	F13 key 
	NO_KEY,		// VK_F14	7D	F14 key 
	NO_KEY,		// VK_F15	7E	F15 key 
	NO_KEY,		// VK_F16	7F	F16 key 
	
	NO_KEY,		// VK_F17	80H	F17 key 
	NO_KEY,		// VK_F18	81H	F18 key 
	NO_KEY,		// VK_F19	82H	F19 key 
	NO_KEY,		// VK_F20	83H	F20 key 
	
	NO_KEY,		// VK_F21	84H	F21 key 
	NO_KEY,		// VK_F22	85H	F22 key 
	NO_KEY,		// VK_F23	86H	F23 key 
	NO_KEY,		// VK_F24	87H	F24 key 
	
	NO_KEY,		//  ѕ 		88-8F	Unassigned 
	NO_KEY,		//		89
	NO_KEY,		//		8A
	NO_KEY,		//		8B
	
	NO_KEY,		//		8C
	NO_KEY,		//		8D
	NO_KEY,		//		8E
	NO_KEY,		//		8F
	
	0x45, //unreal	// VK_NUMLOCK	90	NUM LOCK key 
	0x46, //?????	// VK_SCROLL	91	SCROLL LOCK key 
	NO_KEY,		//  ѕ		92-B9	Unassigned 
	NO_KEY,		//		93
	
	NO_KEY,		//		94
	NO_KEY,		//		95
	NO_KEY,		//		96
	NO_KEY,		//		97
	
	NO_KEY,		//		98
	NO_KEY,		//		99
	NO_KEY,		//		9A
	NO_KEY,		//		9B
	
	NO_KEY,		//		9C
	NO_KEY,		//		9D
	NO_KEY,		//		9E
	NO_KEY,		//		9F
	
	NO_KEY,		//		A0
	NO_KEY,		//		A1
	NO_KEY,		//		A2
	NO_KEY,		//		A3
	
	NO_KEY,		//		A4
	NO_KEY,		//		A5
	NO_KEY,		//		A6
	NO_KEY,		//		A7
	
	NO_KEY,		//		A8
	NO_KEY,		//		A9
	NO_KEY,		//		AA
	NO_KEY,		//		AB
	
	NO_KEY,		//		AC
	NO_KEY,		//		AD
	NO_KEY,		//		AE
	NO_KEY,		//		AF
	
	NO_KEY,		//		B0
	NO_KEY,		//		B1
	NO_KEY,		//		B2
	NO_KEY,		//		B3
	
	NO_KEY,		//		B4
	NO_KEY,		//		B5
	NO_KEY,		//		B6
	NO_KEY,		//		B7
	
	NO_KEY,		//		B8
	NO_KEY,		//		B9
	0x27,		//  ѕ		BA-C0	OEM specific  ";" ":"
	0x0D, //unreal	//		BB	= + не нумпад
	
	0x33, //unreal	//		BC	, <
	0x0C, //unreal	//		BD	- _ не нумпад
	0x34,		//		BE	. >
	0x35,		//		BF	/ ?
	
	0x29, //unrel		//		C0	` ~
	NO_KEY,		//  ѕ		C1-DA	Unassigned 
	NO_KEY,		//		C2
	NO_KEY,		//		C3
	
	NO_KEY,		//		C4
	NO_KEY,		//		C5
	NO_KEY,		//		C6
	NO_KEY,		//		C7
	
	NO_KEY,		//		C8
	NO_KEY,		//		C9
	NO_KEY,		//		CA
	NO_KEY,		//		CB
	
	NO_KEY,		//		CC
	NO_KEY,		//		CD
	NO_KEY,		//		CE
	NO_KEY,		//		CF
	
	NO_KEY,		//		D0
	NO_KEY,		//		D1
	NO_KEY,		//		D2
	NO_KEY,		//		D3
	
	NO_KEY,		//		D4
	NO_KEY,		//		D5
	NO_KEY,		//		D6
	NO_KEY,		//		D7
	
	NO_KEY,		//		D8
	NO_KEY,		//		D9
	NO_KEY,		//		DA
	0x1A, //unreal	//  ѕ		DB-E4	OEM specific 	[ {
	
	0x2B, //unreal	//		DC	\ |
	0x1B, //unreal	//		DD	] }
	0x28, //unreal	//		DE	-'- -"-
	NO_KEY,		//		DF
	
	NO_KEY,		//		E0
	NO_KEY,		//		E1
	NO_KEY,		//		E2
	NO_KEY,		//		E3
	
	NO_KEY,		//		E4
	NO_KEY,		//  ѕ		E5	Unassigned 
	NO_KEY,		//  ѕ		E6	OEM specific 
	NO_KEY,		//  ѕ		E7-E8	Unassigned 
	
	NO_KEY,		//		E8
	NO_KEY,		//  ѕ		E9-F5	OEM specific 
	NO_KEY,		//		EA
	NO_KEY,		//		EB
	
	NO_KEY,		//		EC
	NO_KEY,		//		ED
	NO_KEY,		//		EE
	NO_KEY,		//		EF
	
	NO_KEY,		//		F0
	NO_KEY,		//		F1
	NO_KEY,		//		F2
	NO_KEY,		//		F3
	
	NO_KEY,		//		F4
	NO_KEY,		//		F5
	NO_KEY,		// VK_ATTN	F6	Attn key
	NO_KEY,		// VK_CRSEL	F7	CrSel key
	
	NO_KEY,		// VK_EXSEL	F8	ExSel key
	NO_KEY,		// VK_EREOF	F9	Erase EOF key
	NO_KEY,		// VK_PLAY	FA	Play key
	NO_KEY,		// VK_ZOOM	FB	Zoom key
	
	NO_KEY,		// VK_NONAME	FC	Reserved for future use. 
	NO_KEY,		// VK_PA1	FD	PA1 key
	NO_KEY,		// VK_OEM_CLEAR	FE	Clear key
	NO_KEY,		//		FF
};
*/

//=============================================================================
