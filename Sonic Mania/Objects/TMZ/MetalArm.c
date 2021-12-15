// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MetalArm Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMetalArm *MetalArm;

void MetalArm_Update(void)
{
    RSDK_THIS(MetalArm);
    self->offset.x = -self->posUnknown.x;
    self->offset.y = -self->posUnknown.y;

    int32 valA = self->timer;
    if (self->durationA < self->timer)
        valA = self->durationA;
    int32 difA           = ((self->endAngleA - self->startAngleA) << 16) / self->durationA;
    self->armAngle.x = (self->startAngleA << 16) + difA * valA;

    int32 valB = self->timer;
    if (self->durationB < self->timer)
        valB = self->durationB;
    int32 difB           = ((self->endAngleB - self->startAngleB) << 16) / self->durationB;
    self->armAngle.y = (self->startAngleB << 16) + difB * valB;

    self->posUnknown = MetalArm_Unknown2();
    self->posUnknown.x &= 0xFFFF0000;
    self->posUnknown.y &= 0xFFFF0000;
    self->position.x = self->posUnknown.x;
    self->position.y = self->posUnknown.y;
    self->offset.x += self->position.x;
    self->offset.y += self->position.y;
    MetalArm_Unknown3();
    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
    StateMachine_Run(self->state);
}

void MetalArm_LateUpdate(void) {}

void MetalArm_StaticUpdate(void) {}

void MetalArm_Draw(void)
{
    RSDK_THIS(MetalArm);
    self->rotation = 0;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    int32 x = 0x2400 * RSDK.Cos512((self->armAngle.x >> 16)) + self->position.x;
    int32 y = 0x2400 * RSDK.Sin512((self->armAngle.x >> 16)) + self->position.y;

    Vector2 drawPos;
    drawPos            = MetalArm_Unknown2();
    self->position.x = x;
    self->position.y = y;
    self->position.x &= 0xFFFF0000;
    self->position.y &= 0xFFFF0000;
    self->rotation = (self->armAngle.x + self->armAngle.y) >> 16;
    RSDK.DrawSprite(&self->animator3, NULL, false);

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
    self->position.x &= 0xFFFF0000;
    self->position.y &= 0xFFFF0000;
    self->rotation = (self->armAngle.x >> 16);
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->rotation = 0;
    self->position = drawPos;
    self->position.x &= 0xFFFF0000;
    self->position.y &= 0xFFFF0000;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
}

void MetalArm_Create(void *data)
{
    RSDK_THIS(MetalArm);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderHigh;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;
    if (!self->durationA)
        self->durationA = 60;
    if (!self->durationB)
        self->durationB = 40;
    if (!self->holdDuration)
        self->holdDuration = 60;
    self->hitbox.left   = -56;
    self->hitbox.top    = -27;
    self->hitbox.right  = 35;
    self->hitbox.bottom = -7;
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 1, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 2, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 3, &self->animator4, true, 0);
    self->armAngle.x = self->startAngleA << 16;
    self->armAngle.y = self->startAngleB << 16;
    self->timer      = 0;
    self->posUnknown = MetalArm_Unknown2();
    self->state      = MetalArm_Unknown4;
}

void MetalArm_StageLoad(void)
{
    MetalArm->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MetalArm.bin", SCOPE_STAGE);
    Soundboard_LoadSFX("TMZ1/MetalArm.wav", true, MetalArm_Unknown1, NULL);
}

bool32 MetalArm_Unknown1(void)
{
    int32 count = 0;
    foreach_active(MetalArm, arm)
    {
        if (arm->state == MetalArm_Unknown5 || arm->state == MetalArm_Unknown7)
            ++count;
    }
    return count > 0;
}

Vector2 MetalArm_Unknown2(void)
{
    RSDK_THIS(MetalArm);
    Vector2 result;

    int32 x    = 0x2400 * RSDK.Cos512((self->armAngle.x >> 16)) + self->position.x;
    int32 y    = 0x2400 * RSDK.Sin512((self->armAngle.x >> 16)) + self->position.y;
    result.x = x + 0x3800 * RSDK.Cos512((self->armAngle.x + self->armAngle.y) >> 16);
    result.y = y + 0x3800 * RSDK.Sin512((self->armAngle.x + self->armAngle.y) >> 16);
    return result;
}

void MetalArm_Unknown3(void)
{
    RSDK_THIS(MetalArm);

    foreach_active(Player, player)
    {
        int32 id = RSDK.GetEntityID(player);
        if ((1 << id) & self->activePlayers) {
            player->position.x += self->offset.x;
            player->position.y += self->offset.y;
            player->position.y += 0x10000;
        }

        if (!Player_CheckCollisionPlatform(player, self, &self->hitbox))
            self->activePlayers &= ~(1 << id);
        else
            self->activePlayers |= (1 << id);
    }
}

void MetalArm_Unknown4(void)
{
    RSDK_THIS(MetalArm);
    if ((self->activePlayers & 1)) {
        self->timer  = 0;
        self->timer2 = 0;
        self->active = ACTIVE_NORMAL;
        self->state  = MetalArm_Unknown5;
    }
}

void MetalArm_Unknown5(void)
{
    RSDK_THIS(MetalArm);

    int32 duration = self->durationB;
    if (self->durationA > self->durationB)
        duration = self->durationA;
    if (self->timer >= duration) {
        self->timer2 = 0;
        self->state  = MetalArm_Unknown6;
    }
    else {
        self->timer++;
    }
}

void MetalArm_Unknown6(void)
{
    RSDK_THIS(MetalArm);
    if (self->activePlayers & 1) {
        self->timer2 = 0;
    }
    else {
        if (self->timer2 >= self->holdDuration) {
            self->state = MetalArm_Unknown7;
        }
        else {
            self->timer2++;
        }
    }
}

void MetalArm_Unknown7(void)
{
    RSDK_THIS(MetalArm);
    if (self->timer <= 0) {
        self->state  = MetalArm_Unknown4;
        self->active = ACTIVE_BOUNDS;
    }
    else {
        self->timer--;
    }
}

#if RETRO_INCLUDE_EDITOR
void MetalArm_EditorDraw(void)
{
    RSDK_THIS(MetalArm);
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->armAngle.x = self->startAngleA << 16;
    self->armAngle.y = self->startAngleB << 16;
    self->posUnknown = MetalArm_Unknown2();

    MetalArm_Draw();
}

void MetalArm_EditorLoad(void) { MetalArm->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MetalArm.bin", SCOPE_STAGE); }
#endif

void MetalArm_Serialize(void)
{
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, startAngleA);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, startAngleB);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, endAngleA);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, endAngleB);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, durationA);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, durationB);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, holdDuration);
}
