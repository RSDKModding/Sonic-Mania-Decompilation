// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Tuesday Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectTuesday *Tuesday;

void Tuesday_Update(void)
{
    RSDK_THIS(Tuesday);
    StateMachine_Run(self->state);
}

void Tuesday_LateUpdate(void) {}

void Tuesday_StaticUpdate(void)
{
    foreach_all(Tuesday, tuesday)
    {
        if (!tuesday->velocity.y) {
            RSDK.AddDrawListRef(Zone->drawOrderLow, RSDK.GetEntityID(tuesday));
        }
    }
}

void Tuesday_Draw(void)
{
    RSDK_THIS(Tuesday);
    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        RSDK.DrawSprite(&self->nodeAnimator, &self->drawPos, false);
        if (!self->type)
            RSDK.DrawSprite(&self->gondolaAnimator, &self->drawPos, false);
    }
    else {
        Tuesday_DrawElectricity();
    }
}

void Tuesday_Create(void *data)
{
    RSDK_THIS(Tuesday);
    self->visible = true;
    self->drawFX |= FX_FLIP;
    self->drawOrder     = Zone->drawOrderLow + 1;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (data) {
        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 3, &self->nodeAnimator, true, 0);
        self->active = ACTIVE_NORMAL;
        self->state  = Tuesday_State_Debris;
    }
    else {
        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 1, &self->nodeAnimator, true, 0);
        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 0, &self->gondolaAnimator, true, 0);
        if (!SceneInfo->inEditor) {
            self->health = 1;
            if (!self->type)
                self->state = Tuesday_State_Controller;
            else
                self->state = Tuesday_State_Node;
            self->linkPtrs[0] = NULL;
            self->linkPtrs[1] = NULL;
            self->linkPtrs[2] = NULL;
            self->linkPtrs[3] = NULL;
            self->linkPtrs[4] = NULL;
            self->linkPtrs[5] = NULL;
            self->linkPtrs[6] = NULL;
            self->linkPtrs[7] = NULL;
            if (!self->type)
                self->parent = (Entity *)self;
            self->linkFlags = 0xFF;
            int32 minX      = 0x7FFFFFFF;
            int32 minY      = 0x7FFFFFFF;
            int32 maxX      = 0;
            int32 maxY      = 0;
            self->drawPos   = self->position;
            if (self->type && self->parent) {
                self->position    = self->parent->position;
                self->updateRange = self->parent->updateRange;
            }

            int32 slotID         = RSDK.GetEntityID(self->parent);
            EntityTuesday *child = RSDK_GET_ENTITY(slotID, Tuesday);
            while (child->objectID == Tuesday->objectID || child->objectID == Platform->objectID) {
                if (child != self && child->objectID == Tuesday->objectID) {
                    ++self->linkCount;
                    if (!self->type) {
                        child->parent = (Entity *)self;
                        if (child->position.x < minX)
                            minX = child->position.x;
                        if (child->position.x > maxX)
                            maxX = child->position.x;

                        if (child->position.y < minY)
                            minY = child->position.y;
                        if (child->position.y > maxY)
                            maxY = child->position.y;
                    }
                    if (!child->drawPos.x) {
                        child->drawPos.x = child->position.x;
                        child->drawPos.y = child->position.y;
                    }
                    int32 distX = (child->drawPos.x - self->drawPos.x) >> 16;
                    int32 distY = (child->drawPos.y - self->drawPos.y) >> 16;

                    if (abs(distX) <= 256 && abs(distY) <= 256) {
                        int32 dir = 0;
                        if (!distX)
                            dir |= 1; // no x
                        else if (distX >= 0)
                            dir |= 2; // right

                        if (!distY)
                            dir |= 4; // no y
                        else if (distY >= 0)
                            dir |= 8; // down

                        switch (dir) {
                            case 0: // SE
                                if ((self->links & 0x80) && abs(distX) == abs(distY)) {
                                    EntityTuesday *tuesday = (EntityTuesday *)self->linkPtrs[7];
                                    if (!tuesday || child->drawPos.y > tuesday->drawPos.y) {
                                        self->linkPtrs[7] = (Entity *)child;
                                        if (self->linkFlags)
                                            child->linkFlags |= 0x08;
                                        else if (child->linkFlags)
                                            self->linkFlags = 0x80;
                                    }
                                }
                                break;
                            case 1: // S
                                if (self->links & 0x40) {
                                    EntityTuesday *tuesday = (EntityTuesday *)self->linkPtrs[6];
                                    if (!tuesday || child->drawPos.y > tuesday->drawPos.y) {
                                        self->linkPtrs[6] = (Entity *)child;
                                        if (self->linkFlags)
                                            child->linkFlags |= 0x04;
                                        else if (child->linkFlags)
                                            self->linkFlags = 0x40;
                                    }
                                }
                                break;
                            case 2: // SW
                                if ((self->links & 0x20) && abs(distX) == abs(distY)) {
                                    EntityTuesday *tuesday = (EntityTuesday *)self->linkPtrs[5];
                                    if (!tuesday || child->drawPos.x < tuesday->drawPos.x) {
                                        self->linkPtrs[5] = (Entity *)child;
                                        if (self->linkFlags)
                                            child->linkFlags |= 0x02;
                                        else if (child->linkFlags)
                                            self->linkFlags = 0x20;
                                    }
                                }
                                break;
                            case 4: // E
                                if (self->links & 1) {
                                    EntityTuesday *tuesday = (EntityTuesday *)self->linkPtrs[0];
                                    if (!tuesday || child->drawPos.x > tuesday->drawPos.x) {
                                        self->linkPtrs[0] = (Entity *)child;
                                        if (self->linkFlags)
                                            child->linkFlags |= 0x10;
                                        else if (child->linkFlags)
                                            self->linkFlags = 1;
                                    }
                                }
                                break;
                            case 6: // W
                                if (self->links & 0x10) {
                                    EntityTuesday *tuesday = (EntityTuesday *)self->linkPtrs[4];
                                    if (!tuesday || child->drawPos.x < tuesday->drawPos.x) {
                                        self->linkPtrs[4] = (Entity *)child;
                                        if (self->linkFlags)
                                            child->linkFlags |= 1;
                                        else if (child->linkFlags)
                                            self->linkFlags = 0x10;
                                    }
                                }
                                break;
                            case 8: // NE
                                if ((self->links & 2) && abs(distX) == abs(distY)) {
                                    EntityTuesday *tuesday = (EntityTuesday *)self->linkPtrs[1];
                                    if (!tuesday || child->drawPos.x > tuesday->drawPos.x) {
                                        self->linkPtrs[1] = (Entity *)child;
                                        if (self->linkFlags)
                                            child->linkFlags |= 0x20;
                                        else if (child->linkFlags)
                                            self->linkFlags = 0x02;
                                    }
                                }
                                break;
                            case 9: // N
                                if (self->links & 4) {
                                    EntityTuesday *tuesday = (EntityTuesday *)self->linkPtrs[2];
                                    if (!tuesday || child->drawPos.y < tuesday->drawPos.y) {
                                        self->linkPtrs[2] = (Entity *)child;
                                        if (self->linkFlags)
                                            child->linkFlags |= 0x40;
                                        else if (child->linkFlags)
                                            self->linkFlags = 0x04;
                                    }
                                }
                                break;
                            case 10: // NW
                                if ((self->links & 8) && abs(distX) == abs(distY)) {
                                    EntityTuesday *tuesday = (EntityTuesday *)self->linkPtrs[3];
                                    if (!tuesday || child->drawPos.y < tuesday->drawPos.y) {
                                        self->linkPtrs[3] = (Entity *)child;
                                        if (self->linkFlags)
                                            child->linkFlags |= 0x80;
                                        else if (child->linkFlags)
                                            self->linkFlags = 0x08;
                                    }
                                }
                                break;
                            default: break;
                        }
                    }
                }
                child = RSDK_GET_ENTITY(++slotID, Tuesday);
            }

            if (!self->type) {
                self->position.x    = ((maxX - minX) >> 1) + minX;
                self->position.y    = ((maxY - minY) >> 1) + minY;
                self->updateRange.x = 0x800000 + ((maxX - minX) >> 1);
                self->updateRange.y = 0x800000 + ((maxY - minY) >> 1);
            }
        }
    }
}

