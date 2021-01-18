#ifndef RGBOX_H
#define RGBOX_H

#include <stdint.h>
#include <raylib.h>

enum RGBoxType
{
	RGBOX_HORIZONTAL,
	RGBOX_VERTICAL
};

enum RGBoxMargin
{
	RGBOX_TOP,
	RGBOX_RIGHT,
	RGBOX_BOTTOM,
	RGBOX_LEFT
};

typedef struct RGBox RGBox;

struct RGBox
{
	RGBox* pPrev;
	RGBox* pParent;
	RGBox* pLastChild;
	Rectangle rectangle;
	uint32_t type, weight, minWidth, minHeight, margin[4];
};

extern void CreateRootRGBox(const uint32_t type, const uint32_t width, const uint32_t height, RGBox* pBox);
extern void CreateRGBox(RGBox* pParent, const uint32_t type, const uint32_t weight, const uint32_t minWidth, const uint32_t minHeight, RGBox* pBox);
extern void LayoutRGBox(RGBox* pBox);

#endif
