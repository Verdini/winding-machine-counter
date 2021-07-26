#ifndef program_h
#define program_h

#include "winding_machine_counter.h"

// This module is responsible for handling the program flow through a finite state machine system.

system_state_t program_prod_loop(settings_t &settings);
void HandleEncoderInterruptQuad();
void HandleEncoderInterruptAdicao();
void go_to_prog(settings_t &settings, int increment);

system_state_t program_prog_1_loop(settings_t &settings);
system_state_t program_prog_2_loop(settings_t &settings);
system_state_t program_prog_3_loop(settings_t &settings);

system_state_t program_config_1_loop(settings_t &settings);
system_state_t program_config_2_loop(settings_t &settings);
system_state_t program_config_3_loop(settings_t &settings);
system_state_t program_config_4_loop(settings_t &settings);
system_state_t program_config_5_loop(settings_t &settings);
system_state_t program_config_6_loop(settings_t &settings);

#endif