// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MatryoshkaBom Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    if (self->state == MatryoshkaBom_Unknown8) {
        Vector2 drawPos;
        drawPos.x = self->position.x;
        drawPos.y = self->field_84 + self->position.y;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
    }
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

void MatryoshkaBom_Create(void *data)
{
    RSDK_THIS(MatryoshkaBom);

    self->visible = true;
    if (self->planeFilter > 0 && ((uint8)(self->planeFilter - 1) & 2))
        self->drawOrder = Zone->drawOrderHigh + 2;
    else
        self->drawOrder = Zone->drawOrderLow + 2;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    int32 size = voidToInt(data);
    if (size < MATRYOSHKA_SIZE_SHRAPNEL) {
        self->active = ACTIVE_BOUNDS;
        if (size)
            self->size = size;
        self->startDir = self->direction;
        self->startPos = self->position;

        if (!size)
            self->velocity.x = -0xC000;
        else
            self->velocity.x = 0xC000;
        self->drawFX |= FX_FLIP;
        self->timer = 0x600;

        switch (self->size) {
            default: break;
            case MATRYOSHKA_SIZE_BIG:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 13, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 2, &self->animator3, true, 0);
                self->offsetY    = 0x1B0000;
                self->offsetX    = 0x140000;
                self->canExplode = true;
                break;
            case MATRYOSHKA_SIZE_MED:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 14, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 6, &self->animator3, true, 0);
                self->offsetY    = 0x140000;
                self->offsetX    = 0xD0000;
                self->canExplode = true;
                break;
            case MATRYOSHKA_SIZE_SMALL:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 15, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 10, &self->animator3, true, 0);
                self->canExplode = false;
                self->offsetY    = 0xE0000;
                self->offsetX    = 0x90000;
                break;
        }
        self->state = MatryoshkaBom_State_Setup;
    }
    else {
        self->active = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 16, &self->animator1, true, 0);
        self->state = MatryoshkaBom_Unknown11;
    }
}

void MatryoshkaBom_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
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
            if (self->state != MatryoshkaBom_Unknown10) {
                if (self->canExplode) {
                    if (self->state != MatryoshkaBom_State_Exploding) {
                        if (Player_CheckCollisionTouch(player, self, &MatryoshkaBom->hitboxExplode)) {
                            RSDK.PlaySfx(MatryoshkaBom->sfxButton, false, 255);

                            switch (self->size) {
                                default: break;
                                case MATRYOSHKA_SIZE_BIG: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 1, &self->animator1, true, 0); break;
                                case MATRYOSHKA_SIZE_MED: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 5, &self->animator1, true, 0); break;
                                case MATRYOSHKA_SIZE_SMALL: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &self->animator1, true, 0); break;
                            }

                            self->timer = 21;
                            self->state = MatryoshkaBom_State_Exploding;
                        }
                    }
                }
                else if (self->state != MatryoshkaBom_Unknown8) {
                    if (Player_CheckCollisionTouch(player, self, &MatryoshkaBom->hitboxExplode)) {
                        self->timer = 144;
                        self->state = MatryoshkaBom_Unknown8;
                    }
                }
            }

            if (Player_CheckCollisionTouch(player, self, &MatryoshkaBom->hitboxHurt)) {
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer))
#endif
                Player_CheckHit(player, self);
            }
        }
    }
}

void MatryoshkaBom_CheckOnScreen(void)
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

void MatryoshkaBom_State_Setup(void)
{
    RSDK_THIS(MatryoshkaBom);
    self->active = ACTIVE_NORMAL;
    self->state  = MatryoshkaBom_Unknown5;
    MatryoshkaBom_Unknown5();
}

