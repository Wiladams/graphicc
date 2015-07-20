#include "dproc_clock.h"

#include <Windows.h>
#include <stdint.h>

// for time keeping
uint64_t startcount = 0;
double clockfrequency = 1;

// time keeping
void resettime()
{
	::QueryPerformanceCounter((LARGE_INTEGER*)&startcount);
}

double seconds()
{
	uint64_t currentCount;
	::QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);

	uint64_t ellapsed = currentCount - startcount;
	double secs = ellapsed * clockfrequency;

	return secs;
}

void initTime()
{
	// Setup time
	uint64_t freq;
	::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	::QueryPerformanceCounter((LARGE_INTEGER*)&startcount);
	clockfrequency = 1.0f / freq;
}