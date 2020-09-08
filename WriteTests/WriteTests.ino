#include <SD.h>
#include <SPI.h>

const int chipSelect = BUILTIN_SDCARD; //declare SD socket

//methods:
// testWrite(char 'type, int testLength)
// type is a character, 'L', 'I', 'C' for Long, Integer or Character.
// testlength is # of cycles

File newFile;
int testLength = 500;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; //wait for serial port
  }
  long randLong = random();

  newFile = SD.open("test2.txt", FILE_WRITE);

  if (newFile) {
    Serial.println("Writing Long...");
    for (int i = 0; i < testLength; i++) {
      newFile.print(randLong);
    }


    newFile.print("int: ");
    long intPre = micros();
    //    newFile.println(intTest);
    long intPost = micros();


    newFile.print("long: ");
    long longPre = micros();
    //    newFile.println(longTest);
    long longPost = micros();

    Serial.print("Int time: ");
    long intDiff = intPost - intPre;
    Serial.println(intDiff);
    Serial.print("Long time: ");
    long longDiff = longPost - longPre;
    Serial.println(longDiff);

    newFile.close();
    Serial.println("Written and Closed.");
  } else {
    Serial.println("Cannot Open");
  }


  newFile = SD.open("test2.txt");
  if (newFile) {
    Serial.println("test2.txt:");

    // read from the file until there's nothing else in it:
    while (newFile.available()) {
      Serial.write(newFile.read());
    }
    // close the file:
    newFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}

void SDinit() {
  Serial.print("Initialising . . .");

  if (SD.begin(chipSelect)) {
    Serial.println("Succeeded");
  } else
    Serial.println("Failed");

}

long testWrite(char type, int testLength) {
  switch (type) {
    case 'L':
        // etc
        break;
    case 'I':
        //etc
        break;
    case 'C':
        //etc
        break;
    default:
        // etc
        break;

  }

  return 0;

}

