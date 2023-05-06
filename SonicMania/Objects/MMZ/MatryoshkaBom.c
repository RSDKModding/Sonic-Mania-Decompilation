// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MatryoshkaBom Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMatryoshkaBom *MatryoshkaBom;

void MatryoshkaBom_Update(void)
{
    RSDK_THIS(MatryoshkaBom);

    StateMachine_Run(self->state);
}

void MatryoshkaBom_LateUpdate(void) {}

void MatryoshkaBom_StaticUpdate(void) {}

void MatryoshkaBom_Draw(void)
{
    RSDK_THIS(MatryoshkaBom);

    if (self->state == MatryoshkaBom_State_FuseLit) {
        Vector2 drawPos;
        drawPos.x = self->position.x;
        drawPos.y = self->position.y + self->fusePos;
        RSDK.DrawSprite(&self->fuseAnimator, &drawPos, false);
    }

    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
    RSDK.DrawSprite(&self->legsAnimator, NULL, false);
}

void MatryoshkaBom_Create(void *data)
{
    RSDK_THIS(MatryoshkaBom);

    self->visible = true;

    if (self->planeFilter > 0 && ((uint8)(self->planeFilter - 1) & 2))
        self->drawGroup = Zone->objectDrawGroup[1] + 2;
    else
        self->drawGroup = Zone->objectDrawGroup[0] + 2;

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    int32 size = VOID_TO_INT(data);
    if (size < MATRYOSHKA_SIZE_SHRAPNEL) {
        self->active = ACTIVE_BOUNDS;
        if (size)
            self->size = size;

        self->startDir = self->direction;
        self->startPos = self->position;

        self->velocity.x = size ? 0xC000 : -0xC000;
        self->drawFX |= FX_FLIP;
        self->timer = 0x600;

        switch (self->size) {
            default: break;
            case MATRYOSHKA_SIZE_BIG:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &self->bodyAnimator, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 13, &self->fuseAnimator, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 2, &self->legsAnimator, true, 0);

                self->offsetX    = 0x140000;
                self->offsetY    = 0x1B0000;
                self->canExplode = true;
                break;

            case MATRYOSHKA_SIZE_MED:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &self->bodyAnimator, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 14, &self->fuseAnimator, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 6, &self->legsAnimator, true, 0);

                self->offsetX    = 0xD0000;
                self->offsetY    = 0x140000;
                self->canExplode = true;
                break;

            case MATRYOSHKA_SIZE_SMALL:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &self->bodyAnimator, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 15, &self->fuseAnimator, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 10, &self->legsAnimator, true, 0);

                self->offsetX    = 0x90000;
                self->offsetY    = 0xE0000;
                self->canExplode = false;
                break;
        }

        self->state = MatryoshkaBom_State_Init;
    }
    else {
        self->active = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 16, &self->bodyAnimator, true, 0);
        self->state = MatryoshkaBom_State_Shrapnel;
    }
}

void MatryoshkaBom_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MMZ"))
        MatryoshkaBom->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MatryoshkaBom.bin", SCOPE_STAGE);

    MatryoshkaBom->hitboxHurt.left   = -12;
    MatryoshkaBom->hitboxHurt.top    = -18;
    MatryoshkaBom->hitboxHurt.right  = 12;
    MatryoshkaBom->hitboxHurt.bottom = 18;

    MatryoshkaBom->hitboxExplode.left   = -96;
    MatryoshkaBom->hitboxExplode.top    = -96;
    MatryoshkaBom->hitboxExplode.right  = 96;
    MatryoshkaBom->hitboxExplode.bottom = 96;

    MatryoshkaBom->hitboxShrapnel.left   = -6;
    MatryoshkaBom->hitboxShrapnel.top    = -6;
    MatryoshkaBom->hitboxShrapnel.right  = 6;
    MatryoshkaBom->hitboxShrapnel.bottom = 6;

    DEBUGMODE_ADD_OBJ(MatryoshkaBom);

    MatryoshkaBom->sfxExplosion = RSDK.GetSfx("Stage/Explosion.wav");
    MatryoshkaBom->sfxButton    = RSDK.GetSfx("Stage/Button2.wav");
    MatryoshkaBom->sfxPon       = RSDK.GetSfx("Stage/Pon.wav");
}

