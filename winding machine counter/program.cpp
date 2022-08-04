#include "winding_machine_counter.h"


uint8_t program_number; // Current Program Number;
unsigned long encoder_ticks_copy;

bool prev_achieved_sp1;
bool prev_achieved_sp2;


// Vars that are shared with the interruption
volatile unsigned long ft_corr;
volatile bool encoder_a_set;
volatile bool encoder_b_set;
volatile unsigned long current_sp1;
volatile unsigned long current_sp2;
volatile bool achieved_sp1;
volatile bool achieved_sp2;
volatile unsigned long encoder_ticks;
volatile bool encoder_changed;

unsigned long timer_millis;


// Production Screen loop
system_state_t program_prod_loop(settings_t &settings)
{
  system_state_t state;
  if(program_number == 0)
  {
    program_number = settings.initial_program;
    encoder_ticks = 0;
    encoder_ticks_copy = 0;  // This is necessary because AVRGCC sucks
    current_sp1 = settings.sp1[ program_number - 1 ] * 10000UL; // Arrays start from zero
    current_sp2 = settings.sp2[ program_number - 1 ] * 10000UL; // Arrays start from zero
    achieved_sp1 = false;
    achieved_sp2 = false;
    prev_achieved_sp1 = false;
    prev_achieved_sp2 = false;
    encoder_changed = false;
  }

  display_prod(program_number, settings.production, encoder_ticks_copy);

  // Attach Encoder Interrupt
  if(settings.mode_quad)
  {
    ft_corr = settings.ft_corr / 2.0;
    attachInterrupt(ENCODER_INTERRUPT, HandleEncoderInterruptQuad, CHANGE);
  }
  else
  {
    ft_corr = settings.ft_corr;
    attachInterrupt(ENCODER_INTERRUPT, HandleEncoderInterruptAdicao, RISING);
  }


  for(;;)
  {
    buttons_loop();

    if(encoder_changed)
    {
      noInterrupts();
      encoder_ticks_copy = (encoder_ticks/10000UL);
      interrupts();
      encoder_changed = false;

      if(!prev_achieved_sp1 && achieved_sp1)
      {
        prev_achieved_sp1 = true;

        if(settings.auto_reset && settings.timer_is_sp1)
          buttons_r_set_timer(settings.timer_preset * 1000);

        display_prod_set_sp1();
      }

      if(!prev_achieved_sp2 && achieved_sp2)
      {
        prev_achieved_sp2 = true;
        if(settings.auto_reset && !settings.timer_is_sp1)
          buttons_r_set_timer(settings.timer_preset * 1000);

        display_prod_set_sp2();
      }

      display_prod_update_encoder(encoder_ticks_copy);
    }


    if(buttons_r_check())  // Go to next program
    {
      go_to_prog(settings, 1);
      display_prod(program_number, settings.production, encoder_ticks_copy);
    }
    else if( buttons_p_check() )  // Repeat current program
    {
      go_to_prog(settings, 0);
      display_prod(program_number, settings.production, encoder_ticks_copy);
    }
    else if( buttons_p_long_check() )  // Show programming screen
    {
      state = STATE_PROG_1;
      break;
    }
    else if( buttons_up_check() )  // Go to initial program
    {
      go_to_prog(settings, -2);
      display_prod(program_number, settings.production, encoder_ticks_copy);
    }
    else if( buttons_up_long_check() )  // Reset production counter
    {
      settings.production = 0;
      settings_save_production(settings);
      display_prod_update_prod(settings.production);
    }
    else if( buttons_left_check() )  // Go to previous program
    {
      go_to_prog(settings, -1);
      display_prod(program_number, settings.production, encoder_ticks_copy);
    }
    else if( buttons_left_long_check() )  // Show config screen
    {
      state = STATE_CONFIG_1;
      break;
    }
  } // Infinite for End

  // Detach encoder interrupt for other tasks
  detachInterrupt(ENCODER_INTERRUPT);

  return state;
} //Function end


