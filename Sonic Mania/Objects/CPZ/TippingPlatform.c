#include "SonicMania.h"

ObjectTippingPlatform *TippingPlatform;

void TippingPlatform_Update(void)
{
    RSDK_THIS(TippingPlatform);

    if (self->bossID) {
        if (self->animator.frameID) {
            self->stateCollide = Platform_CollisionState_None;
            self->collision    = PLATFORM_C_SOLID_NONE;
        }
        else {
            self->stateCollide = Platform_CollisionState_TopSolid;
            self->collision    = PLATFORM_C_SOLID_TOP;
        }
        if (!self->stoodAngle && self->bossID > TIPPINGPLATFORM_EGGMAN && self->syringe->activated) {
            self->stoodAngle = 1;
            self->timer      = self->intervalOffset + 1;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
            self->state = TippingPlatform_State_Tipping_Delay;
        }
    }
    else {
        if (self->state == Platform_State_Fixed) {
            int32 timer = (Zone->timer + self->intervalOffset) % self->interval;
            if (timer >= self->duration) {
                if (timer >= self->duration + 22) {
                    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 6);
                }
                else {
                    self->active = ACTIVE_NORMAL;
                    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, (timer - self->duration) >> 2);
                    self->state = TippingPlatform_State_Restore;
                }
            }
            else {
                self->active = ACTIVE_NORMAL;
                self->timer  = self->duration - timer;
                if (self->duration - timer < self->duration - 24) {
                    RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 6);
                    self->state = TippingPlatform_State_Tipping;
                }
                else {
                    RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, timer >> 2);
                    self->state          = TippingPlatform_State_Tipping;
                    self->animator.timer = (self->duration - self->timer) & 3;
                }
            }
        }

        bool32 isSolid = false;
        if (self->animator.animationID == 1)
            isSolid = self->animator.frameID == 0;
        else
            isSolid = self->animator.frameID == 6;
        if (isSolid) {
            self->stateCollide = Platform_CollisionState_TopSolid;
            self->collision    = PLATFORM_C_SOLID_TOP;
        }
        else {
            self->stateCollide = Platform_CollisionState_None;
            self->collision    = PLATFORM_C_SOLID_NONE;
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
    self->collision = PLATFORM_C_SOLID_TOP;
    Platform_Create(NULL);

    if (self->bossID) {
        RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
        self->animator.speed = 0;
    }
    else {
        RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 6);
    }

    self->state = Platform_State_Fixed;
    if (!SceneInfo->inEditor) {
        if (self->bossID > TIPPINGPLATFORM_PLAYER) {
            foreach_all(Syringe, syringe)
            {
                if (syringe->tag == self->bossID) {
                    self->syringe = syringe;
                    if (syringe) {
                        self->updateRange.x = abs(self->position.x - syringe->position.x) + 0x400000;
                        self->updateRange.y = abs(self->position.y - syringe->position.y) + 0x400000;
                    }
                    self->stateCollide = Platform_CollisionState_AllSolid;
                    self->collision    = PLATFORM_C_SOLID_ALL;
                    foreach_break;
                }
            }
        }

        if (self->bossID == TIPPINGPLATFORM_PLAYER) {
            self->direction = FLIP_X;
            self->drawFX    = FX_FLIP;
        }
    }
}

void TippingPlatform_StageLoad(void) {}

void TippingPlatform_State_Tipping_Boss(void)
{
    RSDK_THIS(TippingPlatform);
    if (self->animator.frameID < 6 && ++self->animator.timer == 4) {
        self->animator.timer = 0;
        self->animator.frameID++;
    }

    if (--self->timer <= 0) {
        self->active = ACTIVE_BOUNDS;
        if (self->bossID <= TIPPINGPLATFORM_EGGMAN)
            self->state = TippingPlatform_State_RestorePlatform;
        else
            self->state = Platform_State_Fixed;
    }
}

void TippingPlatform_State_RestorePlatform(void)
{
    RSDK_THIS(TippingPlatform);
    if (self->animator.frameID <= 0) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Platform_State_Fixed;
    }
    else if (++self->animator.timer == 4) {
        self->animator.timer = 0;
        self->animator.frameID--;
    }
}

void TippingPlatform_State_Tipping(void)
{
    RSDK_THIS(TippingPlatform);
    if (--self->timer <= 0) {
        RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 0);
        self->state = TippingPlatform_State_RestorePlatform;
    }
}

void TippingPlatform_State_Restore(void)
{
    RSDK_THIS(TippingPlatform);
    if (self->animator.frameID == 6) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Platform_State_Fixed;
    }
}

void TippingPlatform_State_Tipping_Delay(void)
{
    RSDK_THIS(TippingPlatform);
    if (--self->timer <= 0)
        self->state = TippingPlatform_State_Tipping_Boss;
}

#if RETRO_INCLUDE_EDITOR
void TippingPlatform_EditorDraw(void)
{
    RSDK_THIS(TippingPlatform);
    if (self->bossID) {
        RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
        self->animator.speed = 0;
    }
    else {
        RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 6);
    }

    TippingPlatform_Draw();
}

void TippingPlatform_EditorLoad(void) {}
#endif

void TippingPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, interval);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, intervalOffset);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, duration);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, bossID);
}
