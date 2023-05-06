// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MicDrop Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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

    SpriteFrame *frame      = RSDK.GetFrame(MicDrop->aniFrames, 1, self->wireAnimator.frameID);
    SpriteFrame *otherFrame = RSDK.GetFrame(MicDrop->aniFrames, 1, self->wireAnimator.frameID + 4);

    if (self->wireAnimator.frameID == 3) {
        RSDK.DrawLine(self->startPos.x, self->startPos.y, self->position.x, self->position.y, 0x081828, 0, INK_NONE, false);
        RSDK.DrawLine(self->startPos.x - 0x10000, self->startPos.y, self->position.x - 0x10000, self->position.y, 0x204078, 0, INK_NONE, false);
    }
    else {
        int32 distance = (self->position.y - self->startPos.y) >> 16;
        if (distance > 58) {
            frame->height = otherFrame->height;
            frame->pivotY = otherFrame->pivotY;
            frame->sprY   = otherFrame->sprY;
            RSDK.DrawLine(self->startPos.x, self->startPos.y, self->position.x, self->position.y - (58 << 16), 0x081828, 0, INK_NONE, false);
            RSDK.DrawLine(self->startPos.x - 0x10000, self->startPos.y, self->position.x - 0x10000, self->position.y - (58 << 16), 0x204078, 0,
                          INK_NONE, false);
        }
        else {
            frame->height = distance + 2;
            frame->pivotY = 1 - distance;
            frame->sprY   = otherFrame->sprY - distance + 58;
        }
    }

    RSDK.DrawSprite(&self->wireAnimator, NULL, false);
    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
}

void MicDrop_Create(void *data)
{
    RSDK_THIS(MicDrop);

    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawFX        = FX_ROTATE | FX_FLIP;

    RSDK.SetSpriteAnimation(MicDrop->aniFrames, 0, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(MicDrop->aniFrames, 1, &self->wireAnimator, true, 0);
    self->state = MicDrop_State_Init;
}

void MicDrop_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/MicDrop.bin", SCOPE_STAGE);
    else
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/MicDrop.bin", SCOPE_STAGE);

    MicDrop->hitboxBadnik.left   = -12;
    MicDrop->hitboxBadnik.top    = -15;
    MicDrop->hitboxBadnik.right  = 12;
    MicDrop->hitboxBadnik.bottom = 15;

    MicDrop->hitboxRange.left   = -128;
    MicDrop->hitboxRange.top    = 0;
    MicDrop->hitboxRange.right  = 128;
    MicDrop->hitboxRange.bottom = 256;

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

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = (((self->radius + 25) * RSDK.Cos512(self->angle + 128)) << 7) + self->startPos.x;
    self->position.y = (((self->radius + 25) * RSDK.Sin512(self->angle + 128)) << 7) + self->startPos.y;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &MicDrop->hitboxBadnik)) {
            if (self->bodyAnimator.animationID != 3 || player->shield == SHIELD_LIGHTNING || player->invincibleTimer || player->blinkTimer)
                Player_CheckBadnikBreak(player, self, true);
            else
                Player_Hurt(player, self);
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void MicDrop_CheckOffScreen(void)
{
    RSDK_THIS(MicDrop);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        MicDrop_Create(NULL);
    }
}

void MicDrop_State_Init(void)
{
    RSDK_THIS(MicDrop);

    self->active     = ACTIVE_NORMAL;
    self->velocity.y = 0;
    self->direction  = FLIP_NONE;
    self->timer      = 0;
    self->swingPos   = 0;
    self->angle      = 0;
    self->rotation   = 0;

    self->state = MicDrop_State_CheckForPlayer;
    MicDrop_State_CheckForPlayer();
}

void MicDrop_State_CheckForPlayer(void)
{
    RSDK_THIS(MicDrop);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MicDrop->hitboxRange))
            self->state = MicDrop_State_DropDown;
    }

    MicDrop_CheckPlayerCollisions();
}

