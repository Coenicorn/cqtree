#include "aabb.h"

int AABBoverlaps(AABB *a, AABB *b)
{
	return !(
			a->x > b->x + b->w ||
			a->y > b->y + b->h ||
			a->x + a->w < b->x ||
			a->y + a->h < b->y
		);
}

int PointInsideAABB(AABB *a, float x, float y)
{
	return (
			x > a->x &&
			x < a->x + a->w &&
			y > a->y &&
			y < a->y + a->h
	       );
}
