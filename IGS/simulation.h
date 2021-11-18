#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include "building.h"
#include "vehicle.h"

long ftime, timebase;
int delivery_cooldown;
int log_cooldown;
#define log_interval 1000;


///<summary>
///<para>Gets the elapsed time (in miliseconds) since the last render cycle.</para>
///</summary>
long Get_Elapsed_Time();

void Unload_Vehicle_Into_Building(Vehicle* vehicle, Building* building);
void Load_Building_Into_Vehicle(Building* building, Vehicle* vehicle, material_category category);

///<summary>
///<para>Loops through all the buildings.</para>
///<para>If a building exists, is factory, has production set and has road access, it runs the production method.</para>
///</summary>
void Production_Handler(long elapsed_time);

void Cargo_Loading_Handler();

///<summary>
///<para>Handles spawning of milkrun forklifts from warehouses.</para>
///</summary>
void Building_Milkrun_Handler(Building* building, long elapsed_time);

void Process_Building_Order(Building* building);
void Export_Finished_Goods(Building* building);
void Delivery_Handler(long elapsed_time);

#endif // SIMULATION_H_INCLUDED
