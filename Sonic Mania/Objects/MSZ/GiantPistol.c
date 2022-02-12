// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GiantPistol Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectGiantPistol *GiantPistol;

void GiantPistol_Update(void)
{
    RSDK_THIS(GiantPistol);
    StateMachine_Run(self->state);
}

void GiantPistol_LateUpdate(void) {}

void GiantPistol_StaticUpdate(void) {}

void GiantPistol_Draw(void)
{
    RSDK_THIS(GiantPistol);
    if (SceneInfo->currentDrawGroup == Zone->drawOrderLow) {
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator5, NULL, false);
        RSDK.AddDrawListRef(Zone->drawOrderHigh, SceneInfo->entitySlot);
    }
    else {
        RSDK.DrawSprite(&self->animator7, NULL, false);
        RSDK.DrawSprite(&self->animator4, NULL, false);
        RSDK.DrawSprite(&self->animator3, NULL, false);
        RSDK.DrawSprite(&self->animator1, NULL, false);
        RSDK.DrawSprite(&self->animator6, NULL, false);
        RSDK.DrawSprite(&self->animator8, NULL, false);
    }
}

void GiantPistol_Create(void *data)
{
    RSDK_THIS(GiantPistol);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &self->animator2, true, 1);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 5, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 6, &self->animator4, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 4, &self->animator7, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 1, &self->animator5, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 2, &self->animator6, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 3, &self->animator8, true, 0);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->startPos      = self->position;
        if (self->direction == FLIP_NONE)
            self->field_74.x = self->position.x + 0x300000;
        else
            self->field_74.x = self->position.x - 0x300000;
        self->field_74.y = self->position.y + 0x180000;
        self->state      = GiantPistol_State_WaitForPlayer;
    }
}

void GiantPistol_StageLoad(void)
{
    GiantPistol->aniFrames      = RSDK.LoadSpriteAnimation("MSZ/Pistol.bin", SCOPE_STAGE);
    GiantPistol->hitbox1.left   = -80;
    GiantPistol->hitbox1.top    = -58;
    GiantPistol->hitbox1.right  = 4;
    GiantPistol->hitbox1.bottom = -16;
    GiantPistol->hitbox2.left   = 4;
    GiantPistol->hitbox2.top    = -26;
    GiantPistol->hitbox2.right  = 44;
    GiantPistol->hitbox2.bottom = -16;
    GiantPistol->hitbox3.left   = 44;
    GiantPistol->hitbox3.top    = -58;
    GiantPistol->hitbox3.right  = 64;
    GiantPistol->hitbox3.bottom = -16;
    GiantPistol->sfxClack       = RSDK.GetSfx("Stage/Clack.wav");
    GiantPistol->sfxCannonFire  = RSDK.GetSfx("Stage/CannonFire.wav");
}

void GiantPistol_State_WaitForPlayer(void)
{
    RSDK_THIS(GiantPistol);

    foreach_active(Player, player)
    {
        int side = Player_CheckCollisionBox(player, self, &GiantPistol->hitbox2);
        if (side == C_TOP && (!player->sidekick || self->state == GiantPistol_State_Unknown2)) {
            player->state           = Player_State_None;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->velocity.x      = 0;
            player->velocity.y      = 0;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            self->activePlayers |= (1 << player->playerID);
            self->state = GiantPistol_State_Unknown2;
            if (self->direction == FLIP_NONE)
                self->field_64 = 0x180000;
            else
                self->field_64 = -0x180000;
            RSDK.PlaySfx(GiantPistol->sfxClack, false, 255);
        }
        Player_CheckCollisionBox(player, self, &GiantPistol->hitbox1);
        Player_CheckCollisionBox(player, self, &GiantPistol->hitbox3);
    }
}

void GiantPistol_State_Unknown2(void)
{
    RSDK_THIS(GiantPistol);
    RSDK.ProcessAnimation(&self->animator3);

    foreach_active(Player, player)
    {
        if (((1 << (player->playerID)) & self->activePlayers) > 0) {
            player->position.x += (self->position.x + self->field_64 - player->position.x) >> 3;
            player->position.y += (self->position.y - player->position.y - 0x200000) >> 3;
            player->velocity.x = 0;
            player->velocity.y = 0;
        }
    }

    if (self->animator3.frameID >= self->animator3.frameCount - 1) {
        self->state             = GiantPistol_State_Unknown3;
        self->drawFX            = FX_ROTATE | FX_FLIP;
        self->animator5.frameID = 1;
        self->animator6.frameID = 1;
        self->velocity.y        = -0x20000;
        self->timer             = 6;

        foreach_active(Player, player)
        {
            if (((1 << (player->playerID)) & self->activePlayers) > 0) {
                player->blinkTimer = 0;
                player->visible    = false;
                if (player->camera)
                    player->camera->state = StateMachine_None;
            }
        }
    }
}

