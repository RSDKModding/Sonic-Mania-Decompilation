#include "SonicMania.h"

ObjectTippingPlatform *TippingPlatform;

void TippingPlatform_Update(void)
{
    RSDK_THIS(TippingPlatform);

    if (self->bossID) {
        if (self->animator.frameID) {
            self->stateCollide = Platform_CollisionState_None;
            self->collision    = PLATFORM_C_4;
        }
        else {
            self->stateCollide = Platform_CollisionState_TopSolid;
            self->collision    = PLATFORM_C_0;
        }
        if (!self->stoodAngle && self->bossID > 2 && self->syringe->activated) {
            self->stoodAngle    = 1;
            self->collapseDelay = self->intervalOffset + 1;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
            self->state = TippingPlatform_Unknown5;
        }
    }
    else {
        if (self->state == Platform_State_Normal) {
            int32 timer = (Zone->timer + self->intervalOffset) % self->interval;
            if (timer >= self->duration) {
                if (timer >= self->duration + 22) {
                    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 6);
                }
                else {
                    self->active = ACTIVE_NORMAL;
                    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, (timer - self->duration) >> 2);
                    self->state = TippingPlatform_Unknown4;
                }
            }
            else {
                self->active        = ACTIVE_NORMAL;
                self->collapseDelay = self->duration - timer;
                if (self->duration - timer < self->duration - 24) {
                    RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 6);
                    self->state = TippingPlatform_Unknown3;
                }
                else {
                    RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, timer >> 2);
                    self->state                   = TippingPlatform_Unknown3;
                    self->animator.animationTimer = (self->duration - self->collapseDelay) & 3;
                }
            }
        }

        bool32 flag = false;
        if (self->animator.animationID == 1)
            flag = self->animator.frameID == 0;
        else
            flag = self->animator.frameID == 6;
        if (flag) {
            self->stateCollide = Platform_CollisionState_TopSolid;
            self->collision    = PLATFORM_C_0;
        }
        else {
            self->stateCollide = Platform_CollisionState_None;
            self->collision    = PLATFORM_C_4;
        }
    }
    Platform_Update();
}

void TippingPlatform_LateUpdate(void) {}

void TippingPlatform_StaticUpdate(void) {}

void TippingPlatform_Draw(void)
{
    RSDK_THIS(TippingPlatform);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void TippingPlatform_Create(void *data)
{
    RSDK_THIS(TippingPlatform);
    if (!self->interval)
        self->interval = -16;
    if (!self->duration)
        self->duration = 120;
    self->collision = PLATFORM_C_0;
    Platform_Create(NULL);

    if (self->bossID) {
        RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
        self->animator.animationSpeed = 0;
    }
    else {
        RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 6);
    }

    self->state = Platform_State_Normal;
    if (!SceneInfo->inEditor) {
        if (self->bossID > 1) {
            foreach_all(Syringe, syringe)
            {
                if (syringe->tag == self->bossID) {
                    self->syringe = syringe;
                    if (syringe) {
                        self->updateRange.x = abs(self->position.x - syringe->position.x) + 0x400000;
                        self->updateRange.y = abs(self->position.y - syringe->position.y) + 0x400000;
                    }
                    self->stateCollide = Platform_CollisionState_AllSolid;
                    self->collision    = PLATFORM_C_1;
                    foreach_break;
                }
            }
        }

        if (self->bossID == 1) {
            self->direction = FLIP_X;
            self->drawFX    = FX_FLIP;
        }
    }
}

void TippingPlatform_StageLoad(void) {}

void TippingPlatform_Unknown1(void)
{
    RSDK_THIS(TippingPlatform);
    if (self->animator.frameID < 6 && ++self->animator.animationTimer == 4) {
        self->animator.animationTimer = 0;
        self->animator.frameID++;
    }

    if (--self->collapseDelay <= 0) {
        self->active = ACTIVE_BOUNDS;
        if (self->bossID < 3)
            self->state = TippingPlatform_Unknown2;
        else
            self->state = Platform_State_Normal;
    }
}

void TippingPlatform_Unknown2(void)
{
    RSDK_THIS(TippingPlatform);
    if (self->animator.frameID <= 0) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Platform_State_Normal;
    }
    else if (++self->animator.animationTimer == 4) {
        self->animator.animationTimer = 0;
        self->animator.frameID--;
    }
}

void TippingPlatform_Unknown3(void)
{
    RSDK_THIS(TippingPlatform);
    if (--self->collapseDelay <= 0) {
        RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 0);
        self->state = TippingPlatform_Unknown2;
    }
}

void TippingPlatform_Unknown4(void)
{
    RSDK_THIS(TippingPlatform);
    if (self->animator.frameID == 6) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Platform_State_Normal;
    }
}

void TippingPlatform_Unknown5(void)
{
    RSDK_THIS(TippingPlatform);
    if (--self->collapseDelay <= 0)
        self->state = TippingPlatform_Unknown1;
}

void TippingPlatform_EditorDraw(void)
{
    RSDK_THIS(TippingPlatform);
    if (self->bossID) {
        RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
        self->animator.animationSpeed = 0;
    }
    else {
        RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 6);
    }

    TippingPlatform_Draw();
}

void TippingPlatform_EditorLoad(void) {}

void TippingPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, interval);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, intervalOffset);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, duration);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, bossID);
}
