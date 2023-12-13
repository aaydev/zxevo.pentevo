//-----------------------------------------------------------------------------
// Debug Tools								// [NS]
//-----------------------------------------------------------------------------

#pragma once


	 // [NS] test
//extern HWND hwnd_debug_tool;
//extern HWND hwnd_debug_tool_2;

//=============================================================================
enum 
{
    DT_NONE,
    DT_PALETTE_COMP_PAL,	// comp_pal (ula plus palette)
    DT_PALETTE_ATM3_PAL,	// atm3_pal
    DT_PALETTE_PAL0,		// pal0 (unreal palette)
    DT_TEST
};
//=============================================================================



//  bool debug_tool_create;	
extern int debug_tool_create_nmb;	// костыль



	
#define DEBUG_TOOLS_MAX_COUNT	256

extern HWND debug_tool_hwnd[ DEBUG_TOOLS_MAX_COUNT];
extern int debug_tool_type[ DEBUG_TOOLS_MAX_COUNT];
extern bool debug_tool_exist[ DEBUG_TOOLS_MAX_COUNT];	// 1 - флаг включенности
extern bool debug_tool_delete[ DEBUG_TOOLS_MAX_COUNT];	// 1 - на удаление
    
// для перемещения окна без остановки главного окна
extern int debug_tool_x[ DEBUG_TOOLS_MAX_COUNT];
extern int debug_tool_y[ DEBUG_TOOLS_MAX_COUNT];
extern int debug_tool_Mode[ DEBUG_TOOLS_MAX_COUNT];
extern bool debug_tool_Captured[ DEBUG_TOOLS_MAX_COUNT];



    

    
//=============================================================================
extern void run_debug_tool( int tool_type);
//=============================================================================
extern LRESULT CALLBACK WndProc_debug_tools(	HWND hwnd,
						UINT iMsg,
						WPARAM wParam,
						LPARAM lParam
					);
//=============================================================================

