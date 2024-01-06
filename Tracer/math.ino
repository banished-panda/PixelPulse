/*******************************

      Vec3 Specific functions

    - add
    - sub
    - scale
    - length
    - length_squared
    - dot
    - cross
    - unit_vec

********************************/

/* a + b */
Vec3 add(Vec3 a, Vec3 b){
  Vec3 result = {
    a.x + b.x,
    a.y + b.y,
    a.z + b.z
  };
  return result;
}

/* a - b */
Vec3 sub(Vec3 a, Vec3 b){
  Vec3 result = {
    a.x - b.x,
    a.y - b.y,
    a.z - b.z
  };
  return result;
}

/* v scaled by s */
Vec3 scale(Vec3 v, float s){
  Vec3 result = {
    v.x * s,
    v.y * s,
    v.z * s
  };
  return result;
}

/* Dot product of a and b */
float dot(Vec3 a, Vec3 b){
  float result = 
    a.x * b.x +
    a.y * b.y +
    a.z * b.z
  ;
  return result;
}

/* Cross product of a and b (a X b) */
Vec3 cross(Vec3 a, Vec3 b){
  Vec3 result = {
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  };
  return result;
}

/* Square of length of v */
float length_squared(Vec3 v){
  return dot(v, v);
}

/* length of v */
float length(Vec3 v){
  return sqrt(length_squared(v));
}

/* Unit vector along v */
Vec3 unit_vec(Vec3 v){
  return scale(v, 1.0 / length(v));
}

/*******************************

      Ray Specific functions

    - at

********************************/

Point3 at(Ray r, float t){
  return add(r.origin, scale(r.dir, t));
}
