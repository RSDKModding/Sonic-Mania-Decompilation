// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TVVan Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTVVan *TVVan;

void TVVan_Update(void)
{
    RSDK_THIS(TVVan);

    StateMachine_Run(self->state);
}

void TVVan_LateUpdate(void) {}

void TVVan_StaticUpdate(void) {}

void TVVan_Draw(void)
{
    RSDK_THIS(TVVan);

    StateMachine_Run(self->stateDraw[SceneInfo->currentDrawGroup]);
}

void TVVan_Create(void *data)
{
    RSDK_THIS(TVVan);

    if (!SceneInfo->inEditor) {
        if (data)
            self->type = VOID_TO_INT(data);

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawFX        = FX_FLIP;
        switch (self->type) {
            case TVVAN_SATELLITE_NE_SW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
                self->drawGroup                           = Zone->objectDrawGroup[1];
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Satellite_NE_SW;

                self->targetAngles1[0] = 0xE0;
                self->targetAngles1[1] = 0xE0;
                self->targetAngles2[0] = 0x60;
                self->targetAngles2[1] = 0x60;
                break;

            case TVVAN_SATELLITE_NW_SE:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
                self->drawGroup                           = Zone->objectDrawGroup[1];
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Satellite_NW_SE;

                self->targetAngles1[0] = 0x20;
                self->targetAngles1[1] = 0x20;
                self->targetAngles2[0] = 0xA0;
                self->targetAngles2[1] = 0xA0;
                break;

            case TVVAN_SATELLITE_NW_SW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
                self->drawGroup                           = Zone->objectDrawGroup[1];
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Satellite_NW_SW;

                self->targetAngles1[0] = 0x20;
                self->targetAngles1[1] = 0x20;
                self->targetAngles2[0] = 0xE0;
                self->targetAngles2[1] = 0xE0;
                break;

            case TVVAN_SATELLITE_NE_SE:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
                self->drawGroup                           = Zone->objectDrawGroup[1];
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Satellite_NE_SE;

                self->targetAngles1[0] = 0x60;
                self->targetAngles1[1] = 0x60;
                self->targetAngles2[0] = 0xA0;
                self->targetAngles2[1] = 0xA0;
                break;

            case TVVAN_SATELLITE_NE_NW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
                self->drawGroup                           = Zone->objectDrawGroup[1];
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Satellite_NE_NW;

                self->targetAngles1[0] = 0x20;
                self->targetAngles1[1] = 0x20;
                self->targetAngles2[0] = 0x60;
                self->targetAngles2[1] = 0x60;
                break;

            case TVVAN_SATELLITE_SE_SW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
                self->drawGroup                           = Zone->objectDrawGroup[1];
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Satellite_SE_SW;

                self->targetAngles1[0] = 0xE0;
                self->targetAngles1[1] = 0xE0;
                self->targetAngles2[0] = 0xA0;
                self->targetAngles2[1] = 0xA0;
                break;

            case TVVAN_SATELLITE_SW_N:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
                self->drawGroup                           = Zone->objectDrawGroup[1];
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Satellite_SW_N;

                self->targetAngles1[0] = 0xA0;
                self->targetAngles1[1] = 0xA0;
                self->targetAngles2[0] = 0x00;
                self->targetAngles2[1] = 0x00;
                break;

            case TVVAN_SATELLITE_NW_S:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
                self->drawGroup                           = Zone->objectDrawGroup[1];
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Satellite_NW_S;

                self->targetAngles1[0] = 0x20;
                self->targetAngles1[1] = 0x20;
                self->targetAngles2[0] = 0xC0;
                self->targetAngles2[1] = 0xC0;
                break;

            case TVVAN_SATELLITE_SE_N:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
                self->drawGroup                           = Zone->objectDrawGroup[1];
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Satellite_SE_N;

                self->targetAngles1[0] = 0xA0;
                self->targetAngles1[1] = 0xA0;
                self->targetAngles2[0] = 0x40;
                self->targetAngles2[1] = 0x40;
                break;

            case TVVAN_SATELLITE_NE_S:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
                self->drawGroup                           = Zone->objectDrawGroup[1];
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Satellite_NE_S;

                self->targetAngles1[0] = 0xC0;
                self->targetAngles1[1] = 0xC0;
                self->targetAngles2[0] = 0x40;
                self->targetAngles2[1] = 0x40;
                break;

            case TVVAN_EXITTV:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &self->tvVanAnimator, true, 9);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 15, &self->tvAnimator, true, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 18, &self->tvSnowAnimator, true, 0);
                self->drawGroup     = Zone->objectDrawGroup[0];
                self->updateRange.x = 0x1000000;
                self->updateRange.y = 0x1000000;

                self->stateDraw[Zone->objectDrawGroup[0]] = TVVan_Draw_ExitTV_Active;
                self->state                               = TVVan_StateTV_Idle;
                break;

            case TVVAN_RADIO:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &self->radioAnimator, true, 0);
                self->active    = ACTIVE_NORMAL;
                self->visible   = false;
                self->drawGroup = Zone->objectDrawGroup[1];

                self->state                               = TVVan_StateRadio_StartBroadcast;
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Radio;
                break;

            default:
                self->drawGroup = Zone->objectDrawGroup[0];
                self->state     = TVVan_StateVan_Idle;
                self->direction = self->type == TVVAN_VAN_RIGHT ? FLIP_X : FLIP_NONE;
                self->vanPos    = self->position;
                self->movePos   = self->position;

                self->stateDraw[Zone->objectDrawGroup[0]] = TVVan_Draw_Van_Low;
                self->stateDraw[Zone->objectDrawGroup[1]] = TVVan_Draw_Van_High;
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &self->satelliteAnimator, false, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 1, &self->leftDoorAnimator, false, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 2, &self->rightDoorAnimator, false, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 5, &self->windowAnimator, false, 0);
                break;
        }
    }
}

