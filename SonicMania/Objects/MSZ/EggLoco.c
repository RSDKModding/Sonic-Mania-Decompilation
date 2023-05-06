// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EggLoco Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectEggLoco *EggLoco;

void EggLoco_Update(void)
{
    RSDK_THIS(EggLoco);

    RSDK.ProcessAnimation(&self->eggmanAnimator);
    RSDK.ProcessAnimation(&self->lanternAnimator);
    RSDK.ProcessAnimation(&self->smallWheelAnimator);

    self->couplingAngle = (self->couplingAngle + 16) & 0x1FF;

    StateMachine_Run(self->state);

    if (!(Zone->timer & 0x3F)) {
        RSDK.PlaySfx(EggLoco->sfxSmoke, false, 255);
        CREATE_ENTITY(LocoSmoke, NULL, self->position.x + 0x450000, self->position.y - 0x480000);
    }
}

void EggLoco_LateUpdate(void) {}

void EggLoco_StaticUpdate(void) {}

void EggLoco_Draw(void)
{
    RSDK_THIS(EggLoco);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y - 2 * self->tootPullPos;
    RSDK.DrawSprite(&self->eggmanTootAnimator, 0, false);
    RSDK.DrawSprite(&self->whistleAnimator, &drawPos, false);

    drawPos.x                          = self->position.x - 0x2D0000;
    drawPos.y                          = self->position.y - 0x3F0000;
    self->drawFX                       = FX_ROTATE;
    self->rotation                     = -(self->tootPullPos >> 15);
    self->whistleCableAnimator.frameID = 0;
    RSDK.DrawSprite(&self->whistleCableAnimator, &drawPos, false);

    drawPos.x                          = self->position.x - 0x5F0000;
    drawPos.y                          = self->tootPullPos + self->position.y - 0x3F0000;
    self->drawFX                       = FX_NONE;
    self->whistleCableAnimator.frameID = 1;
    RSDK.DrawSprite(&self->whistleCableAnimator, &drawPos, false);

    self->whistleCableAnimator.frameID = 2;
    RSDK.DrawSprite(&self->whistleCableAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->eggmanTootAnimator, NULL, false);
    RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);
    RSDK.DrawSprite(&self->locomotiveAnimator, NULL, false);

    drawPos.x      = self->position.x - 0x680000;
    drawPos.y      = self->position.y + 0x380000;
    self->rotation = self->couplingAngle;
    self->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&self->bigWheelAnimator, &drawPos, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->bigWheelEdgeAnimator, &drawPos, false);

    drawPos.x += 0x4A0000;
    self->drawFX   = FX_ROTATE;
    self->rotation = self->couplingAngle + 0x100;
    RSDK.DrawSprite(&self->bigWheelAnimator, &drawPos, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->bigWheelEdgeAnimator, &drawPos, false);

    drawPos.x = self->position.x + 0x160000;
    drawPos.y = self->position.y + 0x4B0000;
    RSDK.DrawSprite(&self->smallWheelAnimator, &drawPos, false);

    drawPos.x += 0x3E0000;
    RSDK.DrawSprite(&self->smallWheelAnimator, &drawPos, false);

    drawPos.x                      = (2304 * RSDK.Cos512(self->couplingAngle) + self->position.x - 0x680000) + 0x4C0000;
    drawPos.y                      = ((9 * RSDK.Sin512(self->couplingAngle)) << 8) + self->position.y + 0x380000;
    self->couplingAnimator.frameID = 1;
    self->drawFX                   = FX_ROTATE;
    self->rotation                 = RSDK.Sin512(-self->couplingAngle) / 18 - 4;
    RSDK.DrawSprite(&self->couplingAnimator, &drawPos, false);

    self->drawFX = FX_NONE;
    drawPos.x -= 0x4C0000;
    self->couplingAnimator.frameID = 0;
    RSDK.DrawSprite(&self->couplingAnimator, &drawPos, false);

    self->pistonRodAnimator.frameID = 0;
    drawPos.x                       = self->position.x + ((RSDK.Cos512(self->couplingAngle) + 256) << 11);
    drawPos.y                       = self->position.y;
    RSDK.DrawSprite(&self->pistonRodAnimator, &drawPos, false);

    self->pistonRodAnimator.frameID = 2;
    RSDK.DrawSprite(&self->pistonRodAnimator, &drawPos, false);

    self->pistonRodAnimator.frameID = 1;
    RSDK.DrawSprite(&self->pistonRodAnimator, NULL, false);
    RSDK.DrawSprite(&self->guardAnimator, NULL, false);
    RSDK.DrawSprite(&self->lanternAnimator, NULL, false);
}

