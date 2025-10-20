#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "driver.hpp"
#include "mem.hpp"

class Base_Entity {

public:

	uintptr_t base_addr;
	uintptr_t player_pawn_addr;
	Memory memory;
	Vector3 pos_3d;
	Vector3 head_pos_3d; //temp til i figure out bones
	Vector3 look_angle;
	std::string player_name;
	int team;
	int life_state;
	int health;

	Base_Entity(uintptr_t base_addr, Memory memory) 
	{
		this->base_addr = base_addr;
		this->memory = memory;
	}

	Base_Entity()
	{

	}
	
	void init_entity(int player_index, uintptr_t entity_list) 
	{
		uintptr_t player_pawn = memory.RPM<uintptr_t>(base_addr + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
		
		uintptr_t list_entry2 = memory.RPM<uintptr_t>(entity_list + 0x8 * ((player_pawn & 0x7FFF) >> 9) + 16);
		if (!list_entry2) return;

		player_pawn_addr = memory.RPM<uintptr_t>(list_entry2 + 120 * (player_pawn & 0x1FF));
		if (!player_pawn_addr) return;

		uintptr_t game_scene_ptr = memory.RPM<uintptr_t>(player_pawn_addr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode);
		pos_3d = memory.RPM<Vector3>(game_scene_ptr + cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecAbsOrigin);

		uintptr_t name_ptr = memory.RPM<uintptr_t>(base_addr + cs2_dumper::schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName);
		player_name = memory.read_game_string(name_ptr, 128);

		Vector3 head_pos = pos_3d;
		head_pos.z += 75;
		head_pos_3d = head_pos;

		team = memory.RPM<int>(player_pawn_addr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

		life_state = memory.RPM<int>(player_pawn_addr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_lifeState);

		health = memory.RPM<int>(player_pawn_addr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);

		look_angle = memory.RPM<Vector3>(player_pawn_addr + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_angEyeAngles);

	}

};