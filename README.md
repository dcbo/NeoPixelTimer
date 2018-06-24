# NeoPixelTimer
Game and Sleep Timer for my daugther with a Button to switch between different modes

# Modes
Mode 0: Sleep / Standby   
- a Red Dot cycles arround with 1 Pixel / minute

## Mode 1: Play Timer
- play for 24 minutes
- All Pixel violett (PLAY_COLOR) at beginning
- each minute (PLAY_DELAY) one LED is switched off
- after 24 Minutes switches to Mode 4 (End of Play)
- on button press: switch to mode 2

## Mode 2: Toothbrush Timer
- toothbrushing for four Minutes
- All Pixel white (TOOTH_COLOR) at beginning
- each 10 sec (TOOTH_DELAY) one LED is switched off
- after 2 Minutes switches to Mode 5 (End of Toothbrush)
- on button press: switch to mode 3

## Mode 3: Read Timer
- reading for 24 Minutes
- All Pixel green (READ_COLOR) at beginning
- each minute (READ_DELAY) one LED is switched off
- after 24 Minutes switches to Mode 6 (End of Read)
- on button press: switch to mode 0

## Mode 4: End Of Play
- blink all LEDs dark violett (PLAY_COLOR_END)
- until Button is pressed, then switch to mode 2

## Mode 5: End Of Toothbrush)
- blink all LEDs dark white (TOOTH_COLOR_END)
- until Button is pressed, then switch to mode 3

## Mode 6: End Of Read
- blink all LEDs dark white (READ_COLOR_END)
- until Button is pressed 
- until Button is pressed, then switch to mode 0
   
# Hardware:  
- Arduino Nano
- Button shorts GND       D2
- 24 LED Neopixel Ring    D12
 
