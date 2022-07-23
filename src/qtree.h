#ifndef QTREE_H
#define QTREE_H

#include "aabb.h"

typedef struct QTree
{
	AABB **entries;
	AABB bounds;
	int maxEntries, numEntries, subdivided;
	struct QTree *children[4];
} QTree;

QTree *newQTree(AABB bounds, int maxEntries);
void freeQTree(QTree *q);
void QTreeAdd(QTree *q, AABB *entry);
/* Queries entries from QTree and puts the amount of them in l, expects l to be declared */
void QTreeQuery(QTree *q, AABB range, AABB *out[], int *l, int maxLen);

#endif