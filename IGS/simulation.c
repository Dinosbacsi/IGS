#include "simulation.h"

long Get_Elapsed_Time()
{
	long elapsed_time = 0;

	ftime = glutGet(GLUT_ELAPSED_TIME);

	elapsed_time = ftime - timebase;

	timebase = ftime;

	return elapsed_time;
}