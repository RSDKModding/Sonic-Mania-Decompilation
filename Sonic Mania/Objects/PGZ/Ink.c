// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Ink Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectInk *Ink;

void Ink_Update(void)
{
    RSDK_THIS(Ink);
    RSDK.ProcessAnimation(&self->splashAnimator);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        Player_CheckCollisionBox(player, self, &Ink->hitbox);
        Player_CheckCollisionBox(player, self, &Ink->hitbox2);
        if (Player_CheckCollisionBox(player, self, &Ink->hitbox3) == C_TOP) {
            if (!((1 << playerID) & self->inkedPlayers)) {
                self->inkedPlayers = self->inkedPlayers | (1 << playerID);
                switch (player->characterID) {
                    // This actually wont work on sonic specifically, it uses the "old" sonic palette
                    // This palette starts at index 2, instead of index 64 like usual
                    // To fix this up to work as "intended", simply replace the "2"s with "64"
                    case ID_SONIC: RSDK.CopyPalette(self->type + 3, 2, 0, 2, 6); break;
                    case ID_TAILS: RSDK.CopyPalette(self->type + 3, 70, 0, 70, 6); break;
                    case ID_KNUCKLES:
                        RSDK.CopyPalette(self->type + 3, 80, 0, 80, 6);
                        break;
                        // This is an unused object that was scrapped before plus was created, so there's no mighty/ray code
                        // I've created a mock-up of what mighty/ray code could've looked like, had it been implimented:
                        // case ID_MIGHTY: RSDK.CopyPalette(self->type + 3, 96, 0, 96, 6); break;
                        // case ID_RAY: RSDK.CopyPalette(self->type + 3, 113, 0, 113, 6); break;
                }
                Ink->playerColours[playerID] = self->type + 1;
                RSDK.SetSpriteAnimation(Ink->aniFrames, self->type + 6, &self->splashAnimator, true, 0);
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
        self->drawOrder     = Zone->drawOrderLow;
        self->alpha         = 0x180;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Ink->aniFrames, self->type, &self->bottleAnimator, true, 0);
        RSDK.SetSpriteAnimation(Ink->aniFrames, self->type + 3, &self->contentAnimator, true, 0);
    }
}

void Ink_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        Ink->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Ink.bin", SCOPE_STAGE);
    Ink->hitbox.left    = -24;
    Ink->hitbox.top     = -30;
    Ink->hitbox.right   = -16;
    Ink->hitbox.bottom  = 30;
    Ink->hitbox2.left   = 16;
    Ink->hitbox2.top    = -30;
    Ink->hitbox2.right  = 24;
    Ink->hitbox2.bottom = 30;
    Ink->hitbox3.left   = -24;
    Ink->hitbox3.top    = 26;
    Ink->hitbox3.right  = 24;
    Ink->hitbox3.bottom = 30;
    for (int32 p = 0; p < 4; ++p) Ink->playerColours[p] = 0;
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
