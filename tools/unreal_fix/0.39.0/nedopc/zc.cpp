// Z-Controller by KOE
// Only SD-card
#include "std.h"
#include "emul.h"
#include "vars.h"
#include "sdcard.h"
#include "zc.h"
#include "util.h"

typedef ULONG (WINAPI *CH341GetVersion_t)(); // Get the DLL version number and return the version number
static CH341GetVersion_t CH341GetVersion = nullptr;
typedef ULONG (WINAPI *CH341GetDrvVersion_t)(); // Get the DLL version number and return the version number
static CH341GetDrvVersion_t CH341GetDrvVersion = nullptr;
typedef HANDLE (WINAPI *CH341OpenDevice_t)(ULONG);
static CH341OpenDevice_t CH341OpenDevice = nullptr;
typedef VOID (WINAPI *CH341CloseDevice_t)(ULONG);
static CH341CloseDevice_t CH341CloseDevice = nullptr;
typedef BOOL (WINAPI *CH341StreamSPI4_t)(
	ULONG			iIndex,  // Specify CH341 device serial number
	ULONG			iChipSelect,  // Chip select control, if bit 7 is 0, the chip select control is ignored, if bit 7 is 1, the parameters are valid: Bit 1 and bit 0 are 00/01/10, respectively, select the D0/D1/D2 pin as low level. valid chip select
	ULONG			iLength,  // Number of data bytes to be transmitted
	PVOID			ioBuffer );
static CH341StreamSPI4_t CH341StreamSPI4 = nullptr;
typedef BOOL (WINAPI *CH341Set_D5_D0_t)(ULONG,ULONG,ULONG);
static CH341Set_D5_D0_t CH341Set_D5_D0 = nullptr;
typedef PVOID	(WINAPI	*CH341GetDeviceName_t)(ULONG);
static CH341GetDeviceName_t CH341GetDeviceName = nullptr;
typedef BOOL	(WINAPI	*CH341SetStream_t)(ULONG,ULONG);// To specify the mode, see Downlink
// Bit 1-bit 0: I2C interface speed /SCL frequency, 00= low speed /20KHz,01= standard /100KHz(default),10= fast /400KHz,11= high speed /750KHz
// Bit 2: SPI I/O number /IO pins, 0= single in/single out (D3 clock /D5 out /D7 in)(default),1= double in/double out (D3 clock /D5 out D4 out /D7 in D6 in)
// Bit 7: Bit order in SPI bytes, 0= low first, 1= high first
// All other reservations must be 0
static CH341SetStream_t CH341SetStream = nullptr;

static HMODULE spiDll = nullptr;
//static int SpiDllInited = 0;
static HANDLE ch341 = NULL;
    
void TZc::Reset()
{
  SdCard.Reset();
  Cfg = 0;
  Status = 0;
  RdBuff = 0xff;
}

void TZc::Open(const char *Name) 
{ 
    if ( strcmp(Name, "<CH341>") != 0 )
    {
        // монтирование образа
        SdCard.Open(Name);
        IsCh341 = 0;
    }
    else
    {
        // инициализация ch341
        
        IsCh341 = 1;
        if ( ch341 == NULL )
        {
            // ULONG iDevIndex = 0;
            if( CH341GetVersion == nullptr )
            {
#ifdef __amd64__
                spiDll = LoadLibrary("CH341DLLA64.DLL");
#else
                spiDll = LoadLibrary("CH341DLL.DLL");
#endif
                if (!spiDll)
                {
#ifdef __amd64__
                    errmsg("failed to load CH341DLLA64.DLL");
#else
                    errmsg("failed to load CH341DLL.DLL");
#endif
                    err_win32();
                    exit(1);
                }
                
                CH341GetVersion = (CH341GetVersion_t)GetProcAddress(spiDll, "CH341GetVersion");
                CH341GetDrvVersion = (CH341GetDrvVersion_t)GetProcAddress(spiDll, "CH341GetDrvVersion");
                CH341OpenDevice = (CH341OpenDevice_t)GetProcAddress(spiDll, "CH341OpenDevice");
                CH341CloseDevice = (CH341CloseDevice_t)GetProcAddress(spiDll, "CH341CloseDevice");
                CH341StreamSPI4 = (CH341StreamSPI4_t)GetProcAddress(spiDll, "CH341StreamSPI4");
                CH341Set_D5_D0 = (CH341Set_D5_D0_t)GetProcAddress(spiDll, "CH341Set_D5_D0");
                CH341GetDeviceName = (CH341GetDeviceName_t)GetProcAddress(spiDll, "CH341GetDeviceName");
                CH341SetStream = (CH341SetStream_t)GetProcAddress(spiDll, "CH341SetStream");
            }
            printf("CH341 version: %lu\r\n", CH341GetVersion( ));
            
            char * n = (char*)CH341GetDeviceName(0);
            if ( n != 0 )printf("CH341 name: %s\r\n", n);
            
            if ( CH341OpenDevice == 0 )
            {
                errmsg("failed to load CH341OpenDevice");
                err_win32();
                exit(1);
            }
            ch341 = CH341OpenDevice(0);
                
            if ( ch341 == 0 ){
                errmsg("OpenDevice ch341 failed");
                err_win32();
                exit(1);
            }
            printf("CH341 drv version: %lu\r\n", CH341GetDrvVersion( ));
            
            CH341SetStream(0, 0x80);
            CH341Set_D5_D0(0, 0x29, 0x21);
            Cfg = 0x01;
            
            CH341Set_D5_D0(0, 0x29, 0x20);
            u8 spi_str[20] = "DimkaM";
            CH341StreamSPI4(0, 0x00, sizeof(spi_str), &spi_str);
            CH341Set_D5_D0(0, 0x29, 0x21);
        }
    }
}

void TZc::Close() 
{ 
    // закрытия образа
    SdCard.Close();

    if ( ch341 != 0 )
    {
        // закрытие ch341
        CH341CloseDevice(0);
        ch341 = 0;
        IsCh341 = 0;
    }
}

void TZc::Wr(u32 Port, u8 Val)
{
  switch(Port & 0xFF)
  {
    case 0x77: // config
        if ( Cfg != ( Val & 3 ) )
        {
            Cfg = Val & 3;
            
            if ( IsCh341 != 0 )
            {
                CH341Set_D5_D0(0, 0x29, ((Cfg>>1) & 0x01)|0x20);
            }
        }
    break;

    case 0x57: // data
        if ( IsCh341 == 0 )
        {
          if (!(Cfg & 2))   // SD card selected
          {
            RdBuff = SdCard.Rd();
            SdCard.Wr(Val);
          }
          else
            RdBuff = 255;
        }
        else
        {
            RdBuff = Val;
            CH341StreamSPI4(0, 0, 1, &RdBuff);
        }
      //printf("\nOUT %02X  in %02X",Val,RdBuff);
    break;
  }
}

u8 TZc::Rd(u32 Port)
{
  switch(Port & 0xFF)
  {
    case 0x77: // status
      return Status;      // always returns 0

    case 0x57: // data
        u8 tmp = RdBuff;
      
        if ( IsCh341 == 0 )
        {
          if (!(Cfg & 2))   // SD card selected
          {
            RdBuff = SdCard.Rd();
            SdCard.Wr(0xff);
          }
          else
            RdBuff = 255;
        }
        else
        {
            RdBuff = 0xff;
            CH341StreamSPI4(0, 0, 1, &RdBuff);
        }
      
        //printf("\nout FF  IN %02X (next %02X)",tmp,RdBuff);
        return tmp;
  }

  return 0xFF;
}

TZc Zc;
