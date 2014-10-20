#include "test_common.h"

REAL pts1[] = {
	0,0,1,1,
	2,0,1,1,
	2,3,1,1,
	0,3,1,1,
	0,0,0,1,
	2,0,0,1,
	2,3,0,1,
	0,3,0,1
};

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

void test_scale()
{
	REAL res[32];

	mat4 scalemat;
	trans3d_scale(scalemat, 1.0 / 2.0f, 1.0 / 3.0f, 1.0f);

	size_t npts = sizeof(pts1) / sizeof(REAL) / 4;

	printf("npts: %d\n", npts);

	for (size_t idx = 0; idx < npts; idx++)
	{
		row4_mul_mat4(&res[idx * 4], &pts1[idx * 4], scalemat);
	}

	for (size_t idx = 0; idx < npts; idx++)
	{
		realn_write_array(4, &res[idx*4]);
		printf("\n");
	}


}

int main(int argc, char **argv)
{
	test_scale();
}