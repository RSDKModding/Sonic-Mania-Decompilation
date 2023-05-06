// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BarStool Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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

    int32 playerID = 0;
    foreach_active(Player, player)
    {
        int32 velY = player->velocity.y;
        if (Player_CheckCollisionPlatform(player, self, &BarStool->hitboxStool)) {
            bool32 canTurntable = true;
            if (abs(player->velocity.x) <= 0x20000 && player->state != Player_State_Static) {
                if (abs(self->spinSpeed) <= 0x10000)
                    canTurntable = false;
            }

#if MANIA_USE_PLUS
            canTurntable = canTurntable && player->state != Player_State_MightyHammerDrop;
#endif

            if (canTurntable) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_TURNTABLE, &player->animator, false, 0);
                if (player->state != Player_State_Static) {
                    if (velY >= 0)
                        self->spinSpeed += player->velocity.x;

                    self->playerAngle[playerID] = 0;
                    self->playerPos[playerID]   = (player->position.x - self->position.x) >> 1;

                    player->state           = Player_State_Static;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                }

                player->velocity.x = 0;
                player->groundVel  = 0;

                int32 ang = self->playerAngle[playerID] >> 16;

                player->position.x = CLAMP((self->playerPos[playerID] >> 8) * RSDK.Cos256(ang), -0x100000, 0x100000);
                ang &= 0xFF;

                int32 frame = (ang / 21) % 12;
                if (self->spinSpeed <= 0)
                    player->animator.frameID = 11 - frame;
                else
                    player->animator.frameID = frame;
                player->animator.speed = 0;

                player->position.x += self->position.x;

                if (self->playerPos[playerID] <= 0)
                    self->playerPos[playerID] -= 0x2000;
                else
                    self->playerPos[playerID] += 0x2000;

                self->playerAngle[playerID] += self->spinSpeed;

                if (abs(self->playerPos[playerID]) > 0x100000) {
                    int32 speed = self->spinSpeed;

                    bool32 shouldFling = true;
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
                            shouldFling = false;
                    }
                    else {
                        if (player->position.x > self->position.x)
                            speed = abs(speed);
                        else
                            shouldFling = false;
                    }

                    if (shouldFling) {
                        player->velocity.x = speed;
                        player->onGround   = false;
                        player->velocity.y = -0x20000;
                        player->state      = Player_State_Air;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_AIR_WALK, &player->animator, false, 0);
                        RSDK.PlaySfx(BarStool->sfxHop, false, 0xFF);
                    }
                }

                if (player->jumpPress && player->state == Player_State_Static) {
                    int32 speed = self->spinSpeed;
                    if (player->position.x <= self->position.x)
                        speed = -abs(speed);
                    else
                        speed = -speed;

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

    self->spinAngle             = (self->spinAngle + (self->spinSpeed >> 2)) & 0x7FFFF;
    self->stoolAnimator.frameID = self->spinAngle >> 16;
}

void BarStool_LateUpdate(void) {}

void BarStool_StaticUpdate(void) {}

void BarStool_Draw(void)
{
    RSDK_THIS(BarStool);

    RSDK.DrawSprite(&self->poleAnimator, NULL, false);
    RSDK.DrawSprite(&self->stoolAnimator, NULL, false);
}

void BarStool_Create(void *data)
{
    RSDK_THIS(BarStool);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(BarStool->aniFrames, 0, &self->stoolAnimator, true, 0);
        RSDK.SetSpriteAnimation(BarStool->aniFrames, 1, &self->poleAnimator, true, self->height);

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
    }
}

void BarStool_StageLoad(void)
{
    BarStool->aniFrames = RSDK.LoadSpriteAnimation("MSZ/BarStool.bin", SCOPE_STAGE);

    BarStool->hitboxStool.left   = -16;
    BarStool->hitboxStool.top    = -24;
    BarStool->hitboxStool.right  = 16;
    BarStool->hitboxStool.bottom = -16;

    BarStool->sfxSpin = RSDK.GetSfx("MSZ/StoolSpin.wav");
    BarStool->sfxHop  = RSDK.GetSfx("MSZ/StoolHop.wav");
}

#if GAME_INCLUDE_EDITOR
void BarStool_EditorDraw(void)
{
    RSDK_THIS(BarStool);

    RSDK.SetSpriteAnimation(BarStool->aniFrames, 0, &self->stoolAnimator, true, 0);
    RSDK.SetSpriteAnimation(BarStool->aniFrames, 1, &self->poleAnimator, true, self->height);

    RSDK.DrawSprite(&self->poleAnimator, NULL, false);
    RSDK.DrawSprite(&self->stoolAnimator, NULL, false);
}

void BarStool_EditorLoad(void)
{
    BarStool->aniFrames = RSDK.LoadSpriteAnimation("MSZ/BarStool.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(BarStool, height);
    RSDK_ENUM_VAR("Short", BARSTOOL_HEIGHT_SHORT);
    RSDK_ENUM_VAR("Tall", BARSTOOL_HEIGHT_TALL);
}
#endif

void BarStool_Serialize(void) { RSDK_EDITABLE_VAR(BarStool, VAR_UINT8, height); }