// Interrupt service routines for the left motor's quadrature encoder
void HandleEncoderInterruptQuad()
{
  encoder_a_set = digitalState(PIN_ENCODER_A);   // read the input pin
  encoder_b_set = digitalState(PIN_ENCODER_B);   // read the input pin

  if(encoder_a_set ^ encoder_b_set)  // Clockwise
  {
    encoder_ticks += ft_corr;
    if(encoder_ticks >= 1000000000UL) encoder_ticks = 0;
  }
  else  // Counter clockwise
  {
    encoder_ticks -= ft_corr;
    if(encoder_ticks >= 1000000000UL) encoder_ticks = 999990000UL;
  }

  encoder_changed = true;
  if(encoder_ticks == current_sp1) { digitalHigh(PIN_OUT_1); achieved_sp1 = true; }
  if(encoder_ticks == current_sp2) { digitalHigh(PIN_OUT_2); achieved_sp2 = true; }
}


// Interrupt service routines for the left motor's quadrature encoder
void HandleEncoderInterruptAdicao()
{
  encoder_b_set = digitalState(PIN_ENCODER_B);   // read the input pin

  if(encoder_b_set)  // Soma
  {
    encoder_ticks += ft_corr;
    if(encoder_ticks >= 1000000000UL) encoder_ticks = 0;
  }
  else  // Subtrai
  {
    encoder_ticks -= ft_corr;
    if(encoder_ticks >= 1000000000UL) encoder_ticks = 999990000UL;
  }

  encoder_changed = true;
  if(encoder_ticks == current_sp1) { digitalHigh(PIN_OUT_1); achieved_sp1 = true; }
  if(encoder_ticks == current_sp2) { digitalHigh(PIN_OUT_2); achieved_sp2 = true; }
}


void go_to_prog(settings_t &settings, int increment)
{
  // Go to next PG
  digitalLow(PIN_OUT_1);
  digitalLow(PIN_OUT_2);

  if(increment < -1)  // Return to initial program;
  {
    program_number = settings.initial_program;
  }
  else if(increment == -1)  // Go back one program
  {
    if(program_number != settings.initial_program)
    {
      if(program_number == 0)
        program_number = 15;
      else
        program_number--;
    }
    else
    {
      int last_program = (settings.initial_program + settings.num_programs - 1);
      if(last_program > 15)
        last_program -= 15;
      program_number = last_program;
    }
  }
  else if(increment > 0)  // Go to the next program
  {
    int last_program = (settings.initial_program + settings.num_programs - 1);
    if(last_program > 15)
      last_program -= 15;

    if(program_number != last_program)
    {
      program_number++;
      if(program_number > 15) program_number = 1;
    }
    else
    {
      program_number = settings.initial_program;
      if(settings.count_prod)
      {
        settings.production++;
        settings_save_production(settings);
      }

    }
  }

  noInterrupts();

  if(!achieved_sp1 && !achieved_sp2)
    encoder_ticks = 0;
  else
    if(settings.pos_abs == 0)
      encoder_ticks = 0;
    else
      if(settings.pos_abs == 1)
        if (encoder_ticks > current_sp1)
          encoder_ticks -= current_sp1;
        else
          encoder_ticks = 0;
      else
        if(encoder_ticks > current_sp2)
          encoder_ticks -= current_sp2;
        else
          encoder_ticks = 0;

  encoder_ticks_copy = encoder_ticks/10000UL;
  current_sp1 = settings.sp1[ program_number - 1] * 10000;
  current_sp2 = settings.sp2[ program_number - 1] * 10000;


  prev_achieved_sp1 = false;
  prev_achieved_sp2 = false;

  if(current_sp1 == 0)
  {
    digitalHigh(PIN_OUT_1);
    achieved_sp1 = true;
    encoder_changed = true;
  }
  else
    achieved_sp1 = false;


  if(current_sp2 == 0)
  {
    digitalHigh(PIN_OUT_2);
    achieved_sp2 = true;
    encoder_changed = true;
  }
  else
    achieved_sp2 = false;

  interrupts();
}


