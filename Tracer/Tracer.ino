#define MAX_NUM_DATA 50
#define FLOAT_SIZE sizeof(float)

float data[MAX_NUM_DATA];
int n_items;  // number of items in data

void setup(){
  Serial.begin(9600);
  delay(1000);

  // Send the size of float to PC
  sendByte(FLOAT_SIZE);
}