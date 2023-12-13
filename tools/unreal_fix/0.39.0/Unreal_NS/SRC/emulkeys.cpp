#include "std.h"

#include "resource.h"
#include "emul.h"
#include "vars.h"
#include "config.h"
#include "draw.h"
#include "dx.h"
#include "debug.h"
#include "memory.h"
#include "sndrender/sndcounter.h"
#include "sound.h"
#include "savesnd.h"
#include "tape.h"
#include "gui.h"
#include "leds.h"
#include "snapshot.h"
#include "fdd.h"
#include "init.h"
#include "z80.h"
#include "emulkeys.h"
#include "funcs.h"
#include "util.h"

#include "input.h"
#include "zxusbnet.h"


#include "version.h"	// для окна about [NS]


// вынесено в resource.h
// static const DWORD SCU_SCALE1 = 0x10;
// static const DWORD SCU_SCALE2 = 0x20;
// static const DWORD SCU_SCALE3 = 0x30;
// static const DWORD SCU_SCALE4 = 0x40;
// static const DWORD SCU_LOCK_MOUSE = 0x50;


//=============================================================================
void main_pause()
{
	main_pause_flag ^= 1;	

	if (main_pause_flag == 0)
	{
		//active = 1; //парализует менюшки
		adjust_mouse_cursor();
		sound_play();
		pause = 0;
		//OnExitGui(false);
	}
	else
	{
		text_i(rbuf+temp.scx/2-8,"pause",0x0F); flip();
		pause = 1;
		sound_stop();
		updatebitmap();	// сохранение текущего изображения окна
				// для востановления под окнами
		//active = 0;
		adjust_mouse_cursor();	
		//OnEnterGui();
		
		//clear_buffer();	//sound не тушим но чтоб не пищал
					//не помогает
					//крашитсо при SoundDrv = wave
	}


/*
// нельзя вызывать в дебагере!!!!
	
   text_i(rbuf+temp.scx/2-8,"pause",0x0F); flip();


		if (pause == 1)
		{
		//printf ("pause when pause\n");
		pause = 0;
		return;
		}

//	printf ("pause when run\n");
	
	pause = 1;
	sound_stop();
	updatebitmap();
	active = 0;
	adjust_mouse_cursor();
	
//висит пока не наступит другое событие

//   while (!process_msgs()) Sleep(10);
//   eat();


		while (!process_msgs()) 
		{
		Sleep(10);
			if (pause == 0) break;
		}

//	while (pause)
//	{
//	//dispatch(ac_main);
//	process_msgs();		//обработка менюшек
//   
//	//eat();
//	Sleep(10);
//	//input.make_matrix(); //dispatch_more(ac_main); //dispatch(ac_main);
//		if (kbdpc[DIK_PAUSE] == 1)
//		{
//		break;
//		}
//	}


	eat();

//	printf ("end pause\n");
   active = 1; adjust_mouse_cursor();
   sound_play();
   pause = 0;
   
*/
}
//=============================================================================


//=============================================================================
void main_debug()
{
   Z80 &cpu = CpuMgr.Cpu();

   cpu.dbgchk = 1;	//debug cpu mode
   cpu.dbgbreak = 1;
   dbgbreak = 1;
}
//=============================================================================


enum { FIX_FRAME = 0, FIX_LINE, FIX_PAPER, FIX_NOPAPER, FIX_HWNC, FIX_LAST };
static const char *fix_titles[FIX_LAST] = {
   "%d t-states / int",
   "%d t-states / line",
   "paper starts at %d",
   "border only: %d",
   "hardware mc: %d"
};


static unsigned char whatfix = 0, whatsnd = 0;
static unsigned char fixmode = u8(-1U);
static int mul0 = 100, mul1 = 1000;

