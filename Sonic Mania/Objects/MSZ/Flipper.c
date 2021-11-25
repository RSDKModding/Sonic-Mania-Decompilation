#include "SonicMania.h"

ObjectFlipper *Flipper;

void Flipper_Update(void)
{
    RSDK_THIS(Flipper);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.animationID == 1 && self->animator.frameID == self->animator.frameCount - 1)
        RSDK.SetSpriteAnimation(Flipper->aniFrames, 0, &self->animator, false, 0);

    if (self->direction == FLIP_NONE) {
        foreach_active(Player, player)
        {
            int playerID = RSDK.GetEntityID(player);
            if (player->position.x >= self->position.x + 0x40000)
                self->hitbox.top = ((player->position.x - self->position.x - 0x40000) >> 17) - 20;
            else
                self->hitbox.top = -20;
            self->hitbox.bottom = self->hitbox.top + 16;
            if (Player_CheckCollisionPlatform(player, self, &self->hitbox)) {
                self->activePlayers |= (1 << playerID);
                player->position.y += 0x80000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                if (player->jumpPress) {
                    for (int i = SLOT_PLAYER1; i < Player->playerCount; ++i) {
                        if ((1 << i) & self->activePlayers) {
                            EntityPlayer *playerPtr                  = RSDK_GET_ENTITY(i, Player);
                            int vel                                  = (playerPtr->position.x - self->position.x) - 0x40000;
                            playerPtr->animator.speed = 120;
                            playerPtr->state                         = Player_State_Air;
                            playerPtr->jumpAbilityTimer              = 1;
                            playerPtr->onGround                      = false;
                            playerPtr->velocity.x                    = vel / 7;
                            playerPtr->velocity.y                    = -0x68000 - (vel >> 3);
                        }
                    }
                    RSDK.SetSpriteAnimation(Flipper->aniFrames, 1, &self->animator, false, 0);
                    RSDK.PlaySfx(Flipper->sfxFlipper, false, 255);
                }
                else {
                    player->tileCollisions = true;
                    player->state          = Player_State_Air;
                    if (player->groundVel >= 0x10000) {
                        if (player->groundVel > 0x20000)
                            player->groundVel = 0x20000;
                        player->groundVel += 4096;
                    }
                    else {
                        player->groundVel = 0x10000;
                        player->groundVel += 4096;
                    }
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            int playerID = RSDK.GetEntityID(player);
            if (player->position.x <= self->position.x - 0x40000)
                self->hitbox.top = ((self->position.x - player->position.x - 0x40000) >> 17) - 20;
            else
                self->hitbox.top = -20;
            self->hitbox.bottom = self->hitbox.top + 16;
            if (Player_CheckCollisionPlatform(player, self, &self->hitbox)) {
                self->activePlayers |= (1 << playerID);
                player->position.y += 0x80000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                if (player->jumpPress) {
                    for (int i = SLOT_PLAYER1; i < Player->playerCount; ++i) {
                        if ((1 << i) & self->activePlayers) {
                            EntityPlayer *playerPtr                  = RSDK_GET_ENTITY(i, Player);
                            int vel                                  = (self->position.x - player->position.x) - 0x40000;
                            playerPtr->animator.speed = 120;
                            playerPtr->state                         = Player_State_Air;
                            playerPtr->jumpAbilityTimer              = 1;
                            playerPtr->onGround                      = false;
                            playerPtr->velocity.x                    = -(vel / 7);
                            playerPtr->velocity.y                    = -0x68000 - (vel >> 3);
                        }
                    }
                    RSDK.SetSpriteAnimation(Flipper->aniFrames, 1, &self->animator, false, 0);
                    RSDK.PlaySfx(Flipper->sfxFlipper, false, 255);
                }
                else {
                    player->tileCollisions = true;
                    player->state          = Player_State_Air;
                    if (player->groundVel <= -0x10000) {
                        if (player->groundVel < -0x20000)
                            player->groundVel = -0x20000;
                        player->groundVel -= 0x1000;
                    }
                    else {
                        player->groundVel = -0x10000;
                        player->groundVel -= 0x1000;
                    }
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void Flipper_LateUpdate(void) {}

void Flipper_StaticUpdate(void) {}

void Flipper_Draw(void)
{
    RSDK_THIS(Flipper);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Flipper_Create(void *data)
{
    RSDK_THIS(Flipper);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Flipper->aniFrames, 0, &self->animator, true, 0);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->hitbox.left   = Flipper->hitbox.left;
        self->hitbox.right  = Flipper->hitbox.right;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
    }
}

void Flipper_StageLoad(void)
{
    Flipper->aniFrames    = RSDK.LoadSpriteAnimation("MSZ/Flipper.bin", SCOPE_STAGE);
    Flipper->hitbox.left  = -12;
    Flipper->hitbox.right = 56;
    Flipper->sfxFlipper   = RSDK.GetSfx("Stage/Flipper.wav");
}

void Flipper_EditorDraw(void)
{
    RSDK_THIS(Flipper);
    RSDK.SetSpriteAnimation(Flipper->aniFrames, 0, &self->animator, true, 0);
    Flipper_Draw();
}

void Flipper_EditorLoad(void) { Flipper->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Flipper.bin", SCOPE_STAGE); }

void Flipper_Serialize(void) { RSDK_EDITABLE_VAR(Flipper, VAR_UINT8, direction); }
