// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Woodrow Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
    self->drawGroup     = Zone->objectDrawGroup[0] - 1;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (!SceneInfo->inEditor) {
        if (self->type == WOODROW_BOMB) {
            self->startPos = self->position;
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 2, &self->animator, true, 0);

            self->state = Woodrow_State_BombSpawner;
        }
        else if (data) {
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 3, &self->animator, true, 0);

            self->state = Woodrow_State_Bomb;
        }
        else {
            self->drawGroup = Zone->objectDrawGroup[1];
            self->bombCount = 0;

            int32 slot               = SceneInfo->entitySlot + 1;
            EntityWoodrow *bombSpawn = RSDK_GET_ENTITY(slot, Woodrow);
            while (bombSpawn->classID == Woodrow->classID) {
                if (bombSpawn->type != WOODROW_BOMB)
                    break;
                bombSpawn->parent = self;
                ++self->bombCount;
                ++slot;
                bombSpawn = RSDK_GET_ENTITY(slot, Woodrow);
            }

            self->startPos      = self->position;
            self->startDir      = self->direction;
            self->bombFallDelay = 60;
            self->distUp        = self->position.y - (self->distUp << 16);
            self->distDown      = self->position.y + (self->distDown << 16);
            self->moveDelay     = RSDK.Rand(15, 120);

            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 0, &self->animator, true, 0);
            self->state = Woodrow_State_Init;
        }
    }
}

void Woodrow_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        Woodrow->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Woodrow.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        Woodrow->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Woodrow.bin", SCOPE_STAGE);

    Woodrow->hitboxBadnik.left   = -32;
    Woodrow->hitboxBadnik.top    = -24;
    Woodrow->hitboxBadnik.right  = 12;
    Woodrow->hitboxBadnik.bottom = 5;

    Woodrow->hitboxFullRange.left   = -112;
    Woodrow->hitboxFullRange.top    = -128;
    Woodrow->hitboxFullRange.right  = 144;
    Woodrow->hitboxFullRange.bottom = 128;

    Woodrow->hitboxBombRange.left   = -48;
    Woodrow->hitboxBombRange.top    = -128;
    Woodrow->hitboxBombRange.right  = 80;
    Woodrow->hitboxBombRange.bottom = 128;

    Woodrow->hitboxBomb.left   = -12;
    Woodrow->hitboxBomb.top    = -12;
    Woodrow->hitboxBomb.right  = 12;
    Woodrow->hitboxBomb.bottom = 12;

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

    uint8 rangeMask = 0;
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Woodrow->hitboxBadnik)) {
            Player_CheckBadnikBreak(player, self, true);
        }
        else if (player->state != Ice_PlayerState_Frozen) {
            if (Player_CheckCollisionTouch(player, self, &Woodrow->hitboxBombRange)) {
                if (self->rangeMask == 1)
                    self->bombFallDelay = 30;

                rangeMask |= 2;
                self->rangeMask |= 2;
            }
            else if (Player_CheckCollisionTouch(player, self, &Woodrow->hitboxFullRange)) {
                self->rangeMask |= 1;
                rangeMask |= 1;
            }
        }
    }

    if (rangeMask) {
        if (rangeMask == 1 && self->bombFallDelay > 30)
            self->bombFallDelay = 30;
    }
    else {
        if (self->animator.animationID == 1)
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &self->animator, false, 0);

        self->rangeMask = 0;
    }
}

void Woodrow_CheckOffScreen(void)
{
    RSDK_THIS(Woodrow);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        self->distUp    = (self->position.y - self->distUp) >> 16;
        self->distDown  = (self->distDown - self->position.y) >> 16;
        Woodrow_Create(NULL);
    }
}

void Woodrow_State_Init(void)
{
    RSDK_THIS(Woodrow);

    self->active = ACTIVE_NORMAL;

    self->state = Woodrow_State_Idle;
    Woodrow_State_Idle();
}

void Woodrow_State_Idle(void)
{
    RSDK_THIS(Woodrow);

    RSDK.ProcessAnimation(&self->animator);

    if (self->rangeMask <= 1) {
        if (self->animator.animationID <= 3) {
            if (self->animator.animationID == 1)
                RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &self->animator, false, 0);

            self->rangeMask = 0;
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
                        self->state = Woodrow_State_MoveDown;
                    else
                        self->state = Woodrow_State_MoveUp;
                }
                else {
                    self->state = Woodrow_State_MoveDown;
                }

                if (self->state == Woodrow_State_MoveDown)
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

                if (self->activeBombCount == 0)
                    self->activeBombCount = 1;
                else
                    self->rangeMask = 0;
            }

            if (self->activeBombCount >= self->bombCount) {
                self->bombFallDelay = 30;
                self->rangeMask     = 0;
            }
            else {
                int32 bombSlot = SceneInfo->entitySlot + 1;

                EntityWoodrow *bombSpawn = RSDK_GET_ENTITY(bombSlot + RSDK.Rand(0, self->bombCount), Woodrow);
                while (bombSpawn->activeBombCount) {
                    bombSpawn = RSDK_GET_ENTITY(bombSlot + RSDK.Rand(0, self->bombCount), Woodrow);
                }

                EntityWoodrow *bomb = CREATE_ENTITY(Woodrow, INT_TO_VOID(true), bombSpawn->position.x, bombSpawn->position.y);
                bombSpawn->position.y -= 0x100000;
                bombSpawn->bombFallDelay   = 120;
                bombSpawn->activeBombCount = 32;
                bombSpawn->active          = ACTIVE_NORMAL;
                bomb->parent               = bombSpawn->parent;

                if (++self->activeBombCount == self->bombCount)
                    self->bombFallDelay = -76;
                else
                    self->bombFallDelay = 60;
            }
        }
    }

    Woodrow_CheckPlayerCollisions();
    Woodrow_CheckOffScreen();
}

