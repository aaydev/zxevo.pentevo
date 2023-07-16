#==============================================================================
# Unreal.exe
#==============================================================================

UNREAL_FLAGS =	-DUSE_SND_EXTERNAL_BUFFER

#==============================================================================

OPT =		-Ofast
OPT_1 =		-O1

OPT_0 =		-O0

#------------------------------------------------------------------------------
# NATIVE
#------------------------------------------------------------------------------
UNREAL_EXE =	Unreal.exe
TARGET =	-m32		\
		-march=native	\
		-mtune=native	\
		-mfpmath=both	\
		-DMOD_SSE2
TARGET_MIN =	-m32		\
		-march=native	\
		-mtune=native	\
		-mfpmath=both	\
		-DMOD_SSE2
LINKER_FLAGS =	-m32
RES_TARGET =	--target=pe-i386
#------------------------------------------------------------------------------
# NATIVE x64
#------------------------------------------------------------------------------
#UNREAL_EXE =	Unreal_x64.exe
#TARGET =	-m64		\
#		-march=native	\
#		-mtune=native	\
#		-mfpmath=both	\
#		-DMOD_SSE2
#TARGET_MIN =	-m64		\
#		-march=native	\
#		-mtune=native	\
#		-mfpmath=both	\
#		-DMOD_SSE2
#LINKER_FLAGS =	-m64
#RES_TARGET =	--target=pe-x86-64
#------------------------------------------------------------------------------
# P4
#------------------------------------------------------------------------------
#UNREAL_EXE =	Unreal-p4.exe
#TARGET =	-m32		\
#		-march=pentium4	\
#		-mtune=pentium4	\
#		-mfpmath=both	\
#		-DMOD_SSE2
#TARGET_MIN =	-m32		\
#		-march=pentium4	\
#		-mtune=pentium4	\
#		-mfpmath=both	\
#		-DMOD_SSE2
#LINKER_FLAGS =	-m32
#RES_TARGET =	--target=pe-i386
#------------------------------------------------------------------------------
# P0 - ultra minimal
#------------------------------------------------------------------------------
#UNREAL_EXE =	Unreal-p0.exe
#TARGET =	-m32		\
#		-march=i386	\
#		-mtune=i386	\
#		-mfpmath=387
#TARGET_MIN =	-m32		\
#		-march=i386	\
#		-mtune=i386	\
#		-mfpmath=387	\
#		-mmmx
#LINKER_FLAGS =	-m32
#RES_TARGET =	--target=pe-i386
#------------------------------------------------------------------------------




#windres targets
#pe-x86-64
#pei-x86-64
#pe-bigobj-x86-64
#elf64-x86-64
#elf64-l1om
#elf64-k1om
#pe-i386
#pei-i386
#elf32-i386
#elf32-iamcu
#elf64-little
#elf64-big
#elf32-little
#elf32-big
#pe-bigobj-i386
#srec
#symbolsrec
#verilog
#tekhex
#binary
#ihex
#plugin



#==============================================================================




