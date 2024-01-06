void loop(){

  // Send ready signal
  sendMessage("READY");

  // Receive choice
  int choice = getByte();
  // sendDebugMessage("choice = "+String(choice));
  if(choice == 1){
    // Receive a list of floats
    int num_floats = getByte();
    getFloats(data, num_floats);
    sendMessage("OK");
  }
  
  // Send output signal
  sendMessage("OUT");

  /* Ready for calculations */
  image_width = data[0];
  image_height = data[1];

  // Output the pixel data
  calculate_frame();
}