#include "test_common.h"

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
	mat2 c;
	mat2 a = {
		-3, 0,
		5, 0.5f
	};
	mat2 b = {
		-7, 2,
		4, 6 };

	mat2_mul_mat2(&c, a, b);

	realn_write_array(2, &c.m11);
	printf("\n");
	realn_write_array(2, &c.m21);
}

int main(int argc, char **argv)
{
	test_mat4_transform();
	test_mat2_mul();

	return 0;
}