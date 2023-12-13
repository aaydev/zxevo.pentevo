#include "std.h"

#include "emul.h"
#include "vars.h"
#include "sndrender/sndcounter.h"
#include "sound.h"
#include "draw.h"
#include "dx.h"
#include "dxr_rsm.h"
#include "leds.h"
#include "memory.h"
#include "snapshot.h"
#include "emulkeys.h"
#include "vs1001.h"
#include "z80.h"
#include "mainloop.h"
#include "funcs.h"
#include "zxevo.h"

#include "util.h"

//=============================================================================
void spectrum_frame()
{
//-----------------------------------------------------------------------------
    if (!temp.inputblock || input.keymode != K_INPUT::KM_DEFAULT)
	input.make_matrix();
	
//-----------------------------------------------------------------------------
	    		//if (main_pause_flag == 0)
			//{
	init_snd_frame();
			//}
	  		//if (main_pause_flag == 0)
			//{
	init_frame();
			//}
	    		if (main_pause_flag == 0)
			{
//-----------------------------------------------------------------------------
	// init readfont position to simulate correct font reading for zxevo -- lvd
	zxevo_set_readfont_pos(); 	//NEDOREPO
//-----------------------------------------------------------------------------
    if (cpu.dbgchk)
    {
		//перекрывает сообщения от fast skip mode !!!!
		//strcpy(statusline, "CPU debug mode");
	cpu.SetDbgMemIf();
	z80dbg::z80loop();
    }
    else
    {
		//strcpy(statusline, "CPU fast mode");
	cpu.SetFastMemIf();
	z80fast::z80loop();
    }
    
//-----------------------------------------------------------------------------
    if (modem.open_port)
	modem.io();
//-----------------------------------------------------------------------------
    flush_snd_frame();
    flush_frame();
    showleds();
//-----------------------------------------------------------------------------
    if (!cpu.iff1 || // int disabled in CPU
//	// int disabled by ATM hardware
//	((conf.mem_model == MM_ATM710 || conf.mem_model == MM_ATM3) && !(comp.pFF77 & 0x20))) 
	// int disabled by ATM hardware -- lvd removed int disabling in pentevo (atm3)
	((conf.mem_model == MM_ATM710/* || conf.mem_model == MM_ATM3*/) && !(comp.pFF77 & 0x20))) 	//NEDOREPO
    {
	unsigned char *mp = am_r(cpu.pc);
	if (cpu.halted)
	{
		strcpy(statusline, "CPU HALTED");
		statcnt = 10;
	}
	if (*(unsigned short*)mp == WORD2(0x18,0xFE) ||
	((*mp == 0xC3) && *(unsigned short*)(mp+1) == (unsigned short)cpu.pc))
	{
		strcpy(statusline, "CPU STOPPED");
		statcnt = 10;
	}
    }
//-----------------------------------------------------------------------------

   comp.t_states += conf.frame;
   cpu.t -= conf.frame;
   cpu.eipos -= conf.frame;
   comp.frame_counter++;
			}
			else
			{
				debug_events(&cpu);
				flush_snd_frame();	//помогло заглушить звук
			}

}
//=============================================================================


//=============================================================================
static void do_idle()
{
   if(conf.SyncMode != SM_TSC)
       return;

   static unsigned long long last_cpu = rdtsc();
 
   unsigned long long cpu = rdtsc();
   if (conf.sleepidle && ((cpu-last_cpu) < temp.ticks_frame))
   {
       ULONG Delay = ULONG(((temp.ticks_frame - (cpu-last_cpu)) * 1000ULL) / temp.cpufq);

       if (Delay > 5)
       {
           Sleep(Delay-1);
       }
   }

   for (cpu = rdtsc(); (cpu-last_cpu) < temp.ticks_frame; cpu = rdtsc())
   {
      asm_pause();
   }
   last_cpu = rdtsc();
}
//=============================================================================


// version before frame resampler
//uncommented by Alone Coder
/*
void mainloop()
{
   unsigned char skipped = 0;
   for (;;)
   {
      if (skipped < temp.frameskip)
      {
          skipped++;
          temp.vidblock = 1;
      }
      else
          skipped = temp.vidblock = 0;

      temp.sndblock = !conf.sound.enabled;
      temp.inputblock = 0; //temp.vidblock; //Alone Coder
      spectrum_frame();

      // message handling before flip (they paint to rbuf)
      if (!temp.inputblock)
          dispatch(conf.atm.xt_kbd ? ac_main_xt : ac_main);
      if (!temp.vidblock)
          flip();
      if (!temp.sndblock)
      {
          do_sound();
          Vs1001.Play();

//          if(conf.sound.do_sound != do_sound_ds)
          do_idle();
      }
   }
}
*/


