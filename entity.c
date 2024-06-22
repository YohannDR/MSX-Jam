#include "entity.h"
#include "string.h"
#include "debug.h"
#include "memory.h"
#include "vdp.h"

#include "player.h"
#include "bullet.h"
#include "scrolling.h"

struct Entity gEntities[ENTITY_AMOUNT];

static u8 gNextOamSlot;

const EntityUpdateFunc sEntitiesAiPointers[ENTITY_COUNT] = {
    [ENTITY_PLAYER] = Player,
    [ENTITY_BULLET] = Bullet
};

static void EntityUpdateAnimation(struct Entity* e)
{
    e->adc++;

    if (e->frameData[e->caf].duration < e->adc)
    {
        e->caf++;
        e->adc = 0;

        if (e->frameData[e->caf].duration == 0)
            e->caf = 0;
    }
}

static void EntityDraw(struct Entity* e)
{
    u8 x = e->position.x - ScrollGetX();
    u8 y = e->position.y - ScrollGetY() - 1;

    const u8* frame = e->frameData[e->caf].frame;
    u8 partCount = *frame++;

    for (u8 i = 0; i < partCount; i++)
    {
        u8 yOffset = *frame++;
        u8 xOffset = *frame++;
        u8 pattern = *frame++;

        VDP_SetSprite(gNextOamSlot + i, x + xOffset, y + yOffset, pattern);
    }

    gNextOamSlot += partCount;
}

static void EntityCheckOnScreen(struct Entity* e)
{
    c8 buffer[50];
    u16 screenX = ScrollGetX();
    u16 screenY = ScrollGetY();
    u16 entityX = e->position.x;
    u16 entityY = e->position.y;

    if (entityX < screenX || entityY < screenY)
    {
        e->status &= ~ESTATUS_ON_SCREEN;
        return;
    }

    if (entityX < screenX + 0xFF && entityY < screenY + 0xFF)
    {
        e->status |= ESTATUS_ON_SCREEN;
        return;
    }

    e->status &= ~ESTATUS_ON_SCREEN;
}

struct Entity* EntityInit(enum EntityId entityId, u8 subTypeId, u16 x, u16 y)
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
        e->subTypeId = subTypeId;

        e->pose = 0;

        sEntitiesAiPointers[entityId](e);

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

void EntitySetFrameData(struct Entity* e, const struct FrameData* frameData)
{
    e->frameData = frameData;
    e->adc = 0;
    e->caf = 0;
}

void EntitiesSetup(void)
{
    Mem_Set(0, gEntities, sizeof(gEntities));
}

void EntitiesDraw(void)
{
    // VDP_DisableSpritesFrom(0);
    gNextOamSlot = 0;

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
        EntityUpdateAnimation(e);
    }
}
