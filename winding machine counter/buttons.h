#ifndef buttons_h
#define buttons_h

#include "winding_machine_counter.h"

// This module handles the buttons logic.

// Buttons inputs
#define PIN_BTN_R 4
#define PIN_BTN_P 5
#define PIN_BTN_UP 6
#define PIN_BTN_LEFT 7

void buttons_init();
void buttons_loop();

void buttons_r_set_timer(unsigned long preset);
bool buttons_r_check();
bool buttons_p_check();
bool buttons_p_long_check();
bool buttons_up_check();
bool buttons_up_long_check();
bool buttons_left_check();
bool buttons_left_long_check();

#endif
