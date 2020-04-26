#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
//#include <EEPROM.h>

// Custom characters
#define UP_ICO 0
#define DOWN_ICO 1
#define LEFT_ICO 2
#define RIGHT_ICO 3
#define SELECT_ICO 4
#define ELPS_ICO 5

// Backlight colours
#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7 

/*
    I reccommend you enable folding in the settings. Then
    right-click > folding > collapse all folds 
    for an easier time looking at this massive file.
  
    Most Strings are stored in flash - hence the F("abc")
    I would use PROGMEM but I discovered it too late and
    lead to complications with previous methods.

    This is a good compromise for minimising SRAM usage 
    and not rewriting half of the game. 

    Some methods have a parameter for array length. This 
    is due to a limitation where there's no method to 
    calculate. I can use:
            sizeof(array) / sizeof(array[0]);
    but this is unreliable, especially when the array
    is passed as a parameter or made of Strings.

    Lastly, there is a lack of timer interrupts because I
    could not get it to work. I've tried all I could but 
    the attatched method just isn't called.
*/

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

/**
Creates custom characters
@return null
*/
void makeChar()
{
    /*
        Putting byte arrays here frees up 
        memory when not in use
    */

    byte rightArrow[] = {
        B00000,				// 
        B00000,				// 
        B00100,				//   #
        B00110,				//   ##
        B11111,				// #####
        B00110,				//   ##
        B00100,				//   # 
        B00000				//
    };
    byte leftArrow[] = {
        B00000,				//
        B00000,				//
        B00100,				//   #
        B01100,				//  ##
        B11111,				// #####
        B01100,				//  ##
        B00100,				//	 #
        B00000				//
    };
    byte upArrow[] = {
        B00000,				//
        B00000,				//
        B00100,				//   #
        B01110,				//  ###
        B11111,				// #####
        B00100,				//   #
        B00100,				//   #
        B00000
    };
    byte downArrow[] = {
        B00000,				//
        B00000,				//
        B00100,				//   #
        B00100,				//	 #
        B11111,				// #####
        B01110,				//  ###
        B00100,				//   #
        B00000				//
    };
    byte selectChar[] = {
        B11111,
        B11011,
        B10001,
        B10001,
        B11011,
        B11111,
        B11011,
        B11111
    };
    byte ellipsisChar[] = {
        B00000,
        B00000,
        B00000,
        B00100,
        B00110,
        B00111,
        B00110,
        B00100
    };

    // creating characters
    lcd.createChar(SELECT_ICO, selectChar);
    lcd.createChar(UP_ICO, upArrow);
    lcd.createChar(DOWN_ICO, downArrow);
    lcd.createChar(LEFT_ICO, leftArrow);
    lcd.createChar(RIGHT_ICO, rightArrow);
    lcd.createChar(ELPS_ICO, ellipsisChar);
}