// Initial program selection screen
system_state_t program_prog_1_loop(settings_t &settings)
{
  system_state_t state;
  int initial_program = settings.initial_program;
  int cursor = 1;

  display_prog_initial_program(initial_program, cursor);

  for(;;)
  {
    buttons_loop();


    if(buttons_p_check())
    {
      settings.initial_program = initial_program;
      state = STATE_PROG_2;
      break;
    }
    else if(buttons_up_check())
    {
      if(cursor == 1) // unidade
      {
        if(initial_program < 9)
          initial_program++;
        else if(initial_program == 9)
          initial_program = 1;
        else if(initial_program < 15)
          initial_program++;
        else if(initial_program == 15)
          initial_program = 10;
      }
      else  // dezena
      {
        if(initial_program < 6)
          initial_program += 10;
        else if(initial_program < 10)
          initial_program = 15;
        else if(initial_program == 10)
          initial_program = 1;
        else
          initial_program -= 10;
      }

      display_prog_initial_program(initial_program, cursor);
    }
    else if(buttons_left_check())
    {
      // Move cursor to the left
      cursor++;
      if(cursor > 2) cursor = 1;

      // Display new cursor position
      display_prog_initial_program(initial_program, cursor);
    }
  }

  return state;
}


// Number of programs selection screen
system_state_t program_prog_2_loop(settings_t &settings)
{
  system_state_t state;
  int num_programs = settings.num_programs;
  int cursor = 1;

  display_prog_num_programs(settings.num_programs, cursor);

  for(;;)
  {
    buttons_loop();

    if(buttons_p_check())
    {
      settings.num_programs = num_programs;
      state = STATE_PROG_3;
      break;
    }
    else if(buttons_up_check())
    {
      if(cursor == 1) // unidade
      {
        if(num_programs < 9)
          num_programs++;
        else if(num_programs == 9)
          num_programs = 1;
        else if(num_programs < 15)
          num_programs++;
        else if(num_programs == 15)
          num_programs = 10;
      }
      else  // dezena
      {
        if(num_programs < 6)
          num_programs += 10;
        else if(num_programs < 10)
          num_programs = 15;
        else if(num_programs == 10)
          num_programs = 1;
        else
          num_programs -= 10;
      }

      // Display new value
      display_prog_num_programs(num_programs, cursor);
    }
    else if(buttons_left_check())
    {
      // Move cursor to the left
      cursor++;
      if(cursor > 2) cursor = 1;

      // Display new cursor position
      display_prog_num_programs(num_programs, cursor);
    }
  }

  return state;
}


