#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <raymath.h>

#include "fpcamera.h"

const float FPCAMERA_TURNY_MIN = 0.001f;
const float FPCAMERA_TURNY_MAX = (PI-0.001f);

const Vector3 FPCAMERA_DEFAULT_MOVESPEED = (Vector3) {15.0f, 15.0f, 15.0f};
const Vector3 FPCAMERA_DEFAULT_SPRINTSPEED = (Vector3) {50.0f, 30.0f, 30.0f};
const Vector2 FPCAMERA_DEFAULT_TURNSPEED = (Vector2) {6.0f, 6.0f};

const int FPCAMERA_DEFAULT_MOVEKEYS[FPCAMERA_NUM_MOVE_KEYS] = {KEY_LEFT_SHIFT, KEY_A, KEY_D, KEY_W, KEY_S, KEY_SPACE, KEY_C};

typedef void (*FPCamera_MoveFunc)(FPCamera_t* pCamera, const float frameTime);

static void Sprint(FPCamera_t* pCamera, const float frameTime)
{
	pCamera->sprinting = 1;
}

static void MoveLeft(FPCamera_t* pCamera, const float frameTime)
{
	Vector3 dir = Vector3Subtract(pCamera->camera3d.target, pCamera->camera3d.position);
	Vector3 left = Vector3CrossProduct(pCamera->camera3d.up, dir);
	
	pCamera->camera3d.position = Vector3Add(pCamera->camera3d.position, Vector3Scale(left, (pCamera->sprinting ? pCamera->sprintSpeed.x : pCamera->moveSpeed.x) * frameTime));
}

static void MoveRight(FPCamera_t* pCamera, const float frameTime)
{
	Vector3 dir = Vector3Subtract(pCamera->camera3d.target, pCamera->camera3d.position);
	Vector3 right = Vector3CrossProduct(dir, pCamera->camera3d.up);
	
	pCamera->camera3d.position = Vector3Add(pCamera->camera3d.position, Vector3Scale(right, (pCamera->sprinting ? pCamera->sprintSpeed.x: pCamera->moveSpeed.x) * frameTime));
}

static void MoveForward(FPCamera_t* pCamera, const float frameTime)
{
	Vector3 dir = Vector3Subtract(pCamera->camera3d.target, pCamera->camera3d.position);
	
	pCamera->camera3d.position = Vector3Add(pCamera->camera3d.position, Vector3Scale(dir, (pCamera->sprinting ? pCamera->sprintSpeed.z : pCamera->moveSpeed.z) * frameTime));
}

static void MoveBackward(FPCamera_t* pCamera, const float frameTime)
{
	Vector3 dir = Vector3Subtract(pCamera->camera3d.target, pCamera->camera3d.position);
	
	pCamera->camera3d.position = Vector3Add(pCamera->camera3d.position, Vector3Scale(dir, (pCamera->sprinting ? -pCamera->sprintSpeed.z : -pCamera->moveSpeed.z) * frameTime));
}

static void MoveUp(FPCamera_t* pCamera, const float frameTime)
{
	pCamera->camera3d.position = Vector3Add(pCamera->camera3d.position, Vector3Scale(pCamera->camera3d.up, (pCamera->sprinting ? pCamera->sprintSpeed.y : pCamera->moveSpeed.y) * frameTime));
}

static void MoveDown(FPCamera_t* pCamera, const float frameTime)
{
	pCamera->camera3d.position = Vector3Add(pCamera->camera3d.position, Vector3Scale(pCamera->camera3d.up, (pCamera->sprinting ? -pCamera->sprintSpeed.y : -pCamera->moveSpeed.y) * frameTime));
}

const FPCamera_MoveFunc FPCamera_MoveFunctions[FPCAMERA_NUM_MOVE_KEYS] =
{
	Sprint, MoveLeft, MoveRight, MoveForward, MoveBackward, MoveUp, MoveDown
};

void FPCamera_Init(const Vector3 position, const Vector3 direction, const Vector3 up, const float fov, const float aspect, FPCamera_t* pCamera)
{
	memset(pCamera, 0, sizeof(FPCamera_t));
	
	pCamera->camera3d.position = position;
	pCamera->camera3d.target = Vector3Add(position, direction);
	pCamera->camera3d.up = up;
	pCamera->camera3d.fovy = fov / aspect;
	pCamera->camera3d.type = CAMERA_PERSPECTIVE;
	
	pCamera->aspect = aspect;
	pCamera->turnYMin = FPCAMERA_TURNY_MIN;
	pCamera->turnYMax = FPCAMERA_TURNY_MAX;
	pCamera->moveSpeed = FPCAMERA_DEFAULT_MOVESPEED;
	pCamera->sprintSpeed = FPCAMERA_DEFAULT_SPRINTSPEED;
	pCamera->turnSpeed = FPCAMERA_DEFAULT_TURNSPEED;
	pCamera->lastMousePos = (Vector2){(GetScreenWidth() * 0.5f), (GetScreenHeight() * 0.5f)};
	
	// Direction to sphere coords
	pCamera->turnAngle.y = acos(direction.y);
	pCamera->turnAngle.x = atan2(direction.x, direction.z);
	
	memcpy(pCamera->moveKeys, FPCAMERA_DEFAULT_MOVEKEYS, FPCAMERA_NUM_MOVE_KEYS * sizeof(int));
}

void FPCamera_Update(FPCamera_t* pCamera)
{
	uint32_t i;
	const float frameTime = GetFrameTime();
	
	// Mouse handling (turning)
	Vector2 mousePos = GetMousePosition();
	if ((pCamera->lastMousePos.x == (GetScreenWidth() * 0.5f)) && (pCamera->lastMousePos.y == (GetScreenHeight() * 0.5f)))
		pCamera->lastMousePos = mousePos;
	
	Vector2 mouseDelta = Vector2Subtract(mousePos, pCamera->lastMousePos);
	pCamera->lastMousePos = mousePos;
	
	pCamera->turnAngle.x -= mouseDelta.x * pCamera->turnSpeed.x * frameTime;
	pCamera->turnAngle.y += mouseDelta.y * pCamera->turnSpeed.y * frameTime;
	
	// Clamp to min/max angle
	pCamera->turnAngle.y = pCamera->turnAngle.y < pCamera->turnYMin ? pCamera->turnYMin : (pCamera->turnAngle.y > pCamera->turnYMax ? pCamera->turnYMax : pCamera->turnAngle.y);
	
	// Key handling (moving)
	pCamera->sprinting = 0;
	for (i=0;i<FPCAMERA_NUM_MOVE_KEYS;i++)
		if (IsKeyDown(pCamera->moveKeys[i]))
			FPCamera_MoveFunctions[i](pCamera, frameTime);
	
	// Update camera target (spherecoords to direction to target)
	pCamera->direction = (Vector3){sin(pCamera->turnAngle.y) * sin(pCamera->turnAngle.x),
								  cos(pCamera->turnAngle.y),
								  sin(pCamera->turnAngle.y) * cos(pCamera->turnAngle.x)};
	
	pCamera->camera3d.target = Vector3Add(pCamera->camera3d.position, pCamera->direction);
}
