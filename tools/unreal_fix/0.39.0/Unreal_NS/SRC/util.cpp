#include "std.h"

#include "emul.h"
#include "vars.h"
#include "snapshot.h"
#include "init.h"

#include "util.h"

#include "emul_no_modal_windows.h"	// hwnd-ы немодальных окон [NS]


// для передачи кодов клавишь в качестве результата функции
int key_down_oem_key = 0;			// [NS]
unsigned char key_down_kbdpc_key = 0;	// [NS]
int key_down_v_key = 0;				// [NS]



//=============================================================================
static void cpuid( unsigned CpuInfo[ 4], unsigned _eax)
{
//-----------------------------------------------------------------------------
#ifdef _MSC_VER
    __cpuid((int *)CpuInfo, int(_eax));
#endif
//-----------------------------------------------------------------------------
#ifdef __GNUC__
#ifdef __clang__
    __cpuid((int *)CpuInfo, _eax);
#else
    __cpuid(_eax, CpuInfo[0], CpuInfo[1], CpuInfo[2], CpuInfo[3]);
#endif // __clang__
#endif
//-----------------------------------------------------------------------------
}
//=============================================================================


//=============================================================================
void fillCpuString( char dst[ 49])
{
    dst[0] = dst[12] = dst[48] = 0;
    unsigned CpuInfo[4];
    unsigned *d = (unsigned *)dst;

    cpuid( CpuInfo, 0x80000000);
    //-------------------------------------------------------------------------
    if (CpuInfo[0] < 0x80000004)
    {
	cpuid( CpuInfo, 0);
	d[0] = CpuInfo[1];
	d[1] = CpuInfo[3];
	d[2] = CpuInfo[2];
	return;
    }
    //-------------------------------------------------------------------------
    cpuid( CpuInfo, 0x80000002);
    d[ 0] = CpuInfo[0];
    d[ 1] = CpuInfo[1];
    d[ 2] = CpuInfo[2];
    d[ 3] = CpuInfo[3];
    //-------------------------------------------------------------------------
    cpuid( CpuInfo, 0x80000003);
    d[ 4] = CpuInfo[0];
    d[ 5] = CpuInfo[1];
    d[ 6] = CpuInfo[2];
    d[ 7] = CpuInfo[3];
    //-------------------------------------------------------------------------
    cpuid( CpuInfo, 0x80000004);
    d[ 8] = CpuInfo[0];
    d[ 9] = CpuInfo[1];
    d[10] = CpuInfo[2];
    d[11] = CpuInfo[3];
    //-------------------------------------------------------------------------
}
//=============================================================================


//=============================================================================
unsigned cpuid(unsigned _eax, int ext)
{
    unsigned CpuInfo[ 4];

    cpuid( CpuInfo, _eax);

    return ext    ?	CpuInfo[3] :
			CpuInfo[0];
}
//=============================================================================


//=============================================================================
unsigned __int64 GetCPUFrequency()
{
    LARGE_INTEGER Frequency;
    LARGE_INTEGER Start;
    LARGE_INTEGER Stop;
    unsigned long long c1;
    unsigned long long c2;

    QueryPerformanceFrequency( &Frequency);
    LONGLONG t = Frequency.QuadPart >> 3;
    c1 = rdtsc();
    QueryPerformanceCounter( &Start);
    //-------------------------------------------------------------------------
    do
    {
	QueryPerformanceCounter( &Stop);
    }
    while ((Stop.QuadPart - Start.QuadPart) < t);
    //-------------------------------------------------------------------------
    c2 = rdtsc();

    return (c2 - c1) << 3;
}
//=============================================================================


//=============================================================================
void trim(char *dst)
{
    size_t i = strlen(dst);
    //-------------------------------------------------------------------------
    // trim right spaces
    while (i && isspace( u8( dst[ i - 1])))
	i--;
    //-------------------------------------------------------------------------
    dst[i] = 0;
    //-------------------------------------------------------------------------
    // trim left spaces
    for (i = 0;   isspace( u8( dst[ i]));   i++);
    //-------------------------------------------------------------------------
    strcpy( dst, dst+i);
}
//=============================================================================

const char nop = 0;
const char * const nil = &nop;

//=============================================================================
int ishex(char c)
{
    return (isdigit( c) || (tolower( c) >= 'a' && tolower( c) <= 'f'));
}
//=============================================================================