//=============================================================================
static void chfix(int dx)
{
   if (!fixmode) {
      unsigned value;
      switch (whatfix) {
         case FIX_FRAME: value = (conf.frame = unsigned(int(conf.frame) + dx)); break;
         case FIX_LINE: value = (conf.t_line = unsigned(int(conf.t_line) + dx)); break;
         case FIX_PAPER: value = (conf.paper = unsigned(int(conf.paper) + dx)); break;
         case FIX_NOPAPER: value = (conf.nopaper ^= dx?1:0); break;
         case FIX_HWNC: value = (comp.pEFF7 ^= dx?EFF7_HWMC:0)? 1 : 0; break;
         default: return;
      }
      video_timing_tables();
      apply_sound(); // t/frame affects AY engine!
      sprintf(statusline, fix_titles[whatfix], value); statcnt=50;
      if (dx) conf.ula_preset = u8(-1U);
      return;
   }
   if (fixmode != 1) return;

   dx = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0);

   *statusline = 0; statcnt = 50;
   switch (whatsnd) {
      case 0:
         conf.sound.ay_stereo = u8(int(conf.sound.ay_stereo)+dx+int(num_aystereo)) % num_aystereo;
         sprintf(statusline, "Stereo preset: %s", aystereo[conf.sound.ay_stereo]);
         break;
      case 1:
         if (dx) conf.sound.ay_samples ^= 1;
         sprintf(statusline, "Digital Soundchip: %s", conf.sound.ay_samples? "yes" : "no");
         break;
      case 2:
         conf.sound.ay_vols = u8(int(conf.sound.ay_vols)+int(num_ayvols)+dx) % num_ayvols;
         sprintf(statusline, "Chip Table: %s", ayvols[conf.sound.ay_vols]);
         break;
      case 3:
         conf.pal = unsigned(int(conf.pal)+dx);
         if (conf.pal == conf.num_pals) conf.pal = 0;
         if (conf.pal == -1U) conf.pal = conf.num_pals-1;
         sprintf(statusline, "Palette: %s", pals[conf.pal].name);
         video_color_tables();
         return;
   }
   apply_sound();
}
//=============================================================================


//=============================================================================
void main_selectfix()
{
   if (!fixmode) whatfix = (whatfix+1) % FIX_LAST;
   fixmode = 0; mul0 = 1; mul1 = 10;
   if(whatfix == FIX_FRAME)
   {
       mul0 = 100;
       mul1 = 1000;
   }
   chfix(0);
}
//=============================================================================


//=============================================================================
void main_selectsnd()
{
   if (fixmode==1) whatsnd = (whatsnd+1) & 3;
   fixmode = 1;
   chfix(0);
}
//=============================================================================


void main_incfix() { chfix(mul0); }
void main_decfix() { chfix(-mul0); }
void main_incfix10() { chfix(mul1); }
void main_decfix10() { chfix(-mul1); }


//=============================================================================
void main_leds()
{
   conf.led.enabled ^= 1;
   sprintf(statusline, "leds %s", conf.led.enabled ? "on" : "off"); statcnt = 50;
}
//=============================================================================


//=============================================================================
void main_maxspeed()
{
   conf.sound.enabled ^= 1;
   temp.frameskip = conf.sound.enabled? conf.frameskip : conf.frameskipmax;
   if (conf.sound.enabled) sound_play(); else sound_stop();
   sprintf(statusline, "Max speed: %s", conf.sound.enabled ? "NO" : "YES"); statcnt = 50;
}
//=============================================================================


// select filter / driver through gui dialog ----------------------------
//=============================================================================
static INT_PTR CALLBACK filterdlg(HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
   if (msg == WM_INITDIALOG)
   {
      HWND box = GetDlgItem(dlg, IDC_LISTBOX); int i;

      if (lp) {
         for (i = 0; drivers[i].name; i++)
            SendMessage(box, LB_ADDSTRING, 0, (LPARAM)drivers[i].name);
         SendMessage(box, LB_SETCURSEL, conf.driver, 0);
         SetWindowText(dlg, "Select driver for rendering");
      } else {
         for (i = 0; renders[i].name; i++)
            SendMessage(box, LB_ADDSTRING, 0, (LPARAM)renders[i].name);
         SendMessage(box, LB_SETCURSEL, conf.render, 0);
      }

      RECT rcw, cli; GetWindowRect(box, &rcw); GetClientRect(box, &cli);
      RECT rcd; GetWindowRect(dlg, &rcd);

      int nc_width = (rcw.right - rcw.left) - (cli.right - cli.left);
      int nc_height = (rcw.bottom - rcw.top) - (cli.bottom - cli.top);
      int dlg_w = (rcd.right - rcd.left) - (rcw.right - rcw.left);
      int dlg_h = (rcd.bottom - rcd.top) - (rcw.bottom - rcw.top);
      nc_width += 300;
      nc_height += i*SendMessage(box, LB_GETITEMHEIGHT, 0, 0);
      dlg_w += nc_width; dlg_h += nc_height;
      SetWindowPos(box, nullptr, 0, 0, nc_width, nc_height, SWP_NOZORDER | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_SHOWWINDOW);

      GetWindowRect(wnd, &rcw);
      SetWindowPos(dlg, nullptr,
         rcw.left + ((rcw.right-rcw.left)-dlg_w)/2,
         rcw.top + ((rcw.bottom-rcw.top)-dlg_h)/2,
         dlg_w, dlg_h, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_SHOWWINDOW);

      SetFocus(box);
      return FALSE;
   }

   if ((msg == WM_COMMAND && wp == IDCANCEL) ||
       (msg == WM_SYSCOMMAND && (wp & 0xFFF0) == SC_CLOSE))
   {
       EndDialog(dlg, -1);
       return TRUE;
   }

   if (msg == WM_COMMAND)
   {
      int control = LOWORD(wp);
      if (control == IDOK || (control == IDC_LISTBOX && HIWORD(wp) == LBN_DBLCLK))
      {
         EndDialog(dlg, SendDlgItemMessage(dlg, IDC_LISTBOX, LB_GETCURSEL, 0, 0));
         return TRUE;
      }
   }
   return FALSE;
}
//=============================================================================


