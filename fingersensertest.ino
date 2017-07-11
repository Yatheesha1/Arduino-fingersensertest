
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

uint8_t id=1;
String st="";

// pin #2 is IN from sensor (YELLOW wire)
// pin #3 is OUT from arduino  (BLUE wire)
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(500);
  
  Serial.begin(9600);
  Serial.println("Adafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) 
  {
    Serial.println("Found fingerprint sensor!");
  } 
  else 
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
}


void loop()                     // run over and over again
{  
  while (! getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() 
{

  int p = -1;
  Serial.println("Waiting for valid finger..................."); 
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) 
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      break;
    default:
      Serial.println("Error while taking image");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) 
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    default:
      Serial.println("Error while converting image");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) 
  {
    p = finger.getImage();
  }
  
  p = -1;
  Serial.println("Place same finger again..................");
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      break;
    default:
      Serial.println("Error while taking image");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    default:
      Serial.println("Error while converting image");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  
  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Error while matching image");
    return p;
  }   
  
  Serial.print("ID "); 
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) 
  {
    Serial.println("Success!");
    uploadFingerpintTemplate(id);    
  } 
  else 
  {
    Serial.println("Error while storing image");
    return p;
  }   
}
