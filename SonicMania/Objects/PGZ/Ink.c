// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Ink Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectInk *Ink;

void Ink_Update(void)
{
    RSDK_THIS(Ink);

    RSDK.ProcessAnimation(&self->splashAnimator);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        Player_CheckCollisionBox(player, self, &Ink->hitboxBottleL);
        Player_CheckCollisionBox(player, self, &Ink->hitboxBottleR);

        if (Player_CheckCollisionBox(player, self, &Ink->hitboxBottleBottom) == C_TOP) {
            if (!((1 << playerID) & self->inkedPlayers)) {
                self->inkedPlayers |= 1 << playerID;

                switch (player->characterID) {
                    default: break;

                    // Bug Details:
                    // This actually wont work on sonic specifically, it uses the "old" sonic palette
                    // This palette starts at index 2, instead of index 64 like usual
                    // Fix:
                    // to fix this up to work as "intended", simply replace the "PLAYER_PALETTE_INDEX_SONIC_OLD"s with "PLAYER_PALETTE_INDEX_SONIC"
                    case ID_SONIC:
                        RSDK.CopyPalette(3 + self->type, PLAYER_PALETTE_INDEX_SONIC_OLD, 0, PLAYER_PALETTE_INDEX_SONIC_OLD,
                                         PLAYER_PRIMARY_COLOR_COUNT);
                        break;

                    case ID_TAILS:
                        RSDK.CopyPalette(3 + self->type, PLAYER_PALETTE_INDEX_TAILS, 0, PLAYER_PALETTE_INDEX_TAILS, PLAYER_PRIMARY_COLOR_COUNT);
                        break;

                    case ID_KNUCKLES:
                        RSDK.CopyPalette(3 + self->type, PLAYER_PALETTE_INDEX_KNUX, 0, PLAYER_PALETTE_INDEX_KNUX, 6);
                        break;

                        // This is an unused object that was scrapped before plus was created, so there's no mighty/ray code
                        // I've created a mock-up of what mighty/ray code could've looked like, had it been implemented:
                        // case ID_MIGHTY: RSDK.CopyPalette(3 + self->type, PLAYER_PALETTE_INDEX_MIGHTY, 0, PLAYER_PALETTE_INDEX_MIGHTY,
                        // PLAYER_PRIMARY_COLOR_COUNT); break; case ID_RAY: RSDK.CopyPalette(3 + self->type, PLAYER_PALETTE_INDEX_RAY, 0,
                        // PLAYER_PALETTE_INDEX_RAY, PLAYER_PRIMARY_COLOR_COUNT); break;
                }

                Ink->playerColors[playerID] = 1 + self->type;
                RSDK.SetSpriteAnimation(Ink->aniFrames, 6 + self->type, &self->splashAnimator, true, 0);
            }
        }
        else {
            self->inkedPlayers &= ~(1 << playerID);
        }
    }
}

void Ink_LateUpdate(void) {}

void Ink_StaticUpdate(void) {}

void Ink_Draw(void)
{
    RSDK_THIS(Ink);

    self->inkEffect = INK_SUB;
    RSDK.DrawSprite(&self->splashAnimator, NULL, false);
    RSDK.DrawSprite(&self->contentAnimator, NULL, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->bottleAnimator, NULL, false);
}

void Ink_Create(void *data)
{
    RSDK_THIS(Ink);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->alpha         = 0x180;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Ink->aniFrames, self->type, &self->bottleAnimator, true, 0);
        RSDK.SetSpriteAnimation(Ink->aniFrames, self->type + 3, &self->contentAnimator, true, 0);
    }
}

void Ink_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        Ink->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Ink.bin", SCOPE_STAGE);

    Ink->hitboxBottleL.left   = -24;
    Ink->hitboxBottleL.top    = -30;
    Ink->hitboxBottleL.right  = -16;
    Ink->hitboxBottleL.bottom = 30;

    Ink->hitboxBottleR.left   = 16;
    Ink->hitboxBottleR.top    = -30;
    Ink->hitboxBottleR.right  = 24;
    Ink->hitboxBottleR.bottom = 30;

    Ink->hitboxBottleBottom.left   = -24;
    Ink->hitboxBottleBottom.top    = 26;
    Ink->hitboxBottleBottom.right  = 24;
    Ink->hitboxBottleBottom.bottom = 30;

    for (int32 p = 0; p < PLAYER_COUNT; ++p) Ink->playerColors[p] = 0;
}

#if RETRO_INCLUDE_EDITOR
void Ink_EditorDraw(void)
{
    RSDK_THIS(Ink);

    self->alpha         = 0x180;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Ink->aniFrames, self->type, &self->bottleAnimator, true, 0);
    RSDK.SetSpriteAnimation(Ink->aniFrames, self->type + 3, &self->contentAnimator, true, 0);

    Ink_Draw();
}

void Ink_EditorLoad(void)
{
    Ink->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Ink.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Ink, type);
    RSDK_ENUM_VAR("Cyan", INK_C);
    RSDK_ENUM_VAR("Magenta", INK_M);
    RSDK_ENUM_VAR("Yellow", INK_Y);
}
#endif

void Ink_Serialize(void) { RSDK_EDITABLE_VAR(Ink, VAR_UINT8, type); }
