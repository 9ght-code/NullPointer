#include "Math.h"

int WorldToScreen(PVector3 object, PVector2 screen, PViewMatrix4x4 loadedMatrix, const int WINDOW_WIDTH, const int WINDOW_HEIGHT) {

	float x = loadedMatrix->matrix[0][0] * object->x + loadedMatrix->matrix[0][1] * object->y + loadedMatrix->matrix[0][2] * object->z + loadedMatrix->matrix[0][3];
	float y = loadedMatrix->matrix[1][0] * object->x + loadedMatrix->matrix[1][1] * object->y + loadedMatrix->matrix[1][2] * object->z + loadedMatrix->matrix[1][3];
	float w = loadedMatrix->matrix[3][0] * object->x + loadedMatrix->matrix[3][1] * object->y + loadedMatrix->matrix[3][2] * object->z + loadedMatrix->matrix[3][3];

	if (w < 0.1)
		return 0;

	Vector2 ndc;

	ndc.x = x / w;
	ndc.y = y / w;

	screen->x = (WINDOW_WIDTH / 2 * ndc.x) + (ndc.x + WINDOW_WIDTH / 2);
	screen->y = -(WINDOW_HEIGHT / 2 * ndc.y) + (ndc.y + WINDOW_HEIGHT / 2);

	return 1;


}


float CalculateDistance3D(PVector3 entityPos1, PVector3 entityPos2) {
	if (!entityPos1 || !entityPos2) {
		printf("null vectors");
		return 0;
	}

	return (
		sqrt(pow(entityPos2->x - entityPos1->x, 2)) +
		sqrt(pow(entityPos2->y - entityPos1->y, 2)) +
		sqrt(pow(entityPos2->z - entityPos1->z, 2))
		);
}



float CalculateDistance2D(PVector2 entityPos1, PVector2 entityPos2) {
	if (!entityPos1 || !entityPos2) {
		printf("null vectors");
		return 0;
	}

	return (
		sqrt(pow(entityPos2->x - entityPos1->x, 2)) +
		sqrt(pow(entityPos2->y - entityPos1->y, 2))
		);
}