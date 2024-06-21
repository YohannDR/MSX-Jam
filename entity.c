#include "entity.h"
#include "string.h"
#include "debug.h"
#include "memory.h"

#include "player.h"
#include "ball.h"

struct Entity gEntities[ENTITY_AMOUNT];

const EntityUpdateFunc sEntitiesAiPointers[ENTITY_COUNT] = {
    [ENTITY_PLAYER] = Player,
    [ENTITY_BALL] = Ball
};


struct Entity* EntityInit(enum EntityId entityId, u16 x, u16 y)
{
    for (u32 i = 0; i < ENTITY_AMOUNT; i++)
    {
        struct Entity* e = &gEntities[i];

        if (e->status & ESTATUS_EXISTS)
            continue;

        e->status = ESTATUS_EXISTS;
        e->position.x = x;
        e->position.y = y;
        e->entityId = entityId;

        e->pose = 0;

        sEntitiesAiPointers[entityId](e);

        // TODO bg sync
    	GamePawn_SetPosition(&e->pawn, x, y);

        return e;
    }

    return NULL;
}

struct Entity* EntityFind(enum EntityId entityId)
{
    for (u32 i = 0; i < ENTITY_AMOUNT; i++)
    {
        struct Entity* e = &gEntities[i];

        if ((e->status & ESTATUS_EXISTS) && e->entityId == entityId)
            return e;
    }

    return NULL;
}

void EntitiesSetup(void)
{
    Mem_Set(0, gEntities, sizeof(gEntities));
}

void EntitiesDraw(void)
{
    for (u32 i = 0; i < ENTITY_AMOUNT; i++)
    {
        struct Entity* e = &gEntities[i];

        if (!(e->status & ESTATUS_EXISTS) || (e->status & ESTATUS_NOT_DRAWN))
            continue;

        GamePawn_Draw(&e->pawn);
    }
}

void EntitiesUpdate(void)
{
    for (u32 i = 0; i < ENTITY_AMOUNT; i++)
    {
        struct Entity* e = &gEntities[i];

        if (!(e->status & ESTATUS_EXISTS))
            continue;

        c8 buffer[32];
        String_Format(buffer, "%i ; %i", i, e->entityId);

        DEBUG_LOG(buffer);

        sEntitiesAiPointers[e->entityId](e);

        // TODO bg sync
    	GamePawn_SetPosition(&e->pawn, e->position.x, e->position.y);
        GamePawn_Update(&e->pawn);
    }
}
