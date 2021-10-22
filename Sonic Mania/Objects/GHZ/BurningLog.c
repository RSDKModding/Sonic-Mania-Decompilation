#include "SonicMania.h"

ObjectBurningLog *BurningLog;

void BurningLog_Update(void)
{
    RSDK_THIS(BurningLog);
    RSDK.ProcessAnimation(&entity->animator);
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->position.y += entity->velocity.y;
        entity->velocity.y += 0x3800;
        foreach_active(Player, player)
        {
            if (entity->velocity.y < 0x380000)
                Player_CheckCollisionPlatform(player, entity, &BurningLog->hitbox);
            if (Player_CheckCollisionTouch(player, entity, &BurningLog->hitbox2)) {
                Player_CheckElementalHit(player, entity, SHIELD_FIRE);
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void BurningLog_LateUpdate(void) {}

void BurningLog_StaticUpdate(void) {}

void BurningLog_Draw(void)
{
    RSDK_THIS(BurningLog);
    RSDK.DrawSprite(&entity->animator, NULL, false);
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
    RSDK.SetSpriteAnimation(BurningLog->spriteIndex, 0, &entity->animator, true, 0);
}

void BurningLog_StageLoad(void)
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

void BurningLog_EditorDraw(void) { BurningLog_Draw(); }

void BurningLog_EditorLoad(void) { BurningLog->spriteIndex = RSDK.LoadSpriteAnimation("GHZ/Fireball.bin", SCOPE_STAGE); }

void BurningLog_Serialize(void) { RSDK_EDITABLE_VAR(BurningLog, VAR_ENUM, timer); }
