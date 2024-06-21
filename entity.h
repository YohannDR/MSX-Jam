#ifndef ENTITY_H
#define ENTITY_H

#include "game_pawn.h"
#include "math.h"

#define ENTITY_AMOUNT 10

enum EntityStatus
{
	ESTATUS_EXISTS = 1 << 0,
	ESTATUS_NOT_DRAWN = 1 << 1,
	ESTATUS_FACING_LEFT = 1 << 2,
};

struct Entity
{
    VectorU16 position;
	u8 status;
	u8 pose;
    u8 entityId;
	Game_Pawn pawn;
};

enum EntityId
{
    ENTITY_PLAYER,
    ENTITY_BALL,

	ENTITY_COUNT
};

typedef void (*EntityUpdateFunc)(struct Entity*);

extern struct Entity gEntities[ENTITY_AMOUNT];

extern const EntityUpdateFunc sEntitiesAiPointers[ENTITY_COUNT];

struct Entity* EntityInit(enum EntityId entityId, u16 x, u16 y);
struct Entity* EntityFind(enum EntityId entityId);

void EntitiesSetup(void);
void EntitiesDraw(void);
void EntitiesUpdate(void);

#endif /* ENTITY_H */
