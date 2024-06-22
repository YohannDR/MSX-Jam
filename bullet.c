#include "bullet.h"
#include "color.h"

#define BULLET_SPEED 4

enum BulletPose
{
    BULLET_POSE_MOVING = 1,
    BULLET_POSE_EXPLODING,
};

static const u8 sBulletIdleLeft_Frame0[] = {
    2,
    0, 0, 4*16,
    0, 0, 4*16+8,
};

static const u8 sBulletIdleLeft_Frame1[] = {
    2,
    0, 0, 2*16,
    0, 0, 2*16+8,
};

static const struct FrameData sBulletIdleLeft[] = {
    { sBulletIdleLeft_Frame0, 4 },
    { sBulletIdleLeft_Frame1, 4 },
    { NULL, 0 }
};

static const struct FrameData* const sBulletFrameData[] =
{
    [BULLET_RIGHT] = sBulletIdleLeft,
    [BULLET_TOP_RIGHT] = sBulletIdleLeft,
    [BULLET_TOP] = sBulletIdleLeft,
    [BULLET_TOP_LEFT] = sBulletIdleLeft,
    [BULLET_LEFT] = sBulletIdleLeft,
    [BULLET_BOTTOM_LEFT] = sBulletIdleLeft,
    [BULLET_BOTTOM] = sBulletIdleLeft,
    [BULLET_BOTTOM_RIGHT] = sBulletIdleLeft,
};

static void BulletInit(struct Entity* self)
{
    EntitySetFrameData(self, sBulletFrameData[self->subTypeId]);
    self->pose = BULLET_POSE_MOVING;
    self->status |= ESTATUS_ABSOLUTE_POSITION;
}

static void BulletMoving(struct Entity* self)
{
    switch (self->subTypeId)
    {
        case BULLET_RIGHT:
            self->position.x += BULLET_SPEED;
            break;

        case BULLET_TOP_RIGHT:
            self->position.x += BULLET_SPEED / 2;
            self->position.y -= BULLET_SPEED / 2;
            break;

        case BULLET_TOP:
            self->position.y -= BULLET_SPEED;
            break;

        case BULLET_TOP_LEFT:
            self->position.x -= BULLET_SPEED / 2;
            self->position.y -= BULLET_SPEED / 2;
            break;

        case BULLET_LEFT:
            self->position.x -= BULLET_SPEED;
            break;

        case BULLET_BOTTOM_LEFT:
            self->position.x -= BULLET_SPEED / 2;
            self->position.y += BULLET_SPEED / 2;
            break;

        case BULLET_BOTTOM:
            self->position.y += BULLET_SPEED;
            break;

        case BULLET_BOTTOM_RIGHT:
            self->position.x += BULLET_SPEED / 2;
            self->position.y += BULLET_SPEED / 2;
    }
}

static void BulletExploding(struct Entity* self)
{
}

void Bullet(struct Entity* self)
{
    if (!(self->status & ESTATUS_ON_SCREEN))
    {
        self->status = 0;
        return;
    }

    switch (self->pose)
    {
        case 0:
            BulletInit(self);

        case BULLET_POSE_MOVING:
            BulletMoving(self);
            break;

        case BULLET_POSE_EXPLODING:
            BulletExploding(self);
            break;
    }
}
