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
	printf("\nJarmu rakomanyanak atrakodasa epuletbe: \n");
	for (int i = 0; i < sizeof(vehicle->cargo) / sizeof(Material*); i++)
	{
		for (int j = 0; j < sizeof(building->storage) / sizeof(Material*); j++)
		{
			if (vehicle->cargo[i] != NULL && building->storage[j] == NULL)
			{
				//Transfer_Material(vehicle->cargo[i], building->storage[j]);
				building->storage[j] = Transfer_Material(vehicle->cargo[i]);
				vehicle->cargo[i] = NULL;
			}
		}
	}
}