#==============================================================================
OBJ_LIST =	emul.o			\
		std.o			\
		atm.o			\
		cheat.o			\
		config.o		\
		dbgbpx.o		\
		dbgcmd.o		\
		dbglabls.o		\
		dbgmem.o		\
		dbgoth.o		\
		dbgpaint.o		\
		dbgreg.o		\
		dbgrwdlg.o		\
		dbgtrace.o		\
		debug.o			\
		draw.o			\
		drawnomc.o		\
		draw_384.o		\
		dx.o			\
		dxerr.o			\
		dxovr.o			\
		dxrcopy.o		\
		dxrend.o		\
		dxrendch.o		\
		dxrframe.o		\
		dxr_4bpp.o		\
		dxr_512.o		\
		dxr_advm.o		\
		dxr_atm.o		\
		dxr_atm0.o		\
		dxr_atm2.o		\
		dxr_atm4.o		\
		dxr_atm6.o		\
		dxr_atm7.o		\
		profi.o			\
		dxr_atmf.o		\
		dxr_prof.o		\
		dxr_rsm.o		\
		dxr_text.o		\
		dxr_vd.o		\
		emulkeys.o		\
		fntsrch.o		\
		font.o			\
		font14.o		\
		font16.o		\
		font8.o			\
		fontatm2.o		\
		fontdata.o		\
		gs.o			\
		gshlbass.o		\
		gshle.o			\
		gsz80.o			\
		gui.o			\
		hdd.o			\
		hddio.o			\
		iehelp.o		\
		init.o			\
		input.o			\
		inputpc.o		\
		io.o			\
		keydefs.o		\
		leds.o			\
		mainloop.o		\
		memory.o		\
		modem.o			\
		opendlg.o		\
		savesnd.o		\
		sdcard.o		\
		snapshot.o		\
		snd_bass.o		\
		sound.o			\
		sshot_png.o		\
		tape.o			\
		util.o			\
		vars.o			\
		vs1001.o		\
		wd93cmd.o		\
		wd93crc.o		\
		fdd.o			\
		upd765.o		\
		wd93trk.o		\
		wldr_fdi.o		\
		wldr_isd.o		\
		wldr_pro.o		\
		wldr_td0.o		\
		wldr_trd.o		\
		wldr_udi.o		\
		wldr_dsk.o		\
		wldr_ipf.o		\
		z80.o			\
		z80asm.o		\
		zc.o			\
		savevid.o		\
					\
		zxusbnet.o		\
		zxevo.o			\
		dxr_pentatm7.o		\
					\
		debug_tools.o		\
		midi.o			\
					\
		settings.o
#==============================================================================




#==============================================================================
LIB_LIST =	-lcomctl32		\
		-lcomdlg32		\
		-luser32		\
		-lkernel32		\
		-lgdi32			\
		-lwinmm			\
		-ldxguid		\
		-lddraw			\
		-ldsound		\
		-ldinput		\
		-ldxerr9		\
		-lsetupapi		\
		-lgcc			\
		-lstdc++		\
					\
		-lws2_32		\
		-lIPHLPAPI
#==============================================================================



#==============================================================================
Unreal.exe:	$(OBJ_LIST)

	g++	$(LINKER_FLAGS)		\
		$(OBJ_LIST)		\
					\
	sndrender/snd.a			\
	z80/z80.a			\
					\
	-o $(UNREAL_EXE)		\
		$(LIB_LIST)		\
					\
	-s -static

#==============================================================================
	
	
	
	
#==============================================================================	
emul.o:		emul.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			mods.h						\
			resource.h					\
			sndrender/emul_2203.h				\
			sndrender/sndrender.h				\
				sndrender/sndbuffer.h			\
				sysdefs.h				\
									\
			emul.h						\
									\
			sndrender/sndchip.h				\
				sysdefs.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
			sndrender/sndcounter.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
			init.h						\
			funcs.h						\
			debug.h						\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			dx.h						\
			draw.h						\
			mainloop.h					\
			iehelp.h					\
			util.h						\
			version.h

	g++ emul.cpp		-o emul.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)
	
#==============================================================================
std.o:		std.cpp				\
			std.h			\
				sdk/ddraw.h	\
				sdk/dinput.h	\
				sdk/dsound.h	\
				sdk/ddk.h	\
				mods.h		\

	g++ std.cpp		-o std.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================	
atm.o:		atm.cpp							\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			memory.h					\
			draw.h						\
			atm.h						\
									\
			funcs.h

	g++ atm.cpp		-o atm.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)
	
#==============================================================================
cheat.o:	cheat.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			resource.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			gui.h						\
			dx.h						\
			util.h						\
			cheat.h

	g++ cheat.cpp		-o cheat.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)
	
#==============================================================================
config.o:	config.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			memory.h					\
			debug.h						\
			dbglabls.h					\
			draw.h						\
			dx.h						\
			fontatm2.h					\
			snapshot.h					\
			sndrender/sndcounter.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
			sound.h						\
			sdcard.h					\
			gs.h						\
			zc.h						\
			util.h						\
			init.h						\
			config.h					\
									\
			zxusbnet.h

	g++ config.cpp		-o config.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)
					