//=============================================================================
void main_selectfilter()
{
   OnEnterGui();
   INT_PTR index = DialogBoxParam(hIn, MAKEINTRESOURCE(IDD_FILTER_DIALOG), wnd, filterdlg, 0);
   eat();
   if (index < 0)
   {
       OnExitGui();
       return;
   }
   OnExitGui(false);
   conf.render = unsigned(index);
   sprintf(statusline, "Video: %s", renders[index].name); statcnt = 50;
   apply_video(); eat();
}
//=============================================================================


//=============================================================================
void main_selectdriver()
{
    //printf("main_selectdriver\n");
    if (!(dbgbreak))	// [NS] теперь можно выбирать из дебагера
    {
	if (!(temp.rflags & RF_DRIVER))
	{
	    //printf("1\n");
	    strcpy(statusline, "Not available for this filter");
	    statcnt = 50;
	    return;
	}
    }
    //-------------------------------------------------------------------------
    OnEnterGui();
    INT_PTR index = DialogBoxParam(hIn, MAKEINTRESOURCE(IDD_FILTER_DIALOG), wnd, filterdlg, 1);
    eat();
    //-------------------------------------------------------------------------
    if (index < 0)
    {
	OnExitGui();
	return;
    }
    //-------------------------------------------------------------------------
    OnExitGui(false);
    conf.driver = unsigned(index);
    sprintf(statusline, "Render to: %s", drivers[index].name); statcnt = 50;
    if (!(dbgbreak))	// [NS]
    {
	apply_video();
    }
    eat();
}
//=============================================================================

// ----------------------------------------------------------------------

//=============================================================================
void main_poke()
{
    OnEnterGui();
    DialogBox(	    hIn,
		    MAKEINTRESOURCE( IDD_POKE),
		    wnd,
		    pokedlg
		);
    eat();
    OnExitGui();
}
//=============================================================================


//=============================================================================
void main_starttape()
{
   //if (comp.tape.play_pointer) stop_tape(); else start_tape();
  
    (!comp.tape.stopped)  ?  stop_tape() :
			     start_tape();

/*
    // теперь в WM_INITMENU
    //-------------------------------------------------------------------------
    HMENU main_menu = GetMenu( wnd);
    //-------------------------------------------------------------------------
    if (main_menu)
    {      
	//---------------------------------------------------------------------
	if (comp.tape.stopped)
	{
	    ModifyMenu(	    main_menu,
			    IDM_TAPE_CONTROL,
			    MF_BYCOMMAND | MF_STRING,
			    IDM_TAPE_CONTROL,
			    "Start Tape"
			);
	}
	//---------------------------------------------------------------------
	else
	{
	    ModifyMenu(	    main_menu,
			    IDM_TAPE_CONTROL,
			    MF_BYCOMMAND | MF_STRING,
			    IDM_TAPE_CONTROL,
			    "Stop Tape"
			);
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    */
    
}
//=============================================================================


//=============================================================================
void main_tapebrowser()
{
#ifdef MOD_SETTINGS
    lastpage = "TAPE";
    setup_dlg();
#endif
}
//=============================================================================


