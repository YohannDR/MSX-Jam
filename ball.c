#include "ball.h"
#include "debug.h"
#include "input.h"
#include "color.h"
#include "string.h"

// Actions id
enum BallPose
{
	ACTION_BALL_IDLE = 1,
	ACTION_BALL_BUMP,
};

#pragma region Data

static const Game_Sprite sBallLayers[] =
{
	{ 2, 0, 0, 4,  COLOR_DARK_RED, 0 },
	{ 3, 0, 0, 0,  COLOR_MEDIUM_RED, 0 },
};

// Idle animation frames
static const Game_Frame sBallIdle[] =
{
	{ 0*8+224,	4,	NULL },
};

// Bump animation frames
static const Game_Frame sBallBump[] =
{
	{ 1*8+224,	1,	NULL },
	{ 2*8+224,	5,	NULL },
	{ 1*8+224,	2,	NULL },
};

// List of all ball actions
static const Game_Action sBallActions[] =
{ //  Frames      Number                Loop?  Interrupt?
	[ACTION_BALL_IDLE] = { sBallIdle, numberof(sBallIdle), TRUE,  TRUE },
	[ACTION_BALL_BUMP] = { sBallBump, numberof(sBallBump), FALSE, TRUE },
};

#pragma endregion

static void BallSetPose(struct Entity* self, enum BallPose pose)
{
    self->pose = pose;
    GamePawn_SetAction(&self->pawn, pose);
}

static void BallInit(struct Entity* self)
{
    Game_Pawn* pawn = &self->pawn;

	GamePawn_Initialize(pawn, sBallLayers, numberof(sBallLayers), 0, sBallActions);
    BallSetPose(self, ACTION_BALL_IDLE);
}

void Ball(struct Entity* self)
{
    switch (self->pose)
    {
        case 0:
            BallInit(self);
            break;

        case ACTION_BALL_IDLE:
            break;

        case ACTION_BALL_BUMP:
            break;
    }
}
