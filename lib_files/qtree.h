#ifndef QTREE_H
#define QTREE_H

/**
 * @brief AABB struct with x, y, width and height and an id pointer for identification after querying
 */
typedef struct AABB
{
	float x, y;
	float w, h;
	void *id;
} AABB;

/**
 * @brief The QuadTree struct
 */
typedef struct QTree
{
	AABB **entries;
	AABB bounds;
	int maxEntries, numEntries, subdivided;
	struct QTree *children[4];
} QTree;

/**
 * @brief Tests if two AABB's overlap
 * 
 * @param a 
 * @param b 
 * @return 1 for overlap, 0 for no overlap
 */
int AABBoverlaps(AABB *a, AABB *b);

/**
 * @brief Tests if a point is in the bounds of an AABB
 * 
 * @param a The AABB
 * @param x The point's x coordinate
 * @param y The point's y coordinate
 * @return 1 for yes, 0 for no
 */
int PointInsideAABB(AABB *a, float x, float y);

/**
 * @brief A function to create a new QuadTree
 * 
 * @param bounds The boundaries of the tree
 * @param maxEntries The maximum entries one branch of the tree holds
 * @return QTree*
 */
QTree *newQTree(AABB bounds, int maxEntries);

/**
 * @brief Frees any given QuadTree
 * 
 * @param q The tree to free
 */
void freeQTree(QTree *q);

/**
 * @brief Adds an AABB* to given QuadTree
 * 
 * @param q QuadTree to add to
 * @param entry AABB to add
 * 
 */
void QTreeAdd(QTree *q, AABB *entry);

/**
 * @brief Queries an array of AABBs in a given range
 * 
 * @param q QuadTree to query
 * @param range The bounds to check AABBs in
 * @param out Array of AABBs returned
 * @param l Variable to store amount of AABBs returned
 * @param maxLen The maximum amount of AABBs returned
 * 
 * @note When there are more overlapping AABBs than maxLen, the function will return, so for the sake of memory-safety
 * this function might not always return all overlapping AABBs.
 * @note When an AABB fits inside two quadrants, it gets added to both, so when you query, you sometimes get the same
 * AABB two times, which I don't know how to fix without it being very slow
 */
void QTreeQuery(QTree *q, AABB range, AABB *out[], int *l, int maxLen);

#endif