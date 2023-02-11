#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

byte levelH[8] = {
	0b10001,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};
byte levelM[8] = {
	0b10001,
	0b10001,
	0b10001,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};
byte levelL[8] = {
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b11111,
	0b11111
};


void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void setup(){
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  lcd.createChar(1, levelH);
  lcd.createChar(2, levelM);
  lcd.createChar(3, levelL);
  
}

void loop(){
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  lcd.print("Water level");
  lcd.setCursor(2, 1);
  // print message
  lcd.write(byte(1));
  lcd.setCursor(4, 1);
  lcd.print("HIGH");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Water level");
  lcd.setCursor(2, 1);
  // print message
  lcd.write(byte(2));
  lcd.setCursor(4, 1);
  lcd.print("MIDDLE");
  delay(2000);
   lcd.clear();

   lcd.setCursor(0, 0);
  lcd.print("Water level");
  lcd.setCursor(2, 1);
  // print message
  lcd.write(byte(3));
  lcd.setCursor(4, 1);
  lcd.print("LOW");
  delay (1000);
  scrollText(1,"Fill the water tank", 250, lcdColumns);
  delay(5000);

  // set cursor to first column, second row

 
}