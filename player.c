#include "player.h"
#include "debug.h"
#include "input.h"
#include "color.h"
#include "string.h"

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

static void PlayerSetPose(struct Entity* self, enum PlayerPose pose)
{
    if (self->pose == pose)
        return;

    self->pose = pose;
    EntitySetFrameData(self, pose == PLAYER_POSE_MOVE ? sPlayerMoveRight : sPlayerIdleRight);
}

static void PlayerHandleInputs(struct Entity* self)
{
    u8 moving = FALSE;

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
        PlayerSetPose(self, PLAYER_POSE_MOVE);
    }
    else
    {
        PlayerSetPose(self, PLAYER_POSE_IDLE);
    }
}

static void PlayerInit(struct Entity* self)
{
    EntitySetFrameData(self, sPlayerIdleRight);
    PlayerSetPose(self, PLAYER_POSE_IDLE);
}

static void PlayerIdle(struct Entity* self)
{
    PlayerHandleInputs(self);
}

static void PlayerMoving(struct Entity* self)
{
    PlayerHandleInputs(self);
}

void Player(struct Entity* self)
{
    switch (self->pose)
    {
        case 0:
            PlayerInit(self);
            break;

        case PLAYER_POSE_IDLE:
            PlayerIdle(self);
            break;

        case PLAYER_POSE_MOVE:
            PlayerMoving(self);
            break;
    }
}
