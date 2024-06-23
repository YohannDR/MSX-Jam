#ifndef ENTITY_H
#define ENTITY_H

#include "game_pawn.h"
#include "math.h"

#define ENTITY_AMOUNT 10

enum EntityStatus
{
	ESTATUS_EXISTS = 1 << 0,
	ESTATUS_ON_SCREEN = 1 << 1,
	ESTATUS_NOT_DRAWN = 1 << 2,
	ESTATUS_FACING_LEFT = 1 << 3,
	ESTATUS_FACING_UP = 1 << 4,
	ESTATUS_ABSOLUTE_POSITION = 1 << 5,
	ESTATUS_INVINCIBLE = 1 << 6,
};

struct FrameData
{
	const u8* frame;
	u8 duration;
};

struct Entity
{
    VectorU16 position;
	u8 status;
	u8 pose;
    u8 entityId;
	u8 subTypeId;
	u8 adc; // Animation duration counter
	u8 caf; // Current animation frame
	u8 health;
	i8 hitboxTop;
	i8 hitboxBottom;
	i8 hitboxLeft;
	i8 hitboxRight;
	u8 work0;
	u16 timer;
	const struct FrameData* frameData;
};

struct EntityStats
{
	u8 spawnHealth;
	u8 damage;
};

enum EntityId
{
    ENTITY_PLAYER1,
    ENTITY_PLAYER2,
    ENTITY_BULLET,
    ENTITY_SHOOTER,
    ENTITY_BOOMERANG,

	ENTITY_COUNT
};

typedef void (*EntityUpdateFunc)(struct Entity*);

extern struct Entity gEntities[ENTITY_AMOUNT];
extern u16 gFrameCounter;

extern const EntityUpdateFunc sEntitiesAiPointers[ENTITY_COUNT];
extern const struct EntityStats sEntitiesStats[ENTITY_COUNT];

struct Entity* EntityInit(enum EntityId entityId, u8 subTypeId, u16 x, u16 y);
struct Entity* EntityFind(enum EntityId entityId);

void EntitySetFrameData(struct Entity* e, const struct FrameData* frameData);

void EntitiesSetup(void);
void EntitiesDraw(void);
void EntitiesUpdate(void);

#endif /* ENTITY_H */
