#include "shooter.h"
#include "bullet.h"

#define SHOOTER_SPEED 2
#define SHOOTER_MAX_BOUNCE 4

#define SHOOTER_SHOOT_INTERVAL 64

enum ShooterPose
{
    SHOOTER_POSE_COMING_IN = 1,
    SHOOTER_POSE_MOVING,
    SHOOTER_POSE_GOING_OUT,
};

static const u8 sShooterIdleLeft_Frame0[] = {
    2,
    0, 0, 4*16,
    0, 0, 4*16+8,
};

static const u8 sShooterIdleLeft_Frame1[] = {
    2,
    0, 0, 2*16,
    0, 0, 2*16+8,
};

static const struct FrameData sShooterIdleLeft[] = {
    { sShooterIdleLeft_Frame0, 4 },
    { sShooterIdleLeft_Frame1, 4 },
    { NULL, 0 }
};

static void ShooterInit(struct Entity* self)
{
    self->pose = SHOOTER_POSE_COMING_IN;
    self->status |= ESTATUS_ABSOLUTE_POSITION;
    self->timer = 0;
    self->work0 = 0; // Bounce counter

    EntitySetFrameData(self, sShooterIdleLeft);

    if (self->subTypeId == SHOOTER_HORIZONTAL && self->position.x > 16)
    {
        self->status |= ESTATUS_FACING_LEFT;
    }

    self->hitboxTop = -16;
    self->hitboxBottom = 16;
    self->hitboxLeft = -16;
    self->hitboxRight = 16;
}

static void ShooterComingIn(struct Entity* self)
{
    self->pose = SHOOTER_POSE_MOVING;
}

static void ShooterMoving(struct Entity* self)
{
    u8 bouncing = FALSE;

    self->timer++;

    if (self->timer == SHOOTER_SHOOT_INTERVAL)
    {
        u8 bulletType;

        if (self->subTypeId == SHOOTER_VERTICAL)
        {
            bulletType = BULLET_BOTTOM;
        }
        else if (self->subTypeId == SHOOTER_HORIZONTAL)
        {
            if (self->status & ESTATUS_FACING_LEFT)
                bulletType = BULLET_LEFT;
            else
                bulletType = BULLET_RIGHT;
        }

        EntityInit(ENTITY_BULLET, bulletType, self->position.x, self->position.y);
        self->timer = 0;
    }

    if (self->subTypeId == SHOOTER_VERTICAL)
    {
        if (self->status & ESTATUS_FACING_LEFT)
        {
            self->position.x += SHOOTER_SPEED;

            if (self->position.x > 0x100)
                bouncing = TRUE;
        }
        else
        {
            self->position.x -= SHOOTER_SPEED;

            if (self->position.x < 20)
                bouncing = TRUE;
        }

        if (bouncing)
        {
            self->status ^= ESTATUS_FACING_LEFT;
            self->work0++;
        }
    }
    else if (self->subTypeId == SHOOTER_HORIZONTAL)
    {
        if (self->status & ESTATUS_FACING_UP)
        {
            self->position.y -= SHOOTER_SPEED;

            if (self->position.y < 20)
                bouncing = TRUE;
        }
        else
        {
            self->position.y += SHOOTER_SPEED;

            if (self->position.y > 0x100)
                bouncing = TRUE;
        }

        if (bouncing)
        {
            self->status ^= ESTATUS_FACING_UP;
            self->work0++;
        }
    }

    if (bouncing && self->work0 == SHOOTER_MAX_BOUNCE)
    {
        self->pose = SHOOTER_POSE_GOING_OUT;
    }
}

static void ShooterGoingOut(struct Entity* self)
{
    self->status = 0;
}

void Shooter(struct Entity* self)
{
    switch (self->pose)
    {
        case 0:
            ShooterInit(self);

        case SHOOTER_POSE_COMING_IN:
            ShooterComingIn(self);
            break;

        case SHOOTER_POSE_MOVING:
            ShooterMoving(self);
            break;

        case SHOOTER_POSE_GOING_OUT:
            ShooterGoingOut(self);
            break;
    }
}
