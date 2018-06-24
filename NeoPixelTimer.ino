/***************************
 * Game and Sleep Timer for my daugther 
 * 
 * with a Button you can switch between different modes
 * 
 * Mode 0: Sleep / Standby   
 *   - a Red Dot cycles arround with 1 Pixel / minute
 * Mode 1: Play Timer
 *   - play for 24 minutes
 *   - All Pixel violett (PLAY_COLOR) at beginning
 *   - each minute (PLAY_DELAY) one LED is switched off
 *   - after 24 Minutes switches to Mode 4 (End of Play)
 *   - on button press: switch to mode 2
 * Mode 2: Toothbrush Timer
 *   - toothbrushing for four Minutes
 *   - All Pixel white (TOOTH_COLOR) at beginning
 *   - each 10 sec (TOOTH_DELAY) one LED is switched off
 *   - after 2 Minutes switches to Mode 5 (End of Toothbrush)
 *   - on button press: switch to mode 3
 * Mode 3: Read Timer
 *   - reading for 24 Minutes
 *   - All Pixel green (READ_COLOR) at beginning
 *   - each minute (READ_DELAY) one LED is switched off
 *   - after 24 Minutes switches to Mode 6 (End of Read)
 *   - on button press: switch to mode 0
 * Mode 4: End Of Play
 *   - blink all LEDs dark violett (PLAY_COLOR_END)
 *   - until Button is pressed, then switch to mode 2
 * Mode 5: End Of Toothbrush)
 *   - blink all LEDs dark white (TOOTH_COLOR_END)
 *   - until Button is pressed, then switch to mode 3
 * Mode 6: End Of Read
 *   - blink all LEDs dark white (READ_COLOR_END)
 *   - until Button is pressed 
 *   - until Button is pressed, then switch to mode 0
 *   
 * Hardware:  
 * - Arduino Nano
 * - Button shorts GND       D2
 * - 24 LED Neopixel Ring    D12
 */

/***************************
 * External Libs
 **************************/
#include <NeoPixelBus.h>      // from: https://github.com/Makuna/NeoPixelBus
#include <Bounce2.h>          // from: https://github.com/thomasfredericks/Bounce2

/***************************
 * Hardware Configuration 
 **************************/
#define WS2812PIN         12
#define LEDCOUNT          24 
#define BUTTON             2
#define DEBUG_SPEEDUP      1 // 1:normal Speed, 10: speedup by 10, ....
#define SERIAL_BAUD   115200
#define MAXBRIGHTNESS     50
#define MAXENDBRIGHTNESS  10

/***************************
 * Definition for each Mode
 **************************/
// Sleep/Standby (blue Pixel changes every minute)
#define SLEEP_COLOR         RgbColor(1, 0, 0)
#define SLEEP_DELAY         60000 / DEBUG_SPEEDUP
// Playing
#define PLAY_COLOR          RgbColor(MAXBRIGHTNESS, 0, MAXBRIGHTNESS)
#define PLAY_COLOR_END      RgbColor(MAXENDBRIGHTNESS, 0, MAXENDBRIGHTNESS)
#define PLAY_DELAY          60000 / DEBUG_SPEEDUP
// Tooth Brushing
#define TOOTH_COLOR         RgbColor(MAXBRIGHTNESS, MAXBRIGHTNESS, MAXBRIGHTNESS)
#define TOOTH_COLOR_END     RgbColor(MAXENDBRIGHTNESS, MAXENDBRIGHTNESS, MAXENDBRIGHTNESS)
#define TOOTH_DELAY         10000 / DEBUG_SPEEDUP
// Reading
#define READ_COLOR          RgbColor(0, MAXBRIGHTNESS, 0)
#define READ_COLOR_END      RgbColor(0, MAXENDBRIGHTNESS, 0)
#define READ_DELAY          60000 / DEBUG_SPEEDUP
// Endo of one Program
#define END_DELAY           500 

/***************************
 * global vars 
 **************************/
unsigned long g_curStep;               // Step Count for current Animation
unsigned long g_curDelay;              // Step Delay for current Animation
unsigned long g_lastMillis;            // Time of last Animation Update
byte          g_curAnim;               // Current running Animation
byte          g_newAnim;               // switch to this Animation next Loop
Bounce        g_Button = Bounce();     // Button Debouncer
// NeoPixel
NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> neo(LEDCOUNT, WS2812PIN);
// NeoGamma      <NeoGammaTableMethod> colorGamma; 


/***************************
 * Initialize Serial Port
 **************************/
void initSerial(void) {
    Serial.begin(SERIAL_BAUD);      
    Serial.println("Spiel und Schlafuhr");
    Serial.println("===================");
    Serial.print("Init Serial... ");
    delay(500);
    Serial.println("done."); 
}

/***************************
 * Initialize Neopixel
 **************************/
void initLed(void) {
    Serial.print("Init Neopixel... ");
    neo.Begin();
    neo.Show();
    Serial.println("done."); 
}

/***************************
 * Initialize Global Vars
 **************************/
void initGlobalVars(void) {
    Serial.print("Init Global Vars... ");
    g_curStep=0;
    g_curDelay=1000;
    g_curAnim=0;
    g_newAnim=0;       
    g_lastMillis=0;
    Serial.println("done."); 
}

/***************************
 * Initialize Button
 **************************/
