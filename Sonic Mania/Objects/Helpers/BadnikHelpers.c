// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BadnikHelpers Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectBadnikHelpers *BadnikHelpers = NULL;

void BadnikHelpers_Update(void) {}

void BadnikHelpers_LateUpdate(void) {}

void BadnikHelpers_StaticUpdate(void) {}

void BadnikHelpers_Draw(void) {}

void BadnikHelpers_Create(void *data) {}

void BadnikHelpers_StageLoad(void) {}

int BadnikHelpers_Oscillate(int origin, int speed, int amplitude)
{
    RSDK_THIS_GEN(); // generic type

    self->angle = (self->angle + speed) & 0xFF;
    return ((RSDK.Sin256(self->angle) << amplitude) + origin) & 0xFFFF0000;
}

void BadnikHelpers_HandleExplode(int xMin, int xMax, int yMin, int yMax, uint8 drawOrder)
{
    RSDK_THIS_GEN(); // generic type

    int32 x = self->position.x + (RSDK.Rand(xMin, xMax) << 16);
    int32 y = self->position.y + (RSDK.Rand(yMin, yMax) << 16);
    CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = drawOrder;
}

void BadnikHelpers_HandleExplode16(int xMin, int xMax, int yMin, int yMax, uint8 drawOrder)
{
    RSDK_THIS_GEN(); // generic type

    int x                                                                                            = self->position.x + RSDK.Rand(xMin, xMax);
    int y                                                                                            = self->position.y + RSDK.Rand(yMin, yMax);
    CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = drawOrder;
}

#if RETRO_INCLUDE_EDITOR
void BadnikHelpers_EditorDraw(void) {}

void BadnikHelpers_EditorLoad(void) {}
#endif

void BadnikHelpers_Serialize(void) {}
