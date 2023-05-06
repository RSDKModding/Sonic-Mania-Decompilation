// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Ball Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBall *Ball = NULL;

void Ball_Update(void)
{
    RSDK_THIS(Ball);

    StateMachine_Run(self->state);
}

void Ball_LateUpdate(void) {}

void Ball_StaticUpdate(void) {}

void Ball_Draw(void)
{
    RSDK_THIS(Ball);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Ball_Create(void *data)
{
    RSDK_THIS(Ball);

    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (data) {
        RSDK.SetSpriteAnimation(Ball->aniFrames, 2, &self->animator, true, 0);
        self->state = Ball_State_Splash;
    }
    else {
        self->startPos = self->position;
        if (!self->type) {
            RSDK.SetSpriteAnimation(Ball->aniFrames, 0, &self->animator, true, 0);
            self->state = Ball_State_Init;
        }
        else {
            self->state = Ball_State_Spawner;
        }
    }
}

void Ball_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("CPZ"))
        Ball->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Ball.bin", SCOPE_STAGE);

    Ball->hitboxBall.left   = -10;
    Ball->hitboxBall.top    = -10;
    Ball->hitboxBall.right  = 10;
    Ball->hitboxBall.bottom = 10;

    Ball->hitboxRange.left   = -128;
    Ball->hitboxRange.top    = -128;
    Ball->hitboxRange.right  = 128;
    Ball->hitboxRange.bottom = 128;

    Ball->sfxSplash = RSDK.GetSfx("Stage/Splash2.wav");

    DEBUGMODE_ADD_OBJ(Ball);
}

void Ball_DebugSpawn(void)
{
    RSDK_THIS(Ball);

    CREATE_ENTITY(Ball, NULL, self->position.x, self->position.y);
}

void Ball_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Ball->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Ball_HandleInteractions(void)
{
    RSDK_THIS(Ball);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Ball->hitboxBall)) {
            Player_Hurt(player, self);

            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);

            self->velocity.y = 0;
            RSDK.SetSpriteAnimation(Ball->aniFrames, 1, &self->animator, true, 0);
            self->state = Ball_State_ChemicalDrop;
        }
    }
}

void Ball_CheckOffScreen(void)
{
    RSDK_THIS(Ball);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position   = self->startPos;
        self->direction  = FLIP_NONE;
        self->velocity.x = 0;
        self->velocity.y = 0;
        Ball_Create(NULL);
    }
}

void Ball_SpawnSplashes(void)
{
    RSDK_THIS(Ball);

    RSDK.PlaySfx(Ball->sfxSplash, false, 255);

    for (int32 i = 0; i < 5; ++i) {
        EntityBall *ball = CREATE_ENTITY(Ball, INT_TO_VOID(true), self->position.x, self->position.y);
        ball->drawGroup  = Zone->objectDrawGroup[1];
        ball->velocity.x = RSDK.Rand(-0x100, 0x100) << 10;
        if (ball->velocity.x < 0)
            ball->velocity.x += 0x20000;

        ball->velocity.x -= 0x10000;
        if (i > 0)
            ball->velocity.y = RSDK.Rand(-0x400, 0x400) << 8;
        ball->velocity.y -= self->velocity.y >> 1;
    }

    destroyEntity(self);
}

void Ball_State_Init(void)
{
    RSDK_THIS(Ball);

    self->active = ACTIVE_NORMAL;

    self->state = Ball_State_AwaitPlayer;
    Ball_State_AwaitPlayer();
}

void Ball_State_AwaitPlayer(void)
{
    RSDK_THIS(Ball);

    if (self->direction == FLIP_X) {
        self->velocity.y += 0x800;
        if (self->velocity.y >= 0xC000)
            self->direction = FLIP_NONE;
    }
    else {
        self->velocity.y -= 0x800;
        if (self->velocity.y <= -0xC000)
            self->direction = FLIP_X;
    }
    self->position.y += self->velocity.y;

    RSDK.ProcessAnimation(&self->animator);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Ball->hitboxRange)) {
            self->targetPlayer = player;
            self->state        = Ball_State_TargetingPlayer;
        }
    }

    Ball_HandleInteractions();
    Ball_CheckOffScreen();
}

