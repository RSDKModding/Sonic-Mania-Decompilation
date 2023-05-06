// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Propeller Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPropeller *Propeller;

void Propeller_Update(void)
{
    RSDK_THIS(Propeller);

    RSDK.ProcessAnimation(&self->animator);
    Hitbox *hitboxHurt = RSDK.GetHitbox(&self->animator, 0);

    self->hitboxFan.top    = (RSDK.Sin256(2 * Zone->timer) >> 4) - 64;
    self->hitboxFan.bottom = -8;

    self->hitboxTrigger.top    = -96;
    self->hitboxTrigger.bottom = -8;

    foreach_active(Player, player)
    {
        int32 playerID         = RSDK.GetEntitySlot(player);
        bool32 propellerActive = false;

        int32 anim = player->animator.animationID;
        if (anim != ANI_SHAFT_SWING && anim != ANI_HURT && self->fanEnabled
            && RSDK.CheckObjectCollisionTouchBox(self, &self->hitboxFan, player, &self->hitboxPlayer)) {
            propellerActive = true;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
            player->state      = Player_State_Air;
            player->onGround   = false;
            player->velocity.y = 0;
            player->position.y += (self->position.y + (self->hitboxFan.top << 16) - player->position.y) >> 4;
        }

        if (RSDK.CheckObjectCollisionTouchBox(self, &self->hitboxTrigger, player, &self->hitboxPlayer)) {
            if (!((1 << playerID) & self->activePlayers) && propellerActive) {
                RSDK.PlaySfx(Propeller->sfxFan, false, 0xFF);
                self->activePlayers |= (1 << playerID);
            }
        }
        else {
            self->activePlayers &= ~(1 << playerID);
        }

        if (Player_CheckCollisionTouch(player, self, hitboxHurt))
            Player_Hurt(player, self);
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
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->hitboxPlayer.left   = -1;
        self->hitboxPlayer.top    = -1;
        self->hitboxPlayer.right  = 1;
        self->hitboxPlayer.bottom = 1;

        self->hitboxTrigger.left  = -104;
        self->hitboxTrigger.right = 104;

        self->hitboxFan.left  = -104;
        self->hitboxFan.right = 104;

        RSDK.SetSpriteAnimation(Propeller->aniFrames, 0, &self->animator, true, 0);
    }
}

void Propeller_StageLoad(void)
{
    Propeller->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Propeller.bin", SCOPE_STAGE);

    Propeller->sfxFan = RSDK.GetSfx("FBZ/FBZFan.wav");
}

#if GAME_INCLUDE_EDITOR
void Propeller_EditorDraw(void)
{
    RSDK_THIS(Propeller);
    RSDK.SetSpriteAnimation(Propeller->aniFrames, 0, &self->animator, true, 0);

    Propeller_Draw();
}

void Propeller_EditorLoad(void) { Propeller->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Propeller.bin", SCOPE_STAGE); }
#endif

void Propeller_Serialize(void) { RSDK_EDITABLE_VAR(Propeller, VAR_BOOL, fanEnabled); }
