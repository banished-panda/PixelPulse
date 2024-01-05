void loop(){

  //delay(1000);

  // Send ready signal
  sendMessage("READY");

  // Receive choice
  int choice = getByte();
  // sendDebugMessage("choice = "+String(choice));
  if(choice == 1){
    // Receive a list of floats
    int num_floats = getByte();
    getFloats(data, num_floats);
    // for(int i =0; i < num_floats; i++){
    //   sendDebugMessage("float received = "+String(data[i]));
    // }
    sendMessage("OK");
  }
  
  // Send output signal
  sendMessage("OUT");
  delay(10);

  // Output the pixel data
  for(int i =0; i < 20; i++){
    sendByte(i);
  }

}