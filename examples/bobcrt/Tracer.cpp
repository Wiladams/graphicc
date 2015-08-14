// Reference:
// http://fabiensanglard.net/rayTracing_back_of_business_card/index.php
//
#include "Tracer.h"
#include <math.h>
#include <stdlib.h>
#include "graphicc.h"

//typedef int i;       //Save space by using 'i' instead of 'int'
//typedef float f;     //Save even more space by using 'f' instead of 'float'




//The set of sphere positions describing the world.
//These integers are in fact bit masks.
//int G[] = { 247570, 280596, 280600, 249748, 18578, 18577, 231184, 16, 16 };

int G[] = { 16, 16, 231184, 18577, 18578, 249748, 280600, 280596, 247570};

/*

16                    1
16                    1
231184   111    111   1
18577       1  1   1  1   1
18578       1  1   1  1  1
249748   1111  11111  1 1
280600  1   1  1      11
280596  1   1  1      1 1
247570   1111   111   1  1

*/

// Random generator, return a float within range [0-1]
inline float RAND(){ return(float)rand() / RAND_MAX; }


Tracer::Tracer()
	:Tracer(0,0)
{}

Tracer::Tracer(int width, int height)
:width(width), height(height)
{
	// The '!' are for normalizing each vectors with ! operator. 
	g = !vec3(-6, -16, 0);       // Camera direction
	a = !(vec3(0, 0, 1) ^ g)*.002; // Camera up vector...Seem Z is pointing up :/ WTF !
	b = !(g^a)*.002;        // The right vector, obtained via traditional cross-product
	c = (a + b)*-256 + g;       // WTF ? See https://news.ycombinator.com/item?id=6425965 for more.

}

//The intersection test for line [o,v].
// Return 2 if a hit was found (and also return distance t and bouncing ray n).
// Return 0 if no hit was found but ray goes upward
// Return 1 if no hit was found but ray goes downward
int Tracer::intersect(vec3 o, vec3 d, float & t, vec3& n)
{
	t = 1e9;
	int m = 0;
	float p = -o.z / d.z;
	if (.01 < p) {
		t = p, n = vec3(0, 0, 1), m = 1;
	}

	//The world is encoded in G, with 9 lines and 19 columns
	for (int k = 19; k--;)  //For each columns of objects
	for (int j = 9; j--;)   //For each line on that columns

	if (G[j] & 1 << k){ //For this line j, is there a sphere at column i ?

		// There is a sphere but does the ray hit it ?

		vec3 p = o + vec3(-k, 0, -j - 4);
		float b = p%d, c = p%p - 1, q = b*b - c;

		//Does the ray hit the sphere ?
		if (q>0){
			//It does, compute the distance camera-sphere
			float s = -b - sqrt(q);

			if (s<t && s>.01)
				// So far this is the minimum distance, save it. And also
				// compute the bouncing ray vector into 'n'  
				t = s,
				n = !(p + d*t),
				m = 2;
		}
	}

	return m;
}

// (S)ample the world and return the pixel color for
// a ray passing by point o (Origin) and d (Direction)
vec3 Tracer::Sample(vec3 o, vec3 d)
{
	float t;
	vec3 n;
	static vec3 skycolor(.7, .6, 1);
	static vec3 spherecolor(9 + RAND(), 9 + RAND(), 16);

	//Search for an intersection ray Vs World.
	int m = intersect(o, d, t, n);

	// m==0
	if (!m) {
		//No sphere found and the ray goes upward: Generate a sky color  
		//return vec3(.7, .6, 1)*pow(1 - d.z, 4);
		return skycolor*pow(1 - d.z, 4);
	}

	//A sphere was maybe hit.
	vec3 h = o + d*t,                    // h = intersection coordinate
		l = !(spherecolor + h*-1),  // 'l' = direction to light (with random delta for soft-shadows).
		r = d + n*(n%d*-2);               // r = The half-vector

	//Calculated the lambertian factor
	float b = l%n;

	//Calculate illumination factor (lambertian coefficient > 0 or in shadow)?
	if (b<0 || intersect(h, l, t, n))
		b = 0;

	// Calculate the color 'p' with diffuse and specular component 
	float p = pow(l%r*(b>0), 99);

	if (m & 1){   //m == 1
		h = h*.2; //No sphere was hit and the ray was going downward: Generate a floor color
		return((int)(ceil(h.x) + ceil(h.y)) & 1 ? vec3(3, 1, 1) : vec3(3, 3, 3))*(b*.2 + .1);
	}

	//m == 2 A sphere was hit. Cast an ray bouncing from the sphere surface.
	return vec3(p, p, p) + Sample(h, r)*.5; //Attenuate color by 50% since it is bouncing (* .5)
}

void Tracer::BeginFrame()
{
	y = height - 1;
	x = width - 1;
}

bool Tracer::ContinueFrame(void(*setpixel)(int x, int y, int r, int g, int b))
{
	y--;

	if (y < 0)
		BeginFrame();

	for (x = width; x--;){   //For each pixel in a line

		//Reuse the vector class to store not XYZ but a RGB pixel color
		vec3 p(13, 13, 13);     // Default pixel color is almost pitch black

		//Cast 64 rays per pixel (For blur (stochastic sampling) and soft-shadows. 
		for (int r = 64; r--;)
		{

			// The delta to apply to the origin of the view (For Depth of View blur).
			vec3 t = a*(RAND() - .5) * 99 + b*(RAND() - .5) * 99; // A little bit of delta up/down and left/right 

			// Set the camera focal point v(17,16,8) and Cast the ray 
			// Accumulate the color returned in the p variable
			p = Sample(vec3(17, 16, 8) + t, //Ray Origin
				!(t*-1 + (a*(RAND() + x) + b*(y + RAND()) + c) * 16) // Ray Direction with random deltas
				// for stochastic sampling
				)*3.5 + p; // +p for color accumulation
		}

		setpixel(width - x-1, height-y-1, (int)p.x, (int)p.y, (int)p.z);
	}

	return y == 0;
}
