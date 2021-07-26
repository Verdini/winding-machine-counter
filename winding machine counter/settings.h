
#ifndef settings_h
#define settings_h

#include "winding_machine_counter.h"

// This module defines the settings and the system state model.


#define PROGRAM_ADDRESS 0
#define PRODUCTION_ADDRESS 132

typedef struct settings_t {
	uint8_t initial_program; // Initial program number
	uint8_t num_programs; // Number of programs
	unsigned long sp1[15]; // SP1 array for each program - 00000
	unsigned long sp2[15]; // SP2 array for each program - 00000
	bool mode_quad; // Quadrature mode
	unsigned long ft_corr; // Encoder multiplier scale - multiplied by 10000 -> 13456 = 1.3456
	bool count_prod; // Count production
	uint8_t pos_abs; // Reset pos abs
	bool auto_reset; // Auto reset
	bool timer_is_sp1; // If timer is for SP1 or SP2
	uint8_t timer_preset; // SP2 timer preset
	int production; // production Counter - 0000;
} settings_t;


typedef enum system_state_t {
	STATE_PROD,
	STATE_PROG_1,
	STATE_PROG_2,
	STATE_PROG_3,
	STATE_CONFIG_1,
	STATE_CONFIG_2,
	STATE_CONFIG_3,
	STATE_CONFIG_4,
	STATE_CONFIG_5,
	STATE_CONFIG_6
} system_state_t;


settings_t settings_load();
void settings_save(settings_t settings);
void settings_save_production(settings_t settings);
settings_t settings_init();

#endif