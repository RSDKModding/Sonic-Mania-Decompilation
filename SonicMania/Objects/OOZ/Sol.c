// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Sol Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSol *Sol;

void Sol_Update(void)
{
    RSDK_THIS(Sol);

    StateMachine_Run(self->state);
}

void Sol_LateUpdate(void) {}

void Sol_StaticUpdate(void) {}

void Sol_Draw(void)
{
    RSDK_THIS(Sol);

    for (int32 i = 0; i < SOL_FLAMEORB_COUNT; ++i) {
        if ((1 << i) & self->activeOrbs)
            RSDK.DrawSprite(&self->ballAnimator, &self->positions[i], false);
    }

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void Sol_Create(void *data)
{
    RSDK_THIS(Sol);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX    = FX_FLIP;

    if (data) {
        RSDK.SetSpriteAnimation(Sol->aniFrames, 1, &self->mainAnimator, true, 0);
        self->active = ACTIVE_NORMAL;
        self->drawFX |= FX_ROTATE;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        self->state         = Sol_State_SmallFireball;
    }
    else {
        self->startPos      = self->position;
        self->startDir      = self->direction;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->activeOrbs    = 2 | 8;

        RSDK.SetSpriteAnimation(Sol->aniFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(Sol->aniFrames, 1, &self->ballAnimator, true, 0);
        self->state      = Sol_State_Init;
        self->velocity.x = self->direction == FLIP_NONE ? -0x4000 : 0x4000;
    }
}

void Sol_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("OOZ1") || RSDK.CheckSceneFolder("OOZ2"))
        Sol->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Sol.bin", SCOPE_STAGE);

    Sol->hitboxBadnik.left   = -8;
    Sol->hitboxBadnik.top    = -8;
    Sol->hitboxBadnik.right  = 8;
    Sol->hitboxBadnik.bottom = 8;

    Sol->hitboxOrb.left   = -4;
    Sol->hitboxOrb.top    = -4;
    Sol->hitboxOrb.right  = 4;
    Sol->hitboxOrb.bottom = 4;

    DEBUGMODE_ADD_OBJ(Sol);
}

void Sol_DebugSpawn(void)
{
    RSDK_THIS(Sol);

    CREATE_ENTITY(Sol, NULL, self->position.x, self->position.y);
}

void Sol_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Sol->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Sol_HandlePlayerInteractions(void)
{
    RSDK_THIS(Sol);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    for (int32 i = 0; i < SOL_FLAMEORB_COUNT; ++i) {
        if ((1 << i) & self->activeOrbs) {
            self->position.x = self->positions[i].x;
            self->position.y = self->positions[i].y;

            foreach_active(Player, player)
            {
#if MANIA_USE_PLUS
                if (player->state != Player_State_MightyHammerDrop) {
#endif
                    Sol_HandlePlayerHurt();
#if MANIA_USE_PLUS
                }
#endif
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Sol->hitboxBadnik) && Player_CheckBadnikBreak(player, self, false)) {
            int32 angle = self->angle;
            for (int32 i = 0; i < SOL_FLAMEORB_COUNT; ++i) {
                if ((1 << i) & self->activeOrbs) {
                    self->position.x = self->positions[i].x;
                    self->position.y = self->positions[i].y;

                    EntitySol *sol = CREATE_ENTITY(Sol, INT_TO_VOID(true), self->positions[i].x, self->positions[i].y);

                    sol->state = Sol_State_ActiveFireball;
#if MANIA_USE_PLUS
                    if (player->state == Player_State_MightyHammerDrop)
                        sol->interaction = false;
#endif
                    sol->velocity.x = 0x380 * RSDK.Cos256(angle);
                    sol->velocity.y = 0x380 * RSDK.Sin256(angle);
                }

                angle += (0x100 / SOL_FLAMEORB_COUNT);
            }

            destroyEntity(self);
        }
    }
}

