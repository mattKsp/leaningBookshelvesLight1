/*----------------------------notes----------------------------*/
/*
 * Wemos D32 (and Pro), 80 MHz (..says 240 on the box?), 4M
 * https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
 * 
 * Pinout Wemos D32 (ESP32-WROOM)
 * 
 * 36 - VP (RTC_GPIO0 interrupt)
 * 39 - VN
 * 34 - ADC1_CH6
 * 32 - ADC1_CH4 / TFT_LED
 * 33 - ADC1_CH5 / TFT_RST
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
 * 17 - NC
 * 16 - NC
 * 4
 * 0
 * 2
 * 15
 * 
 * Pinout Wemos D32 Pro (ESP32-WROVER)
 * 
 * 36 - VP (RTC_GPIO0 interrupt)
 * 39 - VN
 * 34 - ADC1_CH6
 * 32 - ADC1_CH4 / TFT_LED
 * 33 - ADC1_CH5 / TFT_RST
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
 * NC
 * NC
 * 4
 * 0
 * 2
 * 15
 * 
 */


/* 
 * WeMos D1 (R2 &) mini, 80 MHz, 115200 baud, 4M, (3M SPIFFS)
 * 
 * Pinout Wemos D1 Mini (ESP-8266)
 * 
 * RX  3
 * TX  1
 * A0  Analog input, max 3.2V input  A0
 * D0  16  IO  GPIO16   - no PWM or I2C or interrupt
 * D1  5  IO, SCL GPIO5  I2C
 * D2  4  IO, SDA GPIO4  I2C
 * D3  0  IO, 10k Pull-up GPIO0
 * D4  2  IO, 10k Pull-up, BUILTIN_LED  GPIO2
 * D5  14  IO, SCK GPIO14
 * D6  12  IO, MISO  GPIO12
 * D7  13  IO, MOSI  GPIO13
 * D8  15  IO, 10k Pull-down, SS GPIO15
 * 
 * 
 * Grove - 12 Key Capacitive I2C Touch Sensor V2 (MPR121)
 * http://wiki.seeedstudio.com/Grove-12_Key_Capacitive_I2C_Touch_Sensor_V2-MPR121/
 * 
 * 
 * https://hackaday.com/2017/01/20/cheating-at-5v-ws2812-control-to-use-a-3-3v-data-line/
 * 
 */

/*
// Example 5 - Receive with start- and end-markers combined with parsing

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char messageFromPC[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;

boolean newData = false;

//============

void setup() {
    Serial.begin(9600);
    Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
    Serial.println("Enter data in this style <HelloWorld, 12, 24.7>  ");
    Serial.println();
}

//============

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    integerFromPC = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    floatFromPC = atof(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
    Serial.print("Message ");
    Serial.println(messageFromPC);
    Serial.print("Integer ");
    Serial.println(integerFromPC);
    Serial.print("Float ");
    Serial.println(floatFromPC);
}

*/

//..

