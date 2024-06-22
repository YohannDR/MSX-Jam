#ifndef BULLET_H
#define BULLET_H

#include "entity.h"

enum BulletSubType
{
    BULLET_RIGHT,
    BULLET_TOP_RIGHT,
    BULLET_TOP,
    BULLET_TOP_LEFT,
    BULLET_LEFT,
    BULLET_BOTTOM_LEFT,
    BULLET_BOTTOM,
    BULLET_BOTTOM_RIGHT
};

void Bullet(struct Entity* self);

#endif /* BULLET_H */
