#include "SonicMania.h"

ObjectTwistedTubes *TwistedTubes;

void TwistedTubes_Update(void)
{
    RSDK_THIS(TwistedTubes);
    StateMachine_Run(self->state);
}

void TwistedTubes_LateUpdate(void) {}

void TwistedTubes_StaticUpdate(void) {}

void TwistedTubes_Draw(void)
{
    RSDK_THIS(TwistedTubes);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    drawPos.y += 0x400000;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    RSDK.DrawSprite(&self->animator3, &drawPos, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

    drawPos.y += 0x200000;
    for (int32 h = 0; h < self->height; ++h) {
        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        RSDK.DrawSprite(&self->animator4, &drawPos, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator4, &drawPos, false);

        drawPos.y += 0x200000;
    }

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    RSDK.DrawSprite(&self->animator5, &drawPos, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator5, &drawPos, false);

    self->direction = FLIP_NONE;
}

void TwistedTubes_Create(void *data)
{
    RSDK_THIS(TwistedTubes);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        if (data) {
            self->active    = ACTIVE_NORMAL;
            self->playerPtr = (EntityPlayer *)data;
            if (self->playerPtr->position.x > self->position.x)
                self->state = TwistedTubes_Unknown3;
            else
                self->state = TwistedTubes_Unknown2;
        }
        else {
            self->visible       = true;
            self->drawOrder     = Zone->drawOrderHigh;
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.y = (self->height + 4) << 21;
            self->updateRange.x = 0x800000;
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->animator2, true, 1);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->animator3, true, 2);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->animator4, true, 3);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->animator5, true, 4);
            self->state = TwistedTubes_HandleInteractions;
        }
    }
}

void TwistedTubes_StageLoad(void)
{
    TwistedTubes->aniFrames = RSDK.LoadSpriteAnimation("CPZ/TwistedTubes.bin", SCOPE_STAGE);

    TwistedTubes->hitbox1.left   = -32;
    TwistedTubes->hitbox1.top    = -16;
    TwistedTubes->hitbox1.right  = 32;
    TwistedTubes->hitbox1.bottom = 16;

    TwistedTubes->hitbox2.left   = -56;
    TwistedTubes->hitbox2.top    = 24;
    TwistedTubes->hitbox2.right  = -32;
    TwistedTubes->hitbox2.bottom = 64;

    TwistedTubes->hitbox3.left   = 32;
    TwistedTubes->hitbox3.top    = 24;
    TwistedTubes->hitbox3.right  = 56;
    TwistedTubes->hitbox3.bottom = 64;

    TwistedTubes->sfxTravel = RSDK.GetSfx("Tube/Travel.wav");
}