//=============================================================================
#ifndef MOD_SETTINGS
void setup_dlg() {}
#endif
//=============================================================================


//=============================================================================
static const char *getrom(ROM_MODE page)
{
   switch (page) {
      case RM_128: return "Basic 128";
      case RM_SYS: return "Service ROM";
      case RM_DOS: return "TR-DOS";
      case RM_SOS: return "Basic 48";
      case RM_CACHE: return "Cache";
   }
   return "???";
}
//=============================================================================


//=============================================================================
static void m_reset(ROM_MODE page)
{
    load_atm_font();

    sprintf(statusline, "Reset to %s", getrom(page));
    statcnt = 50;
   
    input.buffer_enabled = false;	// DimkaM disable ps/2 access
    input.buffer.Empty();		// NEDOREPO
    //-------------------------------------------------------------------------
    if (conf.atm620_xt_keyb)
    {
	input.atm620_xt_keyb_buffer_enabled = TRUE;	// [NS]
	input.atm620_xt_keyb_buffer.Empty();		//
    }
    //-------------------------------------------------------------------------
    nmi_pending = 0;
    cpu.nmi_in_progress = false;
    reset(page);
}
//=============================================================================


//=============================================================================
void main_reset128() { m_reset(RM_128); }
void main_resetsys() { m_reset(RM_SYS); }
void main_reset48() { m_reset(RM_SOS); comp.p7FFD = 0x30; comp.pEFF7 |= EFF7_LOCKMEM; /*Alone Coder*/}
void main_resetbas() { m_reset(RM_SOS); }
void main_resetdos() { if (conf.trdos_present) m_reset(RM_DOS); }
void main_resetcache() { if (conf.cache) m_reset(RM_CACHE); }
void main_reset() { m_reset((ROM_MODE)conf.reset_rom); }
//=============================================================================


//=============================================================================
void m_nmi(ROM_MODE page)
{
	if(conf.mem_model == MM_ATM710 && bankr[(cpu.pc >> 14) & 3] >= RAM_BASE_M+PAGE*MAX_RAM_PAGES)
	{		//NEDOREPO
		return;
	}
   set_mode(page);
   sprintf(statusline, "NMI to %s", getrom(page)); statcnt = 50;
   comp.p00 = 0; // quorum
   cpu.sp -= 2;
   if(cpu.DbgMemIf->rm(cpu.pc) == 0x76) // nmi on halt command
       cpu.pc++;
   cpu.DbgMemIf->wm(cpu.sp, cpu.pcl);
   cpu.DbgMemIf->wm(cpu.sp+1, cpu.pch);
   cpu.pc = 0x66; cpu.iff1 = cpu.halted = 0;
}
//=============================================================================


//=============================================================================
void main_nmi()
{
    nmi_pending  = 1;
	trdos_in_nmi = comp.flags&CF_TRDOS;	//NEDOREPO
    if(conf.mem_model != MM_ATM3)
        m_nmi(RM_NOCHANGE);
}
//=============================================================================


//=============================================================================
void main_nmidos()
{
 if((conf.mem_model == MM_PROFSCORP || conf.mem_model == MM_SCORP) &&
   !(comp.flags & CF_TRDOS) && cpu.pc < 0x4000)
 {
     nmi_pending = conf.frame * 50; // 50 * 20ms
     return;
 }
 m_nmi(RM_DOS);
}
//=============================================================================


//=============================================================================
void main_nmicache() { m_nmi(RM_CACHE); }
//=============================================================================


//=============================================================================
static void qsave(const char *fname) {
   char xx[0x200]; addpath(xx, fname);
   FILE *ff = fopen(xx, "wb");
   if (ff) {
       if(writeSNA(ff))
       {
           sprintf(statusline, "Quick save to %s", fname);
           statcnt = 30;
       }
      fclose(ff);
   }
}
//=============================================================================
void qsave1() { qsave("qsave1.sna"); }
void qsave2() { qsave("qsave2.sna"); }
void qsave3() { qsave("qsave3.sna"); }
//=============================================================================
static void qload(const char *fname) {
   char xx[0x200]; addpath(xx, fname);
   if(loadsnap(xx))
   {
       sprintf(statusline, "Quick load from %s", fname);
       statcnt = 30;
   }
}
//=============================================================================
void qload1() { qload("qsave1.sna"); }
void qload2() { qload("qsave2.sna"); }
void qload3() { qload("qsave3.sna"); }
//=============================================================================