void TVVan_StageLoad(void)
{
    TVVan->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/TVVan.bin", SCOPE_STAGE);

    TVVan->hitboxRoof.left   = -40;
    TVVan->hitboxRoof.right  = 28;
    TVVan->hitboxRoof.top    = -28;
    TVVan->hitboxRoof.bottom = -20;

    TVVan->hitboxFloor.left   = -56;
    TVVan->hitboxFloor.right  = 24;
    TVVan->hitboxFloor.top    = 22;
    TVVan->hitboxFloor.bottom = 32;

    TVVan->hitboxFront.left   = 20;
    TVVan->hitboxFront.right  = 28;
    TVVan->hitboxFront.top    = -20;
    TVVan->hitboxFront.bottom = 32;

    TVVan->hitboxEntry.left   = 12;
    TVVan->hitboxEntry.right  = 20;
    TVVan->hitboxEntry.top    = -16;
    TVVan->hitboxEntry.bottom = 24;

    TVVan->hitboxShopWindow.left   = -24;
    TVVan->hitboxShopWindow.right  = 24;
    TVVan->hitboxShopWindow.top    = -24;
    TVVan->hitboxShopWindow.bottom = 24;

    TVVan->sfxDoor       = RSDK.GetSfx("Stage/Door.wav");
    TVVan->sfxSatellite1 = RSDK.GetSfx("Stage/Satellite1.wav");
    TVVan->sfxSatellite2 = RSDK.GetSfx("Stage/Satellite2.wav");
    TVVan->sfxExplosion  = RSDK.GetSfx("Stage/Explosion3.wav");
}

void TVVan_HandleVanTilt(void)
{
    RSDK_THIS(TVVan);

    if (!self->stood) {
        if (self->stoodAngle > 0)
            self->stoodAngle -= 4;

        if (self->angle < 0)
            self->angle++;
        else if (self->angle > 0)
            self->angle--;
    }
    else {
        if (self->stoodAngle < 64)
            self->stoodAngle += 4;

        self->angle = CLAMP(self->angle + self->stoodPos, -8, 8);
    }

    self->position.x  = self->movePos.x;
    self->position.y  = self->movePos.y;
    self->movePos.x   = self->vanPos.x;
    self->movePos.y   = ((RSDK.Sin256(self->stoodAngle) << 9) + self->vanPos.y) & 0xFFFF0000;
    self->moveOffsetY = self->movePos.y - self->position.y;
}

void TVVan_Draw_Van_Low(void)
{
    RSDK_THIS(TVVan);

    self->drawFX                    = FX_ROTATE | FX_FLIP;
    self->satelliteAnimator.frameID = 1;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->drawFX                    = FX_FLIP;
    self->satelliteAnimator.frameID = 4;
    RSDK.DrawSprite(&self->satelliteAnimator, &self->vanPos, false);

    self->drawFX |= FX_ROTATE;
    self->satelliteAnimator.frameID = 2;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 8;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
    RSDK.DrawSprite(&self->radioAnimator, NULL, false);

    self->drawFX                    = FX_FLIP;
    self->satelliteAnimator.frameID = 3;
    RSDK.DrawSprite(&self->satelliteAnimator, &self->vanPos, false);

    self->drawFX |= FX_ROTATE;
    RSDK.DrawSprite(&self->leftDoorAnimator, NULL, false);
    RSDK.AddDrawListRef(Zone->objectDrawGroup[1], SceneInfo->entitySlot);
}

