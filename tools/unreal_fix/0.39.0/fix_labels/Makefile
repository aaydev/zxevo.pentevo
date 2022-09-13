# AMD64 = 1
# LTO = 1

ifdef AMD64
PREFIX=x86_64
else
PREFIX=i686
endif

ifdef LTO
CXX_LTO=-flto
endif

TOOLPREFIX=$(PREFIX)-w64-mingw32


CXX_COMPILER=$(TOOLPREFIX)-g++

RC=$(TOOLPREFIX)-windres
CXX=$(CXX_COMPILER)
LNK=$(CXX_COMPILER)

CFLAGS_COMMON=-D_CRT_SECURE_NO_DEPRECATE -DUSE_SND_EXTERNAL_BUFFER -D_PREFIX_ -DMOD_SSE2 -std=c++14 -Wno-attributes 

ifdef AMD64
ARCH=-march=x86-64 -msse -msse2 -mno-mmx
else
ARCH=-march=pentium-m -msse -msse2 -mno-mmx
endif

CFLAGS_COMMON+=$(ARCH)

CFLAGS_RELEASE=-O3 -static $(CXX_LTO) -fomit-frame-pointer
LDFLAGS_DEBUG=-O3 $(CXX_LTO) -static $(ARCH)

CXXFLAGS=$(CFLAGS_COMMON) $(CFLAGS_DEBUG) $(CFLAGS_RELEASE)

LDFLAGS=$(LDFLAGS_DEBUG)

LIBS=-lws2_32 -lcomctl32 -lcomdlg32 -luser32 -lkernel32 -lgdi32 -lwinmm -ldxguid \
  -lddraw -ldsound -ldinput -ldxerr9 -lsetupapi -lgcc -lstdc++ -liphlpapi

# top-level sources
SRCS=emul.cpp std.cpp atm.cpp cheat.cpp config.cpp dbgbpx.cpp dbgcmd.cpp dbglabls.cpp \
     dbgmem.cpp dbgoth.cpp dbgpaint.cpp dbgreg.cpp dbgrwdlg.cpp dbgtrace.cpp \
     debug.cpp draw.cpp drawnomc.cpp draw_384.cpp dx.cpp dxerr.cpp dxovr.cpp \
     dxrcopy.cpp dxrend.cpp dxrendch.cpp dxrframe.cpp dxr_4bpp.cpp dxr_512.cpp \
     dxr_advm.cpp dxr_atm.cpp dxr_atm0.cpp dxr_atm2.cpp dxr_atm4.cpp dxr_atm6.cpp \
     dxr_atm7.cpp profi.cpp \
     dxr_atmf.cpp dxr_prof.cpp dxr_rsm.cpp dxr_text.cpp dxr_vd.cpp \
     emulkeys.cpp fntsrch.cpp font.cpp font14.cpp font16.cpp \
     font8.cpp fontatm2.cpp fontdata.cpp gs.cpp gshlbass.cpp gshle.cpp \
     gsz80.cpp gui.cpp hdd.cpp hddio.cpp iehelp.cpp init.cpp \
     input.cpp inputpc.cpp io.cpp keydefs.cpp leds.cpp mainloop.cpp \
     memory.cpp modem.cpp opendlg.cpp savesnd.cpp sdcard.cpp snapshot.cpp \
     snd_bass.cpp sound.cpp sshot_png.cpp tape.cpp util.cpp vars.cpp \
     vs1001.cpp wd93cmd.cpp wd93crc.cpp wd93trk.cpp fdd.cpp upd765.cpp \
     wldr_fdi.cpp wldr_isd.cpp wldr_pro.cpp wldr_td0.cpp wldr_trd.cpp wldr_udi.cpp \
     wldr_dsk.cpp wldr_ipf.cpp \
     z80.cpp z80asm.cpp zc.cpp savevid.cpp zxevo.cpp dxr_pentatm7.cpp zxusbnet.cpp 

# sndrender sources
SRCS+=sndrender/saa1099.cpp sndrender/sndbuffer.cpp sndrender/sndchip.cpp \
      sndrender/sndcounter.cpp sndrender/sndrender.cpp sndrender/emul_2203.cpp

# z80 sources
SRCS+=z80/daa_tabs.cpp z80/op_cb.cpp z80/op_dd.cpp z80/op_ddcb.cpp z80/op_ed.cpp \
      z80/op_fd.cpp z80/op_noprefix.cpp z80/tables.cpp

OBJS=$(SRCS:.cpp=.o)
NAMES=$(SRCS:.cpp=)

PCH=std.h.gch

EXE=x32/emul.exe




.PHONY: all clean
all: $(EXE)

-include $(NAMES:%=%.d)

ifeq (,$(findstring $(MAKECMDGOALS),clean))
%.d: %.cpp 
	$(CXX) -MM $(CXXFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@
endif

$(PCH): std.h
	$(CXX) $(CXXFLAGS) -x c++-header std.h

%.o: %.cpp Makefile $(PCH)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


%.res: %.rc
	$(RC) -o$@ -i$<

settings.o: settings.res
	$(RC) -o$@ -i$<

$(EXE): $(OBJS) settings.o Makefile
	$(LNK) $(LDFLAGS) -o$@ $(OBJS) settings.o $(LIBS)
	strip $(EXE)

clean:
	rm -f *.o *.res *.gch sndrender/*.o z80/*.o
	rm -f *.d sndrender/*.d z80/*.d

