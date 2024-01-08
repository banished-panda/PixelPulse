#define MAX_NUM_DATA 50
#define FLOAT_SIZE sizeof(float)

float data[MAX_NUM_DATA];
int n_items;  // number of items in data

int image_width, image_height;

union Vector3{
  struct {
    float x;
    float y;
    float z;
  };
  struct{
    float r;
    float g;
    float b;
  };
};

typedef union Vector3 Color;
typedef union Vector3 Point3;
typedef union Vector3 Direction;
typedef union Vector3 Vec3;

typedef struct {
  Point3 origin;
  Direction dir;
} Ray;

typedef struct {
  Point3 point;
  Direction normal;
  float t;
  bool front_face;
} HitRecord;

typedef struct {
  float min_val;
  float max_val;
} Range;

void setup(){
  Serial.begin(57600);
  delay(10);

  // Send the size of float to PC
  sendByte(FLOAT_SIZE);
}