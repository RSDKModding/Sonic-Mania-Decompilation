// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TVVan Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
            self->type = voidToInt(data);

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawFX        = FX_FLIP;
        switch (self->type) {
            case TVVAN_SATELLITE_NE_SW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
                self->drawOrder                      = Zone->drawOrderHigh;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Satellite_NE_SW;
                self->field_C4[0]                    = 0xE0;
                self->field_C4[1]                    = 0xE0;
                self->field_CC[0]                    = 0x60;
                self->field_CC[1]                    = 0x60;
                break;
            case TVVAN_SATELLITE_NW_SE:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
                self->drawOrder                      = Zone->drawOrderHigh;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Satellite_NW_SE;
                self->field_C4[0]                    = 0x20;
                self->field_C4[1]                    = 0x20;
                self->field_CC[0]                    = 0xA0;
                self->field_CC[1]                    = 0xA0;
                break;
            case TVVAN_SATELLITE_NW_SW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
                self->drawOrder                      = Zone->drawOrderHigh;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Satellite_NW_SW;
                self->field_C4[0]                    = 0x20;
                self->field_C4[1]                    = 0x20;
                self->field_CC[0]                    = 0xE0;
                self->field_CC[1]                    = 0xE0;
                break;
            case TVVAN_SATELLITE_NE_SE:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
                self->drawOrder                      = Zone->drawOrderHigh;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Satellite_NE_SE;
                self->field_C4[0]                    = 0x60;
                self->field_C4[1]                    = 0x60;
                self->field_CC[0]                    = 0xA0;
                self->field_CC[1]                    = 0xA0;
                break;
            case TVVAN_SATELLITE_NE_NW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
                self->drawOrder                      = Zone->drawOrderHigh;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Satellite_NE_NW;
                self->field_C4[0]                    = 0x20;
                self->field_C4[1]                    = 0x20;
                self->field_CC[0]                    = 0x60;
                self->field_CC[1]                    = 0x60;
                break;
            case TVVAN_SATELLITE_SE_SW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
                self->drawOrder                      = Zone->drawOrderHigh;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Satellite_SE_SW;
                self->field_C4[0]                    = 0xE0;
                self->field_C4[1]                    = 0xE0;
                self->field_CC[0]                    = 0xA0;
                self->field_CC[1]                    = 0xA0;
                break;
            case TVVAN_SATELLITE_SW_N:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
                self->drawOrder                      = Zone->drawOrderHigh;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Satellite_SW_N;
                self->field_C4[0]                    = 0xA0;
                self->field_C4[1]                    = 0xA0;
                self->field_CC[0]                    = 0x00;
                self->field_CC[1]                    = 0x00;
                break;
            case TVVAN_SATELLITE_NW_S:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
                self->drawOrder                      = Zone->drawOrderHigh;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Satellite_NW_S;
                self->field_C4[0]                    = 0x20;
                self->field_C4[1]                    = 0x20;
                self->field_CC[0]                    = 0xC0;
                self->field_CC[1]                    = 0xC0;
                break;
            case TVVAN_SATELLITE_SE_N:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
                self->drawOrder                      = Zone->drawOrderHigh;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Satellite_SE_N;
                self->field_C4[0]                    = 0xA0;
                self->field_C4[1]                    = 0xA0;
                self->field_CC[0]                    = 0x40;
                self->field_CC[1]                    = 0x40;
                break;
            case TVVAN_SATELLITE_NE_S:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
                self->drawOrder                      = Zone->drawOrderHigh;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Satellite_NE_S;
                self->field_C4[0]                    = 0xC0;
                self->field_C4[1]                    = 0xC0;
                self->field_CC[0]                    = 0x40;
                self->field_CC[1]                    = 0x40;
                break;
            case TVVAN_EXITTV:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &self->animator7, true, 9);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 15, &self->animator6, true, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 18, &self->animator9, true, 0);
                self->drawOrder                     = Zone->drawOrderLow;
                self->stateDraw[Zone->drawOrderLow] = TVVan_Draw_ExitTV_Active;
                self->updateRange.x                 = 0x1000000;
                self->updateRange.y                 = 0x1000000;
                self->state                         = TVVan_StateTV_Idle;
                break;
            case TVVAN_RADIO:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &self->animator5, true, 0);
                self->active                         = ACTIVE_NORMAL;
                self->visible                        = 0;
                self->drawOrder                      = Zone->drawOrderHigh;
                self->state                          = TVVan_StateRadio_StartBroadcast;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Radio;
                break;
            default:
                self->drawOrder = Zone->drawOrderLow;
                self->state     = TVVan_StateVan_Idle;
                if (self->type == TVVAN_VAN_RIGHT)
                    self->direction = FLIP_X;
                self->drawPos                        = self->position;
                self->drawPos2                       = self->position;
                self->stateDraw[Zone->drawOrderLow]  = TVVan_Draw_Van_Low;
                self->stateDraw[Zone->drawOrderHigh] = TVVan_Draw_Van_High;
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &self->animator1, false, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 1, &self->animator2, false, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 2, &self->animator3, false, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 5, &self->animator4, false, 0);
                break;
        }
    }
}

