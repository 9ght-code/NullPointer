#pragma once

typedef struct _Vector3 {
	float x, y, z;
}Vector3, * PVector3;

typedef struct _ViewMatrix4x4 {

	float matrix[4][4];

}ViewMatrix4x4, * PViewMatrix4x4;

typedef struct _Vector4 {
	float x, y, z, w;
}Vector4, * PVector4;

typedef struct _Vector2 {
	float x, y;
}Vector2, *PVector2;

Vector3 WorldToScreen(PVector3 world, PViewMatrix4x4 loadedMatrix);
float CalculateDistance(PVector3 entityPos1, PVector3 entityPos2);