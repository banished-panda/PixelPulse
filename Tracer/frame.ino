void calculate_frame() {

  float aspect_ratio = float(image_width) / image_height;



  /* For now we will assume camera axes are in same direction as world axes */
  /* CAMERA */
  float focal_length = 1.0;
  float viewport_width = 2.0;
  float viewport_height = viewport_width / aspect_ratio;
  Point3 camera_center = { 0.0, 0.0, 0.0 };  // Camera at origin

  Vec3 viewport_u = { viewport_width, 0, 0 };
  Vec3 viewport_v = { 0, -viewport_height, 0 };

  Vec3 pixel_delta_u = scale(viewport_u, 1.0 / image_width);
  Vec3 pixel_delta_v = scale(viewport_v, 1.0 / image_width);

  // Upper left corner of viewport
  Point3 viewport_upper_left = camera_center;
  viewport_upper_left = sub(viewport_upper_left, (Vec3){ 0, 0, focal_length });
  viewport_upper_left = sub(viewport_upper_left, scale(viewport_v, 0.5));
  viewport_upper_left = sub(viewport_upper_left, scale(viewport_u, 0.5));
  // Location of pixel(0,0) i.e, upper right pixel, on viewport
  Point3 pixel00_loc = add(scale(pixel_delta_u, 0.5), scale(pixel_delta_u, 0.5));
  pixel00_loc = add(pixel00_loc, viewport_upper_left);


  // For every pixel in the frame
  for (int y = 0; y < image_height; y++) {
    for (int x = 0; x < image_width; x++) {

      // Calculate ray
      Vec3 alongX = scale(pixel_delta_u, x);
      Vec3 alongY = scale(pixel_delta_v, y);
      Point3 pixel_center = add(pixel00_loc, add(alongX, alongY));
      Direction ray_dirirection = sub(pixel_center, camera_center);
      Ray r = { camera_center, ray_dirirection };

      // Calulate color for ray
      Color c = ray_color(r);

      // Write color
      writeColor(c);
    }
  }
}

Color ray_color(Ray r) {

  Direction unit_direction = unit_vec(r.dir);
  float a = unit_direction.y * 0.5 + 0.5;
  Color upper_sky = { 0.5, 0.7, 1.0};
  Color lower_sky = { 1.0, 1.0, 1.0};
  return add(
    scale(upper_sky, a),
    scale(lower_sky, 1 - a));
}