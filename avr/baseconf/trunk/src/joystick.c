#include <avr/io.h>
#include <util/delay.h>

#include "pins.h"
#include "mytypes.h"

#include "rs232.h"
#include "zx.h"
#include "joystick.h"
#include "kbmap.h"

//if want Log than comment next string
#undef LOGENABLE

#define JOY_PIN_R_M   0b00000001
#define JOY_PIN_L_X   0b00000010
#define JOY_PIN_D_Y   0b00000100
#define JOY_PIN_U_Z   0b00001000
#define JOY_PIN_B_A   0b00010000
#define JOY_PIN_RLDUB 0b00011111
#define JOY_PIN_C_S

const UBYTE joymaps[4][12]={
//default				
//		Right	Left	Down	Up		B		C		A
	{	KEY_P,	KEY_O,	KEY_A,	KEY_Q,	KEY_M,	KEY_SS,	KEY_SP,
//		Start	Mode	X		Y		Z
		KEY_EN,	KEY_C,	KEY_N,	KEY_Y,	KEY_Z},
//Kempston
//		Right	Left	Down	Up		B		C		A
	{	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
//		Start	Mode	X		Y		Z
		0x00,	0x00,	KEY_EN,	KEY_SP,	KEY_Z},
//Super Mario
//		Right	Left	Down	Up		B		C		A
	{	KEY_8,	KEY_5,	KEY_6,	KEY_7,	KEY_S,	KEY_SS,	KEY_A,
//		Start	Mode	X		Y		Z
		KEY_EN,	KEY_SP,	KEY_X,	KEY_Y,	KEY_Z},
//Elite
//		Right	Left	Down	Up		B		C		A
	{	KEY_M,	KEY_N,	KEY_X,	KEY_S,	KEY_A,	KEY_SP,	KEY_F,
//		Start	Mode	X		Y		Z
		KEY_D,	KEY_T,	KEY_E,	KEY_J,	KEY_SS}};

#define JOY_MAP_DEFAULT (UBYTE *)joymaps[0]
#define JOY_MAP_KEMPST	(UBYTE *)joymaps[1]
#define JOY_MAP_SMB		(UBYTE *)joymaps[2]
#define JOY_MAP_ELITE	(UBYTE *)joymaps[3]
UBYTE * joymap = JOY_MAP_DEFAULT;
#define J_WAIT 20

volatile UBYTE gamepad_type = JOY_NEED_DETECT;
volatile UWORD jkey_state = 0x0000;

void joystick_init(void){
	UBYTE joy_state;
	
	SEGA_SYNC_UP();
	DDRA  = 0b00000100;
	
	//первый фронт клолка
	_delay_us(J_WAIT);
	//как минимум один из left/right должен быть в 1
	//и как минимум один из up/down должен быть в 1
	joy_state = JOYSTICK_PIN;
	gamepad_type = JOY_SWITCHS;
	if( (joy_state & ( JOY_PIN_L_X|JOY_PIN_R_M ) ) != 0 )
	{
		if( (joy_state & ( JOY_PIN_D_Y|JOY_PIN_U_Z ) ) != 0 )
		{
			gamepad_type = JOY_SEGA_12KEY;
		}
	}
	if( gamepad_type != JOY_SWITCHS )
	{
		SEGA_SYNC_DOWN();
		//первый спад клока
		_delay_us(J_WAIT);
		//как минимум один из left/right должен быть в 0
		//и как минимум один из up/down должен быть в 1
		gamepad_type = JOY_SWITCHS;
		joy_state = JOYSTICK_PIN;
		if( (joy_state & ( JOY_PIN_L_X|JOY_PIN_R_M ) ) != ( JOY_PIN_L_X|JOY_PIN_R_M ) )
		{
			if( (joy_state & ( JOY_PIN_D_Y|JOY_PIN_U_Z ) ) != 0 )
			{
				gamepad_type = JOY_SEGA_12KEY;
			}
		}
		
		SEGA_SYNC_UP();
	}
	
	if( gamepad_type != JOY_SWITCHS )
	{
		// sega-pad есть, определим 8 или 12 кнопок
		_delay_us(J_WAIT);
		SEGA_SYNC_DOWN();
		// второй спад клока
		_delay_us(J_WAIT);
		SEGA_SYNC_UP();
		_delay_us(J_WAIT);
		SEGA_SYNC_DOWN();
		// третий спад клока
		// определим 8 или 12 кнопок у джоя
		// left/right/up/down должы быть в 0
		_delay_us(J_WAIT);
		if( ( JOYSTICK_PIN & ( JOY_PIN_D_Y|JOY_PIN_U_Z|JOY_PIN_L_X|JOY_PIN_R_M ) ) == 0 )
		{
			gamepad_type = JOY_SEGA_12KEY;
		}
		else
		{
			gamepad_type = JOY_SEGA_8KEY;
		}
		SEGA_SYNC_UP();
	}
	
	if( gamepad_type & JOY_SEGA_12KEY )
	{// для 12 кнопок закончим цикл клока
		_delay_us(J_WAIT);
		SEGA_SYNC_DOWN();
		_delay_us(J_WAIT);
		SEGA_SYNC_UP();
	}
	
	if( gamepad_type != JOY_SWITCHS )
	{//запустим прерывание, если есть sega-pad
		OCR0 = TCNT0 + 255;
		TCCR0 = 0b00111101;
	}
	else
	{
		DDRA  = 0b00000000;
	}
}