void MatryoshkaBom_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(MatryoshkaBom, NULL, self->position.x, self->position.y);
}

void MatryoshkaBom_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void MatryoshkaBom_CheckPlayerCollisions(void)
{
    RSDK_THIS(MatryoshkaBom);

    foreach_active(Player, player)
    {
        if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
            if (self->state != MatryoshkaBom_State_Hatched) {
                if (self->canExplode) {
                    if (self->state != MatryoshkaBom_State_ReleaseSmallerBuddy) {
                        if (Player_CheckCollisionTouch(player, self, &MatryoshkaBom->hitboxExplode)) {
                            RSDK.PlaySfx(MatryoshkaBom->sfxButton, false, 255);

                            switch (self->size) {
                                default: break;
                                case MATRYOSHKA_SIZE_BIG: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 1, &self->bodyAnimator, true, 0); break;
                                case MATRYOSHKA_SIZE_MED: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 5, &self->bodyAnimator, true, 0); break;
                                case MATRYOSHKA_SIZE_SMALL: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &self->bodyAnimator, true, 0); break;
                            }

                            self->timer = 21;
                            self->state = MatryoshkaBom_State_ReleaseSmallerBuddy;
                        }
                    }
                }
                else if (self->state != MatryoshkaBom_State_FuseLit) {
                    if (Player_CheckCollisionTouch(player, self, &MatryoshkaBom->hitboxExplode)) {
                        self->timer = 144;
                        self->state = MatryoshkaBom_State_FuseLit;
                    }
                }
            }

            if (Player_CheckCollisionTouch(player, self, &MatryoshkaBom->hitboxHurt)) {
#if MANIA_USE_PLUS
                if (!Player_CheckMightyUnspin(player, 0x300, 2, &player->uncurlTimer))
#endif
                    Player_Hurt(player, self);
            }
        }
    }
}

void MatryoshkaBom_CheckOffScreen(void)
{
    RSDK_THIS(MatryoshkaBom);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        if (self->destroyOffscreen) {
            destroyEntity(self);
        }
        else {
            self->position  = self->startPos;
            self->direction = self->startDir;
            MatryoshkaBom_Create(NULL);
        }
    }
}

void MatryoshkaBom_State_Init(void)
{
    RSDK_THIS(MatryoshkaBom);

    self->active = ACTIVE_NORMAL;

    self->state = MatryoshkaBom_State_Walk;
    MatryoshkaBom_State_Walk();
}

void MatryoshkaBom_State_Walk(void)
{
    RSDK_THIS(MatryoshkaBom);

    self->position.x += self->velocity.x;

    if (!--self->timer) {
        self->timer = 60;
        self->state = MatryoshkaBom_State_Stopped;
    }
    else {
        bool32 shouldTurn = false;
        if (self->velocity.x <= 0)
            shouldTurn = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -self->offsetX, 0, true);
        else
            shouldTurn = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, self->offsetX, 0, true);

        if (!shouldTurn) {
            if (self->direction & FLIP_Y)
                shouldTurn = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, -self->offsetY, 8) == false;
            else
                shouldTurn = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, self->offsetY, 8) == false;
        }

        if (shouldTurn) {
            self->timer = 60;
            self->state = MatryoshkaBom_State_Stopped;

            switch (self->size) {
                default: break;
                case MATRYOSHKA_SIZE_BIG: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &self->bodyAnimator, true, 0); break;
                case MATRYOSHKA_SIZE_MED: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &self->bodyAnimator, true, 0); break;
                case MATRYOSHKA_SIZE_SMALL: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &self->bodyAnimator, true, 0); break;
            }
        }
    }

    RSDK.ProcessAnimation(&self->bodyAnimator);
    RSDK.ProcessAnimation(&self->legsAnimator);

    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOffScreen();
}