// SetPoints selection screen
system_state_t program_prog_3_loop(settings_t &settings)
{
  system_state_t state;
  int current_program = settings.initial_program;
  int num_programs = settings.num_programs;
  unsigned long current_sp = settings.sp1[ current_program - 1 ];
  bool is_sp1 = true;
  int current_sp_cursor = 1;   // 1 to 5, where 1 is LSB


  display_prog_sp(current_program, is_sp1, current_sp, current_sp_cursor );

  for(;;)
  {
    buttons_loop();

    if(buttons_r_check())
    {
      if(is_sp1)
        settings.sp1[ current_program - 1] = current_sp;
      else
        settings.sp2[ current_program - 1] = current_sp;
      display_saving();
      settings_save(settings);
      delay(1000);
      state = STATE_PROD;
      break;
    }
    if(buttons_p_check())  // Accept new value
    {
      if(is_sp1)  // Update SP1 and go to SP2 selection
      {
        settings.sp1[ current_program - 1] = current_sp;  // Save SP1

        if(current_program == program_number)  //if we are updating current program sp1
        {
          current_sp1 = current_sp * 10000;
          if(encoder_ticks >= current_sp1) { digitalHigh(PIN_OUT_1); achieved_sp1 = true; }
          encoder_changed = true;
        }

        is_sp1 = false;  // Change to SP2
        current_sp = settings.sp2[ current_program - 1 ];
        current_sp_cursor = 1;
        display_prog_sp(current_program, is_sp1, current_sp, current_sp_cursor );  // Display new SP
      }
      else
      {
        settings.sp2[ current_program - 1] = current_sp;  // Save SP2

        if(current_program == program_number)  //if we are updating current program sp2
        {
          current_sp2 = current_sp * 10000;
          if(encoder_ticks >= current_sp2) { digitalHigh(PIN_OUT_2); achieved_sp2 = true; }
          encoder_changed = true;
        }

        int last_program = (settings.initial_program + settings.num_programs - 1);
        if(last_program > 15)
          last_program -= 15;

        if(current_program != last_program)  // if is not the last program, then go to the next program
        {
          current_program++;
          if(current_program > 15) current_program = 1;
          current_sp = settings.sp1[ current_program - 1];
          is_sp1 = true;
          current_sp_cursor = 1;

          display_prog_sp(current_program, is_sp1, current_sp, current_sp_cursor );  // Display new SP
        }
        else  // If is the last program, then save and go to production
        {
          display_saving();
          settings_save(settings);
          delay(1000);
          state = STATE_PROD;
          break;
        }
      }

    }
    else if(buttons_up_check())  // Change value based on cursor position
    {
      // Get selected digit
      unsigned long scale = 1;
      for(int i = 1; i < current_sp_cursor; i++) scale *= 10;

      unsigned long current_digit = (current_sp / scale ) % 10;
      current_sp -= current_digit * scale; // Subtract value
      // Increase digit
      current_digit++;
      if(current_digit > 9) current_digit = 0;
      // Add new value
      current_sp += current_digit * scale;
      // Display new value
      display_prog_sp(current_program, is_sp1, current_sp, current_sp_cursor );
    }
    else if(buttons_left_check())  // Changes cursor position
    {
      // Move cursor to the left
      current_sp_cursor++;
      if(current_sp_cursor > 5) current_sp_cursor = 1;

      // Display new cursor position
      display_prog_sp(current_program, is_sp1, current_sp, current_sp_cursor );
    }
  }

  return state;
}


system_state_t program_config_1_loop(settings_t &settings)
{
  system_state_t state;
  bool mode_quad = settings.mode_quad;

  display_config_mode(mode_quad);

  for(;;)
  {
    buttons_loop();

    if(buttons_p_check())
    {
      settings.mode_quad = mode_quad;
      state = STATE_CONFIG_2;
      break;
    }
    else if(buttons_up_check())
    {
      if(mode_quad) { display_config_mode(false); mode_quad = false; }
      else { display_config_mode(true); mode_quad = true; }
    }
  }

  return state;
}


system_state_t program_config_2_loop(settings_t &settings)
{
  system_state_t state;
  unsigned long ft_corr = settings.ft_corr;
  int cursor = 1;  // 1 to 5

  display_config_ft_corr(ft_corr, cursor);

  for(;;)
  {
    buttons_loop();

    if(buttons_p_check())
    {
      settings.ft_corr = ft_corr;
      state = STATE_CONFIG_3;
      break;
    }
    else if(buttons_up_check())
    {
      // Get selected digit
      unsigned long scale = 1;
      for(int i = 1; i < cursor; i++) scale *= 10;

      unsigned long current_digit = (ft_corr / scale ) % 10;
      ft_corr -= current_digit * scale; // Subtract value
      // Increase digit
      current_digit++;
      if(current_digit > (cursor == 5? 1 : 9)) current_digit = 0;
      // Add new value
      ft_corr += current_digit * scale;
      // Display new value
      display_config_ft_corr(ft_corr, cursor);
    }
    else if(buttons_left_check())
    {
      // Move cursor to the left
      cursor++;
      if(cursor > 5) cursor = 1;

      // Display new cursor position
      display_config_ft_corr(ft_corr, cursor);
    }
  }

  return state;
}


