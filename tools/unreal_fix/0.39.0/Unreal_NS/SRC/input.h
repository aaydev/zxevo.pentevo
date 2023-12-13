#pragma once

//=============================================================================
class TKeyboardBuffer			// thims zxevo_ps/2	 NEDOREPO
{
    unsigned char buffer[ 256];
    unsigned char push;
    unsigned char pop;
    bool full;

public:
    //-------------------------------------------------------------------------
    TKeyboardBuffer()
    {
	Empty();
    }
    //-------------------------------------------------------------------------
    void Empty()
    {
        push = pop = 0;
        full = false;
    }
    //-------------------------------------------------------------------------
    void Push( unsigned char key)
    {
        if (!full)
        {
            buffer[push++] = key;
            if (push == sizeof( buffer)) push = 0;
            if (push == pop) full = true;
        }        
    }
    //-------------------------------------------------------------------------
    unsigned char Pop()
    {
        if (!full)
        {
            if (push != pop)
            {
                unsigned char key = buffer[ pop++];
                //-------------------------------------------------------------
		if (pop == sizeof( buffer))
		    pop = 0;
		//-------------------------------------------------------------
                return key;
            }
            return 0;
        }
        return 0xFF;
    }
    //-------------------------------------------------------------------------
};
//=============================================================================


//=============================================================================
class TKeyboardBuffer_atm620_xt_keyb					// [NS]
{
    // в АТМ620 стоит 537РУ10 на 2К
    //
    // в АТМ610 тожы
    // на выход только d0...d4
    // а на вход заюзано только 8 бит
    // тобешь 256 байт
    // видимо там хранятсо только ZX кнопки
    // и нет буфера для XT клавиатуры
    // а только когда есть сигнал готовности
    // можно достать текущий принятый код
    // точнее даже завершение приема вызывает NMI?
    // по которому идет конвертация
    
    // но тк нам надо ловить много кодов
    // а в унриале кнопки не жмутсо в риалтайме
    // то тута будет такой же буфер как у ps2 клавы
    // и читатсо коды будут из него
    unsigned char buffer[ 256];
    unsigned char push;
    unsigned char pop;
    bool full;

public:
    //-------------------------------------------------------------------------
    TKeyboardBuffer_atm620_xt_keyb()
    {
	Empty();
    }
    //-------------------------------------------------------------------------
    void Empty()
    {
        push = pop = 0;
        full = false;
    }
    //-------------------------------------------------------------------------
    void Push(unsigned char key)
    {
        if (!full)
        {
            buffer[push++] = key;
            if (push == sizeof( buffer)) push = 0;
            if (push == pop) full = true;
        }        
    }
    //-------------------------------------------------------------------------
    unsigned char Pop()
    {
        if (!full)
        {
            if (push != pop)
            {
                unsigned char key = buffer[ pop++];
                //-------------------------------------------------------------
		if (pop == sizeof( buffer))
		    pop = 0;
		//-------------------------------------------------------------
                return key;
            }
            return 0;
        }
        return 0xFF;
    }
    //-------------------------------------------------------------------------
};
//=============================================================================



//=============================================================================
struct ATM_KBD
{
    //-------------------------------------------------------------------------
    union 
    {
	unsigned char zxkeys[ 8];
	unsigned zxdata[ 2];
    };
    //-------------------------------------------------------------------------
    unsigned char mode;
    unsigned char R7;
    unsigned char lastscan;
    unsigned char cmd;
    unsigned char kR1;
    unsigned char kR2;
    unsigned char kR3;
    unsigned char kR4;
    unsigned char kR5;
    //-------------------------------------------------------------------------
    void processzx(unsigned scancode, unsigned char pressed);
    //-------------------------------------------------------------------------
    unsigned char read(unsigned char scan, unsigned char zxdata);
    //-------------------------------------------------------------------------
    void setkey(unsigned scancode, unsigned char pressed);
    //-------------------------------------------------------------------------
    void reset();
    //-------------------------------------------------------------------------
    void clear();
    //-------------------------------------------------------------------------
};
//=============================================================================


//=============================================================================
struct K_INPUT
{
#pragma pack( push, 1)
    //-------------------------------------------------------------------------
    union
    {
	volatile unsigned char kbd[ 16];
	volatile unsigned kbd_x4[ 4];
    };
    //-------------------------------------------------------------------------
    union
    { // without keymatrix effect
	volatile unsigned char rkbd[ 16];
	volatile unsigned rkbd_x4[ 4];
    };
    //-------------------------------------------------------------------------
#pragma pack( pop)

    unsigned lastkey;			
    unsigned nokb;			// NEDOREPO
    unsigned nomouse;	
    //-------------------------------------------------------------------------
    enum
    {
	KM_DEFAULT,
	KM_KEYSTICK,
	KM_PASTE_HOLD,
	KM_PASTE_RELEASE
    } keymode;
    //-------------------------------------------------------------------------
    int msx;
    int msy;
    int msx_prev;
    int msy_prev;
    int ay_x0;
    int ay_y0;
    //-------------------------------------------------------------------------
    unsigned ay_reset_t;
    unsigned char mbuttons;
    unsigned char ayR14;
    //-------------------------------------------------------------------------
    volatile unsigned char kjoy;
    unsigned char mousejoy;
    unsigned char kbdled;
    unsigned char mouse_joy_led;
    unsigned char firedelay;	// autofire vars
    unsigned char firestate;	// autofire vars
    volatile u8 fjoy; 		// fuller joystick

    ATM_KBD atm51;

    TKeyboardBuffer buffer;	//NEDOREPO
    bool buffer_enabled;	//NEDOREPO

    TKeyboardBuffer_atm620_xt_keyb atm620_xt_keyb_buffer;	// [NS]
    bool atm620_xt_keyb_buffer_enabled;				//

    unsigned stick_delay;
    int prev_wheel;

    unsigned char *textbuffer;
    unsigned textoffset;
    unsigned textsize;
    
    unsigned char tdelay;  //0.36.6 from 0.35b2
    unsigned char tdata;
    unsigned char wheel;
    //-------------------------------------------------------------------------
    unsigned char kempston_mx();
    unsigned char kempston_my();
    unsigned char aymouse_rd();
    void aymouse_wr( unsigned char val);
    //-------------------------------------------------------------------------
    void clear_zx();
    inline void press_zx( unsigned char key);
    bool process_pc_layout();
    void make_matrix();
    char readdevices();
    unsigned char read( unsigned char scan);
    u8 read_quorum( u8 scan);
    void paste();
    //-------------------------------------------------------------------------
    K_INPUT()
    {
	textbuffer = nullptr;
	// random data on coords -> some programs detects mouse by this
	ay_x0 = msx = 31;
	ay_y0 = msy = 85;

//	nomouse = 0;		//0.39.0
	nokb = nomouse = 0;	//NEDOREPO
    }
};
//=============================================================================

