#include "SonicMania.h"

ObjectPullSwitch *PullSwitch;

void PullSwitch_Update(void)
{
    RSDK_THIS(PullSwitch);

    if (!self->activated) {
        if (self->field_5C > 0) {
            self->position.y -= 0x20000;
            self->field_5C -= 0x20000;
        }
    }
    else if (self->field_5C < 0x300000) {
        self->position.y += 0x20000;
        self->field_5C += 0x20000;
    }
    else {
        if (OOZSetup->fadeTimer > 16 || Smog->field_4) {
            RSDK.PlaySfx(PullSwitch->sfxSmogClear, 0, 255);
        }
        OOZSetup->fadeTimer = 0;
        Smog->field_4       = 0;
    }

    self->activated = false;
    foreach_active(Player, player)
    {
        int32 playerID       = RSDK.GetEntityID(player);
        Hitbox *playerHitbox = Player_GetHitbox(player);

        if (self->playerTimers[playerID]) {
            self->playerTimers[playerID]--;
        }
        else if (((1 << playerID) & self->activePlayers)) {
            player->position.x = self->position.x;
            player->position.y = self->position.y - (playerHitbox->top << 16) + 0x1A0000;
            self->activated  = true;
            if (player->state == Player_State_None) {
                if (player->jumpPress) {
                    self->activePlayers &= ~(1 << playerID);
                    self->playerTimers[playerID] = 60;
                    player->velocity.y             = -0x40000;
                    player->onGround               = false;
                    player->groundedStore          = false;
                    player->jumpAbilityTimer       = 1;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->animator.animationSpeed = 48;
                    player->state                         = Player_State_Air;
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
                self->playerTimers[playerID] = 60;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &PullSwitch->hitbox)) {
                self->activePlayers |= 1 << playerID;
                player->state           = Player_State_None;
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, false, 0);
                player->velocity.x  = 0;
                player->velocity.y  = 0;
                player->groundVel   = 0;
                player->controlLock = 0;
                player->position.x  = self->position.x;
                player->position.y  = self->position.y - (playerHitbox->top << 16) + 0x1A0000;
                self->activated   = true;
            }
        }
    }
}

void PullSwitch_LateUpdate(void) {}

void PullSwitch_StaticUpdate(void) {}

void PullSwitch_Draw(void)
{
    RSDK_THIS(PullSwitch);
    SpriteFrame *frame = RSDK.GetFrame(PullSwitch->aniFrames, 0, 1);
    frame->height      = (self->position.y - self->drawPos.y) >> 16;
    frame->sprY        = self->sprY + self->sprHeight - frame->height;
    RSDK.DrawSprite(&self->animator3, &self->drawPos, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator2, &self->drawPos, false);
}

void PullSwitch_Create(void *data)
{
    RSDK_THIS(PullSwitch);
    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawFX        = FX_FLIP;
        self->drawOrder     = Zone->drawOrderLow;
        self->drawPos       = self->position;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->animator1, true, 2);
        RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->animator3, true, 1);
        SpriteFrame *frame = RSDK.GetFrame(PullSwitch->aniFrames, 0, 1);
        self->sprY       = frame->sprY;
        self->sprHeight  = frame->height;
    }
}

void PullSwitch_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        PullSwitch->aniFrames = RSDK.LoadSpriteAnimation("OOZ/PullSwitch.bin", SCOPE_STAGE);
    PullSwitch->hitbox.left   = -16;
    PullSwitch->hitbox.top    = 20;
    PullSwitch->hitbox.right  = 16;
    PullSwitch->hitbox.bottom = 28;
    PullSwitch->sfxButton     = RSDK.GetSfx("Stage/Button.wav");
    PullSwitch->sfxSmogClear  = RSDK.GetSfx("OOZ/SmogClear.wav");
}

#if RETRO_INCLUDE_EDITOR
void PullSwitch_EditorDraw(void)
{
    RSDK_THIS(PullSwitch);
    self->drawPos = self->position;
    RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->animator1, false, 2);
    RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->animator2, false, 0);
    RSDK.SetSpriteAnimation(PullSwitch->aniFrames, 0, &self->animator3, false, 1);

    RSDK.DrawSprite(&self->animator3, &self->drawPos, false);
    RSDK.DrawSprite(&self->animator2, &self->drawPos, false);

    self->drawPos.y += 0x300000;
    RSDK.DrawSprite(&self->animator1, &self->drawPos, false);
}

void PullSwitch_EditorLoad(void) { PullSwitch->aniFrames = RSDK.LoadSpriteAnimation("OOZ/PullSwitch.bin", SCOPE_STAGE); }
#endif

void PullSwitch_Serialize(void) {}