void MatryoshkaBom_State_Stopped(void)
{
    RSDK_THIS(MatryoshkaBom);

    if (!--self->timer) {
        switch (self->size) {
            default: break;
            case MATRYOSHKA_SIZE_BIG: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 3, &self->bodyAnimator, true, 0); break;
            case MATRYOSHKA_SIZE_MED: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 7, &self->bodyAnimator, true, 0); break;
            case MATRYOSHKA_SIZE_SMALL: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 11, &self->bodyAnimator, true, 0); break;
        }

        RSDK.SetSpriteAnimation(-1, 0, &self->legsAnimator, true, 0);
        self->state = MatryoshkaBom_State_Turn;
    }

    RSDK.ProcessAnimation(&self->bodyAnimator);

    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOffScreen();
}

void MatryoshkaBom_State_Turn(void)
{
    RSDK_THIS(MatryoshkaBom);

    RSDK.ProcessAnimation(&self->bodyAnimator);

    if (self->bodyAnimator.frameID == self->bodyAnimator.frameCount - 1) {
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
        self->timer      = 0x600;
        self->state      = MatryoshkaBom_State_Walk;

        switch (self->size) {
            default: break;
            case MATRYOSHKA_SIZE_BIG:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &self->bodyAnimator, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 2, &self->legsAnimator, true, 0);
                break;

            case MATRYOSHKA_SIZE_MED:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &self->bodyAnimator, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 6, &self->legsAnimator, true, 0);
                break;

            case MATRYOSHKA_SIZE_SMALL:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &self->bodyAnimator, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 10, &self->legsAnimator, true, 0);
                break;
        }
    }

    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOffScreen();
}

void MatryoshkaBom_State_FuseLit(void)
{
    RSDK_THIS(MatryoshkaBom);

    self->fusePos += (self->direction & FLIP_Y) ? -0x1000 : 0x1000;

    if (--self->timer > 0) {
        RSDK.ProcessAnimation(&self->bodyAnimator);
        RSDK.ProcessAnimation(&self->fuseAnimator);

        MatryoshkaBom_CheckPlayerCollisions();
        MatryoshkaBom_CheckOffScreen();
    }
    else {
        RSDK.PlaySfx(MatryoshkaBom->sfxExplosion, false, 255);

        EntityMatryoshkaBom *shrapnel = CREATE_ENTITY(MatryoshkaBom, INT_TO_VOID(MATRYOSHKA_SIZE_SHRAPNEL), self->position.x, self->position.y);
        shrapnel->velocity.x          = -0x20000;
        shrapnel->velocity.y          = -0x30000;
        shrapnel->planeFilter         = self->planeFilter;
        shrapnel->drawGroup           = self->drawGroup;

        shrapnel              = CREATE_ENTITY(MatryoshkaBom, INT_TO_VOID(MATRYOSHKA_SIZE_SHRAPNEL), self->position.x, self->position.y);
        shrapnel->velocity.x  = -0x10000;
        shrapnel->velocity.y  = -0x20000;
        shrapnel->planeFilter = self->planeFilter;
        shrapnel->drawGroup   = self->drawGroup;

        shrapnel              = CREATE_ENTITY(MatryoshkaBom, INT_TO_VOID(MATRYOSHKA_SIZE_SHRAPNEL), self->position.x, self->position.y);
        shrapnel->velocity.x  = 0x20000;
        shrapnel->velocity.y  = -0x30000;
        shrapnel->planeFilter = self->planeFilter;
        shrapnel->drawGroup   = self->drawGroup;

        shrapnel              = CREATE_ENTITY(MatryoshkaBom, INT_TO_VOID(MATRYOSHKA_SIZE_SHRAPNEL), self->position.x, self->position.y);
        shrapnel->velocity.x  = 0x10000;
        shrapnel->velocity.y  = -0x20000;
        shrapnel->planeFilter = self->planeFilter;
        shrapnel->drawGroup   = self->drawGroup;

        EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y);
        explosion->drawGroup       = self->drawGroup + 1;
        explosion->planeFilter     = self->planeFilter;

        destroyEntity(self);
    }
}