system_state_t program_config_3_loop(settings_t &settings)
{
  system_state_t state;
  bool count_prod = settings.count_prod;

  display_config_count_prod(count_prod);

  for(;;)
  {
    buttons_loop();

    if(buttons_p_check())
    {
      settings.count_prod = count_prod;
      state = STATE_CONFIG_4;
      break;
    }
    else if(buttons_up_check())
    {
      if(count_prod) { display_config_count_prod(false); count_prod = false; }
      else { display_config_count_prod(true); count_prod = true; }
    }
  }

  return state;
}


system_state_t program_config_4_loop(settings_t &settings)
{
  system_state_t state;
  uint8_t pos_abs = settings.pos_abs;

  display_config_pos_abs(pos_abs);

  for(;;)
  {
    buttons_loop();

    if(buttons_p_check())
    {
      settings.pos_abs = pos_abs;
      state = STATE_CONFIG_5;
      break;
    }
    else if(buttons_up_check())
    {
      pos_abs++; if(pos_abs > 2) pos_abs = 0;
      display_config_pos_abs(pos_abs);
    }
  }

  return state;
}


system_state_t program_config_5_loop(settings_t &settings)
{
  system_state_t state;
  bool auto_reset = settings.auto_reset;

  display_config_auto_reset(auto_reset);

  for(;;)
  {
    buttons_loop();

    if(buttons_p_check())
    {
      settings.auto_reset = auto_reset;
      if(auto_reset)  // if auto reset is ON, then set the SP2 Timer
      {
        state = STATE_CONFIG_6;
      }
      else  // If auto reset is OFF, the nignore the SP2 Timer
      {
        display_saving();
        settings_save(settings);
        delay(1000);
        state = STATE_PROD;
      }
      break;
    }
    else if(buttons_up_check())
    {
      if(auto_reset) { display_config_auto_reset(false); auto_reset = false; }
      else { display_config_auto_reset(true); auto_reset = true; }
    }
  }

  return state;
}


system_state_t program_config_6_loop(settings_t &settings)
{
  system_state_t state;
  bool timer_is_sp1 = settings.timer_is_sp1;
  uint8_t timer_preset = settings.timer_preset;
  bool is_in_timer_sp = true;
  int cursor = 1; // 1 or 2 because timer is in the interval [00, 99]

  display_config_timer_sp(timer_is_sp1);

  for(;;)
  {
    buttons_loop();

    if(buttons_p_check())
    {
      if(is_in_timer_sp)  // If is in the timer on screen
      {
        settings.timer_is_sp1 = timer_is_sp1;
        is_in_timer_sp = false;
        display_config_timer_preset(timer_preset, cursor);
      }
      else  // if is in the timer preset screen
      {
        settings.timer_preset = timer_preset;
        display_saving();
        settings_save(settings);
        delay(1000);
        state = STATE_PROD;
        break;
      }
    }
    else if(buttons_up_check())
    {
      if(is_in_timer_sp)
      {
        // A simple timer_on = !timer_on won't work!!!!
        if(timer_is_sp1) { display_config_timer_sp(false); timer_is_sp1 = false; }
        else { display_config_timer_sp(true); timer_is_sp1 = true; }
      }
      else
      {
        // Get selected digit
        unsigned long scale = 1;
        for(int i = 1; i < cursor; i++) scale *= 10;

        unsigned long current_digit = (timer_preset / scale ) % 10;
        timer_preset -= current_digit * scale; // Subtract value
        // Increase digit
        current_digit++;
        if(current_digit > 9) current_digit = 0;
        // Add new value
        timer_preset += current_digit * scale;
        // Display new value
        display_config_timer_preset(timer_preset, cursor);
      }
    }
    else if(buttons_left_check())
    {
      if(!is_in_timer_sp)
      {
        cursor++;
        if(cursor > 2) cursor = 1;
        display_config_timer_preset(timer_preset, cursor);
      }
    }
  }

  return state;
}
