#include "entity.h"
#include "string.h"
#include "debug.h"
#include "memory.h"
#include "vdp.h"

#include "player.h"
#include "scrolling.h"

struct Entity gEntities[ENTITY_AMOUNT];

const EntityUpdateFunc sEntitiesAiPointers[ENTITY_COUNT] = {
    [ENTITY_PLAYER] = Player
};

static void EntityDraw(struct Entity* e)
{
    GamePawn_Draw(&e->pawn);
}

static void EntitySetPawnPosition(struct Entity* e)
{
    GamePawn_SetPosition(&e->pawn, e->position.x - ScrollGetX(), e->position.y - ScrollGetY());
}

static void EntityCheckOnScreen(struct Entity* e)
{
    c8 buffer[50];
    u16 screenX = ScrollGetX();
    u16 screenY = ScrollGetY();
    u16 entityX = e->position.x;
    u16 entityY = e->position.y;

    String_Format(buffer, "Entity : %i ; %i | Screen : %i ; %i", entityX, entityY, screenX, screenY);
    DEBUG_LOG(buffer);

    if (entityX < screenX || entityY < screenY)
    {
        DEBUG_LOG("Off screen (behind)");
        e->status &= ~ESTATUS_ON_SCREEN;
        return;
    }

    if (entityX < screenX + 0xFF && entityY < screenY + 0xFF)
    {
        DEBUG_LOG("On screen");
        e->status |= ESTATUS_ON_SCREEN;
        return;
    }

    DEBUG_LOG("Off screen (elsewhere)");
    e->status &= ~ESTATUS_ON_SCREEN;
}

struct Entity* EntityInit(enum EntityId entityId, u16 x, u16 y)
{
    for (u32 i = 0; i < ENTITY_AMOUNT; i++)
    {
        struct Entity* e = &gEntities[i];

        if (e->status & ESTATUS_EXISTS)
            continue;

        e->status = ESTATUS_EXISTS | ESTATUS_ON_SCREEN;
        e->position.x = x;
        e->position.y = y;
        e->entityId = entityId;

        e->pose = 0;

        sEntitiesAiPointers[entityId](e);

        EntitySetPawnPosition(e);

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
    VDP_DisableSpritesFrom(0);
    for (u32 i = 0; i < ENTITY_AMOUNT; i++)
    {
        struct Entity* e = &gEntities[i];

        if (!(e->status & ESTATUS_EXISTS))
            continue;

        if (e->status & ESTATUS_NOT_DRAWN)
            continue;

        if (!(e->status & ESTATUS_ON_SCREEN))
            continue;

        EntityDraw(e);
    }
}

void EntitiesUpdate(void)
{
    for (u32 i = 0; i < ENTITY_AMOUNT; i++)
    {
        struct Entity* e = &gEntities[i];

        if (!(e->status & ESTATUS_EXISTS))
            continue;

        sEntitiesAiPointers[e->entityId](e);

        EntityCheckOnScreen(e);

        EntitySetPawnPosition(e);
        GamePawn_Update(&e->pawn);
    }
}