void TVVan_StageLoad(void)
{
    TVVan->aniFrames      = RSDK.LoadSpriteAnimation("SPZ1/TVVan.bin", SCOPE_STAGE);
    TVVan->hitbox3.left   = -40;
    TVVan->hitbox3.right  = 28;
    TVVan->hitbox3.top    = -28;
    TVVan->hitbox3.bottom = -20;
    TVVan->hitbox1.left   = -56;
    TVVan->hitbox1.right  = 24;
    TVVan->hitbox1.top    = 22;
    TVVan->hitbox1.bottom = 32;
    TVVan->hitbox2.left   = 20;
    TVVan->hitbox2.right  = 28;
    TVVan->hitbox2.top    = -20;
    TVVan->hitbox2.bottom = 32;
    TVVan->hitbox4.left   = 12;
    TVVan->hitbox4.right  = 20;
    TVVan->hitbox4.top    = -16;
    TVVan->hitbox4.bottom = 24;
    TVVan->hitbox5.left   = -24;
    TVVan->hitbox5.right  = 24;
    TVVan->hitbox5.top    = -24;
    TVVan->hitbox5.bottom = 24;
    TVVan->sfxDoor        = RSDK.GetSfx("Stage/Door.wav");
    TVVan->sfxSatellite1  = RSDK.GetSfx("Stage/Satellite1.wav");
    TVVan->sfxSatellite2  = RSDK.GetSfx("Stage/Satellite2.wav");
    TVVan->sfxExplosion   = RSDK.GetSfx("Stage/Explosion3.wav");
}

void TVVan_HandleVanTilt(void)
{
    RSDK_THIS(TVVan);

    if (self->field_B8 != 1) {
        if (self->field_B4 > 0)
            self->field_B4 -= 4;

        if (self->angle <= 0) {
            if (self->angle < 0)
                self->angle++;
        }
        else {
            self->angle--;
        }
    }
    else if (self->field_B4 < 64)
        self->field_B4 += 4;

    self->angle = clampVal(self->angle + self->field_BC, -8, 8);

    self->position.x = self->drawPos2.x;
    self->position.y = self->drawPos2.y;
    self->drawPos2.x = self->drawPos.x;
    self->drawPos2.y = self->drawPos.y;
    self->drawPos2.y = (RSDK.Sin256(self->field_B4) + self->drawPos2.y) & 0xFFFF0000;
    self->field_B0   = self->drawPos2.y - self->position.y;
}

void TVVan_Draw_Van_Low(void)
{
    RSDK_THIS(TVVan);

    self->drawFX            = FX_ROTATE | FX_FLIP;
    self->animator1.frameID = 1;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->drawFX            = FX_FLIP;
    self->animator1.frameID = 4;
    RSDK.DrawSprite(&self->animator1, &self->drawPos, false);

    self->drawFX |= FX_ROTATE;
    self->animator1.frameID = 2;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 8;
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator5, NULL, false);

    self->drawFX            = FX_FLIP;
    self->animator1.frameID = 3;
    RSDK.DrawSprite(&self->animator1, &self->drawPos, false);

    self->drawFX |= FX_ROTATE;
    RSDK.DrawSprite(&self->animator2, 0, false);
    RSDK.AddDrawListRef(Zone->drawOrderHigh, SceneInfo->entitySlot);
}

