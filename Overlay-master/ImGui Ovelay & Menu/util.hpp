#pragma once

namespace util{
	Vector3 WTS(Vector3 vec, view_matrix_t matrix, int screenHeight = 1080, int screenWidth = 1920)
	{
		float _x = matrix[0][0] * vec.x + matrix[0][1] * vec.y + matrix[0][2] * vec.z + matrix[0][3];
		float _y = matrix[1][0] * vec.x + matrix[1][1] * vec.y + matrix[1][2] * vec.z + matrix[1][3];

		float w = matrix[3][0] * vec.x + matrix[3][1] * vec.y + matrix[3][2] * vec.z + matrix[3][3];

		if (w < 0.01f)
			return {};

		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float x = screenWidth / 2;
		float y = screenHeight / 2;

		x += 0.5f * _x * screenWidth + 0.5f;
		y -= 0.5f * _y * screenHeight + 0.5f;

		return { x,y,w };
	}
}
