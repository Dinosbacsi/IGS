#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include "building.h"
#include "vehicle.h"

long ftime, timebase;

long Get_Elapsed_Time();

void Unload_Vehicle_Into_Building(Vehicle* vehicle, Building* building);

#endif // SIMULATION_H_INCLUDED
