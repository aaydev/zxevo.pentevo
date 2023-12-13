//-----------------------------------------------------------------------------
// Debug Tools								// [NS]
//-----------------------------------------------------------------------------

#include "std.h"			// <windows.h> LRESULT
#include "emul.h"
#include "vars.h"			// comp.****



#include "debug_tools.h"

#include "draw.h"	// PALETTEENTRY pal0[0x100];	// unreal palette



//TEST 2 del!!!!!!!!!!
//#include "sndrender/sndrender.h"


// АААГХТУНГХЪ!!!!
// венда шлет сообщения обработчику сообщений
// еще до того как проихойдет присвоение hwnd !!!!
// hwnd = CreateWindow(xxx);

//before CreateWindow()
//        WM_GETMINMAXINFO
//        WM_NCCREATE
//        WM_NCCALCSIZE
//        WM_CREATE
//after CreateWindow()

// на момент WM_CREATE
// debug_tool_hwnd[n] содержит левый мусор!!!!



// должны быть в той же последовательности что и enum !!!!



// компактный вариант
// но сыпет варнинги про бидабида с++ исаиса низянизя так
char *debug_tool_window_names_array[] =
{
    "Wrong Debug Tool",           
    "comp_pal - Ula Plus Palette",
    "atm3_pal - ATM//DDp Palette",
    "pal0 - Unreal Palette",      
    "Test Debug Tool"             
};

/*
// каждая строка занимает 40 блджд байт!
// но не сыпет варнинги
char debug_tool_window_names_array[][40] =	
{

    { "Wrong Debug Tool",               },
    { "comp_pal - Ula Plus Palette",    },
    { "atm3_pal - ATM//DDp Palette",    },
    { "pal0 - Unreal Palette",          },
    { "Test Debug Tool"                 }

};
*/
//=============================================================================




//    bool debug_tool_create;	// костыль


    int debug_tool_create_nmb;	// номер создаваемого окна
				// если за каким то хреном 
				// какаянибудь вин15 начнет слать сообщения
				// паралельна со следующим CreateWindow
				// то наступит кирдык...
				// нужно придумывать другие способы
				// передачи параметров WM_CREATE-у
				// и чего сразу нельзя было сделать оно нормалльно?
				//	вангую что если передать
				//	ту ссылку на структуру в структуре 
				//	в которую закидывать параметры то...
				//	это будет равносильно такой же передаче..
				//	разве что завести 256 структур
				
#define DEBUG_TOOLS_MAX_COUNT	256

    HWND debug_tool_hwnd[ DEBUG_TOOLS_MAX_COUNT];
    int debug_tool_type[ DEBUG_TOOLS_MAX_COUNT];
    bool debug_tool_exist[ DEBUG_TOOLS_MAX_COUNT];	// 1 - флаг включенности
    bool debug_tool_delete[ DEBUG_TOOLS_MAX_COUNT];	// 1 - на удаление
    
    
    int debug_tool_x[ DEBUG_TOOLS_MAX_COUNT];
    int debug_tool_y[ DEBUG_TOOLS_MAX_COUNT];
    int debug_tool_Mode[ DEBUG_TOOLS_MAX_COUNT];
    bool debug_tool_Captured[ DEBUG_TOOLS_MAX_COUNT];

	
	
	
	 // [NS] test
//HWND hwnd_debug_tool;
//HWND hwnd_debug_tool_2;





    HBITMAP hbitmap_buffer[ DEBUG_TOOLS_MAX_COUNT];
    int *buffer_addr[ DEBUG_TOOLS_MAX_COUNT];
    BITMAPINFO bmi[ DEBUG_TOOLS_MAX_COUNT];





