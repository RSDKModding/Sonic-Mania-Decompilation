// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CircleBumper Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectCircleBumper *CircleBumper;

void CircleBumper_Update(void)
{
    RSDK_THIS(CircleBumper);
    StateMachine_Run(self->stateMove);
    StateMachine_Run(self->stateCollide);
}

void CircleBumper_LateUpdate(void) {}

void CircleBumper_StaticUpdate(void) {}

void CircleBumper_Draw(void)
{
    RSDK_THIS(CircleBumper);
    RSDK.DrawSprite(&self->animator, &self->originPos, false);
}

void CircleBumper_Create(void *data)
{
    RSDK_THIS(CircleBumper);
    self->visible     = true;
    self->drawOrder   = Zone->drawOrderHigh;
    self->originPos.x = self->position.x;
    self->originPos.y = self->position.y;
    self->active      = ACTIVE_BOUNDS;
    if (!SceneInfo->inEditor) {
        self->amplitude.x >>= 10;
        self->amplitude.y >>= 10;
    }

    switch (self->type) {
        default:
        case CIRCLEBUMPER_FIXED:
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &self->animator, true, 0);
            self->stateMove = CircleBumper_Move_Fixed;
            break;
        case CIRCLEBUMPER_MOVING:
            self->updateRange.x = (abs(self->amplitude.x) + 0x1000) << 10;
            self->stateMove     = CircleBumper_Move_Moving;
            self->updateRange.y = (abs(self->amplitude.y) + 0x1000) << 10;
            break;
        case CIRCLEBUMPER_CIRCLE:
            self->updateRange.x = (abs(self->amplitude.x) + 0x1000) << 10;
            self->updateRange.y = (abs(self->amplitude.y) + 0x1000) << 10;
            RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &self->animator, true, 0);
            self->stateMove = CircleBumper_Move_Circular;
            break;
        case CIRCLEBUMPER_TRACK:
            self->updateRange.x = (abs(self->amplitude.x) + 0x2000) << 9;
            self->stateMove     = CircleBumper_Move_Track;
            self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 9;
            break;
        case CIRCLEBUMPER_PATH:
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            self->active        = ACTIVE_NEVER;
            self->stateMove     = CircleBumper_Move_Path;
            break;
    }

    RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &self->animator, true, 0);
    self->stateCollide = CircleBumper_Collide_Normal;
    if (!self->hitCount)
        self->hitCount = 5;
}

void CircleBumper_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SPZ2"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("Blueprint"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/CircleBumper.bin", SCOPE_STAGE);

    CircleBumper->hitbox.left   = -7;
    CircleBumper->hitbox.top    = -6;
    CircleBumper->hitbox.right  = 7;
    CircleBumper->hitbox.bottom = 6;

    CircleBumper->sfxBumper = RSDK.GetSfx("Stage/Bumper.wav");

    DEBUGMODE_ADD_OBJ(CircleBumper);
}

void CircleBumper_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void CircleBumper_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(CircleBumper, NULL, self->position.x, self->position.y);
}

void CircleBumper_CheckPlayerCollisions(void)
{
    RSDK_THIS(CircleBumper);
    int32 storeX     = self->position.x;
    int32 storeY     = self->position.y;
    self->position.x = self->originPos.x;
    self->position.y = self->originPos.y;
    foreach_active(Player, player)
    {
        if (player->animator.animationID != ANI_HURT && Player_CheckBadnikTouch(player, self, &CircleBumper->hitbox)) {
            self->animator.frameID = 0;
            self->stateCollide     = CircleBumper_Collide_Bumped;
            RSDK.PlaySfx(CircleBumper->sfxBumper, false, 0xFF);
            self->active = ACTIVE_NORMAL;
            int32 angle  = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
            int32 xVel   = 0x700 * RSDK.Cos256(angle);
            int32 yVel   = 0x700 * RSDK.Sin256(angle);
            if (player->state == Player_State_FlyCarried)
                RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            }
            else {
#endif
                int32 anim = player->animator.animationID;
                if (anim != ANI_FLY && anim != ANI_FLYLIFTTIRED && player->state != Player_State_TailsFlight) {
                    if (player->state != Player_State_DropDash)
                        player->state = Player_State_Air;
                    if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                        player->animator.animationID = ANI_WALK;
                }
#if RETRO_USE_PLUS
            }
#endif
            if (player->animator.animationID != ANI_FLY) {
                player->velocity.x  = xVel;
                player->groundVel   = xVel;
                player->jumpAbility = 0;
            }
            player->velocity.y     = yVel;
            player->onGround       = false;
            player->tileCollisions = true;
            if (self->hitCount) {
                EntityScoreBonus *bonus = CREATE_ENTITY(ScoreBonus, NULL, self->position.x, self->position.y);
                bonus->animator.frameID = 16;
                Player_GiveScore(player, 10);
                --self->hitCount;
            }
        }
    }
    self->position.x = storeX;
    self->position.y = storeY;
}

