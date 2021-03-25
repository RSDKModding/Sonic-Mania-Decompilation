#include "../SonicMania.h"

ObjectBurningLog *BurningLog;

void BurningLog_Update()
{
    RSDK_THIS(BurningLog);
    RSDK.ProcessAnimation(&entity->data);
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->position.y += entity->velocity.y;
        entity->velocity.y += 0x3800;
        foreach_active(Player, player)
        {
            if (entity->velocity.y < 0x380000)
                Player_CheckCollisionPlatform(player, entity, &BurningLog->hitbox);
            if (Player_CheckCollisionTouch(player, entity, &BurningLog->hitbox2) && player->shield != SHIELD_FIRE) {
                if (player->state != Player_State_Hit && player->state != Player_State_Die && player->state != Player_State_Drown
                    && !player->invincibleTimer && !player->blinkTimer) {
                    if (player->position.x > entity->position.x)
                        player->velocity.x = 0x20000;
                    else
                        player->velocity.x = -0x20000;
                    Player_Hit(player);
                }
            }
        }
    }
    else {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
}

void BurningLog_LateUpdate() {}

void BurningLog_StaticUpdate() {}

void BurningLog_Draw()
{
    RSDK_THIS(BurningLog);
    RSDK.DrawSprite(&entity->data, 0, 0);
}

void BurningLog_Create(void *data)
{
    RSDK_THIS(BurningLog);
    entity->active        = ACTIVE_NORMAL;
    entity->visible       = true;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x10000000;
    entity->drawOrder     = Zone->drawOrderLow;
    if (data)
        entity->timer = voidToInt(data);
    RSDK.SetSpriteAnimation(BurningLog->spriteIndex, 0, &entity->data, true, 0);
}

void BurningLog_StageLoad()
{
    if (RSDK.CheckStageFolder("GHZ"))
        BurningLog->spriteIndex = RSDK.LoadSpriteAnimation("GHZ/Fireball.bin", SCOPE_STAGE);
    BurningLog->hitbox.left    = -8;
    BurningLog->hitbox.top     = -8;
    BurningLog->hitbox.right   = 8;
    BurningLog->hitbox.bottom  = 8;
    BurningLog->hitbox2.left   = -8;
    BurningLog->hitbox2.top    = -16;
    BurningLog->hitbox2.right  = 8;
    BurningLog->hitbox2.bottom = 8;
}

void BurningLog_EditorDraw() {}

void BurningLog_EditorLoad() {}

void BurningLog_Serialize() { RSDK_EDITABLE_VAR(BurningLog, VAR_ENUM, timer); }
