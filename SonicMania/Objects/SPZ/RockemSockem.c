// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RockemSockem Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectRockemSockem *RockemSockem;

void RockemSockem_Update(void)
{
    RSDK_THIS(RockemSockem);

    RSDK.ProcessAnimation(&self->ballAnimator);

    if (self->bounceTimer > 0) {
        if (!--self->bounceTimer)
            RSDK.SetSpriteAnimation(RockemSockem->aniFrames, 2, &self->ballAnimator, true, self->ballAnimator.frameID);
    }

    if (self->reboundTimer > 0) {
        if (!--self->reboundTimer) {
            self->position.x = self->startPos.x;
            self->position.y = self->startPos.y;
            self->velocity.x = 0;
            self->velocity.y = 0;
            self->active     = ACTIVE_BOUNDS;
        }
    }

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.x = self->velocity.x + ((self->startPos.x - self->position.x) >> 6) - (self->velocity.x >> 4);
    self->velocity.y = self->velocity.y + ((self->startPos.y - self->position.y) >> 6) - (self->velocity.y >> 4);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &RockemSockem->hitbox)) {
            int32 vel = abs(player->velocity.x) <= abs(player->velocity.y) ? abs(player->velocity.y) : abs(player->velocity.x);

            int32 angle        = RSDK.ATan2((player->position.x - self->position.x) / 3, player->position.y - self->position.y);
            int32 distX        = abs(self->position.x - self->startPos.x);
            int32 distY        = abs(self->position.y - self->startPos.y);
            self->reboundTimer = 120;

            int32 dx = (distX >> 16) * (distX >> 16);
            int32 dy = (distY >> 16) * (distY >> 16);

            if (dx + dy < 0x100 && !self->bounceTimer) {
                if ((angle - 0x20) & 0xC0) {
                    self->velocity.x -= vel * RSDK.Cos256(angle) >> 8;
                    self->velocity.y -= vel * RSDK.Sin256(angle) >> 8;
                }
                else {
                    self->velocity.x -= vel * RSDK.Cos256(angle) >> 9;
                    self->velocity.y -= 0x2000;
                }

                self->velocity.x = CLAMP(self->velocity.x, -0x40000, 0x40000);
                player->velocity.x -= player->velocity.x >> 4;
                if (player->velocity.y <= 0)
                    player->velocity.y -= player->velocity.y >> 5;
                else
                    player->velocity.y -= player->velocity.y >> 4;

                self->active = ACTIVE_NORMAL;
            }
            else if (player->velocity.y < 0) {
                self->active = ACTIVE_NORMAL;
            }
            else {
                int32 angleVal = ((angle - 0x20) >> 6) & 3;

                int32 anim = player->animator.animationID;
                switch (angleVal) {
                    default:
                    case 0:
                        player->velocity.x = ((RSDK.Cos256(angle) << 9) + player->velocity.x) >> 1;
                        player->velocity.y = -(abs(RSDK.Sin256(angle)) * 0x400);

                        if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                            player->animator.animationID = ANI_WALK;
                        break;

                    case 1:
                        player->velocity.x = (0x500 * RSDK.Cos256(angle) + player->velocity.x) >> 1;
                        player->velocity.y = -(abs(RSDK.Sin256(angle)) * 0x500);

                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_CS, &player->animator, false, 0);
                        break;

                    case 2:
                        player->velocity.x = (0x700 * RSDK.Cos256(angle) + player->velocity.x) >> 1;
                        player->velocity.y = -(abs(RSDK.Sin256(angle)) * 0x700);

                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_CS, &player->animator, false, 0);
                        break;

                    case 3: break;
                }

                if (self->ballAnimator.animationID != 3)
                    RSDK.PlaySfx(RockemSockem->sfxRockemSockem, false, 0xFF);

                // Bug Details:
                // this one's actually the opposite of normal, since its fixed in release builds
                // but if you comment out this Player_State_Air line and glide into the rockem sockem as knux
                // you'll get the funny anim bug they showed off at SDCC 2017
                player->state    = Player_State_Air;
                player->onGround = false;
                RSDK.SetSpriteAnimation(RockemSockem->aniFrames, 3, &self->ballAnimator, true, self->ballAnimator.frameID);
                self->bounceTimer = 16;
                self->active      = ACTIVE_NORMAL;
            }
        }
    }

    if (self->position.y > self->drawPos.y - 0xE0000) {
        self->position.y  = self->drawPos.y - 0xE0000;
        self->bounceTimer = 16;
    }

    self->angleVel = 0;
    if (abs(self->startPos.x - self->position.x) > 0x10000)
        self->angleVel = (self->startPos.x - self->position.x) >> 17;

    RockemSockem_HandleJointPositions();
}

void RockemSockem_LateUpdate(void) {}

void RockemSockem_StaticUpdate(void) {}

