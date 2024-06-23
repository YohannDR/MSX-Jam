#ifndef SCROLL_H
#define SCROLL_H

#include "core.h"

struct EnemySpawnInfo
{
    u16 spawnY;
    u8 type;
    u8 subType;
    u16 x;
    u16 y;
};

void ScrollInit(const c8* tilemap);
void ScrollUpdate(void);

void ScrollX(i16 offset);
void ScrollY(i16 offset);

u16 ScrollGetX(void);
u16 ScrollGetY(void);
u8 ScrollGetSpriteIndex(void);

#endif /* SCROLL_H */
