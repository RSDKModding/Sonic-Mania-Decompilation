// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Dragonfly Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDragonfly *Dragonfly;

void Dragonfly_Update(void)
{
    RSDK_THIS(Dragonfly);

    StateMachine_Run(self->state);
}

void Dragonfly_LateUpdate(void) {}

void Dragonfly_StaticUpdate(void) {}

void Dragonfly_Draw(void)
{
    RSDK_THIS(Dragonfly);

    if (self->animator.animationID == 3) {
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
    else {
        for (int32 i = 0; i < DRAGONFLY_SPINE_COUNT; ++i) {
            self->bodyAnimator.frameID = i == 0;
            self->direction            = self->directions[i];
            RSDK.DrawSprite(&self->bodyAnimator, &self->positions[i], false);
        }

        self->direction = false;
        RSDK.DrawSprite(&self->animator, NULL, false);

        self->inkEffect = INK_ALPHA;
        RSDK.DrawSprite(&self->wingAnimator, NULL, false);

        self->inkEffect = INK_NONE;
    }
}

void Dragonfly_Create(void *data)
{
    RSDK_THIS(Dragonfly);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];

    if (!SceneInfo->inEditor) {
        if (!self->speed) {
            self->dist  = 0x40;
            self->speed = 0x04;
        }
        self->drawFX |= FX_FLIP;

        if (data) {
            self->active        = ACTIVE_NORMAL;
            self->updateRange.x = 0x100000;
            self->updateRange.y = 0x100000;
            RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 3, &self->animator, true, 0);
            self->state = Dragonfly_State_Debris;
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = (self->dist + 0x80) << 16;
            self->startPos      = self->position;
            self->alpha         = 0x80;

            RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 2, &self->wingAnimator, true, 0);
            RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 3, &self->bodyAnimator, true, 0);
            self->state = Dragonfly_State_Init;
        }
    }
}

void Dragonfly_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        Dragonfly->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Dragonfly.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        Dragonfly->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Dragonfly.bin", SCOPE_STAGE);

    Dragonfly->hitboxBadnik.left   = -8;
    Dragonfly->hitboxBadnik.top    = -8;
    Dragonfly->hitboxBadnik.right  = 8;
    Dragonfly->hitboxBadnik.bottom = 8;

    Dragonfly->hitboxSpine.left   = -5;
    Dragonfly->hitboxSpine.top    = -3;
    Dragonfly->hitboxSpine.right  = 5;
    Dragonfly->hitboxSpine.bottom = 2;

    DEBUGMODE_ADD_OBJ(Dragonfly);
}

void Dragonfly_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

// rdc would probably not shut the fuck up if i didn't add this /hj
void Dragonfly_CheckOffScreen(void)
{
    RSDK_THIS(Dragonfly);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        Dragonfly_Create(NULL);
    }
}

void Dragonfly_DebugSpawn(void)
{
    RSDK_THIS(Dragonfly);

    CREATE_ENTITY(Dragonfly, NULL, self->position.x, self->position.y);
}

void Dragonfly_CheckPlayerCollisions(void)
{
    RSDK_THIS(Dragonfly);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Dragonfly->hitboxBadnik)) {
            if (Player_CheckBadnikBreak(player, self, false)) {
                for (int32 i = 0; i < DRAGONFLY_SPINE_COUNT; ++i) {
                    EntityDragonfly *child  = CREATE_ENTITY(Dragonfly, INT_TO_VOID(true), self->positions[i].x, self->positions[i].y);
                    child->animator.frameID = i == 0;
                    child->velocity.x       = RSDK.Rand(-4, 4) << 15;
                    child->velocity.y       = RSDK.Rand(-5, 1) << 15;
                }

                destroyEntity(self);
            }
        }
        else {
            Vector2 storePos = self->position;
            for (int32 i = 0; i < DRAGONFLY_SPINE_COUNT; ++i) {
                self->position = self->positions[i];
                if (Player_CheckCollisionTouch(player, self, &Dragonfly->hitboxSpine)) {
                    self->position = storePos;
                    Player_Hurt(player, self);
                }
            }

            self->position = storePos;
        }
    }
}

