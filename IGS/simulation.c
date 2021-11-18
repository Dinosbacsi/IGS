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

void Production_Handler(long elapsed_time)
{
	for (int i = 0; i < sizeof(buildings) / sizeof(Building); i++)
	{
		if (buildings[i].category == factory && buildings[i].produces != NULL && (Check_Tile(buildings[i].entry_point.x, buildings[i].entry_point.y) == 3 || Check_Tile(buildings[i].entry_point.x, buildings[i].entry_point.y) == 2))
		{
			if (buildings[i].order_cooldown < 0)
			{
				Building_Produce(&buildings[i]);
				buildings[i].order_cooldown = 10000;

				Building_Log(&buildings[i]);
			}
			else
			{
				buildings[i].order_cooldown -= elapsed_time;
			}

			tiles[buildings[i].entry_point.x][buildings[i].entry_point.y].highlighted = false;
		}
	}
}

void Cargo_Loading_Handler()
{
	for (int i = 0; i < vehicle_limit; i++)
	{
		if (vehicles[i].exists == true)
		{
			if (vehicles[i].speed == 0 && vehicles[i].current_tile->pos.x == vehicles[i].destination_node->pos.x && vehicles[i].current_tile->pos.y == vehicles[i].destination_node->pos.y)
			{
				if (vehicles[i].status == at_destination)
				{
					// If vehicle is arrived at destination building, unload into building
					Building* destination_building = Get_Building_From_Entry_Point(vehicles[i].current_tile->pos.x, vehicles[i].current_tile->pos.y);
					if (destination_building != NULL)
					{
						Unload_Vehicle_Into_Building(&vehicles[i], destination_building);
						Building_Log(destination_building);

						// If vehicle is empty, load from building and fint path to next destionation
						if (Vehicle_Is_Empty(&vehicles[i]))
						{
							if (vehicles[i].home != NULL)
							{
								if (vehicles[i].home == destination_building)
								{
									Delete_Vehicle(&vehicles[i]);
								}
								else
								{
									if (vehicles[i].home == destination_building->deliver_to)
									{
										Load_Building_Into_Vehicle(destination_building, &vehicles[i], finished);
										Building_Log(destination_building);
									}
									vehicles[i].destination_node = &road_nodes[vehicles[i].home->entry_point.x][vehicles[i].home->entry_point.y];
									vehicles[i].status = going_to_destination;
									Find_Path(&vehicles[i], road_nodes);
								}
							}
							else
							{
								if (destination_building->deliver_to == NULL)
								{
									Load_Building_Into_Vehicle(destination_building, &vehicles[i], finished);
									Building_Log(destination_building);
								}
								Vehicle_Leave_World(&vehicles[i]);
							}
						}
						else
						{
							if ((vehicles[i].home != NULL && vehicles[i].home != destination_building && vehicles[i].home == destination_building->deliver_to) ||
								(vehicles[i].home == NULL && destination_building->deliver_to == NULL))
							{
								Material* material_from_building = NULL;
								Material* material_from_vehicle = NULL;

								int j = 0;
								while (j < destination_building->storage_capacity && material_from_building == NULL)
								{
									if (destination_building->storage[j] != NULL && destination_building->storage[j]->category == finished)
									{
										material_from_building = destination_building->storage[j];
									}
									j++;
								}
								int k = 0;
								while (k < vehicles[i].capacity && material_from_vehicle == NULL)
								{
									if (vehicles[i].cargo[k] != NULL && vehicles[i].cargo[k] != destination_building->storage[j] && vehicles[i].cargo[k]->category != finished)
									{
										material_from_vehicle = vehicles[i].cargo[k];
									}
									k++;
								}

								if (material_from_building != NULL && material_from_vehicle != NULL)
								{
									destination_building->storage[j - 1] = material_from_vehicle;
									vehicles[i].cargo[k - 1] = material_from_building;
									Building_Log(destination_building);
								}

								if (k >= vehicles[i].capacity)
								{
									if (vehicles[i].home == NULL)
										Vehicle_Leave_World(&vehicles[i]);
									else
									{
										vehicles[i].destination_node = &road_nodes[vehicles[i].home->entry_point.x][vehicles[i].home->entry_point.y];
										vehicles[i].status = going_to_destination;
										Find_Path(&vehicles[i], road_nodes);
									}
								}
							}
						}
					}
				}
			}
			else
			{
				Check_For_Traffic_Ahead(&vehicles[i]);
				Vehicle_Cruise(&vehicles[i], road_nodes);
			}
		}

	}
}