void Ball_State_TargetingPlayer(void)
{
    RSDK_THIS(Ball);

    EntityPlayer *targetPlayer = self->targetPlayer;
    if (self->position.x <= targetPlayer->position.x) {
        self->velocity.x += 0x1000;
        if (self->velocity.x > 0x20000)
            self->velocity.x = 0x20000;
    }
    else {
        self->velocity.x -= 0x1000;
        if (self->velocity.x < -0x20000)
            self->velocity.x = -0x20000;
    }

    if (self->position.y <= targetPlayer->position.y - 0x400000) {
        self->velocity.y += 0x1000;
        if (self->velocity.y > 0x20000)
            self->velocity.y = 0x20000;
    }
    else {
        self->velocity.y -= 0x1000;
        if (self->velocity.y < -0x20000)
            self->velocity.y = -0x20000;
    }

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (abs(self->position.x - targetPlayer->position.x) < 0x100000 && abs(0x500000 + self->position.y - targetPlayer->position.y) < 0x100000) {
        if (RSDK.CheckOnScreen(self, &self->updateRange)) {
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);

            self->velocity.y = 0;
            RSDK.SetSpriteAnimation(Ball->aniFrames, 1, &self->animator, true, 0);
            self->state = Ball_State_ChemicalDrop;
        }
    }

    RSDK.ProcessAnimation(&self->animator);

    Ball_HandleInteractions();
    Ball_CheckOffScreen();
}

void Ball_State_ChemicalDrop(void)
{
    RSDK_THIS(Ball);

    RSDK.ProcessAnimation(&self->animator);

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xB0000, false)) {
        Ball_SpawnSplashes();
    }
    else {
        self->position.y += self->velocity.y;
        self->velocity.y += 0x3800;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Ball->hitboxBall)) {
                Player_ElementHurt(player, self, SHIELD_BUBBLE);

                Ball_SpawnSplashes();
            }
        }

        if (!RSDK.CheckOnScreen(self, &self->updateRange))
            destroyEntity(self);
    }
}

void Ball_State_Splash(void)
{
    RSDK_THIS(Ball);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void Ball_State_StraightMovement(void)
{
    RSDK_THIS(Ball);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->velocity.x < 0)
        self->velocity.x += 0x400;
    if (self->velocity.x > 0)
        self->velocity.x -= 0x400;

    if (self->velocity.y < 0)
        self->velocity.y += 0x400;
    if (self->velocity.y > 0)
        self->velocity.y -= 0x400;

    if (!self->velocity.x && !self->velocity.y) {
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
        self->state      = Ball_State_AwaitPlayer;
    }

    Ball_HandleInteractions();
}

void Ball_State_Spawner(void)
{
    RSDK_THIS(Ball);

    EntityBall *child = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, Ball);
    if (child->classID != Ball->classID) {
        RSDK.ResetEntity(child, Ball->classID, NULL);
        child->active     = ACTIVE_NORMAL;
        child->position.x = self->position.x;
        child->position.y = self->position.y;
        child->state      = Ball_State_StraightMovement;
        switch (self->type) {
            case BALL_SPAWN_LEFT: child->velocity.x = -0x20000; break;
            case BALL_SPAWN_UP: child->velocity.y = -0x20000; break;
            case BALL_SPAWN_RIGHT: child->velocity.x = 0x20000; break;
            case BALL_SPAWN_DOWN: child->velocity.y = 0x20000; break;
            default: break;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void Ball_EditorDraw(void)
{
    RSDK_THIS(Ball);

    self->startPos = self->position;
    RSDK.SetSpriteAnimation(Ball->aniFrames, 0, &self->animator, true, 0);

    Ball_Draw();

    if (showGizmos()) {
        switch (self->type) {
            case BALL_SPAWN_LEFT:
                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x - 0x200000, self->position.y, 0xFFFF00, INK_NONE, 0xFF);
                break;

            case BALL_SPAWN_UP:
                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x, self->position.y - 0x200000, 0xFFFF00, INK_NONE, 0xFF);
                break;

            case BALL_SPAWN_RIGHT:
                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x + 0x200000, self->position.y, 0xFFFF00, INK_NONE, 0xFF);
                break;

            case BALL_SPAWN_DOWN:
                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x, self->position.y + 0x200000, 0xFFFF00, INK_NONE, 0xFF);
                break;

            default: break;
        }
    }
}

void Ball_EditorLoad(void)
{
    Ball->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Ball.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Ball, type);
    RSDK_ENUM_VAR("Single Ball", BALL_SINGLE);
    RSDK_ENUM_VAR("Spawner (Left)", BALL_SPAWN_LEFT);
    RSDK_ENUM_VAR("Spawner (Up)", BALL_SPAWN_UP);
    RSDK_ENUM_VAR("Spawner (Right)", BALL_SPAWN_RIGHT);
    RSDK_ENUM_VAR("Spawner (Down)", BALL_SPAWN_DOWN);
}
#endif

void Ball_Serialize(void) { RSDK_EDITABLE_VAR(Ball, VAR_UINT8, type); }
