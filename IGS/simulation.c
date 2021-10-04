#include "simulation.h"

long Get_Elapsed_Time()
{
	long elapsed_time = 0;

	ftime = glutGet(GLUT_ELAPSED_TIME);

	elapsed_time = ftime - timebase;

	timebase = ftime;

	return elapsed_time;
}

void Unload_Vehicle_Into_Building(Vehicle* vehicle, Building* building)
{
	for (int i = 0; i < sizeof(vehicle->cargo) / sizeof(Material*); i++)
	{
		Transfer_Material(vehicle->cargo[i], Get_Storage_Space(building));
	}
}