#pragma once

extern unsigned asmii;
extern const unsigned cs[3][2];

int disasm_line(unsigned addr, char *line);

void mon_step();
void mon_step_x2();		// [NS]
void mon_step_x4();		// [NS]
void mon_step_x8();		// [NS]
void mon_step_x16();		// [NS]
void mon_step_x32();		// [NS]
void mon_step_x64();		// [NS]
void mon_step_x128();		// [NS]
void mon_step_x256();		// [NS]
void mon_stepover();
void mon_stepover_jump();	// [NS]

void cfindpc();

//=============================================================================
void mon_goto_disasm_addr( unsigned addr);				// [NS]

void mon_goto_disasm_rAF();
void mon_goto_disasm_rBC();
void mon_goto_disasm_rDE();
void mon_goto_disasm_rHL();
                             
void mon_goto_disasm_rAF1(); 
void mon_goto_disasm_rBC1(); 
void mon_goto_disasm_rDE1(); 
void mon_goto_disasm_rHL1(); 
                            
void mon_goto_disasm_rIX();
void mon_goto_disasm_rIY();
void mon_goto_disasm_rSP();
void mon_goto_disasm_rPC();
//=============================================================================

void chere();
void cfindtext();
void cfindcode();
void cgoto();
void cbpx();
void cbpr();	//NS
void cbpw();	//NS
void center();
void csetpc();
void cup();
void cdown();
void cleft();
void cright();
void chere();
void cpgup();
void cpgdn();
void pop_pos();
void cjump();
void cdjump();



		
//=============================================================================
void mon_view_mem_addr( unsigned addr);					// [NS]

void mon_view_mem_rAF();
void mon_view_mem_rBC();
void mon_view_mem_rDE();
void mon_view_mem_rHL();

void mon_view_mem_rAF1();
void mon_view_mem_rBC1();
void mon_view_mem_rDE1();
void mon_view_mem_rHL1();

void mon_view_mem_rIX();
void mon_view_mem_rIY();
void mon_view_mem_rSP();
void mon_view_mem_rPC();
//=============================================================================


void csave1();
void csave2();
void csave3();
void csave4();
void csave5();
void csave6();
void csave7();
void csave8();
void crest1();
void crest2();
void crest3();
void crest4();
void crest5();
void crest6();
void crest7();
void crest8();

void cfliplabels();
void c_lbl_import();

namespace z80dbg
{
void __cdecl SetLastT();
}
void showtrace();
char dispatch_trace();


extern unsigned trace_follow_regs_in_view;	//флаг что regs находится в поле зрения	//[NS]

//extern unsigned trace_follow_request;	//флаг что нужно перевести фокус на regs //[NS]

//extern unsigned trace_follow_regs;	//12=PC за кем именно следить
					// теперрь conf.

