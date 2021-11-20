#include "SonicMania.h"

ObjectInk *Ink;

void Ink_Update(void)
{
    RSDK_THIS(Ink);
    RSDK.ProcessAnimation(&self->animator3);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        Player_CheckCollisionBox(player, self, &Ink->hitbox);
        Player_CheckCollisionBox(player, self, &Ink->hitbox2);
        if (Player_CheckCollisionBox(player, self, &Ink->hitbox3)) {
            if (!((1 << playerID) & self->inkedPlayers)) {
                self->inkedPlayers = self->inkedPlayers | (1 << playerID);
                switch (player->characterID) {
                    case ID_SONIC: RSDK.CopyPalette(self->type + 3, 2, 0, 2, 6); break;
                    case ID_TAILS: RSDK.CopyPalette(self->type + 3, 70, 0, 70, 6); break;
                    case ID_KNUCKLES: RSDK.CopyPalette(self->type + 3, 80, 0, 80, 6); break;
                }
                Ink->playerColours[playerID] = self->type + 1;
                RSDK.SetSpriteAnimation(Ink->aniFrames, self->type + 6, &self->animator3, true, 0);
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
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);
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
        RSDK.SetSpriteAnimation(Ink->aniFrames, self->type, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Ink->aniFrames, self->type + 3, &self->animator2, true, 0);
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
void Ink_EditorDraw(void) {}

void Ink_EditorLoad(void) {}
#endif

void Ink_Serialize(void) { RSDK_EDITABLE_VAR(Ink, VAR_UINT8, type); }
