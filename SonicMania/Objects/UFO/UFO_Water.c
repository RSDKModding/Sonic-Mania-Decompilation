// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_Water Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUFO_Water *UFO_Water;

void UFO_Water_Update(void) {}

void UFO_Water_LateUpdate(void) {}

void UFO_Water_StaticUpdate(void)
{
    if (!(UFO_Setup->timer & 3)) {
        UFO_Water->timer = (UFO_Water->timer + 128) & 0x7FF;

        RSDK.DrawAniTiles(UFO_Water->aniFrames, 712, UFO_Water->timer & 0x1FF, (UFO_Water->timer >> 2) & 0xFF80, 128, 128);
    }
}

void UFO_Water_Draw(void) {}

void UFO_Water_Create(void *data) {}

void UFO_Water_StageLoad(void) { UFO_Water->aniFrames = RSDK.LoadSpriteSheet("SpecialUFO/Water.gif", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void UFO_Water_EditorDraw(void) {}

void UFO_Water_EditorLoad(void) {}
#endif

void UFO_Water_Serialize(void) {}
