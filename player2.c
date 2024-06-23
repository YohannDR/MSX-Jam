#include "player2.h"
#include "input.h"

#define PLAYER2_SPEED 2

enum Player2Pose
{
    PLAYER2_POSE_MOVING = 1,
};

static const u8 sPlayerIdleRight_Frame0[] = {
    2,
    0, 0, 4*16 + 7*16,
    0, 0, 4*16 + 7*16 + 8,
};

static const u8 sPlayerIdleRight_Frame1[] = {
    2,
    0, 0, 2*16 + 7*16,
    0, 0, 2*16 + 7*16 + 8,
};

static const struct FrameData sPlayerIdleRight[] = {
    { sPlayerIdleRight_Frame0, 64 },
    { sPlayerIdleRight_Frame1, 24 },
    { NULL, 0 }
};

static void Player2SetHitbox(struct Entity* self)
{
    if (self->status & ESTATUS_FACING_UP)
    {
        self->hitboxTop = -4;
        self->hitboxBottom = 4;
        self->hitboxLeft = -10;
        self->hitboxRight = 10;
    }
    else
    {
        self->hitboxTop = -10;
        self->hitboxBottom = 10;
        self->hitboxLeft = -4;
        self->hitboxRight = 4;
    }
}

static void Player2Init(struct Entity* self)
{
    self->status |= ESTATUS_FACING_UP | ESTATUS_ABSOLUTE_POSITION | ESTATUS_INVINCIBLE;

    Player2SetHitbox(self);

    self->pose = PLAYER2_POSE_MOVING;

    EntitySetFrameData(self, sPlayerIdleRight);
}

static void Player2Move(struct Entity* self)
{
    if (Keyboard_IsKeyPressed(KEY_Q))
        self->position.x -= PLAYER2_SPEED;
    else if (Keyboard_IsKeyPressed(KEY_D))
        self->position.x += PLAYER2_SPEED;

    if (Keyboard_IsKeyPressed(KEY_Z))
        self->position.y -= PLAYER2_SPEED;
    else if (Keyboard_IsKeyPressed(KEY_S))
        self->position.y += PLAYER2_SPEED;

    if (Keyboard_IsKeyPushed(KEY_A))
    {
        self->status ^= ESTATUS_FACING_UP;
        Player2SetHitbox(self);
    }
}

void Player2(struct Entity* self)
{
    switch (self->pose)
    {
        case 0:
            Player2Init(self);
            break;

        case PLAYER2_POSE_MOVING:
            Player2Move(self);
    }
}
