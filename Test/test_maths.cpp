/*
References:
For verifying results
http://www.calcul.com/show/calculator/matrix-multiplication_;4;4;4;4?matrix1=[["10","20","30","40"],["-230","10","17","20"],["0","0","10","0"],["0","0","0","10"]]&matrix2=[["5","0","0","0"],["0","10","0","0"],["0","0","15","0"],["0","0","0","1"]]&operator=*
*/

#include "test_common.h"
#include "transformer.h"

void realn_write_array(const size_t nelems, const REAL *c)
{
	printf("[");
	for (size_t idx = 0; idx < nelems; idx++)
	{
		printf("%f", c[idx]);
		if (idx < nelems - 1) {
			printf(", ");
		}
	}
	printf("]");
}

void realn_print(const size_t nelems, const REAL *c, const char *caption)
{
	printf("%s ==> ", caption);
	realn_write_array(nelems, c);
	printf("\n");
}

void test_arithmetic()
{
	real3 c;
	real3 a = { 1, 0, 1 };
	real3 b = { 0, 1, 0 };
	real3 u = { 1, 1, 1 };
	real3 ux = { 1, 0, 0 };
	real3 uy = { 0, 1, 0 };

	real3_add(c, a, b);
	realn_print(3, c, "c = a + b");

	real3_sub(c, a, b);
	realn_print(3, c, "c = a - b");

	real3_neg(c, a);
	realn_print(3, c, "c = -a");

	real3_mul_scalar(c, a, 5);
	realn_print(3, c, "c = a * 5");

	real3_div_scalar(c, c, 5);
	realn_print(3, c, "c = c / 5");

	printf("MAG: %f\n", real3_mag(u));
	printf("DOT: %f\n", real3_dot(u, u));

	real3_cross(c, ux, uy);
	realn_print(3, c, "c = ux cross uy");

	real3 newer = { 5, 10, 15 };
	real3_normalize(c, newer);
	realn_print(3, c, "c = newer.normalize()");
	printf("UMAG: %f\n", real3_mag(c));

	real3 p1 = { 1, 2, 0 };
	real3 p2 = { 4, 2, 0 };
	printf("DISTANCE: %f\n", real3_distance(p1, p2));
}

void test_mat4_transform()
{
	real4 p1 = { 1, 2, 3,1 };
	real4 c;
	mat4 m = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		1,2,3,1
	};

	row4_mul_mat4(c, p1, m);
	realn_print(4, c, "c = real4*mat4");
}

void test_mat2_mul()
{
	puts("==== test_mat2_mul() ====");

	mat2 c;
	mat2 a = {
		-3, 0,
		5, 0.5f
	};
	mat2 b = {
		-7, 2,
		4, 6 };

	mat2_mul_mat2(c, a, b);

	realn_write_array(2, &c.m11);
	printf("\n");
	realn_write_array(2, &c.m21);

	printf("\n");
}

void test_mat3_mul()
{
	puts("==== test_mat3_mul() ====");

	mat3 c;
	mat3 a = {
		1, -5, 3,
		0, -2, 6,
		7, 2, -4
	};
	mat3 b = {
		-8, 6, 1,
		7, 0, -3,
		2, 4, 5
	};

	mat3_mul_mat3(c, a, b);

	realn_write_array(3, &c.m11);
	printf("\n");
	realn_write_array(3, &c.m21);
	printf("\n");
	realn_write_array(3, &c.m31);
	
	printf("\n");
}

void test_mat4_mul()
{
	puts("==== test_mat4_mul() ====");

	mat4 c;
	mat4 a = {
		10, 20, 30, 40,
		-230, 10, 17, 20,
		0, 0, 10,0,
		0,0,0,10
	};
	mat4 b = {
		5,0,0,0,
		0,10,0,0,
		0, 0, 15,0,
		0,0,0,1
	};

	mat4_mul_mat4(c, a, b);

	realn_write_array(4, &c.m11);
	printf("\n");
	realn_write_array(4, &c.m21);
	printf("\n");
	realn_write_array(4, &c.m31);
	printf("\n");
	realn_write_array(4, &c.m41);

	printf("\n");
}

void test_screen_transform()
{
	// x := -1 (left), 1 (right)
	// y := -1 (bottom), 1 (top)
	//
	REAL screenx;
	REAL screeny;
	REAL clipx = 1.0f;
	REAL clipy=0.0f;
	REAL clipw=1;
	REAL winResx = 1280;
	REAL winResy = 1024;
	REAL winCenterx = 640;
	REAL winCentery = 512;

	trans3d_map_to_window(screenx, screeny,
		clipx, clipy, clipw,
		winResx, winResy,
		winCenterx, winCentery);

	printf("  clip: %4.3f, %4.3f, %4.3f\n", clipx, clipy, clipw);
	printf("screen: %4.3f, %4.3f\n", screenx, screeny);
}

int main(int argc, char **argv)
{
	//test_mat4_transform();
	//test_mat2_mul();
	//test_mat3_mul();
	//test_mat4_mul();
	
	test_screen_transform();

	return 0;
}