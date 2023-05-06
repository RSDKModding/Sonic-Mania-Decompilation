// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SentryBug Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSentryBug *SentryBug;

void SentryBug_Update(void)
{
    RSDK_THIS(SentryBug);

    RSDK.ProcessAnimation(&self->bugAnimator);

    StateMachine_Run(self->state);
    StateMachine_Run(self->stateOrbs);
}

void SentryBug_LateUpdate(void) {}

void SentryBug_StaticUpdate(void) {}

void SentryBug_Draw(void)
{
    RSDK_THIS(SentryBug);

    if (self->orbsDetatched) {
        RSDK.DrawSprite(&self->bugAnimator, NULL, false);

        if (self->showNet) {
            RSDK.MatrixScaleXYZ(&self->matTransform, self->netScale >> 4, self->netScale >> 4, self->netScale >> 4);
            RSDK.MatrixTranslateXYZ(&self->matTransform, self->netPos.x, self->netPos.y, 0, false);
            RSDK.MatrixRotateXYZ(&self->matWorld, self->netRotation, self->netRotation, 0);
            RSDK.MatrixMultiply(&self->matWorld, &self->matWorld, &self->matTransform);

            RSDK.Prepare3DScene(SentryBug->sceneIndex);
            RSDK.AddModelTo3DScene(SentryBug->meshFrames, SentryBug->sceneIndex, S3D_WIREFRAME, &self->matWorld, NULL, self->netColor);

            self->inkEffect = INK_ALPHA;
            RSDK.Draw3DScene(SentryBug->sceneIndex);
            self->inkEffect = INK_NONE;
        }

        for (int32 angle = 0, i = 0; angle < 0xFC; ++i) {
            self->orbAnimator.frameID = ((angle + self->netRotation + 8) >> 5) & 7;
            if (self->netColor == 0xF0F000)
                self->orbAnimator.frameID += 8;

            RSDK.DrawSprite(&self->orbAnimator, &self->orbPositions[i], false);

            angle += 42;
        }
    }
    else {
        RSDK.DrawSprite(&self->orbAnimator, &self->orbPositions[0], false);
        RSDK.DrawSprite(&self->orbAnimator, &self->orbPositions[1], false);
        RSDK.DrawSprite(&self->orbAnimator, &self->orbPositions[2], false);

        RSDK.DrawSprite(&self->bugAnimator, NULL, false);

        RSDK.DrawSprite(&self->orbAnimator, &self->orbPositions[3], false);
        RSDK.DrawSprite(&self->orbAnimator, &self->orbPositions[4], false);
        RSDK.DrawSprite(&self->orbAnimator, &self->orbPositions[5], false);
    }
}

void SentryBug_Create(void *data)
{
    RSDK_THIS(SentryBug);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->amplitude.x >>= 10;
        self->amplitude.y >>= 10;
        self->originPos = self->position;
        self->startPos  = self->position;
        self->netColor  = 0xF02000;
        self->alpha     = 0x100;

        if (!self->speed)
            self->speed = 4;

        if (self->amplitude.x >= 0)
            RSDK.SetSpriteAnimation(SentryBug->aniFrames, 1, &self->bugAnimator, true, 6);
        else
            RSDK.SetSpriteAnimation(SentryBug->aniFrames, 0, &self->bugAnimator, true, 6);

        RSDK.SetSpriteAnimation(SentryBug->aniFrames, 2, &self->orbAnimator, true, 0);

        self->state     = SentryBug_State_AwaitPlayer;
        self->stateOrbs = SentryBug_StateOrbs_Attached;
    }
}

void SentryBug_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("TMZ1") || RSDK.CheckSceneFolder("TMZ2"))
        SentryBug->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/SentryBug.bin", SCOPE_STAGE);

    SentryBug->meshFrames = RSDK.LoadMesh("TMZ/OrbNet.bin", SCOPE_STAGE);
    SentryBug->sceneIndex = RSDK.Create3DScene("View:SentryBug", 512, SCOPE_STAGE);

    SentryBug->hitbox.left   = -12;
    SentryBug->hitbox.top    = -12;
    SentryBug->hitbox.right  = 12;
    SentryBug->hitbox.bottom = 12;

    SentryBug->sfxPon   = RSDK.GetSfx("Stage/Pon.wav");
    SentryBug->sfxSwarm = RSDK.GetSfx("TMZ1/CyberSwarm.wav");

    DEBUGMODE_ADD_OBJ(SentryBug);
}

