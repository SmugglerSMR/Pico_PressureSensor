#include <Arduino.h>
#include <SPI.h>

#define BIT_VALUE(reg, pin)      (((reg) >> (pin)) & 1)
#define BIT_IS_SET(reg, pin)    (BIT_VALUE((reg),(pin))==1)
#define REPEAT_CMD  10

double press_counts = 0;          // digital preasure reading [counts]
double pressure     = 0;          // preassure reading [psi]
double outputmax    = 15099494;   // output at maximum pressure [counts]
double outputmin    = 1677722;    // output at minimum pressure [counts]
double pmax         = 1;          // maximum value of pressure range 
                                  //    [psi]
double pmin         = 0;          // minimum value of pressure range
                                  //    [psi]
double percentage   = 0;          // holds percentage of full scale data
char printBuffer[200], cBuff[40], percBuff[40]  , pBuff[40];



void setup() {
    Serial.begin(115200);
    while(!Serial) {
        delay(10);
    }
    sprintf(printBuffer,
"\n24-bit Sensor data, Digital Pressure Counts, Percentage of full scale pressure, Pressure\r\n");
    Serial.println(printBuffer);
    SPI.begin();
    pinMode(10, OUTPUT);          // pin 10 as SS
    digitalWrite(10, HIGH);       // set SS High
    SPI.beginTransaction(   //SPI at 50kHz     Min-50 Max-800kHz
        SPISettings(50000, MSBFIRST, SPI_MODE1)
    );
}

void loop() {
    delay(1000);
    // holds output data
    uint8_t data[4] = {0xF0, 0x00, 0x00, 0x00};
    uint8_t cmd[3] = {0xAA, 0x00, 0x00}; // command to be sent

    digitalWrite(10, LOW);      // set SS Low
    SPI.transfer(cmd, 3);       // send Read Command
    digitalWrite(10, HIGH);     // set SS High
    if(!BIT_IS_SET(cmd[0], 6)){
      sprintf(printBuffer,
        "Device is not powered");
      Serial.println(printBuffer);
    }

    digitalWrite(10, LOW);
    uint8_t stat = SPI.transfer(0xF0);
    digitalWrite(10, HIGH);

    int counter = 0;
    while(BIT_IS_SET(stat, 5)) {
      if (counter >= REPEAT_CMD){
        sprintf(printBuffer,
          "Unrecovered. Reseting CMD");
        Serial.println(printBuffer);
        delay(1000);
        cmd[0] = 0xAA;
        digitalWrite(10, LOW);
        SPI.transfer(cmd, 3);
        digitalWrite(10, HIGH);
        counter = 0;
      } else {
        counter++;
        digitalWrite(10, LOW);
        stat = SPI.transfer(0xF0);
        digitalWrite(10, HIGH);
        delay(5);
      }
    }
    
    delay(50);
    digitalWrite(10, LOW);
    SPI.transfer(data, 4);
    digitalWrite(10, HIGH);

//    SPI.endTransaction();
    sprintf(printBuffer, "%x",
                         data[0]);
    Serial.print(printBuffer);
    
    if(BIT_IS_SET(data[0], 2)){
      sprintf(printBuffer,
        "!! Integrity Test Failed");
      Serial.println(printBuffer);
    } 
    {
      if(BIT_IS_SET(data[0], 0)){
        sprintf(printBuffer,
          "!! Math Saturation\r\n");
        Serial.println(printBuffer);
      } else {
        // calculate digital pressure counts
        press_counts = data[3] + data[2] * 256 + data[1] * 65536;
        // calculate pressure as percentage of full scale
        percentage = (press_counts / 16777215) * 100;
    
        //calculation of pressure value according to equation 2 of datasheet
        pressure = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;
        dtostrf(press_counts, 5, 0, cBuff);
        dtostrf(percentage, 5, 4, percBuff);
        dtostrf(pressure, 6, 5, pBuff);
        
        /*
        The below code prints the raw data as well as the processed data.
        Data format : Status Register, 24-bit Sensor Data, Digital Counts,
                      percentage of full scale pressure, pressure output,
                      temperature.
        */
        sprintf(printBuffer, "\t%2x %2x %2x\t%s\t%s\t\t%s \r\n",
                             data[1], data[2], data[3],
                             cBuff, percBuff, pBuff);
        Serial.print(printBuffer);
      }
    }
    
}
