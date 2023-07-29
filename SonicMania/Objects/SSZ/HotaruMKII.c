// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HotaruMKII Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHotaruMKII *HotaruMKII;

void HotaruMKII_Update(void)
{
    RSDK_THIS(HotaruMKII);
    StateMachine_Run(self->state);
}

void HotaruMKII_LateUpdate(void) {}

void HotaruMKII_StaticUpdate(void) {}

void HotaruMKII_Draw(void)
{
    RSDK_THIS(HotaruMKII);

    if (self->state != HotaruMKII_State_CheckPlayerInRange) {
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        if (self->state != HotaruMKII_State_FlyOnScreen && ((Zone->timer & 1) || self->state == HotaruMKII_State_LaserAttack)) {
            int32 ink       = self->inkEffect;
            self->inkEffect = INK_ALPHA;
            RSDK.DrawSprite(&self->flashAnimator, NULL, false);

            self->inkEffect = ink;
        }
    }
}

void HotaruMKII_Create(void *data)
{
    RSDK_THIS(HotaruMKII);

    if (!SceneInfo->inEditor) {
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->startPos      = self->position;
        self->startDir      = self->direction;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        self->drawFX        = FX_FLIP;

        if (!self->triggerSize.x) {
            self->triggerSize.x = 0x1000000;
            self->triggerSize.y = 0x1000000;
        }

        self->hitboxTrigger.right  = self->triggerSize.x >> 17;
        self->hitboxTrigger.left   = -self->hitboxTrigger.right;
        self->hitboxTrigger.bottom = self->triggerSize.y >> 17;
        self->hitboxTrigger.top    = -self->hitboxTrigger.bottom;

        self->type = VOID_TO_INT(data);
        switch (self->type) {
            default: break;
            case HOTARUMKII_MAIN:
                self->active    = ACTIVE_BOUNDS;
                self->inkEffect = INK_NONE;
                self->visible   = true;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &self->flashAnimator, true, 0);
                self->state = HotaruMKII_State_Init;
                break;

            case HOTARUMKII_FLASH:
                self->active    = ACTIVE_NORMAL;
                self->inkEffect = INK_ALPHA;
                self->visible   = true;
                self->alpha     = 0x80;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &self->mainAnimator, true, 0);
                self->state = HotaruMKII_State_Flash;
                break;

            case HOTARUMKII_LASER:
                --self->drawGroup;
                self->active    = ACTIVE_NORMAL;
                self->inkEffect = INK_ALPHA;
                self->visible   = true;
                self->alpha     = 0x80;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 2, &self->mainAnimator, true, 0);
                self->state = HotaruMKII_State_Laser;
                break;
        }
    }
}

void HotaruMKII_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/HotaruMKII.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/HotaruMKII.bin", SCOPE_STAGE);

    HotaruMKII->hitboxBadnik.top    = -6;
    HotaruMKII->hitboxBadnik.left   = -6;
    HotaruMKII->hitboxBadnik.right  = 6;
    HotaruMKII->hitboxBadnik.bottom = 6;

    HotaruMKII->hitboxLaser.top    = -8;
    HotaruMKII->hitboxLaser.left   = -8;
    HotaruMKII->hitboxLaser.right  = 8;
    HotaruMKII->hitboxLaser.bottom = 8;

    HotaruMKII->sfxLaser  = RSDK.GetSfx("SSZ1/HotaruLaser.wav");
    HotaruMKII->sfxAppear = RSDK.GetSfx("SSZ1/HotaruAppear.wav");
    HotaruMKII->sfxFly    = RSDK.GetSfx("SSZ1/HotaruFly.wav");
    HotaruMKII->sfxCharge = RSDK.GetSfx("SSZ1/HotaruCharge.wav");

    DEBUGMODE_ADD_OBJ(HotaruMKII);
}

void HotaruMKII_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityHotaruMKII *hotaruMKII = CREATE_ENTITY(HotaruMKII, NULL, self->position.x, self->position.y);
    hotaruMKII->origin           = RSDK.Rand(0, 0x100);

    hotaruMKII->offset1.x = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0x100);
    hotaruMKII->offset1.y = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset2.x = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset2.y = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset3.x = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset3.y = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
}

