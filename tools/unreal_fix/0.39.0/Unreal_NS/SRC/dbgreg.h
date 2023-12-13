#pragma once


// набор констант вместо старых magic number-ов		[NS]
//
//		type		уникальный 8bit
//				l337 magic number
#define		HEX_1		0x01			//0...1
#define		HEX_2		0x02			//0...3

#define		HEX_8		0x08			//xx
#define		HEX_16		0x10			//xxxx

#define		HEX_IM2		0x12			//d с ограничением по числу im0...im2
#define		DEC_6T		0x76			//dddddd с ограничением по числу тактов во фрейме

#define		BIT_0		0xB0			//потом 0xB0 вычитаетсо и XX << BIT_N
#define		BIT_1		0xB1
#define		BIT_2		0xB2
#define		BIT_3		0xB3
#define		BIT_4		0xB4
#define		BIT_5		0xB5
#define		BIT_6		0xB6
#define		BIT_7		0xB7



struct TRegLayout
{
   size_t offs;
   unsigned char width;
   unsigned char x,y;
   unsigned char lf,rt,up,dn;
};

extern const TRegLayout regs_layout[];
extern const size_t regs_layout_count;

void ra();
void rf();
void rbc();
void rde();
void rhl();
void rsp();
void rpc();
void rix();
void riy();
void ri();
void rr();
void rm();
void r_1();
void r_2();
void rSF();
void rZF();
void rF5();
void rHF();
void rF3();
void rPF();
void rNF();
void rCF();

void rcodejump();
void rdatajump();

void rleft();
void rright();
void rup();
void rdown();
void renter();

void showregs();
char dispatch_regs();
