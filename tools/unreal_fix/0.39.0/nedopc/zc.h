// Z-Controller by KOE
// Only SD-card
#ifndef ZC_H
#define ZC_H
class TZc
{
    TSdCard SdCard;
    u8 Cfg;
    u8 Status;
    u8 RdReg;
    u8 RdBuff;
    u8 IsCh341;
public:
    TZc() : SdCard("zc.sdcard") {}
    void Reset();
    void Open(const char *Name);
    void Close();
    void Wr(u32 Port, u8 Val);
    u8 Rd(u32 Port);
};

extern TZc Zc;
#endif
