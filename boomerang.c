#include "boomerang.h"

static const u8 sBoomerangIdleLeft_Frame0[] = {
    2,
    0, 0, 4*16,
    0, 0, 4*16+8,
};

static const u8 sBoomerangIdleLeft_Frame1[] = {
    2,
    0, 0, 2*16,
    0, 0, 2*16+8,
};

static const struct FrameData sBoomerangIdleLeft[] = {
    { sBoomerangIdleLeft_Frame0, 4 },
    { sBoomerangIdleLeft_Frame1, 4 },
    { NULL, 0 }
};

static void BoomerangInit(struct Entity* self)
{
    self->status |= ESTATUS_ABSOLUTE_POSITION;
    self->work0 = 0; // Movement pattern

    if (self->position.x > 32)
        self->status |= ESTATUS_FACING_LEFT;

    EntitySetFrameData(self, sBoomerangIdleLeft);

    self->pose = 1;
}

static void BoomerangMove(struct Entity* self)
{
    switch (self->work0)
    {
        case 0:
            self->position.y += 2;

            if (self->position.y > 0x98)
                self->work0 = 1;
            break;

        case 1:
            self->position.y -= 2;

            u8 xSpeed = 1;

            if (gFrameCounter % 2)
                xSpeed = 2;

            if (self->status & ESTATUS_FACING_LEFT)
                self->position.x -= xSpeed;
            else
                self->position.x += xSpeed;

            if (self->position.y < 10)
                self->work0 = 2;
            break;

        case 2:
            self->position.y += 2;

            if (!(self->status & ESTATUS_ON_SCREEN))
                self->status = 0;
    }
}

void Boomerang(struct Entity* self)
{
    switch (self->pose)
    {
        case 0:
            BoomerangInit(self);
            break;

        default:
            BoomerangMove(self);
    }
}
