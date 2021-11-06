#include "SonicMania.h"

ObjectBadnikHelpers *BadnikHelpers = NULL;

void BadnikHelpers_Update(void) {}

void BadnikHelpers_LateUpdate(void) {}

void BadnikHelpers_StaticUpdate(void) {}

void BadnikHelpers_Draw(void) {}

void BadnikHelpers_Create(void *data) {}

void BadnikHelpers_StageLoad(void) {}

int BadnikHelpers_Oscillate(int origin, int speed, int power)
{
    RSDK_THIS_GEN(); // generic type

    entity->angle = (entity->angle + speed) & 0xFF;
    return ((RSDK.Sin256(entity->angle) << power) + origin) & 0xFFFF0000;
}


void BadnikHelpers_HandleExplode(int xMin, int xMax, int yMin, int yMax, uint8 drawOrder)
{
    RSDK_THIS_GEN(); // generic type

    int x                                                                                   = entity->position.x + (RSDK.Rand(xMin, xMax) << 16);
    int y                                                                                   = entity->position.y + (RSDK.Rand(yMin, yMax) << 16);
    CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = drawOrder;
}

void BadnikHelpers_HandleExplode16(int xMin, int xMax, int yMin, int yMax, uint8 drawOrder)
{
    RSDK_THIS_GEN(); // generic type

    int x                                                                                   = entity->position.x + RSDK.Rand(xMin, xMax);
    int y                                                                                   = entity->position.y + RSDK.Rand(yMin, yMax);
    CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = drawOrder;
}

#if RETRO_INCLUDE_EDITOR
void BadnikHelpers_EditorDraw(void) {}

void BadnikHelpers_EditorLoad(void) {}
#endif

void BadnikHelpers_Serialize(void) {}