void initButton(void) {
    Serial.println("Init Button... ");
    // pinMode(BUTTON,INPUT_PULLUP);
    g_Button.attach(BUTTON, INPUT_PULLUP);
    Serial.println("done."); 
}


/***************************
 * Setup
 **************************/
void setup()
{
    initSerial();
    initLed();
    initGlobalVars();    
    initButton();
}



/****************************************
 * Animation Update: Sleep/Standby
 * - Cycle: One LED every minute
 * - Color: blue
 ***************************************/
void a_Update_Sleep(){
    g_curDelay = SLEEP_DELAY;    
    neo.ClearTo(RgbColor(0, 0, 0));
    neo.SetPixelColor(g_curStep, SLEEP_COLOR);
    // next step
    g_curStep++;
    // after 24 steps: restart
    if (g_curStep == LEDCOUNT){
        g_curStep=0;
    }
}

/****************************************
 * Animation Update: Playing
 * - Timer: 24min (one LED every minute)
 * - Color: violett
 ***************************************/
void a_Update_Play(){
    g_curDelay = PLAY_DELAY;
    if (g_curStep==0){
        // turn on all LEDs
        neo.ClearTo(PLAY_COLOR);        
    } else {
        // switch off one LEDs each Step
        neo.SetPixelColor(g_curStep, RgbColor(0, 0, 0));
    } 
    // next step
    g_curStep++;
    // after 24 steps - Next Program: Sleep    
    if (g_curStep == LEDCOUNT){
        g_newAnim = 4;
    }
}

/****************************************
 * Animation Update: Tooth Brushing
 * - Timer: 4min (one LED every 10 seconds)
 * - Color: white
 ***************************************/
void a_Update_Tooth(){
    g_curDelay = TOOTH_DELAY;
    if (g_curStep==0){
        // turn on all LEDs
        neo.ClearTo(TOOTH_COLOR);
    } else {
        // dim one LEDs each Step
        neo.SetPixelColor(g_curStep, RgbColor(0, 0, 0));
    } 
    // next step
    g_curStep++;
    // after 24 steps - Next Program: Sleep    
    if (g_curStep == LEDCOUNT){
        g_newAnim = 5;
    }
}

/****************************************
 * Animation Update: Reading
 * - Timer: 24min (one LED every minute)
 * - Color: green
 ***************************************/
void a_Update_Read(){
    g_curDelay = READ_DELAY;
    if (g_curStep == 0){
        // turn on all LEDs
        neo.ClearTo(READ_COLOR);
    } else {
        // dimm one LEDs each Step
        neo.SetPixelColor(g_curStep, RgbColor(0, 0, 0));
    } 
    // after 24 steps - Next Program: Sleep
    if (g_curStep++ == LEDCOUNT){
        g_newAnim = 6;
    }
}

/****************************************
 * Animation Update: End of Animation
 * - Blink all LEDs every second (forever) 
 ***************************************/
void a_Update_End(RgbColor c){
    g_curDelay = END_DELAY;
    if (g_curStep==0){
        // turn on all LEDs
        neo.ClearTo(c);
    } else {
        neo.ClearTo(RgbColor(0, 0, 0));        
    } 
    // next step
    g_curStep++;
    // after 24 steps - Next Program: Sleep    
    if (g_curStep == 2){
        g_curStep = 0;
    }
}
   
void animationUpdate(){
    switch (g_curAnim) {
        case 1:
            a_Update_Play();
            break;
        case 2:
            a_Update_Tooth();
            break;
        case 3:
            a_Update_Read();
            break;
        case 4:
            a_Update_End(PLAY_COLOR_END);
            break;
        case 5:
            a_Update_End(TOOTH_COLOR_END);
            break;
        case 6:
            a_Update_End(READ_COLOR_END);
            break;
        default:
            a_Update_Sleep();
            break;
    }
}

/***************************
 * Main Loop
 **************************/
void loop()
{  
    unsigned long thisDelay;
    
    // Force Update if Button Pressed
    boolean forceUpdate = false;
           
    // Check if Button Pressed      
    g_Button.update();    
    if (g_Button.fell()) {                    
        // Next Programm        
        switch (g_curAnim) {
            case 0: // Standby/Sleep  -> Play 
                g_newAnim = 1; 
                break;  
            case 1: // Play           -> Toothbrush
                g_newAnim = 2; 
                break;  
            case 2: // Toothbrush     -> Read
                g_newAnim = 3; 
                break;  
            case 3: // Read           -> Standby/Sleep 
                g_newAnim = 0; 
                break;  
            case 4: // End_Play       -> Toothbrush
                g_newAnim = 2; 
                break;  
            case 5: // End_Toothbrush -> Read
                g_newAnim = 3; 
                break;  
            case 6: // End_Read       -> Standby/Sleep 
                g_newAnim = 0; 
                break;  
            default: 
                g_newAnim = 0; 
                break; 
        }
    }
    
    // Animation changed?
    if (g_curAnim != g_newAnim ) {
        g_curAnim = g_newAnim; 
        g_curStep = 0;
        g_curDelay = 1000;
        forceUpdate = true;
        Serial.print("New Animation: ");
        Serial.println(g_curAnim );
    }

    // Update Current Animation if due
    if ( ((millis() - g_lastMillis) > g_curDelay) || forceUpdate) {        
        g_lastMillis = millis();
        animationUpdate();
        neo.Show();
    }    
}
