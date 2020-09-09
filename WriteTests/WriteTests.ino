// Lewis Hyman - Arduino Write Speed Tester
double fileVersion = 0.2;

#include <SD.h>
#include <SPI.h>


const int chipSelect = BUILTIN_SDCARD; //declare SD socket

//methods:
// testWrite(char 'type', int testLength, bool 'testRead')
// type is a character, 'L', 'I', 'C' for Long, Integer or Character.
// testlength is amount of data per test in kb
// testRead will run a read speed test
// returns total time for test.

File newFile;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; //wait for serial port
  }
  SDinit();
  testAll(50);




}

void loop() {
}

void testAll(int writeSize) {
  testWrite('C', writeSize, false);
  testWrite('I', writeSize, false);
  testWrite('L', writeSize, false);
  testWrite('D', writeSize, false);

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

  Serial.print("\nTesting: ");

  int testLength =-1;
  int dataSize = -1;

  long initTimer = micros();
  switch (type) {
    case 'L': {

        long randLong = random();
        dataSize = sizeof(long);
        testLength = writeSize * 1000 / dataSize;
        Serial.println((String)"Long (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.print(randLong);
        }
        break;
      }
    case 'I': {
        int randInt = random();
        dataSize = sizeof(int);
        testLength = writeSize * 1000 / dataSize;
        Serial.println((String)"Int (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.print(randInt);
        }

        break;
      }
    case 'C': {
        char c = 'a';
        dataSize = sizeof(char);
        testLength = writeSize * 1000 / dataSize;
        Serial.println((String)"Char (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.print(c);
        }

        break;
      }
    case 'D': {
        double randDouble = random();
        dataSize = sizeof(double);
        testLength = writeSize * 1000 / dataSize;
        Serial.println((String)"Double (" + dataSize + " Bytes)");
        for (int i = 0; i < testLength; i++) {
          newFile.print(randDouble);
        }
      }
  }

  long endTimer = micros();

  float diffTimer = endTimer - initTimer;
  Serial.println((String)"Total # of tests run: " + testLength + " (" + (double)testLength * dataSize / 1000 + "Kb)");
  Serial.println((String)"Total Time for test: " + diffTimer + " uS / " + diffTimer / 1000 + " mS / " + diffTimer / 1000000 + " S");
  Serial.println((String)"Time per Write: " + (diffTimer / testLength) + " uS (" + (diffTimer / (testLength * dataSize)) + " uS per byte)");



  if (newFile) {
    newFile.close();
  } else {
    Serial.println("Can't open file to close it");
    return -1;
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
      return -1;
    }
  }

  SD.remove("test.txt");

  return diffTimer;

}