#==============================================================================
dbgbpx.o:	dbgbpx.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			resource.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			debug.h						\
			dbgbpx.h					\
			dbgcmd.h					\
			config.h					\
			util.h						\
									\
			emul_no_modal_windows.h				\
			dx.h						\
			draw.h

	g++ dbgbpx.cpp		-o dbgbpx.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dbgcmd.o:	dbgcmd.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dx.h						\
			debug.h						\
			dbgpaint.h					\
			dbgmem.h					\
			dbgrwdlg.h					\
			dbgcmd.h					\
			memory.h					\
			gui.h						\
			util.h

	g++ dbgcmd.cpp		-o dbgcmd.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)
					
#==============================================================================
dbglabls.o:	dbglabls.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			resource.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			debug.h						\
			dbgpaint.h					\
			dbglabls.h					\
			memory.h					\
			config.h					\
			util.h

	g++ dbglabls.cpp	-o dbglabls.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dbgmem.o:	dbgmem.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			debug.h						\
			dbgpaint.h					\
			dbgcmd.h					\
			dbgmem.h					\
			wd93crc.h					\
			util.h
			
	g++ dbgmem.cpp		-o dbgmem.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dbgoth.o:	dbgoth.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			resource.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			debug.h						\
			dbgpaint.h					\
			dbgoth.h					\
			gui.h						\
			fdd.h						\
			util.h

	g++ dbgoth.cpp		-o dbgoth.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dbgpaint.o:	dbgpaint.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			debug.h						\
			dbgpaint.h					\
			dx.h						\
			draw.h						\
			dxrframe.h					\
			font16.h					\
			util.h						\
									\
			debug_tools.h

	g++ dbgpaint.cpp	-o dbgpaint.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dbgreg.o:	dbgreg.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			debug.h						\
			dbgpaint.h					\
			dbgreg.h

	g++ dbgreg.cpp		-o dbgreg.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dbgrwdlg.o:	dbgrwdlg.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			debug.h						\
			dbgpaint.h					\
			dbgtrace.h					\
			dbgrwdlg.h					\
			fdd.h						\
			util.h

	g++ dbgrwdlg.cpp	-o dbgrwdlg.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dbgtrace.o:	dbgtrace.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			debug.h						\
			dbgtrace.h					\
			dbglabls.h					\
			dbgpaint.h					\
			dbgcmd.h					\
			memory.h					\
			z80asm.h					\
			z80/op_system.h					\
			util.h						\
									\
			gsz80.h

	g++ dbgtrace.cpp	-o dbgtrace.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
debug.o:	debug.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dx.h						\
			debug.h						\
			dbgpaint.h					\
			dbgreg.h					\
			dbgtrace.h					\
			dbgmem.h					\
			dbgoth.h					\
			dbglabls.h					\
			dbgbpx.h					\
			util.h						\
									\
			resource.h					\
			dbgrwdlg.h					\
			dbgcmd.h					\
			z80asm.h					\
			emulkeys.h

	g++ debug.cpp		-o debug.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
# warning: iteration 1 invokes undefined behavior [-Waggressive-loop-optimizations]
# при -O1+ вызывает не выход из цикла for (unsigned i = 0; i < 0x100; i++) по i = 0x100

draw.o:		draw.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			drawnomc.h					\
			dx.h						\
			dxr_text.h					\
			dxr_rsm.h					\
			dxr_advm.h					\
			memory.h					\
			config.h					\
			util.h

	g++ draw.cpp		-o draw.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)


#
# --param max-unroll-times=270
# -Waggressive-loop-optimizations
#  -fno-aggressive-loop-optimizations - работает
#==============================================================================
drawnomc.o:	drawnomc.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			drawnomc.h

	g++ drawnomc.cpp	-o drawnomc.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
