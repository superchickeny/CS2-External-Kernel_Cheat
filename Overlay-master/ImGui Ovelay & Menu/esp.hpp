#pragma once

namespace esp {
	void draw_box(Vector3 foot_pos, Vector3 head_pos, ImColor color)
	{
		float height = foot_pos.y - head_pos.y;
		float width = height / 2.4f;

		int rectBottomX = head_pos.x;
		int rectBottomY = head_pos.y + height;

		int bottomCenterX = 1920 / 2;
		int bottomCenterY = 1080;


		render::DrawRect(head_pos.x - width / 2, head_pos.y, width, height, color, 1);
		
	}

	//kinda works ish
	void draw_box_3d(Vector3 foot_pos_3d, Vector3 head_pos_3d, ImColor color, view_matrix_t matrix, Vector3 forward_dir)
	{
	
		// Move 50 units behind along player's backward direction
	
		Vector3 offset = forward_dir * -10.0f;
		Vector3 foot_world = foot_pos_3d + offset;
		Vector3 head_world = head_pos_3d + offset;

		// Project to screen
		Vector3 foot_screen = util::WTS(foot_world, matrix);
		Vector3 head_screen = util::WTS(head_world, matrix);

		offset = forward_dir * 10.0f;
		Vector3 foot_world1 = foot_pos_3d + offset;
		Vector3 head_world1 = head_pos_3d + offset;

		Vector3 foot_screen1 = util::WTS(foot_world1, matrix);
		Vector3 head_screen1 = util::WTS(head_world1, matrix);

		float height = foot_screen.y - head_screen.y;
		float width = height / 2.4f;

		render::DrawRect(head_screen.x - width / 2, head_screen.y, width, height, color, 0.5f);
		render::DrawRect(head_screen1.x - width / 2, head_screen1.y, width, height, color, 0.5f);

		
		//top lines
		render::Line(head_screen.x - width / 2, head_screen.y, head_screen1.x - width / 2, head_screen1.y, color, 0.5f);
		render::Line(head_screen.x + width / 2, head_screen.y, head_screen1.x + width / 2, head_screen1.y, color, 0.5f);
		//bottom lines
		render::Line(head_screen.x - width / 2, head_screen.y + height - 1, head_screen1.x - width / 2, head_screen1.y + height - 1, color, 0.5f);
		render::Line(head_screen.x + width / 2 + 1, head_screen.y + height - 1,head_screen1.x + width / 2, head_screen1.y + height - 1, color, 0.5f);
	}
	
#define M_PI 3.14159
	Vector3 angle_to_forward(const Vector3& ang) {
		float pitch_rad = ang.x * (M_PI / 180.0f); // pitch
		float yaw_rad = ang.y * (M_PI / 180.0f); // yaw

		Vector3 forward;
		forward.x = cosf(pitch_rad) * cosf(yaw_rad);
		forward.y = cosf(pitch_rad) * sinf(yaw_rad);
		forward.z = -sinf(pitch_rad);  // negative because up = -90
		return forward;
	}

	void draw_esp(Base_Entity entity, Base_Entity local_entity, view_matrix_t view_matrix, int current_player_team, int local_player_team)
	{

		ImColor color;
		Vector3 forward_dir = angle_to_forward(entity.look_angle);

		if (current_player_team == local_player_team) {
			color = ImColor(0.0f, 0.0f, 0.5f, 1.0f);
		}
		else {
			color = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
		}

		Vector3 foot_pos_2d = util::WTS(entity.pos_3d, view_matrix);
		Vector3 head_pos_2d = util::WTS(entity.head_pos_3d, view_matrix);
		float height = foot_pos_2d.y - head_pos_2d.y;
		float width = height / 2.4f;
		
		//checking if it front of player
		if (head_pos_2d.x - width / 2 >= 0 &&
			head_pos_2d.x + width / 2 <= 1920 &&
			head_pos_2d.y >= 0 &&
			head_pos_2d.y + height <= 1080 &&
			head_pos_2d.z > 0) {

			esp::draw_box(foot_pos_2d, head_pos_2d, color);

			float box_x = head_pos_2d.x - (width / 2.0f);
			float box_y = head_pos_2d.y;

			//hp bar
			float hp_height = height * ((float)entity.health / 100.0f);
			float hp_bar_top_y = foot_pos_2d.y - hp_height;

			// Left edge of the box
			render::Line(box_x - 2, (box_y + height) - 2, box_x - 2, hp_bar_top_y, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 1);

			//draw name
			ImGui::GetBackgroundDrawList()->AddText(
				ImGui::GetFont(),
				12.0f,
				ImVec2(head_pos_2d.x + width / 2, head_pos_2d.y),
				ImColor(1.0f, 1.0f, 1.0f, 1.0f),
				entity.player_name.c_str()
			);

			float dx = local_entity.pos_3d.x - entity.pos_3d.x;
			float dy = local_entity.pos_3d.y - entity.pos_3d.y;
			float dz = local_entity.pos_3d.z - entity.pos_3d.z;

			float distance_xyz = std::sqrt(dx * dx + dy * dy + dz * dz);

			//divide by 10 because large ish number
			std::string dist_str = std::to_string((int)distance_xyz / 10);
			dist_str = dist_str + "m";

			ImGui::GetBackgroundDrawList()->AddText(
				ImGui::GetFont(),
				12.0f,
				ImVec2(foot_pos_2d.x - width / 2, foot_pos_2d.y),
				ImColor(1.0f, 1.0f, 1.0f, 1.0f),
				dist_str.c_str()
			);

			Vector3 line_end;
			line_end.x = entity.head_pos_3d.x + forward_dir.x * 200.0f;
			line_end.y = entity.head_pos_3d.y + forward_dir.y * 200.0f;
			line_end.z = entity.head_pos_3d.z + forward_dir.z * 200.0f;
			
			Vector3 end_2d = util::WTS(line_end, view_matrix);

			
			ImGui::GetForegroundDrawList()->AddLine(
				ImVec2(head_pos_2d.x, head_pos_2d.y),
				ImVec2(end_2d.x, end_2d.y),
				IM_COL32(255, 0, 255, 255), // color: light purple
				0.5f                         // thickness
			);

			render::DrawRect(end_2d.x, end_2d.y, 5, 5, IM_COL32(255, 0, 255, 255), 1.0f);
			
			

		}

		
	}
}

