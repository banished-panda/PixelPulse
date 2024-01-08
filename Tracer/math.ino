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
    - hit_sphere

********************************/

Point3 at(Ray r, float t){
  return add(r.origin, scale(r.dir, t));
}

bool hit_sphere(Point3 center, float radius, Ray r, Range t_range, HitRecord *record){

  Vec3 oc = sub(r.origin, center);
  float a = length_squared(r.dir);
  float half_b = dot(r.dir, oc);
  float c = length_squared(oc) - radius*radius;
  float discriminant = half_b*half_b - a*c;

  if(discriminant < 0) return false;

  float sqrtd = sqrt(discriminant);

  // Finding nearest root in acceptable range
  float root = (-half_b - sqrtd) / a; 
  if(!contains(t_range, root)){
    root = (-half_b + sqrtd) / a;
    if(!contains(t_range, root))
      return false;
  }

  // We have a root, now we populate the record
  record->t = root;
  record->point = at(r, root);
  record->normal = scale(sub(record->point, center), 1/radius);
  record->front_face = dot(r.dir, record->normal) < 0;
  if(!record->front_face){
    record->normal = scale(record->normal, -1);
  }
  return true;
}

bool hit_world(Ray r, Range t_range, HitRecord *record){

  HitRecord temp_rec;
  bool hit_anything = false;
  float closest = t_range.max_val;

  // Loop over all spheres in world
  for(int i = 0; i < data[NUM_SPHERES]; i++){
    Point3 center = get_sphere_center(i);
    float radius = get_sphere_radius(i);
    if(hit_sphere(center, radius, r, (Range){t_range.min_val, closest}, &temp_rec)){
      hit_anything = true;
      closest = temp_rec.t;
      // Copy contents of temp_rec to record
      record->front_face = temp_rec.front_face;
      record->normal = temp_rec.normal;
      record->point = temp_rec.point;
      record->t = temp_rec.t;
    }
  }

  return hit_anything;
}

/*******************************

    Other functions

    - contains
    - surrounds

********************************/

// r.min_val <= v && v <= r.min_val
bool contains(Range r, float v){
  return r.min_val <= v && v <= r.max_val;
}
// r.min_val < v && v < r.min_val
bool surrounds(Range r, float v){
  return r.min_val < v && v < r.max_val;
}