void SentryBug_DebugSpawn(void)
{
    RSDK_THIS(SentryBug);

    EntitySentryBug *sentryBug = CREATE_ENTITY(SentryBug, NULL, self->position.x, self->position.y);
    sentryBug->amplitude.x     = 0x1200;
    sentryBug->amplitude.y     = 0x400;
    sentryBug->timer           = 60;
}

void SentryBug_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SentryBug->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void SentryBug_CheckPlayerCollisions(void)
{
    RSDK_THIS(SentryBug);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &SentryBug->hitbox)) {
            if (Player_CheckBadnikBreak(player, self, true))
                RSDK.StopSfx(SentryBug->sfxSwarm);
        }
    }
}

void SentryBug_SetupOrbDropVelocity(void)
{
    RSDK_THIS(SentryBug);

    Vector2 *velocity = NULL;
    if (self->bugAnimator.animationID == 1)
        velocity = (Vector2 *)SentryBug->dropVelR;
    else
        velocity = (Vector2 *)SentryBug->dropVelL;

    for (int32 i = 0; i < 6; ++i) {
        self->orbVelocities[i].x = velocity[i].x;
        self->orbVelocities[i].y = velocity[i].y;
    }
}

void SentryBug_StateOrbs_Attached(void)
{
    RSDK_THIS(SentryBug);

    Vector2 *offsets = NULL;
    if (self->bugAnimator.animationID == 1)
        offsets = (Vector2 *)&SentryBug->orbOffsets_Attatched[12 * self->bugAnimator.frameID];
    else
        offsets = (Vector2 *)&SentryBug->orbOffsets_Attatched[-12 * self->bugAnimator.frameID + 72];

    for (int32 i = 0; i < 6; ++i) {
        self->orbPositions[i].x = self->position.x + offsets[i].x;
        self->orbPositions[i].y = self->position.y + offsets[i].y;
    }
}

void SentryBug_StateOrbs_Dropped(void)
{
    RSDK_THIS(SentryBug);

    for (int32 i = 0; i < 6; ++i) {
        self->orbVelocities[i].y += 0x2800;
        self->orbPositions[i].x += self->orbVelocities[i].x;
        self->orbPositions[i].y += self->orbVelocities[i].y;
    }
}

void SentryBug_StateOrbs_BeginNetRotation(void)
{
    RSDK_THIS(SentryBug);

    int32 angle = self->netRotation;
    for (int32 i = 0; i < 6; ++i) {
        int32 x = self->netScale * RSDK.Cos256(angle) + self->netPos.x;
        int32 y = self->netScale * RSDK.Sin256(angle) + self->netPos.y;

        self->orbVelocities[i].x = (x - self->orbPositions[i].x) >> 3;
        self->orbVelocities[i].y = (y - self->orbPositions[i].y) >> 3;
        self->orbPositions[i].x += self->orbVelocities[i].x;
        self->orbPositions[i].y += self->orbVelocities[i].y;
        angle += 42;
    }

    self->netRotation += 4;
}

void SentryBug_StateOrbs_RotateAroundNet(void)
{
    RSDK_THIS(SentryBug);

    int32 angle = self->netRotation;
    for (int32 i = 0; i < 6; ++i) {
        self->orbPositions[i].x = self->netScale * RSDK.Cos256(angle) + self->netPos.x;
        self->orbPositions[i].y = self->netScale * RSDK.Sin256(angle) + self->netPos.y;
        angle += 42;
    }

    self->netRotation += 4;
}

