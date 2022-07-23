/*

C implementation of a QuadTree.

Might not be the best version out there, but I really don't like using code other people wrote lol.
There is no license, so if you're crazy enough to use this, you're allowed to!

Quick tip for IF you want to use this (y tho):

The AABB struct has a completely optional id member, which is a generic pointer,
so you can identify the AABB after the query.

Love you!

-Coenicorn

*/
#include <stdlib.h>

#include "qtree.h"
#include "aabb.h"

// kinda redundant, might be good in the future tho idk
#define TOPLEFT 0
#define TOPRIGHT 1
#define BOTTOMLEFT 2
#define BOTTOMRIGHT 3

QTree *newQTree(AABB bounds, int maxEntries)
{
	QTree *out = malloc(sizeof(QTree));

	out->maxEntries = maxEntries;
	out->numEntries = out->subdivided = 0;
	out->bounds = bounds;
	out->entries = malloc(sizeof(AABB *) * maxEntries);

	return out;
}

void freeQTree(QTree *q)
{
	// don't check children... if it doesn't have any lol
	if (q->subdivided)
	{
		for (int i = 0; i < 4; i++)
		{
			freeQTree(q->children[i]);
		}
	}
	free(q->entries);
	free(q);
}

void QTreeSubdivide(QTree *q)
{
	AABB b = q->bounds;

	float cx = b.x + b.w / 2;
	float cy = b.y + b.h / 2;
	float hw = b.w / 2;
	float hh = b.h / 2;

	q->children[TOPLEFT] = newQTree((AABB){cx - hw, cy - hh, hw, hh}, q->maxEntries);
	q->children[TOPRIGHT] = newQTree((AABB){cx, cy - hh, hw, hh}, q->maxEntries);
	q->children[BOTTOMLEFT] = newQTree((AABB){cx - hw, cy, hw, hh}, q->maxEntries);
	q->children[BOTTOMRIGHT] = newQTree((AABB){cx, cy, hw, hh}, q->maxEntries);

	q->subdivided = 1;
}

void QTreeAdd(QTree *q, AABB *entry)
{
	// if the entry isn't inside the bounds of this QTree, do nothing
	if (!AABBoverlaps(&q->bounds, entry))
		return;

	// check if the entry fits inside the QTree
	// array index = human index - 1
	if (q->numEntries >= q->maxEntries)
	{
		// if not subdivided, subdivide
		if (!q->subdivided)
		{
			QTreeSubdivide(q);
		}

		// try to insert in all children
		for (int i = 0; i < 4; i++)
		{
			QTreeAdd(q->children[i], entry);
		}

		return;
	}

	// insert into this QTree
	q->entries[q->numEntries++] = entry;
}

void QTreeQuery(QTree *q, AABB range, AABB *out[], int *l, int maxLen)
{
	if (*l >= maxLen)
			return;
			
	// check if the bounds overlap with QTree
	if (!AABBoverlaps(&q->bounds, &range))
		return;

	// check if any of the entries overlap with bounds given
	for (int i = 0; i < q->maxEntries && *l < maxLen; i++)
	{
		if (q->entries[i] == NULL || !AABBoverlaps(q->entries[i], &range))
			continue;

		out[(*l)++] = q->entries[i];
	}

	if (!q->subdivided)
		return;

	// check with all children
	for (int i = 0; i < 4; i++)
	{
		QTreeQuery(q->children[i], range, out, l, maxLen);
	}
}