//=============================================================================
void main_keystick()
{
    input.keymode = (input.keymode == K_INPUT::KM_KEYSTICK)  ? 	K_INPUT::KM_DEFAULT :
								K_INPUT::KM_KEYSTICK;
}
//=============================================================================


//=============================================================================
void main_autofire()
{
    conf.input.fire ^= 1;
    input.firedelay = 1;
    sprintf(statusline, "autofire %s", conf.input.fire ? "on" : "off");
    statcnt = 30;
}
//=============================================================================


//=============================================================================
void main_save()
{
   sound_stop();
   if (conf.cmos)
       save_nv();
   unsigned char optype = 0;
   for (int i = 0; i < 4; i++)
   {
      if (!comp.fdd[i].test())
          return;
      optype |= comp.fdd[i].optype;
   }

   if(!optype)
   {
       sprintf(statusline, "all saved");
       statcnt = 30;
   }
}
//=============================================================================


//=============================================================================
void main_fullscr()
{
    //-------------------------------------------------------------------------
    if (!(temp.rflags & (RF_GDI | RF_OVR | RF_CLIP | RF_D3D)))
    {
	sprintf( statusline, "only for overlay/gdi/nonexclusive modes");
	statcnt = 30;
    }
    //-------------------------------------------------------------------------
    else
    {
	conf.fullscr ^= 1;
	apply_video();
    }
    //-------------------------------------------------------------------------
}
//=============================================================================




//=============================================================================
// toggle mouse - по факту
void main_mouse()	
{
//	printf("mouse locked\n");	//срабатывает при клацание по экрану
					//с захватом мыши с первого клика
    conf.lockmouse ^= 1;
    adjust_mouse_cursor();
    sprintf( statusline, "mouse %slocked", conf.lockmouse ? nil : "un");
    statcnt = 30;
}
//=============================================================================
void main_unlock_mouse()						// [NS]
{
    //-------------------------------------------------------------------------
    //printf("mouse unlocked\n");	
    if (conf.lockmouse != 0)
    {
	conf.lockmouse = 0;
	adjust_mouse_cursor();
	sprintf( statusline, "mouse unlocked");
	statcnt = 30;
    }
    //-------------------------------------------------------------------------
}
//=============================================================================
void main_unlock_mouse_silent()						// [NS]
{
    //-------------------------------------------------------------------------
    //printf("mouse unlocked\n");	
    if (conf.lockmouse != 0)
    {
	conf.lockmouse = 0;
	//adjust_mouse_cursor();
    }
    //-------------------------------------------------------------------------
}
//=============================================================================




//=============================================================================
void main_help()
{
    showhelp();
}
//-------------------------------------------------------------------------
void mon_help()
{
    showhelp("monitor_keys"); 
}
//=============================================================================