void Sol_HandlePlayerHurt(void)
{
    RSDK_THIS(Sol);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Sol->hitboxOrb)) {
            Player_ElementHurt(player, self, SHIELD_FIRE);
        }
    }
}

void Sol_HandleRotation(void)
{
    RSDK_THIS(Sol);

    int32 angle = self->angle;
    if (self->direction)
        self->angle = (angle - 1) & 0xFF;
    else
        self->angle = (angle + 1) & 0xFF;

    for (int32 i = 0; i < SOL_FLAMEORB_COUNT; ++i) {
        if ((1 << i) & self->activeOrbs) {
            self->positions[i].x = (RSDK.Cos256(angle) << 12) + self->position.x;
            self->positions[i].y = (RSDK.Sin256(angle) << 12) + self->position.y;
        }

        angle += (0x100 / SOL_FLAMEORB_COUNT);
    }
}

void Sol_CheckOffScreen(void)
{
    RSDK_THIS(Sol);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Sol_Create(NULL);
    }
}

void Sol_State_Init(void)
{
    RSDK_THIS(Sol);

    self->active = ACTIVE_NORMAL;

    self->state = Sol_State_Moving;
    Sol_State_Moving();
}

void Sol_State_Moving(void)
{
    RSDK_THIS(Sol);

    RSDK.ProcessAnimation(&self->ballAnimator);

    self->position.x += self->velocity.x;
    self->position.y = (RSDK.Sin256(self->oscillateAngle) << 10) + self->startPos.y;
    self->oscillateAngle += 4;

    Sol_HandleRotation();
    Sol_HandlePlayerInteractions();

    if (self->fireOrbs) {
        EntityPlayer *playerPtr = NULL;
        int32 distanceX         = 0x7FFFFFFF;
        int32 distanceY         = 0x7FFFFFFF;

        foreach_active(Player, player)
        {
            if (abs(player->position.y - self->position.y) < distanceY)
                distanceY = abs(player->position.y - self->position.y);

            if (abs(player->position.y - self->position.y) < 0x400000) {
                if (!playerPtr) {
                    if (abs(player->position.x - self->position.x) < distanceX) {
                        distanceX = abs(player->position.x - self->position.x);
                        playerPtr = player;
                    }
                }
                else {
                    if (abs(player->position.x - self->position.x) < distanceX) {
                        distanceX = abs(player->position.x - self->position.x);
                        playerPtr = player;
                    }
                }
            }
        }

        if (!playerPtr)
            playerPtr = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        if (distanceX <= 0x800000) {
            self->state                = Sol_State_ShootingOrbs;
            self->mainAnimator.frameID = 1;
        }

        self->direction = playerPtr->position.x >= self->position.x;
    }

    Sol_CheckOffScreen();
}

void Sol_State_ShootingOrbs(void)
{
    RSDK_THIS(Sol);

    RSDK.ProcessAnimation(&self->ballAnimator);

    self->position.x += self->velocity.x;
    self->position.y = (RSDK.Sin256(self->oscillateAngle) << 10) + self->startPos.y;
    self->oscillateAngle += 4;

    uint8 angle = self->angle;
    Sol_HandleRotation();

    for (int32 i = 0; i < SOL_FLAMEORB_COUNT; ++i) {
        if (angle == 0x40) {
            if ((1 << i) & self->activeOrbs) {
                self->activeOrbs &= ~(1 << i);
                EntitySol *sol  = CREATE_ENTITY(Sol, INT_TO_VOID(true), self->positions[i].x, self->positions[i].y);
                sol->velocity.x = self->direction == FLIP_NONE ? -0x20000 : 0x20000;
            }
        }

        angle += (0x100 / SOL_FLAMEORB_COUNT);
    }

    Sol_HandlePlayerInteractions();

    if (!self->activeOrbs) {
        self->state      = Sol_State_NoOrbs;
        self->velocity.x = self->direction == FLIP_NONE ? -0x4000 : 0x4000;
    }

    if (self->mainAnimator.timer >= 0x10)
        self->mainAnimator.frameID = 2;
    else
        self->mainAnimator.timer++;

    Sol_CheckOffScreen();
}

