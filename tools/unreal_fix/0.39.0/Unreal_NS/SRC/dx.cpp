#include "std.h"

#include "resource.h"
#include "emul.h"
#include "vars.h"
#include "init.h"
#include "dx.h"
#include "draw.h"
#include "dxrend.h"
#include "dxrendch.h"
#include "dxrframe.h"
#include "dxr_text.h"
#include "dxr_rsm.h"
#include "dxr_advm.h"
#include "dxovr.h"
#include "dxerr.h"
#include "sndrender/sndcounter.h"
#include "sound.h"
#include "savesnd.h"
#include "emulkeys.h"
#include "funcs.h"
#include "util.h"


#include "snapshot.h"	//для меню			// [NS]
#include "cheat.h"	//для меню			// [NS]
#include "dbgbpx.h"	//для меню			// [NS]
#include "dbgcmd.h"	//для меню			// [NS]
#include "debug.h"	//для dbg_force_exit		// [NS]
#include "dbgpaint.h"	//для debugflip()		// [NS]
#include "dbglabls.h"	//для mon_show_labels		// [NS]
#include "dbgoth.h"	//mon_gsdialog()		// [NS]
#include "tape.h"	//для меню tape_infosiz		// [NS]

#include "debug_tools.h"	// для создания окна 	// [NS]

unsigned char active = 0;
unsigned char pause = 0;

// вынесено в resource.h
//static const DWORD SCU_SCALE1 = 0x10;
//static const DWORD SCU_SCALE2 = 0x20;
//static const DWORD SCU_SCALE3 = 0x30;
//static const DWORD SCU_SCALE4 = 0x40;
//static const DWORD SCU_LOCK_MOUSE = 0x50;

#define MAXDSPIECE (48000 * 4 / 20)
#define DSBUFFER_SZ (2 * conf.sound.fq * 4)

#define SLEEP_DELAY 2

static HMODULE D3d9Dll = nullptr;
static IDirect3D9 *D3d9 = nullptr;
static IDirect3DDevice9 *D3dDev = nullptr;
static IDirect3DTexture9 *Texture = nullptr;
static IDirect3DSurface9 *SurfTexture = nullptr;

LPDIRECTDRAW2 dd;
LPDIRECTDRAWSURFACE sprim;
LPDIRECTDRAWSURFACE surf0;
LPDIRECTDRAWSURFACE surf1;

static LPDIRECTDRAWSURFACE surf2;
static PVOID SurfMem1 = nullptr;
static ULONG SurfPitch1 = 0;

static LPDIRECTINPUTDEVICE dikeyboard;
static LPDIRECTINPUTDEVICE dimouse;
LPDIRECTINPUTDEVICE2 dijoyst;

static LPDIRECTSOUND ds;
static LPDIRECTSOUNDBUFFER dsbf;
static LPDIRECTDRAWPALETTE pal;
static LPDIRECTDRAWCLIPPER clip;

static unsigned dsoffset;
static unsigned dsbuffer_sz;

static void FlipBltAlign4();
static void FlipBltAlign16();

static void (*FlipBltMethod)() = nullptr;

static void StartD3d( HWND Wnd);
static void DoneD3d( bool DeInitDll = true);
static void SetVideoModeD3d( bool Exclusive);

static HMENU main_menu;		//TSL







void reinit_sound_ds();	// [NS]



/* ---------------------- renders ------------------------- */

//=============================================================================
RENDER renders[] =
{
    { "Normal",				render_small,	"normal",	RF_DRIVER | RF_1X },
    { "Double size",			render_dbl,	"double",	RF_DRIVER | RF_2X },
    { "Triple size",			render_3x,	"triple",	RF_DRIVER | RF_3X },
    { "Quad size",			render_quad,	"quad",		RF_DRIVER | RF_4X },
    { "Anti-Text64",			render_text,	"text",		RF_DRIVER | RF_2X | RF_USEFONT },
    { "Frame resampler",		render_rsm,	"resampler",	RF_DRIVER | RF_8BPCH },

    { "TV Emulation",			render_tv,	"tv",		RF_YUY2 | RF_OVR },
    { "Bilinear filter (MMX,fullscr)",	render_bil,	"bilinear",	RF_2X | RF_PALB },
    { "Vector scaling (fullscr)",	render_scale,	"scale",	RF_2X },
    { "AdvMAME scale (fullscr)",	render_advmame,	"advmame",	RF_DRIVER },

    { "Chunky overlay (fast!)",		      render_ch_ov,  "ch_ov",	RF_OVR  | 0*RF_128x96 | 0*RF_64x48 | RF_BORDER | RF_USE32AS16 },
    { "Chunky 32bit hardware (flt,fullscr)",  render_ch_hw,  "ch_hw",	RF_CLIP | 0*RF_128x96 | 0*RF_64x48 | RF_BORDER | RF_32 | RF_USEC32 },

    { "Chunky 16bit, low-res, (flt,fullscr)", render_c16bl,  "ch_bl",	RF_16 | RF_BORDER |         RF_USEC32 },
    { "Chunky 16bit, hi-res, (flt,fullscr)",  render_c16b,   "ch_b",	RF_16 | RF_2X | RF_BORDER | RF_USEC32 },
    { "Ch4x4 true color (fullscr)",	      render_c4x32b, "ch4true",	RF_32 | RF_2X | RF_BORDER | RF_USEC32 },

    { nullptr,				nullptr,	nullptr,	0 }
};

size_t renders_count = _countof(renders);

//=============================================================================
const RENDER drivers[] =
{
    { "Video Memory (8bpp) (Fullscr)",	nullptr,	"ddraw",	RF_8    },
    { "Video Memory (16bpp) (Fullscr)",	nullptr,	"ddrawh",	RF_16   },
    { "Video Memory (32bpp) (Fullscr)",	nullptr,	"ddrawt",	RF_32   },
    { "GDI Device Context",		nullptr,	"gdi",		RF_GDI  },
    { "Overlay",			nullptr,	"ovr",		RF_OVR  },
    { "Hardware Blitter",		nullptr,	"blt",		RF_CLIP },
    { "Hardware 3D",			nullptr,	"d3d",		RF_D3D  },
    { "Hardware 3D Exclusive (Fullscr)", nullptr,	"d3de",		RF_D3DE },
    { nullptr,				nullptr,	nullptr,	0       }
};
//=============================================================================




//=============================================================================
inline void switch_video()
{
    static unsigned char eff7 = u8(-1U);
    //-------------------------------------------------------------------------
    if ((comp.pEFF7 ^ eff7) & EFF7_HWMC)
    {
	video_timing_tables();
	eff7 = comp.pEFF7;
    }
    //-------------------------------------------------------------------------
}
//=============================================================================




//=============================================================================
static void FlipGdi()
{
    // окно дебагера рисуетсо в другом месте
    
    //-------------------------------------------------------------------------
    if (needclr)
    {
        needclr--;
        gdi_frame();
    }
    //-------------------------------------------------------------------------
    renders[ conf.render].func( gdibuf, temp.ox * temp.obpp / 8); // render to memory buffer

    //printf("XX ");
    // copy bitmap to gdi dc
    SetDIBitsToDevice(	temp.gdidc,	//HDC hdc
			
			int (temp.gx),	//int xDest
			int (temp.gy),	//int yDest
			temp.ox,	//DWORD w
			temp.oy,	//DWORD h
			
			0,		//int xSrc
			0,		//int ySrc
			0,		//UINT StartScan
			temp.oy,	//UINT cLines
			
			gdibuf,		//CONST VOID *lpvBits
			&gdibmp.header,	//CONST BITMAPINFO *lpbmi
			DIB_RGB_COLORS	//UINT ColorUse
		      );
    //-------------------------------------------------------------------------
    // какаято городуха от NS для "рисования черной рамочки в режиме gdi
    // наверное лучше сделать 4/8 квдаратов
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint( wnd, &ps);
    //-------------------------------------------------------------------------
    RECT rect;
    RECT draw_rect;
    GetClientRect(wnd, &rect);
    HBRUSH hbrush = CreateSolidBrush( RGB( 0x00, 0x00, 0x00));
    //-------------------------------------------------------------------------
    // 11111
    // 3 . 4
    // 22222
    draw_rect.top = rect.top;			// 11111
    draw_rect.left = rect.left;
    draw_rect.right = rect.right;
    draw_rect.bottom = temp.gy; //-1;
    FillRect( hdc, &draw_rect, hbrush );

    draw_rect.top = temp.gy + temp.oy; //+1;	// 22222
//  draw_rect.left = rect.left;
//  draw_rect.right = rect.right;
    draw_rect.bottom = rect.bottom;
    FillRect( hdc, &draw_rect, hbrush );
	
    draw_rect.top = temp.gy;			// 3....
//  draw_rect.left = rect.left;
    draw_rect.right = temp.gx; //-1
    draw_rect.bottom = temp.gy + temp.oy;
    FillRect( hdc, &draw_rect, hbrush );
		
    draw_rect.top = temp.gy;			// ....4
    draw_rect.left = temp.gx + temp.ox; //+1
    draw_rect.right = rect.right;
    draw_rect.bottom = temp.gy + temp.oy;
    FillRect( hdc, &draw_rect, hbrush );
    //-------------------------------------------------------------------------
    EndPaint( wnd, &ps);
    
    DeleteObject( hbrush);
    //-------------------------------------------------------------------------
}
//=============================================================================




