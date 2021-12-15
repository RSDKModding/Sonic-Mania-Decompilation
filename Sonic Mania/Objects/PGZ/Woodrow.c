// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Woodrow Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectWoodrow *Woodrow;

void Woodrow_Update(void)
{
    RSDK_THIS(Woodrow);
    StateMachine_Run(self->state);
}

void Woodrow_LateUpdate(void) {}

void Woodrow_StaticUpdate(void) {}

void Woodrow_Draw(void)
{
    RSDK_THIS(Woodrow);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Woodrow_Create(void *data)
{
    RSDK_THIS(Woodrow);
    self->visible = true;
    self->drawFX |= FX_FLIP;
    self->drawOrder     = Zone->drawOrderLow - 1;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (!SceneInfo->inEditor) {
        if (self->type == WOODROW_BOMB) {
            self->startPos.x = self->position.x;
            self->startPos.y = self->position.y;
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 2, &self->animator, true, 0);
            self->state = Woodrow_State_BombSpawner;
        }
        else if (data) {
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 3, &self->animator, true, 0);
            self->state = Woodrow_State_Bomb;
        }
        else {
            self->drawOrder          = Zone->drawOrderHigh;
            self->bombCount          = 0;
            int32 pos                = SceneInfo->entitySlot + 1;
            EntityWoodrow *bombSpawn = RSDK_GET_ENTITY(pos, Woodrow);
            while (bombSpawn->objectID == Woodrow->objectID) {
                if (bombSpawn->type != WOODROW_BOMB)
                    break;
                bombSpawn->parent = (Entity *)self;
                ++self->bombCount;
                ++pos;
                bombSpawn = RSDK_GET_ENTITY(pos, Woodrow);
            }
            self->startPos.x    = self->position.x;
            self->startPos.y    = self->position.y;
            self->startDir      = self->direction;
            self->bombFallDelay = 60;
            self->distUp        = self->position.y - (self->distUp << 16);
            self->distDown      = self->position.y + (self->distDown << 16);
            self->moveDelay     = RSDK.Rand(15, 120);
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 0, &self->animator, true, 0);
            self->state = Woodrow_State_Setup;
        }
    }
}

void Woodrow_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        Woodrow->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Woodrow.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ2"))
        Woodrow->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Woodrow.bin", SCOPE_STAGE);

    Woodrow->hitbox1.left   = -32;
    Woodrow->hitbox1.top    = -24;
    Woodrow->hitbox1.right  = 12;
    Woodrow->hitbox1.bottom = 5;
    Woodrow->hitbox2.left   = -112;
    Woodrow->hitbox2.top    = -128;
    Woodrow->hitbox2.right  = 144;
    Woodrow->hitbox2.bottom = 128;
    Woodrow->hitbox3.left   = -48;
    Woodrow->hitbox3.top    = -128;
    Woodrow->hitbox3.right  = 80;
    Woodrow->hitbox3.bottom = 128;
    Woodrow->hitbox4.left   = -12;
    Woodrow->hitbox4.top    = -12;
    Woodrow->hitbox4.right  = 12;
    Woodrow->hitbox4.bottom = 12;

    Woodrow->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    Woodrow->sfxPeck      = RSDK.GetSfx("PSZ/Peck.wav");
    DEBUGMODE_ADD_OBJ(Woodrow);
}

void Woodrow_DebugSpawn(void)
{
    RSDK_THIS(Woodrow);
    CREATE_ENTITY(Woodrow, NULL, self->position.x, self->position.y);
}

void Woodrow_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Woodrow->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Woodrow_CheckPlayerCollisions(void)
{
    RSDK_THIS(Woodrow);

    uint8 flags = 0;
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Woodrow->hitbox1)) {
            Player_CheckBadnikBreak(self, player, true);
        }
        else if (player->state != Ice_State_FrozenPlayer) {
            if (Player_CheckCollisionTouch(player, self, &Woodrow->hitbox3)) {
                if (self->collisionFlags == 1)
                    self->bombFallDelay = 30;
                flags |= 2;
                self->collisionFlags |= 2;
            }
            else if (Player_CheckCollisionTouch(player, self, &Woodrow->hitbox2)) {
                self->collisionFlags |= 1;
                flags |= 1;
            }
        }
    }

    if (flags) {
        if (flags == 1 && self->bombFallDelay > 30)
            self->bombFallDelay = 30;
    }
    else {
        if (self->animator.animationID == 1)
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &self->animator, false, 0);
        self->collisionFlags = 0;
    }
}

void Woodrow_CheckOnScreen(void)
{
    RSDK_THIS(Woodrow);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        self->distUp     = (self->position.y - self->distUp) >> 16;
        self->distDown   = (self->distDown - self->position.y) >> 16;
        Woodrow_Create(NULL);
    }
}

void Woodrow_State_Setup(void)
{
    RSDK_THIS(Woodrow);
    self->active = ACTIVE_NORMAL;
    self->state  = Woodrow_Unknown4;
    Woodrow_Unknown4();
}

