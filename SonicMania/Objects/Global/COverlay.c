// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: COverlay Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCOverlay *COverlay;

void COverlay_Update(void) {}

void COverlay_LateUpdate(void) {}

void COverlay_StaticUpdate(void) {}

void COverlay_Draw(void)
{
    RSDK_THIS(COverlay);

    int32 tileX = 0;
    for (self->position.x = (ScreenInfo->position.x & 0xFFFFFFF0) << 16; tileX < (ScreenInfo->size.x >> 4) + 2; ++tileX) {
        int32 tileY = 0;
        for (self->position.y = (ScreenInfo->position.y & 0xFFFFFFF0) << 16; tileY < (ScreenInfo->size.y >> 4) + 2; ++tileY) {
            COverlay_DrawTile();
            self->position.y += TO_FIXED(16);
        }
        self->position.x += TO_FIXED(16);
    }
}

void COverlay_Create(void *data)
{
    RSDK_THIS(COverlay);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_ALWAYS;
        self->visible   = true;
        self->drawGroup = Zone->objectDrawGroup[1];
    }
}

void COverlay_StageLoad(void)
{
    COverlay->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);

    DEBUGMODE_ADD_OBJ(COverlay);
}

void COverlay_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(COverlay->aniFrames, 0, &DebugMode->animator, true, 0);
    DebugMode->animator.frameID = 4;

    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void COverlay_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    int32 count = 0;
    foreach_all(COverlay, overlay)
    {
        destroyEntity(overlay);
        ++count;
    }

    if (!count)
        CREATE_ENTITY(COverlay, NULL, self->position.x, self->position.y);
}

void COverlay_DrawTile(void)
{
    RSDK_THIS(COverlay);

    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    int32 tx             = 0;
    for (int32 x = 0; x < 0x10; ++x) {
        uint8 ty    = -1;
        uint8 th2   = -1;
        uint8 ty2   = -1;
        uint8 th    = -1;
        uint8 solid = 0;
        for (int32 y = 0; y < 0x10; ++y) {
            if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, player->collisionPlane, TO_FIXED(x), TO_FIXED(y), false)) {
                solid |= 1;
                th2 = y + 1;
                if (ty == 0xFF)
                    ty = y;
            }

            if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, player->collisionPlane, TO_FIXED(x), TO_FIXED(y), false)) {
                solid |= 2;
                th = y + 1;
                if (ty2 == 0xFF)
                    ty2 = y - 1;
            }
        }

        if ((ty <= ty2 || ty == 0xFF) && ty2 != 0xFF)
            ty = ty2;
        if (th2 < th)
            th = th2;

        if (ty != 0xFF) {
            switch (solid) {
                default: break;

                case 1:
                    RSDK.DrawLine(self->position.x + tx, self->position.y + TO_FIXED(ty), self->position.x + tx, self->position.y + TO_FIXED(th),
                                  0xE0E000, 0xFF, INK_NONE, false);
                    break;

                case 2:
                    RSDK.DrawLine(self->position.x + tx, self->position.y + TO_FIXED(ty), self->position.x + tx, self->position.y + TO_FIXED(th),
                                  0xE00000, 0xFF, INK_NONE, false);
                    break;

                case 3:
                    RSDK.DrawLine(self->position.x + tx, self->position.y + TO_FIXED(ty), self->position.x + tx, self->position.y + TO_FIXED(th),
                                  0xE0E0E0, 0xFF, INK_NONE, false);
                    break;
            }
        }

        tx += TO_FIXED(1);
    }
}

#if GAME_INCLUDE_EDITOR
void COverlay_EditorDraw(void) {}

void COverlay_EditorLoad(void) {}
#endif

void COverlay_Serialize(void) {}
