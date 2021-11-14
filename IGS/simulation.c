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
	for (int i = 0; i < vehicle->capacity; i++)
	{
		for (int j = 0; j < building->storage_capacity; j++)
		{
			if (vehicle->cargo[i] != NULL && building->storage[j] == NULL)
			{
				building->storage[j] = Transfer_Material(vehicle->cargo[i]);
				vehicle->cargo[i] = NULL;
			}
		}
	}
}

void Load_Building_Into_Vehicle(Building* building, Vehicle* vehicle, material_category category)
{
	int cargo_in_vehicle = 0;
	for (int i = 0; i < building->storage_capacity; i++)
	{
		for (int j = 0; j < vehicle->capacity; j++)
		{
			if (cargo_in_vehicle < vehicle->capacity && vehicle->cargo[j] == NULL && building->storage[i] != NULL)
			{
				if (building->storage[i]->category == category || category == NULL)
				{
					vehicle->cargo[j] = Transfer_Material(building->storage[i]);
					building->storage[i] = NULL;
					cargo_in_vehicle++;
				}
			}
		}
	}
}