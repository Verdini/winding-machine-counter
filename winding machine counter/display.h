#ifndef display_h
#define display_h

#include "winding_machine_counter.h"

// This module is responsible for displaying the data to a 16x2 LCD.


void display_init();

void display_intro();

void display_prod(uint8_t program, int prod, unsigned long encoder);
void display_prod_update_encoder(unsigned long encoder);
void display_prod_update_prod(int production);
void display_prod_set_sp1();
void display_prod_set_sp2();

void display_prog_initial_program(int program, int cursor);
void display_prog_num_programs(int num_programs, int cursor);
void display_prog_sp(int program, bool is_sp1, unsigned long sp, int cursor);

void display_config_mode(bool quad_mode);
void display_config_ft_corr(unsigned long ft_corr, int cursor);
void display_config_count_prod(bool count_prod);
void display_config_pos_abs(uint8_t pos_abs);
void display_config_auto_reset(bool auto_reset);
void display_config_timer_sp(bool is_sp1);
void display_config_timer_preset(int timer_preset, int cursor);

void display_saving();

#endif
