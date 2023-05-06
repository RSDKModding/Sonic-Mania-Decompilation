// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ParticleHelpers Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectParticleHelpers *ParticleHelpers = NULL;

void ParticleHelpers_Update(void) {}

void ParticleHelpers_LateUpdate(void) {}

void ParticleHelpers_StaticUpdate(void) {}

void ParticleHelpers_Draw(void) {}

void ParticleHelpers_Create(void *data) {}

void ParticleHelpers_StageLoad(void) {}

void ParticleHelpers_SetupFallingParticles(int32 x, int32 y, void (*callback)(EntityDebris *))
{
    int32 pos = x - 0x80000;
    for (int32 i = 0; i < 5; ++i) {
        int32 spawnX            = pos + (i << 18);
        EntityDebris *debris    = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX, y);
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->gravityStrength = 0x3800;
        debris->velocity.y      = 0;
        debris->velocity.x      = 0x30 * ((spawnX - x) >> 8);

        if (debris->position.x < x)
            debris->direction = FLIP_X;

        if (callback)
            callback(debris);
    }
}

void ParticleHelpers_SetupParticleFX(void *debrisState, void (*callback)(EntityDebris *), int32 speed, int32 xOffset, int32 yOffset, int32 maxX,
                                     int32 maxY)
{
    int32 x              = ZONE_RAND(-(maxX >> 1), maxX >> 1) + xOffset;
    int32 y              = ZONE_RAND(-(maxY >> 1), maxY >> 1) + yOffset;
    EntityDebris *debris = CREATE_ENTITY(Debris, debrisState, x, y);

    debris->drawGroup       = Zone->objectDrawGroup[1];
    debris->gravityStrength = 0x3800;
    debris->velocity.x      = ((x - xOffset) >> 8) * (speed >> 8);
    debris->velocity.y      = ((y - yOffset) >> 8) * (speed >> 8);

    if (debris->position.x < xOffset)
        debris->direction = FLIP_X;

    if (callback)
        callback(debris);
}

#if GAME_INCLUDE_EDITOR
void ParticleHelpers_EditorDraw(void) {}

void ParticleHelpers_EditorLoad(void) {}
#endif

void ParticleHelpers_Serialize(void) {}
