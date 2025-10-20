
namespace render {
	void DrawRect(int x, int y, int w, int h, ImColor color, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, thickness);
	}

	void Line(float x1, float y1, float x2, float y2, ImColor color, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
	}
}
