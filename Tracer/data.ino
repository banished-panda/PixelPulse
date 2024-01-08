/*
Handles the float array
Macros represent the index in the array
*/

#define IMG_WIDTH 0 
#define IMG_HEIGHT 1
#define SKY_UPPER_COLOR 2
#define SKY_LOWER_COLOR 5
#define NUM_SPHERES 8
#define STRIDE 9
#define SPHERE_LIST 10

// Folowing macros represent the offset of a parameter in a sphere data
#define SPHERE_CENTER 0
#define SPHERE_RADIUS 3

Vec3 get_Vec3(int idx){
  Vec3 result = {
    data[idx+0],
    data[idx+1],
    data[idx+2],
  };
  return result;
}

Vec3 get_sphere_center(int idx){
  idx = SPHERE_LIST + idx * data[STRIDE];
  return get_Vec3(idx + SPHERE_CENTER);
}

float get_sphere_radius(int idx){
  idx = SPHERE_LIST + idx * data[STRIDE];
  return data[idx+SPHERE_RADIUS];
}