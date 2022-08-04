#include "winding_machine_counter.h"

settings_t settings;

// Load settings from EEPROM
settings_t settings_load()
{
  // Testing purposes only
  // settings_t settings = settings_init();

  settings_t settings;
  EEPROM.get(0, settings);

  // Security verifications
  if(settings.initial_program < 1 || settings.initial_program > 15) settings.initial_program = 1;
  if(settings.num_programs < 1 || settings.num_programs > 15) settings.num_programs = 1;

  for(int i = 0; i < 15; i++)
  {
    if(settings.sp1[i] > 99999UL) settings.sp1[i] = 100;
    if(settings.sp2[i] > 99999UL) settings.sp2[i] = 100;
  }

  if(settings.ft_corr == 0 || settings.ft_corr > 19999UL) settings.ft_corr = 10000UL;
  if(settings.timer_preset > 99) settings.timer_preset = 0;
  if(settings.production > 9999) settings.production = 0;

  return settings;
}

// Save settings to EEPROM
void settings_save(settings_t settings)
{
  EEPROM.put(PROGRAM_ADDRESS, settings);
}

// Save only settings production to EEPROM
void settings_save_production(settings_t settings)
{
  EEPROM.write(PRODUCTION_ADDRESS, settings.production);
}

// Test function to initialize settings manually
settings_t settings_init()
{
  settings_t settings;

  settings.initial_program = 1;
  settings.num_programs = 2;

  settings.sp1[0] = 200;
  settings.sp2[0] = 400;
  settings.sp1[1] = 200;
  settings.sp2[1] = 400;
  settings.sp1[2] = 200;
  settings.sp2[2] = 400;
  settings.sp1[3] = 200;
  settings.sp2[3] = 400;
  settings.sp1[4] = 200;
  settings.sp2[4] = 400;
  settings.sp1[5] = 200;
  settings.sp2[5] = 400;
  settings.sp1[6] = 200;
  settings.sp2[6] = 400;
  settings.sp1[7] = 200;
  settings.sp2[7] = 400;
  settings.sp1[8] = 200;
  settings.sp2[8] = 400;
  settings.sp1[9] = 200;
  settings.sp2[9] = 400;
  settings.sp1[10] = 200;
  settings.sp2[10] = 400;
  settings.sp1[11] = 200;
  settings.sp2[11] = 400;
  settings.sp1[12] = 200;
  settings.sp2[12] = 400;
  settings.sp1[13] = 200;
  settings.sp2[13] = 400;
  settings.sp1[14] = 200;
  settings.sp2[14] = 400;

  settings.mode_quad = false;
  settings.ft_corr = 10000;
  settings.count_prod = true;
  settings.pos_abs = 0;
  settings.auto_reset = true;
  settings.timer_is_sp1 = true;
  settings.timer_preset = 02;
  settings.production = 0;

  return settings;
}