void TVVan_Draw_Van_High(void)
{
    RSDK_THIS(TVVan);

    self->drawFX                    = FX_FLIP;
    self->satelliteAnimator.frameID = 5;
    RSDK.DrawSprite(&self->satelliteAnimator, &self->vanPos, false);

    self->satelliteAnimator.frameID = 6;
    RSDK.DrawSprite(&self->satelliteAnimator, &self->vanPos, false);

    self->satelliteAnimator.frameID = 7;
    RSDK.DrawSprite(&self->satelliteAnimator, &self->vanPos, false);

    self->drawFX |= FX_ROTATE;
    self->satelliteAnimator.frameID = 0;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
    RSDK.DrawSprite(&self->rightDoorAnimator, NULL, false);
    RSDK.DrawSprite(&self->windowAnimator, NULL, false);
}

void TVVan_Draw_Satellite_NE_SW(void)
{
    RSDK_THIS(TVVan);

    self->satelliteAnimator.frameID = 0;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->direction = FLIP_XY;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 2;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
}

void TVVan_Draw_Satellite_NW_SE(void)
{
    RSDK_THIS(TVVan);

    self->satelliteAnimator.frameID = 0;
    self->direction                 = FLIP_X;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->direction = FLIP_Y;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 2;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
}

void TVVan_Draw_Satellite_NW_SW(void)
{
    RSDK_THIS(TVVan);

    self->satelliteAnimator.frameID = 0;
    self->direction                 = FLIP_X;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->direction = FLIP_XY;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 2;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
}

void TVVan_Draw_Satellite_NE_SE(void)
{
    RSDK_THIS(TVVan);

    self->satelliteAnimator.frameID = 0;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->direction = FLIP_Y;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 2;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
}

void TVVan_Draw_Satellite_NE_NW(void)
{
    RSDK_THIS(TVVan);

    self->satelliteAnimator.frameID = 0;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 2;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
}

void TVVan_Draw_Satellite_SE_SW(void)
{
    RSDK_THIS(TVVan);

    self->satelliteAnimator.frameID = 0;
    self->direction                 = FLIP_Y;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->direction = FLIP_XY;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 2;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
}

void TVVan_Draw_Satellite_SW_N(void)
{
    RSDK_THIS(TVVan);

    self->satelliteAnimator.frameID = 1;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 0;
    self->direction                 = FLIP_XY;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 2;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
}

void TVVan_Draw_Satellite_NW_S(void)
{
    RSDK_THIS(TVVan);

    self->satelliteAnimator.frameID = 1;
    self->direction                 = FLIP_Y;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 0;
    self->direction                 = FLIP_X;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 2;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
}

void TVVan_Draw_Satellite_SE_N(void)
{
    RSDK_THIS(TVVan);

    self->satelliteAnimator.frameID = 1;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 0;
    self->direction                 = FLIP_Y;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 2;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
}

void TVVan_Draw_Satellite_NE_S(void)
{
    RSDK_THIS(TVVan);

    self->satelliteAnimator.frameID = 1;
    self->direction                 = FLIP_Y;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 0;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);

    self->satelliteAnimator.frameID = 2;
    self->direction                 = FLIP_NONE;
    RSDK.DrawSprite(&self->satelliteAnimator, NULL, false);
}

void TVVan_Draw_Radio(void)
{
    RSDK_THIS(TVVan);

    RSDK.DrawSprite(&self->radioAnimator, NULL, false);
}

void TVVan_Draw_ExitTV_Active(void)
{
    RSDK_THIS(TVVan);

    RSDK.DrawSprite(&self->tvVanAnimator, NULL, false);
    RSDK.DrawSprite(&self->tvAnimator, NULL, false);
}

void TVVan_Draw_ExitTV_Destroyed(void)
{
    RSDK_THIS(TVVan);
    Vector2 drawPos;

    RSDK.DrawSprite(&self->tvVanAnimator, NULL, false);
    RSDK.DrawSprite(&self->tvAnimator, NULL, false);

    self->drawFX = FX_SCALE;
    drawPos.x    = self->position.x;
    drawPos.y    = self->position.y - 0x40000;
    RSDK.DrawSprite(&self->playerAnimator, &drawPos, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->tvSnowAnimator, NULL, false);
}

