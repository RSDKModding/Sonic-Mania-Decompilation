// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LavaGeyser Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->animator1, NULL, false);
    if (self->height > 0) {
        SpriteFrame *frame = RSDK.GetFrame(LavaGeyser->aniFrames, 0, self->animator2.frameID);
        frame->height      = 48;
        drawPos.y -= self->height;

        for (int i = (self->height >> 16) / 48; i > 0; --i) {
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
            drawPos.y += 0x300000;
        }

        if ((self->height >> 16) % 48) {
            frame->height = (self->height >> 16) % 48;
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
        }

        drawPos.y = self->position.y - self->height;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);
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
        self->drawOrder     = Zone->drawOrderHigh;
        self->force <<= 12;
        self->state = LavaGeyser_State_Unknown1;
    }
}

void LavaGeyser_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        LavaGeyser->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LavaGeyser.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2"))
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
            Player_CheckElementalHit(player, self, SHIELD_FIRE);
        }
    }
}

void LavaGeyser_HandleSetup(void)
{
    RSDK_THIS(LavaGeyser);
    self->velocity.y = self->force;
    self->visible    = true;
    self->active     = ACTIVE_NORMAL;
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 0, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &self->animator3, true, 16);
    self->state = LavaGeyser_State_Unknown2;
}

void LavaGeyser_HandleIntervals(void)
{
    RSDK_THIS(LavaGeyser);

    if (!((Zone->timer + self->intervalOffset) % self->interval))
        LavaGeyser_HandleSetup();
}

void LavaGeyser_State_Unknown1(void) { LavaGeyser_HandleSetup(); }

void LavaGeyser_State_Unknown2(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == 16) {
        --self->animator1.timer;
        RSDK.PlaySfx(LavaGeyser->sfxLavaGeyser, false, 255);
        self->state = LavaGeyser_State_Unknown3;
        LavaGeyser_State_Unknown3();
    }
}

void LavaGeyser_State_Unknown3(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    self->height += self->velocity.y;
    self->velocity.y -= 0x1800;
    LavaGeyser_CheckPlayerCollisions();

    if (self->velocity.y < 0) {
        if (self->duration) {
            self->field_A8 = self->height;
            self->angle    = 0;
            self->timer    = self->duration;
            self->state    = LavaGeyser_State_Unknown4;
        }
        else {
            self->state = LavaGeyser_State_Unknown5;
        }
    }
}

void LavaGeyser_State_Unknown4(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    self->angle += 2;
    self->height = 0x600 * RSDK.Cos256(self->angle) + self->field_A8;
    LavaGeyser_CheckPlayerCollisions();
    if (!--self->timer)
        self->state = LavaGeyser_State_Unknown5;
}

void LavaGeyser_State_Unknown5(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    self->height += self->velocity.y;
    self->velocity.y -= 0x1800;

    LavaGeyser_CheckPlayerCollisions();
    if (self->height < 0) {
        self->height     = 0;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 2, &self->animator1, true, 0);
        self->state = LavaGeyser_State_Unknown6;
    }
}

void LavaGeyser_State_Unknown6(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        if (self->interval) {
            self->visible = false;
            if (!self->type)
                self->state = LavaGeyser_HandleIntervals;
            else
                self->state = LavaGeyser_State_Unknown1;
            self->active = ACTIVE_BOUNDS;
        }
        else {
            destroyEntity(self);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void LavaGeyser_EditorDraw(void)
{
    RSDK_THIS(LavaGeyser);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 0, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &self->animator3, true, 16);

    self->velocity.y = self->force << 12;
    self->height     = 0;

    while (self->velocity.y > 0) {
        self->height += self->velocity.y;
        self->velocity.y -= 0x1800;
    }

    LavaGeyser_Draw();
}

void LavaGeyser_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        LavaGeyser->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LavaGeyser.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2"))
        LavaGeyser->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LavaGeyser.bin", SCOPE_STAGE);
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
