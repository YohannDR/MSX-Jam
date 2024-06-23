#include "scrolling.h"
#include "scroll.h"
#include "string.h"
#include "debug.h"
#include "input.h"
#include "math.h"

#include "entity.h"
#include "shooter.h"

static VectorU16 gScreenPosition;
static u8 gScrollSpriteIndex;
static u8 gScrollSpawnIndex;

const struct EnemySpawnInfo sEnemySpawnInfo[] = {
    { 20, ENTITY_BOOMERANG, 0, 175, 10 },
    { 33, ENTITY_BOOMERANG, 0, 64, 10 },
    { 45, ENTITY_BOOMERANG, 0, 110, 10 },
    { 48, ENTITY_BOOMERANG, 0, 120, 10 },
    { 55, ENTITY_SHOOTER, SHOOTER_VERTICAL, 20, 10 },
    { 68, ENTITY_SHOOTER, SHOOTER_VERTICAL, 20, 10 },
    { 77, ENTITY_SHOOTER, SHOOTER_HORIZONTAL, 20, 10 },
    { 80, ENTITY_SHOOTER, SHOOTER_HORIZONTAL, 236, 10 },
    { 88, ENTITY_BOOMERANG, 0, 50, 10 },
    { 95, ENTITY_BOOMERANG, 0, 206, 10 },
    { 102, ENTITY_BOOMERANG, 0, 128, 10 },
    { 113, ENTITY_SHOOTER, SHOOTER_HORIZONTAL, 64, 10 },
    { 117, ENTITY_SHOOTER, SHOOTER_HORIZONTAL, 128, 10 },
    { 124, ENTITY_SHOOTER, SHOOTER_VERTICAL, 20, 10 },
    { 127, ENTITY_SHOOTER, SHOOTER_HORIZONTAL, 192, 10 },
    { 130, ENTITY_SHOOTER, SHOOTER_VERTICAL, 20, 10 },
    { 142, ENTITY_SHOOTER, SHOOTER_HORIZONTAL, 236, 10 },
    { 145, ENTITY_SHOOTER, SHOOTER_HORIZONTAL, 236, 10 },
    { 149, ENTITY_SHOOTER, SHOOTER_HORIZONTAL, 20, 10 },
    { 152, ENTITY_SHOOTER, SHOOTER_HORIZONTAL, 236, 10 },
    { 164, ENTITY_SHOOTER, SHOOTER_VERTICAL, 20, 10 },
    { 166, ENTITY_SHOOTER, SHOOTER_VERTICAL, 20, 10 },
    { 168, ENTITY_SHOOTER, SHOOTER_VERTICAL, 20, 10 },
    { 174, ENTITY_BOOMERANG, 0, 75, 10 },
    { 179, ENTITY_BOOMERANG, 0, 128, 10 },
    { 184, ENTITY_BOOMERANG, 0, 180, 10 },
};

void ScrollInit(const c8* tilemap)
{
    gScrollSpriteIndex = Scroll_Initialize((u16)tilemap);
    gScreenPosition.x = 0;
    gScreenPosition.y = 0;
    gScrollSpawnIndex = 0;

    c8 buffer[10];
    String_Format(buffer, "%i", gScrollSpriteIndex);
    DEBUG_LOG(buffer);
}

void ScrollUpdate(void)
{
    Scroll_Update();

    if (Keyboard_IsKeyPressed(KEY_F))
    {
        ScrollX(-1);
    }
    else if (Keyboard_IsKeyPressed(KEY_H))
    {
        ScrollX(1);
    }

    if (Keyboard_IsKeyPressed(KEY_T))
    {
        ScrollY(-1);
    }
    else if (Keyboard_IsKeyPressed(KEY_G))
    {
        ScrollY(1);
    }

    if (gScrollSpawnIndex < numberof(sEnemySpawnInfo))
    {
        const struct EnemySpawnInfo* info = &sEnemySpawnInfo[gScrollSpawnIndex];
        if (info->spawnY <= gScreenPosition.y / 8)
        {
            EntityInit(info->type, info->subType, info->x, info->y);
            gScrollSpawnIndex++;
        }
    }
}

void ScrollX(i16 offset)
{
    gScreenPosition.x += offset;
    Scroll_SetOffsetH(offset);
}

void ScrollY(i16 offset)
{
    gScreenPosition.y += offset;
    Scroll_SetOffsetV(offset);
}

u16 ScrollGetX(void)
{
    return gScreenPosition.x;
}

u16 ScrollGetY(void)
{
    return gScreenPosition.y;
}

u8 ScrollGetSpriteIndex(void)
{
    return gScrollSpriteIndex;
}
