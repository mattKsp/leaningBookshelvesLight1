/*----------------------------notes----------------------------*/

/*----------------------------board notes----------------------------*/
// Wemos D32 (and Pro), 80 MHz (..says 240 on the box?), 4M
// Serial baud 115200
// https://randomnerdtutorials.com/esp32-pinout-reference-gpios/

// 5v regulated power is connected to USB (VBUS)

/* Pinout Wemos D32 (ESP32-WROOM)
 * 
 * 36 - VP (Analog Input) (RTC_GPIO0 interrupt 0)
 * 39 - VN (Analog Input) (RTC_GPIO0 interrupt 3)
 * 34 - ADC1_CH6 (Analog Input)
 * 32 - ADC1_CH4 / TFT_LED (Analog Input)
 * 33 - ADC1_CH5 / TFT_RST (Analog Input)
 * 25 - DAC_1 (Analog Output)
 * 26 - DAC_2 (Analog Output)
 * 27 - TFT_DC
 * 14 - TFT_CS
 * 12 - TS_CS
 * 13
 * 
 * 23 - MOSI
 * 22 - SCL
 * 1 - TX
 * 3 - RX
 * 21 - SDA
 * 19 - MISO
 * 18 - SCK
 * 5 - SS
 * (17) NC
 * (16) NC
 * 4
 * 0
 * 2
 * 15
 * 
 */

/*----------------------------pins notes----------------------------*/
// 5v regulated power is connected to USB (VBUS).
// I2C (for touch) is connected through a dedicated connector.

/*----------------------------touch sensor notes----------------------------*/
// Grove - 12 Key Capacitive I2C Touch Sensor V2 (MPR121)
// http://wiki.seeedstudio.com/Grove-12_Key_Capacitive_I2C_Touch_Sensor_V2-MPR121/

/*----------------------------LED notes----------------------------*/
// https://hackaday.com/2017/01/20/cheating-at-5v-ws2812-control-to-use-a-3-3v-data-line/
 
// might limit power draw even further if add usb charge ports to the system
// or use usb chips and change power draw if usb device attached and charging
// control board 500mA
// 39 x 0.02A = 0.78A x 3 = 2.34 x 3 = 7.02
// 26 x 0.02A = 0.52A x 3 = 1.56
// 7.02 + 1.56 = 8.58 Amp draw for all current lights on full white. Adjust when adding extra strips
