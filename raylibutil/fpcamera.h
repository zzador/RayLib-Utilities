#ifndef FPCAMERA_H
#define FPCAMERA_H

#include <stdint.h>

#include <raylib.h>

// Basic first person free-fly cam. Use this as template for more advanced cameras.

#define FPCAMERA_NUM_MOVE_KEYS	7

enum FPCameraMoveKeys
{
	MOVE_SPRINT = 0,
	MOVE_LEFT = 1,
	MOVE_RIGHT = 2,
	MOVE_FORWARD = 3,
	MOVE_BACKWARD = 4,
	MOVE_UP = 5,
	MOVE_DOWN = 6
};

typedef struct FPCamera_s FPCamera_t;

struct FPCamera_s
{
	Camera camera3d;
	Vector3 moveSpeed, sprintSpeed, direction;
	Vector2 turnSpeed, lastMousePos, turnAngle;
	float aspect, turnYMin, turnYMax;
	int sprinting, moveKeys[FPCAMERA_NUM_MOVE_KEYS];
};

extern void FPCamera_Init(const Vector3 position, const Vector3 direction, const Vector3 up, const float fov, const float aspect, FPCamera_t* pCamera);
extern void FPCamera_Update(FPCamera_t* pCamera);

#endif
