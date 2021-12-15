// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BarStool Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectBarStool *BarStool;

void BarStool_Update(void)
{
    RSDK_THIS(BarStool);

    if (self->spinSpeed <= 0) {
        if (!self->activePlayers)
            self->spinSpeed -= (self->spinSpeed >> 4);
        if (self->spinSpeed < -0x100000)
            self->spinSpeed = -0x100000;
    }
    else {
        if (!self->activePlayers)
            self->spinSpeed -= (self->spinSpeed >> 4);
        if (self->spinSpeed > 0x100000)
            self->spinSpeed = 0x100000;
    }

    self->activePlayers = 0;
    if (abs(self->spinSpeed) > 0x10000 && !(Zone->timer & 0xF))
        RSDK.PlaySfx(BarStool->sfxSpin, false, 255);

    int playerID = 0;
    foreach_active(Player, player)
    {
        int velY = player->velocity.y;
        if (Player_CheckCollisionPlatform(player, self, &BarStool->hitbox)) {
            bool32 flag = true;
            if (abs(player->velocity.x) <= 0x20000 && player->state != Player_State_None) {
                if (abs(self->spinSpeed) <= 0x10000)
                    flag = false;
            }
#if RETRO_USE_PLUS
            flag = flag && player->state != Player_State_MightyHammerDrop;
#endif
            if (flag) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_TURNTABLE, &player->animator, false, 0);
                if (player->state != Player_State_None) {
                    if (velY >= 0)
                        self->spinSpeed += player->velocity.x;
                    self->playerVal1[playerID] = 0;
                    self->playerVal2[playerID] = (player->position.x - self->position.x) >> 1;
                    player->state                = Player_State_None;
                    player->nextAirState         = StateMachine_None;
                    player->nextGroundState      = StateMachine_None;
                }
                player->velocity.x = 0;
                player->groundVel  = 0;

                int ang = self->playerVal1[playerID] >> 16;

                player->position.x = clampVal((self->playerVal2[playerID] >> 8) * RSDK.Cos256(ang), -0x100000, 0x100000);
                ang &= 0xFF;

                int frame = (ang / 21) % 12;
                if (self->spinSpeed <= 0)
                    player->animator.frameID = 11 - frame;
                else
                    player->animator.frameID = frame;
                player->animator.speed = 0;
                player->position.x += self->position.x;

                if (self->playerVal2[playerID] <= 0)
                    self->playerVal2[playerID] -= 0x2000;
                else
                    self->playerVal2[playerID] += 0x2000;

                self->playerVal1[playerID] += self->spinSpeed;

                if (abs(self->playerVal2[playerID]) > 0x100000) {
                    int speed = self->spinSpeed;
                    bool32 flag = true;

                    if (!player->left && !player->right) {
                        if (player->position.x <= self->position.x)
                            speed = -abs(speed);
                        else
                            speed = abs(speed);
                    }
                    else if (player->left) {
                        if (player->position.x < self->position.x)
                            speed = -abs(speed);
                        else
                            flag = false;
                    }
                    else {
                        if (player->position.x > self->position.x)
                            speed = abs(speed);
                        else
                            flag = false;
                    }

                    if (flag) {
                        player->velocity.x = speed;
                        player->onGround   = false;
                        player->velocity.y = -0x20000;
                        player->state      = Player_State_Air;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_AIRWALK, &player->animator, false, 0);
                        RSDK.PlaySfx(BarStool->sfxHop, false, 0xFF);
                    }
                }
                if (player->jumpPress && player->state == Player_State_None) {
                    int speed = self->spinSpeed;
                    if (player->position.x <= self->position.x) {
                        speed = -abs(speed);
                    }
                    else {
                        speed = -speed;
                    }

                    player->velocity.x = speed;
                    player->onGround   = false;
                    player->velocity.y = -0x40000;
                    player->state      = Player_State_Air;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                }

                self->activePlayers |= 1 << playerID;
            }
        }

        playerID++;
    }

    self->spinAngle         = (self->spinAngle + (self->spinSpeed >> 2)) & 0x7FFFF;
    self->animator1.frameID = self->spinAngle >> 16;
}

void BarStool_LateUpdate(void) {}

void BarStool_StaticUpdate(void) {}

void BarStool_Draw(void)
{
    RSDK_THIS(BarStool);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void BarStool_Create(void *data)
{
    RSDK_THIS(BarStool);
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(BarStool->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(BarStool->aniFrames, 1, &self->animator2, true, self->height);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
    }
}

void BarStool_StageLoad(void)
{
    BarStool->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/BarStool.bin", SCOPE_STAGE);
    BarStool->hitbox.left   = -16;
    BarStool->hitbox.top    = -24;
    BarStool->hitbox.right  = 16;
    BarStool->hitbox.bottom = -16;
    BarStool->sfxSpin       = RSDK.GetSfx("MSZ/StoolSpin.wav");
    BarStool->sfxHop        = RSDK.GetSfx("MSZ/StoolHop.wav");
}

void BarStool_EditorDraw(void)
{
    RSDK_THIS(BarStool);
    RSDK.SetSpriteAnimation(BarStool->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(BarStool->aniFrames, 1, &self->animator2, true, self->height);

    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void BarStool_EditorLoad(void) { BarStool->aniFrames = RSDK.LoadSpriteAnimation("MSZ/BarStool.bin", SCOPE_STAGE); }

void BarStool_Serialize(void) { RSDK_EDITABLE_VAR(BarStool, VAR_UINT8, height); }
