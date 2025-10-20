#include "window/window.hpp"
#pragma once
#include "../driver.hpp"
#include "../mem.hpp"
#include "../offsets.hpp"
#include "../client_dll.hpp"
#include "../render.hpp"
#include "../bones.hpp"
#include "../structs.hpp"
#include "../util.hpp"
#include "../base_entity.hpp"
#include "../esp.hpp"

#include <thread>

uintptr_t get_player_controller(Memory memory, uintptr_t game_entity_system, int player_index) {

	uintptr_t es_list_entry = memory.RPM<uintptr_t>(game_entity_system + 8LL * (player_index >> 9) + 16);
	if (es_list_entry == 0) {
		return {};
	}

	uintptr_t controller = memory.RPM<uintptr_t>(es_list_entry + 120LL * (player_index & 0x1FF));
	
	int health = memory.RPM<int>(controller + cs2_dumper::schemas::client_dll::CCSPlayerController::m_iPawnHealth);
	return controller;
}

int main()
{
	Overlay overlay;
	Memory memory;
	
	const DWORD process_id = memory.get_process_id(L"cs2.exe");

	std::cout << "CS2 PID: " << std::hex << process_id << "\n";
	memory.driver_handle = driver::get_driver_handle();

	if (driver::attach_to_process(process_id, memory.driver_handle) == true)
	{
		std::cout << "Attached" << "\n";
	}

	uintptr_t module_base = memory.get_module_base(process_id, L"client.dll");
	uintptr_t engine_base = memory.get_module_base(process_id, L"engine2.dll");

	// hide console window since we're making our own window, or use WinMain() instead.
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	
	printf("[>>] Hit insert to show the menu in this overlay!\n");
	overlay.SetupOverlay(L"overlay");

	//ENGINE VARS

	//Main Loop
	while (overlay.shouldRun)
	{
		Base_Entity local_entity;
		uintptr_t entity_list = memory.RPM<uintptr_t>(module_base + cs2_dumper::offsets::client_dll::dwEntityList);
		uintptr_t dwGameEntitySystem = memory.RPM<uintptr_t>(module_base + cs2_dumper::offsets::client_dll::dwGameEntitySystem);
		uintptr_t my_controller = memory.RPM<uintptr_t>(module_base + cs2_dumper::offsets::client_dll::dwLocalPlayerController);
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));

		 //THIS IS ME
		view_matrix_t view_matrix = memory.RPM<view_matrix_t>(module_base + cs2_dumper::offsets::client_dll::dwViewMatrix);

		overlay.StartRender();

		if (overlay.RenderMenu) {
			overlay.Render();
		}
		else {
			
			
			//Player Loop
			for (int player_index = 0; player_index <= 10; player_index++)
			{	 
		
				uintptr_t player_controller = get_player_controller(memory, dwGameEntitySystem, player_index);
				if (!player_controller) continue;

				Base_Entity entity = {player_controller, memory};
				entity.init_entity(player_index, entity_list);
				//Check if is local player

				if (entity.base_addr == my_controller) {
					local_entity = entity;
					continue;
				}

				if (entity.life_state == 0 || entity.health <= 0) {
					continue;
				}

				//only draw enemies
				if (entity.team != local_entity.team) {
					esp::draw_esp(entity, local_entity, view_matrix, entity.team, local_entity.team);
				}
				

			}

		}
	
		overlay.EndRender();
	}

}