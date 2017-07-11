uint8_t uploadFingerpintTemplate(uint16_t id)
{
 uint8_t p = finger.loadModel(id);
 if(p!=FINGERPRINT_OK)
 {
    Serial.print("Error while loading "); 
    return p;
 }     
  // OK success!

  p = finger.getModel();
  if(p!=FINGERPRINT_OK)
 {
    Serial.print("Error while getting Model"); 
    return p;
 }   
  
  uint8_t templateBuffer[256];
  memset(templateBuffer, 0xff, 256);  //zero out template buffer
  int index=0;
  uint32_t starttime = millis();
  while (index < 256)
  {
    if (mySerial.available())
    {
      templateBuffer[index] = mySerial.read();
      st+=templateBuffer[index];
      index++;
    }
  }
  Serial.print("\nYour ID:");
  Serial.println(st);
  Serial.println();
  st="";
}
