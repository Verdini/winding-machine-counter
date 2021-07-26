
#include "winding_machine_counter.h"

// Button state control variables
int btn_r, btn_p, btn_up, btn_left;
int last_btn_r, last_btn_p, last_btn_up, last_btn_left;

unsigned long last_deb_r, last_deb_p, last_deb_up, last_deb_left;
unsigned long time_low_p, time_low_up, time_low_left;

bool btn_r_pressed, btn_p_pressed, btn_up_pressed, btn_left_pressed;
bool read_p_long, read_up_long, read_left_long;
bool btn_p_long_pressed, btn_up_long_pressed, btn_left_long_pressed;

int signal_reset, last_signal_reset;
unsigned long last_reset;
unsigned long reset_preset = 0;


unsigned long current_millis;

unsigned long debounce_delay = 50; // debounce delay
unsigned long time_low = 1000; // Time for long press


// Buttons interface initialization
void buttons_init()
{
	read_p_long = false;
	read_up_long = false;
	read_left_long = false;
	
	
	// Initialize buttons input
	pinMode(PIN_BTN_R, INPUT_PULLUP);
	pinMode(PIN_BTN_P, INPUT_PULLUP);
	pinMode(PIN_BTN_UP, INPUT_PULLUP);
	pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
	
	delay(100);
	last_btn_p = btn_p = digitalState(PIN_BTN_P);
	last_btn_up = btn_up = digitalState(PIN_BTN_UP);
	last_btn_left = btn_left = digitalState(PIN_BTN_LEFT);
}

// Buttons loop to check state

// Button loop for one hand mode
void buttons_loop()
{
	btn_r_pressed = false;
	btn_p_pressed = false;
	btn_p_long_pressed = false;
	btn_up_pressed = false;
	btn_up_long_pressed = false;
	btn_left_pressed = false;
	btn_left_long_pressed = false;
	
	current_millis = millis();
	
	int read_btn_r = digitalState(PIN_BTN_R);
	int read_btn_p = digitalState(PIN_BTN_P);
	int read_btn_up = digitalState(PIN_BTN_UP);
	int read_btn_left = digitalState(PIN_BTN_LEFT);
	
	if(read_btn_r != last_btn_r) last_deb_r = current_millis;
	if(read_btn_p != last_btn_p) last_deb_p = current_millis; 
	if(read_btn_up != last_btn_up) last_deb_up = current_millis; 
	if(read_btn_left != last_btn_left) last_deb_left = current_millis; 
	if(signal_reset != last_signal_reset) last_reset = current_millis;
	
	
	if((current_millis - last_deb_r ) > debounce_delay) 
	{ 
		if(read_btn_r != btn_r) 
		{ 
			btn_r = read_btn_r;
			if(btn_r == LOW)
			{
				btn_r_pressed = true;
			}
		}  
	}
	
	if((current_millis - last_reset) > reset_preset)
	{
		if(signal_reset)
		{
			btn_r_pressed = true;
			signal_reset = 0;
		}
	}
	
	
	if((current_millis - last_deb_p ) > debounce_delay) 
	{ 
		if(read_btn_p != btn_p) 
		{ 
			btn_p = read_btn_p;
			if(btn_p == LOW)
			{
				time_low_p = last_deb_p;
				read_p_long = true;
			}
			else
			{
				read_p_long = false;
				if( (current_millis - time_low_p) < 1000)
					btn_p_pressed = true;
			}
		}
		
		if(read_p_long)
			if( (current_millis - time_low_p) > 1000 )
			{
				read_p_long = false;
				btn_p_long_pressed = true;
			}
	}
		

	if((current_millis - last_deb_up ) > debounce_delay) 
	{ 
		if(read_btn_up != btn_up) 
		{ 
			btn_up = read_btn_up;
			if(btn_up == LOW)
			{
				time_low_up = last_deb_up;
				read_up_long = true;
			}
			else
			{
				read_up_long = false;
				if( (current_millis - time_low_up) < 1000)
					btn_up_pressed = true;
			}
		}
		
		if(read_up_long)
			if( (current_millis - time_low_up) > 1000 )
			{
				read_up_long = false;
				btn_up_long_pressed = true;
			}
	}
	
	if((current_millis - last_deb_left ) > debounce_delay) 
	{ 
		if(read_btn_left != btn_left) 
		{ 
			btn_left = read_btn_left;
			if(btn_left == LOW)
			{
				time_low_left = last_deb_left;
				read_left_long = true;
			}
			else
			{
				read_left_long = false;
				if( (current_millis - time_low_left) < 1000)
					btn_left_pressed = true;
			}
		}
		
		if(read_left_long)
			if( (current_millis - time_low_left) > 1000 )
			{
				read_left_long = false;
				btn_left_long_pressed = true;
			}
	}
	

	last_btn_r = read_btn_r;
	last_btn_p = read_btn_p;
	last_btn_up = read_btn_up;
	last_btn_left = read_btn_left;
	last_signal_reset = signal_reset;
}


// Simulates the button R pressed after preset time
void buttons_r_set_timer(unsigned long preset)
{
	reset_preset = preset;
	signal_reset = 1;
}

// Return buttons state after processing
bool buttons_r_check()
{
	return btn_r_pressed;
}

bool buttons_p_check()
{
	return btn_p_pressed;
}

bool buttons_p_long_check()
{
	return btn_p_long_pressed;
}

bool buttons_up_check()
{
	return btn_up_pressed;
}

bool buttons_up_long_check()
{
	return btn_up_long_pressed;
}

bool buttons_left_check()
{
	return btn_left_pressed;
}

bool buttons_left_long_check()
{
	return btn_left_long_pressed;
}