/**
Creates a single 2x2 custom character
@param [uint8_t] direction - Custom 1x1 char value
@return null
*/
void makeLargeChar(uint8_t direction) {

    /*
        lcd.createChar() has a limit of 7 characters
        so a "banking system" is needed.

        This takes the value of a 1x1 char and creates
        4 tiles of its larger version

        It always sets to 0-3, 0 = top left and 
        3 = bottom right

        As the previous character set is overwritten,
        makeChar() must be called again.
    */

    // common chars
    byte stalkVL[] = {
    B00011,
    B00011,
    B00011,
    B00011,
    B00011,
    B00011,
    B00011,
    B00011
    };
    byte stalkVR[] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000
    };
    byte stalkHT[] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111
    };
    byte stalkHB[] = {
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
    };

    // creates character according to the smaller character
    if (direction == DOWN_ICO) {
        byte downR[] = {
            B11000,
            B11000,
            B11111,
            B11110,
            B11100,
            B11000,
            B10000,
            B00000
        };
        byte downL[] = {
            B00011,
            B00011,
            B11111,
            B01111,
            B00111,
            B00011,
            B00001,
            B00000
        };
        lcd.createChar(0, stalkVL);		// overwrites previous characters
        lcd.createChar(1, stalkVR);		// due to limited character storage
        lcd.createChar(2, downL);		// at 7 or 8 chars (can't remember
        lcd.createChar(3, downR);		// must be replaced with makeChars()
    }
    else if (direction == UP_ICO) {
        byte upL[] = {
            B00000,
            B00001,
            B00011,
            B00111,
            B01111,
            B11111,
            B00011,
            B00011
        };
        byte upR[] = {
            B00000,
            B10000,
            B11000,
            B11100,
            B11110,
            B11111,
            B11000,
            B11000
        };
        lcd.createChar(2, stalkVL);
        lcd.createChar(3, stalkVR);
        lcd.createChar(0, upL);
        lcd.createChar(1, upR);
    }
    else if (direction == LEFT_ICO) {
        byte leftT[] = {
            B00000,
            B00000,
            B00000,
            B00001,
            B00011,
            B00111,
            B01111,
            B11111
        };
        byte leftB[] = {
            B11111,
            B01111,
            B00111,
            B00011,
            B00001,
            B00000,
            B00000,
            B00000
        };
        lcd.createChar(0, leftT);
        lcd.createChar(1, stalkHT);
        lcd.createChar(2, leftB);
        lcd.createChar(3, stalkHB);
    }
    else if (direction == RIGHT_ICO) {
        byte rightT[] = {
            B00000,
            B00000,
            B00000,
            B10000,
            B11000,
            B11100,
            B11110,
            B11111
        };
        byte rightB[] = {
            B11111,
            B11110,
            B11100,
            B11000,
            B10000,
            B00000,
            B00000,
            B00000
        };
        lcd.createChar(1, rightT);
        lcd.createChar(0, stalkHT);
        lcd.createChar(3, rightB);
        lcd.createChar(2, stalkHB);
    }
    else if (direction == SELECT_ICO) {
        byte a[] = {
            B11111,
            B10000,
            B10011,
            B10111,
            B10111,
            B10011,
            B10011,
            B10011
        };
        byte b[] = {
            B11111,
            B00001,
            B11001,
            B11101,
            B11101,
            B11001,
            B11001,
            B11001
        };
        byte c[] = {
            B10001,
            B10001,
            B10000,
            B10001,
            B10001,
            B10000,
            B10000,
            B11111
        };
        byte d[] = {
            B10001,
            B10001,
            B00001,
            B10001,
            B10001,
            B00001,
            B00001,
            B11111
        };
        lcd.createChar(0, a);
        lcd.createChar(1, b);
        lcd.createChar(2, c);
        lcd.createChar(3, d);
    }
}

/**
Clears a line at a specified row
@return null
*/
void clearLine(uint8_t row) {
    lcd.setCursor(0, row);
    lcd.print("                ");
    lcd.home();
}

/**
Symbol to Button dictionary
@param [uint8_t] symbol
@return [uint8_t] button
*/
uint8_t symbolTranslator(uint8_t symbol) {
    // This is used in startGame()
    switch (symbol) {
    case UP_ICO		: return BUTTON_UP;
    case DOWN_ICO	: return BUTTON_DOWN;
    case LEFT_ICO	: return BUTTON_LEFT;
    case RIGHT_ICO	: return BUTTON_RIGHT;
    case SELECT_ICO	: return BUTTON_SELECT;
    default			: return 0;
    }
}

/**
Button to Symbol dictionary
@return [uint8_t] Symbol
*/
uint8_t buttonTranslator() {
    // This is used in startGame()
    switch (lcd.readButtons()) {
    case BUTTON_DOWN	: return DOWN_ICO;
    case BUTTON_LEFT	: return LEFT_ICO;
    case BUTTON_RIGHT	: return RIGHT_ICO;
    case BUTTON_SELECT	: return SELECT_ICO;
    case BUTTON_UP		: return UP_ICO;
    default				: return 0;
    }
}

