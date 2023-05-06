// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BurningLog Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
                Player_CheckCollisionPlatform(player, self, &BurningLog->hitboxPlatform);

            if (Player_CheckCollisionTouch(player, self, &BurningLog->hitboxFlame)) {
                Player_ElementHurt(player, self, SHIELD_FIRE);
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
    self->drawGroup     = Zone->objectDrawGroup[0];

    if (data)
        self->timer = VOID_TO_INT(data);

    RSDK.SetSpriteAnimation(BurningLog->aniFrames, 0, &self->animator, true, 0);
}

void BurningLog_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        BurningLog->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Fireball.bin", SCOPE_STAGE);

    BurningLog->hitboxPlatform.left   = -8;
    BurningLog->hitboxPlatform.top    = -8;
    BurningLog->hitboxPlatform.right  = 8;
    BurningLog->hitboxPlatform.bottom = 8;

    BurningLog->hitboxFlame.left   = -8;
    BurningLog->hitboxFlame.top    = -16;
    BurningLog->hitboxFlame.right  = 8;
    BurningLog->hitboxFlame.bottom = 8;
}

#if GAME_INCLUDE_EDITOR
void BurningLog_EditorDraw(void) { BurningLog_Draw(); }

void BurningLog_EditorLoad(void) { BurningLog->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Fireball.bin", SCOPE_STAGE); }
#endif

void BurningLog_Serialize(void) { RSDK_EDITABLE_VAR(BurningLog, VAR_ENUM, timer); }