draw_384.o:	draw_384.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			drawnomc.h

	g++ draw_384.cpp	-o draw_384.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dx.o:	dx.cpp								\
		std.h							\
			sdk/ddraw.h					\
			sdk/dinput.h					\
			sdk/dsound.h					\
			sdk/ddk.h					\
			mods.h						\
		resource.h						\
		emul.h							\
			sysdefs.h					\
			z80/defs.h					\
				sysdefs.h				\
			sndrender/sndrender.h				\
				sndrender/sndbuffer.h			\
				sysdefs.h				\
			savevid.h					\
			wd93.h						\
			fdd.h						\
			hddio.h						\
			hdd.h						\
			input.h						\
			modem.h						\
			bass.h						\
			snd_bass.h					\
			gshlbass.h					\
			gshle.h						\
		vars.h							\
			sndrender/emul_2203.h				\
			sndrender/sndchip.h				\
				sysdefs.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
			sndrender/saa1099.h				\
				sysdefs.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
		init.h							\
		dx.h							\
		draw.h							\
		dxrend.h						\
		dxrendch.h						\
		dxrframe.h						\
		dxr_text.h						\
		dxr_rsm.h						\
		dxr_advm.h						\
		dxovr.h							\
		dxerr.h							\
		sndrender/sndcounter.h					\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
		sound.h							\
		savesnd.h						\
		emulkeys.h						\
		funcs.h							\
		util.h							\
									\
		snapshot.h						\
		cheat.h							\
		dbgbpx.h						\
		dbgcmd.h						\
		debug.h							\
		dbgpaint.h						\
		dbglabls.h						\
		dbgoth.h						\
		tape.h							\
									\
		debug_tools.h
		
	g++ dx.cpp		-o dx.o					\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxerr.o:	dxerr.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			dxerr.h						\
			util.h						\
			dxerr9.h

	g++ dxerr.cpp		-o dxerr.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxovr.o:	dxovr.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			dx.h						\
			dxovr.h						\
			dxerr.h						\
			init.h

	g++ dxovr.cpp		-o dxovr.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)
		
#==============================================================================	
dxrcopy.o:	dxrcopy.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxrcopy.h

	g++ dxrcopy.cpp		-o dxrcopy.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)
		
#==============================================================================
dxrend.o:	dxrend.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			dxrend.h					\
			dxrcopy.h					\
			dxr_512.h					\
			dxr_4bpp.h					\
			dxr_prof.h					\
			dxr_atm.h					\
			draw.h						\
			util.h

	g++ dxrend.cpp		-o dxrend.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxrendch.o:	dxrendch.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dx.h						\
			dxrframe.h					\
			dxrendch.h

	g++ dxrendch.cpp	-o dxrendch.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxrframe.o:	dxrframe.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxrframe.h					\
			dxrcopy.h					\
			util.h

	g++ dxrframe.cpp	-o dxrframe.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_4bpp.o:	dxr_4bpp.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxrframe.h					\
			dxr_4bpp.h

	g++ dxr_4bpp.cpp	-o dxr_4bpp.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_512.o:	dxr_512.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxrframe.h					\
			dxr_512.h

	g++ dxr_512.cpp		-o dxr_512.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_advm.o:	dxr_advm.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxrend.h					\
			dxrcopy.h					\
			dxrframe.h					\
			dxr_advm.h

	g++ dxr_advm.cpp	-o dxr_advm.o				\
		-c -pipe $(OPT) $(TARGET_MIN) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_atm.o:	dxr_atm.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxrend.h					\
			dxr_atm.h					\
			dxr_atm0.h					\
			dxr_atm2.h					\
			dxr_atm4.h					\
			dxr_atm6.h					\
			dxr_atm7.h					\
			dxr_atmf.h					\
									\
			dxr_pentatm7.h

	g++ dxr_atm.cpp		-o dxr_atm.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_atm0.o:	dxr_atm0.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxr_atmf.h					\
			dxr_atm0.h

	g++ dxr_atm0.cpp	-o dxr_atm0.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)
						