void GiantPistol_State_Unknown3(void)
{
    RSDK_THIS(GiantPistol);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    if (self->position.y >= self->startPos.y) {
        self->position.y = self->startPos.y;
        self->velocity.y = 0;
    }

    if (self->timer > -64)
        self->timer--;

    if (self->direction)
        self->angle -= self->timer;
    else
        self->angle += self->timer;

    self->rotation = self->angle & 0x1FF;
    int angle        = 256 - (self->rotation >> 1);

    foreach_active(Player, player)
    {
        if (((1 << (player->playerID)) & self->activePlayers) > 0) {
            player->position.x = self->position.x + self->field_64;
            player->position.y = self->position.y - 0x200000;
            int distX          = (player->position.x - self->position.x) >> 8;
            int distY          = (player->position.y - self->position.y) >> 8;
            player->position.x = self->position.x + distY * RSDK.Sin256(angle) + distX * RSDK.Cos256(angle);
            player->position.y = (self->position.y - distX * RSDK.Sin256(angle)) + distY * RSDK.Cos256(angle);
        }
    }

    if (self->angle <= -0x1FC || self->angle >= 0x1FC) {
        self->angle    = 0;
        self->timer    = -(self->timer >> 1);
        self->rotation = 0;
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 7, &self->animator5, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 7, &self->animator6, true, 1);
        self->state  = GiantPistol_State_Aiming;
        self->active = ACTIVE_NORMAL;

        foreach_active(Player, player)
        {
            if (((1 << (player->playerID)) & self->activePlayers) > 0) {
                if (player->camera)
                    player->camera->state = Camera_State_Follow;
            }
        }
    }
}

void GiantPistol_State_Aiming(void)
{
    RSDK_THIS(GiantPistol);

    if (self->timer2) {
        self->timer2--;
    }
    else {
        if (self->timer > 4)
            self->timer--;

        if (self->direction)
            self->angle -= self->timer;
        else
            self->angle += self->timer;
        self->angle &= 0x1FF;
    }

    self->position = self->startPos;
    self->rotation = (-RSDK.Sin512(self->angle) >> 3) & 0x1FF;
    int angle        = 256 - (self->rotation >> 1);
    Zone_RotateOnPivot(&self->position, &self->field_74, angle);

    self->position.x -= RSDK.Sin512(self->rotation) << 10;
    self->position.y -= RSDK.Sin512(self->rotation) << 10;

    if (self->direction) {
        if (self->field_64 <= 0x500000)
            self->field_64 += 0x40000;
    }
    else {
        if (self->field_64 > -0x500000)
            self->field_64 -= 0x40000;
    }

    uint8 flags = 0;
#if RETRO_USE_PLUS
    if (GiantPistol->flag) {
#else 
    if (!Zone->actID && !checkPlayerID(ID_KNUCKLES, 1)) {
#endif
        if (self->angle == 118 && self->activePlayers > 0) {
#if RETRO_USE_PLUS
            if (SceneInfo->filter & FILTER_ENCORE) {
                MSZSetup_Unknown3();
                MSZSetup_Unknown4(0);
            }
            else
#endif
                MSZSetup_Unknown4(1024);
            flags = 1;
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (((1 << (player->playerID)) & self->activePlayers) > 0)
                flags |= player->jumpPress;
        }
    }

    foreach_active(Player, player)
    {
        if (((1 << (player->playerID)) & self->activePlayers) > 0) {
            player->position.x = self->position.x + self->field_64;
            player->position.y = self->position.y - 0x200000;
            int distX          = (player->position.x - self->position.x) >> 8;
            int distY          = (player->position.y - self->position.y) >> 8;

            player->position.x = self->position.x + distY * RSDK.Sin256(angle) + distX * RSDK.Cos256(angle);
            player->position.y = self->position.y - distX * RSDK.Sin256(angle) + distY * RSDK.Cos256(angle);
            if (flags == 1) {
                player->state            = Player_State_Air;
                player->onGround         = false;
                player->jumpAbilityState = 0;
                player->applyJumpCap     = false;
                player->jumpPress        = false;
                player->jumpHold         = false;

                if (self->direction) {
                    player->velocity.x = 0xC00 * RSDK.Cos512(self->rotation);
                    player->velocity.y = 0xC00 * RSDK.Sin512(self->rotation);
                }
                else {
                    player->velocity.x = -0xC00 * RSDK.Cos512(self->rotation);
                    player->velocity.y = -0xC00 * RSDK.Sin512(self->rotation);
                }

#if RETRO_USE_PLUS
                if (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE) && GiantPistol->flag) {
                    player->velocity.x += 0x18000;
                    player->state           = GiantPistol_Player_State_Unknown1;
                    player->nextGroundState = GiantPistol_Player_State_Unknown2;
                }
                else
#endif
                    if (!Zone->actID && !checkPlayerID(ID_KNUCKLES, 1)) {
                    player->jumpAbilityState = 0;
                }
                self->activePlayers &= ~(1 << player->playerID);
                self->timer2 = 16;

                player->velocity.x = clampVal(player->velocity.x, -0x120000, 0x120000);
                player->visible    = true;
                Camera_ShakeScreen(player->playerID, -8, -8);
                RSDK.PlaySfx(GiantPistol->sfxCannonFire, false, 255);
            }
        }
    }

    if (!self->activePlayers && !self->timer2) {
        if (self->rotation > 256)
            self->rotation -= 0x200;
        self->state = GiantPistol_State_FiredPlayers;
    }
}