void SentryBug_StateOrbs_ReturnToSlots(void)
{
    RSDK_THIS(SentryBug);

    Vector2 *offsets = NULL;
    if (self->bugAnimator.animationID == 1)
        offsets = (Vector2 *)&SentryBug->orbOffsets_Attatched[12 * self->bugAnimator.frameID];
    else
        offsets = (Vector2 *)&SentryBug->orbOffsets_Attatched[-12 * self->bugAnimator.frameID + 72];

    for (int32 i = 0; i < 6; ++i) {
        self->orbVelocities[i].x = CLAMP((self->position.x + offsets[i].x - self->orbPositions[i].x) >> 3, -0xC0000, 0xC0000);
        self->orbVelocities[i].y = CLAMP((self->position.y + offsets[i].y - self->orbPositions[i].y) >> 3, -0xC0000, 0xC0000);

        self->orbPositions[i].x += self->orbVelocities[i].x;
        self->orbPositions[i].y += self->orbVelocities[i].y;
    }
}

void SentryBug_State_AwaitPlayer(void)
{
    RSDK_THIS(SentryBug);

    int32 x = self->position.x;

    self->angle      = (self->angle + self->speed) & 0x3FF;
    self->position.x = self->amplitude.x * RSDK.Sin1024(self->angle) + self->originPos.x;
    self->position.y = self->amplitude.y * RSDK.Sin1024(2 * self->angle) + self->originPos.y;
    RSDK.SetSpriteAnimation(SentryBug->aniFrames, (x - self->position.x) < 0, &self->bugAnimator, false, 0);

    if (self->timer > 0)
        self->timer--;

    EntityPlayer *player = Player_GetNearestPlayer();
    if (player) {
        if (!self->timer) {
            int32 rx = (self->position.x - player->position.x) >> 16;
            int32 ry = (self->position.y - player->position.y) >> 16;

            if (rx * rx + ry * ry < 0x2400) {
                self->originPos.x = self->position.x;
                self->originPos.y = self->position.y;
                self->angle       = 0;
                self->playerPtr   = player;
                self->netScale    = 0x6800;
                self->drawGroup   = Zone->objectDrawGroup[1];
                self->active      = ACTIVE_NORMAL;

                RSDK.PlaySfx(SentryBug->sfxPon, false, 255);

                SentryBug_SetupOrbDropVelocity();
                self->stateOrbs = SentryBug_StateOrbs_Dropped;

                RSDK.PlaySfx(SentryBug->sfxSwarm, false, 255);
                self->state = SentryBug_State_DropOrbs;
            }
        }
    }

    SentryBug_CheckPlayerCollisions();
}

void SentryBug_State_DropOrbs(void)
{
    RSDK_THIS(SentryBug);

    EntityPlayer *player = self->playerPtr;

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 4, 10);

    self->netPos.x = player->position.x;
    self->netPos.y = player->position.y;

    if (++self->timer == 120) {
        self->timer     = 0;
        self->netScale  = 0x6000;
        self->stateOrbs = SentryBug_StateOrbs_RotateAroundNet;
        self->showNet   = true;
        self->state     = SentryBug_State_NetAppear;
    }

    if (self->timer == 30) {
        self->orbsDetatched = true;
        self->stateOrbs     = SentryBug_StateOrbs_BeginNetRotation;
    }

    SentryBug_CheckPlayerCollisions();
}

void SentryBug_State_NetAppear(void)
{
    RSDK_THIS(SentryBug);

    EntityPlayer *player = self->playerPtr;

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 4, 10);

    self->netPos.x += (player->position.x - self->netPos.x) >> 3;
    self->netPos.y += (player->position.y - self->netPos.y) >> 3;
    SentryBug_CheckPlayerCollisions();

    self->alpha = 0xA0 + (RSDK.Sin256(4 * Zone->timer) >> 2);

    if (++self->timer == 60) {
        self->timer = 0;
        self->state = SentryBug_State_NetShrink;
    }
}