void RockemSockem_Draw(void)
{
    RSDK_THIS(RockemSockem);

    for (int32 i = 0; i < ROCKEMSOCKEM_COIL_COUNT; ++i) {
        self->rotation = self->jointRotations[i];
        RSDK.DrawSprite(&self->jointAnimator, &self->jointPositions[i], false);
    }

    RSDK.DrawSprite(&self->baseAnimator, &self->drawPos, false);

    if (self->ballAnimator.frameID > 9)
        self->direction = FLIP_X;
    RSDK.DrawSprite(&self->ballAnimator, NULL, false);

    self->direction = FLIP_NONE;
}

void RockemSockem_Create(void *data)
{
    RSDK_THIS(RockemSockem);

    RSDK.SetSpriteAnimation(RockemSockem->aniFrames, 0, &self->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(RockemSockem->aniFrames, 1, &self->jointAnimator, true, 0);
    RSDK.SetSpriteAnimation(RockemSockem->aniFrames, 2, &self->ballAnimator, true, 0);

    if (!SceneInfo->inEditor) {
        self->drawPos = self->position;
        self->position.y -= (0xE0000 + (0x50000 * ROCKEMSOCKEM_COIL_COUNT));
        self->startPos      = self->position;
        self->angle         = 0x80;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->drawFX        = FX_ROTATE | FX_FLIP;
    }
}

void RockemSockem_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        RockemSockem->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/RockemSockem.bin", SCOPE_STAGE);
    else
        RockemSockem->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/RockemSockem.bin", SCOPE_STAGE);

    RockemSockem->hitbox.left   = -16;
    RockemSockem->hitbox.top    = -14;
    RockemSockem->hitbox.right  = 16;
    RockemSockem->hitbox.bottom = 14;

    RockemSockem->sfxRockemSockem = RSDK.GetSfx("Stage/RockemSockem.wav");
}

void RockemSockem_HandleJointPositions(void)
{
    RSDK_THIS(RockemSockem);

    int32 x = (RSDK.Sin256(self->angle) << 11) + self->drawPos.x;
    int32 y = (RSDK.Cos256(self->angle) << 11) + self->drawPos.y;

    int32 ang  = RSDK.ATan2(y - self->position.y, (RSDK.Sin256(self->angle) << 11) + self->drawPos.x - self->position.x);
    int32 posX = (0xC00 * RSDK.Sin256(ang) + self->position.x) & 0xFFFF0000;
    int32 posY = (0xC00 * RSDK.Cos256(ang) + self->position.y) & 0xFFFF0000;

    int32 angle = self->angle;
    x >>= 16;
    y >>= 16;
    posX >>= 16;
    posY >>= 16;

    self->jointRotations[0]   = 2 * (640 - angle);
    self->jointPositions[0].x = 0x10002 * x;
    self->jointPositions[0].y = 0x10002 * y;

    angle += self->angleVel;
    self->jointRotations[1]   = 2 * (640 - angle);
    self->jointPositions[1].x = 0x2AAB * (x + posX + 4 * x);
    self->jointPositions[1].y = 0x2AAB * (y + posY + 4 * y);

    angle += self->angleVel;
    self->jointRotations[2]   = 2 * (640 - angle);
    self->jointPositions[2].x = 0x5556 * (posX + 2 * x);
    self->jointPositions[2].y = 0x5556 * (posY + 2 * y);

    angle += self->angleVel;
    self->jointRotations[3]   = 2 * (640 - angle);
    self->jointPositions[3].x = 0x8001 * (x + posX);
    self->jointPositions[3].y = 0x8001 * (y + posY);

    angle += self->angleVel;
    self->jointRotations[4]   = 2 * (640 - angle);
    self->jointPositions[4].x = 0x5556 * (x + 2 * posX);
    self->jointPositions[4].y = 0x5556 * (y + 2 * posY);

    angle -= self->angleVel;
    self->jointRotations[5]   = 2 * (640 - angle);
    self->jointPositions[5].x = 0x2AAB * (posX + x + 4 * posX);
    self->jointPositions[5].y = 0x2AAB * (posY + y + 4 * posY);
}

#if GAME_INCLUDE_EDITOR
void RockemSockem_EditorDraw(void)
{
    RSDK_THIS(RockemSockem);

    self->drawPos = self->position;
    self->position.y -= (0xE0000 + (0x50000 * ROCKEMSOCKEM_COIL_COUNT));
    self->startPos      = self->position;
    self->angle         = 0x80;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->drawFX        = FX_ROTATE | FX_FLIP;

    self->angleVel = 0;
    if (abs(self->startPos.x - self->position.x) > 0x10000)
        self->angleVel = (self->startPos.x - self->position.x) >> 17;

    RockemSockem_HandleJointPositions();

    RockemSockem_Draw();
}

void RockemSockem_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        RockemSockem->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/RockemSockem.bin", SCOPE_STAGE);
    else
        RockemSockem->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/RockemSockem.bin", SCOPE_STAGE);
}
#endif

void RockemSockem_Serialize(void) {}