void TVVan_StateVan_Idle(void)
{
    RSDK_THIS(TVVan);

    TVVan_HandleVanTilt();

    self->stood    = false;
    self->stoodPos = 0;
    bool32 entered = false;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &TVVan->hitboxRoof) == C_TOP) {
            player->position.y += self->moveOffsetY;
            player->position.y &= 0xFFFF0000;
            self->stood = true;

            int32 dist = player->position.x - self->position.x;
            if (self->direction == FLIP_NONE)
                dist += 0x100000;
            self->stoodPos += dist >> 20;
        }

        Player_CheckCollisionBox(player, self, &TVVan->hitboxFront);

        if (self->direction)
            TVVan->hitboxFloor.top = (player->position.x - self->position.x) >> 17;
        else
            TVVan->hitboxFloor.top = (self->position.x - player->position.x) >> 17;

        TVVan->hitboxFloor.top = CLAMP(TVVan->hitboxFloor.top, 22, 32);

        if (Player_CheckCollisionPlatform(player, self, &TVVan->hitboxFloor) == C_TOP) {
            player->position.y += self->moveOffsetY;
            player->position.y &= 0xFFFF0000;

            if (TVVan->hitboxFloor.top == 22)
                self->stood = true;
        }

        if (!player->sidekick && Player_CheckCollisionTouch(player, self, &TVVan->hitboxEntry)) {
            player->state           = Player_State_Static;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->velocity.x      = 0;
            player->velocity.y      = 0;
            player->groundVel       = 0;
            player->tileCollisions  = TILECOLLISION_NONE;
            player->interaction     = false;
            player->visible         = false;
            player->blinkTimer      = 0;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            player->animator.speed = 120;
            entered                = true;
            self->player           = player;
            foreach_break;
        }
    }

    self->rotation   = self->angle >> 2;
    self->position.y = self->movePos.y - abs(self->rotation << 15);

    if (entered) {
        self->state = TVVan_StateVan_BroadcastRadio;
        RSDK.PlaySfx(TVVan->sfxSatellite1, false, 255);
        self->leftDoorAnimator.speed  = 1;
        self->rightDoorAnimator.speed = 1;
        self->windowAnimator.speed    = 1;
    }

    RSDK.ProcessAnimation(&self->radioAnimator);
}

void TVVan_CheckPlayerCollisions(void)
{
    RSDK_THIS(TVVan);

    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, self, &TVVan->hitboxRoof);
        Player_CheckCollisionBox(player, self, &TVVan->hitboxFront);

        self->direction ^= FLIP_X;
        Player_CheckCollisionBox(player, self, &TVVan->hitboxFront);

        self->direction ^= FLIP_X;
        Player_CheckCollisionPlatform(player, self, &TVVan->hitboxFloor);
    }
}

void TVVan_StateVan_BroadcastRadio(void)
{
    RSDK_THIS(TVVan);

    TVVan_HandleVanTilt();
    TVVan_CheckPlayerCollisions();

    RSDK.ProcessAnimation(&self->leftDoorAnimator);
    RSDK.ProcessAnimation(&self->rightDoorAnimator);
    RSDK.ProcessAnimation(&self->windowAnimator);

    if (self->windowAnimator.frameID >= self->windowAnimator.frameCount - 1) {
        self->state = TVVan_StateVan_FinishedBroadcast;
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 3, &self->leftDoorAnimator, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 4, &self->rightDoorAnimator, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 5, &self->windowAnimator, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 7, &self->radioAnimator, true, 0);

        if (self->direction) {
            EntityTVVan *van = CREATE_ENTITY(TVVan, INT_TO_VOID(TVVAN_RADIO), self->position.x + 0x80000, self->position.y - 0x420000);
            van->isPermanent = true;
            van->player      = self->player;
            van->angle       = 0xA0;
            van->nextNode    = self;
        }
        else {
            EntityTVVan *van = CREATE_ENTITY(TVVan, INT_TO_VOID(TVVAN_RADIO), self->position.x - 0x80000, self->position.y - 0x420000);
            van->isPermanent = true;
            van->player      = self->player;
            van->angle       = 0xE0;
            van->nextNode    = self;
        }

        self->player = NULL;
    }
}

void TVVan_StateVan_FinishedBroadcast(void)
{
    RSDK_THIS(TVVan);

    TVVan_HandleVanTilt();
    TVVan_CheckPlayerCollisions();

    RSDK.ProcessAnimation(&self->leftDoorAnimator);
    RSDK.ProcessAnimation(&self->rightDoorAnimator);
    RSDK.ProcessAnimation(&self->radioAnimator);

    if (self->leftDoorAnimator.frameID >= self->leftDoorAnimator.frameCount - 1) {
        self->state = TVVan_StateVan_Idle;
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 1, &self->leftDoorAnimator, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 2, &self->rightDoorAnimator, true, 0);
    }
}

