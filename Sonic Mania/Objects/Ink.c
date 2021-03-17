#include "../SonicMania.h"

ObjectInk *Ink;

void Ink_Update()
{
    RSDK_THIS(Ink);
    RSDK.ProcessAnimation(&entity->data3);

    EntityPlayer *player = NULL;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        int playerID = RSDK.GetEntityID(player);
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
                Ink->playerTypes[playerID] = entity->type + 1;
                RSDK.SetSpriteAnimation(Ink->spriteIndex, entity->type + 6, &entity->data3, true, 0);
            }
        }
        else {
            entity->inkedPlayers &= ~(1 << playerID);
        }
    }
}

void Ink_LateUpdate() {}

void Ink_StaticUpdate() {}

void Ink_Draw()
{
    RSDK_THIS(Ink);
    RSDK_sceneInfo->entity->inkEffect = INK_SUB;
    RSDK.DrawSprite(&entity->data3, 0, 0);
    RSDK.DrawSprite(&entity->data2, 0, 0);
    entity->inkEffect = INK_NONE;
    RSDK.DrawSprite(&entity->data1, 0, 0);
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
        RSDK.SetSpriteAnimation(Ink->spriteIndex, entity->type, &entity->data1, true, 0);
        RSDK.SetSpriteAnimation(Ink->spriteIndex, entity->type + 3, &entity->data2, true, 0);
    }
}

void Ink_StageLoad()
{
    if (RSDK.CheckStageFolder("PSZ1"))
        Ink->spriteIndex = RSDK.LoadSpriteAnimation("PSZ1/Ink.bin", SCOPE_STAGE);
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
    for (int p = 0; p < 4; ++p) Ink->playerTypes[p] = 0;
}

void Ink_EditorDraw() {}

void Ink_EditorLoad() {}

void Ink_Serialize() { RSDK_EDITABLE_VAR(Ink, VAR_UINT8, type); }