void Woodrow_Unknown4(void)
{
    RSDK_THIS(Woodrow);
    RSDK.ProcessAnimation(&self->animator);

    if (self->collisionFlags <= 1) {
        if (self->animator.animationID <= 3) {
            if (self->animator.animationID == 1)
                RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &self->animator, false, 0);
            self->collisionFlags = 0;
        }
        else if (self->animator.frameID == self->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 0, &self->animator, false, 0);
        }
        if ((self->distUp == self->distDown) != self->startPos.y) {
            if (!--self->moveDelay) {
                self->moveDelay     = RSDK.Rand(15, 120);
                self->bombFallDelay = RSDK.Rand(8, 30);

                if (self->position.y > self->distUp) {
                    if (self->position.y < self->distDown && RSDK.Rand(0, 2) == 1)
                        self->state = Woodrow_Unknown6;
                    else
                        self->state = Woodrow_Unknown5;
                }
                else {
                    self->state = Woodrow_Unknown6;
                }

                if (self->state == Woodrow_Unknown6)
                    RSDK.SetSpriteAnimation(Woodrow->aniFrames, 6, &self->animator, true, 0);
                else
                    RSDK.SetSpriteAnimation(Woodrow->aniFrames, 5, &self->animator, true, 0);
            }
        }
    }
    else {
        if (!(Zone->timer & 7))
            RSDK.PlaySfx(Woodrow->sfxPeck, false, 255);
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 1, &self->animator, false, 0);

        if (!--self->bombFallDelay) {
            if (!self->bombCount) {
                self->bombFallDelay = -76;
                if (self->activeBombCount == 0) {
                    self->activeBombCount = 1;
                }
                else {
                    self->collisionFlags = 0;
                }
            }
            if (self->activeBombCount >= self->bombCount) {
                self->bombFallDelay  = 30;
                self->collisionFlags = 0;
            }
            else {
                int32 pos = SceneInfo->entitySlot + 1;

                EntityWoodrow *bombSpawn = NULL;
                do {
                    bombSpawn = RSDK_GET_ENTITY(pos + RSDK.Rand(0, self->bombCount), Woodrow);
                } while (bombSpawn->activeBombCount);

                EntityWoodrow *bomb = CREATE_ENTITY(Woodrow, intToVoid(true), bombSpawn->position.x, bombSpawn->position.y);
                bombSpawn->position.y -= 0x100000;
                bombSpawn->bombFallDelay   = 120;
                bombSpawn->activeBombCount = 32;
                bombSpawn->active          = ACTIVE_NORMAL;
                bomb->parent               = (Entity *)bombSpawn->parent;
                if (++self->activeBombCount == self->bombCount)
                    self->bombFallDelay = -76;
                else
                    self->bombFallDelay = 60;
            }
        }
    }
    Woodrow_CheckPlayerCollisions();
    Woodrow_CheckOnScreen();
}

void Woodrow_Unknown5(void)
{
    RSDK_THIS(Woodrow);
    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID >= 3) {
        self->position.y -= 0x20000;
        if (self->position.y < self->distUp)
            self->position.y = self->distUp;
    }
    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &self->animator, false, 0);
        self->state = Woodrow_Unknown4;
    }
    Woodrow_CheckPlayerCollisions();
    Woodrow_CheckOnScreen();
}

void Woodrow_Unknown6(void)
{
    RSDK_THIS(Woodrow);
    RSDK.ProcessAnimation(&self->animator);
    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;

    if (!--self->bombFallDelay) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &self->animator, false, 0);
        self->state = Woodrow_Unknown4;
    }
    else if (self->position.y >= self->distDown) {
        self->position.y = self->distDown;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &self->animator, false, 0);
        self->state = Woodrow_Unknown4;
    }
    Woodrow_CheckPlayerCollisions();
    Woodrow_CheckOnScreen();
}

void Woodrow_State_Bomb(void)
{
    RSDK_THIS(Woodrow);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
            RSDK.PlaySfx(Woodrow->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
            destroyEntity(self);
        }
        RSDK.ProcessAnimation(&self->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Woodrow->hitbox4)) {
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, self);
                RSDK.PlaySfx(Woodrow->sfxExplosion, false, 255);
                CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
                destroyEntity(self);
                foreach_break;
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void Woodrow_State_BombSpawner(void)
{
    RSDK_THIS(Woodrow);
    if (self->bombFallDelay) {
        self->visible = false;
        self->bombFallDelay--;
    }
    else {
        self->visible = true;
        if (self->activeBombCount) {
            self->position.y += 0x8000;
            self->activeBombCount--;
        }
        else {
            EntityWoodrow *parent = (EntityWoodrow *)self->parent;
            if (parent) {
                self->position.x = self->startPos.x;
                self->position.y = self->startPos.y;
                if (parent->objectID != Woodrow->objectID) {
                    self->active = ACTIVE_BOUNDS;
                }
                else {
                    if (parent->animator.animationID == 1) {
                        self->position.x += RSDK.Rand(-1, 2) << 16;
                        self->position.y += RSDK.Rand(-1, 2) << 16;
                    }
                    if (self->active == ACTIVE_NORMAL) {
                        --parent->activeBombCount;
                        self->active = ACTIVE_BOUNDS;
                    }
                }
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Woodrow_EditorDraw(void)
{
    RSDK_THIS(Woodrow);

    if (self->type == WOODROW_BOMB) {
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 2, &self->animator, true, 0);
        self->state = Woodrow_State_BombSpawner;
    }
    else {
        self->drawOrder          = Zone->drawOrderHigh;
        self->startPos.x    = self->position.x;
        self->startPos.y    = self->position.y;
        self->startDir      = self->direction;
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 0, &self->animator, true, 0);
    }

    Woodrow_Draw();
}

void Woodrow_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        Woodrow->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Woodrow.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ2"))
        Woodrow->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Woodrow.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Woodrow, type);
    RSDK_ENUM_VAR("Woodrow", WOODROW_BADNIK);
    RSDK_ENUM_VAR("Bomb", WOODROW_BOMB);

    RSDK_ACTIVE_VAR(Woodrow, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Woodrow_Serialize(void)
{
    RSDK_EDITABLE_VAR(Woodrow, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Woodrow, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Woodrow, VAR_ENUM, distUp);
    RSDK_EDITABLE_VAR(Woodrow, VAR_ENUM, distDown);
}