void sega_parsing(void){
	static UWORD jkey_state_old = 0x0000;
	static UBYTE kempst_old = 0x00;
	static UBYTE jkey_mode = 0x00;
	
	if( jkey_state_old == jkey_state )
	{//изменений нет, выходим
		return;
	}
	
	jkey_state_old = jkey_state;
	
	zx_realkbd[5] = 0xff;
	zx_realkbd[6] = 0xff;
	zx_realkbd[7] = 0xff;
	zx_realkbd[8] = 0xff;
	zx_realkbd[9] = 0xff;
	
	if((jkey_state & 0x0e00) == 0x0e00){
		if(jkey_mode == 0){ 
			jkey_mode = 1;
			joymap = (UBYTE *)joymaps[(jkey_state & 0x0030) >> 4];
		}
	}else{
		UBYTE i = 0;
		jkey_mode = 0;
		if((joymap == JOY_MAP_SMB) && (jkey_state & 0x000f)) //CAPSSHIFT
			zx_realkbd[5]&=0xfe;
		if(joymap == JOY_MAP_KEMPST){			
			if (kempst_old ^ (jkey_state & 0x00ff))
			{
				kempst_old = jkey_state & 0x00ff;
				zx_spi_send(SPI_KEMPSTON_JOYSTICK, kempst_old, 0x7F);
			}
			i = 8;
			jkey_state >>= 8;
		}
		for(; i < 12; i++){
			if(jkey_state & 0x0001){
				zx_realkbd[(joymap[i]>>3)+5] &= ~(0b10000000>>(joymap[i]&0x07));
			}
			jkey_state >>= 1;
		}
	}
	
	// UWORD l = jkey_state;
	// rs232_transmit('\r');
	// for(UBYTE j=0;j<16;j++){
		// if(l & 0x8000)rs232_transmit('1');
		// else rs232_transmit('0');
		// l = l << 1;
	// }
}

void joystick_task(void)
{
	static UBYTE joy_state = 0;
	
	if( gamepad_type != JOY_SWITCHS )
	{
		return;
	}
	
	UBYTE temp = (~JOYSTICK_PIN) & JOYSTICK_MASK;

	if ( joy_state ^ temp )
	{
		//change state of joystick pins
		joy_state = temp;

		//send to port
		zx_spi_send(SPI_KEMPSTON_JOYSTICK, joy_state, 0x7F);

#ifdef LOGENABLE
	char log_joystick[] = "JS..\r\n";
	log_joystick[2] = ((temp >> 4) <= 9 )?'0'+(temp >> 4):'A'+(temp >> 4)-10;
	log_joystick[3] = ((temp & 0x0F) <= 9 )?'0'+(temp & 0x0F):'A'+(temp & 0x0F)-10;
	to_log(log_joystick);
#endif
	}
}