#==============================================================================
dxr_atm2.o:	dxr_atm2.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxr_atmf.h					\
			dxr_atm2.h

	g++ dxr_atm2.cpp	-o dxr_atm2.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_atm4.o:	dxr_atm4.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxr_atmf.h					\
			dxr_atm4.h					\
			fontatm2.h

	g++ dxr_atm4.cpp	-o dxr_atm4.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_atm6.o:	dxr_atm6.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxr_atmf.h					\
			dxr_atm6.h					\
			fontatm2.h

	g++ dxr_atm6.cpp	-o dxr_atm6.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_atm7.o:	dxr_atm7.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxrframe.h					\
			dxr_atmf.h					\
			dxr_atm7.h					\
			fontatm2.h

	g++ dxr_atm7.cpp	-o dxr_atm7.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
profi.o:	profi.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			profi.h

	g++ profi.cpp		-o profi.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_atmf.o:	dxr_atmf.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxrframe.h					\
			dxrcopy.h					\
			dxr_atmf.h

	g++ dxr_atmf.cpp	-o dxr_atmf.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_prof.o:	dxr_prof.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxrframe.h					\
			dxrcopy.h					\
			dxr_prof.h

	g++ dxr_prof.cpp	-o dxr_prof.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_rsm.o:	dxr_rsm.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxr_rsm.h

	g++ dxr_rsm.cpp		-o dxr_rsm.o				\
		-c -pipe $(OPT) $(TARGET_MIN) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_text.o:	dxr_text.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxrframe.h					\
			fontdata.h					\
			font8.h						\
			font14.h					\
			font16.h					\
			dxr_text.h					\
			init.h						\
			util.h

	g++ dxr_text.cpp	-o dxr_text.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_vd.o:	dxr_vd.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h		

	g++ dxr_vd.cpp		-o dxr_vd.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
emulkeys.o:	emulkeys.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			resource.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			config.h					\
			draw.h						\
			dx.h						\
			debug.h						\
			memory.h					\
			sndrender/sndcounter.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
			sound.h						\
			savesnd.h					\
			tape.h						\
			gui.h						\
			leds.h						\
			snapshot.h					\
			fdd.h						\
			init.h						\
			z80.h						\
				z80_main.h				\
			emulkeys.h					\
			funcs.h						\
			util.h						\
									\
			input.h						\
			zxusbnet.h					\
									\
			Version.h

	g++ emulkeys.cpp	-o emulkeys.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
fntsrch.o:	fntsrch.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			resource.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			dxr_text.h					\
			fntsrch.h					\
			util.h

	g++ fntsrch.cpp		-o fntsrch.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
font.o:		font.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			font.h

	g++ font.cpp		-o font.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
font14.o:	font14.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			font14.h

	g++ font14.cpp		-o font14.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
font16.o:	font16.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			font16.h

	g++ font16.cpp		-o font16.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
font8.o:	font8.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			font8.h

	g++ font8.cpp		-o font8.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
fontatm2.o:	fontatm2.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			fontatm2.h

	g++ fontatm2.cpp	-o fontatm2.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
fontdata.o:	fontdata.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			fontdata.h

	g++ fontdata.cpp	-o fontdata.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
gs.o:		gs.cpp							\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			gs.h						\
			gsz80.h						\
			vs1001.h					\
				util.h

	g++ gs.cpp		-o gs.o					\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
gshlbass.o:	gshlbass.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			bass.h						\
			snd_bass.h					\
			gshle.h						\
			gs.h						\
			init.h						\
			util.h

	g++ gshlbass.cpp	-o gshlbass.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
gshle.o:	gshle.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			bass.h						\
			snd_bass.h					\
			gshle.h						\
			gs.h						\
			sndrender/sndcounter.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
			sound.h						\
			util.h

	g++ gshle.cpp		-o gshle.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
gsz80.o:	gsz80.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			gs.h						\
			gsz80.h						\
			vs1001.h					\
				util.h					\
			sdcard.h					\
			debug.h						\
			z80/op_noprefix.h				\
			z80/op_system.h					\
			gsz80.inl

	g++ gsz80.cpp		-o gsz80.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