void MatryoshkaBom_Unknown5(void)
{
    RSDK_THIS(MatryoshkaBom);

    self->position.x += self->velocity.x;
    if (!--self->timer) {
        self->timer = 60;
        self->state = MatryoshkaBom_Unknown6;
    }
    else {
        bool32 collided = false;
        if (self->velocity.x <= 0)
            collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -self->offsetX, 0, true);
        else
            collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, self->offsetX, 0, true);

        if (!collided) {
            if (self->direction & FLIP_Y)
                collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_ROOF, 0, 0, -self->offsetY, 8);
            else
                collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, self->offsetY, 8);
        }

        if (!collided) {
            self->timer = 60;
            self->state = MatryoshkaBom_Unknown6;
            switch (self->size) {
                default: break;
                case MATRYOSHKA_SIZE_BIG: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &self->animator1, true, 0); break;
                case MATRYOSHKA_SIZE_MED: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &self->animator1, true, 0); break;
                case MATRYOSHKA_SIZE_SMALL: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &self->animator1, true, 0); break;
            }
        }
    }

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator3);
    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOnScreen();
}

void MatryoshkaBom_Unknown6(void)
{
    RSDK_THIS(MatryoshkaBom);

    if (!--self->timer) {
        switch (self->size) {
            default: break;
            case MATRYOSHKA_SIZE_BIG: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 3, &self->animator1, true, 0); break;
            case MATRYOSHKA_SIZE_MED: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 7, &self->animator1, true, 0); break;
            case MATRYOSHKA_SIZE_SMALL: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 11, &self->animator1, true, 0); break;
        }
        RSDK.SetSpriteAnimation(-1, 0, &self->animator3, true, 0);
        self->state = MatryoshkaBom_Unknown7;
    }
    RSDK.ProcessAnimation(&self->animator1);
    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOnScreen();
}

void MatryoshkaBom_Unknown7(void)
{
    RSDK_THIS(MatryoshkaBom);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
        self->timer      = 0x600;
        self->state      = MatryoshkaBom_Unknown5;
        switch (self->size) {
            default: break;
            case MATRYOSHKA_SIZE_BIG:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 2, &self->animator3, true, 0);
                break;
            case MATRYOSHKA_SIZE_MED:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 6, &self->animator3, true, 0);
                break;
            case MATRYOSHKA_SIZE_SMALL:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 10, &self->animator3, true, 0);
                break;
        }
    }
    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOnScreen();
}

void MatryoshkaBom_Unknown8(void)
{
    RSDK_THIS(MatryoshkaBom);

    if (self->direction & FLIP_Y)
        self->field_84 -= 0x1000;
    else
        self->field_84 += 0x1000;

    if (--self->timer > 0) {
        RSDK.ProcessAnimation(&self->animator1);
        RSDK.ProcessAnimation(&self->animator2);
        MatryoshkaBom_CheckPlayerCollisions();
        MatryoshkaBom_CheckOnScreen();
    }
    else {
        RSDK.PlaySfx(MatryoshkaBom->sfxExplosion, false, 255);

        EntityMatryoshkaBom *shrapnel = CREATE_ENTITY(MatryoshkaBom, intToVoid(3), self->position.x, self->position.y);
        shrapnel->velocity.x          = -0x20000;
        shrapnel->velocity.y          = -0x30000;
        shrapnel->planeFilter         = self->planeFilter;
        shrapnel->drawOrder           = self->drawOrder;

        shrapnel              = CREATE_ENTITY(MatryoshkaBom, intToVoid(3), self->position.x, self->position.y);
        shrapnel->velocity.x  = -0x10000;
        shrapnel->velocity.y  = -0x20000;
        shrapnel->planeFilter = self->planeFilter;
        shrapnel->drawOrder   = self->drawOrder;

        shrapnel              = CREATE_ENTITY(MatryoshkaBom, intToVoid(3), self->position.x, self->position.y);
        shrapnel->velocity.x  = 0x20000;
        shrapnel->velocity.y  = -0x30000;
        shrapnel->planeFilter = self->planeFilter;
        shrapnel->drawOrder   = self->drawOrder;

        shrapnel              = CREATE_ENTITY(MatryoshkaBom, intToVoid(3), self->position.x, self->position.y);
        shrapnel->velocity.x  = 0x10000;
        shrapnel->velocity.y  = -0x20000;
        shrapnel->planeFilter = self->planeFilter;
        shrapnel->drawOrder   = self->drawOrder;

        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(1), self->position.x, self->position.y);
        explosion->drawOrder       = self->drawOrder + 1;
        explosion->planeFilter     = self->planeFilter;

        destroyEntity(self);
    }
}