void Woodrow_State_MoveUp(void)
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
        self->state = Woodrow_State_Idle;
    }

    Woodrow_CheckPlayerCollisions();
    Woodrow_CheckOffScreen();
}

void Woodrow_State_MoveDown(void)
{
    RSDK_THIS(Woodrow);

    RSDK.ProcessAnimation(&self->animator);

    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;

    if (!--self->bombFallDelay) {
        self->velocity.y = 0;

        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &self->animator, false, 0);
        self->state = Woodrow_State_Idle;
    }
    else if (self->position.y >= self->distDown) {
        self->position.y = self->distDown;
        self->velocity.y = 0;

        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &self->animator, false, 0);
        self->state = Woodrow_State_Idle;
    }

    Woodrow_CheckPlayerCollisions();
    Woodrow_CheckOffScreen();
}

void Woodrow_State_Bomb(void)
{
    RSDK_THIS(Woodrow);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
            RSDK.PlaySfx(Woodrow->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
            destroyEntity(self);
        }

        RSDK.ProcessAnimation(&self->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Woodrow->hitboxBomb)) {
#if MANIA_USE_PLUS
                if (!Player_CheckMightyUnspin(player, 0x300, 2, &player->uncurlTimer))
#endif
                    Player_Hurt(player, self);
                RSDK.PlaySfx(Woodrow->sfxExplosion, false, 0xFF);

                CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
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
            EntityWoodrow *parent = self->parent;
            if (parent) {
                self->position.x = self->startPos.x;
                self->position.y = self->startPos.y;

                if (parent->classID != Woodrow->classID) {
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

#if GAME_INCLUDE_EDITOR
void Woodrow_EditorDraw(void)
{
    RSDK_THIS(Woodrow);

    if (self->type == WOODROW_BOMB) {
        self->drawGroup = Zone->objectDrawGroup[0];
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 2, &self->animator, true, 0);
    }
    else {
        self->drawGroup = Zone->objectDrawGroup[1];
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 0, &self->animator, true, 0);
    }

    Woodrow_Draw();

    if (self->type == WOODROW_BADNIK && showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        int32 distUp   = self->distUp;
        int32 distDown = self->distDown;
        int32 y        = self->position.y;

        self->distUp   = self->position.y - (self->distUp << 16);
        self->distDown = self->position.y + (self->distDown << 16);

        self->inkEffect = INK_BLEND;

        if (self->distUp) {
            self->position.y = self->distUp;
            Woodrow_Draw();
        }

        if (self->distDown) {
            self->position.y = self->distDown;
            Woodrow_Draw();
        }

        self->position.y = y;
        self->inkEffect  = INK_NONE;

        if (self->distUp)
            DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x, self->distUp, 0x00FF00, INK_NONE, 0xFF);

        if (self->distDown)
            DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x, self->distDown, 0x00FF00, INK_NONE, 0xFF);

        self->distUp   = distUp;
        self->distDown = distDown;

        int32 slot               = SceneInfo->entitySlot + 1;
        EntityWoodrow *bombSpawn = RSDK_GET_ENTITY(slot, Woodrow);
        while (bombSpawn && bombSpawn->classID == Woodrow->classID) {
            if (bombSpawn->type != WOODROW_BOMB)
                break;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, bombSpawn->position.x, bombSpawn->position.y, 0xE0E0E0, INK_NONE, 0xFF);

            ++slot;
            bombSpawn = RSDK_GET_ENTITY(slot, Woodrow);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Woodrow_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        Woodrow->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Woodrow.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        Woodrow->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Woodrow.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Woodrow, type);
    RSDK_ENUM_VAR("Woodrow", WOODROW_BADNIK);
    RSDK_ENUM_VAR("Bomb", WOODROW_BOMB);

    RSDK_ACTIVE_VAR(Woodrow, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void Woodrow_Serialize(void)
{
    RSDK_EDITABLE_VAR(Woodrow, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Woodrow, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Woodrow, VAR_ENUM, distUp);
    RSDK_EDITABLE_VAR(Woodrow, VAR_ENUM, distDown);
}