void Building_Milkrun_Handler(Building* building, long elapsed_time)
{
	if (building->category == warehouse)
	{
		if (building->milkrun_cooldown < 0)
		{
			// If building can deliver to destination, then send forklift
			if (building->deliver_to != NULL && Building_Has_Storage(building->deliver_to) && Building_Milkrun_Spawned(building) < building->milkrun_vehicle_limit)
			{
				for (int j = 0; j < building->storage_capacity; j++)
				{
					if (Building_Milkrun_Spawned(building) < building->milkrun_vehicle_limit && building->storage[j] != NULL)
					{
						// Spawn forklift, send to destionation building
						int forklift_index = Place_Vehicle(vehicles, &vehicle_types[0], building->entry_point.x, building->entry_point.y, road_segments, road_nodes);
						vehicles[forklift_index].home = building;
						vehicles[forklift_index].destination_node = &road_nodes[building->deliver_to->entry_point.x][building->deliver_to->entry_point.y];
						Find_Path(&vehicles[forklift_index], road_nodes);
						vehicles[forklift_index].status = going_to_destination;

						building->milkrun_cooldown = 20000;

						vehicles[forklift_index].cargo[0] = building->storage[j];
						building->storage[j] = NULL;

						Building_Log(building);
						break;
					}
				}
			}
			// If building can accept from source building, then send forklift
			if (building->source_from != NULL && Building_Has_Storage(building) && Building_Milkrun_Spawned(building) < building->milkrun_vehicle_limit)
			{
				for (int j = 0; j < building->source_from->storage_capacity; j++)
				{
					if (building->source_from->storage[j] != NULL && ((building->source_from->category != factory && building->source_from->storage[j]->category == finished) || (building->source_from->category == factory && building->source_from->storage[j] == building->source_from->produces)))
					{
						// Spawn forklift, send to source building
						int forklift_index = Place_Vehicle(vehicles, &vehicle_types[0], building->entry_point.x, building->entry_point.y, road_segments, road_nodes);
						vehicles[forklift_index].home = building;
						vehicles[forklift_index].destination_node = &road_nodes[building->source_from->entry_point.x][building->source_from->entry_point.y];
						Find_Path(&vehicles[forklift_index], road_nodes);
						vehicles[forklift_index].status = going_to_destination;

						building->milkrun_cooldown = 20000;
						break;
					}
				}
			}
		}
		// Decrease cooldown timer
		else
		{
			building->milkrun_cooldown -= elapsed_time;
		}
	}
}

void Process_Building_Order(Building* building)
{
	int spawn_pos_x = 110;
	int spawn_pos_y = 1;

	if (randInRange(0, 1) == 0)
	{
		spawn_pos_x = 104;
		spawn_pos_y = 299;
	}

	Material* order = Get_Order(building);

	if (order != NULL)
	{
		int new_vehicle_index = -1;
		if (order->state == solid)
		{
			new_vehicle_index = Place_Vehicle(vehicles, &vehicle_types[1], spawn_pos_x, spawn_pos_y, road_segments, road_nodes);
		}
		else if (order->state == gas || order->state == liquid)
		{
			new_vehicle_index = Place_Vehicle(vehicles, &vehicle_types[2], spawn_pos_x, spawn_pos_y, road_segments, road_nodes);
		}

		if (new_vehicle_index != -1)
		{
			//vehicle_spawned_this_loop = true;
			building->delivery_cooldown = 50000;

			int destination_x = building->entry_point.x;
			int destination_y = building->entry_point.y;
			vehicles[new_vehicle_index].destination_node = &road_nodes[destination_x][destination_y];
			Find_Path(&vehicles[new_vehicle_index], road_nodes);
			vehicles[new_vehicle_index].status = going_to_destination;

			vehicles[new_vehicle_index].cargo[0] = order;
			printf("\nCargo placed on delivery truck.");
			int vehicle_cargo_index = 1;
			while (Building_Has_Orders(building) && vehicle_cargo_index < vehicles[new_vehicle_index].capacity)
			{
				if (vehicles[new_vehicle_index].cargo[vehicle_cargo_index] == NULL)
				{
					vehicles[new_vehicle_index].cargo[vehicle_cargo_index] = Get_Order(building);
					printf("\nCargo placed on delivery truck.");
					vehicle_cargo_index++;
				}
			}

			Building_Log(building);
		}
	}
}void Export_Finished_Goods(Building* building)
{
	int spawn_pos_x = 110;
	int spawn_pos_y = 1;

	if (randInRange(0, 1) == 0)
	{
		spawn_pos_x = 104;
		spawn_pos_y = 299;
	}

	Material* product = Get_Product(building);
	int new_vehicle_index = -1;
	if (product->state == solid)
	{
		new_vehicle_index = Place_Vehicle(vehicles, &vehicle_types[1], spawn_pos_x, spawn_pos_y, road_segments, road_nodes);
	}
	else if (product->state == gas || product->state == liquid)
	{
		new_vehicle_index = Place_Vehicle(vehicles, &vehicle_types[2], spawn_pos_x, spawn_pos_y, road_segments, road_nodes);
	}

	if (new_vehicle_index != -1)
	{
		//vehicle_spawned_this_loop = true;

		building->delivery_cooldown = 50000;

		int destination_x = building->entry_point.x;
		int destination_y = building->entry_point.y;
		vehicles[new_vehicle_index].destination_node = &road_nodes[destination_x][destination_y];
		Find_Path(&vehicles[new_vehicle_index], road_nodes);
		vehicles[new_vehicle_index].status = going_to_destination;
	}
}

void Delivery_Handler(long elapsed_time)
{
	bool vehicle_spawned_this_loop = false;

	for (int i = 0; i < sizeof(buildings) / sizeof(Building); i++)
	{
		if (buildings[i].exists == true && (buildings[i].category == factory || buildings[i].category == warehouse))
		{
			// If delivery cooldown is 0, handle export/import
			if (buildings[i].delivery_cooldown < 0)
			{
				// Process order and import materials
				if (Building_Has_Orders(&buildings[i]) && !vehicle_spawned_this_loop)
				{
					Process_Building_Order(&buildings[i]);
				}

				// Export finished goods
				if (buildings[i].deliver_to == NULL && Building_Has_Finished_Product(&buildings[i]))
				{
					Export_Finished_Goods(&buildings[i]);
				}
			}
			else
			{
				// Decrease cooldown counter
				buildings[i].delivery_cooldown -= elapsed_time;
			}

			Building_Milkrun_Handler(&buildings[i], elapsed_time);
		}
	}
}