/**
Prints a single two lined "message box"
@param [String] top - 16 characters
@param [String] bottom - 15 characters (16th reserved)
@return null
*/
void printMessage(String top, String bottom) {
    lcd.clear();

    lcd.home();
    lcd.print(top);

    lcd.setCursor(0, 1);
    lcd.print(bottom);

    lcd.setCursor(15, 1);
    lcd.write(ELPS_ICO);
    delay(1000);
}

/**
Prints a String array as a two lined message box

@param [const String*] lines - Array of messages
@param [uint8_t] length - Length of the array
*/
void messageBox(const String lines[], uint8_t length) {

    /*
        This was created for use with a story line
        but was never realised due to hardware 
        restrictions. Therefore, this is only used 
        for the tutorial messages.
    */

    lcd.clear();

    // First print has to be outside
    // do-while may introduce problems
    printMessage(lines[0], lines[1]);
    uint8_t lineIndex = 2;

    while (lineIndex < length) {

        // Prints next line when buttons are pressed
        if (lcd.readButtons() == BUTTON_SELECT || lcd.readButtons() == BUTTON_RIGHT) {
            printMessage(lines[lineIndex], lines[lineIndex + 1]);
            lineIndex += 2;
        }
    }

    delay(1000);

    // Waits for last button press
    while (lcd.readButtons() != BUTTON_SELECT && lcd.readButtons() != BUTTON_RIGHT);
}

/**
Flashes backlight between two colours with a specified delay
@param [uint8_t] from - First colour to switch to
@param [uint8_t] to - Second colour to switch to
@param [short] d - delay
@return null
*/
void flashBackLight(uint8_t from, uint8_t to, short d) {
    lcd.setBacklight(from);
    delay(d);
    lcd.setBacklight(to);
}

/*
Displays splash/start screen
@return null
*/
void splash()
{
    // Prints title
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Big Brain Game"));

    makeChar();		// make custom chars

    // Prints "Press !"
    lcd.setCursor(4, 1);
    lcd.print(F("Press"));
    lcd.setCursor(10, 1);
    lcd.write(SELECT_ICO);

    while (true) {
        // move on when start is pressed
        if (lcd.readButtons() == BUTTON_SELECT) {
            flashBackLight(GREEN, WHITE, 500);
            break;
        }
    }
}

/**
Multi-purpose menu display.
@param [String] menuTitle
@param [String[]] menuOptions
@param [uint8_t] arrayLength
@return [uint8_t] Menu options selected.
*/
uint8_t menu(String menuTitle, const String menuOptions[], uint8_t arrayLength) {

    volatile uint8_t menuIndex = 0;		// index of selected item

    // prints first item outside loop
    // do-while might cause issues
    lcd.clear();
    lcd.print("< " + menuOptions[menuIndex]);
    lcd.setCursor(15, 0);
    lcd.print(">");

    lcd.setCursor(0, 1);
    lcd.print(menuTitle);

    while (true) {
        switch (lcd.readButtons()) {
        case BUTTON_LEFT:
            // Prevents index from going below zero
            (menuIndex > 0 ? menuIndex-- : 0);

            clearLine(0);
            lcd.print("< " + menuOptions[menuIndex]);
            lcd.setCursor(15, 0);
            lcd.print(">");

            delay(250);
            break;

        case BUTTON_RIGHT:
            // prevents index from going above length
            (menuIndex < (arrayLength - 1) ? menuIndex++ : arrayLength - 1);

            clearLine(0);
            lcd.print("< " + menuOptions[menuIndex]);
            lcd.setCursor(15, 0);
            lcd.print(">");

            delay(250);
            break;

        case BUTTON_SELECT:
            flashBackLight(GREEN, WHITE, 500);
            lcd.clear();
            return menuIndex;		// returns selected option
        }
    }
}