gui.o:		gui.cpp							\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			resource.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			config.h					\
			draw.h						\
			dx.h						\
			dxrend.h					\
			dxr_advm.h					\
			dxr_rsm.h					\
			fntsrch.h					\
			tape.h						\
			snapshot.h					\
			leds.h						\
			sdcard.h					\
			zc.h						\
			gs.h						\
			gui.h						\
			util.h

	g++ gui.cpp		-o gui.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
hdd.o:		hdd.cpp							\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			util.h

	g++ hdd.cpp		-o hdd.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
hddio.o:	hddio.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			init.h						\
			hddio.h						\
			util.h
			

	g++ hddio.cpp		-o hddio.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
iehelp.o:	iehelp.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			iehelp.h					\
			util.h

	g++ iehelp.cpp		-o iehelp.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
#				png/pngconf.h				\
#					png/pngusr.h - нет файла	\
#					png/config.h - нет файла	\
#

init.o:		init.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			config.h					\
			dx.h						\
			draw.h						\
			iehelp.h					\
			gs.h						\
			leds.h						\
			tape.h						\
			emulkeys.h					\
			sshot_png.h					\
				png/zlib.h				\
					png/zconf.h			\
				png/pngconf.h				\
									\
									\
				png/png.h				\
					png/zlib.h			\
						png/zconf.h		\
					png/pngconf.h			\
									\
			init.h						\
			snapshot.h					\
			savesnd.h					\
			fdd.h						\
			z80/tables.h					\
			dbgbpx.h					\
			util.h						\
									\
			zxusbnet.h					\
									\
			midi.h						\
			emul_no_modal_windows.h

	g++ init.cpp		-o init.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
input.o:	input.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			dx.h						\
			tape.h						\
			atm.h						\
			memory.h					\
			input.h						\
			inputpc.h					\
			util.h

	g++ input.cpp		-o input.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
inputpc.o:	inputpc.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			inputpc.h

	g++ inputpc.cpp		-o inputpc.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
io.o:		io.cpp							\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			funcs.h						\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			memory.h					\
			atm.h						\
			profi.h						\
			sndrender/sndcounter.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
			sound.h						\
			gs.h						\
			sdcard.h					\
			zc.h						\
			tape.h						\
			upd765.h

	g++ io.cpp		-o io.o					\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
keydefs.o:	keydefs.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			emulkeys.h					\
			snapshot.h					\
			gui.h						\
			savesnd.h					\
			cheat.h						\
			dbgreg.h					\
			dbgcmd.h					\
			dbgoth.h					\
			dbgrwdlg.h					\
			dbgbpx.h					\
			dbgtrace.h					\
			dbglabls.h					\
			dbgmem.h					\
			funcs.h

	g++ keydefs.cpp		-o keydefs.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
leds.o:		leds.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			font.h						\
			font16.h					\
			gs.h						\
			tape.h						\
			draw.h						\
			debug.h						\
			dbgbpx.h					\
			memory.h					\
			leds.h						\
			util.h

	g++ leds.cpp		-o leds.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
#						warnings

mainloop.o:	mainloop.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			sndrender/sndcounter.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
			sound.h						\
			draw.h						\
			dx.h						\
			dxr_rsm.h					\
			leds.h						\
			memory.h					\
			snapshot.h					\
			emulkeys.h					\
			vs1001.h					\
				util.h					\
			z80.h						\
				z80_main.h				\
			mainloop.h					\
			funcs.h						\
			util.h						\
									\
			zxevo.h

	g++ mainloop.cpp	-o mainloop.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
memory.o:	memory.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			memory.h					\
			util.h

	g++ memory.cpp		-o memory.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
modem.o:	modem.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			util.h

	g++ modem.cpp		-o modem.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
opendlg.o:	opendlg.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			resource.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			gui.h						\
			opendlg.h					\
			util.h

	g++ opendlg.cpp		-o opendlg.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
savesnd.o:	savesnd.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			resource.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			dx.h						\
			memory.h					\
			savesnd.h					\
			util.h

	g++ savesnd.cpp		-o savesnd.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
sdcard.o:	sdcard.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			sdcard.h					\
			util.h

	g++ sdcard.cpp		-o sdcard.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
