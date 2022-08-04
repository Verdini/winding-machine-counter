#include "winding_machine_counter.h"

// Inicializa o display no endereco 0x3F
// Working addresses: 0x3F , 0x27
// LCD_ADDRESS must be defined in main or arduino file
LiquidCrystal_I2C lcd(LCD_ADDRESS,2,1,0,4,5,6,7,3, POSITIVE);

char bufferLCD[16];


void display_init()
{
  lcd.begin (16,2); // Inicializa LCD
  lcd.setBacklight(HIGH);
}


void display_update()
{
  lcd.clear();
}


void display_intro()
{
  lcd.setCursor(0,0);
  lcd.print(LCD_SPLASH_LINE_0);
  lcd.setCursor(0,1);
  lcd.print(LCD_SPLASH_LINE_1);
}


void display_prod(uint8_t program, int prod, unsigned long encoder)
{
  lcd.noCursor();
  lcd.clear();
    sprintf(bufferLCD, "PG: %02d     %05lu",program, encoder);
    lcd.print(bufferLCD);
    lcd.setCursor(0,1);
    sprintf(bufferLCD, "Prod: %04d SP:", prod);
    lcd.print(bufferLCD);
}


void display_prod_update_encoder(unsigned long encoder)
{
  // Podemos usar a função itoa como alternativa
  //lcd.setCursor(11,0);
  //sprintf(bufferLCD, "%05lu", encoder);
  lcd.setCursor(11,0);
  sprintf(bufferLCD, "%05lu", encoder);
    lcd.print(bufferLCD);
}


void display_prod_update_prod(int prod)
{
  lcd.setCursor(6,1);
  sprintf(bufferLCD, "%04d", prod);
    lcd.print(bufferLCD);
}

void display_prod_set_sp1()
{
  lcd.setCursor(14,1);
  lcd.write('1');
}

void display_prod_set_sp2()
{
  lcd.setCursor(15,1);
  lcd.write('2');
}

// cursor - 1 or 2
void display_prog_initial_program(int program, int cursor)
{
  lcd.cursor();
  lcd.clear();
  sprintf(bufferLCD, "PG Inicial: %02d", program);
    lcd.print(bufferLCD);
  lcd.setCursor(14 - cursor, 0);
}

// Cursor - 1 or 2
void display_prog_num_programs(int num_programs, int cursor)
{
  lcd.cursor();
  lcd.clear();
  sprintf(bufferLCD, "Num. PG's: %02d", num_programs);
    lcd.print(bufferLCD);
  lcd.setCursor(13 - cursor, 0);
}

// cursor argument must be from 1 to 5
void display_prog_sp(int pg, bool is_sp1, unsigned long sp, int cursor)
{
  lcd.cursor();
  lcd.clear();
  sprintf(bufferLCD, "PG: %02d", pg);
    lcd.print(bufferLCD);
  lcd.setCursor(0,1);
  if(is_sp1)
    sprintf(bufferLCD, "SP1: %05ld", sp);
  else
    sprintf(bufferLCD, "SP2: %05ld", sp);
    lcd.print(bufferLCD);
  lcd.setCursor(10 - cursor, 1);
}

void display_config_mode(bool mode_quad)
{
  lcd.cursor();
  lcd.clear();
  if(mode_quad)
  {
    lcd.print("Modo: Quadratura");
    lcd.setCursor(15, 0);
  }
  else
  {
    lcd.print("Modo: Adicao");
    lcd.setCursor(11, 0);
  }
}

// cursor argument must be from 1 to 5
void display_config_ft_corr(unsigned long ft_corr, int cursor)
{
  int digit_int = ft_corr / 10000;
  int digit_dec = ft_corr % 10000;
  lcd.cursor();
  lcd.clear();
  sprintf(bufferLCD, "Ft Corr: %1d.%04d", digit_int, digit_dec);
    lcd.print(bufferLCD);
  if(cursor == 5) lcd.setCursor(9,0);
  else lcd.setCursor(15 - cursor, 0);
}

void display_config_count_prod(bool count_prod)
{
  lcd.cursor();
  lcd.clear();
  if(count_prod)
    lcd.print("Total prod: ON");
  else
    lcd.print("Total prod: OFF");
  lcd.setCursor(12, 0);
}

void display_config_pos_abs(uint8_t pos_abs)
{
  lcd.cursor();
  lcd.clear();
  if(pos_abs ==0)
    lcd.print("Pos. Abs: OFF");
  else if(pos_abs == 1)
    lcd.print("Pos. Abs: SP1");
  else
    lcd.print("Pos. Abs: SP2");
  lcd.setCursor(12, 0);
}

void display_config_auto_reset(bool auto_reset)
{
  lcd.cursor();
  lcd.clear();
  if(auto_reset)
    lcd.print("Auto reset: ON");
  else
    lcd.print("Auto reset: OFF");
  lcd.setCursor(12, 0);
}

void display_config_timer_sp(bool is_sp1)
{
  lcd.cursor();
  lcd.clear();
  if(is_sp1)
    lcd.print("Timer: SP1");
  else
    lcd.print("Timer: SP2");
  lcd.setCursor(9, 0);
}

// cursor from 1 to 2
void display_config_timer_preset(int timer_preset, int cursor)
{
  lcd.cursor();
  lcd.clear();
  sprintf(bufferLCD, "Tempo: %02d", timer_preset);
    lcd.print(bufferLCD);
  lcd.setCursor(9 - cursor, 0);
}


void display_saving()
{
  lcd.clear();
  lcd.print("*Salvando dados*");
}