void EggLoco_Create(void *data)
{
    RSDK_THIS(EggLoco);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0xA00000;
        self->updateRange.y = 0x600000;
        self->state         = EggLoco_State_TootDelay;
        self->timer         = 60;

        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 0, &self->locomotiveAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 6, &self->whistleAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 1, &self->bigWheelAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 1, &self->bigWheelEdgeAnimator, true, 1);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 2, &self->smallWheelAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 3, &self->couplingAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 5, &self->guardAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 7, &self->lanternAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 4, &self->pistonRodAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 10, &self->eggmanAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 12, &self->eggmanTootAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 13, &self->whistleCableAnimator, true, 0);
    }
}

void EggLoco_StageLoad(void)
{
    EggLoco->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Train.bin", SCOPE_STAGE);

    EggLoco->sfxSmoke   = RSDK.GetSfx("MSZ/LocoSmoke.wav");
    EggLoco->sfxWhistle = RSDK.GetSfx("MSZ/LocoWhistle.wav");
    Soundboard_LoadSfx("MSZ/LocoChugga.wav", true, EggLoco_SfxCheck_LocoChugga, EggLoco_SfxUpdate_LocoChugga);
}

bool32 EggLoco_SfxCheck_LocoChugga(void)
{
    int32 count   = 0;
    int32 screenX = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
    int32 screenY = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;

    foreach_all(EggLoco, eggLoco)
    {
        if ((MathHelpers_Distance(eggLoco->position.x, eggLoco->position.y, screenX, screenY) >> 16) <= 840)
            count++;
    }

    return count > 0;
}

void EggLoco_SfxUpdate_LocoChugga(int32 sfx)
{
    int32 screenX = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
    int32 screenY = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;

    foreach_all(EggLoco, eggLoco)
    {
        int32 distX = abs(screenX - eggLoco->position.x) >> 16;
        int32 distY = abs(screenY - eggLoco->position.y) >> 16;
        int32 vol   = MIN(MathHelpers_SquareRoot(distX * distX + distY * distY), 840);

        RSDK.SetChannelAttributes(Soundboard->sfxChannel[sfx], 1.0 - (vol / 840.0), 0.0, 1.0);
        foreach_break;
    }
}

void EggLoco_State_TootDelay(void)
{
    RSDK_THIS(EggLoco);

    if (self->timer <= 0) {
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 11, &self->eggmanAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 12, &self->eggmanTootAnimator, true, 0);
        self->state = EggLoco_State_Whistle;
    }
    else {
        self->timer--;
    }
}

void EggLoco_State_Whistle(void)
{
    RSDK_THIS(EggLoco);

    if (++self->timer == 4) {
        RSDK.PlaySfx(EggLoco->sfxWhistle, false, 255);
        self->timer = 0;
        self->state = EggLoco_State_Tooting;
    }
}

void EggLoco_State_Tooting(void)
{
    RSDK_THIS(EggLoco);

    RSDK.ProcessAnimation(&self->eggmanTootAnimator);

    // How much eggman's pulled the "toot" down
    self->tootPullPos = (RSDK.GetFrameID(&self->eggmanTootAnimator) - 'a') << 16;

    if (self->tootPullPos >= 0x10000 && !(Zone->timer & 3)) {
        EntityDust *dust = CREATE_ENTITY(Dust, NULL, self->position.x - 0x240000, self->position.y - 0x4E0000);
        dust->state      = Dust_State_DustPuff_Friction;
        dust->velocity.x = 0x20000;
        dust->velocity.y = -0x18000;
        dust->drawGroup  = Zone->objectDrawGroup[0];
    }

    if (self->eggmanAnimator.animationID == 11 && self->eggmanAnimator.frameID == self->eggmanAnimator.frameCount - 1)
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 10, &self->eggmanAnimator, true, 0);

    if (self->eggmanTootAnimator.frameID == self->eggmanTootAnimator.frameCount - 1) {
        self->timer = RSDK.Rand(60, 180);
        self->state = EggLoco_State_TootDelay;
    }
}

#if GAME_INCLUDE_EDITOR
void EggLoco_EditorDraw(void)
{
    RSDK_THIS(EggLoco);

    self->updateRange.x = 0xA00000;
    self->updateRange.y = 0x600000;

    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 0, &self->locomotiveAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 6, &self->whistleAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 1, &self->bigWheelAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 1, &self->bigWheelEdgeAnimator, true, 1);
    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 2, &self->smallWheelAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 3, &self->couplingAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 5, &self->guardAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 7, &self->lanternAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 4, &self->pistonRodAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 10, &self->eggmanAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 12, &self->eggmanTootAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggLoco->aniFrames, 13, &self->whistleCableAnimator, true, 0);

    EggLoco_Draw();
}

void EggLoco_EditorLoad(void) { EggLoco->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Train.bin", SCOPE_STAGE); }
#endif

void EggLoco_Serialize(void) {}
