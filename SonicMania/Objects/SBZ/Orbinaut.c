// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Orbinaut Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectOrbinaut *Orbinaut = NULL;

void Orbinaut_Update(void)
{
    RSDK_THIS(Orbinaut);
    StateMachine_Run(self->state);
}

void Orbinaut_LateUpdate(void) {}

void Orbinaut_StaticUpdate(void) {}

void Orbinaut_Draw(void)
{
    RSDK_THIS(Orbinaut);

    for (int32 o = 0; o < ORBINAUT_ORB_COUNT; ++o) {
        if ((1 << o) & self->activeOrbs) {
            RSDK.DrawSprite(&self->animatorOrb, &self->orbPositions[o], false);
        }
    }

    RSDK.DrawSprite(&self->animatorFace, NULL, false);
}

void Orbinaut_Create(void *data)
{
    RSDK_THIS(Orbinaut);

    self->visible = true;
    if (self->planeFilter > 0 && ((self->planeFilter - 1) & 2))
        self->drawGroup = Zone->objectDrawGroup[1];
    else
        self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->activeOrbs    = 1 | 2 | 4 | 8;
    if (data) {
        RSDK.SetSpriteAnimation(Orbinaut->aniFrames, 1, &self->animatorFace, true, 0);
        self->state = Orbinaut_State_Orb;
    }
    else {
        RSDK.SetSpriteAnimation(Orbinaut->aniFrames, 0, &self->animatorFace, true, 0);
        RSDK.SetSpriteAnimation(Orbinaut->aniFrames, 1, &self->animatorOrb, true, 0);
        self->state = Orbinaut_State_Init;
        if (self->fireOrbs) {
            self->velocity.x = 0;
        }
        else {
            if (self->direction == FLIP_NONE)
                self->velocity.x = -0x4000;
            else
                self->velocity.x = 0x4000;
        }
    }
}

void Orbinaut_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MMZ"))
        Orbinaut->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Orbinaut.bin", SCOPE_STAGE);

    Orbinaut->hitboxBadnik.left   = -8;
    Orbinaut->hitboxBadnik.top    = -8;
    Orbinaut->hitboxBadnik.right  = 8;
    Orbinaut->hitboxBadnik.bottom = 8;

    Orbinaut->hitboxOrb.left   = -4;
    Orbinaut->hitboxOrb.top    = -4;
    Orbinaut->hitboxOrb.right  = 4;
    Orbinaut->hitboxOrb.bottom = 4;

    DEBUGMODE_ADD_OBJ(Orbinaut);
}

void Orbinaut_DebugSpawn(void)
{
    RSDK_THIS(Orbinaut);
    CREATE_ENTITY(Orbinaut, NULL, self->position.x, self->position.y);
}

void Orbinaut_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Orbinaut->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Orbinaut_HandlePlayerInteractions(void)
{
    RSDK_THIS(Orbinaut);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    for (int32 i = 0; i < ORBINAUT_ORB_COUNT; ++i) {
        if ((1 << i) & self->activeOrbs) {
            self->position.x = self->orbPositions[i].x;
            self->position.y = self->orbPositions[i].y;
            foreach_active(Player, player)
            {
                if (self->planeFilter <= 0 || player->collisionPlane == (uint8)((self->planeFilter - 1) & 1)) {
                    if (Player_CheckCollisionTouch(player, self, &Orbinaut->hitboxOrb)) {
                        Player_Hurt(player, self);
                    }
                }
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Orbinaut->hitboxBadnik) && Player_CheckBadnikBreak(player, self, false)) {
            int32 angle = self->angle;
            for (int32 i = 0; i < ORBINAUT_ORB_COUNT; ++i) {
                if ((1 << i) & self->activeOrbs) {
                    self->position.x    = self->orbPositions[i].x;
                    self->position.y    = self->orbPositions[i].y;
                    EntityOrbinaut *orb = CREATE_ENTITY(Orbinaut, INT_TO_VOID(true), self->orbPositions[i].x, self->orbPositions[i].y);

                    orb->state      = Orbinaut_State_OrbDebris;
                    orb->velocity.x = 0x380 * RSDK.Cos256(angle);
                    orb->velocity.y = 0x380 * RSDK.Sin256(angle);
                }
                angle += (0x100 / ORBINAUT_ORB_COUNT);
            }
            destroyEntity(self);
        }
    }
}

