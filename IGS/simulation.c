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
	//printf("\nJarmu rakomanyanak atrakodasa epuletbe: \n");
	for (int i = 0; i < sizeof(vehicle->cargo) / sizeof(Material*); i++)
	{
		for (int j = 0; j < sizeof(building->storage); j++)
		{
			if (vehicle->cargo[i] != NULL && building->storage[j] == NULL)
			{
				building->storage[j] = Transfer_Material(vehicle->cargo[i]);
				vehicle->cargo[i] = NULL;
			}
		}
	}
}

void Load_Building_Into_Vehicle(Building* building, Vehicle* vehicle)
{
	for (int i = 0; i < sizeof(building->storage); i++)
	{
		for (int j = 0; j < sizeof(vehicle->cargo) / sizeof(Material*); j++)
		{
			if (vehicle->cargo[i] == NULL && building->storage[i] != NULL)
			{
				if (building->storage[i]->category == finished)
				{
					vehicle->cargo[j] = Transfer_Material(building->storage[i]);
					building->storage[i] = NULL;
				}
			}
		}
	}
}