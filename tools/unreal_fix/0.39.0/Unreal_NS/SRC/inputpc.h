#pragma once

//-----------------------------------------------------------------------------
struct PC_KEY
{
    unsigned char vkey;
    unsigned char normal;
    unsigned char shifted;
};
//-----------------------------------------------------------------------------
extern const PC_KEY pc_layout[];
extern const size_t pc_layout_count;
//-----------------------------------------------------------------------------
extern const unsigned short dik_scan[];				// NEDOREPO
extern const unsigned short atm620_xt_keyb_dik_scan[];		// [NS]
//extern const unsigned char vk_key_2_xt_key_tab[];		// [NS] DEPRECATED
//-----------------------------------------------------------------------------



