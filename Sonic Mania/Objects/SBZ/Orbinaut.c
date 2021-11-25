#include "SonicMania.h"

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
    for (int32 o = 0; o < Orbinaut_MaxOrbs; ++o) {
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
        self->drawOrder = Zone->drawOrderHigh;
    else
        self->drawOrder = Zone->drawOrderLow;
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
        self->state = Orbinaut_State_Setup;
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
    if (RSDK.CheckStageFolder("MMZ"))
        Orbinaut->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Orbinaut.bin", SCOPE_STAGE);
    Orbinaut->hitbox1.left   = -8;
    Orbinaut->hitbox1.top    = -8;
    Orbinaut->hitbox1.right  = 8;
    Orbinaut->hitbox1.bottom = 8;
    Orbinaut->hitbox2.left   = -4;
    Orbinaut->hitbox2.top    = -4;
    Orbinaut->hitbox2.right  = 4;
    Orbinaut->hitbox2.bottom = 4;
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
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Orbinaut_HandlePlayerInteractions(void)
{
    RSDK_THIS(Orbinaut);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    for (int32 i = 0; i < Orbinaut_MaxOrbs; ++i) {
        if ((1 << i) & self->activeOrbs) {
            self->position.x = self->orbPositions[i].x;
            self->position.y = self->orbPositions[i].y;
            foreach_active(Player, player)
            {
                if (self->planeFilter <= 0 || player->collisionPlane == (uint8)((self->planeFilter - 1) & 1)) {
                    if (Player_CheckCollisionTouch(player, self, &Orbinaut->hitbox2)) {
                        Player_CheckHit(player, self);
                    }
                }
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Orbinaut->hitbox1) && Player_CheckBadnikBreak(self, player, false)) {
            int32 angle = self->angle;
            for (int32 i = 0; i < Orbinaut_MaxOrbs; ++i) {
                if ((1 << i) & self->activeOrbs) {
                    self->position.x = self->orbPositions[i].x;
                    self->position.y = self->orbPositions[i].y;
                    EntityOrbinaut *orb = CREATE_ENTITY(Orbinaut, intToVoid(1), self->orbPositions[i].x, self->orbPositions[i].y);

                    orb->state = Orbinaut_Unknown10;
                    orb->velocity.x = 0x380 * RSDK.Cos256(angle);
                    orb->velocity.y = 0x380 * RSDK.Sin256(angle);
                }
                angle += (0x100 / Orbinaut_MaxOrbs);
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

    for (int32 i = 0; i < Orbinaut_MaxOrbs; ++i) {
        if ((1 << i) & self->activeOrbs) {
            self->orbPositions[i].x = (RSDK.Cos256(angle) << 12) + self->position.x;
            self->orbPositions[i].y = (RSDK.Sin256(angle) << 12) + self->position.y;
        }
        angle += (0x100 / Orbinaut_MaxOrbs);
    }
}

void Orbinaut_CheckOnScreen(void)
{
    RSDK_THIS(Orbinaut);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Orbinaut_Create(NULL);
    }
}

void Orbinaut_State_Setup(void)
{
    RSDK_THIS(Orbinaut);
    self->active = ACTIVE_NORMAL;
    self->state  = Orbinaut_Unknown6;
    Orbinaut_Unknown6();
}

void Orbinaut_Unknown6(void)
{
    RSDK_THIS(Orbinaut);

    self->position.x += self->velocity.x;
    Orbinaut_HandleRotation();
    Orbinaut_HandlePlayerInteractions();

    if (self->fireOrbs) {
        EntityPlayer *playerPtr = NULL;
        int32 distanceX           = 0x7FFFFFFF;
        int32 distanceY           = 0x7FFFFFFF;
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
            self->state             = Orbinaut_Unknown7;
            self->animatorFace.frameID = 1;
        }

        self->direction = playerPtr->position.x >= self->position.x;
    }

    Orbinaut_CheckOnScreen();
}

void Orbinaut_Unknown7(void)
{
    RSDK_THIS(Orbinaut);

    int32 angle = self->angle;
    Orbinaut_HandleRotation();

    for (int32 i = 0; i < Orbinaut_MaxOrbs; ++i) {
        if (angle == 64) {
            if ((1 << i) & self->activeOrbs) {
                self->activeOrbs &= ~(1 << i);
                EntityOrbinaut *sol = CREATE_ENTITY(Orbinaut, intToVoid(true), self->orbPositions[i].x, self->orbPositions[i].y);
                if (self->direction == FLIP_NONE)
                    sol->velocity.x = -0x20000;
                else
                    sol->velocity.x = 0x20000;
            }
            angle += (0x100 / Orbinaut_MaxOrbs);
        }
    }
    Orbinaut_HandlePlayerInteractions();

    if (!self->activeOrbs) {
        self->state = Orbinaut_Unknown8;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x4000;
        else
            self->velocity.x = 0x4000;
    }

    if (self->animatorFace.timer >= 0x10)
        self->animatorFace.frameID = 2;
    else
        self->animatorFace.timer++;

    Orbinaut_CheckOnScreen();
}

void Orbinaut_Unknown8(void)
{
    RSDK_THIS(Orbinaut);
    self->position.x += self->velocity.x;
    Orbinaut_HandlePlayerInteractions();
    Orbinaut_CheckOnScreen();
}

void Orbinaut_State_Orb(void)
{
    RSDK_THIS(Orbinaut);
    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->position.x += self->velocity.x;

        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == (uint8)((self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionTouch(player, self, &Sol->hitbox2)) {
                    Player_CheckElementalHit(player, self, SHIELD_FIRE);
                }
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void Orbinaut_Unknown10(void)
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
void Orbinaut_EditorDraw(void) {}

void Orbinaut_EditorLoad(void) {}
#endif

void Orbinaut_Serialize(void)
{
    RSDK_EDITABLE_VAR(Orbinaut, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Orbinaut, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(Orbinaut, VAR_BOOL, fireOrbs);
}