/**
Tutorial messages
@return null
*/
void tutorial() {
    // Message String array
    // F() is stored in Flash
    const String lines[] = {
        F("Hey! Welcome to"),
        F("the tutorial!"),

        F("This is a game"),
        F("where you press"),

        F("directional bu-"),
        F("ttons to match"),

        F("a sequence."),
        F(""),

        F("Watch out!"),
        F("There's a time"),

        F("limit, so be"),
        F("fast!"),

        F("Press the right"),
        F("button and the"),

        F("timer will"),
        F("reset!"),

        F("There are two"),
        F("modes to play:"),

        F("Story and"),
        F("Free Play."),

        F("Story mode have"),
        F("you go through"),

        F("levels with inc-"),
        F("reasing diffic-"),

        F("ulty. Free play"),
        F("is an infinite"),

        F("practice mode."),
        F("That's all!")
    };
    messageBox(lines, 28);
}

/**
Menu for selecting freeplay difficulty
@return null
*/
void freePlay() {
    const String difficulties[] = { F("Easy"), F("Normal"), F("Hard"), F("Big Brain") };
    const String infMode = F("Infinite Mode");
    lcd.clear();

    randomSeed(analogRead(0));

    uint8_t seq;				// sequence length
    uint8_t chars;				// no. of chars used

    switch (menu(F("Difficulty"), difficulties, 4)) {
    case 0:
        lcd.print(difficulties[0]);
        lcd.setCursor(0, 1);
        lcd.print(infMode);
        flashBackLight(GREEN, WHITE, 1000);

        do {
            // Variability in parameters for 
            // I M M E R S I V E 
            // gameplay
            seq = random(3, 6);		// 3-5 (Personal notes)
            chars = random(2, 4);	// 2-3

        } while (startGame(seq, chars, 1000, 2500));	// while not losing
        break;

    case 1:
        lcd.print(difficulties[1]);
        lcd.setCursor(0, 1);
        lcd.print(infMode);
        flashBackLight(YELLOW, WHITE, 1000);

        do {
            seq = random(5, 8);		// 5-7
            chars = random(3, 5);	// 3-4
        } while (startGame(seq, chars, 1000, 1500));
        break;

    case 2:
        lcd.print(difficulties[2]);
        lcd.setCursor(0, 1);
        lcd.print(infMode);
        flashBackLight(RED, WHITE, 1000);

        do {
            seq = random(7, 11);	// 7-10
            chars = random(4, 6);	// 4-5
        } while (startGame(10, 5, 750, 1250));
        break;

    case 3:
        lcd.print(difficulties[3]);
        lcd.setCursor(0, 1);
        lcd.print(infMode);
        flashBackLight(VIOLET, WHITE, 1000);

        do {
            seq = random(10, 15);	// 10-14
        } while (startGame(seq, 5, 500, 1000));
        break;
    }
}

/**
Story mode gameplay
@return null
*/
void story() {
    const String difficulties[] = { F("Easy"), F("Normal"), F("Hard"), F("Big Brain") };

    // difficulty parameters
    uint8_t seqLength;
    uint8_t seqMin;
    uint8_t seqMax;
    uint8_t charsUsed;
    short d;
    short t;

    switch (menu("Difficulty", difficulties, 4)) {
    case 0:
        seqLength = 3;
        seqMax = 6;
        charsUsed = 2;
        d = 1000;
        t = 2500;
        break;
    case 1:
        seqLength = 5;
        seqMax = 8;
        charsUsed = 3;
        d = 1000;
        t = 1500;
        break;
    case 2:
        seqLength = 7;
        seqMax = 10;
        charsUsed = 4;
        d = 750;
        t = 1250;
        break;
    case 3:
        seqLength = 9;
        seqMax = 14;
        charsUsed = 5;
        d = 500;
        t = 1000;
        break;
    }

    seqMin = seqLength;
    
    for (uint8_t stage = 1; stage <= 5; stage++) {

        // Prints stage progress
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print(F("STAGE   /5"));
        lcd.setCursor(10, 0);
        lcd.print(stage);
        
        // Each stage has a different backlight
        // for memorability I guess...
        flashBackLight(stage, WHITE, 2000);

        for (uint8_t level = 1; level <= 8; level++) {
            for (uint8_t round = 1; round <= 3; round++) {
                // prints level/round progress
                lcd.clear();
                lcd.setCursor(3, 0);
                lcd.print(F("LEVEL   -"));
                lcd.setCursor(10, 0);
                lcd.print(level);
                lcd.setCursor(12, 0);
                lcd.print(round);

                delay(2000);

                if (!startGame(seqLength, charsUsed, d, t)) {
                    // Game Over, return to menu
                    return;
                }
            }

            // only every other level
            if (level % 2 == 0) {
                // increase sequence length if length is less than max
                seqLength = (seqLength == seqMax ? seqMax : seqLength + 1);
            }
        }

        // increase no. of chars used if less than 5
        charsUsed = (charsUsed < 5 ? charsUsed + 1 : 5);

        // decrease peek time by 50ms if d is more than 500ms
        d = ((d - 50) > 500 ? d - 50 : 500);

        // Reset N when M increases
        seqLength = seqMin;
    }

    // Completion message
    lcd.clear();
    lcd.setBacklight(TEAL);
    lcd.print(F("You did it! The"));
    lcd.setCursor(0, 1);
    lcd.print(F("world's saved..."));
    delay(5000);
    lcd.clear();
    lcd.print(F("Or something..."));
    delay(5000);
    lcd.setBacklight(WHITE);
}

