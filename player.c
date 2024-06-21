#include "player.h"
#include "debug.h"
#include "input.h"
#include "color.h"
#include "string.h"
#include "scroll.h"

// Actions id
enum PlayerPose
{
	PLAYER_POSE_IDLE = 1,
	PLAYER_POSE_MOVE,
	PLAYER_POSE_JUMP,
	PLAYER_POSE_FALL,
};

#pragma region Data

// Player's sprite layers
// This character is made by 3 layers (3 sprites) but the first two are special: One is only visible on even frames while the second only on odd frames.
// So, on the 3 layers, only two are visible at a given frame. The blinking of the first two black layers is done to create shaded colors.
// The counterpart is the flickering effect. The character white color comes from the background and is not in a sprite.
static const Game_Sprite sPlayerLayers[] =
{
//	  Sprite ID
//    |  X offset from pawn's position
//    |  |  Y offset
//    |  |  |  Pattern offset from current animation key
//    |  |  |  |   Layer's color
//    |  |  |  |   |                Layer option
	{ 0, 0, 0, 0,  COLOR_BLACK,     PAWN_SPRITE_EVEN }, // Only visible on even frame number
	{ 0, 0, 0, 12, COLOR_BLACK,     PAWN_SPRITE_ODD }, // Only visible on odd frame number
	{ 4, 0, 0, 8,  COLOR_LIGHT_RED, 0 },
};

// Idle animation frames
// Each line describes an animation key
static const Game_Frame sIdleLeft[] =
{
//	  Pattern offset of this animation key in the sprite data
//    |     Animation key duration (in frame number)
//    |     |   Event to trigger during this animation key (function pointer)
	{ 4*16,	64,	NULL },
	{ 2*16,	24,	NULL },
};

// Move animation frames
static const Game_Frame sMoveLeft[] =
{
	{ 0*16,	4,	NULL },
	{ 1*16,	4,	NULL },
	{ 2*16,	4,	NULL },
	{ 3*16,	4,	NULL },
};

// Jump animation frames
static const Game_Frame sJumpLeft[] =
{
	{ 5*16,	4,	NULL },
};

// Fall animation frames
static const Game_Frame sFallLeft[] =
{
	{ 6*16,	4,	NULL },
};

// List of all player actions
static const Game_Action sAnimActionsLeft[] =
{ //  Frames        Number                  Loop?  Interrupt?
	[PLAYER_POSE_IDLE] = { sIdleLeft, numberof(sIdleLeft), TRUE,  TRUE },
	[PLAYER_POSE_MOVE] = { sMoveLeft, numberof(sMoveLeft), TRUE,  TRUE },
	[PLAYER_POSE_JUMP] = { sJumpLeft, numberof(sJumpLeft), TRUE,  TRUE },
	[PLAYER_POSE_FALL] = { sFallLeft, numberof(sFallLeft), TRUE,  TRUE },
};

//.............................................................................
// Player 2 data

// Idle animation frames
static const Game_Frame sIdleRight[] =
{
	{ 4*16+7*16,	64,	NULL },
	{ 2*16+7*16,	24,	NULL },
};

// Move animation frames
static const Game_Frame sMoveRight[] =
{
	{ 0*16+7*16,	4,	NULL },
	{ 1*16+7*16,	4,	NULL },
	{ 2*16+7*16,	4,	NULL },
	{ 3*16+7*16,	4,	NULL },
};

// Jump animation frames
static const Game_Frame sJumpRight[] =
{
	{ 5*16+7*16,	4,	NULL },
};

// Fall animation frames
static const Game_Frame sFallRight[] =
{
	{ 6*16+7*16,	4,	NULL },
};

// List of all player actions
static const Game_Action sAnimActionsRight[] =
{ //  Frames        Number                  Loop?  Interrupt?
	[PLAYER_POSE_IDLE] = { sIdleRight, numberof(sIdleRight), TRUE,  TRUE },
	[PLAYER_POSE_MOVE] = { sMoveRight, numberof(sMoveRight), TRUE,  TRUE },
	[PLAYER_POSE_JUMP] = { sJumpRight, numberof(sJumpRight), TRUE,  TRUE },
	[PLAYER_POSE_FALL] = { sFallRight, numberof(sFallRight), TRUE,  TRUE },
};

#pragma endregion

static void PlayerSetPose(struct Entity* self, enum PlayerPose pose)
{
    self->pose = pose;
    GamePawn_SetAction(&self->pawn, pose);
}

static void PlayerSetDirection(struct Entity* self)
{
    Game_Pawn* pawn = &self->pawn;

    if (self->status & ESTATUS_FACING_LEFT)
        pawn->ActionList = sAnimActionsLeft;
    else
        pawn->ActionList = sAnimActionsRight;
}

static void PlayerHandleInputs(struct Entity* self)
{
    u8 moving = FALSE;

    if (Keyboard_IsKeyPressed(KEY_LEFT))
    {
        self->position.x--;
        self->status |= ESTATUS_FACING_LEFT;
        moving = TRUE;
        Scroll_SetOffsetH(-1);
    }
    else if (Keyboard_IsKeyPressed(KEY_RIGHT))
    {
        self->position.x++;
        self->status &= ~ESTATUS_FACING_LEFT;
        moving = TRUE;
        Scroll_SetOffsetH(1);
    }

    if (Keyboard_IsKeyPressed(KEY_UP))
    {
        self->position.y--;
        moving = TRUE;
    }
    else if (Keyboard_IsKeyPressed(KEY_DOWN))
    {
        self->position.y++;
        moving = TRUE;
    }

    if (moving)
        PlayerSetPose(self, PLAYER_POSE_MOVE);
    else
        PlayerSetPose(self, PLAYER_POSE_IDLE);
}

static void PlayerInit(struct Entity* self)
{
    Game_Pawn* pawn = &self->pawn;

	GamePawn_Initialize(pawn, sPlayerLayers, numberof(sPlayerLayers), 0, sAnimActionsRight);
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
    if (self->pose != 0)
        PlayerSetDirection(self);

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

        case PLAYER_POSE_JUMP:
        case PLAYER_POSE_FALL:
    }
}
