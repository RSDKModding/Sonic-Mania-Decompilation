#include "SonicMania.h"

ObjectBurningLog *BurningLog;

void BurningLog_Update(void)
{
    RSDK_THIS(BurningLog);
    RSDK.ProcessAnimation(&self->animator);
    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->position.y += self->velocity.y;
        self->velocity.y += 0x3800;
        foreach_active(Player, player)
        {
            if (self->velocity.y < 0x380000)
                Player_CheckCollisionPlatform(player, self, &BurningLog->hitbox);
            if (Player_CheckCollisionTouch(player, self, &BurningLog->hitbox2)) {
                Player_CheckElementalHit(player, self, SHIELD_FIRE);
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void BurningLog_LateUpdate(void) {}

void BurningLog_StaticUpdate(void) {}

void BurningLog_Draw(void)
{
    RSDK_THIS(BurningLog);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void BurningLog_Create(void *data)
{
    RSDK_THIS(BurningLog);
    self->active        = ACTIVE_NORMAL;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x10000000;
    self->drawOrder     = Zone->drawOrderLow;
    if (data)
        self->timer = voidToInt(data);
    RSDK.SetSpriteAnimation(BurningLog->aniFrames, 0, &self->animator, true, 0);
}

void BurningLog_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        BurningLog->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Fireball.bin", SCOPE_STAGE);
    BurningLog->hitbox.left    = -8;
    BurningLog->hitbox.top     = -8;
    BurningLog->hitbox.right   = 8;
    BurningLog->hitbox.bottom  = 8;
    BurningLog->hitbox2.left   = -8;
    BurningLog->hitbox2.top    = -16;
    BurningLog->hitbox2.right  = 8;
    BurningLog->hitbox2.bottom = 8;
}

#if RETRO_INCLUDE_EDITOR
void BurningLog_EditorDraw(void) { BurningLog_Draw(); }

void BurningLog_EditorLoad(void) { BurningLog->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Fireball.bin", SCOPE_STAGE); }
#endif

void BurningLog_Serialize(void) { RSDK_EDITABLE_VAR(BurningLog, VAR_ENUM, timer); }
