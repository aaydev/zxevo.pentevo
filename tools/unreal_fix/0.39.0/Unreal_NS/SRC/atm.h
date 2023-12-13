#pragma once

void set_atm_FF77(unsigned port, unsigned char val);
void set_atm_aFE(unsigned char addr);
//void atm_writepal(unsigned char val);			// 0.39.0
void atm_writepal(unsigned port, unsigned char val);	// NEDOREPO
u8 atm_readpal();
//extern inline 
unsigned char atm450_z(unsigned t);
//extern inline 
unsigned char atm620_z(unsigned t);		// [NS]
