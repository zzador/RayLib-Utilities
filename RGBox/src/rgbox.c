// RGBox layout for RayGui
#include <stdlib.h>
#include <string.h>

#include "rgbox.h"

void CreateRootRGBox(const uint32_t type, const uint32_t width, const uint32_t height, RGBox* pBox)
{
	*pBox = (RGBox){NULL, NULL, NULL, (Rectangle) {0, 0, 0, 0}, type, 0, width, height, 0, 0, 0, 0};
}

void CreateRGBox(RGBox* pParent, const uint32_t type, const uint32_t weight, const uint32_t minWidth, const uint32_t minHeight, RGBox* pBox)
{
	*pBox = (RGBox){pParent->pLastChild, pParent, NULL, (Rectangle) {0, 0, 0, 0}, type, weight, minWidth, minHeight, 0, 0, 0, 0};
	pParent->pLastChild = pBox;
}

static void GetMinSize(const RGBox* pBox, uint32_t* pMinWidth, uint32_t* pMinHeight)
{
	uint32_t w2, h2, w = 0, h = 0;
	
	if (pBox->type == RGBOX_HORIZONTAL)
	{
		RGBox* pChild = pBox->pLastChild;
		while (pChild != NULL)
		{
			GetMinSize(pChild, &w2, &h2);
			
			w += w2;
			h = h < h2 ? h2 : h;
			
			pChild = pChild->pPrev;
		}
	}
	else
	{
		RGBox* pChild = pBox->pLastChild;
		while (pChild != NULL)
		{
			GetMinSize(pChild, &w2, &h2);
			
			h += h2;
			w = w < w2 ? w2 : w;
			
			pChild = pChild->pPrev;
		}
	}
	
	*pMinWidth = pBox->minWidth < w ? w : pBox->minWidth;
	*pMinHeight = pBox->minHeight < h ? h : pBox->minHeight;
}

void LayoutRGBox(RGBox* pBox)
{
	float ds, startPos, childSize, pos0, pos, dynSpace;
	uint32_t size, minW, minH, minSize, paddingX, paddingY, weightSum = 0;
	
	if (pBox->pParent == NULL)
		pBox->rectangle = (Rectangle){0, 0, pBox->minWidth, pBox->minHeight};
	
	// Get weight sum
	RGBox* pChild = pBox->pLastChild;
	while (pChild != NULL)
	{
		weightSum += pChild->weight;
		pChild = pChild->pPrev;
	}
	
	// Get dynamic space
	ds = weightSum > 0 ? 1.0f / (float) weightSum : 0.0f;
	size = pBox->type == RGBOX_HORIZONTAL ? pBox->rectangle.width : pBox->rectangle.height;
	dynSpace = size;
	startPos = size;
	
	pChild = pBox->pLastChild;
	while (pChild != NULL)
	{
		GetMinSize(pChild, &minW, &minH);
		childSize = ((float) pChild->weight * ds) * size;
		minSize = pBox->type == RGBOX_HORIZONTAL ? minW : minH;
		
		dynSpace -= childSize < minSize ? minSize : 0;
		startPos -= weightSum == 0 ? (childSize < minSize ? minSize : childSize) : 0.0f;
		
		pChild = pChild->pPrev;
	}
	
	// Layout children
	startPos = weightSum == 0 ? startPos : 0.0f;
	pos0 = pBox->type == RGBOX_HORIZONTAL ? pBox->rectangle.y : pBox->rectangle.x;
	pos = pBox->type == RGBOX_HORIZONTAL ? pBox->rectangle.x + pBox->rectangle.width : pBox->rectangle.y + pBox->rectangle.height;
	
	pChild = pBox->pLastChild;
	while (pChild != NULL)
	{
		GetMinSize(pChild, &minW, &minH);
		childSize = ((float) pChild->weight * ds) * dynSpace;
		paddingX = pChild->paddingLeft + pChild->paddingRight;
		paddingY = pChild->paddingTop + pChild->paddingBottom;
		
		if (pBox->type == RGBOX_HORIZONTAL)
		{
			minSize = minW;
			
			if (childSize < minSize)
				pChild->rectangle =
					(Rectangle){pos - minSize + pChild->paddingLeft - startPos, pos0 + pChild->paddingTop, minSize - paddingX, pBox->rectangle.height - paddingY};
			else
				pChild->rectangle =
					(Rectangle){pos - childSize + pChild->paddingLeft - startPos, pos0 + pChild->paddingTop, childSize - paddingX, pBox->rectangle.height - paddingY};
			
			pos -= pChild->rectangle.width + paddingX;
		}
		else
		{
			minSize = minH;
			
			if (childSize < minSize)
				pChild->rectangle =
					(Rectangle){pos0 + pChild->paddingLeft, pos - minSize + pChild->paddingTop - startPos, pBox->rectangle.width - paddingX, minSize - paddingY};
			else
				pChild->rectangle =
					(Rectangle){pos0 + pChild->paddingLeft, pos - childSize + pChild->paddingTop - startPos, pBox->rectangle.width - paddingX, childSize - paddingY};
			
			pos -= pChild->rectangle.height + paddingY;
		}
		
		LayoutRGBox(pChild);
		
		pChild = pChild->pPrev;
	}
}