void TwistedTubes_HandleInteractions(void)
{
    RSDK_THIS(TwistedTubes);

    foreach_active(Player, player)
    {
        if (player->state != Player_State_TransportTube) {
            Player_CheckCollisionBox(player, self, &TwistedTubes->hitbox1);
            int32 pID = RSDK.GetEntityID(player);

            bool32 flag = false;
            if (Player_CheckCollisionTouch(player, self, &TwistedTubes->hitbox2)) {
                EntityTwistedTubes *tube = CREATE_ENTITY(TwistedTubes, player, self->position.x, self->position.y + 0x300000);
                tube->isPermanent        = true;
                tube->height             = self->height + 2;
                flag                     = true;
            }
            else if (Player_CheckCollisionTouch(player, self, &TwistedTubes->hitbox3)) {
                EntityTwistedTubes *tube = CREATE_ENTITY(TwistedTubes, player, self->position.x, self->position.y + 0x300000);
                tube->isPermanent        = true;
                tube->height             = self->height + 3;
                flag                     = true;
            }

            if (flag) {
                player->state                  = Player_State_TransportTube;
                player->drawOrder              = 1;
                player->interaction            = false;
                player->tileCollisions         = false;
                player->onGround               = false;
                player->velocity.x             = 0;
                player->velocity.y             = 0;
                player->nextAirState           = StateMachine_None;
                player->nextGroundState        = StateMachine_None;
                TwistedTubes->playerFlags[pID] = true;
                RSDK.PlaySfx(TwistedTubes->sfxTravel, 0, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                player->animator.animationSpeed = 240;
            }
        }
    }
}

void TwistedTubes_Unknown2(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = self->playerPtr;
    player->position.x += (self->position.x - player->position.x - 0x380000) >> 3;
    player->position.y += (self->position.y - player->position.y - 0x80000) >> 3;
    if (++self->timer == 8) {
        self->timer     = 0;
        self->direction = FLIP_NONE;
        self->state     = TwistedTubes_Unknown6;
    }
}

void TwistedTubes_Unknown3(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = self->playerPtr;
    player->position.x += (self->position.x - player->position.x + 0x380000) >> 3;
    player->position.y += (self->position.y - player->position.y - 0x80000) >> 3;
    if (++self->timer == 8) {
        self->timer     = 0;
        self->direction = FLIP_X;
        self->state     = TwistedTubes_Unknown4;
    }
}

void TwistedTubes_Unknown4(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = self->playerPtr;
    self->angle += 8;
    player->position.x = self->position.x + 0x3800 * RSDK.Cos256(self->angle);
    player->position.y = self->position.y - (RSDK.Cos256(self->angle) << 12);
    player->velocity.x = -0x400000;
    player->velocity.y = 0x10000;
    if (self->angle >= 128) {
        self->position.y += 0x100000;
        self->angle = 0;
        self->state = TwistedTubes_Unknown5;
    }

    if (!Player_CheckValidState(player)) {
        TwistedTubes->playerFlags[RSDK.GetEntityID(player)] = false;
        destroyEntity(self);
    }
}

void TwistedTubes_Unknown5(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = self->playerPtr;
    self->angle += 8;
    player->position.x = self->position.x - 0x3800 * RSDK.Cos256(self->angle);
    player->position.y = self->position.y - (RSDK.Cos256(self->angle) << 9);
    player->velocity.x = 0x40000;
    player->velocity.y = 0x10000;

    if (self->angle < 128) {
        if (self->angle >= 64 && self->direction == FLIP_X && self->height == 1) {
            player->position.y                                  = self->position.y;
            player->velocity.x                                  = 0xC0000;
            player->velocity.y                                  = 0;
            self->state                                       = TwistedTubes_Unknown8;
            TwistedTubes->playerFlags[RSDK.GetEntityID(player)] = false;
        }
    }
    else {
        self->angle = 0;
        if (--self->height) {
            self->position.y += 0x100000;
            self->state = TwistedTubes_Unknown4;
        }
        else {
            player->position.y = self->position.y;
            player->velocity.y = 0;
            if (self->direction == FLIP_X)
                self->state = TwistedTubes_Unknown8;
            else
                self->state = TwistedTubes_Unknown7;
            TwistedTubes->playerFlags[RSDK.GetEntityID(player)] = false;
        }
    }

    if (!Player_CheckValidState(player)) {
        TwistedTubes->playerFlags[RSDK.GetEntityID(player)] = false;
        destroyEntity(self);
    }
}

void TwistedTubes_Unknown6(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = self->playerPtr;
    self->angle += 8;
    player->position.x = self->position.x - 0x3800 * RSDK.Cos256(self->angle);
    player->position.y = self->position.y - (RSDK.Cos256(self->angle) << 12);
    player->velocity.x = 0x40000;
    player->velocity.y = 0x10000;

    if (self->angle >= 128) {
        self->position.y += 0x200000;
        self->angle = 0;
        self->state = TwistedTubes_Unknown4;
    }

    if (!Player_CheckValidState(player)) {
        TwistedTubes->playerFlags[RSDK.GetEntityID(player)] = false;
        destroyEntity(self);
    }
}

void TwistedTubes_Unknown7(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = self->playerPtr;
    player->position.y   = self->position.y;
    if (player->velocity.x <= -0x80000)
        destroyEntity(self);
    else
        player->velocity.x -= 0x8000;

    if (!Player_CheckValidState(player))
        destroyEntity(self);
}

void TwistedTubes_Unknown8(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = self->playerPtr;
    player->position.y   = self->position.y;

    if (player->velocity.x >= 0x80000)
        destroyEntity(self);
    else
        player->velocity.x += 0x8000;
    if (!Player_CheckValidState(player))
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void TwistedTubes_EditorDraw(void)
{
    RSDK_THIS(TwistedTubes);

    self->updateRange.y = (self->height + 4) << 21;
    self->updateRange.x = 0x800000;
    RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->animator2, true, 1);
    RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->animator3, true, 2);
    RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->animator4, true, 3);
    RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &self->animator5, true, 4);
    
    TwistedTubes_Draw();
}

void TwistedTubes_EditorLoad(void) { TwistedTubes->aniFrames = RSDK.LoadSpriteAnimation("CPZ/TwistedTubes.bin", SCOPE_STAGE); }
#endif

void TwistedTubes_Serialize(void) { RSDK_EDITABLE_VAR(TwistedTubes, VAR_UINT8, height); }