void MatryoshkaBom_State_ReleaseSmallerBuddy(void)
{
    RSDK_THIS(MatryoshkaBom);

    RSDK.ProcessAnimation(&self->bodyAnimator);

    if (!--self->timer) {
        RSDK.PlaySfx(MatryoshkaBom->sfxPon, false, 255);

        EntityMatryoshkaBom *child = CREATE_ENTITY(MatryoshkaBom, INT_TO_VOID(self->size + 1), self->position.x, self->position.y);

        child->velocity.x = self->direction == FLIP_NONE ? -0x18000 : 0x18000;
        child->velocity.y = -0x40000;

        child->planeFilter      = self->planeFilter;
        child->drawGroup        = self->drawGroup - 1;
        child->active           = ACTIVE_NORMAL;
        child->direction        = self->direction;
        child->destroyOffscreen = true;
        child->state            = MatryoshkaBom_State_Hatched;
        self->canExplode        = false;
    }

    if (self->bodyAnimator.frameID >= self->bodyAnimator.frameDuration - 1)
        self->state = MatryoshkaBom_State_Walk;

    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOffScreen();
}

void MatryoshkaBom_State_Hatched(void)
{
    RSDK_THIS(MatryoshkaBom);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.x) {
        bool32 collided = false;
        if (self->velocity.x <= 0)
            collided = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -self->offsetX, 0, true);
        else
            collided = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, self->offsetX, 0, true);

        if (collided)
            self->velocity.x = 0;
    }

    bool32 collided = false;
    if (self->direction & FLIP_Y)
        collided = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, -self->offsetY, true);
    else
        collided = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, self->offsetY, true);

    if (collided) {
        self->timer      = 0x600;
        self->velocity.x = self->direction == FLIP_NONE ? -0xC000 : 0xC000;
        self->state      = MatryoshkaBom_State_Walk;
    }

    RSDK.ProcessAnimation(&self->bodyAnimator);

    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOffScreen();
}

void MatryoshkaBom_State_Shrapnel(void)
{
    RSDK_THIS(MatryoshkaBom);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        RSDK.ProcessAnimation(&self->bodyAnimator);

        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionTouch(player, self, &MatryoshkaBom->hitboxShrapnel)) {
                    Player_Hurt(player, self);
                }
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void MatryoshkaBom_EditorDraw(void)
{
    RSDK_THIS(MatryoshkaBom);

    self->drawFX |= FX_FLIP;

    switch (self->size) {
        default: break;
        case MATRYOSHKA_SIZE_BIG:
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &self->bodyAnimator, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 13, &self->fuseAnimator, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 2, &self->legsAnimator, false, 0);
            break;

        case MATRYOSHKA_SIZE_MED:
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &self->bodyAnimator, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 14, &self->fuseAnimator, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 6, &self->legsAnimator, false, 0);
            break;

        case MATRYOSHKA_SIZE_SMALL:
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &self->bodyAnimator, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 15, &self->fuseAnimator, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 10, &self->legsAnimator, false, 0);
            break;
    }

    MatryoshkaBom_Draw();
}

void MatryoshkaBom_EditorLoad(void)
{
    MatryoshkaBom->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MatryoshkaBom.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(MatryoshkaBom, direction);
    RSDK_ENUM_VAR("Left (Rightside Up)", FLIP_NONE);
    RSDK_ENUM_VAR("Right (Rightside Up)", FLIP_X);
    RSDK_ENUM_VAR("Left (Upside down)", FLIP_Y);
    RSDK_ENUM_VAR("Right (Upside down)", FLIP_XY);

    RSDK_ACTIVE_VAR(MatryoshkaBom, planeFilter);
    RSDK_ENUM_VAR("None", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("AL", PLANEFILTER_AL);
    RSDK_ENUM_VAR("BL", PLANEFILTER_BL);
    RSDK_ENUM_VAR("AH", PLANEFILTER_AH);
    RSDK_ENUM_VAR("BH", PLANEFILTER_BH);

    RSDK_ACTIVE_VAR(MatryoshkaBom, size);
    RSDK_ENUM_VAR("Big", MATRYOSHKA_SIZE_BIG);
    RSDK_ENUM_VAR("Medium", MATRYOSHKA_SIZE_MED);
    RSDK_ENUM_VAR("Small", MATRYOSHKA_SIZE_SMALL);
}
#endif

void MatryoshkaBom_Serialize(void)
{
    RSDK_EDITABLE_VAR(MatryoshkaBom, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(MatryoshkaBom, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(MatryoshkaBom, VAR_ENUM, size);
}