void Tuesday_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        Tuesday->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Tuesday.bin", SCOPE_STAGE);

    Tuesday->hitbox1.left   = -16;
    Tuesday->hitbox1.top    = -16;
    Tuesday->hitbox1.right  = 16;
    Tuesday->hitbox1.bottom = 16;
    Tuesday->hitbox2.left   = -40;
    Tuesday->hitbox2.top    = -8;
    Tuesday->hitbox2.right  = 40;
    Tuesday->hitbox2.bottom = 80;

    Tuesday->sfxElecCharge = RSDK.GetSfx("Stage/ElecCharge.wav");
    Tuesday->sfxZap        = RSDK.GetSfx("Stage/Zap.wav");
    Tuesday->sfxBossHit    = RSDK.GetSfx("Stage/BossHit.wav");
    Tuesday->sfxExplosion  = RSDK.GetSfx("Stage/Explosion2.wav");
    Tuesday->sfxDrop       = RSDK.GetSfx("Stage/Drop.wav");
}

void Tuesday_DrawElectricity(void)
{
    RSDK_THIS(Tuesday);
    Vector2 drawPos;

    if (self->timer) {
        int32 frameID = 0;
        if (self->timer <= 60)
            frameID = (Zone->timer >> 1) & 3;
        else
            frameID = 3 * ((Zone->timer >> 1) & 1);
        self->drawFX |= FX_FLIP;

        Animator animator;
        for (int32 i = 0; i < 8; ++i) {
            if (((1 << i) & self->shockFlags) && self->linkPtrs[i]) {
                drawPos.x           = self->drawPos.x;
                drawPos.y           = self->drawPos.y;
                EntityTuesday *link = (EntityTuesday *)self->linkPtrs[i];
                switch (i) {
                    case 0:
                        drawPos.x -= 0x200000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 5, &animator, true, frameID);
                        for (int32 p = (self->drawPos.x - link->drawPos.x - 0x400000) >> 21; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x -= 0x100000;
                        }
                        break;
                    case 1:
                        drawPos.y += 0x100000;
                        drawPos.x -= 0x100000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 6, &animator, true, frameID);
                        for (int32 p = ((self->drawPos.x - link->drawPos.x - 0x180000) >> 16) / 19; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x -= 0xC0000;
                            drawPos.y += 0xC0000;
                        }
                        break;
                    case 2:
                        drawPos.y += 0x200000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 4, &animator, true, frameID);
                        for (int32 p = (link->drawPos.y - self->drawPos.y - 0x400000) >> 21; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.y += 0x100000;
                        }
                        break;
                    case 3:
                        self->direction = FLIP_X;
                        drawPos.x += 0x100000;
                        drawPos.y += 0x100000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 6, &animator, true, frameID);
                        for (int32 p = ((link->drawPos.x - self->drawPos.x - 1572864) >> 16) / 19; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x += 0xC0000;
                            drawPos.y += 0xC0000;
                        }
                        self->direction = FLIP_NONE;
                        break;
                    case 4:
                        drawPos.x += 0x200000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 5, &animator, true, frameID);
                        for (int32 p = (link->drawPos.x - self->drawPos.x - 0x400000) >> 21; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x += 0x100000;
                        }
                        break;
                    case 5:
                        drawPos.y -= 0x100000;
                        drawPos.x += 0x100000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 6, &animator, true, frameID);
                        for (int32 p = ((link->drawPos.x - self->drawPos.x - 1572864) >> 16) / 19; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x += 0xC0000;
                            drawPos.y -= 0xC0000;
                        }
                        break;
                    case 6:
                        drawPos.y -= 0x200000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 4, &animator, true, frameID);
                        for (int32 p = (self->drawPos.y - link->drawPos.y - 0x400000) >> 21; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.y -= 0x100000;
                        }
                        break;
                    case 7:
                        self->direction = FLIP_X;
                        drawPos.x -= 0x100000;
                        drawPos.y -= 0x100000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 6, &animator, true, frameID);
                        for (int32 p = ((self->drawPos.x - link->drawPos.x - 0x180000) >> 16) / 19; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x -= 0xC0000;
                            drawPos.y -= 0xC0000;
                        }
                        self->direction = FLIP_NONE;
                        break;
                    default: break;
                }
            }
        }
        self->drawFX &= ~FX_FLIP;
    }
}

