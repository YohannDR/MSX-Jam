#include "player2.h"

static void Player2Init(struct Entity* self)
{
    self->status |= ESTATUS_FACING_UP | ESTATUS_ABSOLUTE_POSITION | ESTATUS_INVINCIBLE;

    self->hitboxTop = -4;
    self->hitboxBottom = 4;
    self->hitboxLeft = -4;
    self->hitboxRight = 4;
}

void Player2(struct Entity* self)
{
    switch (self->pose)
    {
        case 0:
            Player2Init(self);
            break;
    }
}