/**
Starts one game with specified parameters
@param [uint8_t] seqlength - length of sequence
@param [uint8_t] charsUsed - no. of chars to be used
@param [short] d - peek time
@param [short] t - time for each button press
@return [bool] - win or lose
*/
bool startGame(uint8_t seqLength, uint8_t charsUsed, short d, short t) {

    // Prints parameters to Serial Monitor
    Serial.print(F("Param: N="));
    Serial.print(seqLength);
    Serial.print(F(" chars, M="));
    Serial.print(charsUsed);
    Serial.print(F(" chars, D="));
    Serial.print(d);
    Serial.print(F(" ms, t="));
    Serial.print(t);
    Serial.println(F(" ms."));

    randomSeed(analogRead(0));

    uint8_t sequence[16];

    // Generating and displaying sequence
    for (uint8_t i = 0; i < seqLength; i++) {
        uint8_t sym = random(0, charsUsed);		// generates symbols
        sequence[i] = symbolTranslator(sym);	// maps symbols to button values, store in array
        lcd.clear();
        delay(250);

        makeLargeChar(sym);		// create character according to what symbol is generated
        // write top row
        lcd.setCursor(7, 0);
        lcd.write(0);
        lcd.write(1);
        //write bottom row
        lcd.setCursor(7, 1);
        lcd.write(2);
        lcd.write(3);
        delay(d);			// (D value)
    }

    lcd.clear();
    makeChar();
    lcd.setCursor(2, 0);
    lcd.print(F("Get Ready..."));

    delay(1000);

    lcd.setCursor(6, 1);
    lcd.print(F("GO!!"));

    delay(500);

    lcd.clear();

    uint8_t k = 0;		// sequence progression
    volatile unsigned long time = millis() + t;		// Time to press
    volatile unsigned long prevTime = 0;			// previous ms polled

    lcd.setCursor(0, 1);
    lcd.print(F("     TIME: ####"));				// space reserved for story elements

    while (k < seqLength) {
        if (millis() > time) {			// Run out of time
            lcd.clear();
            lcd.setCursor(3, 0);
            lcd.print(F("Time's Up!"));
            lcd.setCursor(3, 1);
            lcd.print(F("Game Over!"));
            flashBackLight(VIOLET, WHITE, 3000);
            return false;
        }
        else if (lcd.readButtons() == sequence[k]) {	// Pressed the right button

            lcd.setCursor(k, 0);
            lcd.write(buttonTranslator());		// Writes the corresponding symbol
            flashBackLight(GREEN, WHITE, 500);
            k++;

            time = millis() + t;		// Reset timer
            lcd.setCursor(11, 1);
            lcd.print("####");

            continue;
        }
        else if (lcd.readButtons() != 0
            && lcd.readButtons() != sequence[k]) {	// Press wrong button

            lcd.clear();
            lcd.setCursor(4, 0);
            lcd.print(F("Aw Heck!"));
            lcd.setCursor(3, 1);
            lcd.print(F("Game Over!"));
            flashBackLight(RED, WHITE, 3000);
            return false;
        }

        // Polls to update timer
        if (millis() - prevTime > 100) {
            long timeLeft = time - millis();
            prevTime = millis();

            // update timer in quarters
            if (timeLeft <= (1.0 / 4.0) * t) {
                lcd.setCursor(11, 1);
                lcd.print("#---");
            }
            else if (timeLeft <= (1.0 / 2.0) * t) {
                lcd.setCursor(11, 1);
                lcd.print("##--");
            } 
            else if (timeLeft <= (3.0 / 4.0) * t) {
                lcd.setCursor(11, 1);
                lcd.print("###-");
            }
        }
    }

    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print(F("Well Done!"));
    flashBackLight(TEAL, WHITE, 1000);
    return true;
}

