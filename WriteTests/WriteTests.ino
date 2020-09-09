#include <SD.h>
#include <SPI.h>

const int chipSelect = BUILTIN_SDCARD; //declare SD socket

//methods:
// testWrite(char 'type', int testLength, bool 'testRead')
// type is a character, 'L', 'I', 'C' for Long, Integer or Character.
// testlength is # of cycles
// testRead will run a read speed test
// returns total time for test.

File newFile;
int testLength = 500;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; //wait for serial port
  }
  SDinit();
  testWrite('L', 5000, false);
  testWrite('I', 5000, false);
  testWrite('D', 5000, false);
  testWrite('C', 5000, false);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void SDinit() {
  Serial.print("Initialising SD write test V0.1 . . .");

  if (SD.begin(chipSelect)) {
    Serial.println("Succeeded");
  } else {
    Serial.println("Failed - Aborting");
    while (true);
  }
}

long testWrite(char type, int testLength, boolean readTest) {
  newFile = SD.open("test.txt", FILE_WRITE);


  Serial.print("\nTesting: ");


  long initTimer = micros();
  int dataSize;

  switch (type) {
    case 'L': {
        long randLong = random();
        dataSize = sizeof(long);
        Serial.println((String)"Long (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.print(randLong);
        }
        break;
      }
    case 'I': {
        int randInt = random();
        dataSize = sizeof(int);
        Serial.println((String)"Int (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.print(randInt);
        }

        break;
      }
    case 'C': {
        char c = 'a';
        dataSize = sizeof(char);
        Serial.println((String)"Char (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.print(c);
        }

        break;
      }
    case 'D': {
        double randDouble = random();
        dataSize = sizeof(double);
        Serial.println((String)"Double (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.print(randDouble);
        }
      }
  }


  long endTimer = micros();

  float diffTimer = endTimer - initTimer;
  Serial.println((String)"Total # of tests run: " + testLength);
  Serial.println((String)"Total Data Written: " + ((float)testLength * dataSize) / 1000 + "kb");
  Serial.println((String)"Total Time for test: " + diffTimer + " uS");
  Serial.println((String)"Time per individual write: " + (diffTimer / testLength) + " uS");
  Serial.println((String)"Time per Byte: " + (diffTimer / (testLength * dataSize)) + " uS");


  if (newFile) {
    newFile.close();
    Serial.println("Written and Closed Succesfully");
  } else {
    Serial.println("Can't open file to close it");
  }

  if (readTest) {
    if (newFile) {
      Serial.println("Reading test2.txt:");
      while (newFile.available()) {
        Serial.write(newFile.read());
      }
      newFile.close();
    } else {
      Serial.println("error opening test.txt");
    }
  }

  SD.remove("test.txt");

  return diffTimer;

}

