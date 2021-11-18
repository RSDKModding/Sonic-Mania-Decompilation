#include "SonicMania.h"

ObjectPropeller *Propeller;

void Propeller_Update(void)
{
    RSDK_THIS(Propeller);
    RSDK.ProcessAnimation(&self->animator);
    Hitbox *hitbox       = RSDK.GetHitbox(&self->animator, 0);
    self->hitbox2.bottom = -8;
    self->hitbox2.top    = (RSDK.Sin256(2 * Zone->timer) >> 4) - 64;
    self->hitbox.top     = -96;
    self->hitbox.bottom  = -8;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        bool32 flag    = 0;
        int32 anim     = player->playerAnimator.animationID;
        if (anim != ANI_SHAFTSWING && anim != ANI_HURT && self->fanEnabled
            && RSDK.CheckObjectCollisionTouchBox(self, &self->hitbox2, player, &self->playerHitbox)) {
            flag = true;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->playerAnimator, false, 0);
            player->state      = Player_State_Air;
            player->onGround   = false;
            player->velocity.y = 0;
            player->position.y += (self->position.y + (self->hitbox2.top << 16) - player->position.y) >> 4;
        }

        if (RSDK.CheckObjectCollisionTouchBox(self, &self->hitbox, player, &self->playerHitbox)) {
            if (!((1 << playerID) & self->activePlayers) && flag) {
                RSDK.PlaySfx(Propeller->sfxFan, 0, 255);
                self->activePlayers |= (1 << playerID);
            }
        }
        else {
            self->activePlayers &= ~(1 << playerID);
        }

        if (Player_CheckCollisionTouch(player, self, hitbox)) {
            Player_CheckHit(player, self);
        }
    }
}

void Propeller_LateUpdate(void) {}

void Propeller_StaticUpdate(void) {}

void Propeller_Draw(void)
{
    RSDK_THIS(Propeller);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Propeller_Create(void *data)
{
    RSDK_THIS(Propeller);
    if (!SceneInfo->inEditor) {
        self->active              = ACTIVE_BOUNDS;
        self->visible             = true;
        self->drawOrder           = Zone->drawOrderHigh;
        self->updateRange.x       = 0x800000;
        self->updateRange.y       = 0x800000;
        self->playerHitbox.left   = -1;
        self->playerHitbox.top    = -1;
        self->playerHitbox.right  = 1;
        self->playerHitbox.bottom = 1;
        self->hitbox.left         = -104;
        self->hitbox.right        = 104;
        self->hitbox2.left        = -104;
        self->hitbox2.right       = 104;
        RSDK.SetSpriteAnimation(Propeller->aniFrames, 0, &self->animator, true, 0);
    }
}

void Propeller_StageLoad(void)
{
    Propeller->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Propeller.bin", SCOPE_STAGE);
    Propeller->sfxFan    = RSDK.GetSFX("FBZ/FBZFan.wav");
}

#if RETRO_INCLUDE_EDITOR
void Propeller_EditorDraw(void) {}

void Propeller_EditorLoad(void) {}
#endif

void Propeller_Serialize(void) { RSDK_EDITABLE_VAR(Propeller, VAR_BOOL, fanEnabled); }