void MatryoshkaBom_State_Exploding(void)
{
    RSDK_THIS(MatryoshkaBom);

    RSDK.ProcessAnimation(&self->animator1);
    if (!--self->timer) {
        RSDK.PlaySfx(MatryoshkaBom->sfxPon, false, 255);

        EntityMatryoshkaBom *child = CREATE_ENTITY(MatryoshkaBom, intToVoid(self->size + 1), self->position.x, self->position.y);

        if (self->direction == FLIP_NONE)
            child->velocity.x = -0x18000;
        else
            child->velocity.x = 0x18000;
        child->velocity.y = -0x40000;

        child->planeFilter      = self->planeFilter;
        child->drawOrder        = self->drawOrder - 1;
        child->active           = ACTIVE_NORMAL;
        child->direction        = self->direction;
        child->destroyOffscreen = true;
        child->state            = MatryoshkaBom_Unknown10;
        self->canExplode      = false;
    }

    if (self->animator1.frameID >= self->animator1.frameDuration - 1)
        self->state = MatryoshkaBom_Unknown5;
    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOnScreen();
}

void MatryoshkaBom_Unknown10(void)
{
    RSDK_THIS(MatryoshkaBom);

    self->position.y += self->velocity.y;
    self->position.x += self->velocity.x;
    self->velocity.y += 0x3800;
    if (self->velocity.x) {
        bool32 collided = false;
        if (self->velocity.x <= 0)
            collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_ROOF, 0, -self->offsetX, 0, true);
        else
            collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, self->offsetX, 0, true);

        if (collided)
            self->velocity.x = 0;
    }

    bool32 collided = false;
    if (self->direction & FLIP_Y)
        collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_ROOF, 0, 0, -self->offsetY, true);
    else
        collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, self->offsetY, true);
    if (collided) {
        self->timer = 0x600;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0xC000;
        else
            self->velocity.x = 0xC000;
        self->state = MatryoshkaBom_Unknown5;
    }

    RSDK.ProcessAnimation(&self->animator1);
    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOnScreen();
}

void MatryoshkaBom_Unknown11(void)
{
    RSDK_THIS(MatryoshkaBom);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;
    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        RSDK.ProcessAnimation(&self->animator1);

        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionTouch(player, self, &MatryoshkaBom->hitboxShrapnel)) {
                    Player_CheckHit(player, self);
                }
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void MatryoshkaBom_EditorDraw(void)
{
    RSDK_THIS(MatryoshkaBom);
    self->drawFX |= FX_FLIP;

    switch (self->size) {
        default: break;
        case MATRYOSHKA_SIZE_BIG:
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &self->animator1, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 13, &self->animator2, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 2, &self->animator3, false, 0);
            break;
        case MATRYOSHKA_SIZE_MED:
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &self->animator1, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 14, &self->animator2, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 6, &self->animator3, false, 0);
            break;
        case MATRYOSHKA_SIZE_SMALL:
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &self->animator1, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 15, &self->animator2, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 10, &self->animator3, false, 0);
            break;
    }

    MatryoshkaBom_Draw();
}

void MatryoshkaBom_EditorLoad(void)
{
    MatryoshkaBom->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MatryoshkaBom.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(MatryoshkaBom, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);

    RSDK_ACTIVE_VAR(MatryoshkaBom, size);
    RSDK_ENUM_VAR("Big", MATRYOSHKA_SIZE_BIG);
    RSDK_ENUM_VAR("Medium", MATRYOSHKA_SIZE_MED);
    RSDK_ENUM_VAR("Small", MATRYOSHKA_SIZE_SMALL);
}

void MatryoshkaBom_Serialize(void)
{
    RSDK_EDITABLE_VAR(MatryoshkaBom, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(MatryoshkaBom, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(MatryoshkaBom, VAR_ENUM, size);
}
