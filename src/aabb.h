#ifndef AABB_H
#define AABB_H

typedef struct AABB
{
	float x, y;
	float w, h;

	// completely optional for the quadtree, used for identification with querying
	void *id;
} AABB;

int AABBoverlaps(AABB *a, AABB *b);
int PointInsideAABB(AABB *a, float x, float y);

#endif