void TVVan_StateRadio_StartBroadcast(void)
{
    RSDK_THIS(TVVan);

    if (++self->timer == 20) {
        self->timer = 0;
        self->state = TVVan_StateRadio_HandleMovement;
    }
}

void TVVan_StateRadio_HandleMovement(void)
{
    RSDK_THIS(TVVan);
    EntityPlayer *player = self->player;

    self->position.x += 0xC00 * RSDK.Cos256(self->angle);
    self->position.y += 0xC00 * RSDK.Sin256(self->angle);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x, self->position.y);
    debris->timer        = 16;
    debris->drawGroup    = self->drawGroup;
    debris->drawFX       = FX_ROTATE;
    debris->inkEffect    = INK_ALPHA;
    debris->alpha        = (RSDK.Sin256(32 * Zone->timer) >> 3) + 192;
    debris->rotation     = self->angle << 1;
    RSDK.SetSpriteAnimation(TVVan->aniFrames, 13, &debris->animator, true, self->timer >> 1);

    self->timer = (self->timer + 1) % 22;
    player->position.x += (self->position.x - player->position.x) >> 2;
    player->position.y += (self->position.y - player->position.y) >> 2;

    if (Player_CheckValidState(player)) {
        foreach_active(TVVan, van)
        {
            if (self != van && self->nextNode != van && van->type && van->type < TVVAN_RADIO) {
                int32 rx = (van->position.x - self->position.x) >> 16;
                int32 ry = (van->position.y - self->position.y) >> 16;
                if (rx * rx + ry * ry < 0x800) {
                    self->position.x = van->position.x;
                    self->position.y = van->position.y;
                    self->nextNode   = van;
                    self->visible    = true;

                    if (van->type == TVVAN_EXITTV) {
                        RSDK.SetSpriteAnimation(TVVan->aniFrames, 14, &self->radioAnimator, true, 0);
                        RSDK.SetSpriteAnimation(TVVan->aniFrames, 16, &van->tvAnimator, true, 0);

                        switch (player->characterID) {
                            default:
                            case ID_SONIC: RSDK.SetSpriteAnimation(TVVan->aniFrames, 19, &van->playerAnimator, true, 0); break;
                            case ID_TAILS: RSDK.SetSpriteAnimation(TVVan->aniFrames, 20, &van->playerAnimator, true, 0); break;
                            case ID_KNUCKLES: RSDK.SetSpriteAnimation(TVVan->aniFrames, 21, &van->playerAnimator, true, 0); break;
#if MANIA_USE_PLUS
                            case ID_MIGHTY: RSDK.SetSpriteAnimation(TVVan->aniFrames, 22, &van->playerAnimator, true, 0); break;
                            case ID_RAY: RSDK.SetSpriteAnimation(TVVan->aniFrames, 23, &van->playerAnimator, true, 0); break;
#endif
                        }

                        van->scale.x                             = 0x000;
                        van->scale.y                             = 0x000;
                        van->stateDraw[Zone->objectDrawGroup[0]] = TVVan_Draw_ExitTV_Destroyed;
                        van->state                               = TVVan_StateTV_Exploding;
                        self->state                              = TVVan_StateRadio_EnterTV;
                        self->timer                              = 0;
                        self->direction                          = FLIP_NONE;
                    }
                    else {
                        switch (self->angle >> 5) {
                            case 1:
                                RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &self->radioAnimator, true, 0);
                                self->direction = FLIP_X;
                                break;

                            case 2:
                                RSDK.SetSpriteAnimation(TVVan->aniFrames, 11, &self->radioAnimator, true, 0);
                                self->direction = FLIP_NONE;
                                break;

                            case 3:
                                RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &self->radioAnimator, true, 0);
                                self->direction = FLIP_NONE;
                                break;

                            case 5:
                                RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &self->radioAnimator, true, 0);
                                self->direction = FLIP_Y;
                                break;

                            case 6:
                                RSDK.SetSpriteAnimation(TVVan->aniFrames, 11, &self->radioAnimator, true, 0);
                                self->direction = FLIP_Y;
                                break;

                            case 7:
                                RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &self->radioAnimator, true, 0);
                                self->direction = FLIP_XY;
                                break;

                            default: break;
                        }

                        self->targetAngles1[0] = van->targetAngles1[0];
                        self->targetAngles1[1] = van->targetAngles1[1];
                        self->targetAngles2[0] = van->targetAngles2[0];
                        self->targetAngles2[1] = van->targetAngles2[1];

                        RSDK.PlaySfx(TVVan->sfxSatellite2, false, 255);
                        self->state = TVVan_StateRadio_EnterSatellite1;
                    }

                    foreach_break;
                }
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void TVVan_StateRadio_EnterSatellite1(void)
{
    RSDK_THIS(TVVan);
    EntityPlayer *player = self->player;

    RSDK.ProcessAnimation(&self->radioAnimator);

    player = self->player;
    player->position.x += (self->position.x - player->position.x) >> 2;
    player->position.y += (self->position.y - player->position.y) >> 2;

    if (Player_CheckValidState(player)) {
        if (self->radioAnimator.frameID >= self->radioAnimator.frameCount - 1) {
            if (self->angle == self->targetAngles1[0] || self->angle == self->targetAngles1[1])
                self->angle = self->targetAngles2[Zone->timer & 1];
            else
                self->angle = self->targetAngles1[Zone->timer & 1];

            switch (self->angle >> 5) {
                case 1:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &self->radioAnimator, true, 0);
                    self->direction = FLIP_X;
                    break;

                case 2:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &self->radioAnimator, true, 0);
                    self->direction = FLIP_NONE;
                    break;

                case 3:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &self->radioAnimator, true, 0);
                    self->direction = FLIP_NONE;
                    break;

                case 5:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &self->radioAnimator, true, 0);
                    self->direction = FLIP_Y;
                    break;

                case 6:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &self->radioAnimator, true, 0);
                    self->direction = FLIP_Y;
                    break;

                case 7:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &self->radioAnimator, true, 0);
                    self->direction = FLIP_XY;
                    break;

                default: break;
            }

            self->angle = (self->angle + 0x80) & 0xFF;
            RSDK.PlaySfx(TVVan->sfxSatellite2, false, 255);
            self->state = TVVan_StateRadio_ExitSatellite2;
        }
    }
    else {
        destroyEntity(self);
    }
}