void Sol_State_NoOrbs(void)
{
    RSDK_THIS(Sol);

    RSDK.ProcessAnimation(&self->ballAnimator);

    self->position.x += self->velocity.x;
    self->position.y = (RSDK.Sin256(self->oscillateAngle) << 10) + self->startPos.y;
    self->oscillateAngle += 4;

    Sol_HandleRotation();
    Sol_HandlePlayerInteractions();
    Sol_CheckOffScreen();
}

void Sol_State_SmallFireball(void)
{
    RSDK_THIS(Sol);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.x += self->velocity.x;

    if (RSDK.CheckOnScreen(self, &self->updateRange))
        Sol_HandlePlayerHurt();
    else
        destroyEntity(self);
}

void Sol_State_ActiveFireball(void)
{
    RSDK_THIS(Sol);

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
        self->velocity.y += 0x3800;

        self->rotation = 2 * RSDK.ATan2(self->velocity.x >> 16, self->velocity.y >> 16) + 384;

        int32 offsetX = RSDK.Sin512(512 - self->rotation) << 10;
        int32 offsetY = RSDK.Cos512(512 - self->rotation) << 10;
        int32 cmode   = 3 - (((self->rotation - 0x40) >> 7) & 3);

        bool32 collided = RSDK.ObjectTileCollision(self, Zone->collisionLayers, cmode, 1, offsetX, offsetY, true);
        if (!collided)
            collided = RSDK.ObjectTileCollision(self, Zone->collisionLayers, cmode, 0, offsetX, offsetY, true);

        if (collided) {
            RSDK.SetSpriteAnimation(Sol->aniFrames, 2, &self->mainAnimator, true, 0);
            self->state    = Sol_State_FlameDissipate;
            self->rotation = (self->rotation + 64) & 0x180;
        }

        int32 spawnX = self->position.x + offsetX;
        int32 spawnY = self->position.y + offsetY;
        uint16 tile  = RSDK.GetTile(Zone->fgLayer[1], spawnX >> 20, (spawnY - 0x10000) >> 20);
        if (tile == (uint16)-1)
            tile = RSDK.GetTile(Zone->fgLayer[0], spawnX >> 20, (spawnY - 0x10000) >> 20);

        int32 tileFlags = RSDK.GetTileFlags(tile, 0);
        if (((tileFlags == OOZ_TFLAGS_OILSTRIP || tileFlags == OOZ_TFLAGS_OILSLIDE) && collided) || tileFlags == OOZ_TFLAGS_OILPOOL) {
            self->position.x = spawnX - 0x40000;
            self->position.y = spawnY - 0x80000;
            self->rotation   = 0;
            self->velocity.x = -0x40000;
            self->velocity.y = 0;
            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &self->mainAnimator, true, 0);
            self->state = Sol_State_FireballOilFlame;

            EntitySol *sol  = CREATE_ENTITY(Sol, INT_TO_VOID(true), spawnX, spawnY - 0x80000);
            sol->velocity.x = 0x40000;
            sol->velocity.y = 0;
            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &sol->mainAnimator, true, 0);
            sol->state          = Sol_State_FireballOilFlame;
            sol->oscillateAngle = sol->position.x & 0xF00000;

            if (tileFlags == OOZ_TFLAGS_OILPOOL) {
                self->position.y = (self->position.y & 0xFFF00000) + 0x20000;
                sol->position.y  = (sol->position.y & 0xFFF00000) + 0x20000;
                sol->state       = Sol_State_OilFlame;
                self->state      = Sol_State_OilFlame;
            }
            else {
                self->position.y -= 0x80000;
            }
        }

        if (self->interaction)
            Sol_HandlePlayerHurt();
    }
    else {
        destroyEntity(self);
    }
}

