#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <EEPROM.h>
#include <TimerOne.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// typedef struct HighScore
// {
//     char name[3];
//     int score;
// } hsList;

////////////////////
// CHARACTER LIST //
////////////////////

void splash()
{
    lcd.print("BigBrain Memory Game");
    lcd.setCursor(1, 0);
    lcd.print("Press any button");
}

void setup()
{
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    splash();
}

void loop()
{
}