void HotaruMKII_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void HotaruMKII_CheckPlayerCollisions(void)
{
    RSDK_THIS(HotaruMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &HotaruMKII->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void HotaruMKII_CheckOffScreen(void)
{
    RSDK_THIS(HotaruMKII);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        self->offsetID  = 0;
        HotaruMKII_Create(NULL);
    }
}

void HotaruMKII_HandleDistances(EntityPlayer *player)
{
    RSDK_THIS(HotaruMKII);

    if (Player_CheckValidState(player)) {
        int32 distX = self->curOffset.x;
        switch (self->offsetID) {
            case 0:
                self->curOffset.x = self->offset1.x;
                self->curOffset.y = self->offset1.y;
                break;

            case 1:
                self->curOffset.x = self->offset2.x;
                self->curOffset.y = self->offset2.y;
                break;

            case 2:
                self->curOffset.x = self->offset3.x;
                self->curOffset.y = self->offset3.y;
                break;

            case 3:
                self->curOffset.x = 0;
                self->curOffset.y = 0;
                break;

            default: break;
        }

        self->curOffset.x &= 0xFFFF0000;
        self->curOffset.y &= 0xFFFF0000;
        if (!self->curOffset.x && !self->curOffset.y) {
            self->curOffset.x = distX;
            self->curOffset.y = 0xB0010000;
        }

        int32 angle =
            RSDK.ATan2(self->curOffset.x + player->position.x - self->position.x, self->curOffset.y + player->position.y - self->position.y);
        self->moveAcceleration.x = 0x300 * RSDK.Cos256(angle);
        self->moveAcceleration.y = 0x300 * RSDK.Sin256(angle);

        self->velocity.x = player->velocity.x + self->moveAcceleration.x;
        self->velocity.y = player->velocity.y + self->moveAcceleration.y;

        ++self->offsetID;
    }
    else {
        self->playerPtr  = NULL;
        self->offsetID   = 4;
        self->velocity.x = 0;
        self->velocity.y = -0x30000;
        self->state      = HotaruMKII_State_FlyAway;
    }
}

void HotaruMKII_State_Init(void)
{
    RSDK_THIS(HotaruMKII);

    self->active     = ACTIVE_NORMAL;
    self->childCount = 0;

    self->state = HotaruMKII_State_CheckPlayerInRange;
    HotaruMKII_State_CheckPlayerInRange();
}

void HotaruMKII_State_CheckPlayerInRange(void)
{
    RSDK_THIS(HotaruMKII);

    bool32 foundTargetPlayer = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitboxTrigger)) {
            self->playerPtr = player;
            if (!player->sidekick) {
                foundTargetPlayer = true;

                int32 screenID = 0;
                if (player->camera)
                    screenID = player->camera->screenID;

                self->position.x       = RSDK.Cos256(self->origin) << 17;
                self->position.y       = RSDK.Sin256(self->origin) << 17;
                RSDKScreenInfo *screen = &ScreenInfo[screenID];

                if (self->position.x > (int32)(screen->size.x & 0xFFFFFFFE) << 15)
                    self->position.x = (int32)(screen->size.x & 0xFFFFFFFE) << 15;

                if (self->position.x < -((int32)(screen->size.x & 0xFFFFFFFE) << 15))
                    self->position.x = -((int32)(screen->size.x & 0xFFFFFFFE) << 15);

                if (self->position.y > (int32)(screen->size.y & 0xFFFFFFFE) << 15)
                    self->position.y = (int32)(screen->size.y & 0xFFFFFFFE) << 15;

                if (self->position.y < -((int32)(screen->size.y & 0xFFFFFFFE) << 15))
                    self->position.y = -((int32)(screen->size.y & 0xFFFFFFFE) << 15);

                if (self->position.x >= 0)
                    self->position.x += 0x100000;
                else
                    self->position.x -= 0x100000;

                if (self->position.y >= 0)
                    self->position.y += 0x100000;
                else
                    self->position.y -= 0x100000;

                self->position.x += ((screen->size.x & 0xFFFFFFFE) + 2 * screen->position.x) << 15;
                self->position.y += ((screen->size.y & 0xFFFFFFFE) + 2 * screen->position.y) << 15;
                HotaruMKII_HandleDistances(player);

                self->inkEffect = INK_ALPHA;
                self->visible   = true;
                self->alpha     = 0x80;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &self->mainAnimator, true, 0);

                RSDK.PlaySfx(HotaruMKII->sfxFly, false, 255);
                self->state = HotaruMKII_State_FlyOnScreen;

                foreach_break;
            }
        }
    }

    if (!foundTargetPlayer)
        HotaruMKII_CheckOffScreen();
}

