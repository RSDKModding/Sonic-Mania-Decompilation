// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MicDrop Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMicDrop *MicDrop;

void MicDrop_Update(void)
{
    RSDK_THIS(MicDrop);
    StateMachine_Run(self->state);
}

void MicDrop_LateUpdate(void) {}

void MicDrop_StaticUpdate(void) {}

void MicDrop_Draw(void)
{
    RSDK_THIS(MicDrop);

    SpriteFrame *frame      = RSDK.GetFrame(MicDrop->aniFrames, 1, self->animator2.frameID);
    SpriteFrame *otherFrame = RSDK.GetFrame(MicDrop->aniFrames, 1, self->animator2.frameID + 4);

    if (self->animator2.frameID == 3) {
        RSDK.DrawLine(self->startPos.x, self->startPos.y, self->position.x, self->position.y, 0x081828, 0, INK_NONE, false);
        RSDK.DrawLine(self->startPos.x - 0x10000, self->startPos.y, self->position.x - 0x10000, self->position.y, 0x204078, 0, INK_NONE,
                      false);
    }
    else {
        int distance = (self->position.y - self->startPos.y) >> 16;
        if (distance > 58) {
            frame->height = otherFrame->height;
            frame->pivotY = otherFrame->pivotY;
            frame->sprY   = otherFrame->sprY;
            RSDK.DrawLine(self->startPos.x, self->startPos.y, self->position.x, self->position.y - (58 << 16), 0x081828, 0, INK_NONE, false);
            RSDK.DrawLine(self->startPos.x - 0x10000, self->startPos.y, self->position.x - 0x10000, self->position.y - (58 << 16), 0x204078,
                          0, INK_NONE, false);
        }
        else {
            frame->height = distance + 2;
            frame->pivotY = 1 - distance;
            frame->sprY   = otherFrame->sprY - distance + 58;
        }
    }

    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void MicDrop_Create(void *data)
{
    RSDK_THIS(MicDrop);

    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    RSDK.SetSpriteAnimation(MicDrop->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(MicDrop->aniFrames, 1, &self->animator2, true, 0);
    self->state = MicDrop_State_Setup;
}

void MicDrop_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/MicDrop.bin", SCOPE_STAGE);
    else
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/MicDrop.bin", SCOPE_STAGE);
    MicDrop->hitbox1.left   = -12;
    MicDrop->hitbox1.top    = -15;
    MicDrop->hitbox1.right  = 12;
    MicDrop->hitbox1.bottom = 15;
    MicDrop->hitbox2.left   = -128;
    MicDrop->hitbox2.top    = 0;
    MicDrop->hitbox2.right  = 128;
    MicDrop->hitbox2.bottom = 256;
    DEBUGMODE_ADD_OBJ(MicDrop);
    MicDrop->sfxElectrify = RSDK.GetSfx("Stage/Electrify.wav");
}

void MicDrop_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityMicDrop *micDrop = CREATE_ENTITY(MicDrop, NULL, self->position.x, self->position.y);
    micDrop->distance      = 128;
}

void MicDrop_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(MicDrop->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void MicDrop_CheckPlayerCollisions(void)
{
    RSDK_THIS(MicDrop);

    int storeX = self->position.x;
    int storeY = self->position.y;

    self->position.x = (((self->radius + 25) * RSDK.Cos512(self->angle + 128)) << 7) + self->startPos.x;
    self->position.y = (((self->radius + 25) * RSDK.Sin512(self->angle + 128)) << 7) + self->startPos.y;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &MicDrop->hitbox1)) {
            if (self->animator1.animationID != 3 || player->shield == SHIELD_LIGHTNING || player->invincibleTimer || player->blinkTimer) 
                Player_CheckBadnikBreak(self, player, true);
            else 
                Player_CheckHit(player, self);
        }
    }
    self->position.x = storeX;
    self->position.y = storeY;
}

void MicDrop_CheckOnScreen(void)
{

    RSDK_THIS(MicDrop);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        MicDrop_Create(NULL);
    }
}

void MicDrop_State_Setup(void)
{
    RSDK_THIS(MicDrop);

    self->active     = ACTIVE_NORMAL;
    self->velocity.y = 0;
    self->direction  = FLIP_NONE;
    self->timer      = 0;
    self->field_A8   = 0;
    self->angle      = 0;
    self->rotation   = 0;
    self->state      = MicDrop_State_Unknown1;
    MicDrop_State_Unknown1();
}

