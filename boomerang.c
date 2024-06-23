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
}

void Boomerang(struct Entity* self)
{
    switch (self->pose)
    {
        case 0:
            BoomerangInit(self);
            break;

        
    }
}
