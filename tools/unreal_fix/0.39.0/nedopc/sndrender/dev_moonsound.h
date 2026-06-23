// From https://github.com/tslabs/zx-evo/blob/master/pentevo/unreal/Unreal/sound/dev_moonsound.h

#pragma once

#ifndef _SOUND_DEV_MOONSOUND_H
#define _SOUND_DEV_MOONSOUND_H

#include "../sysdefs.h"
#include "ymf262.h"
#include "ymf278.h"

struct ZXMMoonSound_priv
{
friend class ZXMMoonSound;

public:
	ZXMMoonSound_priv();
	~ZXMMoonSound_priv();

private:
	YMF262 ymf262;
	int opl3latch;

	YMF278 ymf278;
	int opl4latch;
};

class ZXMMoonSound : public SNDRENDER
{
public:
	ZXMMoonSound();

	int load_rom(char *path);

	void reset();
	bool wr_opl3( u8 port, u8  val );
	bool wr_opl4( u8 port, u8  val );
	bool rd_opl3( u8 port, u8 &val );
	bool rd_opl4( u8 port, u8 &val );

	// set of functions that fills buffer in emulation progress
	void set_timings(unsigned system_clock_rate, unsigned chip_clock_rate, unsigned sample_rate);
	void start_frame() { SNDRENDER::start_frame(); }
	void start_frame(bufptr_t dst);
	unsigned end_frame(unsigned clk_ticks);
	void flush(unsigned chiptick);

private:
	ZXMMoonSound_priv d;

	unsigned chip_clock_rate;
	unsigned system_clock_rate;
	u64 passed_chip_ticks;
	u64 passed_clk_ticks;

	unsigned t;
};

extern ZXMMoonSound zxmmoonsound;

#endif /* _SOUND_DEV_MOONSOUND_H */
