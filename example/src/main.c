#include <stdlib.h>
#include <stdio.h>
#include <qtree.h>
#include <time.h>
#include <raylib.h>

// screen dimensions
#define SCREENWIDTH 500
#define SCREENHEIGHT 500

// make this however big you want... if you dare ;)
#define MAX_ENTITIES 100

#define MINWIDTH 10
#define MAXWIDTH 30
#define MINHEIGHT 10
#define MAXHEIGHT 30

// helper function for random numbers
float randrange(float a, float b)
{
	float diff = b - a;
	return (a + (float)rand() / (float)RAND_MAX * (float)diff);
}

// example gameobject stuff to demonstrate the id
typedef struct Entity
{
    AABB aabb;
    Vector2 velocity;
	Color color;
} Entity;

// update function
void update(QTree *q, Entity *objects)
{
    // update entities
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        Entity *e = &objects[i];

        // update positions
        e->aabb.x += e->velocity.x;
        e->aabb.y += e->velocity.y;

        // collide with walls
        if (e->aabb.x + e->aabb.w > SCREENWIDTH)
        {
            e->velocity.x *= -1;
            e->aabb.x = SCREENWIDTH - e->aabb.w;
        }
        if (e->aabb.x < 0)
        {
            e->velocity.x *= -1;
            e->aabb.x = 0;
        }
        if (e->aabb.y + e->aabb.h > SCREENHEIGHT)
        {
            e->velocity.y *= -1;
            e->aabb.y = SCREENHEIGHT - e->aabb.h;
        }
        if (e->aabb.y < 0)
        {
            e->velocity.y *= -1;
            e->aabb.y = 0;
        }
    }

    // check collisions
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        Entity *e = &objects[i];

        if (e == NULL)
            continue;

        e->color = GREEN;

        AABB *overlapsWith[100];
        int len = 0;

        QTreeQuery(q, e->aabb, overlapsWith, &len, 100);

        for (int i = 0; i < len; i++)
        {
            // check if the entity isn't overlapping with itself
            Entity *o = overlapsWith[i]->id;

            if (o != e)
                e->color = RED;            
        }
    }
}

int main()
{
    // initialize random number generator
    srand(time(NULL));

    // initialize object array
    Entity *objects = calloc(MAX_ENTITIES, sizeof(Entity));
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        objects[i] = (Entity){
            // choose some random values
            (AABB){randrange(0, SCREENWIDTH), randrange(0, SCREENHEIGHT), randrange(MINWIDTH, MAXWIDTH), randrange(MINHEIGHT, MAXHEIGHT), &objects[i]},
            (Vector2){randrange(-1, 1), randrange(-1, 1)},
			GREEN
        };
    }

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Agent in C");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        QTree *quadtree = newQTree((AABB){0, 0, 500, 500}, 10);

        // build the QuadTree
        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            Entity *e = &objects[i];

            QTreeAdd(quadtree, &e->aabb);
        }

        update(quadtree, objects);

        // VERY IMPORTANT, ALWAYS DO THIS!!!
        freeQTree(quadtree);

        BeginDrawing();
        ClearBackground(WHITE);

        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            Entity *e = &objects[i];

            DrawRectangle(e->aabb.x, e->aabb.y, e->aabb.w, e->aabb.h, e->color);
        }

        DrawFPS(0, 0);

        EndDrawing();
    }

    CloseWindow();

    // VERY IMPORTANT
    free(objects);

    return 0;
}