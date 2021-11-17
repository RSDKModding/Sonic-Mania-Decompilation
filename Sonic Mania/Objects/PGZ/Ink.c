#include "SonicMania.h"

ObjectInk *Ink;

void Ink_Update(void)
{
    RSDK_THIS(Ink);
    RSDK.ProcessAnimation(&entity->animator3);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        Player_CheckCollisionBox(player, entity, &Ink->hitbox);
        Player_CheckCollisionBox(player, entity, &Ink->hitbox2);
        if (Player_CheckCollisionBox(player, entity, &Ink->hitbox3)) {
            if (!((1 << playerID) & entity->inkedPlayers)) {
                entity->inkedPlayers = entity->inkedPlayers | (1 << playerID);
                switch (player->characterID) {
                    case ID_SONIC: RSDK.CopyPalette(entity->type + 3, 2, 0, 2, 6); break;
                    case ID_TAILS: RSDK.CopyPalette(entity->type + 3, 70, 0, 70, 6); break;
                    case ID_KNUCKLES: RSDK.CopyPalette(entity->type + 3, 80, 0, 80, 6); break;
                }
                Ink->playerColours[playerID] = entity->type + 1;
                RSDK.SetSpriteAnimation(Ink->aniFrames, entity->type + 6, &entity->animator3, true, 0);
            }
        }
        else {
            entity->inkedPlayers &= ~(1 << playerID);
        }
    }
}

void Ink_LateUpdate(void) {}

void Ink_StaticUpdate(void) {}

void Ink_Draw(void)
{
    RSDK_THIS(Ink);

    RSDK_sceneInfo->entity->inkEffect = INK_SUB;
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    entity->inkEffect = INK_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Ink_Create(void *data)
{
    RSDK_THIS(Ink);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->alpha         = 0x180;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Ink->aniFrames, entity->type, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Ink->aniFrames, entity->type + 3, &entity->animator2, true, 0);
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