void MicDrop_State_Unknown1(void)
{
    RSDK_THIS(MicDrop);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MicDrop->hitbox2))
            self->state = MicDrop_State_Unknown2;
    }
    MicDrop_CheckPlayerCollisions();
}

void MicDrop_State_Unknown2(void)
{
    RSDK_THIS(MicDrop);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->radius = (self->position.y - self->startPos.y) >> 16;

    if (self->radius >= 0x20000)
        self->animator1.frameID = 3;

    if (self->radius >= self->distance) {
        self->radius            = self->distance;
        self->position.y        = self->startPos.y + (self->distance << 16);
        self->animator1.frameID = 4;
        self->state             = MicDrop_State_Unknown3;
    }
    MicDrop_CheckPlayerCollisions();
}

void MicDrop_State_Unknown3(void)
{
    RSDK_THIS(MicDrop);

    switch (++self->timer) {
        case 2:
            self->animator2.frameID = 1;
            self->animator1.frameID = 5;
            break;
        case 4:
            self->animator2.frameID = 2;
            self->animator1.frameID = 1;
            break;
        case 6:
            self->animator2.frameID = 3;
            self->animator1.frameID = 0;
            self->timer             = 0;
            self->state             = MicDrop_State_Unknown4;
            break;
    }
    MicDrop_CheckPlayerCollisions();
    MicDrop_CheckOnScreen();
}

void MicDrop_State_Unknown4(void)
{
    RSDK_THIS(MicDrop);

    if (++self->timer >= 30) {
        self->field_9C = 64;
        self->field_A0 = 4;
        self->state    = MicDrop_State_Unknown5;
    }
    MicDrop_CheckPlayerCollisions();
    MicDrop_CheckOnScreen();
}

void MicDrop_State_Unknown5(void)
{
    RSDK_THIS(MicDrop);

    self->field_A8 += self->field_9C;
    if (self->field_A8 < 0) {
        self->field_A0 = 1;
        self->field_A8 -= self->field_9C;
        self->field_9C = self->field_9C >> 1;
        self->state    = MicDrop_State_Unknown6;
        MicDrop_State_Unknown6();
    }
    else {
        self->rotation   = self->field_A8 >> 4;
        self->angle      = self->rotation >> 2;
        self->field_9C   = self->field_9C - self->field_A0;
        self->position.x = (self->radius << 7) * RSDK.Cos512(self->angle + 128) + self->startPos.x;
        self->position.y = (self->radius << 7) * RSDK.Sin512(self->angle + 128) + self->startPos.y;
        MicDrop_CheckPlayerCollisions();
        MicDrop_CheckOnScreen();
    }
}

void MicDrop_State_Unknown6(void)
{
    RSDK_THIS(MicDrop);

    self->field_A8 += self->field_9C;
    self->angle    = self->field_A8 >> 4;
    self->rotation = self->angle;
    if (self->angle >= 0)
        self->field_9C -= self->field_A0;
    else
        self->field_9C += self->field_A0;
    self->position.x = (self->radius << 7) * RSDK.Cos512(self->angle + 128) + self->startPos.x;
    self->position.y = (self->radius << 7) * RSDK.Sin512(self->angle + 128) + self->startPos.y;

    switch (++self->timer) {
        case 60: RSDK.SetSpriteAnimation(MicDrop->aniFrames, 2, &self->animator1, true, 0); break;
        case 90:
            RSDK.PlaySfx(MicDrop->sfxElectrify, false, 255);
            RSDK.SetSpriteAnimation(MicDrop->aniFrames, 3, &self->animator1, true, 0);
            break;
        case 120:
            self->timer = 0;
            RSDK.SetSpriteAnimation(MicDrop->aniFrames, 0, &self->animator1, true, 0);
            break;
    }

    if (self->timer >= 60)
        RSDK.ProcessAnimation(&self->animator1);
    MicDrop_CheckPlayerCollisions();
    MicDrop_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void MicDrop_EditorDraw(void)
{
    RSDK_THIS(MicDrop);

    self->startPos.y = self->position.y;
    self->position.y = self->startPos.y + (self->distance << 16);

    MicDrop_Draw();

    self->position.y = self->startPos.y;
}

void MicDrop_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/MicDrop.bin", SCOPE_STAGE);
    else
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/MicDrop.bin", SCOPE_STAGE);
}
#endif

void MicDrop_Serialize(void) { RSDK_EDITABLE_VAR(MicDrop, VAR_UINT16, distance); }
