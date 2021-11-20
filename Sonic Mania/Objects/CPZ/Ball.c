#include "SonicMania.h"

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
    self->drawOrder     = Zone->drawOrderLow;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (data) {
        RSDK.SetSpriteAnimation(Ball->aniFrames, 2, &self->animator, true, 0);
        self->state = Ball_State2_Unknown1;
    }
    else {
        self->startPos = self->position;
        if (!self->type) {
            RSDK.SetSpriteAnimation(Ball->aniFrames, 0, &self->animator, true, 0);
            self->state = Ball_State_Setup;
        }
        else {
            self->state = Ball_State4_Unknown1;
        }
    }
}

void Ball_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ"))
        Ball->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Ball.bin", SCOPE_STAGE);
    Ball->hitbox1.left   = -10;
    Ball->hitbox1.top    = -10;
    Ball->hitbox1.right  = 10;
    Ball->hitbox1.bottom = 10;
    Ball->hitbox2.left   = -128;
    Ball->hitbox2.top    = -128;
    Ball->hitbox2.right  = 128;
    Ball->hitbox2.bottom = 128;
    Ball->sfxSplash      = RSDK.GetSfx("Stage/Splash2.wav");
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
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Ball_HandleInteractions(void)
{
    RSDK_THIS(Ball);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Ball->hitbox1)) {
            Player_CheckHit(player, self);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
            self->velocity.y = 0;
            RSDK.SetSpriteAnimation(Ball->aniFrames, 1, &self->animator, true, 0);
            self->state = Ball_State_Unknown3;
        }
    }
}

void Ball_CheckOnScreen(void)
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

void Ball_SpawnChildren(void)
{
    RSDK_THIS(Ball);
    RSDK.PlaySfx(Ball->sfxSplash, false, 255);
    for (int32 i = 0; i < 5; ++i) {
        EntityBall *ball = CREATE_ENTITY(Ball, intToVoid(true), self->position.x, self->position.y);
        ball->drawOrder  = Zone->drawOrderHigh;
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

void Ball_State_Setup(void)
{
    RSDK_THIS(Ball);
    self->active = ACTIVE_NORMAL;
    self->state  = Ball_State_Unknown1;
    Ball_State_Unknown1();
}

void Ball_State_Unknown1(void)
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
        if (Player_CheckCollisionTouch(player, self, &Ball->hitbox2)) {
            self->playerPtr = (Entity *)player;
            self->state     = Ball_State_Unknown2;
        }
    }
    Ball_HandleInteractions();
    Ball_CheckOnScreen();
}

void Ball_State_Unknown2(void)
{
    RSDK_THIS(Ball);
    Entity *playerPtr = self->playerPtr;
    if (self->position.x <= playerPtr->position.x) {
        self->velocity.x += 0x1000;
        if (self->velocity.x > 0x20000)
            self->velocity.x = 0x20000;
    }
    else {
        self->velocity.x -= 0x1000;
        if (self->velocity.x < -0x20000)
            self->velocity.x = -0x20000;
    }
    if (self->position.y <= playerPtr->position.y - 0x400000) {
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

    if (abs(self->position.x - playerPtr->position.x) < 0x100000) {
        if (abs(0x500000 + self->position.y - playerPtr->position.y) < 0x100000 && RSDK.CheckOnScreen(self, &self->updateRange)) {
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
            self->velocity.y = 0;
            RSDK.SetSpriteAnimation(Ball->aniFrames, 1, &self->animator, true, 0);
            self->state = Ball_State_Unknown3;
        }
    }
    RSDK.ProcessAnimation(&self->animator);
    Ball_HandleInteractions();
    Ball_CheckOnScreen();
}

void Ball_State_Unknown3(void)
{
    RSDK_THIS(Ball);
    RSDK.ProcessAnimation(&self->animator);
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xB0000, false)) {
        Ball_SpawnChildren();
    }
    else {
        self->position.y += self->velocity.y;
        self->velocity.y += 0x3800;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Ball->hitbox1)) {
                Player_CheckElementalHit(player, self, SHIELD_BUBBLE);
                Ball_SpawnChildren();
            }
        }

        if (!RSDK.CheckOnScreen(self, &self->updateRange))
            destroyEntity(self);
    }
}

void Ball_State2_Unknown1(void)
{
    RSDK_THIS(Ball);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void Ball_State3_Unknown1(void)
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
        self->state      = Ball_State_Unknown1;
    }
    Ball_HandleInteractions();
}

void Ball_State4_Unknown1(void)
{
    RSDK_THIS(Ball);
    EntityBall *child = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, Ball);
    if (child->objectID != Ball->objectID) {
        RSDK.ResetEntityPtr(child, Ball->objectID, NULL);
        child->active     = ACTIVE_NORMAL;
        child->position.x = self->position.x;
        child->position.y = self->position.y;
        child->state      = Ball_State3_Unknown1;
        switch (self->type) {
            case 1: child->velocity.x = -0x20000; break;
            case 2:
                child->velocity.y = -0x20000;
                child->state      = Ball_State3_Unknown1;
                break;
            case 3:
                child->velocity.x = 0x20000;
                child->state      = Ball_State3_Unknown1;
                break;
            case 4:
                child->velocity.y = 0x20000;
                child->state      = Ball_State3_Unknown1;
                break;
            default: break;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Ball_EditorDraw(void)
{
    RSDK_THIS(Ball);

    self->startPos = self->position;
    if (!self->type) 
        RSDK.SetSpriteAnimation(Ball->aniFrames, 0, &self->animator, true, 0);
    else 
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator, true, 0);

    Ball_Draw();
}

void Ball_EditorLoad(void) { Ball->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Ball.bin", SCOPE_STAGE); }
#endif

void Ball_Serialize(void) { RSDK_EDITABLE_VAR(Ball, VAR_UINT8, type); }