void Tuesday_State_Controller(void)
{
    RSDK_THIS(Tuesday);
    if (++self->shockTimer == 240) {
        RSDK.PlaySfx(Tuesday->sfxElecCharge, false, 255);
        self->timer            = 120;
        int32 slotID           = SceneInfo->entitySlot + 1;
        EntityTuesday *tuesday = RSDK_GET_ENTITY(slotID, Tuesday);
        int32 count            = self->linkCount;
        for (int32 i = 0; i < count;) {
            if (tuesday->objectID == Tuesday->objectID) {
                ++i;
                tuesday->nextShockFlags = 0;
            }
            tuesday = RSDK_GET_ENTITY(++slotID, Tuesday);
        }

        switch (self->shockType) {
            case 0:
                self->nextShockFlags = 0b10101010;
                self->shockFlags     = 0b10101010;
                self->shockType      = 1;
                break;
            case 1:
                self->nextShockFlags = 0b00010001;
                self->shockFlags     = 0b00010001;
                self->shockType      = 2;
                break;
            case 2:
                self->nextShockFlags = 0b01000100;
                self->shockFlags     = 0b01000100;
                self->shockType      = 0;
                break;
        }

        self->shockTimer = 0;
    }
    if (self->timer == 60)
        RSDK.PlaySfx(Tuesday->sfxZap, false, 255);
    Tuesday_State_Node();
}