void Dragonfly_State_Init(void)
{
    RSDK_THIS(Dragonfly);
    self->active = ACTIVE_NORMAL;

    if (!self->dir) {
        self->angle = 0;
        RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 0, &self->animator, true, 0);
    }
    else {
        self->angle = 0x200;
        RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 1, &self->animator, true, 0);
    }

    self->state = Dragonfly_State_Move;
    Dragonfly_State_Move();
}

void Dragonfly_State_Move(void)
{
    RSDK_THIS(Dragonfly);

    self->angle += self->speed;

    self->position.x = self->startPos.x + 0xC00 * RSDK.Cos256(self->angle + 0x40);
    self->position.y = self->startPos.y + (self->dist << 6) * RSDK.Sin1024(self->angle);

    int32 currentAngle = self->angle - (((DRAGONFLY_SPINE_COUNT * 13) + 13) - 1);
    for (int32 i = 0; i < DRAGONFLY_SPINE_COUNT; ++i) {
        self->directions[i] = ((currentAngle + 0x100) & 0x3FF) < 0x200 ? 2 : 0;

        self->positions[i].x = self->startPos.x + 0xC00 * RSDK.Cos256(currentAngle + 0x40);
        self->positions[i].y = self->startPos.y + (self->dist << 6) * RSDK.Sin1024(currentAngle);

        currentAngle += 13;
    }

    bool32 dir = ((self->angle + 0x100) & 0x3FF) < 0x200;
    if (self->animator.animationID == 1) {
        if (dir)
            RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 0, &self->animator, true, 0);
    }
    else if (!dir)
        RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 1, &self->animator, true, 0);

    RSDK.ProcessAnimation(&self->animator);
    RSDK.ProcessAnimation(&self->wingAnimator);

    Dragonfly_CheckPlayerCollisions();
    Dragonfly_CheckOffScreen();
}

void Dragonfly_State_Debris(void)
{
    RSDK_THIS(Dragonfly);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->visible ^= true;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Dragonfly_EditorDraw(void)
{
    RSDK_THIS(Dragonfly);

    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = (self->dist + 0x80) << 16;
    self->startPos      = self->position;
    self->alpha         = 0x80;

    RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 2, &self->wingAnimator, true, 0);
    RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 3, &self->bodyAnimator, true, 0);

    if (!self->dir) {
        self->angle = 0;
        RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 0, &self->animator, true, 4);
    }
    else {
        self->angle = 0x200;
        RSDK.SetSpriteAnimation(Dragonfly->aniFrames, 1, &self->animator, true, 4);
    }

    self->position.x = self->startPos.x + 0xC00 * RSDK.Cos256(self->angle + 0x40);
    self->position.y = self->startPos.y + (self->dist << 6) * RSDK.Sin1024(self->angle);

    int32 currentAngle = self->angle - (((DRAGONFLY_SPINE_COUNT * 13) + 13) - 1);
    for (int32 i = 0; i < DRAGONFLY_SPINE_COUNT; ++i) {
        self->directions[i] = ((currentAngle + 0x100) & 0x3FF) < 0x200 ? 2 : 0;

        self->positions[i].x = self->startPos.x + 0xC00 * RSDK.Cos256(currentAngle + 0x40);
        self->positions[i].y = self->startPos.y + (self->dist << 6) * RSDK.Sin1024(currentAngle);
        currentAngle += 13;
    }

    Dragonfly_Draw();

    self->position = self->startPos;
}

void Dragonfly_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        Dragonfly->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Dragonfly.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        Dragonfly->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Dragonfly.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Dragonfly, dir);
    RSDK_ENUM_VAR("Down", FLIP_NONE);
    RSDK_ENUM_VAR("Up", FLIP_X);
}
#endif

void Dragonfly_Serialize(void)
{
    RSDK_EDITABLE_VAR(Dragonfly, VAR_UINT8, dir);
    RSDK_EDITABLE_VAR(Dragonfly, VAR_UINT8, dist);
    RSDK_EDITABLE_VAR(Dragonfly, VAR_UINT8, speed);
}