void setup()
{
    Serial.begin(115200);
    lcd.begin(16, 2);
    splash();
}

void loop()
{
    const String menuOptions[] = { 
        F("Story Mode"), 
        F("Free Play"), 
        F("Tutorial") 
    };
    
    // main menu and option selection
    switch (menu(F("Main Menu"), menuOptions, 3)) {
    case 0:
        lcd.print(F("Story"));
        story();
        break;
    case 1:
        lcd.print(F("Free Play"));
        freePlay();
        break;
    case 2:
        lcd.print(F("Tutorial"));
        tutorial();
        break;
    }
}


/*
        UNUSED CODE
*/

//struct HighScores
//{
//	char name[3];
//	int score;
//};
//
//struct HighScores hs;

//void hsEntry(uint32_t score) {
//	lcd.clear();
//
//	lcd.print(F("Enter Your Name"));
//	delay(1000);
//
//	lcd.clear();
//	lcd.print(F("   >A   A   A   "));
//
//	lcd.setCursor(0, 1);
//	lcd.write(UP_ICO);
//	lcd.setCursor(1, 1);
//	lcd.write(DOWN_ICO);
//	lcd.setCursor(2, 1);
//	lcd.print(F("=Aa"));
//
//	lcd.setCursor(6, 1);
//	lcd.write(LEFT_ICO);
//	lcd.setCursor(7, 1);
//	lcd.write(RIGHT_ICO);
//	lcd.setCursor(8, 1);
//	lcd.print(F("=SEL"));
//
//	char* name = { 'A', 'A', 'A' };		// Ascii {A, A, A}
//	uint8_t index = 0;
//	uint8_t cursor = 3;
//
//	while (true) {
//		switch (lcd.readButtons()) {
//		case BUTTON_LEFT:
//			lcd.setCursor(cursor, 0);
//			lcd.print(" ");
//
//			(index > 0 ? index-- : 0);
//			(cursor > 3 ? cursor -= 4 : 3);
//
//			lcd.setCursor(cursor, 0);
//			lcd.print(">");
//			delay(500);
//			break;
//		case BUTTON_RIGHT:
//			lcd.setCursor(cursor, 0);
//			lcd.print(" ");
//
//			(index < 2 ? index++ : 2);
//			(cursor < 11 ? cursor += 4 : 11);
//
//			lcd.setCursor(cursor, 0);
//			lcd.print(">");
//			delay(500);
//			break;
//		case BUTTON_UP:
//			name[index] = (name[index] > 65 ? name[index] - 1 : 90);
//			lcd.setCursor(cursor + 1, 0);
//			lcd.write(name[index]);
//			delay(250);
//			break;
//		case BUTTON_DOWN:
//			name[index] = (name[index] < 90 ? name[index] + 1 : 65);
//			lcd.setCursor(cursor + 1, 0);
//			lcd.write(name[index]);
//			delay(250);
//			break;
//		case BUTTON_SELECT:
//			Serial.print(name);
//			Serial.println(score);
//			return;
//		}
//	}
//}

