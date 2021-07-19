#include "../SonicMania.h"

ObjectParticleHelpers *ParticleHelpers = NULL;

void ParticleHelpers_Update(void)
{

}

void ParticleHelpers_LateUpdate(void)
{

}

void ParticleHelpers_StaticUpdate(void)
{

}

void ParticleHelpers_Draw(void)
{

}

void ParticleHelpers_Create(void* data)
{

}

void ParticleHelpers_StageLoad(void)
{

}

void ParticleHelpers_Unknown1(int x, int y)
{
    int pos = x - 0x80000;
    for (int i = 0; i < 5; ++i) {
        int spawnX           = pos + (i << 18);
        EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, Debris_State_Fall, spawnX, y);
        debris->drawOrder    = Zone->drawOrderHigh;
        debris->gravity      = 0x3800;
        debris->velocity.y   = 0;
        debris->velocity.x   = 0x30 * ((spawnX - x) >> 8);
        if (debris->position.x < x)
            debris->direction = FLIP_X;
        CPZ1Intro_Particle_CB((Entity *)debris);
    }
}

void ParticleHelpers_Unknown2(void *debrisState, void (*callback)(EntityDebris *), int speed, int xOffset, int yOffset, int maxX, int maxY)
{
#if RETRO_USE_PLUS
    int x = RSDK.Random(-(maxX >> 1), maxX >> 1, &Zone->randKey) + xOffset;
    int y = RSDK.Random(-(maxY >> 1), maxY >> 1, &Zone->randKey) + yOffset;
#else
    int x = RSDK.Rand(-(maxX >> 1), maxX >> 1) + xOffset;
    int y = RSDK.Rand(-(maxY >> 1), maxY >> 1) + yOffset;
#endif
    EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, debrisState, x, y);
    debris->drawOrder    = Zone->drawOrderHigh;
    debris->gravity      = 0x3800;
    debris->velocity.x   = ((x - xOffset) >> 8) * (speed >> 8);
    debris->velocity.y   = ((y - yOffset) >> 8) * (speed >> 8);
    if (debris->position.x < xOffset)
        debris->direction = FLIP_X;
    if (callback)
        callback(debris);
}


void ParticleHelpers_EditorDraw(void)
{

}

void ParticleHelpers_EditorLoad(void)
{

}

void ParticleHelpers_Serialize(void)
{

}