void GiantPistol_State_FiredPlayers(void)
{
    RSDK_THIS(GiantPistol);

    if (self->rotation <= 0) {
        self->rotation += 4;
        if (self->rotation > 0)
            self->rotation = 0;
    }
    else {
        self->rotation -= 4;
        if (self->rotation < 0)
            self->rotation = 0;
    }

    if (self->animator3.frameID > 0)
        self->animator3.frameID--;

    ++self->timer2;
    self->position.x += (self->startPos.x - self->position.x) >> 3;
    self->position.y += (self->startPos.y - self->position.y) >> 3;
    if (self->timer2 == 60) {
        self->position = self->startPos;
        self->timer2   = 0;
        self->angle    = 0;
        self->drawFX   = FX_FLIP;
        self->active   = ACTIVE_BOUNDS;
        self->state    = GiantPistol_State_WaitForPlayer;
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 5, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 1, &self->animator5, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 2, &self->animator6, true, 0);
    }
}

void GiantPistol_Player_State_Unknown1(void)
{
    RSDK_THIS(Player);
    self->left      = false;
    self->right     = true;
    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;
    Player_State_Air();
    self->nextGroundState = GiantPistol_Player_State_Unknown2;
}

void GiantPistol_Player_State_Unknown2(void)
{
    RSDK_THIS(Player);
    self->left      = true;
    self->right     = false;
    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;
    Player_State_Ground();
    self->state           = GiantPistol_Player_State_Unknown2;
    self->nextGroundState = GiantPistol_Player_State_Unknown2;
    if (self->groundVel <= 0) {
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_IDLE, &self->animator, true, 0);
        self->left            = false;
        self->skidding        = 0;
        self->groundVel       = 0;
        self->velocity.x      = 0;
        self->direction       = FLIP_NONE;
        self->state           = Player_State_Ground;
        self->nextGroundState = Player_State_Ground;
    }
}

void GiantPistol_EditorDraw(void)
{
    RSDK_THIS(GiantPistol);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &self->animator2, true, 1);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 5, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 6, &self->animator4, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 4, &self->animator7, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 1, &self->animator5, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 2, &self->animator6, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 3, &self->animator8, true, 0);

    RSDK.DrawSprite(&self->animator7, NULL, false);
    RSDK.DrawSprite(&self->animator4, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator6, NULL, false);
    RSDK.DrawSprite(&self->animator8, NULL, false);
}

void GiantPistol_EditorLoad(void) { GiantPistol->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Pistol.bin", SCOPE_STAGE); }

void GiantPistol_Serialize(void) { RSDK_EDITABLE_VAR(GiantPistol, VAR_UINT8, direction); }