void Orbinaut_HandleRotation(void)
{
    RSDK_THIS(Orbinaut);

    int32 angle = self->angle;
    if (self->direction)
        self->angle = (angle - 1) & 0xFF;
    else
        self->angle = (angle + 1) & 0xFF;

    for (int32 i = 0; i < ORBINAUT_ORB_COUNT; ++i) {
        if ((1 << i) & self->activeOrbs) {
            self->orbPositions[i].x = (RSDK.Cos256(angle) << 12) + self->position.x;
            self->orbPositions[i].y = (RSDK.Sin256(angle) << 12) + self->position.y;
        }
        angle += (0x100 / ORBINAUT_ORB_COUNT);
    }
}

void Orbinaut_CheckOffScreen(void)
{
    RSDK_THIS(Orbinaut);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Orbinaut_Create(NULL);
    }
}

void Orbinaut_State_Init(void)
{
    RSDK_THIS(Orbinaut);

    self->active = ACTIVE_NORMAL;
    self->state  = Orbinaut_State_Moving;
    Orbinaut_State_Moving();
}

void Orbinaut_State_Moving(void)
{
    RSDK_THIS(Orbinaut);

    self->position.x += self->velocity.x;

    Orbinaut_HandleRotation();
    Orbinaut_HandlePlayerInteractions();

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
            self->state                = Orbinaut_State_ReleasingOrbs;
            self->animatorFace.frameID = 1;
        }

        self->direction = playerPtr->position.x >= self->position.x;
    }

    Orbinaut_CheckOffScreen();
}

void Orbinaut_State_ReleasingOrbs(void)
{
    RSDK_THIS(Orbinaut);

    uint8 angle = self->angle;
    Orbinaut_HandleRotation();

    for (int32 i = 0; i < ORBINAUT_ORB_COUNT; ++i) {
        if (angle == 64) {
            if ((1 << i) & self->activeOrbs) {
                self->activeOrbs &= ~(1 << i);
                EntityOrbinaut *orb = CREATE_ENTITY(Orbinaut, INT_TO_VOID(true), self->orbPositions[i].x, self->orbPositions[i].y);
                if (self->direction == FLIP_NONE)
                    orb->velocity.x = -0x20000;
                else
                    orb->velocity.x = 0x20000;
            }
        }
        angle += (0x100 / ORBINAUT_ORB_COUNT);
    }
    Orbinaut_HandlePlayerInteractions();

    if (!self->activeOrbs) {
        self->state = Orbinaut_State_Orbless;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x4000;
        else
            self->velocity.x = 0x4000;
    }

    if (self->animatorFace.timer >= 0x10)
        self->animatorFace.frameID = 2;
    else
        self->animatorFace.timer++;

    Orbinaut_CheckOffScreen();
}

void Orbinaut_State_Orbless(void)
{
    RSDK_THIS(Orbinaut);

    self->position.x += self->velocity.x;

    Orbinaut_HandlePlayerInteractions();
    Orbinaut_CheckOffScreen();
}

void Orbinaut_State_Orb(void)
{
    RSDK_THIS(Orbinaut);

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->position.x += self->velocity.x;

        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == (uint8)((self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionTouch(player, self, &Orbinaut->hitboxOrb)) {
                    Player_Hurt(player, self);
                }
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void Orbinaut_State_OrbDebris(void)
{
    RSDK_THIS(Orbinaut);

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
        self->velocity.y += 0x3800;
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Orbinaut_EditorDraw(void)
{
    RSDK_THIS(Orbinaut);

    int32 angle = self->angle;
    Orbinaut_HandleRotation();
    self->angle = angle;
    
    Orbinaut_Draw();
}

void Orbinaut_EditorLoad(void)
{
    Orbinaut->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Orbinaut.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Orbinaut, planeFilter);
    RSDK_ENUM_VAR("None", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("AL", PLANEFILTER_AL);
    RSDK_ENUM_VAR("BL", PLANEFILTER_BL);
    RSDK_ENUM_VAR("AH", PLANEFILTER_AH);
    RSDK_ENUM_VAR("BH", PLANEFILTER_BH);

    RSDK_ACTIVE_VAR(Orbinaut, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Orbinaut_Serialize(void)
{
    RSDK_EDITABLE_VAR(Orbinaut, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Orbinaut, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(Orbinaut, VAR_BOOL, fireOrbs);
}