//=============================================================================
unsigned char hex( char p)
{
    p = char( tolower( p));
    return u8(( p < 'a') ? p-'0' : p-'a'+10);
}
//=============================================================================


//=============================================================================
unsigned char hex( const char *p)
{
    return 0x10 * hex( p[0]) + hex( p[1]);
}
//=============================================================================


//=============================================================================
unsigned process_msgs()
{
    MSG msg;
    unsigned key = 0;		// для return V_KEY

    // глобальные переменные
    key_down_oem_key = 0;	// [NS]
    key_down_kbdpc_key = 0;	// [NS]
    key_down_v_key = 0;		// [NS]
	    
    //printf("msgs ");

    //-------------------------------------------------------------------------
    while (PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE))
    {
	
	// IsDialogMessage вроде какпроверяет сообщение ли это для нужного hwnd
	// и если для него то отправляет и возвращает TRUE
	//---------------------------------------------------------------------
	// немодальное окна бряков [NS]			//
	if (hwnd_bp_dialog != NULL)			// hwnd существует
	    if (IsDialogMessage( hwnd_bp_dialog, &msg))	// сообщение для hwnd
		continue;				// значит сообение не для main window !!!
	//---------------------------------------------------------------------
	// немодальное окна вачесов [NS]		//
	if (hwnd_watchdlg != NULL)			// hwnd существует
	    if (IsDialogMessage( hwnd_watchdlg, &msg))	// сообщение для hwnd
		continue;				// значит сообение не для main window !!!
	//---------------------------------------------------------------------
	
	
	
	/*
	if (msg.message == WM_NCLBUTTONDOWN)
	    continue;
	*/


	// можот добавить обработку паузы прямо сюда еще ?

// WM_LBUTTONDOWN WM_LBUTTONUP WM_LBUTTONDBLCLK
// WM_MBUTTONDOWN WM_MBUTTONUP WM_MBUTTONDBLCLK
// WM_RBUTTONDOWN WM_RBUTTONUP WM_RBUTTONDBLCLK


	//---------------------------------------------------------------------
	// mouse messages must be processed further
	if (msg.message == WM_LBUTTONDBLCLK)
	{
	    debug_mouse_keys |= DEBUG_MOUSE_LEFT_DBL;
	    mousepos = DWORD( msg.lParam);
	    key = VK_LMB;
	}
	//---------------------------------------------------------------------
	if (msg.message == WM_LBUTTONDOWN)
	{
	    debug_mouse_keys |= DEBUG_MOUSE_LEFT_ON;
	    mousepos = DWORD( msg.lParam);
	    key = VK_LMB;
	}
	//---------------------------------------------------------------------
	if (msg.message == WM_RBUTTONDOWN)
	{
	    debug_mouse_keys |= DEBUG_MOUSE_RIGHT_ON;
	    // видимо оно ТУПО ИГНОРИТ
	    // то что мыша может летать по координате 0x8000+
	    mousepos = DWORD( msg.lParam | 0x80000000);	// ВЫПИЛИТЬ НАХ!!!
	    key = VK_RMB;
	}
	//---------------------------------------------------------------------
	if (msg.message == WM_MBUTTONDOWN)
	{
	    mousepos = DWORD( msg.lParam | 0x80000000);	// ВЫПИЛИТЬ НАХ!!!
	    key = VK_MMB;
	}
	//---------------------------------------------------------------------
	// [vv] Process mouse whell only in client area
	if (msg.message == WM_MOUSEWHEEL) 
	{
	    //-----------------------------------------------------------------
	    POINT Pos =
	    {
		GET_X_LPARAM( msg.lParam),
		GET_Y_LPARAM( msg.lParam)
	    };
	    //-----------------------------------------------------------------
	    RECT ClientRect;
	    GetClientRect( msg.hwnd, &ClientRect);
	    MapWindowPoints( msg.hwnd, HWND_DESKTOP, (LPPOINT) &ClientRect, 2);
	    //-----------------------------------------------------------------
	    if (PtInRect( &ClientRect, Pos))
	    {
		key = GET_WHEEL_DELTA_WPARAM( msg.wParam) < 0  ?  VK_MWD :
								  VK_MWU;
		//printf("whell %x\n",key);
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	// WM_KEYDOWN and WM_SYSKEYDOWN must not be dispatched,
	// bcoz window will be closed on alt-f4
	if ((msg.message == WM_KEYDOWN) || (msg.message == WM_SYSKEYDOWN))
	{
	    //-----------------------------------------------------------------
	    // wparam
	    //     V_KEY
	    //-----------------------------------------------------------------
	    // lparam
	    //     d31 - Transition State		0x8000
	    //     d30 - Previos Key State		0x4000
	    //     d29 - Context Code			0x2000
	    //               1 - alt pressed
	    //     d28...d25				
	    //     d24 - Extended Key Flag		0x100
	    //     d23...d16 - 8-Bit OEM Scan Code
	    //     d15...d0 - 16-Bit Repeat Count
	    //-----------------------------------------------------------------
	    if (conf.atm.xt_kbd)
	    {
		
		input.atm51.setkey( unsigned( msg.lParam >> 16), 1);
	    }
	    //	тобешь щас xt клава вообще не работает без глобального отключения хоткеев !!!!
	    //  что есть не пральна?
	    //-----------------------------------------------------------------
	    switch ((msg.lParam >> 16) & 0x1FF)
	    {
		case 0x02A:	kbdpcEX[0] = (kbdpcEX[0] ^ 0x01) | 0x80;	break;	//XT LShift
		case 0x036:	kbdpcEX[1] = (kbdpcEX[1] ^ 0x01) | 0x80;	break;	//XT RShift
		case 0x01D:	kbdpcEX[2] = (kbdpcEX[2] ^ 0x01) | 0x80;	break;	//XT LCtrl
		case 0x11D:	kbdpcEX[3] = (kbdpcEX[3] ^ 0x01) | 0x80;	break;	//XT RCtrl ???
		case 0x038:	kbdpcEX[4] = (kbdpcEX[4] ^ 0x01) | 0x80;	break;	//XT LAlt
		case 0x138:	kbdpcEX[5] = (kbdpcEX[5] ^ 0x01) | 0x80;	break;	//XT RAlt ???
	    } //Dexus
	    //-----------------------------------------------------------------
//          printf("%s, WM_KEYDOWN, WM_SYSKEYDOWN\n", __FUNCTION__);
		//printf("V_KEY %02X\n",( msg.wParam));
		//printf("oem key %X\n",( msg.lParam >> 16));
	    key = unsigned( msg.wParam);
	    // тут был бы полезен oem scan code...
	    
	    // глобальные переменные
	    key_down_oem_key = (msg.lParam >> 16);	// [NS]
	    key_down_v_key = msg.wParam;		// [NS]
	    
		//  oem - kbdpc
		//
	    	// 4000 - АВТОПОВТОР
		// 2000 - ALT+
		// 0100 - ext key
		//
		//  152 - D2		insert
		//  153 - D3		del
		//   45 - C5		pause	(иссключениe)
		//  15B - DB		lwin
		//  15C - DC		rwin
		// 2038 - 38		lalt
		// 2138 - B8		ralt
		//  15D - DD		menu
		//  11D - 9D		rctrl
		//  149 - C9		pg up
		//  151 - D1		pg down
		//  14B - CB		left
		//  14D - CD		right
		//  148 - C8		up
		//  150 - D0		down
		//  145 - 45		numlock (иссключениe)
		//  135 - B5		num "/"
		//  11C - 9C		num enter
		//  147 - C7		home
		//  14F - CF		end
		//  146 - C6		break
		//
		//  119 - 99	player next
		//  110 - 90	player prew
		//  124 - A4	player stop
		//  122 - A2	player play\pause
		//
		// 
		//
		// иссключения
		// 45 -> С5 pause
		// 145 -> 45 numlock

		// конфликты
		// oem - wvk - kbdpc
		//  1D -  11 - 1D	lctrl (нормальный вариант)
		//
		// 11D -  11 - 9D	rctrl (нормальный вариант)
		//  E0 -  11 - 		rctrl (win osk)
		//
		// 145 -  90 - 45	numlock (нормальный вариант)
		//  45 -  90 - 45	numlock (win osk)
		//
		//  45 -  13 - C5	pause	(иссключениe)
		//  E1 -  13 - 		brk (win osk)
		//
		// 146 -  03 - C6	break (нормальный вариант)



	    //-----------------------------------------------------------------
	    // numlock (обычный?)
	    if ((key_down_oem_key & 0x1FF) == 0x145)		
	    {
		key_down_kbdpc_key = 0x45;	// исключение
	    }
	    //-----------------------------------------------------------------
	    // numlock (win osk)
	    else if (  	((key_down_oem_key & 0x1FF) == 0x045)	&&
			(key_down_v_key == 0x90) // дополнительная проверка
	       )
	    {
		key_down_kbdpc_key = 0x45;	// исключение
	    }
	    //-----------------------------------------------------------------
	    // pause (обычная?)
	    else if ((key_down_oem_key & 0x1FF) == 0x045)		
	    {
		key_down_kbdpc_key = 0xC5;	// исключение
	    }
	    //-----------------------------------------------------------------
	    // brk (win osk) вместо pause почему то
	    else if (	((key_down_oem_key & 0x1FF) == 0x0E1)	&&
			(key_down_v_key == 0x13) // дополнительная проверка
	       )
	    {
		key_down_kbdpc_key = 0xC5;	// исключение
	    }
	    //-----------------------------------------------------------------
	    // rctrl (win osk)
	    else if (	((key_down_oem_key & 0x1FF) == 0x0E0)	&&
			(key_down_v_key == 0x11) // дополнительная проверка
	       )
	    {
		key_down_kbdpc_key = 0x9D;	// исключение
		// E0 - все равно при этом проскакивает !!!
		// через readdevice()
	    }
	    //-----------------------------------------------------------------
	    // 0x1xx - ext key
	    else if (key_down_oem_key & 0x100)			
	    {
		key_down_kbdpc_key = key_down_oem_key | 0x80;
	    }
	    //-----------------------------------------------------------------
	    // 0x0xx - simple key
	    else						
	    {
		key_down_kbdpc_key = (key_down_oem_key & 0xFF);		//0x7F);
		// win osk вполне себе шлет всякие E0, E1
		// так что пусть будет 0xFF
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	else if ((msg.message == WM_KEYUP) || (msg.message == WM_SYSKEYUP))
	{
	    //-----------------------------------------------------------------
	    if (conf.atm.xt_kbd)
		input.atm51.setkey( unsigned( msg.lParam >> 16), 0);
	    //-----------------------------------------------------------------
	    switch (( msg.lParam >> 16) & 0x1FF)
	    {
		case 0x02A:	kbdpcEX[0] &= 0x01; kbdpcEX[1] &= 0x01;	break;	//XT LShift
		case 0x036:	kbdpcEX[0] &= 0x01; kbdpcEX[1] &= 0x01;	break;	//XT RShift
		case 0x01D:	kbdpcEX[2] &= 0x01;			break;	//XT LCtrl
		case 0x11D:	kbdpcEX[3] &= 0x01;			break;	//XT RCtrl ???
		case 0x038:	kbdpcEX[4] &= 0x01;			break;	//XT LAlt
		case 0x138:	kbdpcEX[5] &= 0x01;			break;	//XT RAlt ???
	    } //Dexus
	    //-----------------------------------------------------------------
//	    printf("%s, WM_KEYUP, WM_SYSKEYUP\n", __FUNCTION__);
//	    DispatchMessage(&msg); //Dexus
	}
	//---------------------------------------------------------------------
	if (	!((WM_KEYFIRST <= msg.message) && (msg.message <= WM_KEYLAST))
		||
		((WM_MOUSEFIRST <= msg.message) && (msg.message <= WM_MOUSELAST))
	 )
	{
	    DispatchMessage( &msg);
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    return key;
}
//=============================================================================


//=============================================================================
// eat messages
void eat()
{
    //printf("EAT ");
    Sleep( 20);	
    //-------------------------------------------------------------------------
    while (process_msgs())
        Sleep( 10);
    //-------------------------------------------------------------------------
}
//=============================================================================





/*
//=============================================================================
// эксперемментальная kbdpc_prev вверрсия
// рабочая ниже
// Опрос клавы по таблице хоткеев
bool dispatch_more( action *table)
{
    //printf("more\n");
    //-------------------------------------------------------------------------
    if (!table)
	return false;
    //-------------------------------------------------------------------------
    u8 tmp = kbdpc_prev[0];
    kbdpc_prev[0] = 0x80;	// nil button is always pressed
    //-------------------------------------------------------------------------
//   __debugbreak();
    while (table->name)
    {
//	printf("table->name %s\n",table->name);
//	printf("%02X|%02X|%02X|%02X\n", table->k1, table->k2, table->k3, table->k4);
	
	if (0) //(table->k1 == 0xC5)	// debug	//45
	{
	printf("%x|%x|%x|%x brute\n", table->k1, table->k2, table->k3, table->k4);
	
		for (int temp = 0;    temp < 256;   temp++)	//VK_MAX
		{
		
			if(kbdpc_prev[temp])
			printf("%02x ",kbdpc_prev[temp]);
			else
			printf(".. ");
			if ((temp % 16) == 15) printf("\n");
		}
		printf("\n");
	}
	//---------------------------------------------------------------------
	unsigned k[4] =
	{
	    table->k1,
	    table->k2,
	    table->k3,
	    table->k4
	};
	unsigned b[4];
	//---------------------------------------------------------------------
	for (unsigned i = 0;   i < 4;   i++)
	{
	    switch (k[ i])
	    {
		//-------------------------------------------------------------
		// игнор левости правости?
		// но оно же предлагаетсо вводить в конфиге
		// слевостями и праавостями?
		case DIK_MENU:
		    b[i] = kbdpc_prev[ DIK_LMENU] | kbdpc_prev[ DIK_RMENU]; // Alt
		     	if (table->k1 == 0xc5)
			{
			printf("DIK_RMENU %x\n",b[i]);
			}
		    break;
		//-------------------------------------------------------------
		case DIK_CONTROL:
		    b[i] = kbdpc_prev[ DIK_LCONTROL] | kbdpc_prev[ DIK_RCONTROL];
		    	if (table->k1 == 0xc5)
			{
			printf("DIK_LCONTROL %x\n",b[i]);
			}
		    break;
		//-------------------------------------------------------------
		case DIK_SHIFT:
		    b[i] = kbdpc_prev[ DIK_LSHIFT] | kbdpc_prev[ DIK_RSHIFT];
		    	if (table->k1 == 0xc5)
			{
			printf("DIK_LSHIFT %x\n",b[i]);
			}
		    break;
		//-------------------------------------------------------------
		default:
		    b[i] = kbdpc_prev[ k[ i]];
			if (table->k1 == 0xc5)
			{
			printf("kbdpc_prev[ k[ i] %x\n",b[i]);
			}
		//-------------------------------------------------------------
	    }
	}
	//---------------------------------------------------------------------
	//printf("b[0]b[1]b[2]b[3] %x %x %x %x\n",b[0],b[1],b[2],b[3]);
	if (b[0] & b[1] & b[2] & b[3] & 0x80)
	{
	    printf("	func %x\n",table->k1);
	    table->func();	// это каким то хуем вызывает функцию из таблицы
	    kbdpc_prev[0] = tmp;
	    return true;
	}
	//---------------------------------------------------------------------
	table++;
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    kbdpc_prev[0] = tmp;
    return false;
}
//=============================================================================
*/



//=============================================================================
// Опрос клавы по таблице хоткеев
bool dispatch_more( action *table)
{
    //printf("more\n");
    //-------------------------------------------------------------------------
    if (!table)
	return false;
    //-------------------------------------------------------------------------
    u8 tmp = kbdpc[0];
    kbdpc[0] = 0x80;	// nil button is always pressed
    //-------------------------------------------------------------------------
//   __debugbreak();

	//	// tzt
	//	for (int temp = 1;    temp < 256;   temp++)	//VK_MAX
	//	{
	//	    if (kbdpc[ temp])
	//		printf("%02X - %02x\n", temp,kbdpc[ temp]);
	//	}// tzt
		
    while (table->name)
    {
//	printf("table->name %s\n",table->name);
//	printf("%02X|%02X|%02X|%02X\n", table->k1, table->k2, table->k3, table->k4);
	
	//if (table->k1 == 1)	// debug	//45
	//{
	//printf("%x|%x|%x|%x brute\n", table->k1, table->k2, table->k3, table->k4);
	//}
	//---------------------------------------------------------------------
	unsigned k[4] =
	{
	    table->k1,
	    table->k2,
	    table->k3,
	    table->k4
	};
	unsigned b[4];
	//---------------------------------------------------------------------
	for (unsigned i = 0;   i < 4;   i++)
	{
	    switch (k[ i])
	    {
		//-------------------------------------------------------------
		// игнор левости правости?
		// но оно же предлагаетсо вводить в конфиге
		// слевостями и праавостями?
		case DIK_MENU:
		    b[i] = kbdpc[ DIK_LMENU] | kbdpc[ DIK_RMENU]; // Alt
		    break;
		//-------------------------------------------------------------
		case DIK_CONTROL:
		    b[i] = kbdpc[ DIK_LCONTROL] | kbdpc[ DIK_RCONTROL];
		    break;
		//-------------------------------------------------------------
		case DIK_SHIFT:
		    b[i] = kbdpc[ DIK_LSHIFT] | kbdpc[ DIK_RSHIFT];
		    break;
		//-------------------------------------------------------------
		default:
		    b[i] = kbdpc[ k[ i]];
		//-------------------------------------------------------------
	    }
	}
	//---------------------------------------------------------------------
	if (b[0] & b[1] & b[2] & b[3] & 0x80)
	{
	    //printf("	func %x\n",table->k1);
	    table->func();	// это каким то хуем вызывает функцию из таблицы
	    kbdpc[0] = tmp;
	    return true;
	}
	//---------------------------------------------------------------------
	table++;
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    kbdpc[0] = tmp;
    return false;
}
//=============================================================================


//=============================================================================
bool dispatch( action *table)
{
    //-------------------------------------------------------------------------
    if (*droppedFile)
    {
	trd_toload = DefaultDrive;
	loadsnap( droppedFile);
	*droppedFile = 0;
    }
    //-------------------------------------------------------------------------
    if (!input.readdevices())
    {
	return false;	// выходит если небыло K-On !!
    }
    //-------------------------------------------------------------------------
    // дальше хоткеи обрабатываютсо по K-On или по автоповтору венды
    dispatch_more( table);
    return true;
}
//=============================================================================


//=============================================================================
bool wcmatch(char *string, char *wc)
{
    //-------------------------------------------------------------------------
    for (    ;    ;wc++, string++) 
    {
	//---------------------------------------------------------------------
	if (!*string && !*wc)
	    return 1;
	//---------------------------------------------------------------------
	if (*wc == '?')
	{
	    //-----------------------------------------------------------------
	    if (*string)
	    {
		continue;
	    }
	    //-----------------------------------------------------------------
	    else
	    {
		return 0; 
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	if (*wc == '*') 
	{
	    //-----------------------------------------------------------------
	    for (wc++;    *string;    string++)
		if (wcmatch( string, wc)) 
		    return 1;
	    //-----------------------------------------------------------------
	    return 0;
	}
	//---------------------------------------------------------------------
	if (tolower( *string) != tolower( *wc))
	    return 0;
	//---------------------------------------------------------------------
    }
}
//=============================================================================



//=============================================================================
void dump1( BYTE *p, unsigned sz)
{
    //-------------------------------------------------------------------------
    while (sz) 
    {
	printf("\t");
	unsigned chunk = (sz > 16)  ?  16 :
				       sz;
	unsigned i; 				// Alone Coder 0.36.7
	//---------------------------------------------------------------------
	for (/*unsigned*/ i = 0;    i < chunk;    i++)
	    printf("%02X ", p[i]);
	//---------------------------------------------------------------------
	for (    ; i < 16;    i++) 
	    printf("   ");
	//---------------------------------------------------------------------
	for (i = 0;    i < chunk;    i++)
	    printf("%c", (p[i] < 0x20)  ?  '.'  :
					   p[i] );
	//---------------------------------------------------------------------
	printf("\n");
	sz -= chunk;
	p += chunk;
    }
    //-------------------------------------------------------------------------
    printf("\n");
}
//=============================================================================


//=============================================================================
void color( int ink)
{
   SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE), WORD( ink));
}
//=============================================================================



//=============================================================================
void err_win32( DWORD errcode)
{
    //-------------------------------------------------------------------------
    if (errcode == 0xFFFFFFFF)
	errcode = GetLastError();
    //-------------------------------------------------------------------------

    char msg[ 512];
    //-------------------------------------------------------------------------
    if (!FormatMessage(	FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			errcode,
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT),
			msg,
			sizeof( msg),
			nullptr
		)
     )
    {	
	*msg = 0;
    }
    //-------------------------------------------------------------------------
    trim( msg);
    CharToOem( msg, msg);

    color();
    printf( ( *msg)  ?  "win32 error message: " :
			"win32 error code: ");
    color( CONSCLR_ERRCODE);
    //-------------------------------------------------------------------------
    if (*msg)
    {
	printf("%s\n", msg);
    }
    //-------------------------------------------------------------------------
    else
    {
	printf("%04lX\n", errcode);
    }
    //-------------------------------------------------------------------------
    color();
}
//=============================================================================



//=============================================================================
void errmsg( const char *err, const char *str)
{
    color();
    printf("error: ");
    color( CONSCLR_ERROR);
    printf( err, str);
    color();
    printf("\n");

#ifdef _DEBUG
    OutputDebugString( err);
    OutputDebugString( str);
    OutputDebugString("\n");
#endif
}
//=============================================================================



//=============================================================================
void err_printf( const char *format, ...)
{
    va_list args;
    va_start( args, format);
    color();
    printf("error: ");
    color( CONSCLR_ERROR);
    vprintf( format, args);
    color();
    printf("\n");
    va_end( args);
}
//=============================================================================


//=============================================================================
void __declspec( noreturn) errexit( const char *err, const char *str)
{
    errmsg( err, str);
    exit();
}
//=============================================================================


extern "C" void * _ReturnAddress( void);

//=============================================================================

#ifndef __INTEL_COMPILER
#pragma intrinsic(_ReturnAddress)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_ushort)
#endif

//=============================================================================








//=============================================================================
// обрезалка пробелов и табов перед ";" у строк				// [NS]
// тк GetPrivateProfileString ниче не можот...
// юзать так 	string_comment_trimmer(helpname);

void string_comment_trimmer( char *string_for_comment_trim)
{
    int comment_trim_cnt = 0;
    //-------------------------------------------------------------------------
    while (	*string_for_comment_trim != '\0' &&	// 0x00 string end
		*string_for_comment_trim != 0x3B 	// ";"
      )	
    {
	// printf("%d %x %c \n",comment_trim_cnt,*string_for_comment_trim,*string_for_comment_trim);
	string_for_comment_trim++;
	comment_trim_cnt++;
    }
    //-------------------------------------------------------------------------
    // затыкаем ";" или 0х00 если на него наехали
    // printf("%d %x %c \n",comment_trim_cnt,*string_for_comment_trim,*string_for_comment_trim);
    *string_for_comment_trim = 0x00;
    // printf("%d %x %c \n",comment_trim_cnt,*string_for_comment_trim,*string_for_comment_trim);
    // подтираем пробелы и табы
    //-------------------------------------------------------------------------
    while (comment_trim_cnt > 0)
    {
	string_for_comment_trim--;
	comment_trim_cnt--;
	//---------------------------------------------------------------------	
	if (	*string_for_comment_trim == 0x09 ||		// TAB
			*string_for_comment_trim == 0x20 	// SPACE
	 )
	{
	    // printf("%d %x %c \n",comment_trim_cnt,*string_for_comment_trim,*string_for_comment_trim);
	    *string_for_comment_trim = 0x00;
	}
	//---------------------------------------------------------------------
	else
	{
	    break;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    return;
}


	/*
	int comment_trim_cnt = 0;
	int comment_trim_cnt_size = strlen(helpname);
	while (comment_trim_cnt < comment_trim_cnt_size)
	{
		printf("%d %x %c \n",comment_trim_cnt,helpname[comment_trim_cnt],helpname[comment_trim_cnt]);
		if (helpname[comment_trim_cnt] == 0x3B) 
		{
		helpname[comment_trim_cnt] = 0;
		break; 
		}
	comment_trim_cnt++;
	}
	while (comment_trim_cnt > 0)
	{
		comment_trim_cnt--;
		printf("%d %x %c \n",comment_trim_cnt,helpname[comment_trim_cnt],helpname[comment_trim_cnt]);
		if (	helpname[comment_trim_cnt] == 0x09 || 
			helpname[comment_trim_cnt] == 0x20 ) 
		{
		helpname[comment_trim_cnt] = 0;
		}
		else
		{
		break;
		}
	}
	*/
//=============================================================================