//=============================================================================
static void FlipBltAlign16()
{
    // Отрисовка в буфер выравненный на 16 байт (необходимо для записи через sse2)
    renders[conf.render].func(PUCHAR(SurfMem1), SurfPitch1);

restore_lost:;
    DDSURFACEDESC desc;
    desc.dwSize = sizeof(desc);
    HRESULT r = surf1->Lock(nullptr, &desc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr);
    //-------------------------------------------------------------------------
    if (r != DD_OK)
    {
	//---------------------------------------------------------------------
	if (r == DDERR_SURFACELOST)
	{
	    surf1->Restore();
	    HRESULT r2 = surf1->IsLost();
	    //-----------------------------------------------------------------
	    if (r2 == DDERR_SURFACELOST)
		Sleep(1);
	    //-----------------------------------------------------------------
	    if ((r2 == DD_OK) || (r2 == DDERR_SURFACELOST))
		goto restore_lost;
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	if (!active)
	    return;
	//---------------------------------------------------------------------
	printrdd("IDirectDrawSurface2::Lock() [buffer]", r);
	exit();
    }
    //-------------------------------------------------------------------------
    PUCHAR SrcPtr, DstPtr;
    SrcPtr = PUCHAR(SurfMem1);
    DstPtr = PUCHAR(desc.lpSurface);
    //-------------------------------------------------------------------------
    size_t LineSize = temp.ox * (temp.obpp >> 3);
    //-------------------------------------------------------------------------
    for (unsigned y = 0;    y < temp.oy;    y++)
    {
	memcpy(DstPtr, SrcPtr, LineSize);
	SrcPtr += SurfPitch1;
	DstPtr += desc.lPitch;
    }
    //-------------------------------------------------------------------------
    r = surf1->Unlock(nullptr);
    //-------------------------------------------------------------------------
    if (r != DD_OK)
    {
	//---------------------------------------------------------------------
	if (r == DDERR_SURFACELOST)
	{
	    surf1->Restore();
	    HRESULT r2 = surf1->IsLost();
	    //-----------------------------------------------------------------
	    if (r2 == DDERR_SURFACELOST)
		Sleep(1);
	    //-----------------------------------------------------------------
	    if ((r2 == DD_OK) || (r2 == DDERR_SURFACELOST))
		goto restore_lost;
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	printrdd("IDirectDrawSurface2::Unlock() [buffer]", r);
	exit();
    }
}
//=============================================================================




//=============================================================================
static void FlipBltAlign4()
{

restore_lost:;
    
    DDSURFACEDESC desc;
    desc.dwSize = sizeof(desc);
    HRESULT r = surf1->Lock(nullptr, &desc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr);
    //-------------------------------------------------------------------------
    if (r != DD_OK)
    {
	//---------------------------------------------------------------------
	if (r == DDERR_SURFACELOST)
	{
	    surf1->Restore();
	    HRESULT r2 = surf1->IsLost();
	    //-----------------------------------------------------------------
	    if (r2 == DDERR_SURFACELOST)
		Sleep(1);
	    //-----------------------------------------------------------------
	    if ((r2 == DD_OK) || (r2 == DDERR_SURFACELOST))
		goto restore_lost;
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	if (!active)
	    return;
	//---------------------------------------------------------------------
	printrdd("IDirectDrawSurface2::Lock() [buffer]", r);
	exit();
    }
    //-------------------------------------------------------------------------
    // Отрисовка в буфер выравненный на 4 байта
    renders[conf.render].func( PUCHAR( desc.lpSurface), unsigned( desc.lPitch));

    r = surf1->Unlock(nullptr);
    //-------------------------------------------------------------------------
    if (r != DD_OK)
    {
	//---------------------------------------------------------------------
	if (r == DDERR_SURFACELOST)
	{
	    surf1->Restore();
	    HRESULT r2 = surf1->IsLost();
	    //-----------------------------------------------------------------
	    if (r2 == DDERR_SURFACELOST)
		Sleep(1);
	    //-----------------------------------------------------------------
	    if ((r2 == DD_OK) || (r2 == DDERR_SURFACELOST))
		goto restore_lost;
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	printrdd("IDirectDrawSurface2::Unlock() [buffer]", r);
	exit();
    }
    //-------------------------------------------------------------------------
}
//=============================================================================




//=============================================================================
static bool CalcFlipRect( LPRECT R)
{
    int w = temp.client.right - temp.client.left;
    int h = temp.client.bottom - temp.client.top;
    int n = min( w / int( temp.ox), h / int( temp.oy));

    POINT P = { 0, 0 };
    ClientToScreen( wnd, &P);
    int x0 = P.x;
    int y0 = P.y;
    R->left =   (w - (int(temp.ox) * n)) / 2;
    R->right =  (w + (int(temp.ox) * n)) / 2;
    R->top =    (h - (int(temp.oy) * n)) / 2;
    R->bottom = (h + (int(temp.oy) * n)) / 2;
    OffsetRect(R, x0, y0);
    return IsRectEmpty(R) == FALSE;
}
//=============================================================================




//=============================================================================
static void FlipBlt()
{
    HRESULT r;

restore_lost:;

    FlipBltMethod();

    assert(!IsRectEmpty(&temp.client));

    DDBLTFX Fx;
    Fx.dwSize = sizeof(Fx);
    Fx.dwDDFX = 0;

    RECT R;
    //-------------------------------------------------------------------------
    if (!CalcFlipRect(&R))
	return;
    //-------------------------------------------------------------------------
    // Очистка back буфера
    Fx.dwFillColor = 0;
    r = surf2->Blt(nullptr, nullptr, nullptr, DDBLT_WAIT | DDBLT_ASYNC |  DDBLT_COLORFILL, &Fx);
    //-------------------------------------------------------------------------
    if (r != DD_OK)
    {
	//---------------------------------------------------------------------
	if (r == DDERR_SURFACELOST)
	{
	    surf2->Restore();
	    //-----------------------------------------------------------------
	    if (surf2->IsLost() == DDERR_SURFACELOST)
		Sleep(1);
	    //-----------------------------------------------------------------
	    goto restore_lost;
	}
	//---------------------------------------------------------------------
	printrdd("IDirectDrawSurface2::Blt(1)", r);
	exit();
    }
    //-------------------------------------------------------------------------
    // Рисование картинки с масштабированием в n раз из surf1 (320x240) -> surf2 (размер экрана)
    r = surf2->Blt(&R, surf1, nullptr, DDBLT_WAIT | DDBLT_ASYNC | DDBLT_DDFX, &Fx);
    //-------------------------------------------------------------------------
    if (r != DD_OK)
    {
	//---------------------------------------------------------------------
	if (r == DDERR_SURFACELOST)
	{
	    surf1->Restore();
	    surf2->Restore();
	    //-----------------------------------------------------------------
	    if ((surf1->IsLost() == DDERR_SURFACELOST) || (surf2->IsLost() == DDERR_SURFACELOST))
		Sleep(1);
	    //-----------------------------------------------------------------
	    goto restore_lost;
	}
	//---------------------------------------------------------------------
	printrdd("IDirectDrawSurface2::Blt(2)", r);
	exit();
    }
    //-------------------------------------------------------------------------
    // Копирование surf2 на surf0 (экран, размер экрана)
    r = surf0->Blt(nullptr, surf2, nullptr, DDBLT_WAIT | DDBLT_ASYNC | DDBLT_DDFX, &Fx);
    //-------------------------------------------------------------------------
    if (r != DD_OK)
    {
	//---------------------------------------------------------------------
	if (r == DDERR_SURFACELOST)
	{
	    surf0->Restore();
	    surf2->Restore();
	    //-----------------------------------------------------------------
	    if ((surf0->IsLost() == DDERR_SURFACELOST) || (surf2->IsLost() == DDERR_SURFACELOST))
		Sleep(1);
	    //-----------------------------------------------------------------
	    goto restore_lost;
	}
	//---------------------------------------------------------------------
	printrdd("IDirectDrawSurface2::Blt(3)", r);
	exit();
    }
}
//=============================================================================




//=============================================================================
static bool CalcFlipRectD3d(LPRECT R)
{
    int w = temp.client.right - temp.client.left;
    int h = temp.client.bottom - temp.client.top;
    int n = min(w / int(temp.ox), h / int(temp.oy));

    R->left =   ( w - ( int( temp.ox) *n )) / 2;
    R->right =  ( w + ( int( temp.ox) *n )) / 2;
    R->top =    ( h - ( int( temp.oy) *n )) / 2;
    R->bottom = ( h + ( int( temp.oy) *n )) / 2;

    return IsRectEmpty(R) == FALSE;
}
//=============================================================================




//=============================================================================
static void FlipD3d()
{
    //-------------------------------------------------------------------------
    if (!SUCCEEDED(D3dDev->BeginScene()))
	return;
    //-------------------------------------------------------------------------
    HRESULT Hr;
    IDirect3DSurface9 *SurfBackBuffer0 = nullptr;

    Hr = D3dDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &SurfBackBuffer0);
    //-------------------------------------------------------------------------
    if (Hr != DD_OK)
    {
	printrdd("IDirect3DDevice9::GetBackBuffer(0)", Hr);
	exit();
    }
    //-------------------------------------------------------------------------

    D3DLOCKED_RECT Rect = { };

    Hr = SurfTexture->LockRect(&Rect, nullptr, D3DLOCK_DISCARD);
    //-------------------------------------------------------------------------
    if (FAILED(Hr))
    {
        printrdd("IDirect3DDevice9::LockRect()", Hr);
//        __debugbreak();
        exit();
    }
    //-------------------------------------------------------------------------

    renders[conf.render].func((u8 *)Rect.pBits, unsigned(Rect.Pitch));

    Hr = SurfTexture->UnlockRect();
    //-------------------------------------------------------------------------
    if (FAILED(Hr))
    {
	printrdd("IDirect3DDevice9::UnlockRect()", Hr);
	//        __debugbreak();
	exit();
    }
    //-------------------------------------------------------------------------
    Hr = D3dDev->ColorFill(SurfBackBuffer0, nullptr, D3DCOLOR_XRGB(0, 0, 0));
    //-------------------------------------------------------------------------
    if (FAILED(Hr))
    {
	printrdd("IDirect3DDevice9::ColorFill()", Hr);
	//	__debugbreak();
	exit();
    }
    //-------------------------------------------------------------------------

    RECT R;
    //-------------------------------------------------------------------------
    if (!CalcFlipRectD3d(&R))
    {
	D3dDev->EndScene();
	SurfBackBuffer0->Release();
	return;
    }
    //-------------------------------------------------------------------------
    Hr = D3dDev->StretchRect(SurfTexture, nullptr, SurfBackBuffer0, &R, D3DTEXF_POINT);
    //-------------------------------------------------------------------------
    if (FAILED(Hr))
    {
	printrdd("IDirect3DDevice9::StretchRect()", Hr);
	//	__debugbreak();
	exit();
    }
    //-------------------------------------------------------------------------
    D3dDev->EndScene();

    // Present the backbuffer contents to the display
    Hr = D3dDev->Present(nullptr, nullptr, nullptr, nullptr);
    SurfBackBuffer0->Release();
    //-------------------------------------------------------------------------
    if (FAILED(Hr))
    {
	//---------------------------------------------------------------------
	if (Hr == D3DERR_DEVICELOST)
	{
	    SetVideoModeD3d( (temp.rflags & RF_D3DE) != 0);
	    return;
	}
	//---------------------------------------------------------------------
        printrdd("IDirect3DDevice9::Present()", Hr);
	//        __debugbreak();
        exit();
    }
    //-------------------------------------------------------------------------
}
//=============================================================================




//=============================================================================
static void FlipVideoMem()
{
	//printf("FlipVideoMem()\n");

    // draw direct to video memory, overlay
    LPDIRECTDRAWSURFACE drawsurf = conf.flip  ?	 surf1 :
						 surf0;

    DDSURFACEDESC desc;
    desc.dwSize = sizeof desc;

restore_lost:;

    HRESULT r = drawsurf->Lock(nullptr, &desc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr);
    //-------------------------------------------------------------------------
    if (r != DD_OK)
    {
	//---------------------------------------------------------------------
	if (!active)
	    return;
	//---------------------------------------------------------------------
	if (r == DDERR_SURFACELOST)
	{
	    //printf("(r == DDERR_SURFACELOST)\n");
	    drawsurf->Restore();
	    //-----------------------------------------------------------------
	    if (drawsurf->IsLost() == DDERR_SURFACELOST)
		Sleep(1);
	    //-----------------------------------------------------------------
	    goto restore_lost;
	}
	//---------------------------------------------------------------------
	printrdd("IDirectDrawSurface2::Lock()", r);
	if (!exitflag)
	    exit();
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    if (needclr)
    {
	//printf("needclr\n");
	needclr--;
	_render_black((unsigned char*)desc.lpSurface, unsigned(desc.lPitch));
    }
    //-------------------------------------------------------------------------
    renders[conf.render].func((unsigned char*)desc.lpSurface + unsigned(desc.lPitch)*temp.ody + temp.odx, unsigned(desc.lPitch));
    drawsurf->Unlock(nullptr);
    //-------------------------------------------------------------------------
    if (conf.flip) // draw direct to video memory
    {
	//printf("conf.flip\n");	//not overlay?
	r = surf0->Flip(nullptr, DDFLIP_WAIT);
	//---------------------------------------------------------------------
	if (r != DD_OK)
	{
	    //-----------------------------------------------------------------
	    if (r == DDERR_SURFACELOST)
	    {
		surf0->Restore();
		//-------------------------------------------------------------
		if (surf0->IsLost() == DDERR_SURFACELOST)
                  Sleep(1);
		//-------------------------------------------------------------
		goto restore_lost;
	    }
	    //-----------------------------------------------------------------
	    printrdd("IDirectDrawSurface2::Flip()", r);
	    exit();
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
}
//=============================================================================




//=============================================================================
// flip всего и сразу
void flip()
{

    //-------------------------------------------------------------------------
    // debug tools [NS]

    //-------------------------------------------------------------------------
    for (int cnt = 0;    cnt < DEBUG_TOOLS_MAX_COUNT;    cnt++)
    {
	//---------------------------------------------------------------------
	// обновляем только открытые окна
	if (debug_tool_exist[ cnt])
	{
	    InvalidateRect( debug_tool_hwnd[cnt], NULL, FALSE);	// не стирать фон
	    UpdateWindow(   debug_tool_hwnd[cnt]);		// [NS] test
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
//  InvalidateRect( debug_tool_hwnd[0], NULL, FALSE);	// не стирать фон
//  UpdateWindow(   debug_tool_hwnd[0]);		// [NS] test
//  InvalidateRect( debug_tool_hwnd[1], NULL, FALSE);	// не стирать фон
//  UpdateWindow(   debug_tool_hwnd[1]);		// [NS] test
//  InvalidateRect( debug_tool_hwnd[2], NULL, FALSE);	// не стирать фон
//  UpdateWindow(   debug_tool_hwnd[2]);		// [NS] test

    //-------------------------------------------------------------------------
    if (temp.Minimized)
	return;
    //-------------------------------------------------------------------------
    switch_video();
    //-------------------------------------------------------------------------
    if (conf.flip && (temp.rflags & (RF_GDI | RF_CLIP)))
	dd->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN, nullptr);
    //-------------------------------------------------------------------------
    // gdi
    if (temp.rflags & RF_GDI) 
    {
	FlipGdi();
	return;
    }
    //-------------------------------------------------------------------------
    if (surf0 && (surf0->IsLost() == DDERR_SURFACELOST))
	surf0->Restore();
    //-------------------------------------------------------------------------
    if (surf1 && (surf1->IsLost() == DDERR_SURFACELOST))
	surf1->Restore();
    //-------------------------------------------------------------------------
    // hardware blitter
    if (temp.rflags & RF_CLIP) 
    {
	//---------------------------------------------------------------------
	// client area is empty
	if (IsRectEmpty( &temp.client)) 
	    return;
	//---------------------------------------------------------------------
	FlipBlt();
	return;
    }
    //-------------------------------------------------------------------------
    // direct 3d
    if (temp.rflags & (RF_D3D | RF_D3DE)) 
    {
	//-------------------------------------------------------------------------
	// client area is empty
	if (IsRectEmpty( &temp.client)) 
	    return;
	//-------------------------------------------------------------------------
	FlipD3d();
	return;
    }
    //-------------------------------------------------------------------------
    // overlay
    if (temp.rflags & RF_OVR)	// [NS]
    {
	if (sprim) //хз че это но пусть будет для надежносте (по идеи sprim это оверлей?)
	{ 
	    update_overlay();	// тут вообще не было обновления !!!
	}
    }
    //-------------------------------------------------------------------------

    // draw direct to video memory, overlay
    FlipVideoMem();		// может оно должно было вызывать update_overlay?
}
//=============================================================================


//=============================================================================
static HWAVEOUT hwo = nullptr;
static WAVEHDR wq[MAXWQSIZE];
static unsigned char wbuffer[MAXWQSIZE*MAXDSPIECE];
static unsigned wqhead;
static unsigned wqtail;
//=============================================================================
void __fastcall do_sound_none()
{

}
//=============================================================================
void __fastcall do_sound_wave()
{
    MMRESULT r;
    //-------------------------------------------------------------------------
    for (    ;    ;    ) // release all done items
    {
	//---------------------------------------------------------------------
	while ((wqtail != wqhead) && (wq[wqtail].dwFlags & WHDR_DONE))
	{ // ready!
	    //-----------------------------------------------------------------
	    if ((r = waveOutUnprepareHeader(hwo, &wq[wqtail], sizeof(WAVEHDR))) != MMSYSERR_NOERROR)
	    {
		printrmm("waveOutUnprepareHeader()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	    if (++wqtail == conf.soundbuffer)
		wqtail = 0;
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	if ( (wqhead + 1) % conf.soundbuffer != wqtail)
	    break; // some empty item in queue
	//---------------------------------------------------------------------
/* [vv] */
	//---------------------------------------------------------------------
	if (conf.sleepidle)
	    Sleep( SLEEP_DELAY);
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    if (!spbsize)
	return;
    //-------------------------------------------------------------------------
    //   __debugbreak();
    // put new item and play
    PCHAR bfpos = PCHAR(wbuffer + wqhead*MAXDSPIECE);
    memcpy(bfpos, sndplaybuf, spbsize);
    wq[wqhead].lpData = bfpos;
    wq[wqhead].dwBufferLength = spbsize;
    wq[wqhead].dwFlags = 0;
    //-------------------------------------------------------------------------
    if ((r = waveOutPrepareHeader(hwo, &wq[wqhead], sizeof(WAVEHDR))) != MMSYSERR_NOERROR)
    {
	printrmm("waveOutPrepareHeader()", r);
	exit();
    }
    //-------------------------------------------------------------------------
    if ((r = waveOutWrite(hwo, &wq[wqhead], sizeof(WAVEHDR))) != MMSYSERR_NOERROR)
    {
	printrmm("waveOutWrite()", r);
	exit();
    }
    //-------------------------------------------------------------------------
    if (++wqhead == conf.soundbuffer)
	wqhead = 0;
    //-------------------------------------------------------------------------
    
//  int bs = wqhead-wqtail;
//  if (bs < 0) bs += conf.soundbuffer;
//  if (bs < 8) goto again;

}
//=============================================================================




//=============================================================================
// directsound part
// begin
//=============================================================================
static void restore_sound_buffer()
{
//  for (;;) 
//  {
    DWORD status = 0;
    dsbf->GetStatus(&status);
    //-------------------------------------------------------------------------
    if (status & DSBSTATUS_BUFFERLOST)
    {
          printf("%s\n", __FUNCTION__);
          Sleep(18);
          dsbf->Restore();
          // Sleep(200);
    }
    //-------------------------------------------------------------------------
//      else break;
//  }
}
//=============================================================================


//=============================================================================
//static void clear_buffer()
extern void clear_buffer()
{
//  printf("%s\n", __FUNCTION__);
//  __debugbreak();

    restore_sound_buffer();
    HRESULT r;
    void *ptr1, *ptr2;
    DWORD sz1, sz2;
    r = dsbf->Lock( 0, 0, &ptr1, &sz1, &ptr2, &sz2, DSBLOCK_ENTIREBUFFER);
    //-------------------------------------------------------------------------
    if (r != DS_OK)
	return;
    //-------------------------------------------------------------------------
    memset( ptr1, 0, sz1);
    //memset(ptr2, 0, sz2);
    dsbf->Unlock( ptr1, sz1, ptr2, sz2);
}
//=============================================================================




//=============================================================================

static int maxgap;

//=============================================================================
void __fastcall do_sound_ds()
{
    HRESULT r;
    //-------------------------------------------------------------------------
    do
    {
	int play;
	int write;
	//---------------------------------------------------------------------
	if ((r = dsbf->GetCurrentPosition( (DWORD*) &play, (DWORD*) &write)) != DS_OK)
	{
	    //-----------------------------------------------------------------
	    if (r == DSERR_BUFFERLOST)
	    {
		restore_sound_buffer();
		return;
	    }
	    //-----------------------------------------------------------------
	    if (r == DSERR_NODRIVER)	//USB AUDIO отпал	// [NS]
	    {
		if (MessageBox( wnd, "Sound\nGetCurrentPosition DSERR_NODRIVER CRASH.\nReinit sound?", "Unreal", MB_YESNO | MB_ICONERROR | MB_SETFOREGROUND) == IDYES)
		{
		    reinit_sound_ds();
		    restore_sound_buffer();
		    //дальше мы ловим
		    //DirectSoundCreate(): 0x88780078, DSERR_NODRIVER
		    //goto error_skip;
		    return;
		}
	    }
	    //-----------------------------------------------------------------
	    printrds("IDirectSoundBuffer::GetCurrentPosition()", r);
	    exit();
	}
	//---------------------------------------------------------------------
	int gap = write - play;
	//---------------------------------------------------------------------
	if (gap < 0)
	    gap += dsbuffer_sz;
	//---------------------------------------------------------------------
	int pos = int( dsoffset) - play;
	//---------------------------------------------------------------------
	if (pos < 0)
	    pos += dsbuffer_sz;
	//---------------------------------------------------------------------
	maxgap = max( maxgap, gap);
	//---------------------------------------------------------------------
	if (pos < maxgap || pos > 10 * (int) maxgap)
	{
	    dsoffset = unsigned( 3 * maxgap);
	    clear_buffer();
	    dsbf->Stop();
	    dsbf->SetCurrentPosition( 0);
	    break;
	}
	//---------------------------------------------------------------------
	if (pos < 2 * maxgap)
	    break;
	//---------------------------------------------------------------------
	if ((r = dsbf->Play( 0, 0, DSBPLAY_LOOPING)) != DS_OK)
	{
	    //-----------------------------------------------------------------
	    if (r == DSERR_BUFFERLOST)
	    {
		restore_sound_buffer();
		return;
	    }
	    //-----------------------------------------------------------------
	    if (r == AUDCLNT_E_DEVICE_IN_USE)
	    {
		printrds("IDirectSoundBuffer::Play()", r);
		printf("sound device is used exclusively by another apllication, switching to no sound mode\n");
		conf.sound.do_sound = do_sound_none;
		return;
	    }
	    //-----------------------------------------------------------------
	    // хз срабатывает ли оно тут
	    if (r == DSERR_NODRIVER)	//USB AUDIO отпал	// [NS]
	    {
		if (MessageBox( wnd, "Sound\nPlay DSERR_NODRIVER CRASH.\nReinit sound?", "Unreal", MB_YESNO | MB_ICONERROR | MB_SETFOREGROUND) == IDYES)
		{
		    reinit_sound_ds();
		    restore_sound_buffer();
		    //дальше мы ловим
		    //DirectSoundCreate(): 0x88780078, DSERR_NODRIVER
		    return;
		}
	    }
	    //-----------------------------------------------------------------
	    printrds("IDirectSoundBuffer::Play()", r);
	    exit();
	}
	//---------------------------------------------------------------------
	if ((conf.SyncMode == SM_SOUND) && conf.sleepidle)
	{
	    //printf("sleep %d\n", SLEEP_DELAY);
	    Sleep( SLEEP_DELAY);	// SLEEP_DELAY = 2
	}
	//---------------------------------------------------------------------
    }
    while (conf.SyncMode == SM_SOUND);	//do while
    //-------------------------------------------------------------------------
    dsoffset %= dsbuffer_sz;
    //-------------------------------------------------------------------------
    if (!spbsize)
	return;
    //-------------------------------------------------------------------------
    void *ptr1;
    void *ptr2;
    DWORD sz1;
    DWORD sz2;
    //-------------------------------------------------------------------------
    r = dsbf->Lock( dsoffset, spbsize, &ptr1, &sz1, &ptr2, &sz2, 0);
    //-------------------------------------------------------------------------
    if (r != DS_OK)
    {
	//---------------------------------------------------------------------
	if (r == DSERR_BUFFERLOST)
	{
	    restore_sound_buffer();
	    return;
	}
	//-----------------------------------------------------------------
	// хз срабатывает ли оно тут
	if (r == DSERR_NODRIVER)	//USB AUDIO отпал	// [NS]
	{
		if (MessageBox( wnd, "Sound\nLock DSERR_NODRIVER CRASH.\nReinit sound?", "Unreal", MB_YESNO | MB_ICONERROR | MB_SETFOREGROUND) == IDYES)
		{
		    reinit_sound_ds();
		    restore_sound_buffer();
		    //дальше мы ловим
		    //DirectSoundCreate(): 0x88780078, DSERR_NODRIVER
		    return;
		}
	}
	//-----------------------------------------------------------------
	// __debugbreak();
	printf("dsbuffer_sz=%u, dsoffset=%u, spbsize=%u\n", dsbuffer_sz, dsoffset, spbsize);
	printrds("IDirectSoundBuffer::Lock()", r);
	exit();
    }
    //-------------------------------------------------------------------------
    memcpy( ptr1, sndplaybuf, sz1);	// 37.503636 (128)
					// 37.681034 (atarin)
    //-------------------------------------------------------------------------
    // какое то редкое явление
    if (ptr2)	// 20.973913 (128)
    {
	memcpy( ptr2, ((char *)sndplaybuf) + sz1, sz2);	// 26.727273 (atarin)
    }

    //-------------------------------------------------------------------------
    r = dsbf->Unlock( ptr1, sz1, ptr2, sz2);
    //-------------------------------------------------------------------------
    if (r != DS_OK)
    {
	//---------------------------------------------------------------------
	if (r == DSERR_BUFFERLOST)
	{
	    restore_sound_buffer();
	    return;
	}
	//-----------------------------------------------------------------
	// хз срабатывает ли оно тут
	if (r == DSERR_NODRIVER)	//USB AUDIO отпал	// [NS]
	{
		if (MessageBox( wnd, "Sound\nUnlock DSERR_NODRIVER CRASH.\nReinit sound?", "Unreal", MB_YESNO | MB_ICONERROR | MB_SETFOREGROUND) == IDYES)
		{
		    reinit_sound_ds();
		    restore_sound_buffer();
		    //дальше мы ловим
		    //DirectSoundCreate(): 0x88780078, DSERR_NODRIVER
		    return;
		}
	}
	//-----------------------------------------------------------------
	// __debugbreak();
	printf("dsbuffer_sz=%u, dsoffset=%u, spbsize=%u\n", dsbuffer_sz, dsoffset, spbsize);
	printrds("IDirectSoundBuffer::Unlock()", r);
	exit();
    }
    //-------------------------------------------------------------------------
    dsoffset += spbsize;
    dsoffset %= dsbuffer_sz;
}
//=============================================================================
// directsound part
// end
//=============================================================================





//=============================================================================

static unsigned OldRflags = 0;

//=============================================================================
void sound_play()
{
//   printf("%s\n", __FUNCTION__);
    maxgap = 2000;
    restart_sound();
}
//=============================================================================


//=============================================================================
void sound_stop()
{
//   printf("%s\n", __FUNCTION__);
//   __debugbreak();
    //-------------------------------------------------------------------------
    if (dsbf)
    {
	dsbf->Stop(); // don't check
	clear_buffer();
    }
    //-------------------------------------------------------------------------
}
//=============================================================================


//=============================================================================
void do_sound()					
{						
    //-------------------------------------------------------------------------
    if (savesndtype == 1)
    {
	//---------------------------------------------------------------------
	if (fwrite( sndplaybuf, 1, spbsize, savesnd) != spbsize)
	{
	    savesnddialog();	// write error - disk full - close file
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    
	// conf.sound.do_sound = do_sound_wave;
	// do_sound_none()
	// do_sound_wave()
	// do_sound_ds()
    conf.sound.do_sound();	// 40373.822626	(800 MHz)
				// 61170.809365 (2,5 GHz)
			
}
//=============================================================================




//=============================================================================
void OnEnterGui()
{
//	printf("%s->%p\n", __FUNCTION__, D3dDev);
    //-------------------------------------------------------------------------
    sound_stop();
    //-------------------------------------------------------------------------
    if (conf.gs_type == 2) //GS BASS
    {
	gs.pause_mod();	// STOP FCKN BASS NOW!
			// чтоб не пищал в менюшках в дебагере итд...
    }
    //-------------------------------------------------------------------------
    // Вывод GUI при активном d3d exclusive режиме
    // на время вывода GUI переход в non exclusive fullscreen
    if ((temp.rflags & RF_D3DE) && D3dDev)
    {
	OldRflags = temp.rflags;
	SetVideoModeD3d( false);
	SendMessage( wnd, WM_USER, 0, 0);
	flip();
    }
    //-------------------------------------------------------------------------
}
//=============================================================================


//=============================================================================
void OnExitGui( bool RestoreVideo)
{
//    printf("%s->%p, %d\n", __FUNCTION__, D3dDev, RestoreVideo);
    
    //-------------------------------------------------------------------------
    sound_play();
    //-------------------------------------------------------------------------
    if (conf.gs_type == 2) //GS BASS
    {
	gs.play_mod();	// PLAY FCKN BASS NOW!	[NS]
			// на выходе из менюшек
    }
    //-------------------------------------------------------------------------
    if (!RestoreVideo)
    {
	return;
    }
    //-------------------------------------------------------------------------
    if (!D3dDev)
    {
	return;
    }
    //-------------------------------------------------------------------------
    // Возврат из GUI в d3d exclusive режим
    if ((temp.rflags & RF_D3DE))
    {
	SetVideoModeD3d( true);
    }
    //-------------------------------------------------------------------------
    // Переключение RF_D3DE->RF_D3D (в меню настроек переключен видеодрайвер)
    if ((OldRflags & RF_D3DE) && (temp.rflags & RF_D3D))
    {
	SetVideoModeD3d( false);
    }
    //-------------------------------------------------------------------------
}
//=============================================================================




//=============================================================================
#define STATUS_PRIVILEGE_NOT_HELD        ((NTSTATUS)0xC0000061L)
#define SE_INC_BASE_PRIORITY_PRIVILEGE      (14L)
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

typedef NTSTATUS (NTAPI *TRtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN Client, PBOOLEAN WasEnabled);

//=============================================================================
void set_priority()
{
    //-------------------------------------------------------------------------
    if (!conf.highpriority || !conf.sleepidle)
	return;
    //-------------------------------------------------------------------------
    SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);
    ULONG Cpus = SysInfo.dwNumberOfProcessors;

    ULONG HighestPriorityClass = HIGH_PRIORITY_CLASS;
    //-------------------------------------------------------------------------
    if (Cpus > 1) // видимо на 1 проце с риалтаймом ползает вся система
    {
	HMODULE NtDll = GetModuleHandle("ntdll.dll");
	TRtlAdjustPrivilege RtlAdjustPrivilege = (TRtlAdjustPrivilege)GetProcAddress(NtDll, "RtlAdjustPrivilege");

	BOOLEAN WasEnabled = FALSE;
	NTSTATUS Status = RtlAdjustPrivilege(SE_INC_BASE_PRIORITY_PRIVILEGE, TRUE, FALSE, &WasEnabled);
	//---------------------------------------------------------------------
	if (!NT_SUCCESS(Status))
	{
	    err_printf("enabling SE_INC_BASE_PRIORITY_PRIVILEGE, %X", Status);
	    //---------------------------------------------------------------------
	    if (Status == STATUS_PRIVILEGE_NOT_HELD)
	    {
		err_printf("program not run as administrator or SE_INC_BASE_PRIORITY_PRIVILEGE is not enabled via group policy");
	    }
	    //---------------------------------------------------------------------
	    printf("REALTIME_PRIORITY_CLASS not available, fallback to HIGH_PRIORITY_CLASS\n");
	}
	//---------------------------------------------------------------------
	else
	{
	    HighestPriorityClass = REALTIME_PRIORITY_CLASS;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    SetPriorityClass(GetCurrentProcess(), HighestPriorityClass);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
}
//=============================================================================


//=============================================================================
void adjust_mouse_cursor()	// шито оно делаит? о_О
{				// может оно не дает выехать курсору за пределы экрана?

//  printf("adjust_mouse_cursor()\n");
//  unsigned showcurs = conf.input.joymouse || !active || !(conf.fullscr || conf.lockmouse) || dbgbreak;
    unsigned showcurs = conf.input.joymouse || !active || !(conf.lockmouse) || dbgbreak; // [NS] отображать мышу в фулскрине
    //-------------------------------------------------------------------------
    while (ShowCursor(0) >= 0);			// hide cursor
    //-------------------------------------------------------------------------
    if (showcurs)
	while (ShowCursor(1) <= 0);		// show cursor
    //-------------------------------------------------------------------------
    if (active && conf.lockmouse && !dbgbreak)
    {
	RECT rc;
	GetClientRect(wnd, &rc);
	POINT p =
	{
	    rc.left,
	    rc.top
	};
	ClientToScreen(wnd, &p);
	rc.left = p.x;
	rc.top = p.y;
	p.x = rc.right;
	p.y = rc.bottom;
	ClientToScreen(wnd, &p);
	rc.right = p.x;
	rc.bottom = p.y;
	// printf("rc={%d,%d,%d,%d} [%dx%d]\n", rc.left, rc.top, rc.right, rc.bottom, rc.right - rc.left, rc.bottom - rc.top);
	ClipCursor(&rc);
    }
    //-------------------------------------------------------------------------
    else
    {
	ClipCursor(nullptr);
    }
    //-------------------------------------------------------------------------

}
//=============================================================================

static HCURSOR crs[9];
static unsigned char mousedirs[9] = { 10, 8, 9, 2, 0, 1, 6, 4, 5 };

//=============================================================================




//=============================================================================
void updatebitmap()
{
    RECT rc;
    GetClientRect(wnd, &rc);
    DWORD newdx = DWORD(rc.right - rc.left);
    DWORD newdy = DWORD(rc.bottom - rc.top);
   
    //-------------------------------------------------------------------------
    if (hbm && (bm_dx != newdx || bm_dy != newdy))
    {
	DeleteObject(hbm);
	hbm = nullptr;
    }
    //-------------------------------------------------------------------------
    if (sprim)
	return; // don't trace window contents in overlay mode
    //-------------------------------------------------------------------------
    if (hbm)
    {
	DeleteObject(hbm);
	hbm = nullptr; // keeping bitmap is unsafe - screen paramaters may change
    }
    //-------------------------------------------------------------------------
    if (!hbm)
    {
	hbm = CreateCompatibleBitmap(	temp.gdidc,
					int(newdx),
					int(newdy)
				     );
    }
    //-------------------------------------------------------------------------
    HDC dc = CreateCompatibleDC(temp.gdidc);

    bm_dx = newdx;
    bm_dy = newdy;
    HGDIOBJ PrevObj = SelectObject(dc, hbm);
    //SetDIBColorTable(dc, 0, 0x100, (RGBQUAD*)pal0);
    
    //printf("XX ");
    
    // сохранение изображения на экране
    // чтоб потом им востанавливать изображениие под окноами
    // НО ОНО УСТАРЕВАЕТ УЖЕ ЧЕРЕЗ ФРЕЙМ
    // И ПОТОМ МИГАЕТ КААКОЙ ТО ХУЙНЕЙ КОТОРОЙ ДАВНО НА ЭКРРАНЕ НЕТ!!!
    // ТАК ЖЕ ЧАСТО ОНО СОХРАНЯЕТ СОВЕРШЕННО НЕВАЛИДНОЕ ИЗОБРАЖЕНИЕ !!!
    // С КУСКАМИ ЧУЖИХ ОКОН
    
//    BitBlt(	dc,
//		0,
//		0,
//		int(newdx),
//		int(newdy),
//		temp.gdidc,
//		0,
//		0,
//		SRCCOPY
//	  );
	 
    SelectObject( dc, PrevObj);
    DeleteDC( dc);
}
//=============================================================================


/*
 movesize.c
 These values are indexes that represent rect sides. These indexes are
 used as indexes into rgimpiwx and rgimpiwy (which are indexes into the
 the rect structure) which tell the move code where to store the new x & y
 coordinates. Notice that when two of these values that represent sides
 are added together, we get a unique list of contiguous values starting at
 1 that represent all the ways we can size a rect. That also leaves 0 free
 a initialization value.

 The reason we need rgimpimpiw is for the keyboard interface - we
 incrementally decide what our 'move command' is. With the mouse interface
 we know immediately because we registered a mouse hit on the segment(s)
 we're moving.

     4           5
      \ ___3___ /
       |       |
       1       2
       |_______|
      /    6    \
     7           8
*/

//static const int rgimpimpiw[] = {1, 3, 2, 6};
//=============================================================================
static const int rgimpiwx[] = 
{
     0, // WMSZ_KEYSIZE
     0, // WMSZ_LEFT
     2, // WMSZ_RIGHT
    -1, // WMSZ_TOP
     0, // WMSZ_TOPLEFT
     2, // WMSZ_TOPRIGHT
    -1, // WMSZ_BOTTOM
     0, // WMSZ_BOTTOMLEFT
     2, // WMSZ_BOTTOMRIGHT
     0  // WMSZ_KEYMOVE
};
//=============================================================================
static const int rgimpiwy[]   = {0, -1, -1,  1, 1, 1,  3, 3, 3, 1};
//=============================================================================
#define WMSZ_KEYSIZE        0           // ;Internal
#define WMSZ_LEFT           1
#define WMSZ_RIGHT          2
#define WMSZ_TOP            3
#define WMSZ_TOPLEFT        4
#define WMSZ_TOPRIGHT       5
#define WMSZ_BOTTOM         6
#define WMSZ_BOTTOMLEFT     7
#define WMSZ_BOTTOMRIGHT    8
#define WMSZ_MOVE           9           // ;Internal
#define WMSZ_KEYMOVE        10          // ;Internal
#define WMSZ_SIZEFIRST      WMSZ_LEFT   // ;Internal
//=============================================================================
struct MOVESIZEDATA
{
    RECT rcDragCursor;
    POINT ptMinTrack;
    POINT ptMaxTrack;
    int cmd;
};
//=============================================================================


//=============================================================================
static void SizeRectX(MOVESIZEDATA *Msd, ULONG Pt)
{
    PINT psideDragCursor = ((PINT)(&Msd->rcDragCursor));

//  * DO HORIZONTAL

//  * We know what part of the rect we're moving based on
//  * what's in cmd.  We use cmd as an index into rgimpiw? which
//  * tells us what part of the rect we're dragging.

//  * Get the approriate array entry.

    int index = (int)rgimpiwx[Msd->cmd];   // AX
    
//    printf("Msd.cmd = %d, idx_x=%d\n", Msd->cmd, index);

//  * Is it one of the entries we don't map (i.e.  -1)?

    //-------------------------------------------------------------------------
    if (index < 0)
        return;
    //-------------------------------------------------------------------------
    psideDragCursor[index] = GET_X_LPARAM(Pt);

    int indexOpp = index ^ 0x2;

//  * Now check to see if we're below the min or above the max. Get the width
//  * of the rect in this direction (either x or y) and see if it's bad. If
//  * so, map the side we're moving to the min or max.

    int w = psideDragCursor[index] - psideDragCursor[indexOpp];
    //-------------------------------------------------------------------------
    if (indexOpp & 0x2)
    {
	w = -w;
    }
    //-------------------------------------------------------------------------
    int x;
    //-------------------------------------------------------------------------
    if (w < Msd->ptMinTrack.x)
    {
	x = Msd->ptMinTrack.x;
    }
    //-------------------------------------------------------------------------
    else if (w > Msd->ptMaxTrack.x)
    {
	x = Msd->ptMaxTrack.x;
    }
    //-------------------------------------------------------------------------
    else
    {
	return;
    }
    //-------------------------------------------------------------------------
    if (indexOpp & 0x2)
    {
	x = -x;
    }
    //-------------------------------------------------------------------------
    psideDragCursor[index] = x + psideDragCursor[indexOpp];
}
//=============================================================================


//=============================================================================
static void SizeRectY(MOVESIZEDATA *Msd, ULONG Pt)
{
    PINT psideDragCursor = ((PINT)(&Msd->rcDragCursor));

//  * DO VERTICAL

//  * We know what part of the rect we're moving based on
//  * what's in cmd.  We use cmd as an index into rgimpiw? which
//  * tells us what part of the rect we're dragging.

//  * Get the approriate array entry.

    int index = (int)rgimpiwy[Msd->cmd];   // AX

//    printf("idx_y=%d\n", index);

//  * Is it one of the entries we don't map (i.e.  -1)?

    //-------------------------------------------------------------------------
    if (index < 0)
	return;
    //-------------------------------------------------------------------------
    psideDragCursor[index] = GET_Y_LPARAM(Pt);

    int indexOpp = index ^ 0x2;

//  * Now check to see if we're below the min or above the max. Get the width
//  * of the rect in this direction (either x or y) and see if it's bad. If
//  * so, map the side we're moving to the min or max.

    int h = psideDragCursor[index] - psideDragCursor[indexOpp];
    //-------------------------------------------------------------------------
    if (indexOpp & 0x2)
    {
	h = -h;
    }
    //-------------------------------------------------------------------------
    int y;
    if (h < Msd->ptMinTrack.y)
    {
	y = Msd->ptMinTrack.y;
    }
    //-------------------------------------------------------------------------
    else if (h > Msd->ptMaxTrack.y)
    {
	y  = Msd->ptMaxTrack.y;
    }
    //-------------------------------------------------------------------------
    else
    {
	return;
    }
    //-------------------------------------------------------------------------
    if (indexOpp & 0x2)
    {
	y = -y;
    }
    //-------------------------------------------------------------------------
    psideDragCursor[index] = y + psideDragCursor[indexOpp];
}
//=============================================================================


//=============================================================================
static void SizeRect(MOVESIZEDATA *Msd, ULONG Pt)
{
    //-------------------------------------------------------------------------
    if (size_t(Msd->cmd) >= _countof(rgimpiwx))
    {
	return;
    }
    //-------------------------------------------------------------------------
    SizeRectX(Msd, Pt);
    SizeRectY(Msd, Pt);
}
//=============================================================================





//=============================================================================
// обработчик событий
//=============================================================================
static LRESULT CALLBACK WndProc(	HWND hwnd,
					UINT uMessage,
					WPARAM wparam,
					LPARAM lparam
          )
//-----------------------------------------------------------------------------
{
    //-------------------------------------------------------------------------
    // Модификация пунктов главного меню и контексного меню главного окна
    if (uMessage == WM_INITMENU)
    {
	// по ходу мы не можем отличить главное меню от контексного
	// потому меняем оба

		if (wparam == (WPARAM)main_menu)	// так у TSL
		{					// пусть будет тоже
			//-----------------------------------------------------
			// File
			//-----------------------------------------------------
			EnableMenuItem(	main_menu,
					IDM_SAVE_DISKB,
				//	(comp.wd.fdd[1].rawdata in TSL unreal
					(comp.fdd[1].rawdata  ?  MF_ENABLED:
								 MF_GRAYED | MF_DISABLED)
				);
			//-----------------------------------------------------
			EnableMenuItem(	main_menu,
					IDM_SAVE_DISKC,
				//	(comp.wd.fdd[2].rawdata in TSL unreal
					(comp.fdd[2].rawdata  ?  MF_ENABLED:
								 MF_GRAYED | MF_DISABLED)
				);
			//-----------------------------------------------------
			EnableMenuItem(	main_menu,
					IDM_SAVE_DISKD,
				//	(comp.wd.fdd[3].rawdata in TSL unreal
					(comp.fdd[3].rawdata  ?  MF_ENABLED:
								 MF_GRAYED | MF_DISABLED)
				);	
			//-----------------------------------------------------
			ModifyMenu(	main_menu,
					IDM_AUDIOREC,
					MF_BYCOMMAND,
					IDM_AUDIOREC,
					savesndtype  ?  "Stop Audio Recording":
							"Start Audio Recording"
				);
			//-----------------------------------------------------
			// Emulation
			//-----------------------------------------------------
			CheckMenuItem(	main_menu,
					IDM_PAUSE,
					(main_pause_flag  ?  MF_CHECKED:
							     MF_UNCHECKED)
				);
			//-----------------------------------------------------	
			CheckMenuItem(	main_menu,
					IDM_MAXIMUMSPEED,
					(conf.sound.enabled  ?  MF_UNCHECKED:
								MF_CHECKED)
				);
			//-----------------------------------------------------
			ModifyMenu(	main_menu,
					IDM_TAPE_CONTROL,
					(tape_infosize  ?  MF_ENABLED:
							   MF_GRAYED | MF_DISABLED),
					IDM_TAPE_CONTROL,
					comp.tape.play_pointer  ?  "Stop Tape":
								   "Start Tape"
				);	
			//-----------------------------------------------------
			CheckMenuItem(	main_menu,
					IDM_USETAPETRAPS,
					(conf.tape_traps  ?  MF_CHECKED:
							     MF_UNCHECKED)
				);
			//-----------------------------------------------------
			CheckMenuItem(	main_menu,
					IDM_AUTOSTARTTAPE,
					(conf.tape_autostart  ?  MF_CHECKED:
								 MF_UNCHECKED)
				);
			//-----------------------------------------------------
			// View
			//-----------------------------------------------------
			CheckMenuItem(	main_menu,
					IDM_FULLSCREEN,
					(conf.fullscr  ?  MF_CHECKED :
							  MF_UNCHECKED)
				);
			//-----------------------------------------------------
			CheckMenuItem(	main_menu,
					IDM_SCALE1,
					((temp.scale == 1)  ?  MF_CHECKED :
							       MF_UNCHECKED)
				);
			CheckMenuItem(	main_menu,
					IDM_SCALE2,
					((temp.scale == 2)  ?  MF_CHECKED :
							       MF_UNCHECKED)
				);
			CheckMenuItem(	main_menu,
					IDM_SCALE3,
					((temp.scale == 3)  ?  MF_CHECKED :
							       MF_UNCHECKED)
				);
			CheckMenuItem(	main_menu,
					IDM_SCALE4,
					((temp.scale == 4)  ?  MF_CHECKED :
							       MF_UNCHECKED)
				);
			//-----------------------------------------------------
			// Input
			//-----------------------------------------------------
			CheckMenuItem(	main_menu,
					IDM_KEYSTICK,
					((input.keymode == K_INPUT::KM_KEYSTICK)  ?  MF_CHECKED :
										     MF_UNCHECKED)
				);
			//-----------------------------------------------------
			CheckMenuItem(	main_menu,
					IDM_FULL_XT_KEYBOARD,
					(conf.atm.xt_kbd  ?  MF_CHECKED :
							     MF_UNCHECKED)
				);
			//-----------------------------------------------------
			// Debug
			//-----------------------------------------------------	
			// int temp_memband_on =     (*(&conf.led.ay + 6) & 0x80000000);	// КОСТЫЛЬ !!!!
			// int temp_memband_256_on = (*(&conf.led.ay + 7) & 0x80000000);	// КОСТЫЛЬ !!!!
			int temp_memband_on =     (conf.led.memband     & 0x80000000);
			int temp_memband_256_on = (conf.led.memband_256 & 0x80000000);
							
			CheckMenuItem(	main_menu,		
					IDM_MEMORY_BAND,
					(temp_memband_on  ?  MF_CHECKED :
							     MF_UNCHECKED)
				);	
			CheckMenuItem(	main_menu,
					IDM_MEMORY_BAND_1,
					((conf.led.bandBpp == 512)  ?  (temp_memband_on)  ?  MF_CHECKED :
											     MF_UNCHECKED:
								       MF_UNCHECKED)
				);
			CheckMenuItem(	main_menu,
					IDM_MEMORY_BAND_2,
					((conf.led.bandBpp == 256)  ?  (temp_memband_on)  ?  MF_CHECKED :
											     MF_UNCHECKED:
								       MF_UNCHECKED)
				);
			CheckMenuItem(	main_menu,
					IDM_MEMORY_BAND_4,
					((conf.led.bandBpp == 128)  ?  (temp_memband_on)  ?  MF_CHECKED :
											     MF_UNCHECKED:
								       MF_UNCHECKED)
				);
			CheckMenuItem(	main_menu,
					IDM_MEMORY_BAND_8,
					((conf.led.bandBpp == 64)  ?  (temp_memband_on)  ?  MF_CHECKED :
											    MF_UNCHECKED:
								       MF_UNCHECKED)
				);
			//-----------------------------------------------------
			CheckMenuItem(	main_menu,		// КОСТЫЛЬ !!!!
					IDM_MEMORY_BAND_256,
					(temp_memband_256_on  ?  MF_CHECKED :
								 MF_UNCHECKED)
				);
			CheckMenuItem(	main_menu,		// КОСТЫЛЬ !!!!
					IDM_MEMORY_BAND_256_256PPL,
					(temp_memband_256_on  ?  MF_CHECKED :
								 MF_UNCHECKED)
				);
			//-----------------------------------------------------

		
		
		
			//-----------------------------------------------------
			HMENU sys = GetSystemMenu(	wnd,
							FALSE	//0
						    );  
			//-----------------------------------------------------
			if (sys)
			{
			    CheckMenuItem(	sys,
						SCU_SCALE1,
						((temp.scale == 1)  ?  MF_CHECKED :
								       MF_UNCHECKED)
					);
			    CheckMenuItem(	sys,
						SCU_SCALE2,
						((temp.scale == 2)  ?  MF_CHECKED :
								       MF_UNCHECKED)
					);
			    CheckMenuItem(	sys,
						SCU_SCALE3,
						((temp.scale == 3)  ?  MF_CHECKED :
								       MF_UNCHECKED)
					);
			    CheckMenuItem(	sys,
						SCU_SCALE4,
						((temp.scale == 4)  ?  MF_CHECKED :
								       MF_UNCHECKED)
					);
			}
			//-----------------------------------------------------
			
			
		}
    }
    //-------------------------------------------------------------------------
    if (uMessage == WM_QUIT) // never heppens
    {
//	__debugbreak();
//	printf("WM_QUIT\n");
	exit();
    }
//-----------------------------------------------------------------------------   
//        WM_SYNCPAINT
//        WM_NCPAINT
//        WM_ERASEBKGND
//        WM_SYNCPAINT
//        WM_NCPAINT		//рисование заголовка окна и менюшек
//        WM_ERASEBKGND		//стираание фона
//        WM_SYNCPAINT		//хз
//        WM_NCPAINT
//        WM_PAINT

    //-------------------------------------------------------------------------
//    if (uMessage == WM_SYNCPAINT)	//NS test
//    {					//нет описания что это
//	//printf("WM_SYNCPAINT ");
//	return 0;
//    }
    //-------------------------------------------------------------------------
    // очистка при вытягивании окна изза границы экрана 
    // и в main window и в дебагере
    // НО не при тягании окон поверх
    // 		стандартный обработчик рисует херню поверх
    //		при вытягивании окна изза границы экрана
    if (uMessage == WM_ERASEBKGND)					// [NS]
    {
	//printf("WM_ERASEBKGND ");
	return 0;
    }
    //-------------------------------------------------------------------------
    if (uMessage == WM_CLOSE) 	// never heppens
    {
//  	__debugbreak();
    	printf("WM_CLOSE\n");
	return 1;
    }
    //-------------------------------------------------------------------------
//#if 1
    if ((uMessage == WM_SETFOCUS) && (!pause))
    {
	active = 1;
	setpal(0);
//	sound_play();
	adjust_mouse_cursor();
	uMessage = WM_USER;
    }
    //-------------------------------------------------------------------------
    if ((uMessage == WM_KILLFOCUS) && (!pause))
    {
	//---------------------------------------------------------------------
	if (dd)
	    dd->FlipToGDISurface();
	//---------------------------------------------------------------------
	updatebitmap();		// сохранение текущего изображения окна
				// ЧАСТО невалидного
				// для востановления под окнами
				// выпилить нах !!!!
	setpal(1);
	active = 0;
//	sound_stop();
	conf.lockmouse = 0;
	adjust_mouse_cursor();
    }
    //-------------------------------------------------------------------------
    if (uMessage == WM_SIZING)
    {
//	printf("WM_SIZING(0x%X)\n", uMessage);
	return TRUE;
    }
    //-------------------------------------------------------------------------
    static bool Captured = false;
    static int x = 0;
    static int y = 0;
    static ULONG Mode;
    static MOVESIZEDATA Msd;
    
    static bool maximazed_window_mode = false;	// [NS]
     
/*
       WM_NCHITTEST
       WM_SETCURSOR
       WM_MOUSEMOVE    WM_MOUSEFIRST
       WM_NCHITTEST
       WM_SETCURSOR
       WM_MOUSEMOVE    WM_MOUSEFIRST
       WM_NCHITTEST
       WM_SETCURSOR
       
       WM_LBUTTONDOWN
       WM_NCHITTEST
       WM_SETCURSOR
       WM_LBUTTONUP
       
       WM_NCHITTEST
       WM_SETCURSOR
       WM_MOUSEMOVE    WM_MOUSEFIRST
       WM_NCACTIVATE
       WM_ACTIVATE
       WM_ACTIVATEAPP
       WM_ACTIVATEAPP
       WM_KILLFOCUS
efault iMsg 281 (641)
*/   


/*

       WM_NCLBUTTONDOWN
       WM_SYSCOMMAND
       WM_ENTERMENULOOP
       WM_SETCURSOR
       WM_INITMENU
       WM_CAPTURECHANGED
       WM_MENUSELECT
       WM_EXITMENULOOP
       WM_NCHITTEST
       WM_NCHITTEST
       WM_NCHITTEST
       WM_NCHITTEST
       WM_SETCURSOR
       WM_NCLBUTTONUP
      
       WM_NCMOUSEMOVE
       
*/
	
    // для детекции развертывания окна
    // переделать !!!
    // нужно определять другими средствами!!!!
    //-------------------------------------------------------------------------
    if ((uMessage == WM_SIZE) && (wparam == SIZEFULLSCREEN))	// [NS]
    {
	//printf("fullscreen\n");
	maximazed_window_mode = TRUE;
    }
    //-------------------------------------------------------------------------
    if ((uMessage == WM_SIZE) && (wparam == SIZENORMAL))	// [NS]
    {
	//printf("fullscreen EXIT\n");
	maximazed_window_mode = FALSE;
    }
    //-------------------------------------------------------------------------
    // перетягивание окна за любую его часть				[NS]
    // сделано методом научного тыка
    //
    if ( (!maximazed_window_mode)	&& 	// НЕЛЬЗЯ ТЯНУТЬ РАЗВЕРНУТОЕ ОКНО!!!
	 (!conf.fullscr) 		&&	// и в фуллскрине
	 (!conf.lockmouse)		&&	// и когда мыша захвачена
	 (conf.All_Screen_Drag)
      )	 	
    {								
	//---------------------------------------------------------------------
	if (uMessage == WM_LBUTTONDOWN)
	{				
	    //printf("WM_LBUTTONDOWN\n");
	    POINT p =
	    {
		GET_X_LPARAM(lparam),
		GET_Y_LPARAM(lparam)
	    };
	    ClientToScreen(hwnd, &p);
	    x = p.x;
	    y = p.y;
	    Mode = SC_MOVE;
	    SetCapture(hwnd); 
	    Captured = true;
	}
	//---------------------------------------------------------------------

/*	if (uMessage == WM_NCLBUTTONDOWN)	// так можно таксать и за менюшку
	{					// НО нужна дополнительная проверка
	    //printf("WM_NCLBUTTONDOWN\n");	// а не ткнули ли мы в "кнопку" менюшки
	    POINT p =			// иначе менюшка неработает !!!
	    {
		GET_X_LPARAM(lparam),
		GET_Y_LPARAM(lparam)
	    };
	    //ClientToScreen(hwnd, &p);
	    x = p.x;
	    y = p.y;
	    Mode = SC_MOVE;
	    SetCapture(hwnd); 
	    Captured = true;
	    //return DefWindowProc(hwnd, uMessage, wparam, lparam);
	}
*/

    }
    //-------------------------------------------------------------------------
    if ((
	    (uMessage == WM_LBUTTONUP) 	||
	    (uMessage == WM_NCLBUTTONUP)	// [NS]
	)
	&&
	Captured
      )
    {
//	printf("WM_LBUTTONUP(0x%X)\n", uMessage);
	Captured = false;
	ReleaseCapture(); 
	return 0;
    }
//-----------------------------------------------------------------------------
// debug deflicker			//мерзская мигалка была в gdi_frame()
					//если оно выпилить то и тут костыли не нада
/*
   if(uMessage == WM_WINDOWPOSCHANGED)		//NS
   {
	if (dbgbreak)			// КРИВОЙ КОСТЫЛЬ !
	{				// в режиме дебагера тушит мигалку 
	    needclr = 1;		// которую видимо не видно в win15?
	    return 0;			// НО сделано методом научного тыка...
	}				// и хз чего вообще происходит
   }					// но тк "у меня работает" то трижды пох
  */ 
//-----------------------------------------------------------------------------
// еще вариант для подавления мигалок?
// но как то нужно заполнить структуру? WINDOWPOS
// а интернетов под рукой нету
//
//if (uMessage == WM_WINDOWPOSCHANGING)
//   {
//   //WINDOWPOS.flags |= SWP_NOCOPYBITS;
//   return 0;
//   }
//-----------------------------------------------------------------------------
    if ((uMessage == WM_MOUSEMOVE)	||
	(uMessage == WM_NCMOUSEMOVE)		// [NS]
      )
    {
	//---------------------------------------------------------------------
	if (Captured)
	{
//	    printf("WM_MOUSEMOVE(0x%X), w=%d\n", uMessage, wparam);
	    //-----------------------------------------------------------------
	    // кастомная обработка события перемещения
	    // тк обработчик венды останавливает всё на время cmd == SC_MOVE ?
	    if (Mode == SC_MOVE)
	    {
		POINT p =
		{
		    GET_X_LPARAM(lparam),
		    GET_Y_LPARAM(lparam)
		};
		ClientToScreen(hwnd, &p);
//		printf("nx=%d,ny=%d\n", p.x, p.y);
		int dx = p.x - x;
		int dy = p.y - y;
//		printf("dx=%d,dy=%d\n", dx, dy);
		//-------------------------------------------------------------
		if ((dx == 0) && (dy == 0))
		{
                   return 0;
		   //goto window_not_moving;
		}
		//-------------------------------------------------------------
		RECT r = {};
		GetWindowRect(hwnd, &r);

		// кастомная обработка события перемещения
		// дикая мигалка (была)
		SetWindowPos(	hwnd,
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

		x = p.x;
		y = p.y;
		return 0;
	    }
	    //-----------------------------------------------------------------
	    // кастомная обработка события ресайза
	    else if (Mode == SC_SIZE)
	    {
		POINT p =
		{
		    GET_X_LPARAM(lparam),
		    GET_Y_LPARAM(lparam)
		};
		ClientToScreen(hwnd, &p);
		ULONG pt = ULONG(p.y << 16) | (p.x & 0xFFFF);
		SizeRect(&Msd, pt);
		const RECT &r = Msd.rcDragCursor;
		//printf("r={%d,%d,%d,%d} [%dx%d]\n", r.left, r.top, r.right, r.bottom, r.right - r.left, r.bottom - r.top);
               
		conf.win_static_size_x = r.right - r.left;
		conf.win_static_size_y = r.bottom - r.top;
	       
		// кастомная обработка события ресайза
		SetWindowPos(	hwnd,
				nullptr,
				r.left,
				r.top,
				r.right - r.left,
				r.bottom - r.top,
				SWP_NOOWNERZORDER 	|
				 SWP_NOZORDER 		|
				  SWP_NOSENDCHANGING		//???
			     );
		return 0;
	    }
	}
    }

window_not_moving:

    //-------------------------------------------------------------------------
    // WM_NCLBUTTONDOWN -> WM_SYSCOMMAND(SC_MOVE) -|-> WM_CAPTURECHANGED -> WM_GETMINMAXINFO -> WM_ENTERSIZEMOVE
    // При получении WM_SYSCOMMAND(SC_MOVE) стандартная процедура обработки сообщения запускает модальный цикл
    if (uMessage == WM_SYSCOMMAND)
    {
	// printf("WM_SYSCOMMAND(0x%X), w=%X, l=%X\n", uMessage, wparam, lparam);
	//
	// Кодирование сторон рамки окна для SC_SIZE (код угла получается суммированием кодов смежных сторон)
	//     4           5
	//      \ ___3___ /
	//       |       |
	//       1       2
	//       |_______|
	//      /    6    \
	//     7           8

	ULONG Cmd = (wparam & 0xFFF0);
	ULONG BrdSide = (wparam & 0xF);
	//---------------------------------------------------------------------
	if (Cmd == SC_MOVE)		// видимо не даем тягать окно обработчиком венды
	{				// тк пока держишь зажатую мышу
	    //printf("SC_MOVE\n");	// управление программе не передаетсо
	    x = GET_X_LPARAM(lparam);	// и ставим переменные
	    y = GET_Y_LPARAM(lparam);	// для кастомного обработчика
	    SetCapture(hwnd); 
	    Captured = true;
	    Mode = SC_MOVE;
	    return 0;
	}
	//---------------------------------------------------------------------
	else if (Cmd == SC_SIZE)
	{
	    //printf ("(Cmd == SC_SIZE)\n");
	    Mode = SC_SIZE;
	
	    RECT rcWindow;
	    GetWindowRect(hwnd, &rcWindow);
	    RECT rcClient;
	    GetClientRect(hwnd, &rcClient);
	    RECT rcDesktop;
	    GetWindowRect(GetDesktopWindow(), &rcDesktop);

	    Msd.cmd = int(BrdSide);
	    CopyRect(&Msd. rcDragCursor, &rcWindow);
	    LONG cw = rcClient.right - rcClient.left;
	    LONG ch = rcClient.bottom - rcClient.top;
	    LONG ww = rcWindow.right - rcWindow.left;
	    LONG wh = rcWindow.bottom - rcWindow.top;
	    LONG dw = rcDesktop.right - rcDesktop.left;
	    LONG dh = rcDesktop.bottom - rcDesktop.top;
	    Msd.ptMinTrack = 
	    {
		LONG(temp.ox) + (ww - cw),
		LONG(temp.oy) + (wh - ch)
	    };
	    Msd.ptMaxTrack =
	    {
		dw,
		dh
	    };
	    SetCapture(hwnd);
	    Captured = true;
	    SizeRect(&Msd, ULONG(lparam));
	    return 0;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------

/*
#define WM_NCUAHDRAWCAPTION 0x00AE
#define WM_NCUAHDRAWFRAME   0x00AF
   if(!(uMessage == WM_SIZING || uMessage == WM_SIZE || uMessage == WM_PAINT || uMessage == WM_NCPAINT
     || uMessage == WM_MOUSEMOVE || uMessage == WM_SETCURSOR || uMessage == WM_GETICON
     || uMessage == WM_IME_SETCONTEXT || uMessage == WM_IME_NOTIFY || uMessage == WM_ACTIVATE
     || uMessage == WM_NCUAHDRAWCAPTION || uMessage == WM_NCUAHDRAWFRAME))
   {
       printf("MSG(0x%X)\n", uMessage);
   }
*/
    //-------------------------------------------------------------------------
    if (conf.input.joymouse)
    {
	//---------------------------------------------------------------------
	if (uMessage == WM_LBUTTONDOWN)
	{
	    //printf("input.joymouse\n");
	    input.mousejoy = (input.mousejoy & 0x0F) | 0x10;	//(uMessage == WM_LBUTTONDOWN ? 0x10 : 0);
	    input.kjoy = (input.kjoy & 0x0F) | 0x10;		//(uMessage == WM_LBUTTONDOWN ? 0x10 : 0);
	}
	//---------------------------------------------------------------------
	if (uMessage == WM_LBUTTONUP)
	{
	    //printf("input.joymouse\n");
	    input.mousejoy = (input.mousejoy & 0x0F) | 0;	//(uMessage == WM_LBUTTONDOWN ? 0x10 : 0);
	    input.kjoy = (input.kjoy & 0x0F) | 0;		//(uMessage == WM_LBUTTONDOWN ? 0x10 : 0);
	}
	//---------------------------------------------------------------------
	if (uMessage == WM_MOUSEMOVE)
	{
	    RECT rc; GetClientRect(hwnd, &rc);
	    unsigned xx = LOWORD(lparam) * 3 / unsigned(rc.right - rc.left);
	    unsigned yy = HIWORD(lparam) * 3 / unsigned(rc.bottom - rc.top);
	    unsigned nn = yy * 3 + xx;
//	    SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG)crs[nn]); //Alone Coder
	    SetCursor(crs[nn]); //Alone Coder
	    input.mousejoy = (input.mousejoy & 0x10) | mousedirs[nn];
	    input.kjoy = (input.kjoy & 0x10) | mousedirs[nn];
	    return 0;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
//  else if (uMessage == WM_LBUTTONDOWN && !conf.lockmouse)	// уебащный захват мыши
								// при попытке перевести фокус на эмулятор
    //-------------------------------------------------------------------------						
    // нормальный захват мыши [NS]
    else if (	(uMessage == WM_LBUTTONDBLCLK)	&&
		(!conf.lockmouse)		&&
		(!dbgbreak)		// в дебагере нельзя делать захват!!!!
       )
    {
	printf("%s\n", __FUNCTION__);
	input.nomouse = 20;
	main_mouse();
    }
    //-------------------------------------------------------------------------
    if (uMessage == WM_ENTERSIZEMOVE)	// щас не срабатывает?
    {
	//printf("WM_ENTERSIZEMOVE(0x%X)\n", uMessage);	
	sound_stop();
    }
    //-------------------------------------------------------------------------
    if (uMessage == WM_EXITSIZEMOVE)	// щас не срабатывает?
    {
	//printf("WM_EXITSIZEMOVE(0x%X)\n", uMessage);
	sound_play();
    }
    //-------------------------------------------------------------------------
    if (	(uMessage == WM_SIZE) || 
		(uMessage == WM_MOVE) ||
		(uMessage == WM_USER)
      )
    {

/*
	//debug
	printf("%s(WM_SIZE || WM_MOVE || WM_USER)\n", __FUNCTION__);
	RECT rr = {};
	GetWindowRect(wnd, &rr);
	printf("r={%d,%d,%d,%d} [%dx%d]\n", rr.left, rr.top, rr.right, rr.bottom, rr.right - rr.left, rr.bottom - rr.top);
*/

	GetClientRect(wnd, &temp.client);
     
/*
	//debug
	printf("temp.client={%d,%d,%d,%d} \n",	temp.client.left,
						temp.client.top,
						temp.client.right,
						temp.client.bottom);
*/
	temp.gdx = unsigned(temp.client.right-temp.client.left);
	temp.gdy = unsigned(temp.client.bottom-temp.client.top);

	temp.gx = (temp.gdx > temp.ox)    ?	(temp.gdx-temp.ox) / 2 :
						0;
	temp.gy = (temp.gdy > temp.oy)    ?	(temp.gdy-temp.oy) / 2 :
						0;

	ClientToScreen(wnd, (POINT*)&temp.client.left);
	ClientToScreen(wnd, (POINT*)&temp.client.right);

//	printf("temp.client.left %d\n",temp.client.left);
//	printf("temp.client.right %d\n",temp.client.right);
	
	adjust_mouse_cursor();
	//---------------------------------------------------------------------
	if (sprim)			// по моему это overlay driver
	    uMessage = WM_PAINT;
	//---------------------------------------------------------------------
	needclr = 2;
	//uMessage = WM_PAINT;	//test
	//---------------------------------------------------------------------
	if (	(uMessage == WM_SIZE)		&&
		(wparam != SIZE_MINIMIZED)	&&
		temp.ox				&&
		temp.oy
	  )
	{
	    //-----------------------------------------------------------------
	    if (wnd && (temp.rflags & RF_D3D)) // skip call from CreateWindow
	    {
		//printf("%s(WM_SIZE, temp.rflags & RF_D3D)\n", __FUNCTION__);
		SetVideoModeD3d(false);
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------

/*
лог событий при тыкании по главной менюшке
        WM_NCHITTEST
        WM_SETCURSOR
        WM_NCLBUTTONDOWN
        WM_SYSCOMMAND
	
        WM_ENTERMENULOOP	
        WM_SETCURSOR
        WM_INITMENU
			дето до этого происходит оставляние пред окна
        WM_MENUSELECT		!!! тут можно затирать мусор
        WM_INITMENUPOPUP	!!! тут можно затирать мусор
        WM_NCMOUSELEAVE
        WM_ENTERIDLE		!!! тут можно затирать мусор
        WM_ENTERIDLE
        WM_ENTERIDLE
        WM_UNINITMENUPOPUP
        WM_CAPTURECHANGED

        WM_MENUSELECT
	
        WM_EXITMENULOOP
	
        WM_NCACTIVATE
        WM_ACTIVATE
        WM_ACTIVATEAPP
        WM_ACTIVATEAPP
        WM_KILLFOCUS

*/

    //-------------------------------------------------------------------------
    if (!dbgbreak)						//[NS]	Костыли
    {
	if (uMessage == WM_ENTERMENULOOP)	// для тушения зацикленного буфера звука
	{					// на время менюшки
	    //printf("WM_ENTERMENULOOP ");
	    OnEnterGui();
	}
	//---------------------------------------------------------------------
//	// [NS] просто какието мудаки в WM_PAINT не делали flip() перерисовку экрана
//	// а делали "скриншот" во время WM_KILLFOCUS и потом его иногда показывали всегда когда не надо
//	if (uMessage == WM_INITMENUPOPUP)	// для избавления от всегда висячей
//	{					// 1-й ткнутой менюшки на фоне
//	    //printf("WM_INITMENUPOPUP ");	// в не GDI режимах
//	    flip();
//	}
	//---------------------------------------------------------------------
	if (uMessage == WM_EXITMENULOOP)	// на всякий случай по выходу 
	{					// официально звук включаем
	    //printf("WM_EXITMENULOOP ");		// хотя он, на вид, играет и без этого
	    OnExitGui();
	}
    }
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    if (uMessage == WM_PAINT)
    {	
	//printf("WM_PAINT ");
	//---------------------------------------------------------------------
	// затирание необновляемых хвостов в дебагере [NS]
	if (dbgbreak)	
	{
	    debugflip();	//может вынести в flip() по такому же условию
	}
	// затирание необновляемых хвостов в эмуляторе
	else
	{
	    flip();
	}
	//---------------------------------------------------------------------
	if (sprim)						// OVERLAY
	{ // background for overlay
	    //printf("WM_PAINT overlay\n");
	    RECT rc;
	    GetClientRect(hwnd, &rc);
	    //HBRUSH br = CreateSolidBrush(RGB(0xFF,0x00,0xFF));
	    HBRUSH br = CreateSolidBrush(RGB(0x04,0x01,0x02));
	    //рисуем квадратик через который будем смотреть в европу
	    //а лучше это было бы делоть "кистью" которая вообще закрашивает окно самой первой
	    FillRect(	temp.gdidc,
			&rc,
			br
		     );
	    DeleteObject(br);
	    update_overlay();
	}
	//---------------------------------------------------------------------
	else if (hbm && !active)
	{
	    // отрисовывает экран во время перемещения других окон по поверхности
	    // оно же перерисовывает дебагер при перемещении окон сверху
	    // НО ОНО рисует устаревшую картинку !!!!
	    // если убрать в дебагере белвые следы!!!
	    // и белвые хвосты на зх экране (особенно видно при низкой частоте инта)
	    //printf("%s, WM_PAINT\n", __FUNCTION__);
	    
	    //printf("!! ");
	    HDC hcom = CreateCompatibleDC(temp.gdidc);
	    HGDIOBJ PrevObj = SelectObject(hcom, hbm);

	    //BitBlt(hdcDest, xDest, yDest, xWidth, yHeight, hdcSrc, xSrc, ySrc, dwROP);

	    // [NS] это гафно ЧАСТО всегда рисует УЖЕ устаревшее изобрражение
//	    BitBlt(	temp.gdidc,	// hdcDest	//оригиинальный код
//			0,		// xDest
//			0,		// yDest
//			int(bm_dx),	// xWidth
//			int(bm_dy),	// yHeight
//			hcom,		// hdcSrc
//			0,		// xSrc
//			0,		// ySrc
//			SRCCOPY		// dwROP
//		    );


		    
		  // [NS] not works	(это мы пытались делать debug_flip() но без полной перестрройки буфера)
		  /*
    SetDIBitsToDevice(	temp.gdidc,
			
			int(temp.gx),
			int(temp.gy),
			temp.ox,
			temp.oy,
			
			0,
			0,
			temp.ox,
			temp.oy,
			
			gdi_dbg_buf,		//gdibuf,
			&gdi_dbg_bmp.header,	//&gdibmp.header,
			DIB_RGB_COLORS
		      );
		      */
		      
	  
	    SelectObject(hcom, PrevObj);
	    DeleteDC(hcom);
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    if (uMessage == WM_SYSCOMMAND)
    {
	//  printf("%s, WM_SYSCOMMAND 0x%04X\n", __FUNCTION__, (ULONG)wparam);

	switch (wparam & 0xFFF0)
	{
	    //-----------------------------------------------------------------
	    case SCU_SCALE1:	temp.scale = 1; wnd_resize(1);	return 0;
	    case SCU_SCALE2:	temp.scale = 2; wnd_resize(2);	return 0;
	    case SCU_SCALE3:	temp.scale = 3; wnd_resize(3);	return 0;
	    case SCU_SCALE4:	temp.scale = 4; wnd_resize(4);	return 0;
	    //-----------------------------------------------------------------
	    case SCU_LOCK_MOUSE:
		main_mouse();
		return 0;
	    //-----------------------------------------------------------------
	    case SC_CLOSE:
		//-------------------------------------------------------------
		if (ConfirmExit())
		    correct_exit();
		//-------------------------------------------------------------
		return 0;
	    //-----------------------------------------------------------------
	    case SC_MINIMIZE:
		temp.Minimized = true;
		break;
	    //-----------------------------------------------------------------
	    case SC_RESTORE:
		temp.Minimized = false;
		break;
	    //-----------------------------------------------------------------
	}
    }
    //-------------------------------------------------------------------------
    if (uMessage == WM_DROPFILES)
    {
	HDROP hDrop = (HDROP)wparam;
	DragQueryFile( hDrop, 0, droppedFile, sizeof(droppedFile));
	DragFinish( hDrop);
	return 0;
    }
    //-------------------------------------------------------------------------
//#endif
    //-------------------------------------------------------------------------
    if (uMessage == WM_COMMAND)	// позаимствовано у TSL
    {
	int disk = -1;
	//---------------------------------------------------------------------
	switch(wparam)
	{
	    //=================================================================
	    // FILE
	    //=================================================================
	    //    LOAD
	    case IDM_LOAD:		opensnap();		break;
	    //-----------------------------------------------------------------
	    //    SAVE
	    case IDM_SAVE:		savesnap();		break;
	    //-----------------------------------------------------------------
	    case IDM_SAVE_DISKB:	disk = 1;		goto save_disk;
	    case IDM_SAVE_DISKC:	disk = 2;		goto save_disk;
	    case IDM_SAVE_DISKD:	disk = 3;		goto save_disk;
save_disk:
		sound_stop();
		savesnap(disk);
		eat();
		sound_play();
		break;
	    //-----------------------------------------------------------------
	    //    QUICKLOAD
	    case IDM_QUICKLOAD_1:	qload1();		break;
	    case IDM_QUICKLOAD_2:	qload2();		break;
	    case IDM_QUICKLOAD_3:	qload3();		break;
	    //-----------------------------------------------------------------
	    //    QUICKSAVE
	    case IDM_QUICKSAVE_1:	qsave1();		break;
	    case IDM_QUICKSAVE_2:	qsave2();		break;
	    case IDM_QUICKSAVE_3:	qsave3();		break;
	    //-----------------------------------------------------------------
	    case IDM_AUDIOREC:		savesnddialog();	break;
	    //-----------------------------------------------------------------
	    case IDM_MAKESCREENSHOT:	main_scrshot();		break;
	    //-----------------------------------------------------------------
	    case IDM_EXIT: 		correct_exit();		break;
	    //=================================================================
	    // Emulation
	    //=================================================================
	    case IDM_PAUSE:		main_pause(); 		break;
	    //-----------------------------------------------------------------
	    case IDM_MAXIMUMSPEED:	main_maxspeed();	break;
	    //-----------------------------------------------------------------
	    //    RESET
	    case IDM_RESET:		main_reset();		break;
	    case IDM_RESET_128:		main_reset128();	break;
	    case IDM_RESET_48:		main_resetbas();	break;
	    case IDM_RESET_48_LOCK:	main_reset48();		break; // [NS]
	    case IDM_RESET_DOS:		main_resetdos();	break;
	    case IDM_RESET_SERVICE:	main_resetsys();	break;
	    case IDM_RESET_CACHE:	main_resetcache();	break;
	    case IDM_RESET_GS:		reset_gs();		break;
	    //-----------------------------------------------------------------
	    //    NMI
	    case IDM_NMI: 		main_nmi();		break;
	    case IDM_NMI_DOS:		main_nmidos();		break;
	    case IDM_NMI_CACHE:		main_nmicache();	break;
	    //-----------------------------------------------------------------
	    case IDM_POKE: main_poke(); break;
	    //-----------------------------------------------------------------
	    //    TAPE
	    case IDM_TAPE_CONTROL:	main_starttape();		break;
	    case IDM_USETAPETRAPS:	conf.tape_traps ^= 1;		break;
	    case IDM_AUTOSTARTTAPE:	conf.tape_autostart ^= 1;	break;
	    case IDM_TAPE_BROWSER:	main_tapebrowser();		break;
	    //-----------------------------------------------------------------
	    case IDM_SETTINGS:
		if (!dbgbreak)
		    setup_dlg();	//настройки с выходом в ебулятор (дебагер расколбасит)
		else
		    mon_setup_dlg(); 	//настройки с выходом в дебагер
		break;
	    //=================================================================
	    // VIEW
	    //=================================================================
	    case IDM_FULLSCREEN: main_fullscr(); break;
	    //-----------------------------------------------------------------
	    //	  SIZE
	    case IDM_SCALE1:	temp.scale = 1; wnd_resize(1);	break;
	    case IDM_SCALE2:	temp.scale = 2; wnd_resize(2);	break;
	    case IDM_SCALE3:	temp.scale = 3; wnd_resize(3);	break;
	    case IDM_SCALE4:	temp.scale = 4; wnd_resize(4);	break;
	    //-----------------------------------------------------------------
	    case IDM_VIDEO_DRIVER: main_selectdriver(); break;
	    //-----------------------------------------------------------------
	    case IDM_VIDEOFILTER: main_selectfilter(); break;
	    //=================================================================
	    // Input
	    //-----------------------------------------------------------------
	    case IDM_KEYSTICK:		main_keystick();		break;
	    //-----------------------------------------------------------------
	    case IDM_FULL_XT_KEYBOARD:	main_atmkbd();			break;
	    //=================================================================
	    // Tools
	    //=================================================================
	    case IDM_TOOLS_PAL_ATM3_PAL: run_debug_tool( DT_PALETTE_ATM3_PAL);	break;
	    case IDM_TOOLS_PAL_COMP_PAL: run_debug_tool( DT_PALETTE_COMP_PAL);	break;
	    case IDM_TOOLS_PAL_PAL0:	 run_debug_tool( DT_PALETTE_PAL0);	break;
	    //-----------------------------------------------------------------
	    case IDM_TEST_DEBUG_TOOL:	 run_debug_tool( DT_TEST);		break;
	    //-----------------------------------------------------------------
	    case IDM_TEST_BUTTON_1:	mon_show_labels();		break;	
	    case IDM_TEST_BUTTON_2:					break;	
	    case IDM_TEST_BUTTON_3:					break;	
	    case IDM_TEST_BUTTON_4:					break;	
	    //-----------------------------------------------------------------
	    case IDM_GS_BASS_STATUS:	mon_gsdialog();			break;	
	    //=================================================================
	    // Debug
	    //=================================================================
	    case IDM_MEMORY_SEARCH: main_cheat(); break;
	    //-----------------------------------------------------------------
	    case IDM_MEMORY_BAND:
		//сделано костылем !!!!
		// *(&conf.led.ay + 6) ^= 0x80000000;	//mbnd
		// *(&conf.led.ay + 7) &= 0x7FFFFFFF;	//mbnd256
		conf.led.memband     ^= 0x80000000;	//mbnd
		conf.led.memband_256 &= 0x7FFFFFFF;	//mbnd256
		apply_video();	// без него не включаетсо ?
		break;
force_memory_band:
		// *(&conf.led.ay + 6) |= 0x80000000;	//mbnd
		// *(&conf.led.ay + 7) &= 0x7FFFFFFF;	//mbnd256
		conf.led.memband     |= 0x80000000;	//mbnd
		conf.led.memband_256 &= 0x7FFFFFFF;	//mbnd256
		apply_video();
		break;
	    //-----------------------------------------------------------------
	    case IDM_MEMORY_BAND_1:   conf.led.bandBpp = 512;	goto force_memory_band; //break;
	    case IDM_MEMORY_BAND_2:   conf.led.bandBpp = 256;	goto force_memory_band; //break;
	    case IDM_MEMORY_BAND_4:   conf.led.bandBpp = 128;	goto force_memory_band; //break;
	    case IDM_MEMORY_BAND_8:   conf.led.bandBpp = 64;	goto force_memory_band; //break;
	    //-----------------------------------------------------------------
	    case IDM_MEMORY_BAND_256: 	// КОСТЫЛЬ !!!!
		// *(&conf.led.ay + 6) &= 0x7FFFFFFF; 	//mbnd
		// *(&conf.led.ay + 7) ^= 0x80000000; 	//mbnd256
		conf.led.memband     &= 0x7FFFFFFF; 	//mbnd
		conf.led.memband_256 ^= 0x80000000; 	//mbnd256
		apply_video();
		break;
force_memory_band_256:
		// *(&conf.led.ay + 6) &= 0x7FFFFFFF; 	//mbnd
		// *(&conf.led.ay + 7) |= 0x80000000; 	//mbnd256
		conf.led.memband     &= 0x7FFFFFFF; 	//mbnd
		conf.led.memband_256 |= 0x80000000; 	//mbnd256
		apply_video();
		break;
	    //-----------------------------------------------------------------
	    case IDM_MEMORY_BAND_256_256PPL:	goto force_memory_band_256;
	    //-----------------------------------------------------------------
	    case IDM_BREAKPOINTS:	mon_bpdialog();			break;
	    //-----------------------------------------------------------------
	    case IDM_WATCH_DIALOG:	mon_watchdialog();		break;
	    //-----------------------------------------------------------------
	    case IDM_DEBUGGER:	
		if (!dbgbreak)
		    main_debug();	//вход в дебагер
		else
		    mon_emul(); 	//выход из дебагера
		dbg_force_exit = 1;
		break;
	    //=================================================================
	    // Help
	    //=================================================================
	    case IDM_HELP_SHORTKEYS:	main_help();	 break;
	    //-----------------------------------------------------------------
	    case IDM_HELP_ABOUT:	main_about();	 break;
		//DialogBox(hIn, MAKEINTRESOURCE(IDD_ABOUT), wnd, aboutdlg); break;


	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
		//needclr=1;
    }
    //-------------------------------------------------------------------------

    return DefWindowProc( hwnd, uMessage, wparam, lparam);
}
//=============================================================================





//=============================================================================
void readdevice( VOID *md, DWORD sz, LPDIRECTINPUTDEVICE dev)
{
    //-------------------------------------------------------------------------
    if (!active || !dev)
	return;
    //-------------------------------------------------------------------------
    HRESULT r = dev->GetDeviceState( sz, md);
    //-------------------------------------------------------------------------
    if ((r == DIERR_INPUTLOST) || (r == DIERR_NOTACQUIRED))
    {
	//---------------------------------------------------------------------
	r = dev->Acquire();
	//---------------------------------------------------------------------
	while (r == DIERR_INPUTLOST)
	    r = dev->Acquire();
	//---------------------------------------------------------------------
	if (r == DIERR_OTHERAPPHASPRIO) // Приложение находится в background
	    return;
	//---------------------------------------------------------------------
	if (r != DI_OK)
	{
	    printrdi("IDirectInputDevice::Acquire()", r);
	    exit();
	}
	//---------------------------------------------------------------------
	r = dev->GetDeviceState( sz, md);
    }
    //---------------------------------------------------------------------
    if (r != DI_OK)
    {
	printrdi("IDirectInputDevice::GetDeviceState()", r);
	exit();
    }
    //---------------------------------------------------------------------
}
//=============================================================================




//=============================================================================
void readmouse( DIMOUSESTATE *md)
{
    memset( md, 0, sizeof *md);
    readdevice( md, sizeof *md, dimouse);
}
//=============================================================================
void ReadKeyboard( PVOID KbdData)
{
    //memset( KbdData, 0, 256);	//TZT не помогает
    readdevice( KbdData, 256, dikeyboard);			
}
//=============================================================================
void setpal(char system)
{
    //-------------------------------------------------------------------------
    if (!active || !dd || !surf0 || !pal)
	return;
    //-------------------------------------------------------------------------
    HRESULT r;
    //-------------------------------------------------------------------------
    if (surf0->IsLost() == DDERR_SURFACELOST)
	surf0->Restore();
    //-------------------------------------------------------------------------
    if (  ( r = pal->SetEntries( 0, 0, 0x100, system  ?	syspalette :
							pal0)
	  ) != DD_OK
       )
    {
	printrdd("IDirectDrawPalette::SetEntries()", r);
	exit();
    }
    //-------------------------------------------------------------------------
}
//=============================================================================


//=============================================================================
static void trim_right(char *str)
{
    size_t i; //Alone Coder 0.36.7
    //-------------------------------------------------------------------------
    for (/*unsigned*/ i = strlen(str);    i && str[i - 1] == ' ';    i--);
	str[i] = 0;
    //-------------------------------------------------------------------------
}
//=============================================================================



//=============================================================================
#define MAX_MODES 512

static struct MODEPARAM 
{
    unsigned x;
    unsigned y;
    unsigned b;
    unsigned f;
} modes[MAX_MODES];

static unsigned max_modes;

// Для инициализации fullscreen режима необходимо выполнение несколтких условий:
// 1. Размер окна должен совпадать с разрешением экрана, координаты левого верхнего угла должны быть 0, 0
// 2. Окно должно иметь расширенный стиль WS_EX_TOPMOST
// При не выполнении хотя бы одного из пунктов при возврате из fullscreen к окнному режиму на vista и выше будет 
// откючен DWM и окно приобретет "устаревший" вид бордюра

//=============================================================================
static void SetVideoModeD3d(bool Exclusive)
{
    HRESULT r;
//    printf("%s\n", __FUNCTION__);

    //-------------------------------------------------------------------------
    if (!wnd)
    {
	__debugbreak();
    }
    //-------------------------------------------------------------------------
    // release textures if exist
    if (SurfTexture)
    {
	ULONG RefCnt = SurfTexture->Release();
	(void)RefCnt;
/*
#ifdef _DEBUG
        if (RefCnt != 0)
        {
            __debugbreak();
        }
#endif
*/
	SurfTexture = nullptr;
    }
    //-------------------------------------------------------------------------
    if (Texture)
    {
	ULONG RefCnt = Texture->Release();
	
#ifdef _DEBUG //---------------------------------------------------------------
        if (RefCnt != 0)
        {
            __debugbreak();
        }
#endif //----------------------------------------------------------------------

	Texture = nullptr;
    }
    //-------------------------------------------------------------------------
    bool CreateDevice = false;
    //-------------------------------------------------------------------------
    if (!D3dDev)
    {
	CreateDevice = true;
	//---------------------------------------------------------------------
	if (!D3d9)
	{
	    StartD3d( wnd);
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    D3DDISPLAYMODE DispMode;
    r = D3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DispMode);
    //-------------------------------------------------------------------------
    if (r != D3D_OK)
    {
	printrdd("IDirect3D::GetAdapterDisplayMode()", r); exit();
    }
    //-------------------------------------------------------------------------
    D3DPRESENT_PARAMETERS D3dPp = { };
    //-------------------------------------------------------------------------
    // exclusive full screen
    if (Exclusive)
    {
/*  
#if 0 //-----------------------------------------------------------------------
	printf("exclusive full screen (%ux%u %uHz)\n", DispMode.Width, DispMode.Height, temp.ofq);
	RECT rr = { };
	GetWindowRect(wnd, &rr);
	printf("r1={%d,%d,%d,%d} [%dx%d]\n", rr.left, rr.top, rr.right, rr.bottom, rr.right - rr.left, rr.bottom - rr.top);
	printf("SetWindowPos(%p, HWND_TOPMOST, 0, 0, %u, %u)\n", wnd, DispMode.Width, DispMode.Height);
#endif //----------------------------------------------------------------------
*/
	//---------------------------------------------------------------------
	//хз когда
	if ( !SetWindowPos(	wnd,
				HWND_TOPMOST,
				0,
				0,
				int(DispMode.Width),	//ширина
				int(DispMode.Height),	//высота
				0
			)
	  ) // Установка WS_EX_TOPMOST
        {
            __debugbreak();
        }
	//---------------------------------------------------------------------
/*
#if 0 //-----------------------------------------------------------------------
	rr = { };
	GetWindowRect(wnd, &rr);
	printf("r2={%d,%d,%d,%d} [%dx%d]\n", rr.left, rr.top, rr.right, rr.bottom, rr.right - rr.left, rr.bottom - rr.top);
#endif //----------------------------------------------------------------------
*/
	D3dPp.Windowed = FALSE;
	D3dPp.BackBufferWidth = DispMode.Width;
	D3dPp.BackBufferHeight = DispMode.Height;
	D3dPp.BackBufferFormat = DispMode.Format;
	D3dPp.FullScreen_RefreshRateInHz = temp.ofq;
	D3dPp.PresentationInterval = conf.flip    ?	D3DPRESENT_INTERVAL_ONE :
							D3DPRESENT_INTERVAL_IMMEDIATE;
    }
    //-------------------------------------------------------------------------
    // windowed mode
    else
    {
/*
#if 0 //-----------------------------------------------------------------------
	printf("windowed mode\n");
	RECT rr = { };
	GetWindowRect(wnd, &rr);
	printf("w=%p, r={%d,%d,%d,%d} [%dx%d]\n", wnd, rr.left, rr.top, rr.right, rr.bottom, rr.right - rr.left, rr.bottom - rr.top);
#endif //----------------------------------------------------------------------
*/
	D3dPp.Windowed = TRUE;
    }
    //-------------------------------------------------------------------------
    D3dPp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    D3dPp.BackBufferCount = 1;
    D3dPp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    D3dPp.hDeviceWindow = wnd;
    //-------------------------------------------------------------------------
    if (CreateDevice)
    {
	r = D3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3dPp, &D3dDev);
	//---------------------------------------------------------------------
	if (r != D3D_OK)
	{
	    printrdd("IDirect3D::CreateDevice()", r);
	    exit();
	}
	//---------------------------------------------------------------------
	if ( !SUCCEEDED( r = D3dDev->TestCooperativeLevel() ))
	{
	    printrdd("IDirect3DDevice::TestCooperativeLevel()", r);
	    exit();
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    // reset existing device
    else 
    {
	//---------------------------------------------------------------------
	r = D3D_OK;
	do
	{
	    if (r != D3D_OK)
	    {
		Sleep(10);
	    }
	    r = D3dDev->Reset(&D3dPp);
	}
	while (r == D3DERR_DEVICELOST);
	//---------------------------------------------------------------------
	if (r != DD_OK)
	{
	    printrdd("IDirect3DDevice9::Reset()", r);
//	    __debugbreak();
	    exit();
	}
	//---------------------------------------------------------------------
//	printf("D3dDev->Reset(%d, %d)\n", D3dPp.BackBufferWidth, D3dPp.BackBufferHeight);
    }
    //-------------------------------------------------------------------------
    // recreate textures
//    printf("IDirect3DDevice9::CreateTexture(%d,%d)\n", temp.ox, temp.oy);
    r = D3dDev->CreateTexture(	temp.ox,
				temp.oy,
				1,
				D3DUSAGE_DYNAMIC,
				DispMode.Format,
				D3DPOOL_DEFAULT,
				&Texture,
				nullptr
			      );
    //-------------------------------------------------------------------------
    if (r != DD_OK)
    {
	printrdd("IDirect3DDevice9::CreateTexture()", r);
	exit();
    }
    //-------------------------------------------------------------------------
    r = Texture->GetSurfaceLevel( 0, &SurfTexture);
    if (r != DD_OK)
    {
	printrdd("IDirect3DTexture::GetSurfaceLevel()", r);
	exit();
    }
    //-------------------------------------------------------------------------
    if (!SurfTexture)
        __debugbreak();
    //-------------------------------------------------------------------------
}
//=============================================================================

static bool NeedRestoreDisplayMode = false;








//=============================================================================
//void set_vidmode()
void set_vidmode( bool preserve_size)				// [NS]
{
// bool preserve_size = FALSE by default !!!

//  printf("conf.win_static_size_x %d\n",conf.win_static_size_x);
//  printf("conf.win_static_size_y %d\n",conf.win_static_size_y);

    //-------------------------------------------------------------------------
    RECT current_main_wnd_size;
    GetClientRect( wnd, &current_main_wnd_size);
    int current_main_wnd_size_x = current_main_wnd_size.right -  current_main_wnd_size.left;
    int current_main_wnd_size_y = current_main_wnd_size.bottom - current_main_wnd_size.top;
    //-------------------------------------------------------------------------
    if (preserve_size)
    {
	//printf( "try to preserve size\n");
    }
    //-------------------------------------------------------------------------

//   printf("%s\n", __FUNCTION__);
    //-------------------------------------------------------------------------
    if (pal)
    {
	pal->Release();
	pal = nullptr;
    }
    //-------------------------------------------------------------------------
    if (surf2)
    {
	surf2->Release();
	surf2 = nullptr;
    }
    //-------------------------------------------------------------------------
    if (surf1)
    {
	surf1->Release();
	surf1 = nullptr;
    }
    //-------------------------------------------------------------------------
    if (surf0)
    {
	ULONG RefCnt = surf0->Release();
	//---------------------------------------------------------------------
	if (RefCnt != 0)
	{
	    printf("surf0->Release(), RefCnt=%lu\n", RefCnt);
	    exit();
	}
	//---------------------------------------------------------------------
	surf0 = nullptr;
    }
    //-------------------------------------------------------------------------
    if (sprim)	//OVERLAY??
    {
	sprim->Release();
	sprim = nullptr;
    }
    //-------------------------------------------------------------------------
    if (clip)
    {
	clip->Release();
	clip = nullptr;
    }
    //-------------------------------------------------------------------------
    if (SurfTexture)
    {
	SurfTexture->Release();
	SurfTexture = nullptr;
    }
    //-------------------------------------------------------------------------
    if (Texture)
    {
	ULONG RefCnt = Texture->Release();
	(void) RefCnt;
	
#ifdef _DEBUG //---------------------------------------------------------------
	if (RefCnt != 0)
	{
	    __debugbreak();
	}
#endif //----------------------------------------------------------------------

	Texture = nullptr;
    }
    //-------------------------------------------------------------------------
    HRESULT r;

    DDSURFACEDESC desc;
    desc.dwSize = sizeof desc;
    r = dd->GetDisplayMode( &desc);
    //-------------------------------------------------------------------------
    if (r != DD_OK)
    {
	printrdd("IDirectDraw2::GetDisplayMode()", r);
	exit();
    }
    //-------------------------------------------------------------------------
    temp.ofq = desc.dwRefreshRate; // nt only?
    if (!temp.ofq)
	temp.ofq = conf.refresh;
    //-------------------------------------------------------------------------
    // Проверка наличия hw overlay
    if (drivers[ conf.driver].flags & RF_OVR)
    {
	DDCAPS Caps;
	Caps.dwSize = sizeof( Caps);
	//---------------------------------------------------------------------
	if ((r = dd->GetCaps( &Caps, nullptr)) == DD_OK)
	{
	    if (Caps.dwMaxVisibleOverlays == 0)
	    {
		errexit("HW Overlay not supported");
	    }
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    // select fullscreen, set window style
    if (temp.rflags & RF_DRIVER)
	temp.rflags |= drivers[ conf.driver].flags;
    //-------------------------------------------------------------------------
    // fullscreen для режимов не поддерживающих window mode
    if (!(temp.rflags &	( RF_GDI  |
			  RF_OVR  |
			  RF_CLIP |
			  RF_D3D
			))
     )
    {
	 conf.fullscr = 1;
    }
    //-------------------------------------------------------------------------
    if ((temp.rflags & RF_32) && (desc.ddpfPixelFormat.dwRGBBitCount != 32))
	conf.fullscr = 1; // for chunks via blitter
    //-------------------------------------------------------------------------
    static RECT rc;
    LONG oldstyle = GetWindowLong( wnd, GWL_STYLE);
    //-------------------------------------------------------------------------
    if (oldstyle & WS_CAPTION)
	GetWindowRect( wnd, &rc);
    //-------------------------------------------------------------------------
    LONG style = LONG( conf.fullscr  ?	(WS_VISIBLE | WS_POPUP) :
					(WS_VISIBLE | WS_OVERLAPPEDWINDOW));
    //-------------------------------------------------------------------------
    if ((oldstyle ^ style) & WS_CAPTION)
    {
//	printf("set style=%X, fullscr=%d\n", style, conf.fullscr);
	SetWindowLong( wnd, GWL_STYLE, style);
    }
    //-------------------------------------------------------------------------
    // select exclusive
    u8 excl = conf.fullscr;
    //-------------------------------------------------------------------------
    if ((temp.rflags & RF_CLIP) && (desc.ddpfPixelFormat.dwRGBBitCount == 8))
	excl = 1;
    //-------------------------------------------------------------------------
    if (!(temp.rflags & (RF_MON | RF_D3D | RF_D3DE)))
    {
	r = dd->SetCooperativeLevel( wnd, excl  ?  DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN :
						   DDSCL_ALLOWREBOOT | DDSCL_NORMAL);
	if (r != DD_OK)
	{
	    printrdd("IDirectDraw2::SetCooperativeLevel()", r);
	    exit();
	}
    }
    //-------------------------------------------------------------------------
//	temp.b_left = (((conf.paper % conf.t_line) & 0x07 ) == 0)   ?	(conf.paper % conf.t_line)		:
//									(((conf.paper % conf.t_line) | 0x07)+1)	;


// border full - тк размеры по X нужны кратные 8
// то нельзя отобразить правильный размер
// добавляя слева отъедаетсо часть изображения справа
// поэтому изображение задаетсо с запасом со всех сторон
// и видно мусор
// может вбудухщем невалидная часть будет затиратсо поверх
// так же 16с режимы рисуют мимо !!!!!!111
		
    //-------------------------------------------------------------------------		
    // select resolution
    const unsigned size_x[4] = { 256U,
				 conf.mcx_small,
				 conf.mcx_full,
				 512	//(((conf.t_line * 2) & 0x07 ) == 0)  ?	((conf.t_line * 2) + 8)		 :
					//					(((conf.t_line * 2) | 0x07) + 9)
					//				// full X mode
			       };
    const unsigned size_y[4] = { 192U,
				 conf.mcy_small,
				 conf.mcy_full,
				 324			// full y mode
			      };
    //-------------------------------------------------------------------------
    // достаем из готового набора размеров
    temp.ox = temp.scx = size_x[ conf.bordersize];
    temp.oy = temp.scy = size_y[ conf.bordersize];
    //-------------------------------------------------------------------------
//	printf("temp.ox %d\n",temp.ox);
//	printf("temp.oy %d\n",temp.oy);

    //-------------------------------------------------------------------------
    if (temp.rflags & RF_2X)
    {
	temp.ox *= 2;
	temp.oy *= 2;
	//---------------------------------------------------------------------
	if (	conf.fast_sl				&&
		(temp.rflags & RF_DRIVER)		&&
		(temp.rflags & (RF_CLIP | RF_OVR))
	 )
	{
	    temp.oy /= 2;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    //RF_* - video filter!!!
    //а еще есть winscale который работает поверх и который не влияет на размер дебагера!!!!
    //который зоветсо тут  temp.scale
    if (temp.rflags & RF_3X)		{ temp.ox *= 3;		temp.oy *= 3; }
    if (temp.rflags & RF_4X)		{ temp.ox *= 4;		temp.oy *= 4; }
    if (temp.rflags & RF_64x48)		{ temp.ox = 64;		temp.oy = 48; }
    if (temp.rflags & RF_128x96)	{ temp.ox = 128;	temp.oy = 96; }

//	printf("temp.ox %d\n",temp.ox);
//	printf("temp.oy %d\n",temp.oy);
//	printf("temp.scale %d\n",temp.scale);
//	printf("temp.mon_scale %d\n",temp.mon_scale);
	
   //дебагер
//   if (temp.rflags & RF_MON) { temp.ox = 640; temp.oy = 480; }

//NS
//    if (temp.rflags & RF_MON)
//    {
//	if	(temp.rflags & RF_2X)	{ temp.ox = 640; temp.oy = 480; }	//RF_2X
//	else if	(temp.rflags & RF_3X)	{ temp.ox = 1280; temp.oy = 640; }	//RF_3X
//	else				{ temp.ox = 320; temp.oy = 240; }	//RF_1X
//    }
    
    //-------------------------------------------------------------------------
    if (temp.rflags & RF_MON)
    {					//дебагер ставит temp.scale = 1 !!!!
	temp.ox *= temp.mon_scale;	//а тот ресайз что делает temp.scale
	temp.oy *= temp.mon_scale;	//не работает в gdi режиме дебагера
    }					//да и эмуля тожы...
    //-------------------------------------------------------------------------			

/*
	// так кррашитсо
	if ((preserve_size) && (!conf.fullscr))
	{
	temp.ox = current_main_wnd_size_x;
	temp.oy = current_main_wnd_size_y;
	printf("current_main_wnd_size_x %d\n",current_main_wnd_size_x);
	printf("current_main_wnd_size_y %d\n",current_main_wnd_size_y);
	}
*/
	/*
	if ((preserve_size) && (!conf.fullscr))
	{
	temp.scx = current_main_wnd_size_x;
	temp.scy = current_main_wnd_size_y;
	printf("current_main_wnd_size_x %d\n",current_main_wnd_size_x);
	printf("current_main_wnd_size_y %d\n",current_main_wnd_size_y);
	}
	*/



//тут можно сделать еще и произвольный размер дебагера

//   printf("temp.ox=%d, temp.oy=%d\n", temp.ox, temp.oy);

   // select color depth
    temp.obpp = 8;
    //-------------------------------------------------------------------------
    if (temp.rflags & (RF_CLIP | RF_D3D | RF_D3DE))
	temp.obpp = desc.ddpfPixelFormat.dwRGBBitCount;
    //-------------------------------------------------------------------------
    if (temp.rflags & (RF_16 | RF_OVR))
	temp.obpp = 16;
    //-------------------------------------------------------------------------
    if (temp.rflags & RF_32)
	temp.obpp = 32;
    //-------------------------------------------------------------------------
    if ((temp.rflags & (RF_GDI|RF_8BPCH)) == (RF_GDI|RF_8BPCH))
	temp.obpp = 32;
    //-------------------------------------------------------------------------
    // Full screen
    if (conf.fullscr || ((temp.rflags & RF_MON) && desc.dwHeight < 480))
    {
	// select minimal screen mode
	unsigned newx = 100000;
	unsigned newy = 100000;
	unsigned newfq = conf.refresh  ?  conf.refresh :
					  temp.ofq;
	unsigned newb = temp.obpp;
	unsigned minx = temp.ox;
	unsigned miny = temp.oy;
	unsigned needb = temp.obpp;
	//---------------------------------------------------------------------
	if (temp.rflags & (RF_64x48 | RF_128x96))
	{
	    needb = (temp.rflags & RF_16)  ?  16:
					      32;
					      
	    minx = desc.dwWidth;  if (minx < 640) minx = 640;
	    miny = desc.dwHeight; if (miny < 480) miny = 480;
	}
	//---------------------------------------------------------------------
	// if (temp.rflags & RF_MON) // - ox=640, oy=480 - set above
	//---------------------------------------------------------------------
	for (unsigned i = 0;    i < max_modes;    i++)
	{
	    //-----------------------------------------------------------------
	    if (modes[i].y < miny || modes[i].x < minx)
		continue;
	    //-----------------------------------------------------------------
	    if (!(temp.rflags & RF_MON) && modes[i].b != temp.obpp)
		continue;
	    //-----------------------------------------------------------------
	    if (modes[i].y < conf.minres)
		continue;
	    //-----------------------------------------------------------------
	    if (	(modes[i].x < newx || modes[i].y < newy)	&& 
			(!conf.refresh || (modes[i].f == newfq))
	      )
	    {
		newx = modes[i].x;
		newy = modes[i].y;
		if (!conf.refresh && modes[i].f > newfq)
		    newfq = modes[i].f;
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	if (newx == 100000)
	{
	    color( CONSCLR_ERROR);
	    printf("can't find situable mode for %u x %u * %u bits\n", temp.ox, temp.oy, temp.obpp);
	    exit();
	}
	//---------------------------------------------------------------------
	// use minimal or current mode
	if (temp.rflags & (RF_OVR | RF_GDI | RF_CLIP | RF_D3D | RF_D3DE))
	{
	    //-----------------------------------------------------------------
	    // leave screen size, if enough width/height
	    newx = desc.dwWidth;
	    newy = desc.dwHeight;
	    //-----------------------------------------------------------------
	    if (newx < minx || newy < miny)
	    {
		newx = minx;
		newy = miny;
	    }
	    //-----------------------------------------------------------------
	    // leave color depth, until specified directly
	    if (!(temp.rflags & (RF_16 | RF_32)))
		newb = desc.ddpfPixelFormat.dwRGBBitCount;
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	if (	(desc.dwWidth != newx)				||
		(desc.dwHeight != newy) 			||
		(temp.ofq != newfq)				||
		(desc.ddpfPixelFormat.dwRGBBitCount != newb)
	  )
	{
//	    printf("SetDisplayMode:%ux%u %uHz\n", newx, newy, newfq);
	    //-----------------------------------------------------------------
	    if ((r = dd->SetDisplayMode( newx, newy, newb, newfq, 0)) != DD_OK)
	    {
		printrdd("IDirectDraw2::SetDisplayMode()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	    GetSystemPaletteEntries( temp.gdidc, 0, 0x100, syspalette);
	    //-----------------------------------------------------------------
	    if (newfq)
		temp.ofq = newfq;
	    //-----------------------------------------------------------------
	    NeedRestoreDisplayMode = true;
	}
	//---------------------------------------------------------------------
	temp.odx = temp.obpp * (newx - temp.ox) / 16;
	temp.ody = (newy - temp.oy) / 2;
	temp.rsx = newx;
	temp.rsy = newy;
//      printf("vmode=%ux%u %uHz\n", newx, newy, newfq);
//      ShowWindow(wnd, SW_SHOWMAXIMIZED);
	printf("SetWindowPos(%p, HWND_TOPMOST, 0, 0, %u, %u)\n", wnd, newx, newy);
	//---------------------------------------------------------------------
	//хз когда
	if (!SetWindowPos(	wnd,			// Установка WS_EX_TOPMOST
				HWND_TOPMOST, 
				0, 
				0, 
				int( newx), 
				int( newy), 
				0
	  )) 
	{
	    __debugbreak();
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    // Windowed
    else
    {
	//---------------------------------------------------------------------
	// Восстановление предыдущего видеорежима при возврате из fullscreen (если была переустановка видеорежима)
	if (NeedRestoreDisplayMode)
	{
	    if ((r = dd->RestoreDisplayMode()) != DD_OK)
	    {
		printrdd("IDirectDraw2::SetDisplayMode()", r);
		exit();
	    }
	    NeedRestoreDisplayMode = false;
	}
	//---------------------------------------------------------------------
	// restore window position to last saved position in non-fullscreen mode
	ShowWindow( wnd, SW_SHOWNORMAL);
	//---------------------------------------------------------------------
	if (temp.rflags & RF_GDI)
	{
	    RECT client;
	    if (preserve_size)
	    {
		RECT client =
		{
		    0,
		    0,
		    LONG( current_main_wnd_size_x),
		    LONG( current_main_wnd_size_y) 
		};    
	    }
	    else
	    {
	    RECT client =
	    {
		0,
		0,
		LONG( temp.ox),
		LONG( temp.oy) 
	    };    
	    }
	    AdjustWindowRect( &client, WS_VISIBLE | WS_OVERLAPPEDWINDOW, 0);
	    rc.right = rc.left + (client.right - client.left);
	    rc.bottom = rc.top + (client.bottom - client.top);
	}
	//---------------------------------------------------------------------


   //-------------------------------------------------------------------------
	/*
	// нужен при возврате из фуллскрина
	// чтоб окно оказывалось там где надо
	// НО приводит к мерзскому дергосмыканию при входе в дебагер
	MoveWindow(	wnd,
			rc.left,
			rc.top,
			rc.right - rc.left,
			rc.bottom - rc.top,
			1
		   );
	*/
    }
    //-------------------------------------------------------------------------
    if (!(temp.rflags & (RF_D3D | RF_D3DE)))
    {
	dd->FlipToGDISurface(); // don't check result
    }
    //-------------------------------------------------------------------------
    desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    desc.dwFlags = DDSD_CAPS;

    DWORD pl[ 0x101];
    pl[0] = 0x01000300;
    memcpy( pl+1, pal0, 0x400);
    HPALETTE hpal = CreatePalette( (LOGPALETTE*) &pl);
    DeleteObject( SelectPalette( temp.gdidc, hpal, 0));
    RealizePalette( temp.gdidc); // for RF_GDI and for bitmap, used in WM_PAINT
    //-------------------------------------------------------------------------
    if (temp.rflags & RF_GDI)
    {
	//---------------------------------------------------------------------
	// так расколбашивает gdi режим
	// пока предыдущее разрешение не есть оригинальное
//	if (preserve_size)
//	{
//	    gdibmp.header.bmiHeader.biWidth = LONG( current_main_wnd_size_x);
//	    gdibmp.header.bmiHeader.biHeight = -LONG( current_main_wnd_size_y);
//	}
//	//---------------------------------------------------------------------
//	else
//	{
	    gdibmp.header.bmiHeader.biWidth = LONG( temp.ox);
	    gdibmp.header.bmiHeader.biHeight = -LONG( temp.oy);
//	}
	//---------------------------------------------------------------------
	gdibmp.header.bmiHeader.biBitCount = WORD( temp.obpp);
    }
    //-------------------------------------------------------------------------
    else if (temp.rflags & RF_OVR)
    {
 
	temp.odx = temp.ody = 0;
	if ((r = dd->CreateSurface( &desc, &sprim, nullptr)) != DD_OK)
	{
	    printrdd("IDirectDraw2::CreateSurface() [primary,test]", r);
	    exit();
	}
	//---------------------------------------------------------------------
	desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	desc.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_VIDEOMEMORY;
	desc.dwWidth = temp.ox;
	desc.dwHeight = temp.oy;
	//---------------------------------------------------------------------
	// conf.flip = 0;	// overlay always synchronized without Flip()! on radeon videocards
				// double flip causes fps drop
	//---------------------------------------------------------------------
	if (conf.flip)
	{
	    desc.dwBackBufferCount = 1;
	    desc.dwFlags |= DDSD_BACKBUFFERCOUNT;
	    desc.ddsCaps.dwCaps |= DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	}
	//---------------------------------------------------------------------
	static DDPIXELFORMAT ddpfOverlayFormat16 =
	{
	    sizeof( DDPIXELFORMAT),
	    DDPF_RGB,
	    0,
	    {16},
	    {0xF800},
	    {0x07E0},
	    {0x001F},
	    {0} 
	};
	//---------------------------------------------------------------------
	static DDPIXELFORMAT ddpfOverlayFormat15 =
	{
	    sizeof( DDPIXELFORMAT),
	    DDPF_RGB,
	    0,
	    {16},
	    {0x7C00},
	    {0x03E0},
	    {0x001F},
	    {0} 
	};
	//---------------------------------------------------------------------
	static DDPIXELFORMAT ddpfOverlayFormatYUY2 = 
	{
	    sizeof( DDPIXELFORMAT),
	    DDPF_FOURCC,
	    MAKEFOURCC( 'Y','U','Y','2'),
	    {0},
	    {0},
	    {0},
	    {0},
	    {0} 
	};
	//---------------------------------------------------------------------
	if (temp.rflags & RF_YUY2)
	    goto YUY2;
	//---------------------------------------------------------------------
	temp.hi15 = 0;
	desc.ddpfPixelFormat = ddpfOverlayFormat16;
	r = dd->CreateSurface( &desc, &surf0, nullptr);
	//---------------------------------------------------------------------
	if (r == DDERR_INVALIDPIXELFORMAT)
	{
	    temp.hi15 = 1;
	    desc.ddpfPixelFormat = ddpfOverlayFormat15;
	    r = dd->CreateSurface( &desc, &surf0, nullptr);
	}
	//---------------------------------------------------------------------
	if (r == DDERR_INVALIDPIXELFORMAT /*&& !(temp.rflags & RF_RGB)*/)
	{
YUY2:
	    temp.hi15 = 2;
	    desc.ddpfPixelFormat = ddpfOverlayFormatYUY2;
	    r = dd->CreateSurface( &desc, &surf0, nullptr);
	}
	//---------------------------------------------------------------------
	if (r != DD_OK)
	{
	    printrdd("IDirectDraw2::CreateSurface() [overlay]", r);
	    exit();
	    //r = DDERR_OUTOFCAPS - crash
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    else if (temp.rflags & (RF_D3D | RF_D3DE)) // d3d windowed, d3d full screen exclusive
    {
//	printf("%s(RF_D3D)\n", __FUNCTION__);
	// Сначала нужно отмасштабировать окно до нужного размера, а только потом устанавливать видеорежим
	// т.к. видеорежим определяет размеры back buffer'а из размеров окна.
    }
    //-------------------------------------------------------------------------
    else  // blt, direct video mem
    {
//	desc.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;
	//---------------------------------------------------------------------
	if (conf.flip && !(temp.rflags & RF_CLIP))
	{
	    desc.dwBackBufferCount = 1;
	    desc.dwFlags |= DDSD_BACKBUFFERCOUNT;
	    desc.ddsCaps.dwCaps |= DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	}
	//---------------------------------------------------------------------
	if ((r = dd->CreateSurface( &desc, &surf0, nullptr)) != DD_OK)
	{
	    printrdd( "IDirectDraw2::CreateSurface() [primary1]", r);
	    exit();
	}
	//---------------------------------------------------------------------
	if (temp.rflags & RF_CLIP)
	{
	    DDSURFACEDESC SurfDesc;
	    SurfDesc.dwSize = sizeof( SurfDesc);
	    //-----------------------------------------------------------------
	    if ((r = surf0->GetSurfaceDesc( &SurfDesc)) != DD_OK)
	    {
		printrdd( "IDirectDrawSurface::GetSurfaceDesc()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	    desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	    desc.dwWidth = SurfDesc.dwWidth; 
	    desc.dwHeight = SurfDesc.dwHeight;
	    desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;
	    //-----------------------------------------------------------------
	    if ((r = dd->CreateSurface( &desc, &surf2, nullptr)) != DD_OK)
	    {
		printrdd( "IDirectDraw2::CreateSurface() [2]", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	    r = dd->CreateClipper( 0, &clip, nullptr);
	    
	    if (r != DD_OK)
	    {
		printrdd( "IDirectDraw2::CreateClipper()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	    r = clip->SetHWnd( 0, wnd);
	    
	    if (r != DD_OK)
	    {
		printrdd( "IDirectDraw2::SetHWnd()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	    r = surf0->SetClipper( clip);
	    
	    if (r != DD_OK)
	    {
	        printrdd("surf0, IDirectDrawSurface2::SetClipper()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	    r = surf2->SetClipper( clip);
	  
	    if (r != DD_OK)
	    {
	        printrdd("surf2, IDirectDrawSurface2::SetClipper()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	    r = dd->GetDisplayMode( &desc);
	    
	    if (r != DD_OK)
	    {
	        printrdd("IDirectDraw2::GetDisplayMode()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	    if ((temp.rflags & RF_32) && (desc.ddpfPixelFormat.dwRGBBitCount != 32))
		errexit( "video driver requires 32bit color depth on desktop for this mode");
	    //-----------------------------------------------------------------
	    desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	    desc.dwWidth = temp.ox; 
	    desc.dwHeight = temp.oy;

	    // Видеокарты AMD Radeon HD не поддерживают surface в системной памяти
	    // из за этого приходится отдельный буфер в системно памяти и делать программное
	    // копирование в surface выделенный в видеопамяти иначе никак не задать выравнивание на 16 байт
	    desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;

#ifdef MOD_SSE2
	    //-----------------------------------------------------------------
	    //if (!(renders[conf.render].flags & 0))	// все время работало как
	    if (!(renders[ conf.render].flags & RF_1X))	// если не RF_1X
	    {
		//printf("SSE2\n");
		SurfPitch1 = (temp.ox * temp.obpp) >> 3;
		SurfPitch1 = (SurfPitch1 + 15) & ~15U; // Выравнивание на 16
		//-------------------------------------------------------------
		if (SurfMem1)
		    _aligned_free( SurfMem1);
		//-------------------------------------------------------------
		SurfMem1 = _aligned_malloc( SurfPitch1 * temp.oy, 16);
		FlipBltMethod = FlipBltAlign16;
	    }
	    //-----------------------------------------------------------------
	    else
#endif
	    {
		//printf("not SSE2\n");
		FlipBltMethod = FlipBltAlign4;
	    }
	    //-----------------------------------------------------------------
	    r = dd->CreateSurface( &desc, &surf1, nullptr);
	    if (r != DD_OK)
	    {
		printrdd("IDirectDraw2::CreateSurface()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	if (temp.obpp == 16)
	{
	    DDPIXELFORMAT fm;
	    fm.dwSize = sizeof fm;
	    //-----------------------------------------------------------------
	    if ((r = surf0->GetPixelFormat( &fm)) != DD_OK)
	    {
		printrdd( "IDirectDrawSurface2::GetPixelFormat()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	    if (	(fm.dwRBitMask == 0xF800)	&&
			(fm.dwGBitMask == 0x07E0)	&&
			(fm.dwBBitMask == 0x001F)
	      )
	    {
		temp.hi15 = 0;
	    }
	    //-----------------------------------------------------------------
	    else if (	(fm.dwRBitMask == 0x7C00)	&&
			(fm.dwGBitMask == 0x03E0)	&&
			(fm.dwBBitMask == 0x001F)
	      )
            {
		temp.hi15 = 1;
	    }
	    //-----------------------------------------------------------------
	    else
            {
		errexit( "invalid pixel format (need RGB:5-6-5 or URGB:1-5-5-5)");
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	else if (temp.obpp == 8)
	{
	    //-----------------------------------------------------------------
	    if ((r = dd->CreatePalette( DDPCAPS_8BIT | DDPCAPS_ALLOW256, syspalette, &pal, nullptr)) != DD_OK)
	    {
		printrdd( "IDirectDraw2::CreatePalette()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	    if ((r = surf0->SetPalette( pal)) != DD_OK)
	    {
		printrdd( "IDirectDrawSurface2::SetPalette()", r);
		exit();
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    if (conf.flip && !(temp.rflags & (RF_GDI|RF_CLIP|RF_D3D|RF_D3DE)))
    {
	DDSCAPS caps = { DDSCAPS_BACKBUFFER };
	//---------------------------------------------------------------------
	if ((r = surf0->GetAttachedSurface( &caps, &surf1)) != DD_OK)
	{
	    printrdd( "IDirectDraw2::GetAttachedSurface()", r);
	    exit();
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    // Настраиваем функцию конвертирования из текущего формата в BGR24
    switch (temp.obpp)
    {
	//---------------------------------------------------------------------
	case 8:
	    ConvBgr24 = ConvPal8ToBgr24;
	    break;
	//---------------------------------------------------------------------
	case 16:
	    //-----------------------------------------------------------------
	    switch (temp.hi15)
	    {
		//-------------------------------------------------------------
		case 0: ConvBgr24 = ConvRgb16ToBgr24;	// RGB16
		    break; 
		//-------------------------------------------------------------
		case 1: ConvBgr24 = ConvRgb15ToBgr24;	// RGB15
		    break; 
		//-------------------------------------------------------------
		case 2: ConvBgr24 = ConvYuy2ToBgr24;	// YUY2
		    break; 
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    break;
	//---------------------------------------------------------------------
	case 32:
	    ConvBgr24 = ConvBgr32ToBgr24;
	    break;
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    SendMessage( wnd, WM_USER, 0, 0); // setup rectangle for RF_GDI,OVR,CLIP, adjust cursor
    //-------------------------------------------------------------------------
    // корень зла
    //if (conf.win_resize_request)	// [NS]
    if (!preserve_size)
    {
	scale_normal();
	//if (!conf.fullscr)
	//    scale_normal();
    }	    
    //-------------------------------------------------------------------------
    // d3d windowed, d3d full screen exclusive
    if (temp.rflags & (RF_D3D | RF_D3DE))
    {
	// Сначала нужно отмасштабировать окно до нужного размера, а только потом устанавливать видеорежим
	// т.к. видеорежим определяет размеры back buffer'а из размеров окна.
	SetVideoModeD3d( (temp.rflags & RF_D3DE) != 0);
    }
    //-------------------------------------------------------------------------
    
    
    
/*
    //-------------------------------------------------------------------------
    if (conf.win_resize_request)
    {
	// сохраняем текущее разрешение
	printf("save size\n");
	conf.win_static_size_x = temp.ox;
	conf.win_static_size_y = temp.oy;
	conf.win_resize_request = 0;
    }
    //-------------------------------------------------------------------------
    else
    {
	// используем текущее разрешение
	printf("use static size\n");
	temp.ox = conf.win_static_size_x;
	temp.oy = conf.win_static_size_y;
    }
    //-------------------------------------------------------------------------    
*/
    
    
    
    
}
//=============================================================================


//=============================================================================
static HRESULT SetDIDwordProperty(	LPDIRECTINPUTDEVICE	pdev,
					REFGUID			guidProperty,
					DWORD		dwObject,
					DWORD		dwHow,
					DWORD		dwValue
  )
{
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof( dipdw);
    dipdw.diph.dwHeaderSize = sizeof( dipdw.diph);
    dipdw.diph.dwObj        = dwObject;
    dipdw.diph.dwHow        = dwHow;
    dipdw.dwData            = dwValue;
    return pdev->SetProperty( guidProperty, &dipdw.diph);
}
//=============================================================================


//=============================================================================
static BOOL CALLBACK InitJoystickInput(LPCDIDEVICEINSTANCE pdinst, LPVOID pvRef)
{
    HRESULT r;
    LPDIRECTINPUT pdi = (LPDIRECTINPUT)pvRef;
    LPDIRECTINPUTDEVICE dijoyst1;
    LPDIRECTINPUTDEVICE2 dijoyst;
    //-------------------------------------------------------------------------
    if ((r = pdi->CreateDevice(pdinst->guidInstance, &dijoyst1, nullptr)) != DI_OK)
    {
	printrdi("IDirectInput::CreateDevice() (joystick)", r);
	return DIENUM_CONTINUE;
    }
    //-------------------------------------------------------------------------
    r = dijoyst1->QueryInterface(IID_IDirectInputDevice2, (void**)&dijoyst);
    if (r != S_OK)
    {
	printrdi("IDirectInputDevice::QueryInterface(IID_IDirectInputDevice2) [dx5 not found]", r);
	dijoyst1->Release();
	dijoyst1=nullptr;
	return DIENUM_CONTINUE;
    }
    //-------------------------------------------------------------------------
    dijoyst1->Release();

    DIDEVICEINSTANCE dide = { sizeof dide };
    //-------------------------------------------------------------------------
    if ((r = dijoyst->GetDeviceInfo(&dide)) != DI_OK)
    {
	printrdi("IDirectInputDevice::GetDeviceInfo()", r);
	return DIENUM_STOP;
    }
    //-------------------------------------------------------------------------
    DIDEVCAPS dc = { sizeof dc };
    //-------------------------------------------------------------------------
    if ((r = dijoyst->GetCapabilities(&dc)) != DI_OK)
    {
	printrdi("IDirectInputDevice::GetCapabilities()", r);
	return DIENUM_STOP;
    }
    //-------------------------------------------------------------------------
    DIPROPDWORD JoyId;
    JoyId.diph.dwSize       = sizeof(JoyId);
    JoyId.diph.dwHeaderSize = sizeof(JoyId.diph);
    JoyId.diph.dwObj        = 0;
    JoyId.diph.dwHow        = DIPH_DEVICE;
   //-------------------------------------------------------------------------
    if ((r = dijoyst->GetProperty(DIPROP_JOYSTICKID, &JoyId.diph)) != DI_OK)
    {
	printrdi("IDirectInputDevice::GetProperty(DIPROP_JOYSTICKID)", r);
	exit();
    }
    //-------------------------------------------------------------------------
    trim_right( dide.tszInstanceName);
    trim_right( dide.tszProductName);

    CharToOem( dide.tszInstanceName, dide.tszInstanceName);
    CharToOem( dide.tszProductName, dide.tszProductName);
    //-------------------------------------------------------------------------
    if (strcmp( dide.tszProductName, dide.tszInstanceName))
    {
	strcat( dide.tszInstanceName, ", ");
    }
    else
    {
	dide.tszInstanceName[0] = 0;
    }
    //-------------------------------------------------------------------------
    bool UseJoy = (JoyId.dwData == conf.input.JoyId);
    color( CONSCLR_HARDINFO);
    printf("%cjoy(%lu): %s%s (%lu axes, %lu buttons, %lu POVs)\n",	UseJoy    ?	'*' :
											' ',
									JoyId.dwData,
									dide.tszInstanceName,
									dide.tszProductName,
									dc.dwAxes,
									dc.dwButtons,
									dc.dwPOVs
								  );

    //-------------------------------------------------------------------------
    if (UseJoy)
    {
	//---------------------------------------------------------------------
	if ((r = dijoyst->SetDataFormat(&c_dfDIJoystick)) != DI_OK)
	{
	    printrdi("IDirectInputDevice::SetDataFormat() (joystick)", r);
	    exit();
	}
	//---------------------------------------------------------------------
	if ((r = dijoyst->SetCooperativeLevel(wnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)) != DI_OK)
	{
	    printrdi("IDirectInputDevice::SetCooperativeLevel() (joystick)", r);
	    exit();
	}
	//---------------------------------------------------------------------
	DIPROPRANGE diprg;
	diprg.diph.dwSize       = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj        = DIJOFS_X;
	diprg.diph.dwHow        = DIPH_BYOFFSET;
	diprg.lMin              = -1000;
	diprg.lMax              = +1000;
	//---------------------------------------------------------------------
	if ((r = dijoyst->SetProperty(DIPROP_RANGE, &diprg.diph)) != DI_OK)
	{
	    printrdi("IDirectInputDevice::SetProperty(DIPH_RANGE)", r);
	    exit(); 
	}
	//---------------------------------------------------------------------
        diprg.diph.dwObj        = DIJOFS_Y;
	//---------------------------------------------------------------------
	if ((r = dijoyst->SetProperty(DIPROP_RANGE, &diprg.diph)) != DI_OK)
	{
	    printrdi("IDirectInputDevice::SetProperty(DIPH_RANGE) (y)", r);
	    exit();
	}
	//---------------------------------------------------------------------
	if ((r = SetDIDwordProperty(dijoyst, DIPROP_DEADZONE, DIJOFS_X, DIPH_BYOFFSET, 2000)) != DI_OK)
	{
	    printrdi("IDirectInputDevice::SetProperty(DIPH_DEADZONE)", r);
	    exit(); 
	}
	//---------------------------------------------------------------------
	if ((r = SetDIDwordProperty(dijoyst, DIPROP_DEADZONE, DIJOFS_Y, DIPH_BYOFFSET, 2000)) != DI_OK)
	{
	    printrdi("IDirectInputDevice::SetProperty(DIPH_DEADZONE) (y)", r);
	    exit(); 
	}
	//---------------------------------------------------------------------
	::dijoyst = dijoyst;
    }
    //-------------------------------------------------------------------------
    else
    {
	dijoyst->Release();
    }
    //-------------------------------------------------------------------------
    return DIENUM_CONTINUE;
}
//=============================================================================


//=============================================================================
static HRESULT WINAPI callb(LPDDSURFACEDESC surf, void *lpContext)
{
    (void)lpContext;

    //-------------------------------------------------------------------------
    if (max_modes >= MAX_MODES)
	return DDENUMRET_CANCEL;
    //-------------------------------------------------------------------------
    modes[max_modes].x = surf->dwWidth;
    modes[max_modes].y = surf->dwHeight;
    modes[max_modes].b = surf->ddpfPixelFormat.dwRGBBitCount;
    modes[max_modes].f = surf->dwRefreshRate;
    max_modes++;
    return DDENUMRET_OK;
}
//=============================================================================


//=============================================================================
void scale_normal()
{
    ULONG cmd;
    //printf("send\n");
    //-------------------------------------------------------------------------
    switch (temp.scale)
    {
	default:
	case 1: cmd = SCU_SCALE1; break;
	case 2: cmd = SCU_SCALE2; break;
	case 3: cmd = SCU_SCALE3; break;
	case 4: cmd = SCU_SCALE4; break;
    }
    //-------------------------------------------------------------------------
    SendMessage(wnd, WM_SYSCOMMAND, cmd, 0); // set window size
    
    //ну вот а потом один видео режим уже включен
    //а костыли для видео режима в епаной очереди сообщений...
    
}
//=============================================================================

#ifdef _DEBUG
#define D3D_DLL_NAME "d3d9d.dll"
#else
#endif
#define D3D_DLL_NAME "d3d9.dll"

//=============================================================================
static void DbgPrint(const char *s)
{
    OutputDebugStringA(s);
}
//=============================================================================


//=============================================================================
static void StartD3d(HWND Wnd)
{
    (void)Wnd;

/*
#if 0 //-----------------------------------------------------------------------
    OutputDebugString(__FUNCTION__"\n");
    printf("%s\n", __FUNCTION__);
#endif //----------------------------------------------------------------------
*/
    //-------------------------------------------------------------------------
    if (!D3d9Dll)
    {
        D3d9Dll = LoadLibrary(D3D_DLL_NAME);
	//---------------------------------------------------------------------
	if (!D3d9Dll)
	{
	    errexit("unable load d3d9.dll");
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    if (!D3d9)
    {
	typedef IDirect3D9 * (WINAPI *TDirect3DCreate9)(UINT SDKVersion);
	TDirect3DCreate9 Direct3DCreate9 = (TDirect3DCreate9) GetProcAddress( D3d9Dll, "Direct3DCreate9");
	D3d9 = Direct3DCreate9( D3D_SDK_VERSION);
    }
    //-------------------------------------------------------------------------
}
//=============================================================================


//=============================================================================
static void CalcWindowSize()
{
	//printf("CalcWindowSize\n");
    temp.rflags = renders[conf.render].flags;
    //-------------------------------------------------------------------------
    if (renders[conf.render].func == render_advmame)
    {
	//-------------------------------------------------------------------------
	if (conf.videoscale == 2)
	    temp.rflags |= RF_2X;
	//-------------------------------------------------------------------------
	if (conf.videoscale == 3)
	    temp.rflags |= RF_3X;
	//-------------------------------------------------------------------------
	if (conf.videoscale == 4)
	    temp.rflags |= RF_4X;
	//-------------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    if (temp.rflags & RF_DRIVER)
	temp.rflags |= drivers[conf.driver].flags;
    //-------------------------------------------------------------------------
    // select resolution
    const unsigned size_x[4] = {	256U,
					conf.mcx_small,
					conf.mcx_full,
					512	//(((conf.t_line * 2) & 0x07 ) == 0)  ?	((conf.t_line * 2) + 8)		 :
						//					(((conf.t_line * 2) | 0x07) + 9)
											// full X mode
				};
    const unsigned size_y[4] = {	192U,
					conf.mcy_small,
					conf.mcy_full,
					324			// full y mode
				};
	temp.ox = temp.scx = size_x[conf.bordersize];
	temp.oy = temp.scy = size_y[conf.bordersize];
    
    //-------------------------------------------------------------------------
    if (temp.rflags & RF_2X)
    {
	temp.ox *= 2;
	temp.oy *= 2;
	//---------------------------------------------------------------------
	if (conf.fast_sl && (temp.rflags & RF_DRIVER) && (temp.rflags & (RF_CLIP | RF_OVR)))
            temp.oy /= 2;
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    if (temp.rflags & RF_3X)		{ temp.ox *= 3;  temp.oy *= 3; }
    if (temp.rflags & RF_4X)		{ temp.ox *= 4;  temp.oy *= 4; }
    if (temp.rflags & RF_64x48)		{ temp.ox = 64;  temp.oy = 48; }
    if (temp.rflags & RF_128x96)	{ temp.ox = 128; temp.oy = 96; }
    //-------------------------------------------------------------------------
   //дебагер
// if(temp.rflags & RF_MON)	{ temp.ox = 640; temp.oy = 480; }

//NS
//    if (temp.rflags & RF_MON)
//    {
//	if	(temp.rflags & RF_2X)	{ temp.ox = 640; temp.oy = 480; }	//RF_2X
//	else if	(temp.rflags & RF_3X)	{ temp.ox = 900; temp.oy = 480; }	//RF_3X
//	else				{ temp.ox = 320; temp.oy = 240; }	//RF_1X
//    }
    //-------------------------------------------------------------------------
    if (temp.rflags & RF_MON)
    {
	temp.ox *= temp.mon_scale;	//temp.scale;
	temp.oy *= temp.mon_scale;	//temp.scale;
    }
    //-------------------------------------------------------------------------
    
//   printf("temp.ox=%d, temp.oy=%d - 2nd\n", temp.ox, temp.oy);
   
}
//=============================================================================


//=============================================================================
static BOOL WINAPI DdEnumDevs(GUID *DevGuid, PSTR DrvDesc, PSTR DrvName, PVOID Ctx, HMONITOR Hm)
{
    (void) DrvDesc;
    (void) DrvName;
    (void) Hm;
    //-------------------------------------------------------------------------
    if (DevGuid)
    {
	memcpy( Ctx, DevGuid, sizeof(GUID));
	return FALSE;
    }
    //-------------------------------------------------------------------------
    return TRUE;
}
//=============================================================================








//=============================================================================
void start_dx()
{
//   printf("%s\n", __FUNCTION__);

//	printf("start_dx()\n");

    dsbuffer_sz = DSBUFFER_SZ;


    //=========================================================================
    // [NS] test

	
	WNDCLASSEX wndclass_debug_tool;	
	wndclass_debug_tool.cbSize =	sizeof(wndclass_debug_tool);
	wndclass_debug_tool.style =	CS_HREDRAW | 	//redraw window when horizontal resize
					CS_VREDRAW;	//redraw window when vertical resize
	wndclass_debug_tool.lpfnWndProc =	WndProc_debug_tools;	//WndProc;	
	wndclass_debug_tool.cbClsExtra =	0;
	wndclass_debug_tool.cbWndExtra =	0;	
	wndclass_debug_tool.hInstance =		GetModuleHandle(nullptr);
	wndclass_debug_tool.hIcon = 		LoadIcon(	NULL,
								IDI_APPLICATION
					     );	
	wndclass_debug_tool.hCursor = LoadCursor(	NULL,
							IDC_ARROW
						 );
	wndclass_debug_tool.hbrBackground =	(HBRUSH) GetStockObject(NULL_BRUSH);	//(WHITE_BRUSH);
	wndclass_debug_tool.lpszMenuName =	NULL;	
	wndclass_debug_tool.lpszClassName =	"Debug_Tool";	

	wndclass_debug_tool.hIconSm = LoadIcon(		NULL,
							IDI_APPLICATION
						);	
	//---------------------------------------------------------------------
	RegisterClassEx( &wndclass_debug_tool);
	
	
	//---------------------------------------------------------------------
	// на всякий случай инициализация масивов debug tools-ов
	for (int cnt = 0;    cnt < DEBUG_TOOLS_MAX_COUNT;    cnt++)
	{
	    debug_tool_type[ cnt] = DT_NONE;
	    debug_tool_exist[ cnt] = FALSE;	// 1 - флаг включенности
	    debug_tool_delete[ cnt] = FALSE;	// 1 - на удаление
	}
	//---------------------------------------------------------------------
	
//	run_debug_tool( DT_PALETTE_COMP_PAL);
//	run_debug_tool( DT_PALETTE_ATM3_PAL);
//	run_debug_tool( DT_PALETTE_PAL0);

/*
	
	//hwnd_debug_tool = CreateWindowEx(	WS_EX_NOPARENTNOTIFY,
	//debug_tool_create = TRUE;
	debug_tool_create_nmb = 0;
	debug_tool_type[0] = 1;
	debug_tool_hwnd[0] = CreateWindowEx(	WS_EX_NOPARENTNOTIFY,
						//WS_EX_NOPARENTNOTIFY,// |
						//  WS_EX_PALETTEWINDOW,
						"Debug_Tool", 		// window class name
						"Debug_Tool_Window",	// window caption
						WS_OVERLAPPEDWINDOW,	// window style
						CW_USEDEFAULT,		// initial x position
						CW_USEDEFAULT,		// initial y position
						320,			// initial x size
						320,			// initial y size
						NULL,			// parent window handle
						NULL,			// window menu handle
						GetModuleHandle(nullptr), // program instance handle
						NULL			// creation parameters
				       );
	ShowWindow( debug_tool_hwnd[0], SW_SHOWNORMAL);	//SW_SHOWMINNOACTIVE 
	UpdateWindow( debug_tool_hwnd[0]);
	//---------------------------------------------------------------------
	
	//hwnd_debug_tool_2 = CreateWindow(
	//debug_tool_create = TRUE;
	debug_tool_create_nmb = 1;
	debug_tool_type[1] = 2;
	debug_tool_hwnd[1] = CreateWindow(	
					"Debug_Tool", 		// window class name
					"Debug_Tool_Window_2",	// window caption
					WS_OVERLAPPEDWINDOW,	// window style
					CW_USEDEFAULT,		// initial x position
					CW_USEDEFAULT,		// initial y position
					320,			// initial x size
					200,			// initial y size
					NULL,			// parent window handle
					NULL,			// window menu handle
					GetModuleHandle(nullptr), // program instance handle
					NULL			// creation parameters
				       );
	ShowWindow( debug_tool_hwnd[1], SW_SHOWNORMAL);	//SW_SHOWMINNOACTIVE 
	UpdateWindow( debug_tool_hwnd[1]);
    //=========================================================================

	debug_tool_create_nmb = 2;
	debug_tool_type[2] = 3;
	debug_tool_hwnd[2] = CreateWindow(	
					"Debug_Tool", 		// window class name
					"Debug_Tool_Window_2",	// window caption
					WS_OVERLAPPEDWINDOW,	// window style
					CW_USEDEFAULT,		// initial x position
					CW_USEDEFAULT,		// initial y position
					320,			// initial x size
					200,			// initial y size
					NULL,			// parent window handle
					NULL,			// window menu handle
					GetModuleHandle(nullptr), // program instance handle
					NULL			// creation parameters
				       );
	ShowWindow( debug_tool_hwnd[2], SW_SHOWNORMAL);	//SW_SHOWMINNOACTIVE 
	UpdateWindow( debug_tool_hwnd[2]);
*/


    //=========================================================================
    
    WNDCLASSEX  wc = { };

    wc.cbSize = sizeof(WNDCLASSEX);

    wc.lpfnWndProc = WndProc;
    hIn = wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = "EMUL_WND";
    wc.hIcon = LoadIcon(hIn, MAKEINTRESOURCE(IDI_ICON2));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.style =	CS_HREDRAW |
		CS_VREDRAW |
		CS_DBLCLKS ;	// [NS] двойные клики по окну
		
//#define WHITE_BRUSH 0
//#define LTGRAY_BRUSH 1
//#define GRAY_BRUSH 2
//#define DKGRAY_BRUSH 3
//#define BLACK_BRUSH 4
//#define NULL_BRUSH 5
//#define HOLLOW_BRUSH NULL_BRUSH

//  wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);	// [NS]
    //  так можно закрашивать окно черным по дефолту
    //    так меньше видно некоторые белые хвосто артифакты
    //      пушо тода они черные
    // неп		НО без этого работает режим не рисовать ничего вообще
    //			  что лучше работает когда вытягиваешь дебагер из под границы экрана
    //  		  тогда на необновленной части висит старое изображение которое похоже на новое
    // на самом деле если убрать WM_ERASEBKGND то все ОК
    
    // более лучший вариант
    // теперь вообще без левых хвостов
    // (теперь хвосты в виде пролетающего сверху окна (на большой скорости это вообще не видно))
    wc.hbrBackground = (HBRUSH) GetStockObject( NULL_BRUSH);	// [NS]
  
    // есть еще какойто DC_BRUSH
    // интересно что он делает

      
    
    
    RegisterClassEx( &wc);		//!!!!
    //-------------------------------------------------------------------------
    for (int i = 0;     i < 9;     i++)
	crs[i] = LoadCursor(hIn, MAKEINTRESOURCE(IDC_C0+i));
    //-------------------------------------------------------------------------	
	
    //Alone Coder 0.36.6
    RECT rect1;
    SystemParametersInfo(	SPI_GETWORKAREA,
				0,
				&rect1,
				0
			);
    //~
    CalcWindowSize();

    int cx = int(temp.ox * temp.scale);
    int cy = int(temp.oy * temp.scale);

    RECT Client = {	0,
			0,
			cx,
			cy
		   };
    AdjustWindowRect(	&Client,
			WS_VISIBLE | WS_OVERLAPPEDWINDOW,
			0
		     );
    cx = Client.right - Client.left;
    cy = Client.bottom - Client.top;
    int winx = rect1.left + (rect1.right - rect1.left - cx) / 2;
    int winy = rect1.top + (rect1.bottom - rect1.top - cy) / 2;

    winx = winx < 0 ? 0 : winx;	// [alex rider]
    winy = winy < 0 ? 0 : winy;	// чтоб заголовок не вылазил за границу экрана
	
    main_menu = LoadMenu(	hIn,
				MAKEINTRESOURCE( IDR_MAINMENU)
			 );
//  wnd = CreateWindow(
    wnd = CreateWindowEx(	0,	//WS_EX_STATICEDGE	// DWORD dwExStyle,
				"EMUL_WND",			// LPCTSTR lpClassName,
				"Unreal_NS", //"UnrealSpeccy", 	// LPCTSTR lpWindowName
				WS_VISIBLE	|		// DWORD dwStyle
				  WS_OVERLAPPEDWINDOW,	//orig
				//WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX ,
				winx,				// int x
				winy,				// int y
				cx,				// int nWidth
				cy,				// int nHeight
				nullptr,			// HWND hWndParent
				main_menu,	//nullptr,	// HMENU hMenu
				hIn,				// HINSTANCE hInstance
				nullptr				// LPVOID lpParam
			);
			
    //-------------------------------------------------------------------------
    if (!wnd)
    {
	__debugbreak();
    }
    //-------------------------------------------------------------------------

    DragAcceptFiles(	wnd,
			1
		    );

    temp.gdidc = GetDC(wnd);
    GetSystemPaletteEntries(	temp.gdidc,
				0,
				0x100,
				syspalette
			    );

    //-------------------------------------------------------------------------
    HMENU sys = GetSystemMenu(	wnd,
				FALSE	//0
			      );

// WINUSERAPI WINBOOL WINAPI RemoveMenu(  HMENU hMenu, UINT uPosition, UINT uFlags);
// WINUSERAPI WINBOOL WINAPI DeleteMenu(  HMENU hMenu, UINT uPosition, UINT uFlags);

// WINUSERAPI WINBOOL WINAPI InsertMenuA( HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCSTR  lpNewItem);
// WINUSERAPI WINBOOL WINAPI InsertMenuW( HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCWSTR lpNewItem);
// WINUSERAPI WINBOOL WINAPI AppendMenuA( HMENU hMenu,                 UINT uFlags, UINT_PTR uIDNewItem, LPCSTR  lpNewItem);
// WINUSERAPI WINBOOL WINAPI AppendMenuW( HMENU hMenu,                 UINT uFlags, UINT_PTR uIDNewItem, LPCWSTR lpNewItem);
// WINUSERAPI WINBOOL WINAPI ModifyMenuA( HMENU hMnu,  UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCSTR  lpNewItem);
// WINUSERAPI WINBOOL WINAPI ModifyMenuW( HMENU hMnu,  UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCWSTR lpNewItem);

  
    if (sys)
    {
   


   
//	int cnt;
//	int menu_size = GetMenuItemCount( sys);
//	//---------------------------------------------------------------------
//	// удаляем всё из контексного меню нах
//	// ибо хз чего там напихает вин15 и в каких количествах
//	for (cnt = 0;    cnt < menu_size;    cnt++)
//	{
//	    printf("cnt %d\n",cnt);
//	    char string[200];
//	    GetMenuString( sys, cnt, string, 50, MF_BYPOSITION); 
//	    printf("%s\n",string);
//	    RemoveMenu( sys, cnt, MF_BYPOSITION);
//	//  DeleteMenu( sys, cnt, MF_BYPOSITION);
//	}
//	// но чета оно не удаляет все пункты
	//---------------------------------------------------------------------
	
//	// такое удаляет все пункты
//	DeleteMenu( sys, SC_MOVE,	MF_STRING);
//	DeleteMenu( sys, SC_RESTORE,	MF_STRING);
//	DeleteMenu( sys, SC_SIZE, 	MF_STRING);
//	DeleteMenu( sys, SC_MINIMIZE,	MF_STRING);
//	DeleteMenu( sys, SC_MAXIMIZE,	MF_STRING);
//	DeleteMenu( sys, SC_CLOSE,	MF_STRING);	// вместе отпадает кнопка закрытия
//	RemoveMenu( sys, SC_CLOSE,	MF_STRING);	// вместе отпадает кнопка закрытия

//	DeleteMenu( sys, SC_CLOSE,	MF_STRING);	// 
//	AppendMenu( sys, MF_STRING, SC_CLOSE, "Сlouze");	//так кнопка не отпадает



	//---------------------------------------------------------------------
	// генерация разных названий масштабов (выставляется только при старте)		// [NS]
	// а потом меняется в emulkeys.cpp
	char sys_x1[5]; char sys_x2[5]; char sys_x3[5]; char sys_x4[5];
	//---------------------------------------------------------------------
	if 	(temp.rflags & RF_4X)
	    { strcpy(sys_x1,"x4"); strcpy(sys_x1,"x8"); strcpy(sys_x1,"x12"); strcpy(sys_x1,"x16"); }
	//---------------------------------------------------------------------
	else if	(temp.rflags & RF_3X)
	    { strcpy(sys_x1,"x3"); strcpy(sys_x2,"x6"); strcpy(sys_x3,"x9"); strcpy(sys_x4,"x12"); }
	//---------------------------------------------------------------------
	else if	(temp.rflags & RF_2X)
	    { strcpy(sys_x1,"x2"); strcpy(sys_x2,"x4"); strcpy(sys_x3,"x6"); strcpy(sys_x4,"x8"); }
	//---------------------------------------------------------------------
	else if (temp.rflags & RF_1X)
	    { strcpy(sys_x1,"x1"); strcpy(sys_x2,"x2"); strcpy(sys_x3,"x3"); strcpy(sys_x4,"x4"); }
	//---------------------------------------------------------------------


	InsertMenu( sys, 0, MF_BYPOSITION | MF_STRING, SCU_LOCK_MOUSE, "&Lock mouse");
	InsertMenu( sys, 1, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr); //------
	InsertMenu( sys, 2, MF_BYPOSITION | MF_STRING, SCU_SCALE1, sys_x1);
	InsertMenu( sys, 3, MF_BYPOSITION | MF_STRING, SCU_SCALE2, sys_x2);
	InsertMenu( sys, 4, MF_BYPOSITION | MF_STRING, SCU_SCALE3, sys_x3);
	InsertMenu( sys, 5, MF_BYPOSITION | MF_STRING, SCU_SCALE4, sys_x4);
	InsertMenu( sys, 6, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr); //------
//	InsertMenu( sys, 7, MF_BYPOSITION, SC_CLOSE);

	// оно и без этого вроде работает
	// но на всякий случай
	// ато вдруг не будет отображать стартовые настройки из конфига
	//---------------------------------------------------------------------
        HMENU main_menu = GetMenu( wnd);
	//---------------------------------------------------------------------
	if (main_menu)
	{
	    ModifyMenu( main_menu, IDM_SCALE1, MF_BYCOMMAND | MF_STRING, IDM_SCALE1, sys_x1);
	    ModifyMenu( main_menu, IDM_SCALE2, MF_BYCOMMAND | MF_STRING, IDM_SCALE2, sys_x2);
	    ModifyMenu( main_menu, IDM_SCALE3, MF_BYCOMMAND | MF_STRING, IDM_SCALE3, sys_x3);
	    ModifyMenu( main_menu, IDM_SCALE4, MF_BYCOMMAND | MF_STRING, IDM_SCALE4, sys_x4);
	}
	//---------------------------------------------------------------------

//	// старая менюшука
//	AppendMenu(sys, MF_SEPARATOR, 0, nullptr);
//	AppendMenu(sys, MF_STRING, SCU_SCALE1, "x1");
//	AppendMenu(sys, MF_STRING, SCU_SCALE2, "x2");
//	AppendMenu(sys, MF_STRING, SCU_SCALE3, "x3");
//	AppendMenu(sys, MF_STRING, SCU_SCALE4, "x4");
//	AppendMenu(sys, MF_STRING, SCU_LOCK_MOUSE, "&Lock mouse");

    }
    //-------------------------------------------------------------------------
    InitCommonControls();

    HRESULT r;
    GUID DdDevGuid;
    //-------------------------------------------------------------------------
    if ( (r = DirectDrawEnumerateEx(	DdEnumDevs,
					&DdDevGuid,
					DDENUM_ATTACHEDSECONDARYDEVICES
				)
	 ) != DD_OK
     )
    {
	printrdd("DirectDrawEnumerate()", r);
	exit();
    }
    //-------------------------------------------------------------------------
    LPDIRECTDRAW dd0;
    //-------------------------------------------------------------------------
    if ( (r = DirectDrawCreate(	   nullptr /*&DdDevGuid*/,
				   &dd0,
				   nullptr
			         )
	) != DD_OK
     )
    {
	printrdd("DirectDrawCreate()", r);
	exit();
    }
    //-------------------------------------------------------------------------
    if ((r = dd0->QueryInterface( IID_IDirectDraw2, (void**)&dd)) != DD_OK)
    {
	printrdd("IDirectDraw::QueryInterface(IID_IDirectDraw2)", r);
	exit();
    }
    //-------------------------------------------------------------------------
    dd0->Release();

    color(CONSCLR_HARDITEM);
    printf("gfx: ");
    //-------------------------------------------------------------------------
    char vmodel[MAX_DDDEVICEID_STRING + 32];
    *vmodel = 0;
    //-------------------------------------------------------------------------
    if (conf.detect_video)
    {
	LPDIRECTDRAW4 dd4;
	//---------------------------------------------------------------------
	if ((r = dd->QueryInterface(IID_IDirectDraw4, (void**)&dd4)) == DD_OK)
	{
	    DDDEVICEIDENTIFIER di;
	    //-----------------------------------------------------------------
	    if (dd4->GetDeviceIdentifier( &di, 0) == DD_OK)
	    {
		trim_right(di.szDescription);
		CharToOem(di.szDescription, di.szDescription);
		sprintf(vmodel, "%04lX-%04lX (%s)", di.dwVendorId, di.dwDeviceId, di.szDescription);
	    }
	    //-----------------------------------------------------------------
	    else
	    {
		sprintf(vmodel, "unknown device");
	    }
	    //-----------------------------------------------------------------
	    dd4->Release();
	}
	//---------------------------------------------------------------------
	if (*vmodel)
	    strcat( vmodel, ", ");
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    DDCAPS caps;
    caps.dwSize = sizeof caps;
    dd->GetCaps(&caps, nullptr);

    color(CONSCLR_HARDINFO);

    const u32 Vmem = caps.dwVidMemTotal;
    printf("%s%uMb VRAM available\n", vmodel, unsigned(Vmem/(1024U*1024U)+((Vmem%(1024U*1024U))>512U*1024U)));

    max_modes = 0;
    dd->EnumDisplayModes(DDEDM_REFRESHRATES | DDEDM_STANDARDVGAMODES, nullptr, nullptr, callb);
    //-------------------------------------------------------------------------
    if ((temp.rflags & (RF_D3D | RF_D3DE)))
	StartD3d(wnd);
    //-------------------------------------------------------------------------
    
    
    
    //-------------------------------------------------------------------------
    WAVEFORMATEX wf = { };
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nSamplesPerSec = conf.sound.fq;
    wf.nChannels = 2;
    wf.wBitsPerSample = 16;
    wf.nBlockAlign = 4;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
    //-------------------------------------------------------------------------
    if (conf.sound.do_sound == do_sound_wave)
    {
	MMRESULT mmr;
	//---------------------------------------------------------------------
	if ( (mmr = waveOutOpen(    &hwo,
				    WAVE_MAPPER,
				    &wf,
				    0,
				    0,
				    CALLBACK_NULL
				 )
	    ) != MMSYSERR_NOERROR
	 )
	{
	    printrmm("waveOutOpen()", mmr);
	    hwo = nullptr;
	    goto sfail;
	}
	//---------------------------------------------------------------------
	wqhead = 0;
	wqtail = 0;
    }
    //-------------------------------------------------------------------------
    else if (conf.sound.do_sound == do_sound_ds)
    {
	//---------------------------------------------------------------------
	if ( (r = DirectSoundCreate(	nullptr,
					&ds,
					nullptr
				)
		) != DS_OK
	 )
	{
	    printrds("DirectSoundCreate()", r);
	    goto sfail;
	}
	//---------------------------------------------------------------------
	r = -1;
	//---------------------------------------------------------------------
	if (conf.sound.dsprimary)	// Off by default
	{
	    r = ds->SetCooperativeLevel(    wnd,
					    DSSCL_WRITEPRIMARY
					  );
	}
	//---------------------------------------------------------------------
	if (r != DS_OK)
	{
	    r = ds->SetCooperativeLevel(    wnd,
					    DSSCL_NORMAL
					  );
	    conf.sound.dsprimary = 0;
	}
	//---------------------------------------------------------------------
	if (r != DS_OK)
	{
	    printrds("IDirectSound::SetCooperativeLevel()", r);
	    goto sfail;
	}
	//---------------------------------------------------------------------
	DSBUFFERDESC dsdesc = { sizeof(DSBUFFERDESC) };
	r = -1;
	//---------------------------------------------------------------------
	if (conf.sound.dsprimary)	// Off by default
	{
	    dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_PRIMARYBUFFER;
	    dsdesc.dwBufferBytes = 0;
	    dsdesc.lpwfxFormat = nullptr;
	    r = ds->CreateSoundBuffer(	&dsdesc,
					&dsbf,
					nullptr
				    );
	    //-----------------------------------------------------------------
	    if (r != DS_OK)
	    {
		printrds("IDirectSound::CreateSoundBuffer() [primary]", r);
	    }
	    //-----------------------------------------------------------------
	    else
	    {
		r = dsbf->SetFormat( &wf);
		//-------------------------------------------------------------
		if (r != DS_OK)
		{
		    printrds("IDirectSoundBuffer::SetFormat()", r);
		    goto sfail;
		}
		//-------------------------------------------------------------
		DSBCAPS caps;
		caps.dwSize = sizeof caps;
		dsbf->GetCaps( &caps);
		dsbuffer_sz = caps.dwBufferBytes;
	    }
	    //-----------------------------------------------------------------
	} // (conf.sound.dsprimary)
	//---------------------------------------------------------------------
	if (r != DS_OK)
	{
	    dsdesc.lpwfxFormat = &wf;
	    dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS;
	    dsbuffer_sz = dsdesc.dwBufferBytes = DSBUFFER_SZ;
	    //-----------------------------------------------------------------
	    if ( (r = ds->CreateSoundBuffer(	&dsdesc,
						&dsbf,
						nullptr
					    )
			) != DS_OK
	     )
	    {
		printrds("IDirectSound::CreateSoundBuffer()", r);
		goto sfail;
	    }
	    //-----------------------------------------------------------------
	    conf.sound.dsprimary = 0;
	}
	//---------------------------------------------------------------------
	dsoffset = dsbuffer_sz / 4;
    }
    //-------------------------------------------------------------------------
    else
    {
sfail:
	conf.sound.do_sound = do_sound_none;
    }
    //-------------------------------------------------------------------------
    
    
    
    
    //-------------------------------------------------------------------------
    
    LPDIRECTINPUT di;
    r = DirectInputCreate( hIn, DIRECTINPUT_VERSION, &di,nullptr);
    
    //-------------------------------------------------------------------------
    if ((r != DI_OK) && (r = DirectInputCreate( hIn, 0x0300, &di, nullptr)) != DI_OK)
    {
	printrdi("DirectInputCreate()", r);
	exit();
    }
    //-------------------------------------------------------------------------
    if ((r = di->CreateDevice(GUID_SysKeyboard, &dikeyboard, nullptr)) != DI_OK)
    {
	printrdi("IDirectInputDevice::CreateDevice() (keyboard)", r);
	exit();
    }
    //-------------------------------------------------------------------------
    if ((r = dikeyboard->SetDataFormat( &c_dfDIKeyboard)) != DI_OK)
    {
	printrdi("IDirectInputDevice::SetDataFormat() (keyboard)", r);
	exit();
    }
    //-------------------------------------------------------------------------
    if ((r = dikeyboard->SetCooperativeLevel( wnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)) != DI_OK)
    {
	printrdi("IDirectInputDevice::SetCooperativeLevel() (keyboard)", r);
	exit();
    }
    //-------------------------------------------------------------------------
    if ((r = di->CreateDevice( GUID_SysMouse, &dimouse, nullptr)) == DI_OK)
    {
	//---------------------------------------------------------------------
	if ((r = dimouse->SetDataFormat( &c_dfDIMouse)) != DI_OK)
	{
	    printrdi("IDirectInputDevice::SetDataFormat() (mouse)", r);
	    exit();
	}
	//---------------------------------------------------------------------
	if ((r = dimouse->SetCooperativeLevel( wnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)) != DI_OK)
	{
	    printrdi("IDirectInputDevice::SetCooperativeLevel() (mouse)", r);
	    exit();
	}
	//---------------------------------------------------------------------
	DIPROPDWORD dipdw = { };
	dipdw.diph.dwSize       = sizeof( dipdw);
	dipdw.diph.dwHeaderSize = sizeof( dipdw.diph);
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = DIPROPAXISMODE_ABS;
	//---------------------------------------------------------------------
	if ((r = dimouse->SetProperty( DIPROP_AXISMODE, &dipdw.diph)) != DI_OK)
	{
	    printrdi("IDirectInputDevice::SetProperty() (mouse)", r);
	    exit();
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    else
    {
	color( CONSCLR_WARNING);
	printf("warning: no mouse\n");
	dimouse = nullptr;
    }
    //-------------------------------------------------------------------------
    if ((r = di->EnumDevices( DIDEVTYPE_JOYSTICK, InitJoystickInput, di, DIEDFL_ATTACHEDONLY)) != DI_OK)
    {
	printrdi("IDirectInput::EnumDevices(DIDEVTYPE_JOYSTICK,...)", r);
	exit();
    }
    //-------------------------------------------------------------------------
    di->Release();
//[vv]   SetKeyboardState(kbdpc); // fix bug in win95
}
//=============================================================================




//=============================================================================
static void DoneD3d(bool DeInitDll)
{
//    printf("%s(%d)\n", __FUNCTION__, DeInitDll);
    ULONG RefCnt;
    //-------------------------------------------------------------------------
    if (SurfTexture)
    {
	RefCnt = SurfTexture->Release();
	(void)RefCnt;
/*
#ifdef _DEBUG //---------------------------------------------------------------
	if(RefCnt != 0)
	{
	    __debugbreak();
	}
#endif //----------------------------------------------------------------------
*/
	SurfTexture = nullptr;
    }
    //-------------------------------------------------------------------------
    if (Texture)
    {
	RefCnt = Texture->Release();
	
#ifdef _DEBUG //---------------------------------------------------------------
	if (RefCnt != 0)
        {
	    __debugbreak();
        }
#endif //----------------------------------------------------------------------

	Texture = nullptr;
    }
    //-------------------------------------------------------------------------
    if (D3dDev)
    {
	RefCnt = D3dDev->Release();
	
#ifdef _DEBUG //---------------------------------------------------------------
	if (RefCnt != 0)
        {
	    __debugbreak();
	}
#endif //----------------------------------------------------------------------

	D3dDev = nullptr;
    }
    //-------------------------------------------------------------------------
    if (D3d9)
    {
	RefCnt = D3d9->Release();
	
#ifdef _DEBUG //---------------------------------------------------------------
	if (RefCnt != 0)
	{
	    __debugbreak();
	}
#endif //----------------------------------------------------------------------

	D3d9 = nullptr;
    }
    //-------------------------------------------------------------------------
    if (DeInitDll && D3d9Dll)
    {
	FreeLibrary(D3d9Dll);
	D3d9Dll = nullptr;
    }
    //-------------------------------------------------------------------------
}
//=============================================================================


//=============================================================================
void done_dx()
{
    sound_stop();
    // ????????????
    //-------------------------------------------------------------------------
    if (pal)	 
	pal->Release();	   
    pal = nullptr;
    //-------------------------------------------------------------------------
    if (surf2)	  
	surf2->Release();	   
    surf2 = nullptr;
    //-------------------------------------------------------------------------
    if (surf1)	  
	surf1->Release();	   
    surf1 = nullptr;
    //-------------------------------------------------------------------------
    if (surf0)	  
	surf0->Release();	  
    surf0 = nullptr;
    //-------------------------------------------------------------------------
    if (sprim)	  
	sprim->Release();	   
    sprim = nullptr;
    //-------------------------------------------------------------------------
    if (clip)	  
	clip->Release();	   
    clip = nullptr;
    //-------------------------------------------------------------------------
    if (dd)	  
	dd->Release();	   
    dd = nullptr;
    //-------------------------------------------------------------------------
    if (SurfMem1)
	_aligned_free( SurfMem1); 
    SurfMem1 = nullptr;
    //-------------------------------------------------------------------------
    if (dikeyboard)
    {
	dikeyboard->Unacquire();
	dikeyboard->Release();
	dikeyboard = nullptr;
    }
    //-------------------------------------------------------------------------
    if (dimouse)
    {
	dimouse->Unacquire();
	dimouse->Release();
	dimouse = nullptr;
    }
    //-------------------------------------------------------------------------
    if (dijoyst)
    {
	dijoyst->Unacquire();
	dijoyst->Release();
	dijoyst = nullptr;
    }
    //-------------------------------------------------------------------------
    if (hwo)
    {
	waveOutReset( hwo);
	/* waveOutUnprepareHeader()'s ? */ 
	waveOutClose( hwo);
    }
    //-------------------------------------------------------------------------
    if (dsbf)
	dsbf->Release();
    dsbf = nullptr;
    //-------------------------------------------------------------------------
    if (ds)
	ds->Release();
    ds = nullptr;
    //-------------------------------------------------------------------------
    if (hbm)
	DeleteObject( hbm);
    hbm = nullptr;
    //-------------------------------------------------------------------------
    if (temp.gdidc)
	ReleaseDC( wnd, temp.gdidc);
    temp.gdidc = nullptr;
    //-------------------------------------------------------------------------
    DoneD3d();
    //-------------------------------------------------------------------------
    if (wnd)
	DestroyWindow( wnd);
    //-------------------------------------------------------------------------
}
//=============================================================================










void reinit_sound_ds()	// [NS]
{
// так получаетсо вытянууть usb audio и продолжить работать на наборртном звуке
// но не получаетсо подхватить воткнутое обратно usb audio
// пишот DirectSoundCreate(): 0x88780078, DSERR_NODRIVER

	//printf("1\n");
	sound_stop();
	
reinit_again:
	//conf.sound.do_sound == do_sound_wave;
        //sound_play();
	//return;

    printf("reinit sound\n");
    Sleep( 200);

        HRESULT r;

    WAVEFORMATEX wf = { };
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nSamplesPerSec = conf.sound.fq;
    wf.nChannels = 2;
    wf.wBitsPerSample = 16;
    wf.nBlockAlign = 4;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
    //-------------------------------------------------------------------------
    if (conf.sound.do_sound == do_sound_wave)
    {
	MMRESULT mmr;
	//---------------------------------------------------------------------
	if ((mmr = waveOutOpen( &hwo, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL)) != MMSYSERR_NOERROR)
	{
	    printrmm("waveOutOpen()", mmr);
	    hwo = nullptr;
	    goto sfail;
	}
	//---------------------------------------------------------------------
	wqhead = 0;
	wqtail = 0;
    }
    //-------------------------------------------------------------------------
    else if (conf.sound.do_sound == do_sound_ds)
    {
	//---------------------------------------------------------------------
	if ((r = DirectSoundCreate( nullptr, &ds, nullptr)) != DS_OK)
	{
	    printrds("DirectSoundCreate()", r);
	    //тут потом ошибка DSERR_NODRIVER
	    //if (r == DSERR_NODRIVER)
	    //	goto reinit_again;	// не помогает
	    goto sfail;
	}
	//---------------------------------------------------------------------
	r = -1;
	//---------------------------------------------------------------------
	if (conf.sound.dsprimary)
	    r = ds->SetCooperativeLevel( wnd, DSSCL_WRITEPRIMARY);
	//---------------------------------------------------------------------
	if (r != DS_OK)
	{
	    r = ds->SetCooperativeLevel( wnd, DSSCL_NORMAL);
	    conf.sound.dsprimary = 0;
	}
	//---------------------------------------------------------------------
	if (r != DS_OK)
	{
	    printrds("IDirectSound::SetCooperativeLevel()", r);
	    goto sfail;
	}
	//---------------------------------------------------------------------
	DSBUFFERDESC dsdesc = { sizeof(DSBUFFERDESC) };
	r = -1;
	//---------------------------------------------------------------------
	if (conf.sound.dsprimary)
	{
	    dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_PRIMARYBUFFER;
	    dsdesc.dwBufferBytes = 0;
	    dsdesc.lpwfxFormat = nullptr;
	    r = ds->CreateSoundBuffer( &dsdesc, &dsbf, nullptr);
	    //-----------------------------------------------------------------
	    if (r != DS_OK)
	    {
		printrds("IDirectSound::CreateSoundBuffer() [primary]", r);
	    }
	    //-----------------------------------------------------------------
	    else
	    {
		r = dsbf->SetFormat( &wf);
		//-------------------------------------------------------------
		if (r != DS_OK)
		{
		    printrds("IDirectSoundBuffer::SetFormat()", r);
		    goto sfail;
		}
		//-------------------------------------------------------------
		DSBCAPS caps;
		caps.dwSize = sizeof caps;
		dsbf->GetCaps( &caps);
		dsbuffer_sz = caps.dwBufferBytes;
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	if (r != DS_OK)
	{
	    dsdesc.lpwfxFormat = &wf;
	    dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS;
	    dsbuffer_sz = dsdesc.dwBufferBytes = DSBUFFER_SZ;
	    //-----------------------------------------------------------------
	    if ((r = ds->CreateSoundBuffer( &dsdesc, &dsbf, nullptr)) != DS_OK)
	    {
		printrds("IDirectSound::CreateSoundBuffer()", r);
		goto sfail;
	    }
	    //-----------------------------------------------------------------
	    conf.sound.dsprimary = 0;
	}
	//---------------------------------------------------------------------
	dsoffset = dsbuffer_sz / 4;
    }
    //-------------------------------------------------------------------------
    else
    {
sfail:
	conf.sound.do_sound = do_sound_none;
    }
    
    
    sound_play();
}

