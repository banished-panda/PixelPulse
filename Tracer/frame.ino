#define SAMPLES_PER_PIXEL 5
#define MAX_DEPTH 5

void calculate_frame() {

  float aspect_ratio = float(image_width) / image_height;



  /* For now we will assume camera axes are in same direction as world axes */
  /* CAMERA */
  float focal_length = 1.0;
  float viewport_height = 2.0;
  float viewport_width = viewport_height * aspect_ratio;
  Point3 camera_center = { 0.0, 0.0, 0.0 };  // Camera at origin

  Vec3 viewport_u = { viewport_width, 0, 0 };
  Vec3 viewport_v = { 0, -viewport_height, 0 };

  Vec3 pixel_delta_u = scale(viewport_u, 1.0 / image_width);
  Vec3 pixel_delta_v = scale(viewport_v, 1.0 / image_height);

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

      // Calculate pixel center on viewport
      Vec3 alongX = scale(pixel_delta_u, x);
      Vec3 alongY = scale(pixel_delta_v, y);
      Point3 pixel_center = add(pixel00_loc, add(alongX, alongY));

      // Multiple samples per pixel

      Color cumulativeColor = {0.0, 0.0, 0.0};
      for(int i = 0; i < SAMPLES_PER_PIXEL; i++){

        Vec3 randomDX = scale(pixel_delta_u, random_float() - 0.5);
        Vec3 randomDY = scale(pixel_delta_v, random_float() - 0.5);
        Point3 pixel_sample = add(pixel_center, add(randomDX, randomDY));

        // Get direction and hence ray
        Direction ray_dirirection = sub(pixel_sample, camera_center);
        Ray r = { camera_center, ray_dirirection };

        // cumulatively add calulated color for ray
        cumulativeColor = add(cumulativeColor, ray_color(r, MAX_DEPTH));
      }

      // Write color = cumulativeColor scaled by (1/SAMPLES_PER_PIXEL)
      writeColor(scale(cumulativeColor, (1.0/SAMPLES_PER_PIXEL)));
    }
  }
}

Color ray_color(Ray r, int max_depth) {

  if(max_depth <= 0){
    return (Color){0.0, 0.0, 0.0};
  }

  HitRecord record;
  if(hit_world(r, (Range){0, INFINITY}, &record)){
    Vec3 dir = random_Vec3_on_hemisphere(record.normal);
    return scale(ray_color((Ray){record.point, dir}, max_depth - 1), 0.5);
  }


  Direction unit_direction = unit_vec(r.dir);
  float a = unit_direction.y * 0.5 + 0.5;
  Color upper_sky = get_Vec3(SKY_UPPER_COLOR);
  Color lower_sky = get_Vec3(SKY_LOWER_COLOR);
  return add(
    scale(upper_sky, a),
    scale(lower_sky, 1 - a));
}