void HotaruMKII_State_FlyAway(void)
{
    RSDK_THIS(HotaruMKII);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    HotaruMKII_CheckOffScreen();
}

void HotaruMKII_State_FlyOnScreen(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = self->playerPtr;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (player && !Player_CheckValidState(player)) {
        HotaruMKII_HandleDistances(player);
    }
    else {
        if (!(Zone->timer & 7)) {
            EntityHotaruMKII *flash = CREATE_ENTITY(HotaruMKII, INT_TO_VOID(HOTARUMKII_FLASH), self->position.x, self->position.y);
            flash->playerPtr        = self->playerPtr;
            flash->curOffset.x      = self->position.x - player->position.x;
            flash->curOffset.y      = self->position.y - player->position.y;
        }

        int32 moveFinished = 0;
        if (self->moveAcceleration.x < 0) {
            int32 x = player->position.x + self->curOffset.x;
            if (self->position.x <= x) {
                moveFinished     = 1;
                self->position.x = x;
            }
        } else if (self->moveAcceleration.x >= 1) {
            int32 x = player->position.x + self->curOffset.x;
            if (self->position.x >= x) {
                moveFinished     = 1;
                self->position.x = x;
            }
        } else {
            moveFinished = 1;
        }

        if (self->moveAcceleration.y < 0) {
            int32 y = player->position.y + self->curOffset.y;
            if (self->position.y <= y) {
                moveFinished++;
                self->position.y = y;
            }
        } else if (self->moveAcceleration.y > 0) {
            int32 y = player->position.y + self->curOffset.y;
            if (self->position.y >= y) {
                moveFinished++;
                self->position.y = y;
            }
        } else {
            moveFinished++;
        }

        if (moveFinished == 2) {
            self->inkEffect = INK_NONE;
            self->alpha     = 0x00;
            RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 0, &self->mainAnimator, true, 0);

            self->timer = 60;
            RSDK.PlaySfx(HotaruMKII->sfxAppear, false, 255);
            self->state = HotaruMKII_State_AttackDelay;
        }
        else {
            RSDK.ProcessAnimation(&self->mainAnimator);

            int32 angle =
                RSDK.ATan2(player->position.x + self->curOffset.x - self->position.x, player->position.y + self->curOffset.y - self->position.y);
            self->moveAcceleration.x = 0x300 * RSDK.Cos256(angle);
            self->moveAcceleration.y = 0x300 * RSDK.Sin256(angle);

            self->velocity.x = player->velocity.x + self->moveAcceleration.x;
            self->velocity.y = player->velocity.y + self->moveAcceleration.y;
        }

        HotaruMKII_CheckOffScreen();
    }
}

void HotaruMKII_State_AttackDelay(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = self->playerPtr;

    RSDK.ProcessAnimation(&self->unusedAnimator);

    if (Player_CheckValidState(player)) {
        self->position.x = player->position.x + self->curOffset.x;
        self->position.y = player->position.y + self->curOffset.y;
        if (--self->timer <= 0) {
            RSDK.PlaySfx(HotaruMKII->sfxCharge, false, 255);
            self->state = HotaruMKII_State_Charging;
        }
        HotaruMKII_CheckOffScreen();
    }
    else {
        HotaruMKII_HandleDistances(player);
    }
}