void TVVan_StateRadio_ExitSatellite2(void)
{
    RSDK_THIS(TVVan);
    EntityPlayer *player = self->player;

    RSDK.ProcessAnimation(&self->radioAnimator);

    player->position.x += (self->position.x - player->position.x) >> 2;
    player->position.y += (self->position.y - player->position.y) >> 2;
    if (Player_CheckValidState(player)) {
        if (self->radioAnimator.frameID >= self->radioAnimator.frameCount - 1) {
            int32 vel = 0;
            if (self->radioAnimator.animationID == 8)
                vel = 0x240000;

            if (self->direction & FLIP_X)
                vel = -vel;
            self->position.x += vel;

            self->position.y += (self->direction & FLIP_Y) ? 0x280000 : -0x280000;
            self->visible = false;
            self->timer   = 0;
            self->state   = TVVan_StateRadio_HandleMovement;
        }
    }
    else {
        destroyEntity(self);
    }
}

void TVVan_StateTV_Idle(void)
{
    RSDK_THIS(TVVan);

    RSDK.ProcessAnimation(&self->tvAnimator);
}

void TVVan_StateTV_Exploding(void)
{
    RSDK_THIS(TVVan);

    RSDK.ProcessAnimation(&self->tvAnimator);
    RSDK.ProcessAnimation(&self->playerAnimator);
    RSDK.ProcessAnimation(&self->tvSnowAnimator);

    if (self->scale.x >= 0x200) {
        RSDK.SetSpriteAnimation(-1, 0, &self->tvVanAnimator, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 17, &self->tvAnimator, true, 0);
        self->state                               = TVVan_StateTV_Destroyed;
        self->stateDraw[Zone->objectDrawGroup[0]] = TVVan_Draw_ExitTV_Active;
        self->active                              = ACTIVE_NORMAL;
    }
    else {
        self->scale.x += 16;
        self->scale.y = self->scale.x;
    }
}

void TVVan_StateTV_Destroyed(void)
{
    RSDK_THIS(TVVan);

    if (!RSDK.CheckOnScreen(self, NULL)) {
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &self->tvVanAnimator, true, 9);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 15, &self->tvAnimator, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 18, &self->tvSnowAnimator, true, 0);
        self->state  = TVVan_StateTV_Idle;
        self->active = ACTIVE_BOUNDS;
    }
}

