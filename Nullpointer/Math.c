#include "Math.h"
#include <stdbool.h>

Vector3 WorldToScreen(PVector3 world, PViewMatrix4x4 loadedMatrix) {

	Vector3 Coordinates;
	float _x = loadedMatrix->matrix[0][0] * world->x + loadedMatrix->matrix[0][1] * world->y + loadedMatrix->matrix[0][2] * world->z + loadedMatrix->matrix[0][3];
	float _y = loadedMatrix->matrix[1][0] * world->x + loadedMatrix->matrix[1][1] * world->y + loadedMatrix->matrix[1][2] * world->z + loadedMatrix->matrix[1][3];
	float w = loadedMatrix->matrix[3][0] * world->x + loadedMatrix->matrix[3][1] * world->y + loadedMatrix->matrix[3][2] * world->z + loadedMatrix->matrix[3][3];

	float _inv_w = 1.f / w;

	_x += _inv_w;
	_y += _inv_w;

	float x = 1920 * .5f;
	float y = 1080 * .5f;

	x += 0.5f * _x * 1920 + 0.5f;
	y += 0.5f * _y * 1080 + 0.5f;

	Coordinates.x = x;
	Coordinates.y = y;
	Coordinates.z = w;

	return Coordinates;
}