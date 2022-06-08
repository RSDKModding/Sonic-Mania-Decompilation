// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BadnikHelpers Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBadnikHelpers *BadnikHelpers = NULL;

// NOTE:
// I'm not actually sure *what* this object was for
// BadnikHelpers_Oscillate was always inlined so I can't say for sure that it was a real func, though I assume it was?
// I've never seen definitive proof of any funcs this object may have once had so be it what you will

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

#if RETRO_INCLUDE_EDITOR
void BadnikHelpers_EditorDraw(void) {}

void BadnikHelpers_EditorLoad(void) {}
#endif

void BadnikHelpers_Serialize(void) {}