void CircleBumper_Collide_Normal(void) { CircleBumper_CheckPlayerCollisions(); }

void CircleBumper_Collide_Bumped(void)
{
    RSDK_THIS(CircleBumper);
    CircleBumper_CheckPlayerCollisions();
    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->animator.frameID = 0;
        self->active           = ACTIVE_BOUNDS;
        self->stateCollide     = CircleBumper_Collide_Normal;
    }
}

void CircleBumper_Move_Fixed(void)
{
    RSDK_THIS(CircleBumper);
    self->originPos.x = self->position.x;
    self->originPos.y = self->position.y;
}

void CircleBumper_Move_Moving(void)
{
    RSDK_THIS(CircleBumper);
    self->originPos.x = self->amplitude.x * RSDK.Sin1024(self->speed * Zone->timer) + self->position.x;
    self->originPos.y = self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer) + self->position.y;
}

void CircleBumper_Move_Circular(void)
{
    RSDK_THIS(CircleBumper);
    self->originPos.x = self->amplitude.x * RSDK.Cos1024(self->speed * Zone->timer + 4 * self->angle) + self->position.x;
    self->originPos.y = self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer + 4 * self->angle) + self->position.y;
}

void CircleBumper_Move_Path(void)
{
    RSDK_THIS(CircleBumper);
    self->originPos.x += self->velocity.x;
    self->originPos.y += self->velocity.y;
    Entity *ent = RSDK.GetEntityByID(self->speed);

    if (self->velocity.x <= 0) {
        if (self->originPos.x < ent->position.x)
            self->originPos.x = ent->position.x;
    }
    else if (self->originPos.x > ent->position.x) {
        self->originPos.x = ent->position.x;
    }

    if (self->velocity.y <= 0) {
        if (self->originPos.y < ent->position.y)
            self->originPos.y = ent->position.y;
    }
    else if (self->originPos.y > ent->position.y) {
        self->originPos.y = ent->position.y;
    }
}

void CircleBumper_Move_Track(void)
{
    RSDK_THIS(CircleBumper);
    int32 val = Zone->timer << 7;
    if (((val >> 16) & 1) == self->direction) {
        self->originPos.x = self->position.x + (val * self->amplitude.x >> 6) - (self->amplitude.x << 15);
        self->originPos.y = self->position.y + (val * self->amplitude.y >> 6) - (self->amplitude.y << 15);
    }
    else {
        self->originPos.x = self->position.x + (self->amplitude.x << 15) - (val * self->amplitude.x >> 6);
        self->originPos.y = self->position.y + (self->amplitude.y << 15) - (val * self->amplitude.y >> 6);
    }
}

#if RETRO_INCLUDE_EDITOR
void CircleBumper_EditorDraw(void)
{
    RSDK_THIS(CircleBumper);

    switch (self->type) {
        default:
        case CIRCLEBUMPER_FIXED:
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            break;
        case CIRCLEBUMPER_MOVING:
            self->updateRange.x = (abs(self->amplitude.x) + 0x1000) << 10;
            self->updateRange.y = (abs(self->amplitude.y) + 0x1000) << 10;
            break;
        case CIRCLEBUMPER_CIRCLE:
            self->updateRange.x = (abs(self->amplitude.x) + 0x1000) << 10;
            self->updateRange.y = (abs(self->amplitude.y) + 0x1000) << 10;
            break;
        case CIRCLEBUMPER_TRACK:
            self->updateRange.x = (abs(self->amplitude.x) + 0x2000) << 9;
            self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 9;
            break;
        case CIRCLEBUMPER_PATH:
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            break;
    }
    RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &self->animator, true, 0);
    self->originPos = self->position;

    CircleBumper_Draw();
}

void CircleBumper_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SPZ2"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("Blueprint"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/CircleBumper.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(CircleBumper, type);
    RSDK_ENUM_VAR("Fixed", CIRCLEBUMPER_FIXED);
    RSDK_ENUM_VAR("Moving", CIRCLEBUMPER_MOVING);
    RSDK_ENUM_VAR("Circle", CIRCLEBUMPER_CIRCLE);
    RSDK_ENUM_VAR("Track", CIRCLEBUMPER_TRACK);
    RSDK_ENUM_VAR("Path", CIRCLEBUMPER_PATH);
}
#endif

void CircleBumper_Serialize(void)
{
    RSDK_EDITABLE_VAR(CircleBumper, VAR_ENUM, hitCount);
    RSDK_EDITABLE_VAR(CircleBumper, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(CircleBumper, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(CircleBumper, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(CircleBumper, VAR_INT32, angle);
}
