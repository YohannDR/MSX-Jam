#include "player1.h"
#include "debug.h"
#include "input.h"
#include "color.h"
#include "string.h"

#include "bullet.h"

#define PLAYER_SHOOT_DELAY 32

// Actions id
enum PlayerPose
{
	PLAYER_POSE_IDLE = 1,
	PLAYER_POSE_MOVE,
};

#pragma region Data

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

static const u8 sPlayerMoveRight_Frame0[] = {
    2,
    0, 0, 0*16 + 7*16,
    0, 0, 0*16 + 7*16 + 8,
};

static const u8 sPlayerMoveRight_Frame1[] = {
    2,
    0, 0, 1*16 + 7*16,
    0, 0, 1*16 + 7*16 + 8,
};

static const u8 sPlayerMoveRight_Frame2[] = {
    2,
    0, 0, 2*16 + 7*16,
    0, 0, 2*16 + 7*16 + 8,
};

static const u8 sPlayerMoveRight_Frame3[] = {
    2,
    0, 0, 3*16 + 7*16,
    0, 0, 3*16 + 7*16 + 8,
};

static const struct FrameData sPlayerMoveRight[] = {
    { sPlayerMoveRight_Frame0, 4 },
    { sPlayerMoveRight_Frame1, 4 },
    { sPlayerMoveRight_Frame2, 4 },
    { sPlayerMoveRight_Frame3, 4 },
    { NULL, 0 }
};

#pragma endregion

static void Player1SetPose(struct Entity* self, enum PlayerPose pose)
{
    if (self->pose == pose)
        return;

    self->pose = pose;
    EntitySetFrameData(self, pose == PLAYER_POSE_MOVE ? sPlayerMoveRight : sPlayerIdleRight);
}

static void Player1Init(struct Entity* self)
{
    EntitySetFrameData(self, sPlayerIdleRight);
    Player1SetPose(self, PLAYER_POSE_IDLE);

    self->timer = 0;

    self->hitboxTop = -4;
    self->hitboxBottom = 4;
    self->hitboxLeft = -4;
    self->hitboxRight = 4;
}

static void Player1HandleInputs(struct Entity* self)
{
    u8 moving = FALSE;

    if (self->timer != 0)
        self->timer--;

    if (Keyboard_IsKeyPressed(KEY_LEFT))
    {
        self->position.x -= 2;
        moving = TRUE;
    }
    else if (Keyboard_IsKeyPressed(KEY_RIGHT))
    {
        self->position.x += 2;
        moving = TRUE;
    }

    /*if (self->timer == 0 && Keyboard_IsKeyPressed(KEY_UP))
    {
        EntityInit(ENTITY_BULLET, BULLET_TOP, self->position.x, self->position.y);
        self->timer = PLAYER_SHOOT_DELAY;
    }*/

    if (Keyboard_IsKeyPressed(KEY_UP))
    {
        self->position.y -= 2;
        moving = TRUE;
    }
    else if (Keyboard_IsKeyPressed(KEY_DOWN))
    {
        self->position.y += 2;
        moving = TRUE;
    }

    if (moving)
    {
        Player1SetPose(self, PLAYER_POSE_MOVE);
    }
    else
    {
        Player1SetPose(self, PLAYER_POSE_IDLE);
    }
}

static void Player1Idle(struct Entity* self)
{
    Player1HandleInputs(self);
}

static void Player1Moving(struct Entity* self)
{
    Player1HandleInputs(self);
}

void Player1(struct Entity* self)
{
    switch (self->pose)
    {
        case 0:
            Player1Init(self);
            break;

        case PLAYER_POSE_IDLE:
            Player1Idle(self);
            break;

        case PLAYER_POSE_MOVE:
            Player1Moving(self);
            break;
    }
}