//=============================================================================
// скопипащено с Петзольдов
intptr_t CALLBACK AboutDlgProc(	HWND hDlg,
				UINT iMsg,
				WPARAM wParam,
				LPARAM lParam
			)
{
    //-------------------------------------------------------------------------
    switch(iMsg)
    {
	//---------------------------------------------------------------------
	case WM_INITDIALOG:
	    return TRUE;
	//---------------------------------------------------------------------
	case WM_COMMAND :
	    //-----------------------------------------------------------------
	    switch( LOWORD( wParam))
	    {
		case IDOK:
		case IDCANCEL:
		EndDialog( hDlg, 0);
		return TRUE;
	    }
	    //-----------------------------------------------------------------
	    break;
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    return FALSE;
}
//=============================================================================




void main_about()
{


/*
    char temp_string_1[128];
    sprintf( temp_string_1, "Unreal_NS %s, %s",
				UNREAL_NS_VERS_STRING,
				__DATE__		);
 
    char temp_string_2[128];
    sprintf( temp_string_1, "Based on UnrealSpeccy %s by DeathSoft merged with %s NedoPC repo\n",
				VERS_STRING_, 
				NEDOREPO_VERS		);
    char temp_string[256];
    sprintf( temp_string,
		"%s\n\r\n\r%s",
			temp_string_1,
			temp_string_2					);

    MessageBox(	wnd,
		temp_string,
		"blablabla",//	
		MB_OK | MB_ICONINFORMATION );
*/

/*
    CreateDialog(	hIn,
		MAKEINTRESOURCE( IDD_ABOUT),
		wnd,
		AboutDlgProc
	);
*/

    DialogBox(	hIn,
		MAKEINTRESOURCE( IDD_ABOUT),
		wnd,
		AboutDlgProc
	);

}


//=============================================================================
void main_atmkbd()
{
    conf.atm.xt_kbd ^= 1;
    //-------------------------------------------------------------------------
    if (conf.atm.xt_kbd)
    {
	sprintf(statusline, "ATM mode on. emulator hotkeys disabled");
    }
    //-------------------------------------------------------------------------
    else
    {
	sprintf(statusline, "ATM mode off");
    }
    //-------------------------------------------------------------------------
    statcnt = 50;
}
//=============================================================================
void main_pastetext() { input.paste(); }
//=============================================================================


//=============================================================================
void wnd_resize( int scale)
{
    printf("wnd_resize()\n");

    // ресайз resize main windows сразу после старта
    // и после смены видео режима/ входе в дебагер

    //-------------------------------------------------------------------------
    if (conf.fullscr)
    {
	sprintf( statusline, "impossible in fullscreen mode");
	statcnt = 50;
	return;
    }
    //-------------------------------------------------------------------------
    if (!scale)
    {
	ShowWindow( wnd, SW_MAXIMIZE);
	return;
    }
   //-------------------------------------------------------------------------
    ShowWindow( wnd, SW_RESTORE);
    
    LONG style = GetWindowLong( wnd, GWL_STYLE);
    RECT rc =
    {
	0,
	0,
	(LONG(temp.ox) * scale),
	(LONG(temp.oy) * scale)
    };
    //-------------------------------------------------------------------------
    // printf("old cx %d\n",(rc.right - rc.left));
    // printf("old cy %d\n",(rc.bottom - rc.top));

    // AdjustWindowRect(&rc, DWORD(style), 0);  //orig

    // нужно так чтобы как то получить высоту меню и учесть ее
    // иначе hardware blitter режим не рисует ничего при старте
    // пока не будет увеличение высоты окна до размера экрана
    // при этом менюшка при маленьком экране может завернутсо в 2+? ряда !!!

    // вариант TSL-а, по моему, не спасал от менюшки в 2 строки
    AdjustWindowRect( &rc, style, GetMenu( wnd) != 0 );	// TSL
    
//	printf("new cx %d\n",(rc.right - rc.left));
//	printf("new cy %d\n",(rc.bottom - rc.top));
	
    //-------------------------------------------------------------------------
    // ресайзим как нибудь (через Ж (пока хз как правильно такое делоть))
    SetWindowPos(	wnd,
			nullptr,
			0,
			0,
			(rc.right - rc.left),
			(rc.bottom - rc.top),
			    SWP_NOCOPYBITS		|
			      SWP_NOMOVE 		|
				SWP_NOOWNERZORDER	|
				  SWP_NOZORDER
		);
    //потом лучше еще потыкать всякие get system metrics или около того
    
    //-------------------------------------------------------------------------	
    // и смотрим что мы фактически наресайзили
    RECT rct_Window;
    GetWindowRect( wnd, &rct_Window);	   
    int win_size_x = rct_Window.right - rct_Window.left;
    int win_size_y = rct_Window.bottom - rct_Window.top;
    
    RECT rct_Client;
    GetClientRect( wnd, &rct_Client);
    int clnt_size_x = rct_Client.right - rct_Client.left;
    int clnt_size_y = rct_Client.bottom - rct_Client.top;

    int gap_size_x = win_size_x - clnt_size_x;
    int gap_size_y = win_size_y - clnt_size_y;

    int new_size_x = gap_size_x + (LONG( temp.ox) * scale);
    int new_size_y = gap_size_y + (LONG (temp.oy) * scale);
	
    //printf("final cx %d\n",(new_size_x));
    //printf("final cy %d\n",(new_size_y));

    //-------------------------------------------------------------------------
    // ресайзим уже как нада
    // и походу НЕ иззо этого скачет размер окна в дебагере по Y
    //	мало того окно там ресайзитсо 2 раза!!!! о_О
    SetWindowPos(	wnd,
			nullptr,
			0,
			0,
			new_size_x,
			new_size_y,
			    SWP_NOCOPYBITS		|
			      SWP_NOMOVE 		|
				SWP_NOOWNERZORDER	|
				  SWP_NOZORDER
		);

    //-------------------------------------------------------------------------
    // подмена масштабов в контекстном меню на актуальныые 		[NS]
    char sys_x1[5]; char sys_x2[5]; char sys_x3[5]; char sys_x4[5];
    //-------------------------------------------------------------------------
    if (temp.rflags & RF_4X)
    {
	scale *= 4;	strcpy(sys_x1,"x4"); strcpy(sys_x2,"x8"); strcpy(sys_x3,"x12"); strcpy(sys_x4,"x16");
    }
    //-------------------------------------------------------------------------
    else if (temp.rflags & RF_3X)
    {
	scale *= 3;	strcpy(sys_x1,"x3"); strcpy(sys_x2,"x6"); strcpy(sys_x3,"x9"); strcpy(sys_x4,"x12");
    }
    //-------------------------------------------------------------------------
    else if (temp.rflags & RF_2X)
    {
	scale *= 2;	strcpy(sys_x1,"x2"); strcpy(sys_x2,"x4"); strcpy(sys_x3,"x6"); strcpy(sys_x4,"x8");
    }
    //-------------------------------------------------------------------------
    else if (temp.rflags & RF_1X)
    {
			strcpy(sys_x1,"x1"); strcpy(sys_x2,"x2"); strcpy(sys_x3,"x3"); strcpy(sys_x4,"x4");
    }
    //-------------------------------------------------------------------------
    HMENU sys = GetSystemMenu( wnd, FALSE);	//0);
    //-------------------------------------------------------------------------
    if (sys)
    {
	// доки на ModifyMenu нету! сделано по аналогии с InsertMenu !!!
//	ModifyMenu( sys, 2, MF_BYPOSITION | MF_STRING, SCU_SCALE1, sys_x1);
//	ModifyMenu( sys, 3, MF_BYPOSITION | MF_STRING, SCU_SCALE2, sys_x2);
//	ModifyMenu( sys, 4, MF_BYPOSITION | MF_STRING, SCU_SCALE3, sys_x3);
//	ModifyMenu( sys, 5, MF_BYPOSITION | MF_STRING, SCU_SCALE4, sys_x4);

	// так не нужно знать положение
	ModifyMenu( sys, SCU_SCALE1, MF_BYCOMMAND | MF_STRING, SCU_SCALE1, sys_x1);
	ModifyMenu( sys, SCU_SCALE2, MF_BYCOMMAND | MF_STRING, SCU_SCALE2, sys_x2);
	ModifyMenu( sys, SCU_SCALE3, MF_BYCOMMAND | MF_STRING, SCU_SCALE3, sys_x3);
	ModifyMenu( sys, SCU_SCALE4, MF_BYCOMMAND | MF_STRING, SCU_SCALE4, sys_x4);
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
    }
    //-------------------------------------------------------------------------
    // печатаем фактический масштаб
    sprintf(	statusline,
		"scale: %dx",
		scale
	    );
    statcnt = 50;
}
//=============================================================================
void main_size1() { wnd_resize(1); }
void main_size2() { wnd_resize(2); }
void main_sizem() { wnd_resize(0); }
//=============================================================================


//=============================================================================
static void SetBorderSize(unsigned BorderSize)
{
// 0 - none
// 1 - small
// 2 - wide
// 3 - full
    //-------------------------------------------------------------------------
    if (BorderSize > 3)
    {
	return;
    }
    //-------------------------------------------------------------------------
    conf.bordersize = u8(BorderSize);
    apply_video();
}
//=============================================================================
void main_border_none()  { SetBorderSize(0); }
void main_border_small() { SetBorderSize(1); }
void main_border_full()  { SetBorderSize(2); }	//wide на самом деле
//=============================================================================


//=============================================================================
void correct_exit()
{
    sound_stop();
   
    //-------------------------------------------------------------------------
    if (conf.wiznet)		//NEDOREPO
	Wiz5300_Close(); 	
    //-------------------------------------------------------------------------
    if (!done_fdd(true))
	return;
    //-------------------------------------------------------------------------
    nowait = 1;
    normal_exit = true;
    exit();
}
//=============================================================================


//=============================================================================
void opensnap()
{
    OnEnterGui();
    opensnap(0);
    eat();
    OnExitGui();
}
//=============================================================================


//=============================================================================
void savesnap()
{
    OnEnterGui();
    savesnap(-1);
    eat();
    OnExitGui();
}
//=============================================================================
