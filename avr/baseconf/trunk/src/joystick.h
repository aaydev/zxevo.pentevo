#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

/**
 * @file
 * @brief Kempstone joystick support.
 * @author http://www.nedopc.com
 *
 * Kempston joystick support for ZX Evolution.
 *
 * Kempston joystick port bits (if bit set - button pressed):
 * - 0: right;
 * - 1: left;
 * - 2: down;
 * - 3: up;
 * - 4: fire;
 * - 5-7: 0.
 */

#define JOY_SWITCHS		0b00000000 
#define JOY_SEGA_8KEY	0b00000001
#define JOY_SEGA_12KEY	0b00000010
#define JOY_WITH_KBD	0b00000100
#define JOY_NEED_DETECT	0b10000000

#define SEGA_SYNC_UP() PORTA = 0b11111111
#define SEGA_SYNC_DOWN() PORTA = 0b11111011

extern volatile unsigned char gamepad_type;
extern volatile UWORD jkey_state;

/** Kempstone joystick task. */
void joystick_task(void);

void joystick_init(void);

void sega_parsing(void);

#endif //__JOYSTICK_H__