#				png/pngconf.h				\
#					png/pngusr.h - нет файла	\
#					png/config.h - нет файла	\
#

snapshot.o:	snapshot.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			resource.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			snapshot.h					\
			tape.h						\
			memory.h					\
			opendlg.h					\
			draw.h						\
			config.h					\
			z80.h						\
				z80_main.h				\
			sshot_png.h					\
				png/zlib.h				\
					png/zconf.h			\
				png/pngconf.h				\
									\
									\
				png/png.h				\
					png/zlib.h			\
						png/zconf.h		\
					png/pngconf.h			\
									\
			funcs.h						\
			util.h

	g++ snapshot.cpp	-o snapshot.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
snd_bass.o:	snd_bass.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			util.h	

	g++ snd_bass.cpp	-o snd_bass.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
sound.o:	sound.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			gs.h						\
			tape.h						\
			config.h					\
			sndrender/sndcounter.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
			sound.h

	g++ sound.cpp		-o sound.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
#				png/pngconf.h				\
#					png/pngusr.h - нет файла	\
#					png/config.h - нет файла	\
#

sshot_png.o:	sshot_png.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			sshot_png.h					\
				png/zlib.h				\
					png/zconf.h			\
				png/pngconf.h				\
									\
									\
				png/png.h				\
					png/zlib.h			\
						png/zconf.h		\
					png/pngconf.h			\
									

	g++ sshot_png.cpp	-o sshot_png.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
# зачем png/zlib.h ????
#
# error: unsupported size for integer register при -O2...-Ofast !!!!

tape.o:		tape.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			tape.h						\
			memory.h					\
			util.h						\
			png/zlib.h					\
				png/zconf.h

	g++ tape.cpp		-o tape.o				\
		-c -pipe $(OPT_1) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
util.o:		util.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			snapshot.h					\
			init.h						\
			util.h						\
									\
			emul_no_modal_windows.h

	g++ util.cpp		-o util.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
vars.o:		vars.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			debug.h						\
			memory.h					\
			gsz80.h						\
			z80.h						\
				z80_main.h				\
			util.h						\
			sndrender/sndcounter.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
			sound.h						\
			z80_main.h

	g++ vars.cpp		-o vars.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
vs1001.o:	vs1001.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			vs1001.h					\
				util.h					\
			bass.h						\
			snd_bass.h

	g++ vs1001.cpp		-o vs1001.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
wd93cmd.o:	wd93cmd.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			wd93crc.h					\
			util.h

	g++ wd93cmd.cpp		-o wd93cmd.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
wd93crc.o:	wd93crc.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			wd93crc.h

	g++ wd93crc.cpp		-o wd93crc.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
fdd.o:		fdd.cpp							\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			snapshot.h					\
			fdd.h						\
			util.h

	g++ fdd.cpp		-o fdd.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
upd765.o:	upd765.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			util.h						\
			upd765.h

	g++ upd765.cpp		-o upd765.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
wd93trk.o:	wd93trk.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			wd93crc.h					\
			util.h

	g++ wd93trk.cpp		-o wd93trk.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
wldr_fdi.o:	wldr_fdi.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			util.h						\
			wd93crc.h

	g++ wldr_fdi.cpp	-o wldr_fdi.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
wldr_isd.o:	wldr_isd.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			util.h

	g++ wldr_isd.cpp	-o wldr_isd.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
wldr_pro.o:	wldr_pro.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			util.h

	g++ wldr_pro.cpp	-o wldr_pro.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
wldr_td0.o:	wldr_td0.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			wd93crc.h					\
			util.h

	g++ wldr_td0.cpp	-o wldr_td0.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
wldr_trd.o:	wldr_trd.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			util.h

	g++ wldr_trd.cpp	-o wldr_trd.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
wldr_udi.o:	wldr_udi.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			wd93crc.h					\
			util.h

	g++ wldr_udi.cpp	-o wldr_udi.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
wldr_dsk.o:	wldr_dsk.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			fdd.h						\
			upd765.h

	g++ wldr_dsk.cpp	-o wldr_dsk.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
#					warnings

