// Lewis Hyman - Arduino Write Speed Tester
double fileVersion = 0.40;


#include <SD.h>
#include <SPI.h>
#include <MCP342x.h>
#include <Wire.h>

const int chipSelect = BUILTIN_SDCARD; //declare SD socket

//Declare ADC
uint8_t address = 0x6E;
MCP342x adc = MCP342x(address);
MCP342x::Config config(MCP342x::channel1, MCP342x::continous, MCP342x::resolution18, MCP342x::gain1);
MCP342x::Config status;


//methods:
// testWrite(char 'type', int testLength, bool 'testRead')
// type is a character, 'L', 'I', 'C' for Long, Integer or Character.
// testlength is amount of data per test in kB
// testRead will run a read speed test
// returns total time for test.

File newFile;
char serialInput = 0;
String inputStr = "";

boolean startConversion = false;

void setup() {
  serialInit();
  Wire.begin();
  SDinit();

  MCP342x::generalCallReset();
  delay(1); //allow 300us to reset ADC
  checkWire();
  startConversion = true;

  testWrite('A', 1000, false);


  testAll(5000);
}

void loop() {
}

void checkWire() {
  Wire.requestFrom(address, (uint8_t)1);
  if (!Wire.available()) {
    Serial.print("No device found at address ");
    Serial.println(address, HEX);
    while (1)
      ;
  }
}
void serialInit() {
  Serial.begin(9600);
  while (!Serial) {
    ; //wait for serial port
  }
}

void testAll(int writeSize) {
  //testWrite('C', writeSize, false);
  //testWrite('S', writeSize, false);
  //testWrite('I', writeSize, false);
  //testWrite('D', writeSize, false);

  Serial.println("\nAll tests completed.");
}

void SDinit() {
  Serial.print((String)"Initialising SD write test V" + fileVersion + " . . . ");

  if (SD.begin(chipSelect)) {
    Serial.println("Succeeded");
  } else {
    Serial.println("Failed - Aborting");
    while (true);
  }
}

long testWrite(char type, int writeSize, boolean readTest) {
  newFile = SD.open("test.txt", FILE_WRITE);

  int testLength = -1;
  int dataSize = -1;

  long initTimer = micros();
  Serial.print((String)"\nWriting " + writeSize + "kB of datatype ");
  switch (type) {

    case 'S': {
        short randShort = random();
        dataSize = sizeof(short);
        testLength = writeSize * 1000 / dataSize;
        Serial.println((String)"Short (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.write(randShort);
        }
        break;
      }

    case 'I': {
        int randInt = random();
        dataSize = sizeof(int);
        testLength = writeSize * 1000 / dataSize;
        Serial.println((String)"Int (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.write(randInt);
        }
        break;
      }

    case 'C': {
        char c = 'a';
        dataSize = sizeof(char);
        testLength = writeSize * 1000 / dataSize;
        Serial.println((String)"Char (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.write(c);
        }

        break;
      }

    case 'D': {
        double randDouble = random();
        dataSize = sizeof(double);
        testLength = writeSize * 1000 / dataSize;
        Serial.println((String)"Double (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.write(randDouble);
        }


      }
    case 'A': {
        dataSize = sizeof(long);
        testLength = writeSize;
        Serial.println((String)"Long (" + dataSize + " Bytes)");
        int i = 0;

        while (i < testLength) {
          long value = 0;
          uint8_t err;
          if (startConversion) {
            //Serial.println("Convert");
            err = adc.convert(config);

            if (err) {
              Serial.print("Convert error: ");
              Serial.println(err);
            }
            startConversion = false;

          }

          err = adc.read(value, status);
          if (!err && status.isReady()) {
            // For debugging purposes print the return value.
            // Serial.println((String)"Value: " + value);
            // Serial.print("Config: 0x");
            // Serial.println((int)config, HEX);
            // Serial.print((String)"Convert error: " + err);
            // Serial.println(i);
            startConversion = true;
            newFile.write(value);
            i++;
          }
        }
      }
  }

  long endTimer = micros();

  float diffTimer = endTimer - initTimer;
  float timePerByte = diffTimer / (testLength * dataSize);
  Serial.println((String)"Time: " + (diffTimer / 1000000) + "s, Total writes: " + testLength + " (" + (double)testLength * dataSize / 1000 + "kB)");
  Serial.println((String)"Time per Write: " + (diffTimer / testLength) + " uS, (" + timePerByte + " uS per byte)");
  Serial.println((String) "Transfer rate: "  + (1000000 / (timePerByte * 1000)) + "kB/S");

  if (newFile) {
    newFile.close();
  } else {
    Serial.println("Can't open file to close it");
    return -1;
  }

  if (readTest) {
    if (writeSize <= 10) {
      if (newFile) {
        Serial.println("Reading test.txt:");
        while (newFile.available()) {
          Serial.write(newFile.read());
        }
        newFile.close();
      } else {
        Serial.println("error opening test.txt");
        return -1;
      }
    } else {
      Serial.print("File too big, please try writeSize of <10");
    }
  }

  SD.remove("test.txt");

  return diffTimer;

}