void Tuesday_State_Node(void)
{
    RSDK_THIS(Tuesday);
    EntityPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, Platform);
    if (platform->objectID == Platform->objectID && platform->childCount > 0) {
        self->position.x -= platform->collisionOffset.x;
        self->position.y -= platform->collisionOffset.y;
        self->drawPos.x += platform->collisionOffset.x;
        self->drawPos.y += platform->collisionOffset.y;
    }

    if (self->timer) {
        if (self->timer <= 60)
            RSDK.SetSpriteAnimation(Tuesday->aniFrames, 2, &self->nodeAnimator, false, 0);
        else
            RSDK.SetSpriteAnimation(Tuesday->aniFrames, 1, &self->nodeAnimator, false, 0);
        RSDK.ProcessAnimation(&self->nodeAnimator);
    }
    else {
        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 1, &self->nodeAnimator, true, 0);
    }

    int32 storeX   = self->position.x;
    int32 storeY   = self->position.y;
    self->position = self->drawPos;
    Hitbox hitbox;

    foreach_active(Player, player)
    {
        if (!self->invincibleTimer) {
            bool32 hit = false;
            if (self->type)
                hit = Player_CheckBadnikTouch(player, self, &Tuesday->hitbox1);
            else
                hit = Player_CheckBadnikTouch(player, self, &Tuesday->hitbox2);
            if (hit) {
                if (!player->invincibleTimer && player->shield != SHIELD_LIGHTNING && player->blinkTimer <= 0 && self->timer && self->timer <= 60
                    && (self->type || Player_CheckCollisionTouch(player, self, &Tuesday->hitbox1))) {
                    Player_CheckHit(player, self);
                }
                else if (Player_CheckBossHit(player, self)) {
                    if (!--self->health) {
                        self->state = Tuesday_State_Destroyed;
                        if (!self->type)
                            self->invincibleTimer = 48;
                        else
                            self->invincibleTimer = 16;
                    }
                    else {
                        self->invincibleTimer = 30;
                        RSDK.PlaySfx(Tuesday->sfxBossHit, false, 255);
                    }
                }
            }
            else {
                if (self->timer < 60) {
                    if (self->shockFlags == 17) {
                        EntityTuesday *child = (EntityTuesday *)self->linkPtrs[0];
                        if (child) {
                            hitbox.top    = -2;
                            hitbox.bottom = 2;
                            hitbox.left   = (child->drawPos.x - self->position.x) >> 16;
                            hitbox.right  = 0;
                            if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                                Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
                            }
                        }

                        child = (EntityTuesday *)self->linkPtrs[4];
                        if (child) {
                            hitbox.left   = -0;
                            hitbox.top    = -2;
                            hitbox.bottom = 2;
                            hitbox.right  = (child->drawPos.x - self->position.x) >> 16;
                            if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                                Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
                            }
                        }
                    }
                    else {
                        if (self->shockFlags != 68) {
                            if (self->shockFlags == 170) {
                                EntityTuesday *child = (EntityTuesday *)self->linkPtrs[1];
                                if (child) {
                                    hitbox.top    = 0;
                                    hitbox.bottom = (child->drawPos.y - self->position.y) >> 16;
                                    hitbox.left   = (child->drawPos.x - self->position.x) >> 16;
                                    hitbox.right  = 0;
                                    if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                                        int32 distX = abs((player->position.x - self->position.x) >> 16);
                                        int32 distY = abs((player->position.y - self->position.y) >> 16);
                                        if (abs(distX - distY) < 13) {
                                            Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
                                        }
                                    }
                                }
                                else {
                                    child = (EntityTuesday *)self->linkPtrs[3];
                                    if (child) {
                                        hitbox.bottom = (child->drawPos.y - self->position.y) >> 16;
                                        hitbox.left   = 0;
                                        hitbox.top    = 0;
                                        hitbox.right  = (child->drawPos.x - self->position.x) >> 16;
                                        if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                                            int32 distX = abs((player->position.x - self->position.x) >> 16);
                                            int32 distY = abs((player->position.y - self->position.y) >> 16);
                                            if (abs(distX - distY) < 13) {
                                                Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
                                            }
                                        }
                                    }
                                    else {
                                        child = (EntityTuesday *)self->linkPtrs[5];
                                        if (child) {
                                            hitbox.top    = (child->drawPos.y - self->position.y) >> 16;
                                            hitbox.bottom = 0;
                                            hitbox.left   = 0;
                                            hitbox.right  = (child->drawPos.x - self->position.x) >> 16;
                                            if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                                                int32 distX = abs((player->position.x - self->position.x) >> 16);
                                                int32 distY = abs((player->position.y - self->position.y) >> 16);
                                                if (abs(distX - distY) < 13) {
                                                    Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
                                                }
                                            }
                                        }
                                        else {
                                            child = (EntityTuesday *)self->linkPtrs[7];
                                            if (child) {
                                                hitbox.top    = (child->drawPos.y - self->position.y) >> 16;
                                                hitbox.bottom = 0;
                                                hitbox.left   = (child->drawPos.x - self->position.x) >> 16;
                                                hitbox.right  = 0;
                                                if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                                                    int32 distX = abs((player->position.x - self->position.x) >> 16);
                                                    int32 distY = abs((player->position.y - self->position.y) >> 16);
                                                    if (abs(distX - distY) < 13) {
                                                        Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            EntityTuesday *child = (EntityTuesday *)self->linkPtrs[2];
                            if (child) {
                                hitbox.left   = 2;
                                hitbox.top    = -2;
                                hitbox.right  = 2;
                                hitbox.bottom = (child->drawPos.y - self->position.y) >> 16;
                                if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                                    Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
                                }
                            }

                            child = (EntityTuesday *)self->linkPtrs[6];
                            if (child) {
                                hitbox.left   = -2;
                                hitbox.right  = 2;
                                hitbox.top    = (child->drawPos.y - self->position.y) >> 16;
                                hitbox.bottom = 0;
                                if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                                    Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
    if (self->nextShockFlags != 0b11111111 && self->nextShockFlags) {
        for (int32 i = 0; i < 8; ++i) {
            EntityTuesday *child = (EntityTuesday *)self->linkPtrs[i];
            if (child) {
                if (!child->nextShockFlags)
                    child->nextShockFlags = self->nextShockFlags;

                if (self->nextShockFlags == 0b00010001) {
                    if (!i || i == 4) {
                        child->timer      = 120;
                        self->timer       = 120;
                        child->shockFlags = 0b00010001;
                        self->shockFlags  = 0b00010001;
                    }
                }
                else if (self->nextShockFlags == 0b01000100) {
                    if (i == 2 || i == 6) {
                        child->timer      = 120;
                        self->timer       = 120;
                        child->shockFlags = 0b01000100;
                        self->shockFlags  = 0b01000100;
                    }
                }
                else if (self->nextShockFlags == 0b10101010 && (i == 1 || i == 3 || i == 5 || i == 7)) {
                    child->timer      = 120;
                    self->timer       = 120;
                    child->shockFlags = 0b10101010;
                    self->shockFlags  = 0b10101010;
                }
            }
        }
        self->nextShockFlags = 0b11111111;
    }

    if (self->timer) {
        if (!--self->timer)
            self->shockFlags = 0b00000000;
    }
}

void Tuesday_State_Destroyed(void)
{
    RSDK_THIS(Tuesday);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(Tuesday->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 data = ((RSDK.Rand(0, 256) > 192) + 2);

            int32 val    = 0;
            int32 minVal = 0, maxVal = 0;
            if (self->type) {
                val    = RSDK.Rand(-12, 12);
                maxVal = 12;
                minVal = -12;
            }
            else {
                val    = RSDK.Rand(-8, 73);
                maxVal = 33;
                minVal = -32;
            }
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid(data), (RSDK.Rand(minVal, maxVal) << 16) + self->drawPos.x, (val << 16) + self->drawPos.y);
            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (!--self->invincibleTimer) {
        --((EntityTuesday *)self->parent)->linkCount;

        EntityTuesday *child = (EntityTuesday *)self->linkPtrs[0];
        if (child) {
            child->linkPtrs[4] = NULL;
            child->linkFlags &= ~0x10;
        }

        child = (EntityTuesday *)self->linkPtrs[1];
        if (child) {
            child->linkPtrs[5] = NULL;
            child->linkFlags &= ~0x20;
        }

        child = (EntityTuesday *)self->linkPtrs[2];
        if (child) {
            child->linkPtrs[6] = NULL;
            child->linkFlags &= ~0x40;
        }

        child = (EntityTuesday *)self->linkPtrs[3];
        if (child) {
            child->linkPtrs[7] = NULL;
            child->linkFlags &= ~0x80;
        }

        child = (EntityTuesday *)self->linkPtrs[4];
        if (child) {
            child->linkPtrs[0] = NULL;
            child->linkFlags &= ~1;
        }

        child = (EntityTuesday *)self->linkPtrs[5];
        if (child) {
            child->linkPtrs[1] = NULL;
            child->linkFlags &= ~2;
        }

        child = (EntityTuesday *)self->linkPtrs[6];
        if (child) {
            child->linkPtrs[2] = NULL;
            child->linkFlags &= ~4;
        }

        child = (EntityTuesday *)self->linkPtrs[7];
        if (child) {
            child->linkPtrs[3] = NULL;
            child->linkFlags &= ~8;
        }

        child                       = CREATE_ENTITY(Tuesday, intToVoid(true), self->drawPos.x, self->drawPos.y);
        child->drawPos.x            = child->position.x;
        child->drawPos.y            = child->position.y;
        child->nodeAnimator.frameID = 0;
        child->velocity.x           = -0x20000;
        child->velocity.y           = -0x40000;
        child->type                 = 1;

        child                       = CREATE_ENTITY(Tuesday, intToVoid(true), self->drawPos.x, self->drawPos.y);
        child->drawPos.x            = child->position.x;
        child->drawPos.y            = child->position.y;
        child->nodeAnimator.frameID = 1;
        child->velocity.x           = 0x20000;
        child->velocity.y           = -0x40000;
        child->type                 = 1;

        child                       = CREATE_ENTITY(Tuesday, intToVoid(true), self->drawPos.x, self->drawPos.y);
        child->drawPos.x            = child->position.x;
        child->drawPos.y            = child->position.y;
        child->nodeAnimator.frameID = 2;
        child->velocity.x           = -0x10000;
        child->velocity.y           = -0x20000;
        child->type                 = 1;

        child                       = CREATE_ENTITY(Tuesday, intToVoid(true), self->drawPos.x, self->drawPos.y);
        child->drawPos.x            = child->position.x;
        child->drawPos.y            = child->position.y;
        child->nodeAnimator.frameID = 3;
        child->velocity.x           = 0x10000;
        child->velocity.y           = -0x20000;
        child->type                 = 1;

        if (!self->type) {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->gondolaAnimator, true, 0);
            child = CREATE_ENTITY(Tuesday, intToVoid(true), self->drawPos.x, self->drawPos.y);
            RSDK.SetSpriteAnimation(Tuesday->aniFrames, 0, &child->nodeAnimator, true, 0);
            child->drawPos.x  = child->position.x;
            child->drawPos.y  = child->position.y;
            child->velocity.y = -0x30000;
            child->state      = Tuesday_State_Debris;
        }

        self->linkPtrs[4] = NULL;
        self->linkPtrs[5] = NULL;
        self->linkPtrs[6] = NULL;
        self->linkPtrs[7] = NULL;
        self->linkPtrs[0] = NULL;
        self->linkPtrs[1] = NULL;
        self->linkPtrs[2] = NULL;
        self->linkPtrs[3] = NULL;
        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 3, &self->nodeAnimator, true, 4);
        self->state = StateMachine_None;
    }
}

void Tuesday_State_Debris(void)
{
    RSDK_THIS(Tuesday);
    self->drawPos.x += self->velocity.x;
    self->drawPos.y += self->velocity.y;
    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    self->velocity.y += 0x3800;

    if (RSDK.CheckOnScreen(self, NULL)) {
        if (self->type) {
            self->visible ^= true;
        }
        else if (self->velocity.y == 0x8000) {
            RSDK.PlaySfx(Tuesday->sfxDrop, false, 255);
        }
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Tuesday_EditorDraw(void)
{
    RSDK_THIS(Tuesday);
    self->drawPos = self->position;

    RSDK.DrawSprite(&self->nodeAnimator, &self->drawPos, false);
    if (!self->type)
        RSDK.DrawSprite(&self->gondolaAnimator, &self->drawPos, false);

    if (showGizmos()) {
        // connection area
        RSDK.DrawRect(self->position.x - 0x1000000, self->position.y - 0x1000000, 0x2000000, 0x2000000, 0xFFFF00, 0x10, INK_ALPHA, false);

        // active links
        if (self->links & 0x80) {
            DrawHelpers_DrawArrow(0x00FF00, self->position.x, self->position.y, self->position.x + 0x18 * (RSDK.Cos256(0xA0) << 9),
                                  self->position.y + 0x18 * (RSDK.Sin256(0xA0) << 9));
        }
        if (self->links & 0x40) {
            DrawHelpers_DrawArrow(0x00FF00, self->position.x, self->position.y, self->position.x, self->position.y - 0x300000);
        }
        if (self->links & 0x20) {
            DrawHelpers_DrawArrow(0x00FF00, self->position.x, self->position.y, self->position.x + 0x18 * (RSDK.Cos256(0xE0) << 9),
                                  self->position.y + 0x18 * (RSDK.Sin256(0xE0) << 9));
        }
        if (self->links & 0x1) {
            DrawHelpers_DrawArrow(0x00FF00, self->position.x, self->position.y, self->position.x - 0x300000, self->position.y);
        }
        if (self->links & 0x10) {
            DrawHelpers_DrawArrow(0x00FF00, self->position.x, self->position.y, self->position.x + 0x300000, self->position.y);
        }
        if (self->links & 0x2) {
            DrawHelpers_DrawArrow(0x00FF00, self->position.x, self->position.y, self->position.x + 0x18 * (RSDK.Cos256(0x60) << 9),
                                  self->position.y + 0x18 * (RSDK.Sin256(0x60) << 9));
        }
        if (self->links & 0x4) {
            DrawHelpers_DrawArrow(0x00FF00, self->position.x, self->position.y, self->position.x, self->position.y + 0x300000);
        }
        if (self->links & 0x8) {
            DrawHelpers_DrawArrow(0x00FF00, self->position.x, self->position.y, self->position.x + 0x18 * (RSDK.Cos256(0x20) << 9),
                                  self->position.y + 0x18 * (RSDK.Sin256(0x20) << 9));
        }
    }
}

void Tuesday_EditorLoad(void)
{
    Tuesday->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Tuesday.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Tuesday, type);
    RSDK_ENUM_VAR("Gondola", TUESDAY_GONDOLA);
    RSDK_ENUM_VAR("Node", TUESDAY_NODE);
}
#endif

void Tuesday_Serialize(void)
{
    RSDK_EDITABLE_VAR(Tuesday, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Tuesday, VAR_UINT8, links);
}
