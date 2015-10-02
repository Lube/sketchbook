//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

/*******************************************************

This program will test the LCD panel and the buttons
Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
int estado      = 0;
int pos_x       = 0;
int pos_y       = 0;
int bernie_pos_x = 10;
int bernie_pos_y = 1;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define PRESENTACION 0
#define JUEGO 1
#define WIN 2
#define GAMEOVER 3

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result

 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   



 return btnNONE;  // when all others fail, return this...
}

void getNewBernieBotPosition() 
{
  int x = random(-1,2);
  int y = random(-1,2);
  
  if (x == 0)
    x = -1;
  if (y == 0)
    y = -1;
  
  if (bernie_pos_x + x < 0 || bernie_pos_x + x > 15)
    bernie_pos_x -= x;
  else
    bernie_pos_x += x;

  if (bernie_pos_y + y < 0 || bernie_pos_y + y > 1)
    bernie_pos_y -= y;
  else
    bernie_pos_y += y;
}

boolean ganoBernie() 
{
  if (bernie_pos_x == pos_x && bernie_pos_y == pos_y)
    return true;
  else
    return false;
}

boolean ganoManu()
{
  if (pos_x == 15 && pos_y == 0)
    return true;
  else
    return false;
}

int process_estado(int key, int estado) 
{
  if (estado == PRESENTACION) 
  { 
    if (key != btnNONE) 
    {
      return JUEGO;
    }
    else
    {
      return PRESENTACION;
    }
  } 
  if (estado == JUEGO) 
  { 
    if (key != btnNONE)
        getNewBernieBotPosition();
    
    switch (lcd_key)               // depending on which button was pushed, we perform an action
    {
      case btnRIGHT:
      {
        if (pos_x < 15)
          pos_x++;
        break;
      }
      case btnLEFT:
      {
        if (pos_x > 0)
          pos_x--;
        break;        
      }
      case btnUP:
      {
        if (pos_y < 1)
          pos_y++;
        break;
      }
      case btnDOWN:
      {
        if (pos_y > 0)
          pos_y--;
        break;
      }
      case btnNONE:
      {
        break;
      }
      case btnSELECT:
      {
        break;
      }
    }
    return estado;
  }
  if (estado == GAMEOVER || estado == WIN)
     if (key != btnNONE)
        return PRESENTACION;
     else
        return estado;
}

void setup()
{
 lcd.begin(16, 2);              // start the library
 randomSeed(analogRead(1));
}
 
void loop()
{
 if (estado == PRESENTACION) 
 {
   lcd.setCursor(0,0);
   lcd.print("BERNIEBOT ATACA!");// print a simple message
   lcd.setCursor(0,1);
   lcd.print("RESCATA A MANU!");// print a simple message
 } else if (estado == JUEGO) 
 {
   lcd.setCursor(0,0);
   lcd.print("               S");
   lcd.setCursor(0,1);
   lcd.print("                ");
   
   lcd.setCursor(pos_x,pos_y);
   lcd.print("X");
         
   lcd.setCursor(bernie_pos_x, bernie_pos_y);
   lcd.print("B");
   
   if (ganoBernie())
     estado = GAMEOVER;
   if (ganoManu())
     estado = WIN;
 } else if (estado == GAMEOVER) 
 {
   lcd.setCursor(0,0);
   lcd.print("BERNIEBOT WINS! ");// print a simple message
   lcd.setCursor(0,1);
   lcd.print("MUA HA HA HA    ");// print a simple message 
 }else if (estado == WIN) 
 {
   lcd.setCursor(0,0);
   lcd.print("THAT'S WHAT     ");// print a simple message
   lcd.setCursor(0,1);
   lcd.print("       SHE SAID ");// print a simple message 
 }
 lcd_key = read_LCD_buttons();
 estado = process_estado(lcd_key, estado);
 delay(200);

}
