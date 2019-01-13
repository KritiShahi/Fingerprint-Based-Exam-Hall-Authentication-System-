/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal LCD(10,9,5,4,12,13);
uint8_t id;

uint8_t getFingerprintEnroll();

// Software serial for when you dont have a hardware serial port
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// On Leonardo/Micro/Yun, use pins 8 & 9. On Mega, just grab a hardware serialport 
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);


void setup()  
{
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(500);
  
  Serial.begin(9600);
  Serial.println("Adafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  boolean validnum = false; 
  while (1) {
    while (! Serial.available());
    char c = Serial.read();
    if (isdigit(c)) {
       num *= 10;
       num += c - '0';
       validnum = true;
    } else if (validnum) {
      return num;
    }
  }
}

void loop()                     // run over and over again
{
  LCD.begin(16,2);
  LCD.setCursor(0,0);
  LCD.println("Ready to enroll");
  delay(2000);
  LCD.clear();
  LCD.println("Type ID #");
  Serial.println("Ready to enroll a fingerprint! Please Type in the ID # you want to save this finger as...");
  id = readnumber();
  Serial.print("Enrolling ID #");
  Serial.println(id);
  LCD.println(id);
  delay(2000);
  LCD.clear();
  
 
  
  
  
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); 
  LCD.print("Enroll now");
  delay(2000);
  Serial.println(id);
  LCD.print(id);
  LCD.clear();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      LCD.println("Image taken");
      delay(2000);
      LCD.clear();
      
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      LCD.print(".");
      LCD.clear();
      delay(2000);
       
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      LCD.print("Communication error");
       delay(2000);
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      LCD.print("Imaging error");
       
      break;
    default:
      Serial.println("Unknown error");
      LCD.print("Unknown error");
       LCD.clear();
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      LCD.println("Image converted");
       delay(1000);
       LCD.clear();
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      LCD.print("Image too messy");
      delay(1000);
       LCD.clear();
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      LCD.print("Communication error");
      delay(2000);
       LCD.clear();
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      LCD.print("Could not find fingerprint features");
      delay(1000);
       LCD.clear();
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      LCD.print("Could not find fingerprint features");
      delay(1000);
       LCD.clear();
      return p;
    default:
      Serial.println("Unknown error");
      LCD.print("Unknown error");
       LCD.clear();
      return p;
  }
  
  Serial.println("Remove finger");
  LCD.print("Remove finger");
  delay(1000);
  LCD.clear();
  
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); 
  
  
  Serial.println(id);
  
  p = -1;
  Serial.println("Place same finger again");
  LCD.print("Put same finger");
  delay(2000);
  LCD.clear();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      LCD.print("Image taken");
      delay(2000);
      LCD.clear();
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      LCD.print(".");
      delay(2000);
      LCD.clear();
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
       LCD.print("Communication error");
      delay(2000);
      LCD.clear();
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      LCD.print("Imaging error");
      delay(2000);
      LCD.clear();
      break;
    default:
      Serial.println("Unknown error");
      LCD.print("Unknown error");
      delay(2000);
      LCD.clear();
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      LCD.print("Image converted");
      delay(2000);
      LCD.clear();
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
       LCD.print("Image too messy");
       delay(2000);
      LCD.clear();
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      LCD.print("Communication error");
      delay(2000);
      LCD.clear();
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      LCD.print("Could not find fingerprint features");
      delay(2000);
      LCD.clear();
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      LCD.print("Could not find fingerprint features");
      delay(2000);
      LCD.clear();
      return p;
    default:
      Serial.println("Unknown error");
      LCD.print("Unknown error");
      delay(2000);
      LCD.clear();
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
    LCD.print("Prints matched");
    delay(2000);
    LCD.clear();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    LCD.print("Communication error");
    delay(2000);
    LCD.clear();
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    LCD.print("Fingerprints did not match");
    delay(2000);
    LCD.clear();
    return p;
  } else {
    Serial.println("Unknown error");
    LCD.print("Unknown error");
    delay(2000);
    LCD.clear();
    return p;
  }   
  
  Serial.print("ID ");
  Serial.println(id);
  LCD.print("ID ");
  LCD.print(id);
  delay(2000);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    LCD.print("Stored!");
    delay(2000);
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    LCD.print("Communication error");
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    LCD.print("Could not store in that location");
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    LCD.print("Error writing to flash");
    delay(2000);
    return p;
  } else {
    Serial.println("Unknown error");
    LCD.print("Unknown error");
    delay(2000);
    return p;
  }   
}
