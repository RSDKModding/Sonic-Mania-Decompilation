// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LavaGeyser Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLavaGeyser *LavaGeyser;

void LavaGeyser_Update(void)
{
    RSDK_THIS(LavaGeyser);

    StateMachine_Run(self->state);
}

void LavaGeyser_LateUpdate(void) {}

void LavaGeyser_StaticUpdate(void) {}

void LavaGeyser_Draw(void)
{
    RSDK_THIS(LavaGeyser);

    Vector2 drawPos = self->position;
    RSDK.DrawSprite(&self->plumeAnimator, NULL, false);

    if (self->height > 0) {
        SpriteFrame *frame = RSDK.GetFrame(LavaGeyser->aniFrames, 0, self->flowAnimator.frameID);
        frame->height      = 48;
        drawPos.y -= self->height;

        for (int32 i = (self->height >> 16) / 48; i > 0; --i) {
            RSDK.DrawSprite(&self->flowAnimator, &drawPos, false);
            drawPos.y += 0x300000;
        }

        if ((self->height >> 16) % 48) {
            frame->height = (self->height >> 16) % 48;
            RSDK.DrawSprite(&self->flowAnimator, &drawPos, false);
        }

        drawPos.y = self->position.y - self->height;
        RSDK.DrawSprite(&self->plumeLoopAnimator, &drawPos, false);
    }
}

void LavaGeyser_Create(void *data)
{
    RSDK_THIS(LavaGeyser);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->force <<= 12;
        self->state = LavaGeyser_State_Init;
    }
}

void LavaGeyser_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        LavaGeyser->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LavaGeyser.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2"))
        LavaGeyser->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LavaGeyser.bin", SCOPE_STAGE);

    LavaGeyser->sfxHit        = RSDK.GetSfx("Stage/BossHit.wav");
    LavaGeyser->sfxExplosion  = RSDK.GetSfx("Stage/Explosion2.wav");
    LavaGeyser->sfxDrill      = RSDK.GetSfx("LRZ/Drill.wav");
    LavaGeyser->sfxLavaGeyser = RSDK.GetSfx("LRZ/LavaGeyser.wav");
}

void LavaGeyser_CheckPlayerCollisions(void)
{
    RSDK_THIS(LavaGeyser);

    Hitbox hitbox;
    hitbox.left   = -24;
    hitbox.top    = -(self->height >> 16);
    hitbox.right  = 24;
    hitbox.bottom = 0;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &hitbox)) {
            Player_ElementHurt(player, self, SHIELD_FIRE);
        }
    }
}

void LavaGeyser_HandleSetup(void)
{
    RSDK_THIS(LavaGeyser);

    self->velocity.y = self->force;
    self->visible    = true;
    self->active     = ACTIVE_NORMAL;

    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &self->plumeAnimator, true, 0);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 0, &self->flowAnimator, true, 0);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &self->plumeLoopAnimator, true, 16);

    self->state = LavaGeyser_State_ShowPlume;
}

void LavaGeyser_State_Intervals(void)
{
    RSDK_THIS(LavaGeyser);

    if (!((Zone->timer + self->intervalOffset) % self->interval))
        LavaGeyser_HandleSetup();
}

void LavaGeyser_State_Init(void) { LavaGeyser_HandleSetup(); }

void LavaGeyser_State_ShowPlume(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&self->plumeAnimator);

    if (self->plumeAnimator.frameID == 16) {
        --self->plumeAnimator.timer;
        RSDK.PlaySfx(LavaGeyser->sfxLavaGeyser, false, 0xFF);
        self->state = LavaGeyser_State_Erupting;
        LavaGeyser_State_Erupting();
    }
}

void LavaGeyser_State_Erupting(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&self->plumeAnimator);
    RSDK.ProcessAnimation(&self->flowAnimator);
    RSDK.ProcessAnimation(&self->plumeLoopAnimator);

    self->height += self->velocity.y;
    self->velocity.y -= 0x1800;

    LavaGeyser_CheckPlayerCollisions();

    if (self->velocity.y < 0) {
        if (self->duration) {
            self->startingHeight = self->height;
            self->angle          = 0;
            self->timer          = self->duration;
            self->state          = LavaGeyser_State_Erupted;
        }
        else {
            self->state = LavaGeyser_State_Recede;
        }
    }
}

void LavaGeyser_State_Erupted(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&self->plumeAnimator);
    RSDK.ProcessAnimation(&self->flowAnimator);
    RSDK.ProcessAnimation(&self->plumeLoopAnimator);

    self->angle += 2;
    self->height = 0x600 * RSDK.Cos256(self->angle) + self->startingHeight;

    LavaGeyser_CheckPlayerCollisions();

    if (!--self->timer)
        self->state = LavaGeyser_State_Recede;
}

void LavaGeyser_State_Recede(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&self->plumeAnimator);
    RSDK.ProcessAnimation(&self->flowAnimator);
    RSDK.ProcessAnimation(&self->plumeLoopAnimator);

    self->height += self->velocity.y;
    self->velocity.y -= 0x1800;

    LavaGeyser_CheckPlayerCollisions();

    if (self->height < 0) {
        self->height     = 0;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 2, &self->plumeAnimator, true, 0);
        self->state = LavaGeyser_State_HandleFinish;
    }
}

void LavaGeyser_State_HandleFinish(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&self->plumeAnimator);

    if (self->plumeAnimator.frameID == self->plumeAnimator.frameCount - 1) {
        if (self->interval) {
            self->visible = false;

            if (!self->type)
                self->state = LavaGeyser_State_Intervals;
            else
                self->state = LavaGeyser_State_Init;

            self->active = ACTIVE_BOUNDS;
        }
        else {
            destroyEntity(self);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void LavaGeyser_EditorDraw(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &self->plumeAnimator, true, 16);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 0, &self->flowAnimator, true, 0);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &self->plumeLoopAnimator, true, 16);

    if (showGizmos()) {
        self->velocity.y = self->force << 12;
        self->height     = 0;

        while (self->velocity.y > 0) {
            self->height += self->velocity.y;
            self->velocity.y -= 0x1800;
        }

        LavaGeyser_Draw();
    }
    else {
        self->height = 0;
        LavaGeyser_Draw();
    }
}

void LavaGeyser_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        LavaGeyser->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LavaGeyser.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2"))
        LavaGeyser->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LavaGeyser.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(LavaGeyser, type);
    RSDK_ENUM_VAR("Use Intervals", LAVAGEYSER_INTERVAL);
    RSDK_ENUM_VAR("No Intervals", LAVAGEYSER_REPEAT);
}
#endif

void LavaGeyser_Serialize(void)
{
    RSDK_EDITABLE_VAR(LavaGeyser, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LavaGeyser, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(LavaGeyser, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(LavaGeyser, VAR_UINT16, duration);
    RSDK_EDITABLE_VAR(LavaGeyser, VAR_ENUM, force);
}
