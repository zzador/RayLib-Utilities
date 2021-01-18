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
	uint32_t size, minW, minH, minSize, marginX, marginY, weightSum = 0;
	
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
	pChild = pBox->pLastChild;
	ds = weightSum > 0 ? 1.0f / (float) weightSum : 0.0f;
	size = pBox->type == RGBOX_HORIZONTAL ? pBox->rectangle.width : pBox->rectangle.height;
	dynSpace = size;
	startPos = size;
	
	while (pChild != NULL)
	{
		GetMinSize(pChild, &minW, &minH);
		childSize = ((float) pChild->weight * ds) * size;
		minSize = pBox->type == RGBOX_HORIZONTAL ? minW : minH;
		
		if (childSize < minSize)
			dynSpace -= minSize;
		
		startPos -= weightSum == 0 ? (childSize < minSize ? minSize : childSize) : 0.0f;
		
		pChild = pChild->pPrev;
	}
	
	// Layout children
	pChild = pBox->pLastChild;
	startPos = weightSum == 0 ? startPos : 0.0f;
	pos0 = pBox->type == RGBOX_HORIZONTAL ? pBox->rectangle.y : pBox->rectangle.x;
	pos = pBox->type == RGBOX_HORIZONTAL ? pBox->rectangle.x + pBox->rectangle.width : pBox->rectangle.y + pBox->rectangle.height;
	
	while (pChild != NULL)
	{
		GetMinSize(pChild, &minW, &minH);
		childSize = ((float) pChild->weight * ds) * dynSpace;
		marginX = pChild->marginLeft + pChild->marginRight;
		marginY = pChild->marginTop + pChild->marginBottom;
		
		if (pBox->type == RGBOX_HORIZONTAL)
		{
			minSize = minW;
			
			if (childSize < minSize)
				pChild->rectangle =
					(Rectangle){pos - minSize + pChild->marginLeft - startPos, pos0 + pChild->marginTop, minSize - marginX, pBox->rectangle.height - marginY};
			else
				pChild->rectangle =
					(Rectangle){pos - childSize + pChild->marginLeft - startPos, pos0 + pChild->marginTop, childSize - marginX, pBox->rectangle.height - marginY};
			
			pos -= pChild->rectangle.width + marginX;
		}
		else
		{
			minSize = minH;
			
			if (childSize < minSize)
				pChild->rectangle =
					(Rectangle){pos0 + pChild->marginLeft, pos - minSize + pChild->marginTop - startPos, pBox->rectangle.width - marginX, minSize - marginY};
			else
				pChild->rectangle =
					(Rectangle){pos0 + pChild->marginLeft, pos - childSize + pChild->marginTop - startPos, pBox->rectangle.width - marginX, childSize - marginY};
			
			pos -= pChild->rectangle.height + marginY;
		}
		
		LayoutRGBox(pChild);
		
		pChild = pChild->pPrev;
	}
}