void MicDrop_State_DropDown(void)
{
    RSDK_THIS(MicDrop);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->radius = (self->position.y - self->startPos.y) >> 16;
    if (self->radius >= 0x20000)
        self->bodyAnimator.frameID = 3;

    if (self->radius >= self->distance) {
        self->radius               = self->distance;
        self->position.y           = self->startPos.y + (self->distance << 16);
        self->bodyAnimator.frameID = 4;
        self->state                = MicDrop_State_DropRecoil;
    }

    MicDrop_CheckPlayerCollisions();
}

void MicDrop_State_DropRecoil(void)
{
    RSDK_THIS(MicDrop);

    switch (++self->timer) {
        case 2:
            self->wireAnimator.frameID = 1;
            self->bodyAnimator.frameID = 5;
            break;

        case 4:
            self->wireAnimator.frameID = 2;
            self->bodyAnimator.frameID = 1;
            break;

        case 6:
            self->wireAnimator.frameID = 3;
            self->bodyAnimator.frameID = 0;

            self->timer = 0;
            self->state = MicDrop_State_Idle;
            break;
    }

    MicDrop_CheckPlayerCollisions();
    MicDrop_CheckOffScreen();
}

void MicDrop_State_Idle(void)
{
    RSDK_THIS(MicDrop);

    if (++self->timer >= 30) {
        self->swingSpeed = 64;
        self->swingVel   = 4;
        self->state      = MicDrop_State_Swinging;
    }

    MicDrop_CheckPlayerCollisions();
    MicDrop_CheckOffScreen();
}

void MicDrop_State_Swinging(void)
{
    RSDK_THIS(MicDrop);

    self->swingPos += self->swingSpeed;
    if (self->swingPos < 0) {
        self->swingVel = 1;
        self->swingPos -= self->swingSpeed;
        self->swingSpeed >>= 1;

        self->state = MicDrop_State_Electrify;
        MicDrop_State_Electrify();
    }
    else {
        self->rotation = self->swingPos >> 4;
        self->angle    = self->rotation >> 2;
        self->swingSpeed -= self->swingVel;
        self->position.x = self->startPos.x + (self->radius << 7) * RSDK.Cos512(self->angle + 0x80);
        self->position.y = self->startPos.y + (self->radius << 7) * RSDK.Sin512(self->angle + 0x80);

        MicDrop_CheckPlayerCollisions();
        MicDrop_CheckOffScreen();
    }
}

void MicDrop_State_Electrify(void)
{
    RSDK_THIS(MicDrop);

    self->swingPos += self->swingSpeed;
    self->angle    = self->swingPos >> 4;
    self->rotation = self->angle;
    if (self->angle >= 0)
        self->swingSpeed -= self->swingVel;
    else
        self->swingSpeed += self->swingVel;

    self->position.x = self->startPos.x + (self->radius << 7) * RSDK.Cos512(self->angle + 0x80);
    self->position.y = self->startPos.y + (self->radius << 7) * RSDK.Sin512(self->angle + 0x80);

    switch (++self->timer) {
        case 60: RSDK.SetSpriteAnimation(MicDrop->aniFrames, 2, &self->bodyAnimator, true, 0); break;

        case 90:
            RSDK.PlaySfx(MicDrop->sfxElectrify, false, 0xFF);
            RSDK.SetSpriteAnimation(MicDrop->aniFrames, 3, &self->bodyAnimator, true, 0);
            break;

        case 120:
            self->timer = 0;
            RSDK.SetSpriteAnimation(MicDrop->aniFrames, 0, &self->bodyAnimator, true, 0);
            break;
    }

    if (self->timer >= 60)
        RSDK.ProcessAnimation(&self->bodyAnimator);

    MicDrop_CheckPlayerCollisions();
    MicDrop_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
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
    if (RSDK.CheckSceneFolder("SPZ1"))
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/MicDrop.bin", SCOPE_STAGE);
    else
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/MicDrop.bin", SCOPE_STAGE);
}
#endif

void MicDrop_Serialize(void) { RSDK_EDITABLE_VAR(MicDrop, VAR_UINT16, distance); }