void SentryBug_State_NetShrink(void)
{
    RSDK_THIS(SentryBug);

    EntityPlayer *player = self->playerPtr;

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 4, 10);

    self->netPos.x += (player->position.x - self->netPos.x) >> 3;
    self->netPos.y += ((player->position.y - self->netPos.y) >> 3);

    int32 rx = (self->netPos.x - self->position.x) >> 16;
    int32 ry = (self->netPos.y - self->position.y) >> 16;

    self->alpha = 0xA0 + (RSDK.Sin256(4 * Zone->timer) >> 2);

    if (rx * rx + ry * ry <= 0x40000) {
        if (self->netScale <= 0x1800) {
            rx = (self->netPos.x - player->position.x) >> 16;
            ry = (self->netPos.y - player->position.y) >> 16;

            if (rx * rx + ry * ry < 0x200 && Player_CheckValidState(player)) {
                self->showNet     = true;
                self->alpha       = 0x100;
                self->state       = SentryBug_State_NetFlash;
                self->isPermanent = true;
            }
            else {
                self->orbAnimator.frameID = 0;
                self->orbsDetatched       = false;
                self->stateOrbs           = SentryBug_StateOrbs_ReturnToSlots;
                self->state               = SentryBug_State_ReturnToSlots;
                RSDK.StopSfx(SentryBug->sfxSwarm);
            }
        }
        else {
            self->netScale -= 0x80;
        }

        SentryBug_CheckPlayerCollisions();
    }
    else {
        self->orbAnimator.frameID = 0;
        self->orbsDetatched       = false;

        self->stateOrbs = SentryBug_StateOrbs_ReturnToSlots;
        self->state     = SentryBug_State_ReturnToSlots;

        RSDK.StopSfx(SentryBug->sfxSwarm);
    }
}

void SentryBug_State_NetFlash(void)
{
    RSDK_THIS(SentryBug);

    EntityPlayer *player = self->playerPtr;

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 4, 10);

    self->netPos.x = player->position.x;
    self->netPos.y = player->position.y;

    player->groundVel >>= 2;
    player->velocity.x >>= 2;
    player->velocity.y >>= 2;

    self->netColor = (Zone->timer & 2) ? 0xF0F000 : 0xF02000;

    if (++self->timer == 30) {
        self->timer = 0;
        Player_Hurt(player, self);

        self->orbAnimator.frameID = 0;
        self->netColor            = 0xF02000;
        self->orbsDetatched       = false;
        self->stateOrbs           = SentryBug_StateOrbs_ReturnToSlots;
        self->state               = SentryBug_State_ReturnToSlots;

        RSDK.StopSfx(SentryBug->sfxSwarm);
        self->isPermanent = false;
    }

    SentryBug_CheckPlayerCollisions();
}

void SentryBug_State_ReturnToSlots(void)
{
    RSDK_THIS(SentryBug);

    if (self->alpha > 0)
        self->alpha -= 0x40;

    self->velocity.x = (self->startPos.x - self->position.x) >> 4;
    self->velocity.y = (self->startPos.y - self->position.y) >> 4;

    RSDK.SetSpriteAnimation(SentryBug->aniFrames, self->velocity.x > 0, &self->bugAnimator, false, 0);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (++self->timer == 60) {
        self->originPos.x = self->startPos.x;
        self->originPos.y = self->startPos.y;
        self->angle       = 0;
        self->timer       = 60;
        self->active      = ACTIVE_BOUNDS;
        self->drawGroup   = Zone->objectDrawGroup[0];
        self->stateOrbs   = SentryBug_StateOrbs_Attached;
        self->state       = SentryBug_State_AwaitPlayer;
    }

    SentryBug_CheckPlayerCollisions();
}

#if GAME_INCLUDE_EDITOR
void SentryBug_EditorDraw(void)
{
    RSDK_THIS(SentryBug);

    if (!self->speed)
        self->speed = 4;

    self->originPos = self->position;
    self->startPos  = self->position;

    if (self->amplitude.x >= 0)
        RSDK.SetSpriteAnimation(SentryBug->aniFrames, 1, &self->bugAnimator, true, 6);
    else
        RSDK.SetSpriteAnimation(SentryBug->aniFrames, 0, &self->bugAnimator, true, 6);

    RSDK.SetSpriteAnimation(SentryBug->aniFrames, 2, &self->orbAnimator, true, 0);

    RSDK.DrawSprite(&self->bugAnimator, NULL, false);
}

void SentryBug_EditorLoad(void) { SentryBug->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/SentryBug.bin", SCOPE_STAGE); }
#endif

void SentryBug_Serialize(void)
{
    RSDK_EDITABLE_VAR(SentryBug, VAR_UINT8, speed);
    RSDK_EDITABLE_VAR(SentryBug, VAR_VECTOR2, amplitude);
}
