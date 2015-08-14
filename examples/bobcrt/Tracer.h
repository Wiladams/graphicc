#include <math.h>

//Define a vector class with constructor and operator: 'v'
struct vec3 {
	float x, y, z;  // Vector has three float attributes.

	vec3(){}                                  //Empty constructor
	vec3(float a, float b, float c){ x = a; y = b; z = c; }            //Constructor

	vec3 operator+(vec3 r){ return vec3(x + r.x, y + r.y, z + r.z); }	//Vector add
	vec3 operator*(float r){ return vec3(x*r, y*r, z*r); }				//Vector scaling
	float operator%(vec3 r){ return x*r.x + y*r.y + z*r.z; }			//Vector dot product
	vec3 operator^(vec3 r){ return vec3(y*r.z - z*r.y, z*r.x - x*r.z, x*r.y - y*r.x); } //Cross-product
	vec3 operator!(){ return *this*(1 / sqrt(*this%*this)); }			// Used later for normalizing the vector
}; 

struct Tracer
{
	Tracer();
	Tracer(int width, int height);

	int intersect(vec3 o, vec3 d, float & t, vec3& n);
	vec3 Sample(vec3 o, vec3 d);

	void BeginFrame();
	bool ContinueFrame(void(*setpixel)(int x, int y, int r, int g, int b));


	vec3 g, a, b, c;
	int x, y;	// current pixel being rendered
	int width, height;
};