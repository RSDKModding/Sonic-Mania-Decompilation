// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SeltzerBottle Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSeltzerBottle *SeltzerBottle;

void SeltzerBottle_Update(void)
{
    RSDK_THIS(SeltzerBottle);

    RSDK.ProcessAnimation(&self->waterAnimator);

    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, self, &SeltzerBottle->hitboxBottle);

        if (Player_CheckCollisionPlatform(player, self, &SeltzerBottle->hitboxButton) && !self->buttonAnimator.frameID) {
            self->buttonAnimator.frameID = 1;
            self->active                 = ACTIVE_NORMAL;
            self->state                  = SeltzerBottle_State_Spraying;
            RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 6, &self->sprayAnimator, false, 0);
            player->groundVel = CLAMP(player->groundVel, -0xC0000, 0xC0000);

            for (int32 p = 0; p < Player->playerCount; ++p) RSDK_GET_ENTITY(p, Player)->collisionLayers |= SeltzerBottle->seltzerPathLayerMask;

            RSDK.PlaySfx(SeltzerBottle->sfxSpray, false, 0xFF);
        }
    }

    StateMachine_Run(self->state);
}

void SeltzerBottle_LateUpdate(void) {}

void SeltzerBottle_StaticUpdate(void) {}

void SeltzerBottle_Draw(void)
{
    RSDK_THIS(SeltzerBottle);

    RSDK.DrawRect(self->position.x - 0x2E0000, self->position.y - self->waterLevel + 0x2C0000, 0x5C0000, self->waterLevel, 0x00F0F0, 0x40, INK_SUB,
                  false);

    Vector2 drawPos = self->position;
    drawPos.y += 0x2C0000 - self->waterLevel;
    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->waterAnimator, &drawPos, false);

    self->bottleAnimator.frameID = 0;
    self->inkEffect              = INK_SUB;
    RSDK.DrawSprite(&self->bottleAnimator, NULL, false);

    self->bottleAnimator.frameID = 1;
    self->inkEffect              = INK_ADD;
    RSDK.DrawSprite(&self->bottleAnimator, NULL, false);

    self->bottleAnimator.frameID = 2;
    self->inkEffect              = INK_NONE;
    RSDK.DrawSprite(&self->bottleAnimator, NULL, false);

    self->bottleAnimator.frameID = 3;
    RSDK.DrawSprite(&self->bottleAnimator, NULL, false);

    self->bottleAnimator.frameID = 4;
    RSDK.DrawSprite(&self->bottleAnimator, NULL, false);

    self->bottleAnimator.frameID = 5;
    self->drawFX                 = FX_FLIP;
    RSDK.DrawSprite(&self->sprayAnimator, NULL, false);

    RSDK.DrawSprite(&self->bottleAnimator, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->buttonAnimator, NULL, false);
}

void SeltzerBottle_Create(void *data)
{
    RSDK_THIS(SeltzerBottle);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 0, &self->bottleAnimator, true, 0);
        RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 1, &self->buttonAnimator, true, 0);
        RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 2, &self->waterAnimator, true, 0);

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->alpha         = 0xFF;
        self->timer         = self->sprayTime;
        self->waterLevel    = 0x400000;
    }
}

void SeltzerBottle_StageLoad(void)
{
    SeltzerBottle->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Seltzer.bin", SCOPE_STAGE);

    SeltzerBottle->hitboxBottle.left   = -48;
    SeltzerBottle->hitboxBottle.top    = -80;
    SeltzerBottle->hitboxBottle.right  = 48;
    SeltzerBottle->hitboxBottle.bottom = 64;

    SeltzerBottle->hitboxButton.left   = -16;
    SeltzerBottle->hitboxButton.top    = -90;
    SeltzerBottle->hitboxButton.right  = 16;
    SeltzerBottle->hitboxButton.bottom = SeltzerBottle->hitboxButton.top + 16;

    SeltzerBottle->seltzerPathLayer = RSDK.GetTileLayerID("Seltzer Path");
    if (SeltzerBottle->seltzerPathLayer)
        SeltzerBottle->seltzerPathLayerMask = 1 << SeltzerBottle->seltzerPathLayer;

    SeltzerBottle->sfxSpray = RSDK.GetSfx("MSZ/Spray.wav");
}

