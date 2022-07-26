/*

This is an example program for how one might use the quadtree implementation in the parent directory
It is made with raylib, a free, open source game development graphics library (check it out at: https://raylib.com)

Ennjoy!

-Coenicorn

*/

#include <raylib.h>
#include <qtree.h>
#include <stdlib.h>
#include <time.h>

// Window dimensions
#define W_WIDTH 500
#define W_HEGIHT 500

#define NUM_ENTITIES 3000

// change this to see effect of qtree
#define USEQTREE 1

// Entity struct
typedef struct Entity
{
    AABB bounds;
    float velX, velY;
    int isColliding;
} Entity;

// global entities array, I know global variables are evil but I'm lazy
Entity *entities;

// random value between a and b
float randrange(float a, float b)
{
    float diff = b - a;
    return (a + (float)rand() / (float)RAND_MAX * diff);
}

void update(QTree *tree)
{
    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        Entity *e = &entities[i];

        // update positions
        e->bounds.x += e->velX;
        e->bounds.y += e->velY;

        // collide with walls
        if (e->bounds.x + e->bounds.w > W_WIDTH)
        {
            e->bounds.x = W_WIDTH - e->bounds.w;
            e->velX *= -1;
        }
        if (e->bounds.x < 0)
        {
            e->bounds.x = 0;
            e->velX *= -1;
        }
        if (e->bounds.y + e->bounds.h > W_HEGIHT)
        {
            e->bounds.y = W_HEGIHT - e->bounds.h;
            e->velY *= -1;
        }
        if (e->bounds.y < 0)
        {
            e->bounds.y = 0;
            e->velY *= -1;
        }
    }

    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        Entity *e = &entities[i];

        e->isColliding = 0;

        if (USEQTREE){
            int len = 0;
            AABB *a[20];

            // query tree with aabb slightly larger than the entity to get all potential collision cases
            QTreeQuery(tree, (AABB){e->bounds.x-5, e->bounds.y-5, e->bounds.w+10, e->bounds.h+10}, a, &len, 20);

            for (int j = 0; j < len; j++)
            {
                // ID's save the day!
                Entity *o = a[j]->id;

                // check if the entity isn't colliding with itself
                if (e == o)
                    continue;
                
                // if it is a close collision, entity is colliding!
                if (AABBoverlaps(&e->bounds, &o->bounds))
                {
                    e->isColliding = 1;
                    break;
                }
            }
        }
        else
        {
            for (int j = 0; j < NUM_ENTITIES; j++)
            {
                // Check if the entity isn't colliding with itself
                if (i == j)
                    continue;

                // Brute force collision detection with every other entity
                if (AABBoverlaps(&entities[i].bounds, &entities[j].bounds))
                {
                    e->isColliding = 1;
                    break;
                }
            }
        }
    }
}

int main()
{
    srand((unsigned int)clock());

    // Initialize entities array with aabb's with random positions
    entities = calloc(NUM_ENTITIES, sizeof(Entity));
    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        entities[i] = (Entity){
            // Add pointer to the entity at the end for identification
            (AABB){randrange(0, W_WIDTH), randrange(0, W_HEGIHT), 5, 5, &entities[i]},
            // random velocities
            randrange(-2, 2), randrange(-2, 2),
            // Entity hasn't collided yet, isColliding = 0
            0
        };
    }

    AABB QTreeBounds = {0, 0, W_WIDTH, W_HEGIHT};

    // Initialize raylib window
    InitWindow(W_WIDTH, W_HEGIHT, "Example usage of cqtree");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // build QTree
        QTree *tree = newQTree(QTreeBounds, 10);
        for (int i = 0; i < NUM_ENTITIES; i++)
        {
            QTreeAdd(tree, &entities[i].bounds);
        }

        update(tree);

        freeQTree(tree);

        // render entities
        BeginDrawing();
        ClearBackground(WHITE);

        for (int i = 0; i < NUM_ENTITIES; i++)
        {
            Entity *e = &entities[i];

            // entities that are colliding are green, those that are not are black
            Color c = BLACK;
            if (e->isColliding)
                c = RED;
            
            DrawRectangle(e->bounds.x, e->bounds.y, e->bounds.w, e->bounds.h, c);
        }

        DrawFPS(0, 0);

        EndDrawing();
    }

    CloseWindow();

    // In contrary to your dad, we ARE being safe
    free(entities);

    return 0;
}