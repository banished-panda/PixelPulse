void sendMessage(String msg){
  Serial.print(msg);
}

void sendDebugMessage(String msg){
  Serial.println(msg);
  Serial.flush();
}

int getByte(){
  while(Serial.available() == 0){}
  int incomingByte = Serial.read();
  return incomingByte;
}

void sendByte(byte b){
  Serial.write(b);
}

float getFloat(){
  float receivedFloat = 0.0;
  char* p = (char*)&receivedFloat;
  for(int i = 0; i < FLOAT_SIZE; i++){
    while(Serial.available() == 0){}
    p[i] = Serial.read();
  }
  return receivedFloat;
}

void getFloats(float* buffer, int n){
  int n_bytes = n * FLOAT_SIZE;
  char* p = (char*)buffer;
  for(int i = 0; i < n_bytes; i++){
    while(Serial.available() == 0){}
    p[i] = Serial.read();
  }
}

void writeColor(Color c){
  sendByte(255 * c.r);
  sendByte(255 * c.g);
  sendByte(255 * c.b);
}