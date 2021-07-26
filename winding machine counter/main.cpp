#include "winding_machine_counter.h"


// Program entry point
int main(void) {
	system_state_t system_state;
	settings_t settings;
	
	// Arduino init
	init();
	
	// Pin Setup
	pinMode(PIN_ENCODER_A, INPUT_PULLUP); 
	pinMode(PIN_ENCODER_B, INPUT_PULLUP);  
	pinMode(PIN_OUT_1, OUTPUT);
	pinMode(PIN_OUT_2, OUTPUT);
	buttons_init();
	
	// Set Outs to HIGH
	digitalHigh(PIN_OUT_1);
	digitalHigh(PIN_OUT_2);

	// Load Program from EEPROM
	settings = settings_load();

	// Init LCD
	display_init();
	
	// Splash screen
	display_intro();
	
	// Delay for splash screen
	delay(1500);
	
	// Set Outs to LOW
	digitalLow(PIN_OUT_1);
	digitalLow(PIN_OUT_2);
	
	system_state = STATE_PROD;
	
    
	// Main Loop
	for (;;) {
		switch(system_state)
		{
			case STATE_PROD:	
			default:
				system_state = program_prod_loop(settings);
				break;
				
			case STATE_PROG_1:
				system_state = program_prog_1_loop(settings);
				break;
				
			case STATE_PROG_2:
				system_state = program_prog_2_loop(settings);
				break;
				
			case STATE_PROG_3:
				system_state = program_prog_3_loop(settings);
				break;
				
			case STATE_CONFIG_1:
				 system_state = program_config_1_loop(settings);
				 break;
			
			case STATE_CONFIG_2:
				 system_state = program_config_2_loop(settings);
				 break;
				 
			case STATE_CONFIG_3:
				 system_state = program_config_3_loop(settings);
				 break;
				 
			case STATE_CONFIG_4:
				 system_state = program_config_4_loop(settings);
				 break;
				 
			case STATE_CONFIG_5:
				 system_state = program_config_5_loop(settings);
				 break;
				 
			case STATE_CONFIG_6:
				 system_state = program_config_6_loop(settings);
				 break;
		}
	}
        
	return 0; // Never reached
}

