#include "entity.h"
#include "string.h"
#include "debug.h"
#include "memory.h"
#include "vdp.h"

#include "player1.h"
#include "player2.h"
#include "bullet.h"
#include "shooter.h"
#include "boomerang.h"
#include "scrolling.h"

struct Entity gEntities[ENTITY_AMOUNT];

static u8 gNextOamSlot;
u16 gFrameCounter;

const EntityUpdateFunc sEntitiesAiPointers[ENTITY_COUNT] = {
    [ENTITY_PLAYER1] = Player1,
    [ENTITY_PLAYER2] = Player2,
    [ENTITY_BULLET] = Bullet,
    [ENTITY_SHOOTER] = Shooter,
    [ENTITY_BOOMERANG] = Boomerang,
};

const struct EntityStats sEntitiesStats[ENTITY_COUNT] = {
    [ENTITY_PLAYER1] = {
        .spawnHealth = 1,
        .damage = 0
    },
    [ENTITY_PLAYER2] = {
        .spawnHealth = 0xFF,
        .damage = 1
    },
    [ENTITY_BULLET] = {
        .spawnHealth = 1,
        .damage = 1
    },
    [ENTITY_SHOOTER] = {
        .spawnHealth = 1,
        .damage = 1
    },
    [ENTITY_BOOMERANG] = {
        .spawnHealth = 1,
        .damage = 1
    }
};

u32 CheckObjectsTouching(u16 o1Top, u16 o1Bottom, u16 o1Left, u16 o1Right, u16 o2Top, u16 o2Bottom, u16 o2Left, u16 o2Right)
{
    if (o2Bottom >= o1Top && o2Top < o1Bottom && o2Right >= o1Left && o2Left < o1Right)
        return TRUE;
    else
        return FALSE;
}

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
    u8 x = e->position.x;
    u8 y = e->position.y;

    if (!(e->status & ESTATUS_ABSOLUTE_POSITION))
    {
        x -= ScrollGetX();
        y -= ScrollGetY() - 1;
    }

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
    u16 screenX = 0;
    u16 screenY = 0;

    if (!(e->status & ESTATUS_ABSOLUTE_POSITION))
    {
        screenX = ScrollGetX();
        screenY = ScrollGetY();
    }

    u16 entityX = e->position.x;
    u16 entityY = e->position.y;

    if (entityX < screenX || entityY < screenY)
    {
        e->status &= ~ESTATUS_ON_SCREEN;
        return;
    }

    if (entityX < screenX + 0xFF && entityY < screenY + 0xF0)
    {
        e->status |= ESTATUS_ON_SCREEN;
        return;
    }

    e->status &= ~ESTATUS_ON_SCREEN;
}

static void EntitiesCheckCollisions(void)
{
    for (u32 i = 0; i < ENTITY_AMOUNT - 1; i++)
    {
        struct Entity* e1 = &gEntities[i];

        if (!(e1->status & ESTATUS_EXISTS))
            continue;

        for (u32 j = i + 1; j < ENTITY_AMOUNT; j++)
        {
            struct Entity* e2 = &gEntities[j];

            if (!(e2->status & ESTATUS_EXISTS))
                continue;

            if ((e1->entityId == ENTITY_PLAYER1 && e2->entityId == ENTITY_PLAYER2) ||
                (e1->entityId == ENTITY_PLAYER2 && e2->entityId == ENTITY_PLAYER1))
            {
                continue;
            }

            u16 e1Top = e1->position.y + e1->hitboxTop;
            u16 e1Bottom = e1->position.y + e1->hitboxBottom;
            u16 e1Left = e1->position.x + e1->hitboxLeft;
            u16 e1Right = e1->position.x + e1->hitboxRight;

            u16 e2Top = e2->position.y + e2->hitboxTop;
            u16 e2Bottom = e2->position.y + e2->hitboxBottom;
            u16 e2Left = e2->position.x + e2->hitboxLeft;
            u16 e2Right = e2->position.x + e2->hitboxRight;

            if (!CheckObjectsTouching(e1Top, e1Bottom, e1Left, e1Right, e2Top, e2Bottom, e2Left, e2Right))
                continue;

            u8 e1Damage = sEntitiesStats[e1->entityId].damage;
            u8 e2Damage = sEntitiesStats[e2->entityId].damage;

            if (!(e1->status & ESTATUS_INVINCIBLE))
            {
                if (e1->health > e2Damage)
                    e1->health -= e2Damage;
                else
                    e1->health = 0;
            }

            if (!(e2->status & ESTATUS_INVINCIBLE))
            {
                if (e2->health > e1Damage)
                    e2->health -= e1Damage;
                else
                    e2->health = 0;
            }

            if (e1->health == 0)
            {
                e1->status = 0;
            }

            if (e2->health == 0)
            {
                e2->status = 0;
            }
        }
    }
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
        e->health = sEntitiesStats[e->entityId].spawnHealth;

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
    gFrameCounter++;

    if ((gFrameCounter % 4) == 0)
        EntitiesCheckCollisions();

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