//void hsEntry(int score) {
//	lcd.clear();
//
//	lcd.print(F("Enter name:"));
//	lcd.setCursor(0, 1);
//	lcd.print(F(">A  A  A"));
//
//	uint8_t index = 0;
//	char name[] = "AAA";
//
//	while (true) {
//		switch (lcd.readButtons()) {
//		case BUTTON_LEFT:
//			lcd.setCursor(index * 3, 1);
//			lcd.print(" ");
//
//			(index > 0 ? index-- : index = 0);
//			lcd.setCursor(index * 3, 1);
//			lcd.print(">");
//			delay(500);
//			break;
//
//		case BUTTON_RIGHT:
//			lcd.setCursor(index * 3, 1);
//			lcd.print(" ");
//
//			(index < 2 ? index++ : index = 2);
//			lcd.setCursor(index * 3, 1);
//			lcd.print(">");
//			delay(500);
//			break;
//
//		case BUTTON_DOWN:
//			if (name[index] < 'Z') {
//				name[index]++;
//			}
//			else {
//				name[index] = 'A';
//			}
//
//			lcd.setCursor((index * 3) + 1, 1);
//			lcd.print(name[index]);
//			delay(500);
//			break;
//
//		case BUTTON_UP:
//			if (name[index] > 'A') {
//				name[index]--;
//			}
//			else {
//				name[index] = 'Z';
//			}
//
//			lcd.setCursor((index * 3) + 1, 1);
//			lcd.print(name[index]);
//			delay(500);
//			break;
//
//		case BUTTON_SELECT:
//			Serial.print(index);
//			Serial.println(name);
//
//			HighScoresTable hs;
//
//			EEPROM.get(1, hs);
//
//			for (size_t i = 0; i < 9; i++) {
//				if (hs.table[i].score < score) {
//					strcpy(hs.table[i].name, name);
//					hs.table[i].score = score;
//				}
//			}
//
//			EEPROM.put(1, hs);
//
//			delay(500);
//			break;
//		}
//	}
//}

// Not implemented due to SRAM restrictions
/*const String dialogue1[16] = {
    F("Well, hello"),
    F("there..."),
    F("Nice seeing your"),
    F("face again."),
    F("Don't recognise"),
    F("me? Oh well,"),
    F("that doesn't"),
    F("really matter."),
    F("All you need to"),
    F("know is to just"),
    F("must match the"),
    F("sequence shown."),
    F("Do it 8 times"),
    F("then come back."),
    F("Now get to it."),
    F("ZEHAHAHA!")
};

const String dialogue2[16] = {
    F("Hey, Hey! You"),
    F("did it."),
    F("What? It was too"),
    F("easy? ZEHAHAHA!"),
    F("Okay, I'll make"),
    F("it harder, you"),
    F("masochist!"),
    F(""),
    F("Hm? What exactly"),
    F("are you doing?"),
    F("Don't worry"),
    F("about it..."),
    F("Gah! So many"),
    F("questions!"),
    F("I'll tell you"),
    F("later, promise.")
};

const String dialogue3[16] = {
    F("(DISTANT)"),
    F("ZEHAHAHA! Soon,"),
    F("my AI will"),
    F("solve all of"),
    F("the world's"),
    F("CAPTCHAs! ZEHAHA"),
    F("THIS AI WILL"),
    F("BREAK THE WEB"),
    F("AND G**GLE WILL"),
    F("BE AT MY MERCY!"),
    F("ZEHAHAHAHA!!!"),
    F("..."),
    F("Hopefully none"),
    F("of my enslaved"),
    F("workforce heard"),
    F("this monologue.")
};

const String dialogue4[16] = {
    F(""),
    F("")
};

const String dialogue5[16] = {
    F(""),
    F("")
};

const String* diaList[5] = {
    dialogue1,
    dialogue2,
    dialogue3,
    dialogue3,
    dialogue3
};*/
