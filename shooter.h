#ifndef SHOOTER_H
#define SHOOTER_H

#include "entity.h"

enum ShooterSubType
{
    SHOOTER_VERTICAL,
    SHOOTER_HORIZONTAL,
};

void Shooter(struct Entity* self);

#endif /* SHOOTER_H */