void HotaruMKII_State_Charging(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = self->playerPtr;

    RSDK.ProcessAnimation(&self->flashAnimator);

    if (Player_CheckValidState(player)) {
        self->position.x = player->position.x + self->curOffset.x;
        self->position.y = player->position.y + self->curOffset.y;

        self->alpha += 2;
        if (self->alpha == 0x100) {
            self->timer     = 90;
            self->alpha     = 0x100;
            self->inkEffect = INK_ALPHA;
            self->state     = HotaruMKII_State_LaserAttack;
        }
        HotaruMKII_CheckPlayerCollisions();
        HotaruMKII_CheckOffScreen();
    }
    else {
        HotaruMKII_HandleDistances(player);
    }
}

void HotaruMKII_State_LaserAttack(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = self->playerPtr;

    RSDK.ProcessAnimation(&self->mainAnimator);
    if (Player_CheckValidState(player)) {
        if (--self->timer > 0) {
            if (self->timer <= 60) {
                if (self->timer == 60) {
                    RSDK.PlaySfx(HotaruMKII->sfxLaser, false, 255);
                    RSDK.StopSfx(HotaruMKII->sfxCharge);
                }

                self->alpha -= 3;
                if (!(self->timer & 3)) {
                    EntityHotaruMKII *laser = CREATE_ENTITY(HotaruMKII, INT_TO_VOID(HOTARUMKII_LASER), self->position.x, self->position.y + 0xE0000);

                    if (!self->childCount) {
                        self->childCount            = 1;
                        laser->mainAnimator.frameID = 1;
                    }

                    if (self->childCount == 1)
                        laser->childCount = 1;

                    if (++self->childCount == 3)
                        self->childCount = 1;
                }
            }

            RSDK.ProcessAnimation(&self->flashAnimator);
            HotaruMKII_CheckPlayerCollisions();
            HotaruMKII_CheckOffScreen();
        }
        else {
            HotaruMKII_HandleDistances(player);
            self->childCount = 0;
            self->alpha      = 0x80;
            self->inkEffect  = INK_ALPHA;
            RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &self->mainAnimator, true, 0);
            self->state = HotaruMKII_State_FlyOnScreen;
        }
    }
    else {
        HotaruMKII_HandleDistances(player);
    }
}

void HotaruMKII_State_Flash(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = self->playerPtr;

    self->position.x = player->position.x + self->curOffset.x;
    self->position.y = player->position.y + self->curOffset.y;

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->alpha -= 4;
    if (self->alpha <= 0)
        destroyEntity(self);
}

void HotaruMKII_State_Laser(void)
{
    RSDK_THIS(HotaruMKII);

    self->position.y += 0x40000;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &HotaruMKII->hitboxLaser)) {
            Player_ElementHurt(player, self, SHIELD_LIGHTNING);
        }
    }

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
        if (self->childCount == 1) {
            ++self->drawGroup;
            self->position.y += 0x80000;
            self->inkEffect = INK_NONE;
            RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 3, &self->mainAnimator, true, 0);
            self->state = HotaruMKII_State_LaserStrike;
        }
        else {
            destroyEntity(self);
        }
    }
}

void HotaruMKII_State_LaserStrike(void)
{
    RSDK_THIS(HotaruMKII);

    RSDK.ProcessAnimation(&self->mainAnimator);
    if (self->mainAnimator.frameID >= self->mainAnimator.frameCount - 1)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void HotaruMKII_EditorDraw(void)
{
    RSDK_THIS(HotaruMKII);
    self->drawGroup     = Zone->objectDrawGroup[1];
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->inkEffect     = INK_NONE;
    self->visible       = true;
    RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 0, &self->mainAnimator, false, 0);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    if (showGizmos()) {
        Vector2 size = self->triggerSize;
        if (!self->triggerSize.x) {
            self->triggerSize.x = 0x1000000;
            self->triggerSize.y = 0x1000000;
        }

        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->triggerSize.x, self->triggerSize.y, 0xFF0000);

        RSDK_DRAWING_OVERLAY(false);

        self->triggerSize = size;
    }
}

void HotaruMKII_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/HotaruMKII.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/HotaruMKII.bin", SCOPE_STAGE);
}
#endif

void HotaruMKII_Serialize(void)
{
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_UINT8, origin);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, offset1);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, offset2);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, offset3);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, triggerSize);
}