void TVVan_StateRadio_EnterTV(void)
{
    RSDK_THIS(TVVan);
    EntityPlayer *player = self->player;

    player->position.x += (self->position.x - player->position.x) >> 2;
    player->position.y += (self->position.y - player->position.y) >> 2;
    if (Player_CheckValidState(player)) {
        RSDK.ProcessAnimation(&self->radioAnimator);

        if (++self->timer != 32)
            return;

        player->state = Player_State_Air;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
        player->velocity.x     = 0;
        player->animator.speed = 120;
        player->velocity.y     = -0x80000;
        player->onGround       = false;
        player->tileCollisions = TILECOLLISION_DOWN;
        player->interaction    = true;
        player->visible        = true;
        player->applyJumpCap   = false;
    }

    RSDK.PlaySfx(TVVan->sfxExplosion, false, 255);

    EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ITEMBOX), self->position.x, self->position.y - 0x100000);
    explosion->drawFX          = FX_SCALE;
    explosion->scale.x         = 0x300;
    explosion->scale.y         = 0x300;

    for (int32 i = 0; i < 12; ++i) {
        int32 x              = self->position.x + RSDK.Rand(-0xC0000, 0xC0000);
        int32 y              = self->position.y + RSDK.Rand(-0xC0000, 0xC0000);
        EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x, y);

        debris->state           = Debris_State_Fall;
        debris->gravityStrength = 0x4000;
        debris->velocity.x      = RSDK.Rand(0, 0x20000);
        if (debris->position.x < self->position.x)
            debris->velocity.x = -debris->velocity.x;
        debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
        debris->drawFX     = FX_FLIP;
        debris->direction  = i & 3;
        debris->drawGroup  = Zone->objectDrawGroup[1];
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 6, &debris->animator, true, RSDK.Rand(0, 4));
    }

    foreach_active(ShopWindow, window)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &TVVan->hitboxShopWindow, window, &window->hitboxWindowX)) {
            window->stateDraw = ShopWindow_Draw_Shattered;
            window->state     = ShopWindow_State_Shattered;
        }
    }

    destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void TVVan_EditorDraw(void)
{
    RSDK_THIS(TVVan);

    self->drawFX    = FX_FLIP;
    self->direction = FLIP_NONE;
    switch (self->type) {
        case TVVAN_SATELLITE_NE_SW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
            self->drawGroup        = Zone->objectDrawGroup[1];
            self->targetAngles1[0] = 0xE0;
            self->targetAngles1[1] = 0xE0;
            self->targetAngles2[0] = 0x60;
            self->targetAngles2[1] = 0x60;
            TVVan_Draw_Satellite_NE_SW();
            break;

        case TVVAN_SATELLITE_NW_SE:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
            self->drawGroup        = Zone->objectDrawGroup[1];
            self->targetAngles1[0] = 0x20;
            self->targetAngles1[1] = 0x20;
            self->targetAngles2[0] = 0xA0;
            self->targetAngles2[1] = 0xA0;
            TVVan_Draw_Satellite_NW_SE();
            break;

        case TVVAN_SATELLITE_NW_SW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
            self->drawGroup        = Zone->objectDrawGroup[1];
            self->targetAngles1[0] = 0x20;
            self->targetAngles1[1] = 0x20;
            self->targetAngles2[0] = 0xE0;
            self->targetAngles2[1] = 0xE0;
            TVVan_Draw_Satellite_NW_SW();
            break;

        case TVVAN_SATELLITE_NE_SE:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
            self->drawGroup        = Zone->objectDrawGroup[1];
            self->targetAngles1[0] = 0x60;
            self->targetAngles1[1] = 0x60;
            self->targetAngles2[0] = 0xA0;
            self->targetAngles2[1] = 0xA0;
            TVVan_Draw_Satellite_NE_SE();
            break;

        case TVVAN_SATELLITE_NE_NW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
            self->drawGroup        = Zone->objectDrawGroup[1];
            self->targetAngles1[0] = 0x20;
            self->targetAngles1[1] = 0x20;
            self->targetAngles2[0] = 0x60;
            self->targetAngles2[1] = 0x60;
            TVVan_Draw_Satellite_NE_NW();
            break;

        case TVVAN_SATELLITE_SE_SW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
            self->drawGroup        = Zone->objectDrawGroup[1];
            self->targetAngles1[0] = 0xE0;
            self->targetAngles1[1] = 0xE0;
            self->targetAngles2[0] = 0xA0;
            self->targetAngles2[1] = 0xA0;
            TVVan_Draw_Satellite_SE_SW();
            break;

        case TVVAN_SATELLITE_SW_N:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
            self->drawGroup        = Zone->objectDrawGroup[1];
            self->targetAngles1[0] = 0xA0;
            self->targetAngles1[1] = 0xA0;
            self->targetAngles2[0] = 0x00;
            self->targetAngles2[1] = 0x00;
            TVVan_Draw_Satellite_SW_N();
            break;

        case TVVAN_SATELLITE_NW_S:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
            self->drawGroup        = Zone->objectDrawGroup[1];
            self->targetAngles1[0] = 0x20;
            self->targetAngles1[1] = 0x20;
            self->targetAngles2[0] = 0xC0;
            self->targetAngles2[1] = 0xC0;
            TVVan_Draw_Satellite_NW_S();
            break;

        case TVVAN_SATELLITE_SE_N:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
            self->drawGroup        = Zone->objectDrawGroup[1];
            self->targetAngles1[0] = 0xA0;
            self->targetAngles1[1] = 0xA0;
            self->targetAngles2[0] = 0x40;
            self->targetAngles2[1] = 0x40;
            TVVan_Draw_Satellite_SE_N();
            break;

        case TVVAN_SATELLITE_NE_S:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->satelliteAnimator, true, 0);
            self->drawGroup        = Zone->objectDrawGroup[1];
            self->targetAngles1[0] = 0xC0;
            self->targetAngles1[1] = 0xC0;
            self->targetAngles2[0] = 0x40;
            self->targetAngles2[1] = 0x40;
            TVVan_Draw_Satellite_NE_S();
            break;

        case TVVAN_EXITTV:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &self->tvVanAnimator, true, 9);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 15, &self->tvAnimator, true, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 18, &self->tvSnowAnimator, true, 0);
            self->drawGroup = Zone->objectDrawGroup[0];
            TVVan_Draw_ExitTV_Active();
            break;

        case TVVAN_RADIO:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &self->radioAnimator, true, 0);
            self->active    = ACTIVE_NORMAL;
            self->drawGroup = Zone->objectDrawGroup[1];
            TVVan_Draw_Radio();
            break;

        default:
            self->drawGroup = Zone->objectDrawGroup[0];
            self->state     = TVVan_StateVan_Idle;
            self->direction = self->type == TVVAN_VAN_RIGHT ? FLIP_X : FLIP_NONE;
            self->vanPos    = self->position;
            self->movePos   = self->position;
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &self->satelliteAnimator, false, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 1, &self->leftDoorAnimator, false, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 2, &self->rightDoorAnimator, false, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 5, &self->windowAnimator, false, 0);

            TVVan_Draw_Van_Low();
            TVVan_Draw_Van_High();
            break;
    }
}

