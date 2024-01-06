void calculate_frame(){

  for(int y = 0; y < image_height; y++){
    for(int x = 0; x < image_width; x++){
      
      Color c = {x/data[0], y/data[1], 0.0};
      writeColor(c);
    }
  }

}