//=============================================================================
void run_debug_tool( int tool_type)
{
    	// перед созданием окна нужно
	// удалить hwnd-ы закрытых окон
	// найти пустой "слот"
	// и туда создать новый hwnd
	
    printf("tool_type %d\n",tool_type);
    
    int cnt = 0;
    //-------------------------------------------------------------------------
    // удаление завершенных слотов
    for (cnt = 0;    cnt < DEBUG_TOOLS_MAX_COUNT;    cnt++)
    {
	//---------------------------------------------------------------------
	if (debug_tool_delete[ cnt])
	{
	    debug_tool_exist[ cnt] = FALSE;	// удаляем
	    debug_tool_delete[ cnt] = FALSE;	// можно снова удалять
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    cnt = 0; 
    //-------------------------------------------------------------------------
    // ищем пустой слот
    while (cnt < DEBUG_TOOLS_MAX_COUNT)
    {
	//-------------------------------------------------------------------------
	if (!(debug_tool_exist[ cnt]))
	{
	    debug_tool_exist[ cnt] = TRUE;	// забиваем пустой слот
	    goto free_slot_found;
	}
	//---------------------------------------------------------------------
    cnt++;
    }
//-----------------------------------------------------------------------------
free_slot_not_found:
    printf("free debug tool slots not found\n");
    return;
//-----------------------------------------------------------------------------
free_slot_found:
    printf("free slot %d\n",cnt);
    //-------------------------------------------------------------------------
    debug_tool_create_nmb = cnt;
    debug_tool_type[ cnt] = tool_type;	//1;
    debug_tool_hwnd[ cnt] = CreateWindowEx(	WS_EX_NOPARENTNOTIFY,
						//WS_EX_NOPARENTNOTIFY,// |
						//  WS_EX_PALETTEWINDOW,
						"Debug_Tool", 				// window class name
						debug_tool_window_names_array[ tool_type],	// "Debug_Tool_Window",	// window caption
						WS_OVERLAPPEDWINDOW,			// window style
						CW_USEDEFAULT,				// initial x position
						CW_USEDEFAULT,				// initial y position
						320,					// initial x size
						320,					// initial y size
						NULL,					// parent window handle
						NULL,					// window menu handle
						GetModuleHandle(nullptr), 		// program instance handle
						NULL					// creation parameters
				       );
    ShowWindow( debug_tool_hwnd[cnt], SW_SHOWNORMAL);	//SW_SHOWMINNOACTIVE 
    UpdateWindow( debug_tool_hwnd[cnt]);
	
    return;
}
//=============================================================================
















//=============================================================================
void test_viewer_init( HWND dbg_hwnd, int current_wind)
{
    buffer_addr[ current_wind] = (int*)malloc( 256 * 256 * 4 * sizeof(char));
    ZeroMemory( &bmi[ current_wind], sizeof( BITMAPINFO));
    bmi[ current_wind].bmiHeader.biSize = sizeof( BITMAPINFOHEADER);
    bmi[ current_wind].bmiHeader.biPlanes = 1;
    bmi[ current_wind].bmiHeader.biBitCount = 32;
    bmi[ current_wind].bmiHeader.biCompression = BI_RGB;
    bmi[ current_wind].bmiHeader.biWidth = 256;
    bmi[ current_wind].bmiHeader.biHeight = -256;	//переварачиваем в нормальный людской вид где верх слева сверху	

    return;
}
//=============================================================================
void test_viewer_destroy( HWND dbg_hwnd, int current_wind)
{ 
    // освобождение hwnd на всякий случай
    // идет при создание нового окна
    
    free( buffer_addr[ current_wind]);		// освобождаем память
    debug_tool_exist[ current_wind] = FALSE;	// отключаем отрисовку итд
    debug_tool_delete[ current_wind] = TRUE;	// 1 - на удаление

    return;
}
//=============================================================================
void test_viewer( HWND dbg_hwnd, int current_wind)
{

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint( dbg_hwnd, &ps);  



//	    #ifdef SND_EXTERNAL_BUFFER
//		SND_EXTERNAL_BUFFER[ dstpos].sample += sample_value;
//		dstpos = (dstpos + 1) & (SND_EXTERNAL_BUFFER_SIZE - 1);
//	    //-----------------------------------------------------------------
//	    #else
	    
	    
    int *current_buffer_addr = buffer_addr[ current_wind];

//SNDBUFSIZE
    unsigned *current_snd_addr = (unsigned*)sndplaybuf;	//works
//  int *current_snd_addr = (int*)sndbuf;	// очищаетсо во время перекидывания
						// в sndplaybuf
    // printf("\r sndplaybuf %x", sndplaybuf[0]);
    //-----------------------------------------------------------------	
    int cnt = 0;
    while ((cnt < (PLAYBUFSIZE)) && (cnt < 256))
    {
	int ch1 = ((*current_snd_addr & 0x0000FFFF));
	int ch2 = ((*current_snd_addr & 0xFFFF0000) >> 16);
	//---------------------------------------------------------------------
	ch1 = 0xFFFF & (ch1 + 0x8000);
	ch2 = 0xFFFF & (ch2 + 0x8000);   
	//---------------------------------------------------------------------
	int black_cnt = (0xFFFF - ch1) / 256;
	int white_cnt = (ch1) / 256;
	int *draw_buffer_addr = current_buffer_addr;

//	*draw_buffer_addr = 0xFF;
//	*(draw_buffer_addr+1) = 0xFF;
//	*(draw_buffer_addr+2) = 0xFF;
	
	while (black_cnt > 0)
	{
	    *draw_buffer_addr = 0x00FFC080;
	    draw_buffer_addr++;
	    black_cnt--;
	}
	while (white_cnt > 0)
	{
	    *draw_buffer_addr = 0x00003010;
	    draw_buffer_addr++;
	    white_cnt--;
	}

	

	//*current_buffer_addr = (R << 16) | (G << 8) | (B);
	current_snd_addr++;
	current_buffer_addr += 256;
	cnt++;
    }
	    //-----------------------------------------------------------------

	    //-----------------------------------------------------------------
	    StretchDIBits(	hdc,	//с масштабированием
				0,
				0,
				256,
				256,
				0,
				0,
				256,
				256,
				buffer_addr[ current_wind],
				&bmi[ current_wind],
				DIB_RGB_COLORS,
				SRCCOPY
				);
	    //-----------------------------------------------------------------
	    // защита от стирания
	    ExcludeClipRect(	hdc,
				0,
				0,
				256,
				256
			   );
	    //-----------------------------------------------------------------
	    RECT rect;
	    GetClientRect( dbg_hwnd, &rect);
	    HBRUSH hbrush = CreateSolidBrush( RGB( 0x00, 0x00, 0x00));

	    FillRect(	hdc,
			&rect,
			hbrush
		     );
	    //-----------------------------------------------------------------

	    EndPaint( dbg_hwnd, &ps);	 
	
	    DeleteObject( hbrush);
    return;
}
//=============================================================================























//=============================================================================
// comp_pal viewer (ula plus подобная поллитра)
//=============================================================================
void comp_pal_viewer_init( HWND dbg_hwnd, int current_wind)
{
 
    buffer_addr[ current_wind] = (int*)malloc( 16 * 16 * 4 * sizeof(char));
    //-------------------------------------------------------------
//    int *current_buffer_addr = buffer_addr[ current_wind];
//    int cnt = 0;
//    //-------------------------------------------------------------
//    // заливка
//    while (cnt < (16 * 16))
//    {
//	*current_buffer_addr = ((cnt) & 0x000000FF);
//	current_buffer_addr++;
//	cnt++;
//    }
//    //-------------------------------------------------------------
//    current_buffer_addr = buffer_addr[ current_wind];
//    *current_buffer_addr++ = 0x000000FF; //B
//    *current_buffer_addr++ = 0x0000FF00; //G
//    *current_buffer_addr++ = 0x00FF0000; //R
//    *current_buffer_addr++ = 0xFF000000;

//    hbitmap_buffer[ current_wind] = CreateBitmap( 16, 16, 1, 32, buffer_addr[ current_wind]);
		
    ZeroMemory( &bmi[ current_wind], sizeof( BITMAPINFO));
    bmi[ current_wind].bmiHeader.biSize = sizeof( BITMAPINFOHEADER);
    bmi[ current_wind].bmiHeader.biPlanes = 1;
    bmi[ current_wind].bmiHeader.biBitCount = 32;
    bmi[ current_wind].bmiHeader.biCompression = BI_RGB;
    bmi[ current_wind].bmiHeader.biWidth = 16;
    bmi[ current_wind].bmiHeader.biHeight = -16;	//переварачиваем в нормальный людской вид где верх слева сверху	

    return;
}
//=============================================================================
void comp_pal_viewer_destroy( HWND dbg_hwnd, int current_wind)
{ 
    // освобождение hwnd на всякий случай
    // идет при создание нового окна
    
    free( buffer_addr[ current_wind]);		// освобождаем память
    debug_tool_exist[ current_wind] = FALSE;	// отключаем отрисовку итд
    debug_tool_delete[ current_wind] = TRUE;	// 1 - на удаление

    return;
}
//=============================================================================
//void comp_pal_viewer( HWND dbg_hwnd, HDC hdc, int current_wind)
void comp_pal_viewer( HWND dbg_hwnd, int current_wind)
{

//unsigned char comp_pal[0x40]; // Формат палитры GggRrrBb (формат ULA+)

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint( dbg_hwnd, &ps);  
	 
// как юзоть reinterpret_cast
//frame_nmb = 0x01&(*(reinterpret_cast<unsigned char*>(0x00B9D908)));


    int *current_buffer_addr = buffer_addr[ current_wind];

    //int *current_pal0_addr = ((reinterpret_cast<unsigned char>(pal0)));//pal0;
    //-----------------------------------------------------------------	
    int cnt = 0;
    while (cnt < (256))
    {
	//printf(" %d",cnt);
	//GggRrrBb (формат ULA+)
	int R = ((comp.comp_pal[cnt] & 0b00011100) << 3);
	int G = ((comp.comp_pal[cnt] & 0b11100000));
	int B = ((comp.comp_pal[cnt] & 0b00000011) << 6);
	
	//*current_buffer_addr = *current_pal0_addr;
	*current_buffer_addr = (R << 16) | (G << 8) | (B);
	//current_pal0_addr++;
	current_buffer_addr++;
	cnt++;
    }
	    //printf("cnt %d\n",cnt);
	    //-----------------------------------------------------------------

	    //-----------------------------------------------------------------
	    StretchDIBits(	hdc,	//с масштабированием
				0,
				0,
				256,
				256,
				0,
				0,
				16,
				16,
				buffer_addr[ current_wind],
				&bmi[ current_wind],
				DIB_RGB_COLORS,
				SRCCOPY
				);
	    //-----------------------------------------------------------------
	    // защита от стирания
	    ExcludeClipRect(	hdc,
				0,
				0,
				256,
				256
			   );
	    //-----------------------------------------------------------------
	    RECT rect;
	    GetClientRect( dbg_hwnd, &rect);
	    HBRUSH hbrush = CreateSolidBrush( RGB( 0x00, 0x00, 0x00));

	    FillRect(	hdc,
			&rect,
			hbrush
		     );
	    //-----------------------------------------------------------------
	    // DrawText - выводит на экран строку текста.

//	    DrawText(	hdc,
//			"Hello, Windows 95!",
//			-1,
//			&rect,
//			DT_SINGLELINE	|
//			  DT_CENTER	|
//			    DT_VCENTER
//		    );
	 

	    //-----------------------------------------------------------------

	    EndPaint( dbg_hwnd, &ps);	 
	
	    DeleteObject( hbrush);
    return;
}
//=============================================================================






//=============================================================================
// atm3_pal viewer (ATM/DDp палитра)
//=============================================================================
void atm3_pal_viewer_init( HWND dbg_hwnd, int current_wind)
{
    buffer_addr[ current_wind] = (int*)malloc( 16 * 16 * 4 * sizeof(char));	
    ZeroMemory( &bmi[ current_wind], sizeof( BITMAPINFO));
    bmi[ current_wind].bmiHeader.biSize = sizeof( BITMAPINFOHEADER);
    bmi[ current_wind].bmiHeader.biPlanes = 1;
    bmi[ current_wind].bmiHeader.biBitCount = 32;
    bmi[ current_wind].bmiHeader.biCompression = BI_RGB;
    bmi[ current_wind].bmiHeader.biWidth = 16;
    bmi[ current_wind].bmiHeader.biHeight = -16;	//переварачиваем в нормальный людской вид где верх слева сверху	

    return;
}
//=============================================================================
void atm3_pal_viewer_destroy( HWND dbg_hwnd, int current_wind)
{ 
    // освобождение hwnd на всякий случай
    // идет при создание нового окна
    
    free( buffer_addr[ current_wind]);		// освобождаем память
    debug_tool_exist[ current_wind] = FALSE;	// отключаем отрисовку итд
    debug_tool_delete[ current_wind] = TRUE;	// 1 - на удаление

    return;
}
//=============================================================================
void atm3_pal_viewer( HWND dbg_hwnd, int current_wind)
{
//    u16 atm3_pal[0x40];	// Формат палитры %grbG11RB(low in HSB), %grbG11RB(high in LSB), inverted



    PAINTSTRUCT ps;
    HDC hdc = BeginPaint( dbg_hwnd, &ps);  

    int *current_buffer_addr = buffer_addr[ current_wind];
    //-----------------------------------------------------------------	
    int cnt = 0;
    while (cnt < (0x40))
    {
    
// 0x000000FF; //B
// 0x0000FF00; //G
// 0x00FF0000; //R

	//printf(" %d",cnt);
	// Формат палитры %grbG11RB(low in HSB),
	//		  %grbG11RB(high in LSB), inverted
	int data = comp.atm3_pal[cnt];
	
	//                 low     high
	//                 grbG11RBgrbG11RB
	int R =	((data & 0b0000000000000010) << 6) | //high
	//                         *
		((data & 0b0000000001000000)     ) |
	//                          *
		((data & 0b0000001000000000) >> 4) | //low
	//                           *
		((data & 0b0100000000000000) >> 10);
	//                            *
	//                 low     high
	//                 grbG11RBgrbG11RB
	int G =	((data & 0b0000000000010000) << 3) | //high
	//                         *
		((data & 0b0000000010000000) >> 1) |
	//                          *
		((data & 0b0001000000000000) >> 7) | //low
	//                           *
		((data & 0b1000000000000000) >> 11);
	//                            *
	//                 low     high
	//                 grbG11RBgrbG11RB
	int B =	((data & 0b0000000000000001) << 7) | //high
	//                         *
		((data & 0b0000000000100000) << 1) |
	//                          *
		((data & 0b0000000100000000) >> 3) | //low
	//                           *
		((data & 0b0010000000000000) >> 9);
	//                            *

	// атм полиитра инвертирована
	R ^= 0xF0;
	G ^= 0xF0;
	B ^= 0xF0;



	
	//*current_buffer_addr = *current_pal0_addr;
	*current_buffer_addr = (R << 16) | (G << 8) | (B);
	//current_pal0_addr++;
	current_buffer_addr++;
	cnt++;
    }

	    //-----------------------------------------------------------------
	    StretchDIBits(	hdc,	//с масштабированием
				0,
				0,
				256,
				256,
				0,
				0,
				16,
				16,
				buffer_addr[ current_wind],
				&bmi[ current_wind],
				DIB_RGB_COLORS,
				SRCCOPY
				);
	    //-----------------------------------------------------------------
	    // защита от стирания
	    ExcludeClipRect(	hdc,
				0,
				0,
				256,
				256
			   );
	    //-----------------------------------------------------------------
	    RECT rect;
	    GetClientRect( dbg_hwnd, &rect);
	    HBRUSH hbrush = CreateSolidBrush( RGB( 0x00, 0x00, 0x00));

	    FillRect(	hdc,
			&rect,
			hbrush
		     );
	    //-----------------------------------------------------------------
	    EndPaint( dbg_hwnd, &ps);	 
	
	    DeleteObject( hbrush);
    return;
}
//=============================================================================






//=============================================================================
// pal0 viewer (палитра унриала)
//=============================================================================
void pal0_viewer_init( HWND dbg_hwnd, int current_wind)
{
    buffer_addr[ current_wind] = (int*)malloc( 16 * 16 * 4 * sizeof(char));
    ZeroMemory( &bmi[ current_wind], sizeof( BITMAPINFO));
    bmi[ current_wind].bmiHeader.biSize = sizeof( BITMAPINFOHEADER);
    bmi[ current_wind].bmiHeader.biPlanes = 1;
    bmi[ current_wind].bmiHeader.biBitCount = 32;
    bmi[ current_wind].bmiHeader.biCompression = BI_RGB;
    bmi[ current_wind].bmiHeader.biWidth = 16;
    bmi[ current_wind].bmiHeader.biHeight = -16;	//переварачиваем в нормальный людской вид где верх слева сверху	

    return;
}
//=============================================================================
void pal0_viewer_destroy( HWND dbg_hwnd, int current_wind)
{ 
    // освобождение hwnd на всякий случай
    // идет при создание нового окна
    
    free( buffer_addr[ current_wind]);		// освобождаем память
    debug_tool_exist[ current_wind] = FALSE;	// отключаем отрисовку итд
    debug_tool_delete[ current_wind] = TRUE;	// 1 - на удаление

    return;
}
//=============================================================================
void pal0_viewer( HWND dbg_hwnd, int current_wind)
{


    PAINTSTRUCT ps;
    HDC hdc = BeginPaint( dbg_hwnd, &ps);  

    int *current_buffer_addr = buffer_addr[ current_wind];

    int *current_pal0_addr = (int*)pal0;//pal0;
    //-----------------------------------------------------------------	
    int cnt = 0;
    while (cnt < (256))
    {

	//int R = ((comp.comp_pal[cnt] & 0b00011100) << 3);
	//int G = ((comp.comp_pal[cnt] & 0b11100000));
	//int B = ((comp.comp_pal[cnt] & 0b00000011) << 6);
	
	*current_buffer_addr = *current_pal0_addr;
	//*current_buffer_addr = (R << 16) | (G << 8) | (B);
	current_pal0_addr++;
	current_buffer_addr++;
	cnt++;
    }
	    //-----------------------------------------------------------------

	    //-----------------------------------------------------------------
	    StretchDIBits(	hdc,	//с масштабированием
				0,
				0,
				256,
				256,
				0,
				0,
				16,
				16,
				buffer_addr[ current_wind],
				&bmi[ current_wind],
				DIB_RGB_COLORS,
				SRCCOPY
				);
	    //-----------------------------------------------------------------
	    // защита от стирания
	    ExcludeClipRect(	hdc,
				0,
				0,
				256,
				256
			   );
	    //-----------------------------------------------------------------
	    RECT rect;
	    GetClientRect( dbg_hwnd, &rect);
	    HBRUSH hbrush = CreateSolidBrush( RGB( 0x00, 0x00, 0x00));

	    FillRect(	hdc,
			&rect,
			hbrush
		     );
	    //-----------------------------------------------------------------

	    EndPaint( dbg_hwnd, &ps);	 
	
	    DeleteObject( hbrush);
    return;
}
//=============================================================================

















//=============================================================================
// Обработчик событий сразу всех окно debug tools-ов
//=============================================================================
LRESULT CALLBACK WndProc_debug_tools(		HWND dbg_hwnd,
						UINT iMsg,
						WPARAM wParam,
						LPARAM lParam
            )
//-----------------------------------------------------------------------------
{

    // Тут нужно по hwnd найти номер окна
    // а потом загрузить все переменные
    // и создать переменную с номером текущего окна    


//    HDC hdc;
//    PAINTSTRUCT ps;
//    RECT rect;

    //-------------------------------------------------------------------------
    // Ищем номер текущего окна
    int current_wind = 0;
    //-------------------------------------------------------------------------
    while (current_wind < DEBUG_TOOLS_MAX_COUNT)
    {
	//---------------------------------------------------------------------
	if (debug_tool_hwnd[ current_wind] == dbg_hwnd)
	{
	    break;
	}
	//---------------------------------------------------------------------
	current_wind++;
    }
    	//system ("pause");
    //-------------------------------------------------------------------------
    //printf("%d\n",current_wind);
    //static int x;
    //static int y;
    //static int Mode;
    //static bool Captured;

    //-------------------------------------------------------------------------
    // переменные с прямой записью а массив
    // чтобы не сохранять каждую при выходе из обработчика сообщений
    int *x =		&debug_tool_x[ current_wind];
    int *y =		&debug_tool_y[ current_wind];
    int *Mode =		&debug_tool_Mode[ current_wind];
    bool *Captured =	&debug_tool_Captured[ current_wind];

    //-------------------------------------------------------------------------
    // переменные которые нужно сохранять ручками
    int current_debug_tool_type = debug_tool_type[ current_wind];



    
    //-------------------------------------------------------------------------
    switch (iMsg)
    {
	//---------------------------------------------------------------------
	// АХТУНГХ!!!!!
	// НА МОМЕНТ WM_CREATE
	// current_wind НЕДЕЙСТВИТЕЛЕН!!!!
	// есть int debug_tool_create_nmb - номер который щас создаетсо
	case WM_CREATE:
	{
	    //printf("WM_CREATE ");
	    current_debug_tool_type = debug_tool_type[ debug_tool_create_nmb];
	    current_wind = debug_tool_create_nmb;
	    
   
	    //-----------------------------------------------------------------
	    switch (current_debug_tool_type)
	    {
		//-------------------------------------------------------------
		case DT_PALETTE_ATM3_PAL:
		    atm3_pal_viewer_init( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		case DT_PALETTE_COMP_PAL:
		    comp_pal_viewer_init( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		case DT_PALETTE_PAL0:
		    pal0_viewer_init( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		case DT_TEST:
		    test_viewer_init( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		default:
		    printf("wrong current_debug_tool_type\n");
		    test_viewer_init( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    
	}
	//---------------------------------------------------------------------
	return 0;
	//---------------------------------------------------------------------
	case WM_ERASEBKGND:
	// стандартный обработчик рисует херню поверх
	// при вытягивании окна изза границы экрана
	    return 0;
	//---------------------------------------------------------------------
	case WM_PAINT:
	{ 
	    // printf("WM_PAINT ");
	    // PAINTSTRUCT ps;
	    // HDC hdc = BeginPaint( dbg_hwnd, &ps);  
	    // comp_pal_viewer( dbg_hwnd, hdc, current_wind);
	    //-----------------------------------------------------------------
	    switch (current_debug_tool_type)
	    {
		//-------------------------------------------------------------
		case DT_PALETTE_ATM3_PAL:
		    atm3_pal_viewer( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		case DT_PALETTE_COMP_PAL:
		    comp_pal_viewer( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		case DT_PALETTE_PAL0:
		    pal0_viewer( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		case DT_TEST:
		    test_viewer( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		default:
		    test_viewer( dbg_hwnd, current_wind);
		    break;			    
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    
	//    EndPaint( dbg_hwnd, &ps);	    
	   // printf("end ");
	}
	return 0; 
	//---------------------------------------------------------------------
/*
		// BeginPaint - инициирует начало процесса рисования окна.
		hdc = BeginPaint(	dbg_hwnd,
					&ps
				 );
		// GetClientRect - получает размер рабочей области окна
		GetClientRect(	dbg_hwnd,
				&rect
			      );
		// DrawText - выводит на экран строку текста.
		DrawText(	hdc,
				"Hello, Windows 95!",
				-1,
				&rect,
					DT_SINGLELINE |
					DT_CENTER |
					DT_VCENTER
			 );
		// EndPaint - прекращает рисование окна
		EndPaint(	dbg_hwnd,
				&ps
			 );
*/

	

	//---------------------------------------------------------------------
	case WM_CLOSE: 
   	    printf("tool WM_CLOSE\n");
	    break;	//return 1;
	//---------------------------------------------------------------------
	case WM_DESTROY:
	    printf("tool WM_DESTROY\n");
	    // PostQuitMessage - вставляет сообщение "завершить" в очередь сообщений.
	    // PostQuitMessage(0);

	    // Ищем номер текущего окна
	    current_wind = 0;
	    //-----------------------------------------------------------------
	    while (current_wind < DEBUG_TOOLS_MAX_COUNT)
	    {
		//-------------------------------------------------------------
		if (debug_tool_hwnd[ current_wind] == dbg_hwnd)
		{
		    break;
		}
		//-------------------------------------------------------------
		current_wind++;
	    }
	    printf("destroy %d\n", current_wind);
	    //-----------------------------------------------------------------
	    switch (current_debug_tool_type)
	    {
		//-------------------------------------------------------------
		case DT_PALETTE_ATM3_PAL:
		    atm3_pal_viewer_destroy( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		case DT_PALETTE_COMP_PAL:
		    comp_pal_viewer_destroy( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		case DT_PALETTE_PAL0:
		    pal0_viewer_destroy( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		case DT_TEST:
		    test_viewer_destroy( dbg_hwnd, current_wind);
		    break;
		//-------------------------------------------------------------
		default:
		    test_viewer_destroy( dbg_hwnd, current_wind);
		    break;			    
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break; //return 0;
	//---------------------------------------------------------------------
	case WM_LBUTTONDOWN:	
	{
	    //printf("WM_LBUTTONDOWN\n");
	    POINT point =
	    {
	    	GET_X_LPARAM(lParam),
	    	GET_Y_LPARAM(lParam)
	    };
	    ClientToScreen( dbg_hwnd, &point);
	    *x = point.x;
	    *y = point.y;
	    *Mode = SC_MOVE;
	    SetCapture( dbg_hwnd); 
	    *Captured = TRUE;
	}
	return 0;
	//---------------------------------------------------------------------
	case WM_MOVE:
	    //printf("WM_MOVE\n");
	    return 0; 
	//---------------------------------------------------------------------
    
    
    
    
	//---------------------------------------------------------------------
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
	{
	    //printf("WM_MOUSEMOVE\n");
	    //-----------------------------------------------------------------
	    if (*Captured)
	    {
		//-------------------------------------------------------------
		// кастомная обработка события перемещения
		// тк обработчик венды останавливает всё на время cmd == SC_MOVE ?
		if (*Mode == SC_MOVE)
		{
		    //printf("moviiiing\n");
		    POINT p =
		    {
			GET_X_LPARAM( lParam),
			GET_Y_LPARAM( lParam)
		    };
		    ClientToScreen( dbg_hwnd, &p);
//		    printf("nx=%d,ny=%d\n", p.x, p.y);
		    int dx = p.x - *x;
		    int dy = p.y - *y;
//		    printf("dx=%d,dy=%d\n", dx, dy);
		    //---------------------------------------------------------
		    if ((dx == 0) && (dy == 0))
		    {
			return 0;
			//goto window_not_moving;
		    }
		    //---------------------------------------------------------
		    RECT r = {};
		    GetWindowRect( dbg_hwnd, &r);

		    // кастомная обработка события перемещения
		    // дикая мигалка (была)
		    SetWindowPos(	dbg_hwnd,
					nullptr,
					(r.left + dx),
					(r.top + dy),
					0,
					0,
					SWP_NOOWNERZORDER	|
					 SWP_NOZORDER		|
					  SWP_NOSIZE		|
					   SWP_NOSENDCHANGING
			    );

		    *x = p.x;
		    *y = p.y;
		    return 0;
		}
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	}
	return 0;
	//---------------------------------------------------------------------
	case WM_LBUTTONUP:
	case WM_NCLBUTTONUP:

	    //printf("WM_LBUTTONUP\n");
	    *Captured = FALSE;
	    ReleaseCapture(); 
	    return 0;
	//---------------------------------------------------------------------
   
	
	
	
	
	
	
	
	
	
	
	
	
	
	case WM_SYSCOMMAND:
	{
//       printf("WM_SYSCOMMAND(0x%X), w=%X, l=%X\n", uMessage, wparam, lparam);
// Кодирование сторон рамки окна для SC_SIZE (код угла получается суммированием кодов смежных сторон)
//     4           5
//      \ ___3___ /
//       |       |
//       1       2
//       |_______|
//      /    6    \
//     7           8


	
       ULONG Cmd = (wParam & 0xFFF0);
       ULONG BrdSide = (wParam & 0xF);

       if (Cmd == SC_MOVE)
       {
           //printf("SC_MOVE\n");
	   *x = GET_X_LPARAM( lParam);
           *y = GET_Y_LPARAM( lParam);
           SetCapture( dbg_hwnd);	//hwnd_debug_tool); 
           *Captured = TRUE;
           *Mode = SC_MOVE;
           return 0;
       }
       
       }
       
       
 
       
       
       
       

       
       
       /*
       else if (Cmd == SC_SIZE)
       {
           *Mode = SC_SIZE;
		//printf ("(Cmd == SC_SIZE)\n");
           RECT rcWindow;
           GetWindowRect(dbg_hwnd, &rcWindow);
           RECT rcClient;
           GetClientRect(dbg_hwnd, &rcClient);
           RECT rcDesktop;
           GetWindowRect(GetDesktopWindow(), &rcDesktop);

           Msd.cmd = int(BrdSide);
           CopyRect(&Msd.rcDragCursor, &rcWindow);
           LONG cw = rcClient.right - rcClient.left, ch = rcClient.bottom - rcClient.top;
           LONG ww = rcWindow.right - rcWindow.left, wh = rcWindow.bottom - rcWindow.top;
           LONG dw = rcDesktop.right - rcDesktop.left, dh = rcDesktop.bottom - rcDesktop.top;
           Msd.ptMinTrack = { LONG(temp.ox) + (ww - cw), LONG(temp.oy) + (wh - ch) };
           Msd.ptMaxTrack = { dw, dh };

           SetCapture(dbg_hwnd);
           *Captured = true;
           SizeRect(&Msd, ULONG(lparam));
           return 0;
       }
       */
		
		
		
		
		
    } //switch(iMsg)
    //-------------------------------------------------------------------------
    
    
    return DefWindowProc(	dbg_hwnd,
				iMsg,
				wParam,
				lParam
			 );
}

