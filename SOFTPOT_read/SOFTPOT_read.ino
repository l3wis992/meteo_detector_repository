/* MCP 3424 version 1.2 example sketch OneShotConversion
 Written by B@tto
 Contact : batto@hotmail.fr

 In this example, one conversion per second is performed on channel 1 and 16 bits resolution.
 A new conversion has to be initiated by the user
 */


 #include <Wire.h>
 #include <MCP342x.h>


 uint8_t address = 0x6E;
 MCP342x adc(address);  // Declaration of MCP3424 with Address of I2C

 long Voltage;

 void setup(){
   Serial.begin(9600);
   adc.convert(1,0,16,1); // Channel 1, 16 bits resolution, one-shot mode, amplifier gain = 1

  
  }

 }

 void loop(){

   adc.NewConversion();    // New conversion is initiated

   Voltage=MCP.Measure();  // Measure, note that the library waits for a complete conversion

   Serial.print("Voltage = ");
   Serial.print(Voltage);
   Serial.println("uV");  // unit: microVolt

   delay (1000);

 }