//=============================================================================
void mainloop( const volatile bool &Exit)
{
    // printf("%s\n", __FUNCTION__);
    unsigned char skipped = 0;
    //-------------------------------------------------------------------------
    for (    ; !Exit   ;    )
    {
			// QueryPerformanceCounter
			// 3579545 попугаев в секунду есть (такты проца не влияют!!)
			// проверка на статичных 800 MHz
			// 20.357367 попугаев только сама мерялка без ничего
			// 	весь цикл 50Гц !!!!
			//	значит где то задеркжа\синхронизация!!!!
	//---------------------------------------------------------------------
	extern int main_pause_flag;
	//---------------------------------------------------------------------
	if (skipped < temp.frameskip)
	{
	    skipped++;
	    temp.vidblock = 1;
	}
	//---------------------------------------------------------------------
	else
	{
	    skipped = temp.vidblock = 0;
	}
	//---------------------------------------------------------------------
	if (!temp.vidblock)	// без этого не обновляетсо экран
	{	
	    flip();	// 7203.7550611 (128)
			// 6769.135877 (atarin)
			// 7176.8725490 (break 0)
			//	2429.617333 (2,5GHz)
	}
	//---------------------------------------------------------------------
	for ( unsigned f = rsm.needframes[ rsm.frame];    f;    f--   )
	{
	    //comp.fddIO2Ram_wr_disable = false;	//ЗАКОМЕНЧЕНО В NEDOREPO
	    temp.sndblock = !conf.sound.enabled;
	    temp.inputblock = temp.vidblock && conf.sound.enabled;
	    //if (main_pause_flag == 0)	//так невойти в дебагер
	    //{
	    spectrum_frame();	// 19073.4151470 (128)	
				// 17742.097473 (atarin)
				// 20623.20489860 (break 0)    
				//	6007.869088 (2,5GHz)	
	    //}
	    //VideoSaver();
	    //----------------------------------------------------------------- 
	    if (videosaver_state)
		savevideo_gfx();
	    //-----------------------------------------------------------------
	    // message handling before flip (they paint to rbuf)
	    if (!temp.inputblock)
	    {		
		// без этого окко вообще не отвечает !!!!
		//printf("KYB ");
		dispatch( conf.atm.xt_kbd   ?   ac_main_xt :		// 45 -- 999+
						ac_main);		// 70.921015710 (128)
									// 74.549958740 (atarin)
									// 45 (debug)					
	    }
	    //-----------------------------------------------------------------
	    if (!temp.sndblock)
	    {
		//-------------------------------------------------------------
		if (videosaver_state)
			savevideo_snd();
		//-------------------------------------------------------------
		do_sound();	// 41905.864632 (128)		
				// 43458.824821 (atarin)
				// 39506.222415 (debug 0)
				//	61697.805217 (2,5GHz)
			
		Vs1001.Play();		// 22.923798 (atarin)
					// 22.249513 (debug 0)		
	    }
	    //-----------------------------------------------------------------
	    // просто так не вызываетсо (только когда frame resampler?)
	    if (rsm.mix_frames > 1)
	    {
		memcpy( rbuf_s + rsm.rbuf_dst * rb2_offs, rbuf, temp.scx * temp.scy / 4);
		//-------------------------------------------------------------
		if (++rsm.rbuf_dst == rsm.mix_frames)
		    rsm.rbuf_dst = 0;
		//-------------------------------------------------------------
	    }
	    //-----------------------------------------------------------------
	    if (!temp.sndblock)
	    {
		do_idle();		// 20.708498 (128)
					// 21.071146 (break)
	    }
	    //-----------------------------------------------------------------
	}
	//---------------------------------------------------------------------
	if (++rsm.frame == rsm.period)
	    rsm.frame = 0;
	//---------------------------------------------------------------------
    }	//for (    ; !Exit   ;    )
    //-------------------------------------------------------------------------
    correct_exit();
}
//=============================================================================
