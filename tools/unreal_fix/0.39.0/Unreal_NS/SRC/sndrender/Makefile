#==============================================================================
# snd.a
#==============================================================================

UNREAL_FLAGS =	-DUSE_SND_EXTERNAL_BUFFER

#==============================================================================

OPT =		-Ofast

#------------------------------------------------------------------------------
# NATIVE
#------------------------------------------------------------------------------
TARGET =	-m32		\
		-march=native	\
		-mtune=native	\
		-mfpmath=both	\
		-DMOD_SSE2
#------------------------------------------------------------------------------
# NATIVE x64
#------------------------------------------------------------------------------
#TARGET =	-m64		\
#		-march=native	\
#		-mtune=native	\
#		-mfpmath=both	\
#		-DMOD_SSE2
#------------------------------------------------------------------------------
# P4
#------------------------------------------------------------------------------
#TARGET =	-m32		\
#		-march=pentium4	\
#		-mtune=pentium4	\
#		-mfpmath=both	\
#		-DMOD_SSE2
#------------------------------------------------------------------------------
# P0 - ultra minimal
#------------------------------------------------------------------------------
#TARGET =	-m32		\
#		-march=i386	\
#		-mtune=i386	\
#		-mfpmath=387
#==============================================================================




#==============================================================================
OBJ_LIST =	sndbuffer.o	\
		sndchip.o	\
		emul_2203.o	\
		saa1099.o	\
		sndcounter.o	\
		sndrender.o
#==============================================================================




#==============================================================================
snd.a:		$(OBJ_LIST)

	ar crf snd.a							\
		$(OBJ_LIST)
#==============================================================================


#==============================================================================	
sndbuffer.o:	sndbuffer.cpp						\
			../sndrender/sndbuffer.h

	g++ sndbuffer.cpp	-o sndbuffer.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================	
#		часть инклудов std.h есть только в mingw w64
sndchip.o:	sndchip.cpp						\
			../std.h					\
				../sdk/ddraw.h				\
				../sdk/dinput.h				\
				../sdk/dsound.h				\
				../sdk/ddk.h				\
				../mods.h				\
			../emul.h					\
				../sysdefs.h				\
				../z80/defs.h				\
					../sysdefs.h			\
				../sndrender/sndrender.h		\
					../sndrender/sndbuffer.h	\
					../sysdefs.h			\
				../savevid.h				\
				../wd93.h				\
				../fdd.h				\
				../hddio.h				\
				../hdd.h				\
				../input.h				\
				../modem.h				\
				../bass.h				\
				../snd_bass.h				\
				../gshlbass.h				\
				../gshle.h				\
			../vars.h					\
				../sndrender/emul_2203.h		\
				../sndrender/sndchip.h			\
					../sysdefs.h			\
					../sndrender/sndrender.h	\
				../sndrender/saa1099.h			\
					../sysdefs.h			\
					../sndrender/sndrender.h	\
			../sndrender/emul_2203.h			\
			../sndrender/sndchip.h				\
				../sysdefs.h				\
				../sndrender/sndrender.h		
				
	g++ sndchip.cpp		-o sndchip.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
emul_2203.o:	emul_2203.cpp				\
			../std.h			\
				../sdk/ddraw.h		\
				../sdk/dinput.h		\
				../sdk/dsound.h		\
				../sdk/ddk.h		\
				../mods.h		\
			../sysdefs.h			\
			../sndrender/emul_2203.h	
		
	g++ emul_2203.cpp	-o emul_2203.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
saa1099.o:	saa1099.cpp					\
		../std.h					\
			../sdk/ddraw.h				\
			../sdk/dinput.h				\
			../sdk/dsound.h				\
			../sdk/ddk.h				\
			../mods.h				\
		../emul.h					\
			../sysdefs.h				\
			../z80/defs.h				\
				../sysdefs.h			\
			../sndrender/sndrender.h		\
				../sndrender/sndbuffer.h	\
				../sysdefs.h			\
			../savevid.h				\
			../wd93.h				\
			../fdd.h				\
			../hddio.h				\
			../hdd.h				\
			../input.h				\
			../modem.h				\
			../bass.h				\
			../snd_bass.h				\
			../gshlbass.h				\
			../gshle.h				\
		../vars.h					\
			../sndrender/emul_2203.h		\
			../sndrender/sndchip.h			\
				../sysdefs.h			\
				../sndrender/sndrender.h	\
			../sndrender/saa1099.h			\
				../sysdefs.h			\
				../sndrender/sndrender.h	\
		../sndrender/saa1099.h				\
			../sysdefs.h				\
			../sndrender/sndrender.h
			
	g++ saa1099.cpp		-o saa1099.o			\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
#		НЕНАДЕЖНАЯ ВЕТКА!!!
#sndcounter.o:	.FORCE
#.FORCE:

sndcounter.o:	sndcounter.cpp						\
			../sndrender/sndcounter.h			\
				../sndrender/sndrender.h		\
					../sndrender/sndbuffer.h	\
					../sysdefs.h			\
				../sndrender/*.h			\
				../sndrender/*.cpp
			
	g++ sndcounter.cpp	-o sndcounter.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================
sndrender.o:	sndrender.cpp						\
			../std.h"					\
				../sdk/ddraw.h				\
				../sdk/dinput.h				\
				../sdk/dsound.h				\
				../sdk/ddk.h				\
				../mods.h				\
			../emul.h					\
				../sysdefs.h				\
				../z80/defs.h				\
					../sysdefs.h			\
				../sndrender/sndrender.h		\
					../sndrender/sndbuffer.h	\
					../sysdefs.h			\
				../savevid.h				\
				../wd93.h				\
				../fdd.h				\
				../hddio.h				\
				../hdd.h				\
				../input.h				\
				../modem.h				\
				../bass.h				\
				../snd_bass.h				\
				../gshlbass.h				\
				../gshle.h				\
			../vars.h					\
				../sndrender/emul_2203.h		\
				../sndrender/sndchip.h			\
					../sysdefs.h			\
					../sndrender/sndrender.h	\
				../sndrender/saa1099.h			\
					../sysdefs.h			\
					../sndrender/sndrender.h	\
			../sndrender/sndrender.h			\
				../sndrender/sndbuffer.h		\
				../sysdefs.h

	g++ sndrender.cpp	-o sndrender.o				\
		-c -pipe $(OPT) $(TARGET) \
		$(UNREAL_FLAGS)

#==============================================================================


# для sndrender/sndbuffer.h
# нужно включать -DUSE_SND_EXTERNAL_BUFFER в аргументах компилера!!!
# хотя без него вообще невозможно собрать