void SeltzerBottle_State_Spraying(void)
{
    RSDK_THIS(SeltzerBottle);

    self->waterLevel = MAX(((self->timer << 14) / self->sprayTime) << 8, 0x40000);

    EntitySeltzerWater *spray = CREATE_ENTITY(SeltzerWater, NULL, self->position.x, self->position.y - 0x4C0000);
    spray->position.x += self->direction == FLIP_X ? 0x300000 : -0x300000;
    spray->oscillateRadius = RSDK.Rand(0xA00, 0xC00);
    spray->offsetAngle     = RSDK.Rand(0, 0x100);
    spray->drawGroup       = Zone->playerDrawGroup[0];
    spray->nodeSlot        = SceneInfo->entitySlot + 1;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    foreach_active(Player, player)
    {
        if (player->onGround) {
            self->position.x = player->position.x + (RSDK.Sin256(player->angle) << 13);
            self->position.y = player->position.y + (RSDK.Cos256(player->angle) << 13);

            if (RSDK.ObjectTileCollision(self, SeltzerBottle->seltzerPathLayerMask, player->collisionMode, 0, 0, 0, false)) {
                if (self->direction == FLIP_X) {
                    if (player->groundVel < 0x50000)
                        player->groundVel = 0x50000;
                }
                else {
                    if (player->groundVel > -0x50000)
                        player->groundVel = -0x50000;
                }
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;

    RSDK.ProcessAnimation(&self->sprayAnimator);

    if (--self->timer <= 0) {
        for (int32 p = 0; p < Player->playerCount; ++p) RSDK_GET_ENTITY(p, Player)->collisionLayers &= ~SeltzerBottle->seltzerPathLayerMask;

        RSDK.SetSpriteAnimation(-1, 0, &self->sprayAnimator, false, 0);
        self->state = SeltzerBottle_State_TryReset;
        foreach_active(SeltzerWater, water)
        {
            water->gravityStrength = RSDK.Rand(0x3800, 0x4000);
            water->state           = SeltzerWater_State_Falling;
        }
    }
}

void SeltzerBottle_State_TryReset(void)
{
    RSDK_THIS(SeltzerBottle);

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->state                  = StateMachine_None;
        self->timer                  = self->sprayTime;
        self->waterLevel             = 0x400000;
        self->buttonAnimator.frameID = 0;
        self->active                 = ACTIVE_BOUNDS;
    }
}

#if GAME_INCLUDE_EDITOR
void SeltzerBottle_EditorDraw(void)
{
    RSDK_THIS(SeltzerBottle);

    RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 0, &self->bottleAnimator, true, 0);
    RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 1, &self->buttonAnimator, true, 0);
    RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 2, &self->waterAnimator, true, 0);

    self->waterLevel = 0x400000;

    SeltzerBottle_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        self->inkEffect = INK_BLEND;

        int32 slotID   = SceneInfo->entitySlot;
        int32 nextSlot = SceneInfo->entitySlot + 1;

        if (nextSlot != -1) {
            EntityPlatformNode *lastNode = RSDK_GET_ENTITY(slotID, PlatformNode);
            while (lastNode) {
                EntityPlatformNode *nextNode = RSDK_GET_ENTITY(nextSlot, PlatformNode);
                if (!nextNode || nextNode->classID != PlatformNode->classID)
                    break;

                DrawHelpers_DrawArrow(lastNode->position.x, lastNode->position.y, nextNode->position.x, nextNode->position.y, 0xFFFF00, INK_NONE,
                                      0xFF);

                if (slotID >= nextSlot) {
                    nextSlot--;
                    slotID--;
                }
                else {
                    nextSlot++;
                    slotID++;
                }

                lastNode = nextNode;
            }
        }

        self->inkEffect = INK_NONE;

        RSDK_DRAWING_OVERLAY(false);
    }
}

void SeltzerBottle_EditorLoad(void)
{
    SeltzerBottle->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Seltzer.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SeltzerBottle, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void SeltzerBottle_Serialize(void)
{
    RSDK_EDITABLE_VAR(SeltzerBottle, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(SeltzerBottle, VAR_ENUM, sprayTime);
}