void TVVan_EditorLoad(void)
{
    TVVan->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/TVVan.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TVVan, type);
    RSDK_ENUM_VAR("Van (Left)", TVVAN_VAN_LEFT);
    RSDK_ENUM_VAR("Van (Right)", TVVAN_VAN_RIGHT);
    RSDK_ENUM_VAR("Satellite (NE, SW)", TVVAN_SATELLITE_NE_SW);
    RSDK_ENUM_VAR("Satellite (NW, SE)", TVVAN_SATELLITE_NW_SE);
    RSDK_ENUM_VAR("Satellite (NW, SW)", TVVAN_SATELLITE_NW_SW);
    RSDK_ENUM_VAR("Satellite (NE, SE)", TVVAN_SATELLITE_NE_SE);
    RSDK_ENUM_VAR("Satellite (NE, NE)", TVVAN_SATELLITE_NE_NW);
    RSDK_ENUM_VAR("Satellite (SE, SW)", TVVAN_SATELLITE_SE_SW);
    RSDK_ENUM_VAR("Satellite (SW, N)", TVVAN_SATELLITE_SW_N);
    RSDK_ENUM_VAR("Satellite (NW, S)", TVVAN_SATELLITE_NW_S);
    RSDK_ENUM_VAR("Satellite (SE, S)", TVVAN_SATELLITE_SE_N);
    RSDK_ENUM_VAR("Satellite (NE, S)", TVVAN_SATELLITE_NE_S);
    RSDK_ENUM_VAR("(Unused)", TVVAN_UNUSED1);
    RSDK_ENUM_VAR("(Unused)", TVVAN_UNUSED2);
    RSDK_ENUM_VAR("Exit TV", TVVAN_EXITTV);
}
#endif

void TVVan_Serialize(void) { RSDK_EDITABLE_VAR(TVVan, VAR_UINT8, type); }