void Sol_State_FlameDissipate(void)
{
    RSDK_THIS(Sol);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (!RSDK.CheckOnScreen(self, &self->updateRange) || self->mainAnimator.frameID == self->mainAnimator.frameCount - 1) {
        destroyEntity(self);
    }
}

void Sol_State_FireballOilFlame(void)
{
    RSDK_THIS(Sol);

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        bool32 collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 1, 0, 0x80000, 16);
        if (!collided)
            collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, 16);

        if (collided) {
            uint16 tile = RSDK.GetTile(Zone->fgLayer[1], self->position.x >> 20, (self->position.y + 0x90000) >> 20);
            if (tile == (uint16)-1)
                tile = RSDK.GetTile(Zone->fgLayer[0], self->position.x >> 20, (self->position.y + 0x90000) >> 20);

            self->rotation = 2 * RSDK.GetTileAngle(tile, 0, 0);
        }

        uint16 tile = RSDK.GetTile(Zone->fgLayer[1], self->position.x >> 20, (self->position.y + 0x70000) >> 20);
        if (tile == (uint16)-1)
            tile = RSDK.GetTile(Zone->fgLayer[0], self->position.x >> 20, (self->position.y + 0x70000) >> 20);

        int32 tileFlags = RSDK.GetTileFlags(tile, 0);
        if (tileFlags == OOZ_TFLAGS_NORMAL || tileFlags == OOZ_TFLAGS_OILFALL) {
            if (collided) {
                RSDK.SetSpriteAnimation(Sol->aniFrames, 2, &self->mainAnimator, true, 0);
                self->state = Sol_State_FlameDissipate;
            }
            else {
                self->state = Sol_State_ActiveFireball;
            }
        }
        else {
            self->position.y -= 0x80000;
            if ((self->position.x & 0xF00000) != self->oscillateAngle)
                OOZSetup_StartFire((self->position.x & 0xFFF00000) + 0x70000, self->position.y & 0xFFFF0000, self->rotation >> 1);

            self->oscillateAngle = self->position.x & 0xF00000;
        }

        self->position.x += self->velocity.x;
        self->position.y += 0x80000;

        RSDK.ProcessAnimation(&self->mainAnimator);

        Sol_HandlePlayerHurt();
    }
    else {
        destroyEntity(self);
    }
}

void Sol_State_OilFlame(void)
{
    RSDK_THIS(Sol);

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        uint16 tile = RSDK.GetTile(Zone->fgLayer[1], self->position.x >> 20, (self->position.y + 0xF0000) >> 20);
        if (tile == (uint16)-1)
            tile = RSDK.GetTile(Zone->fgLayer[0], self->position.x >> 20, (self->position.y + 0xF0000) >> 20);

        if (RSDK.GetTileFlags(tile, 0) == OOZ_TFLAGS_OILPOOL) {
            if ((self->position.x & 0xF00000) != self->oscillateAngle)
                OOZSetup_StartFire((self->position.x & 0xFFF00000) + 0x70000, self->position.y & 0xFFFF0000, self->rotation >> 1);

            self->oscillateAngle = self->position.x & 0xF00000;
        }
        else {
            RSDK.SetSpriteAnimation(Sol->aniFrames, 2, &self->mainAnimator, true, 0);
            self->state = Sol_State_FlameDissipate;
        }

        self->position.x += self->velocity.x;

        RSDK.ProcessAnimation(&self->mainAnimator);

        Sol_HandlePlayerHurt();
    }
    else {
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void Sol_EditorDraw(void)
{
    RSDK_THIS(Sol);

    int32 angle = self->angle;
    Sol_HandleRotation();
    self->angle = angle;

    Sol_Draw();
}

void Sol_EditorLoad(void)
{
    Sol->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Sol.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Sol, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Sol_Serialize(void)
{
    RSDK_EDITABLE_VAR(Sol, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Sol, VAR_BOOL, fireOrbs);
}