void TVVan_Draw_Van_High(void)
{
    RSDK_THIS(TVVan);

    self->drawFX            = FX_FLIP;
    self->animator1.frameID = 5;
    RSDK.DrawSprite(&self->animator1, &self->drawPos, false);

    self->animator1.frameID = 6;
    RSDK.DrawSprite(&self->animator1, &self->drawPos, false);

    self->animator1.frameID = 7;
    RSDK.DrawSprite(&self->animator1, &self->drawPos, false);

    self->drawFX |= FX_ROTATE;
    self->animator1.frameID = 0;
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator4, NULL, false);
}

void TVVan_Draw_Satellite_NE_SW(void)
{
    RSDK_THIS(TVVan);

    self->animator1.frameID = 0;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_XY;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void TVVan_Draw_Satellite_NW_SE(void)
{
    RSDK_THIS(TVVan);

    self->animator1.frameID = 0;
    self->direction         = FLIP_X;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_Y;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void TVVan_Draw_Satellite_NW_SW(void)
{
    RSDK_THIS(TVVan);

    self->animator1.frameID = 0;
    self->direction         = FLIP_X;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_XY;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void TVVan_Draw_Satellite_NE_SE(void)
{
    RSDK_THIS(TVVan);

    self->animator1.frameID = 0;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_Y;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void TVVan_Draw_Satellite_NE_NW(void)
{
    RSDK_THIS(TVVan);

    self->animator1.frameID = 0;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void TVVan_Draw_Satellite_SE_SW(void)
{
    RSDK_THIS(TVVan);

    self->animator1.frameID = 0;
    self->direction         = FLIP_Y;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_XY;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void TVVan_Draw_Satellite_SW_N(void)
{
    RSDK_THIS(TVVan);

    self->animator1.frameID = 1;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 0;
    self->direction         = FLIP_XY;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void TVVan_Draw_Satellite_NW_S(void)
{
    RSDK_THIS(TVVan);

    self->animator1.frameID = 1;
    self->direction         = FLIP_Y;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 0;
    self->direction         = FLIP_X;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void TVVan_Draw_Satellite_SE_N(void)
{
    RSDK_THIS(TVVan);

    self->animator1.frameID = 1;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 0;
    self->direction         = FLIP_Y;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void TVVan_Draw_Satellite_NE_S(void)
{
    RSDK_THIS(TVVan);

    self->animator1.frameID = 1;
    self->direction         = FLIP_Y;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 0;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void TVVan_Draw_Radio(void)
{
    RSDK_THIS(TVVan);

    RSDK.DrawSprite(&self->animator5, NULL, false);
}

void TVVan_Draw_ExitTV_Active(void)
{
    RSDK_THIS(TVVan);

    RSDK.DrawSprite(&self->animator7, NULL, false);
    RSDK.DrawSprite(&self->animator6, NULL, false);
}

void TVVan_Draw_ExitTV_Destroyed(void)
{
    RSDK_THIS(TVVan);
    Vector2 drawPos;

    RSDK.DrawSprite(&self->animator7, NULL, false);
    RSDK.DrawSprite(&self->animator6, NULL, false);

    self->drawFX = FX_SCALE;
    drawPos.x    = self->position.x;
    drawPos.y    = self->position.y - 0x40000;
    RSDK.DrawSprite(&self->animator8, &drawPos, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->animator9, NULL, false);
}

void TVVan_StateVan_Idle(void)
{
    RSDK_THIS(TVVan);

    TVVan_HandleVanTilt();
    self->field_B8 = 0;
    self->field_BC = 0;
    bool32 entered = false;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &TVVan->hitbox3) == C_TOP) {
            player->position.y += self->field_B0;
            player->position.y &= 0xFFFF0000;
            self->field_B8 = 1;

            int dist = player->position.x - self->position.x;
            if (self->direction == FLIP_NONE)
                dist += 0x100000;
            self->field_BC += dist >> 20;
        }
        Player_CheckCollisionBox(player, self, &TVVan->hitbox2);

        int dist = 0;
        if (self->direction)
            dist = player->position.x - self->position.x;
        else
            dist = self->position.x - player->position.x;
        TVVan->hitbox1.top = dist >> 17;

        if (TVVan->hitbox1.top >= 22) {
            if (TVVan->hitbox1.top > 32)
                TVVan->hitbox1.top = 32;
        }
        else {
            TVVan->hitbox1.top = 22;
        }

        if (Player_CheckCollisionPlatform(player, self, &TVVan->hitbox1) == C_TOP) {
            player->position.y += self->field_B0;
            player->position.y &= 0xFFFF0000;
            if (TVVan->hitbox1.top == 22)
                self->field_B8 = 1;
        }
        if (!player->sidekick) {
            if (Player_CheckCollisionTouch(player, self, &TVVan->hitbox4)) {
                player->state           = Player_State_None;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->groundVel       = 0;
                player->tileCollisions  = false;
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
    }

    self->rotation   = self->angle >> 2;
    self->position.y = self->drawPos2.y - abs(self->rotation << 15);
    if (entered) {
        self->state = TVVan_StateVan_BroadcastRadio;
        RSDK.PlaySfx(TVVan->sfxSatellite1, false, 255);
        self->animator2.speed = 1;
        self->animator3.speed = 1;
        self->animator4.speed = 1;
    }
    RSDK.ProcessAnimation(&self->animator5);
}

void TVVan_CheckPlayerCollisions(void)
{
    RSDK_THIS(TVVan);

    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, self, &TVVan->hitbox3);
        Player_CheckCollisionBox(player, self, &TVVan->hitbox2);
        self->direction ^= FLIP_X;
        Player_CheckCollisionBox(player, self, &TVVan->hitbox2);
        self->direction ^= FLIP_X;
        Player_CheckCollisionPlatform(player, self, &TVVan->hitbox1);
    }
}

void TVVan_StateVan_BroadcastRadio(void)
{
    RSDK_THIS(TVVan);

    TVVan_HandleVanTilt();
    TVVan_CheckPlayerCollisions();
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);

    if (self->animator4.frameID >= self->animator4.frameCount - 1) {
        self->state = TVVan_StateVan_FinishedBroadcast;
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 3, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 4, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 5, &self->animator4, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 7, &self->animator5, true, 0);

        if (self->direction) {
            EntityTVVan *van = CREATE_ENTITY(TVVan, intToVoid(TVVAN_RADIO), self->position.x + 0x80000, self->position.y - 0x420000);
            van->isPermanent = true;
            van->player      = self->player;
            van->angle       = 0xA0;
            van->nextNode    = (Entity *)self;
        }
        else {
            EntityTVVan *van = CREATE_ENTITY(TVVan, intToVoid(TVVAN_RADIO), self->position.x - 0x80000, self->position.y - 0x420000);
            van->isPermanent = true;
            van->player      = self->player;
            van->angle       = 0xE0;
            van->nextNode    = (Entity *)self;
        }
        self->player = NULL;
    }
}

void TVVan_StateVan_FinishedBroadcast(void)
{
    RSDK_THIS(TVVan);

    TVVan_HandleVanTilt();
    TVVan_CheckPlayerCollisions();
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator5);

    if (self->animator2.frameID >= self->animator2.frameCount - 1) {
        self->state = TVVan_StateVan_Idle;
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 2, &self->animator3, true, 0);
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
    debris->drawOrder    = self->drawOrder;
    debris->drawFX       = FX_ROTATE;
    debris->inkEffect    = INK_ALPHA;
    debris->alpha        = (RSDK.Sin256(32 * Zone->timer) >> 3) + 192;
    debris->rotation     = 2 * self->angle;
    RSDK.SetSpriteAnimation(TVVan->aniFrames, 13, &debris->animator, true, self->timer >> 1);

    self->timer = (self->timer + 1) % 22;
    player->position.x += (self->position.x - player->position.x) >> 2;
    player->position.y += (self->position.y - player->position.y) >> 2;

    if (Player_CheckValidState(player)) {
        foreach_active(TVVan, van)
        {
            if (self != van && self->nextNode != (Entity *)van) {
                if (van->type && van->type < TVVAN_RADIO) {
                    int rx = (van->position.x - self->position.x) >> 16;
                    int ry = (van->position.y - self->position.y) >> 16;
                    if (rx * rx + ry * ry < 0x800) {
                        self->position.x = van->position.x;
                        self->position.y = van->position.y;
                        self->nextNode   = (Entity *)van;
                        self->visible    = true;
                        if (van->type == TVVAN_EXITTV) {
                            RSDK.SetSpriteAnimation(TVVan->aniFrames, 14, &self->animator5, true, 0);
                            RSDK.SetSpriteAnimation(TVVan->aniFrames, 16, &van->animator6, true, 0);
                            switch (player->characterID) {
                                case ID_TAILS: RSDK.SetSpriteAnimation(TVVan->aniFrames, 20, &van->animator8, true, 0); break;
                                case ID_KNUCKLES: RSDK.SetSpriteAnimation(TVVan->aniFrames, 21, &van->animator8, true, 0); break;
#if RETRO_USE_PLUS
                                case ID_MIGHTY: RSDK.SetSpriteAnimation(TVVan->aniFrames, 22, &van->animator8, true, 0); break;
                                case ID_RAY: RSDK.SetSpriteAnimation(TVVan->aniFrames, 23, &van->animator8, true, 0); break;
#endif
                                default: RSDK.SetSpriteAnimation(TVVan->aniFrames, 19, &van->animator8, true, 0); break;
                            }
                            van->scale.x                       = 0x000;
                            van->scale.y                       = 0x000;
                            van->stateDraw[Zone->drawOrderLow] = TVVan_Draw_ExitTV_Destroyed;
                            van->state                         = TVVan_StateTV_Exploding;
                            self->state                        = TVVan_StateRadio_EnterTV;
                            self->timer                        = 0;
                            self->direction                    = FLIP_NONE;
                        }
                        else {
                            switch (self->angle >> 5) {
                                case 1:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &self->animator5, true, 0);
                                    self->direction = FLIP_X;
                                    break;
                                case 2:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 11, &self->animator5, true, 0);
                                    self->direction = FLIP_NONE;
                                    break;
                                case 3:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &self->animator5, true, 0);
                                    self->direction = FLIP_NONE;
                                    break;
                                case 5:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &self->animator5, true, 0);
                                    self->direction = FLIP_Y;
                                    break;
                                case 6:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 11, &self->animator5, true, 0);
                                    self->direction = FLIP_Y;
                                    break;
                                case 7:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &self->animator5, true, 0);
                                    self->direction = FLIP_XY;
                                    break;
                                default: break;
                            }
                            self->field_C4[0] = van->field_C4[0];
                            self->field_C4[1] = van->field_C4[1];
                            self->field_CC[0] = van->field_CC[0];
                            self->field_CC[1] = van->field_CC[1];
                            RSDK.PlaySfx(TVVan->sfxSatellite2, false, 255);
                            self->state = TVVan_StateRadio_EnterSatellite1;
                        }
                        foreach_break;
                    }
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

    RSDK.ProcessAnimation(&self->animator5);
    player = self->player;
    player->position.x += (self->position.x - player->position.x) >> 2;
    player->position.y += (self->position.y - player->position.y) >> 2;

    if (Player_CheckValidState(player)) {
        if (self->animator5.frameID >= self->animator5.frameCount - 1) {
            if (self->angle == self->field_C4[0] || self->angle == self->field_C4[1])
                self->angle = self->field_CC[Zone->timer & 1];
            else
                self->angle = self->field_C4[Zone->timer & 1];
            switch (self->angle >> 5) {
                case 1:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &self->animator5, true, 0);
                    self->direction = FLIP_X;
                    break;
                case 2:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &self->animator5, true, 0);
                    self->direction = FLIP_NONE;
                    break;
                case 3:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &self->animator5, true, 0);
                    self->direction = FLIP_NONE;
                    break;
                case 5:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &self->animator5, true, 0);
                    self->direction = FLIP_Y;
                    break;
                case 6:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &self->animator5, true, 0);
                    self->direction = FLIP_Y;
                    break;
                case 7:
                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &self->animator5, true, 0);
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

    RSDK.ProcessAnimation(&self->animator5);
    player->position.x += (self->position.x - player->position.x) >> 2;
    player->position.y += (self->position.y - player->position.y) >> 2;
    if (Player_CheckValidState(player)) {
        if (self->animator5.frameID >= self->animator5.frameCount - 1) {
            int vel = 0;
            if (self->animator5.animationID == 8)
                vel = 0x240000;
            else
                vel = 0;

            if (self->direction & FLIP_X)
                vel = -vel;
            self->position.x += vel;

            if (self->direction & FLIP_Y)
                self->position.y += 0x280000;
            else
                self->position.y += -0x280000;
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
    RSDK.ProcessAnimation(&self->animator6);
}

void TVVan_StateTV_Exploding(void)
{
    RSDK_THIS(TVVan);

    RSDK.ProcessAnimation(&self->animator6);
    RSDK.ProcessAnimation(&self->animator8);
    RSDK.ProcessAnimation(&self->animator9);

    if (self->scale.x >= 0x200) {
        RSDK.SetSpriteAnimation(-1, 0, &self->animator7, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 17, &self->animator6, true, 0);
        self->state                         = TVVan_StateTV_Destroyed;
        self->stateDraw[Zone->drawOrderLow] = TVVan_Draw_ExitTV_Active;
        self->active                        = ACTIVE_NORMAL;
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
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &self->animator7, true, 9);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 15, &self->animator6, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 18, &self->animator9, true, 0);
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
        RSDK.ProcessAnimation(&self->animator5);
        if (++self->timer != 32)
            return;
        player->state = Player_State_Air;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
        player->velocity.x     = 0;
        player->animator.speed = 120;
        player->velocity.y     = -0x80000;
        player->onGround       = false;
        player->tileCollisions = true;
        player->interaction    = true;
        player->visible        = true;
        player->jumpAbility    = 0;
    }
    RSDK.PlaySfx(TVVan->sfxExplosion, false, 255);

    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), self->position.x, self->position.y - 0x100000);
    explosion->drawFX          = FX_SCALE;
    explosion->scale.x         = 0x300;
    explosion->scale.y         = 0x300;

    for (int i = 0; i < 12; ++i) {
        int x                = self->position.x + RSDK.Rand(-0xC0000, 0xC0000);
        int y                = self->position.y + RSDK.Rand(-0xC0000, 0xC0000);
        EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x, y);
        debris->state        = Debris_State_Fall;
        debris->gravity      = 0x4000;
        debris->velocity.x   = RSDK.Rand(0, 0x20000);
        if (debris->position.x < self->position.x)
            debris->velocity.x = -debris->velocity.x;
        debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
        debris->drawFX     = FX_FLIP;
        debris->direction  = i & 3;
        debris->drawOrder  = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 6, &debris->animator, true, RSDK.Rand(0, 4));
    }

    foreach_active(ShopWindow, window)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &TVVan->hitbox5, window, &window->hitbox)) {
            window->stateDraw = ShopWindow_Draw_Shattered;
            window->state     = ShopWindow_State_Shattered;
        }
    }

    destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void TVVan_EditorDraw(void)
{
    RSDK_THIS(TVVan);

    self->drawFX = FX_FLIP;
    switch (self->type) {
        case TVVAN_SATELLITE_NE_SW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
            self->drawOrder   = Zone->drawOrderHigh;
            self->field_C4[0] = 0xE0;
            self->field_C4[1] = 0xE0;
            self->field_CC[0] = 0x60;
            self->field_CC[1] = 0x60;
            TVVan_Draw_Satellite_NE_SW();
            break;
        case TVVAN_SATELLITE_NW_SE:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
            self->drawOrder   = Zone->drawOrderHigh;
            self->field_C4[0] = 0x20;
            self->field_C4[1] = 0x20;
            self->field_CC[0] = 0xA0;
            self->field_CC[1] = 0xA0;
            TVVan_Draw_Satellite_NW_SE();
            break;
        case TVVAN_SATELLITE_NW_SW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
            self->drawOrder   = Zone->drawOrderHigh;
            self->field_C4[0] = 0x20;
            self->field_C4[1] = 0x20;
            self->field_CC[0] = 0xE0;
            self->field_CC[1] = 0xE0;
            TVVan_Draw_Satellite_NW_SW();
            break;
        case TVVAN_SATELLITE_NE_SE:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
            self->drawOrder   = Zone->drawOrderHigh;
            self->field_C4[0] = 0x60;
            self->field_C4[1] = 0x60;
            self->field_CC[0] = 0xA0;
            self->field_CC[1] = 0xA0;
            TVVan_Draw_Satellite_NE_SE();
            break;
        case TVVAN_SATELLITE_NE_NW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
            self->drawOrder   = Zone->drawOrderHigh;
            self->field_C4[0] = 0x20;
            self->field_C4[1] = 0x20;
            self->field_CC[0] = 0x60;
            self->field_CC[1] = 0x60;
            TVVan_Draw_Satellite_NE_NW();
            break;
        case TVVAN_SATELLITE_SE_SW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
            self->drawOrder   = Zone->drawOrderHigh;
            self->field_C4[0] = 0xE0;
            self->field_C4[1] = 0xE0;
            self->field_CC[0] = 0xA0;
            self->field_CC[1] = 0xA0;
            TVVan_Draw_Satellite_SE_SW();
            break;
        case TVVAN_SATELLITE_SW_N:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
            self->drawOrder   = Zone->drawOrderHigh;
            self->field_C4[0] = 0xA0;
            self->field_C4[1] = 0xA0;
            self->field_CC[0] = 0x00;
            self->field_CC[1] = 0x00;
            TVVan_Draw_Satellite_SW_N();
            break;
        case TVVAN_SATELLITE_NW_S:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
            self->drawOrder   = Zone->drawOrderHigh;
            self->field_C4[0] = 0x20;
            self->field_C4[1] = 0x20;
            self->field_CC[0] = 0xC0;
            self->field_CC[1] = 0xC0;
            TVVan_Draw_Satellite_NW_S();
            break;
        case TVVAN_SATELLITE_SE_N:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
            self->drawOrder   = Zone->drawOrderHigh;
            self->field_C4[0] = 0xA0;
            self->field_C4[1] = 0xA0;
            self->field_CC[0] = 0x40;
            self->field_CC[1] = 0x40;
            TVVan_Draw_Satellite_SE_N();
            break;
        case TVVAN_SATELLITE_NE_S:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &self->animator1, true, 0);
            self->drawOrder   = Zone->drawOrderHigh;
            self->field_C4[0] = 0xC0;
            self->field_C4[1] = 0xC0;
            self->field_CC[0] = 0x40;
            self->field_CC[1] = 0x40;
            TVVan_Draw_Satellite_NE_S();
            break;
        case TVVAN_EXITTV:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &self->animator7, true, 9);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 15, &self->animator6, true, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 18, &self->animator9, true, 0);
            self->drawOrder = Zone->drawOrderLow;
            TVVan_Draw_ExitTV_Active();
            break;
        case TVVAN_RADIO:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &self->animator5, true, 0);
            self->active    = ACTIVE_NORMAL;
            self->drawOrder = Zone->drawOrderHigh;
            TVVan_Draw_Radio();
            break;
        default:
            self->drawOrder = Zone->drawOrderLow;
            self->state     = TVVan_StateVan_Idle;
            if (self->type == TVVAN_VAN_RIGHT)
                self->direction = FLIP_X;
            self->drawPos  = self->position;
            self->drawPos2 = self->position;
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &self->animator1, false, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 1, &self->animator2, false, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 2, &self->animator3, false, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 5, &self->animator4, false, 0);

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
    RSDK_ENUM_VAR("Unused 1", TVVAN_UNUSED1);
    RSDK_ENUM_VAR("Unused 2", TVVAN_UNUSED2);
    RSDK_ENUM_VAR("Exit TV", TVVAN_EXITTV);
}
#endif

void TVVan_Serialize(void) { RSDK_EDITABLE_VAR(TVVan, VAR_UINT8, type); }
