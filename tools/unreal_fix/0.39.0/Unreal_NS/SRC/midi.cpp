
#include <stdio.h>
#include <stdint.h>	// uint8_t

#include <windows.h>	// некоторые компилеры не инклудят все файлы?
			// пробовать инклудить то шо нижо
//#include <stdarg.h>	// или можот это относитсо только к winbase.h
//#include <windef.h>
//#include <winbase.h>
//#include <mmsystem.h>


// Ущербное MIDI от NS
// выдрано из PCemV17Win который завалялсо на диске у NS-а
// тк интернетов нету
// а описаний как слать в мидю тожо нету

#include "midi.h"





//=============================================================================
// целеком притянутое из PCemV17Win

#define CFG_MACHINE 0	// ХЗ шо это
			// что то из PCem




/*
#include <windows.h>
#include <mmsystem.h>
#include "ibm.h"
#include "config.h"
#include "plat-midi.h"
*/



static int midi_id;
static HMIDIOUT midi_out_device = NULL;

//=============================================================================
void midi_close();
//=============================================================================
void midi_init()
{
    MMRESULT hr;
        
//  midi_id = config_get_int( CFG_MACHINE, NULL, "midi", 0);
    midi_id = NULL;	// тк нету config_get_int
			// можно предполоижть что это выбор миди девайса
			// и то что null возможно выберет дефолтный

    hr = midiOutOpen(	&midi_out_device,
			midi_id,
			0,
			0,
			CALLBACK_NULL
		      );
    //-------------------------------------------------------------------------
    if (hr != MMSYSERR_NOERROR) 
    {
	printf("midiOutOpen error - %08X\n", hr);
	midi_id = 0;
	hr = midiOutOpen(	&midi_out_device,
				midi_id, 
				0,
				0, 
				CALLBACK_NULL
			   );
	//---------------------------------------------------------------------
	if (hr != MMSYSERR_NOERROR) 
	{
	    printf("midiOutOpen error - %08X\n", hr);
	    return;
	}
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    midiOutReset( midi_out_device);
}
//=============================================================================
void midi_close()
{
    //-------------------------------------------------------------------------
    if (midi_out_device != NULL)
    {
	midiOutReset( midi_out_device);
	midiOutClose( midi_out_device);
	midi_out_device = NULL;
    }
    //-------------------------------------------------------------------------
}
//=============================================================================
int midi_get_num_devs()
{
    return midiOutGetNumDevs();
}
//=============================================================================
void midi_get_dev_name( int num, char *s)
{
    MIDIOUTCAPS caps;

    midiOutGetDevCaps( num, &caps, sizeof(caps));
    strcpy( s, caps.szPname);
}
//=============================================================================
static int midi_pos;
static int midi_len;
static uint32_t midi_command;
static int midi_lengths[8] = {3, 3, 3, 3, 2, 2, 3, 1};
static int midi_insysex;
static uint8_t midi_sysex_data[1024+2];
//=============================================================================
static void midi_send_sysex()
{
    MIDIHDR hdr;
        
    hdr.lpData = (LPSTR)midi_sysex_data;
    hdr.dwBufferLength = midi_pos;
    hdr.dwFlags = 0;
        
/*       
    pclog("Sending sysex : ");
    for (c = 0;    c < midi_pos;     c++)
	pclog("%02x ", midi_sysex_data[c]);
    pclog("\n");
*/
        
    midiOutPrepareHeader( midi_out_device, &hdr, sizeof( MIDIHDR));
    midiOutLongMsg( midi_out_device, &hdr, sizeof( MIDIHDR));
        
    midi_insysex = 0;
}
//=============================================================================
void midi_write( uint8_t val)
{
    //-------------------------------------------------------------------------
    if ((val & 0x80) && !(val == 0xF7 && midi_insysex))
    {
	midi_pos = 0;
	midi_len = midi_lengths[(val >> 4) & 7];
	midi_command = 0;
	//---------------------------------------------------------------------
	if (val == 0xF0)
	    midi_insysex = 1;
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
    if (midi_insysex)
    {
	midi_sysex_data[ midi_pos++] = val;
	//---------------------------------------------------------------------
	if ((val == 0xF7)   ||   (midi_pos >= 1024 + 2))
	    midi_send_sysex();
	//---------------------------------------------------------------------
	return;
    }
    //-------------------------------------------------------------------------                 
    if (midi_len)
    {                
	midi_command |= (val << (midi_pos * 8));
                
	midi_pos++;
	//---------------------------------------------------------------------
	if (midi_pos == midi_len)
	    midiOutShortMsg( midi_out_device, midi_command);
	//---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
}
//=============================================================================




// целеком притянутое из PCemV17Win
//=============================================================================