wldr_ipf.o:	wldr_ipf.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			fdd.h						\
			util.h

	g++ wldr_ipf.cpp	-o wldr_ipf.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
z80.o:		z80.cpp							\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			memory.h					\
			tape.h						\
			debug.h						\
			sndrender/sndcounter.h				\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
			sound.h						\
			atm.h						\
			gs.h						\
			emulkeys.h					\
			z80/op_system.h					\
			z80/op_noprefix.h				\
			fontatm2.h					\
			sdcard.h					\
			zc.h						\
			z80.h						\
				z80_main.h				\
			util.h						\
									\
			zxusbnet.h					\
									\
			midi.h						\
									\
			z80_main.inl
			
	g++ z80.cpp		-o z80.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
z80asm.o:	z80asm.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			memory.h					\
			dbglabls.h					\
			z80asm.h					\
			util.h

	g++ z80asm.cpp		-o z80asm.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
zc.o:		zc.cpp							\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			sdcard.h					\
			zc.h

	g++ zc.cpp		-o zc.o					\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
savevid.o:	savevid.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			util.h						\
			savevid.h

	g++ savevid.cpp		-o savevid.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================


#==============================================================================
zxusbnet.o:	zxusbnet.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			zxusbnet.h

	g++ zxusbnet.cpp	-o zxusbnet.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)
		
## -fpermissive нужон для x64
## ибо ошибке
#==============================================================================
zxevo.o:	zxevo.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			zxevo.h						\
			fontatm2.h

	g++ zxevo.cpp		-o zxevo.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
dxr_pentatm7.o:	dxr_pentatm7.cpp					\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
			draw.h						\
			dxr_atmf.h					\
			dxr_pentatm7.h					\
			fontatm2.h

	g++ dxr_pentatm7.cpp	-o dxr_pentatm7.o			\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================



#==============================================================================
debug_tools.o:	debug_tools.cpp						\
			std.h						\
				sdk/ddraw.h				\
				sdk/dinput.h				\
				sdk/dsound.h				\
				sdk/ddk.h				\
				mods.h					\
			emul.h						\
				sysdefs.h				\
				z80/defs.h				\
					sysdefs.h			\
				sndrender/sndrender.h			\
					sndrender/sndbuffer.h		\
					sysdefs.h			\
				savevid.h				\
				wd93.h					\
				fdd.h					\
				hddio.h					\
				hdd.h					\
				input.h					\
				modem.h					\
				bass.h					\
				snd_bass.h				\
				gshlbass.h				\
				gshle.h					\
			vars.h						\
				sndrender/emul_2203.h			\
				sndrender/sndchip.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
				sndrender/saa1099.h			\
					sysdefs.h			\
					sndrender/sndrender.h		\
						sndrender/sndbuffer.h	\
						sysdefs.h		\
									\
			debug_tools.h						\
			draw.h

	g++ debug_tools.cpp	-o debug_tools.o			\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
midi.o:		midi.cpp						\
			midi.h
	
	g++ midi.cpp		-o midi.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)
	
#==============================================================================




#==============================================================================
settings.o:	settings.rc						\
			resource.h					\
			afxres.h					\
				winres.h				\
			res/cur00000.cur				\
			res/cur00001.cur				\
			res/cur00002.cur				\
			res/cur00003.cur				\
			res/cur00004.cur				\
			res/cur00005.cur				\
			res/cur00006.cur				\
			res/cur00007.cur				\
			res/cur00008.cur				\
			res/cursor1.cur					\
			res/u1.ico
			
	windres	settings.rc	-o settings.o	\
		$(RES_TARGET)

#==============================================================================


#==============================================================================
#snd.a:
#
#	cd sndrender
#	make.exe
#	cd ..
#	pause
#==============================================================================
#z80.a:
#
#	cd z80
#	make.exe
#	cd ..
#	pause
#==============================================================================
	
	
	
	
	
	
	

#==============================================================================
# для sndrender/sndbuffer.h
# нужно включать -DUSE_SND_EXTERNAL_BUFFER в аргументах компилера!!!
# но без него вообще не компилитсо			
			
			