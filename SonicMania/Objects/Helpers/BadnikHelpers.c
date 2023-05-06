// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BadnikHelpers Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBadnikHelpers *BadnikHelpers = NULL;

void BadnikHelpers_Update(void) {}

void BadnikHelpers_LateUpdate(void) {}

void BadnikHelpers_StaticUpdate(void) {}

void BadnikHelpers_Draw(void) {}

void BadnikHelpers_Create(void *data) {}

void BadnikHelpers_StageLoad(void) {}

void BadnikHelpers_BadnikBreak(void *badnik, bool32 destroy, bool32 spawnAnimals)
{
    Entity *badnikEntity = badnik;

    if (spawnAnimals) {
        CREATE_ENTITY(Animals, INT_TO_VOID((Animals->animalTypes[(ZONE_RAND(0, 32) >> 4)]) + 1), badnikEntity->position.x, badnikEntity->position.y);
    }

    EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), badnikEntity->position.x, badnikEntity->position.y);
    explosion->drawGroup       = Zone->objectDrawGroup[1];
    RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);

    if (destroy)
        destroyEntity(badnikEntity);
}

void BadnikHelpers_BadnikBreakUnseeded(void *badnik, bool32 destroy, bool32 spawnAnimals)
{
    Entity *badnikEntity = badnik;

    if (spawnAnimals) {
        CREATE_ENTITY(Animals, INT_TO_VOID((Animals->animalTypes[(RSDK.Rand(0, 32) >> 4)]) + 1), badnikEntity->position.x, badnikEntity->position.y);
    }

    EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), badnikEntity->position.x, badnikEntity->position.y);
    explosion->drawGroup       = Zone->objectDrawGroup[1];
    RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);

    if (destroy)
        destroyEntity(badnikEntity);
}

int32 BadnikHelpers_Oscillate(int32 origin, int32 speed, int32 amplitude)
{
    RSDK_THIS_GEN(); // generic type

    self->angle = (self->angle + speed) & 0xFF;
    return ((RSDK.Sin256(self->angle) << amplitude) + origin) & 0xFFFF0000;
}

#if GAME_INCLUDE_EDITOR
void BadnikHelpers_EditorDraw(void) {}

void BadnikHelpers_EditorLoad(void) {}
#endif

void BadnikHelpers_Serialize(void) {}
