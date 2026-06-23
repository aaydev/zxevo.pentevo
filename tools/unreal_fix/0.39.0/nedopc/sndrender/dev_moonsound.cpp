// From https://github.com/tslabs/zx-evo/blob/master/pentevo/unreal/Unreal/sound/dev_moonsound.cpp

#include "../std.h"
#include "../emul.h"
#include "../vars.h"
#include "../util.h"
#include "dev_moonsound.h"


#define MASTER_CLOCK 33868800


inline EmuTime SystemTime()
{
	return cpu.t + comp.frame_counter * conf.frame;
}

ZXMMoonSound_priv::ZXMMoonSound_priv() : ymf262(0, 0), ymf278(0, 4096/*ram*/, 2048/*rom*/, 0)
{
	EmuTime systemTime = 0;

//	ymf262 = new YMF262(0, systemTime);
	ymf262.setSampleRate(44100, 1);
	ymf262.setVolume(32767 * 2 / 10);

//	ymf278 = new YMF278(0, 4096, 2048*1024, systemTime);
	ymf278.setSampleRate(44100, 1);
	ymf278.setVolume(32767 * 2 / 10);
}

ZXMMoonSound_priv::~ZXMMoonSound_priv()
{
//	delete ymf262;
//	delete ymf278;
}

/*  */
ZXMMoonSound::ZXMMoonSound() :
	system_clock_rate( 0 )
{
//	d = new ZXMMoonSound_priv();
	reset();
}

int ZXMMoonSound::load_rom(char *path)
{
	u8 * moon_rom;

	moon_rom = d.ymf278.getRom();

	if( !moon_rom )
		return -1;

	FILE *fp = fopen( path, "rb" );
	if ( !fp )
	{
		return -2;
	}

	if( 1 != fread( moon_rom, d.ymf278.getRomSize(), 1, fp ) )
	{
		fclose(fp);
		return -3;
	}

	fclose( fp );

	return 0;
}

void ZXMMoonSound::reset()
{
	unsigned tStatesPerSecond = conf.frame * conf.intfq;
	EmuTime systemTime = SystemTime();

	d.ymf262.reset( systemTime, tStatesPerSecond );
	d.ymf278.reset( systemTime );
}

bool ZXMMoonSound::wr_opl3( u8 port, u8 val )
{
	EmuTime systemTime = SystemTime();

	if ( !(port & 0xFC) )
	{
		switch (port & 0x03) {
		case 0:
			d.opl3latch = val;
			break;
		case 2: // select register bank 1
			d.opl3latch = val | 0x100;
			break;
		case 1:
		case 3: // write fm register
			d.ymf262.writeReg(d.opl3latch, val, systemTime);
			break;
		}

		return true;
	}

	return false;
}

bool ZXMMoonSound::wr_opl4( u8 port, u8 val )
{
	EmuTime systemTime = SystemTime();

	if ( !(port & 0xFE) )
	{
		switch (port & 0x01) {
		case 0: // select register
			d.opl4latch = val;
			break;
		case 1:
  			d.ymf278.writeRegOPL4(d.opl4latch, val, systemTime);
			break;
		}

		return true;
	}

	return false;
}

bool ZXMMoonSound::rd_opl3( u8 port, u8 &val )
{
	EmuTime systemTime = SystemTime();

	if( !(port & 0xFC) )
	{
		switch (port & 0x03) {
		case 0: // read status
		case 2:
			val = d.ymf262.readStatus(systemTime) | d.ymf278.readStatus(systemTime);
			break;
		case 1:
		case 3: // read fm register
			val = d.ymf262.readReg(d.opl3latch);
			break;
		}

		return true;
	}

	return false;
}

bool ZXMMoonSound::rd_opl4( u8 port, u8 &val )
{
	EmuTime systemTime = SystemTime();

	if ( !(port & 0xFE) )
	{
		switch (port & 0x01) {
		case 1: // read wave register
			val = d.ymf278.readRegOPL4(d.opl4latch, systemTime);
			break;
		}

		return true;
	}

	return false;
}


void ZXMMoonSound::set_timings(unsigned system_clock_rate, unsigned chip_clock_rate, unsigned sample_rate)
{
	d.ymf262.setSampleRate( sample_rate, 1 );
	d.ymf262.setVolume((u16)(1 * (conf.sound.moonsound_vol / 8192.0))); // doesn't work
	d.ymf278.setSampleRate( sample_rate, 1 );
	d.ymf278.setVolume((u16)(2000 * (conf.sound.moonsound_vol / 8192.0)));
	
	chip_clock_rate = sample_rate;

	ZXMMoonSound::system_clock_rate = system_clock_rate;
	ZXMMoonSound::chip_clock_rate = chip_clock_rate;

	SNDRENDER::set_timings(chip_clock_rate, sample_rate);
	passed_chip_ticks = passed_clk_ticks = 0;
	t = 0;
}

void ZXMMoonSound::start_frame(bufptr_t dst)
{
    SNDRENDER::start_frame(dst);
}

unsigned ZXMMoonSound::end_frame(unsigned clk_ticks)
{
	u64 end_chip_tick = ((passed_clk_ticks + clk_ticks) * chip_clock_rate) / system_clock_rate;

	flush((unsigned)(end_chip_tick - passed_chip_ticks));

    unsigned Val = SNDRENDER::end_frame(t);
	passed_clk_ticks += clk_ticks;
    passed_chip_ticks += t;
    t = 0;

    return Val;
}

void ZXMMoonSound::flush(unsigned chiptick)
{
	while (t < chiptick)
    {
		int buffer[2] = { 0, 0 };
		int *buf;

		t++;

		buf = d.ymf262.updateBuffer(1);
		if ( buf )
		{
			buffer[0] += buf[0] / 10;
			buffer[1] += buf[1] / 10;
		}

		buf = d.ymf278.updateBuffer(1);
		if ( buf )
		{
			buffer[0] += buf[0];
			buffer[1] += buf[1];
		}
		
		SNDRENDER::update( t, buffer[0], buffer[1] );
	}
}

ZXMMoonSound zxmmoonsound;
