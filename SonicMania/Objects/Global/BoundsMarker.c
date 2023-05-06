// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BoundsMarker Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBoundsMarker *BoundsMarker;

void BoundsMarker_Update(void)
{
    RSDK_THIS(BoundsMarker);

    for (int32 p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        BoundsMarker_ApplyBounds(player, self, false);
    }
}

void BoundsMarker_LateUpdate(void) {}

void BoundsMarker_StaticUpdate(void) {}

void BoundsMarker_Draw(void) {}

void BoundsMarker_Create(void *data)
{
    RSDK_THIS(BoundsMarker);

    if (!SceneInfo->inEditor) {
        if (self->vsDisable && globals->gameMode == MODE_COMPETITION) {
            destroyEntity(self);
        }
        else {
            self->active = ACTIVE_XBOUNDS;
            self->width  = self->width ? (self->width << 15) : (48 << 15);
            self->updateRange.x += self->width << 15;

            for (int32 p = 0; p < Player->playerCount; ++p) {
                EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
                BoundsMarker_ApplyBounds(player, self, true);
            }
        }
    }
}

void BoundsMarker_StageLoad(void) {}

void BoundsMarker_ApplyBounds(EntityPlayer *player, EntityBoundsMarker *marker, bool32 setPos)
{
    uint16 playerID = RSDK.GetEntitySlot(player);

    if (Player_CheckValidState(player) || player->classID == DebugMode->classID) {
        if (abs(marker->position.x - player->position.x) < marker->width) {
            switch (marker->type) {
                case BOUNDSMARKER_ANY_Y:
                    Zone->playerBoundsB[playerID] = marker->position.y;
                    Zone->cameraBoundsB[playerID] = FROM_FIXED(Zone->playerBoundsB[playerID]);
                    Zone->deathBoundary[playerID] = marker->position.y;
                    break;

                case BOUNDSMARKER_ABOVE_Y:
                    if (player->position.y < marker->position.y - (marker->offset << 16)) {
                        Zone->playerBoundsB[playerID] = marker->position.y;
                        Zone->cameraBoundsB[playerID] = FROM_FIXED(Zone->playerBoundsB[playerID]);
                        Zone->deathBoundary[playerID] = marker->position.y;
                    }
                    break;

                case BOUNDSMARKER_BELOW_Y:
                    if (player->position.y > marker->position.y + (marker->offset << 16)) {
                        Zone->playerBoundsT[playerID] = marker->position.y;
                        Zone->cameraBoundsT[playerID] = FROM_FIXED(Zone->playerBoundsT[playerID]);
                    }
                    break;

                case BOUNDSMARKER_BELOW_Y_ANY:
                    Zone->playerBoundsT[playerID] = marker->position.y;
                    Zone->cameraBoundsT[playerID] = FROM_FIXED(Zone->playerBoundsT[playerID]);
                    break;

                default: break;
            }
        }

        if (setPos) {
            EntityCamera *camera = player->camera;
            if (camera) {
                camera->boundsL = Zone->cameraBoundsL[playerID];
                camera->boundsR = Zone->cameraBoundsR[playerID];
                camera->boundsT = Zone->cameraBoundsT[playerID];
                camera->boundsB = Zone->cameraBoundsB[playerID];
            }
        }
    }
}
void BoundsMarker_ApplyAllBounds(EntityPlayer *player, bool32 setPos)
{
    if (Player_CheckValidState(player) || player->classID == DebugMode->classID) {
        foreach_all(BoundsMarker, entity) { BoundsMarker_ApplyBounds(player, entity, setPos); }
    }
}

#if GAME_INCLUDE_EDITOR
void BoundsMarker_EditorDraw(void)
{
    RSDK_THIS(BoundsMarker);

    Animator animator;
    RSDK.SetSpriteAnimation(BoundsMarker->aniFrames, 0, &animator, true, 2);
    RSDK.DrawSprite(&animator, NULL, false);

    int32 w             = self->width ? (self->width << 15) : (48 << 15);
    self->updateRange.x = w;

    // Bounds
    RSDK_DRAWING_OVERLAY(true);

    RSDK.DrawLine(self->position.x - w, self->position.y, self->position.x + w, self->position.y, 0xFFFF00, 0xFF, INK_NONE, false);
    if (self->type == BOUNDSMARKER_ABOVE_Y) {
        RSDK.DrawLine(self->position.x + w, self->position.y - (self->offset << 16), self->position.x + w, self->position.y - (self->offset << 16),
                      0xFFFF00, 0x80, INK_BLEND, false);
    }
    else if (self->type == BOUNDSMARKER_BELOW_Y) {
        RSDK.DrawLine(self->position.x + w, self->position.y + (self->offset << 16), self->position.x + w, self->position.y + (self->offset << 16),
                      0xFFFF00, 0x80, INK_BLEND, false);
    }

    RSDK_DRAWING_OVERLAY(false);
}

void BoundsMarker_EditorLoad(void)
{
    BoundsMarker->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(BoundsMarker, type);
    RSDK_ENUM_VAR("Any Y", BOUNDSMARKER_ANY_Y);
    RSDK_ENUM_VAR("Above Y", BOUNDSMARKER_ABOVE_Y);
    RSDK_ENUM_VAR("Below Y", BOUNDSMARKER_BELOW_Y);
    RSDK_ENUM_VAR("Below Y Any", BOUNDSMARKER_BELOW_Y_ANY);
}
#endif

void BoundsMarker_Serialize(void)
{
    RSDK_EDITABLE_VAR(BoundsMarker, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(BoundsMarker, VAR_ENUM, width);
    RSDK_EDITABLE_VAR(BoundsMarker, VAR_BOOL, vsDisable);
    RSDK_EDITABLE_VAR(BoundsMarker, VAR_ENUM, offset);
}
