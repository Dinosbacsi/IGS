#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include "building.h"
#include "vehicle.h"

long ftime, timebase;
int delivery_cooldown;
int log_cooldown;
#define log_interval 2000;

long Get_Elapsed_Time();

void Unload_Vehicle_Into_Building(Vehicle* vehicle, Building* building);
void Load_Building_Into_Vehicle(Building* building, Vehicle* vehicle, material_category category);

#endif // SIMULATION_H_INCLUDED
