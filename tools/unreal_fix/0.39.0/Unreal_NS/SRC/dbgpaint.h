#pragma once

struct MENUITEM
{
   const char *text;
   enum FLAGS { DISABLED = 1, LEFT = 2, RIGHT = 4, CENTER = 8 } flags;
};

struct MENUDEF
{
   MENUITEM *items;
   unsigned n_items;
   const char *title;
   unsigned pos;
};


//=============================================================================
extern const int c8;		// = 8;								// [NS]
extern const int c16;		// = 16;

#define SB80	80//80	// BUFFER SIZE
#define SB30	30//30	// несмотря на то
			// что используетсо только для указания разрмера txtscr[]
			// почемуто оно ДОЛЖНО ТОЧНО соответствовать s80 s30
			// не больше!!! О_о
			// нужно втыкать в memset-ы debugscr()-а

extern const int s80;		// = 84;//80;
extern const int s30;		// = 30;//32;//30;

extern const int s192;	// = 192;
extern const int s640;	// = 672;//640;
extern const int s480;	// = 480;//512;//480;

//extern unsigned char txtscr[ 80 * 30 * 2];
extern unsigned char txtscr[ SB80 * SB30 * 2];
//=============================================================================



extern char str[0x80];
extern unsigned nfr;

void filledframe( unsigned x, unsigned y, unsigned dx, unsigned dy, unsigned char color = FFRAME_INSIDE);
void fillattr( unsigned x, unsigned y, unsigned dx, unsigned char color = FFRAME_INSIDE);
void tprint( unsigned x, unsigned y, const char *str, unsigned char attr);
void tprint_fg( unsigned x, unsigned y, const char *str, unsigned char attr);
unsigned inputhex( unsigned x, unsigned y, unsigned sz, bool hex, bool insert_mode = TRUE);	// insert_mode [NS]

int input1(unsigned x, unsigned y, unsigned val);	// [NS]
int input2(unsigned x, unsigned y, unsigned val);
int input4(unsigned x, unsigned y, unsigned val);
int input6dec(unsigned x, unsigned y, unsigned val);	//NS

void debugflip();
char handle_menu(MENUDEF *menu);
void frame(unsigned x, unsigned y, unsigned dx, unsigned dy, unsigned char attr);
