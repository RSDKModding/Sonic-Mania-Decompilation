// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CircleBumper Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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

    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void CircleBumper_Create(void *data)
{
    RSDK_THIS(CircleBumper);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[1];
    self->drawPos   = self->position;
    self->active    = ACTIVE_BOUNDS;
    self->amplitude.x >>= 10;
    self->amplitude.y >>= 10;

    switch (self->type) {
        default:
        case CIRCLEBUMPER_FIXED:
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &self->animator, true, 0);
            self->stateMove = CircleBumper_Move_Fixed;
            break;

        case CIRCLEBUMPER_LINEAR:
            self->updateRange.x = (abs(self->amplitude.x) + 0x1000) << 10;
            self->updateRange.y = (abs(self->amplitude.y) + 0x1000) << 10;
            self->stateMove     = CircleBumper_Move_Linear;
            break;

        case CIRCLEBUMPER_CIRCLE:
            self->updateRange.x = (abs(self->amplitude.x) + 0x1000) << 10;
            self->updateRange.y = (abs(self->amplitude.y) + 0x1000) << 10;
            RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &self->animator, true, 0);
            self->stateMove = CircleBumper_Move_Circular;
            break;

        case CIRCLEBUMPER_TRACK:
            self->updateRange.x = (abs(self->amplitude.x) + 0x2000) << 9;
            self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 9;
            self->stateMove     = CircleBumper_Move_Track;
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
    if (RSDK.CheckSceneFolder("SPZ1"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SPZ2"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("Blueprint"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/CircleBumper.bin", SCOPE_STAGE);

    CircleBumper->hitboxBumper.left   = -7;
    CircleBumper->hitboxBumper.top    = -6;
    CircleBumper->hitboxBumper.right  = 7;
    CircleBumper->hitboxBumper.bottom = 6;

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

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    foreach_active(Player, player)
    {
        if (player->animator.animationID != ANI_HURT && Player_CheckBadnikTouch(player, self, &CircleBumper->hitboxBumper)) {
            self->animator.frameID = 0;
            self->stateCollide     = CircleBumper_Collide_Bumped;
            RSDK.PlaySfx(CircleBumper->sfxBumper, false, 0xFF);
            self->active = ACTIVE_NORMAL;

            int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
            int32 xVel  = 0x700 * RSDK.Cos256(angle);
            int32 yVel  = 0x700 * RSDK.Sin256(angle);
            if (player->state == Player_State_FlyCarried)
                RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

#if MANIA_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            }
            else {
#endif
                int32 anim = player->animator.animationID;
                if (anim != ANI_FLY && anim != ANI_FLY_LIFT_TIRED && player->state != Player_State_TailsFlight) {
                    if (player->state != Player_State_DropDash)
                        player->state = Player_State_Air;
                    if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                        player->animator.animationID = ANI_WALK;
                }
#if MANIA_USE_PLUS
            }
#endif
            if (player->animator.animationID != ANI_FLY) {
                player->velocity.x   = xVel;
                player->groundVel    = xVel;
                player->applyJumpCap = false;
            }

            player->velocity.y     = yVel;
            player->onGround       = false;
            player->tileCollisions = TILECOLLISION_DOWN;

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

    self->drawPos.x = self->position.x;
    self->drawPos.y = self->position.y;
}

void CircleBumper_Move_Linear(void)
{
    RSDK_THIS(CircleBumper);

    self->drawPos.x = self->position.x + self->amplitude.x * RSDK.Sin1024(self->speed * Zone->timer);
    self->drawPos.y = self->position.y + self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer);
}

void CircleBumper_Move_Circular(void)
{
    RSDK_THIS(CircleBumper);

    self->drawPos.x = self->position.x + self->amplitude.x * RSDK.Cos1024(self->speed * Zone->timer + 4 * self->angle);
    self->drawPos.y = self->position.y + self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer + 4 * self->angle);
}

void CircleBumper_Move_Path(void)
{
    RSDK_THIS(CircleBumper);

    self->drawPos.x += self->velocity.x;
    self->drawPos.y += self->velocity.y;
    Entity *node = RSDK_GET_ENTITY_GEN(self->speed);

    if (self->velocity.x <= 0) {
        if (self->drawPos.x < node->position.x)
            self->drawPos.x = node->position.x;
    }
    else if (self->drawPos.x > node->position.x) {
        self->drawPos.x = node->position.x;
    }

    if (self->velocity.y <= 0) {
        if (self->drawPos.y < node->position.y)
            self->drawPos.y = node->position.y;
    }
    else if (self->drawPos.y > node->position.y) {
        self->drawPos.y = node->position.y;
    }
}

void CircleBumper_Move_Track(void)
{
    RSDK_THIS(CircleBumper);

    int32 timer = Zone->timer << 7;
    if (((timer >> 16) & 1) == self->direction) {
        self->drawPos.x = self->position.x + (timer * self->amplitude.x >> 6) - (self->amplitude.x << 15);
        self->drawPos.y = self->position.y + (timer * self->amplitude.y >> 6) - (self->amplitude.y << 15);
    }
    else {
        self->drawPos.x = self->position.x + (self->amplitude.x << 15) - (timer * self->amplitude.x >> 6);
        self->drawPos.y = self->position.y + (self->amplitude.y << 15) - (timer * self->amplitude.y >> 6);
    }
}

#if GAME_INCLUDE_EDITOR
void CircleBumper_EditorDraw(void)
{
    RSDK_THIS(CircleBumper);

    Vector2 amplitude;
    amplitude.x = self->amplitude.x >> 10;
    amplitude.y = self->amplitude.y >> 10;
    switch (self->type) {
        default:
        case CIRCLEBUMPER_FIXED:
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            break;

        case CIRCLEBUMPER_LINEAR:
            self->updateRange.x = (abs(amplitude.x) + 0x1000) << 10;
            self->updateRange.y = (abs(amplitude.y) + 0x1000) << 10;
            break;

        case CIRCLEBUMPER_CIRCLE:
            self->updateRange.x = (abs(amplitude.x) + 0x1000) << 10;
            self->updateRange.y = (abs(amplitude.y) + 0x1000) << 10;
            break;

        case CIRCLEBUMPER_TRACK:
            self->updateRange.x = (abs(amplitude.x) + 0x2000) << 9;
            self->updateRange.y = (abs(amplitude.y) + 0x2000) << 9;
            break;

        case CIRCLEBUMPER_PATH:
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            break;
    }

    RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &self->animator, true, 0);
    self->drawPos = self->position;

    CircleBumper_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        Vector2 pos;
        switch (self->type) {
            case CIRCLEBUMPER_LINEAR:
                self->updateRange.x = (abs(amplitude.x) + 0x1000) << 10;
                self->updateRange.y = (abs(amplitude.y) + 0x1000) << 10;

                // draw distance previews
                self->inkEffect = INK_BLEND;
                self->drawPos.x = amplitude.x * RSDK.Sin1024(0x100) + self->position.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(0x100) + self->position.y;
                pos             = self->drawPos;
                CircleBumper_Draw();

                self->drawPos.x = amplitude.x * RSDK.Sin1024(0x300) + self->position.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(0x300) + self->position.y;
                CircleBumper_Draw();

                RSDK.DrawLine(pos.x, pos.y, self->drawPos.x, self->drawPos.y, 0x00FF00, 0, INK_NONE, false);
                break;

            case CIRCLEBUMPER_CIRCLE:
                self->updateRange.x = (abs(amplitude.x) + 0x1000) << 10;
                self->updateRange.y = (abs(amplitude.y) + 0x1000) << 10;

                // draw distance preview
                self->inkEffect = INK_BLEND;
                self->drawPos.x = amplitude.x * RSDK.Cos1024(4 * self->angle) + self->position.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(4 * self->angle) + self->position.y;

                CircleBumper_Draw();
                break;

            case CIRCLEBUMPER_TRACK:
                self->updateRange.x = (abs(amplitude.x) + 0x2000) << 9;
                self->updateRange.y = (abs(amplitude.y) + 0x2000) << 9;

                // draw distance preview
                self->inkEffect = INK_BLEND;

                // draw distance previews
                self->inkEffect = INK_BLEND;
                self->drawPos.x = self->position.x + (0x100 * amplitude.x >> 6) - (amplitude.x << 15);
                self->drawPos.y = self->position.y + (0x100 * amplitude.y >> 6) - (amplitude.y << 15);
                pos             = self->drawPos;
                CircleBumper_Draw();

                self->drawPos.x = self->position.x + (amplitude.x << 15) - (0x000 * amplitude.x >> 6);
                self->drawPos.y = self->position.y + (amplitude.y << 15) - (0x000 * amplitude.y >> 6);
                CircleBumper_Draw();

                RSDK.DrawLine(pos.x, pos.y, self->drawPos.x, self->drawPos.y, 0x00FF00, 0x00, INK_NONE, false);
                break;

            case CIRCLEBUMPER_PATH: {
                Entity *target = RSDK_GET_ENTITY_GEN(self->speed);

                if (target)
                    DrawHelpers_DrawArrow(self->position.x, self->position.y, target->position.x, target->position.y, 0x00FF00, INK_NONE, 0xFF);
                break;
            }

            default:
            case CIRCLEBUMPER_FIXED:
                // nothin'
                break;
        }

        self->inkEffect = INK_NONE;
        RSDK_DRAWING_OVERLAY(false);
    }
}

void CircleBumper_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SPZ2"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("Blueprint"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/CircleBumper.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(CircleBumper, type);
    RSDK_ENUM_VAR("Fixed", CIRCLEBUMPER_FIXED);
    RSDK_ENUM_VAR("Linear", CIRCLEBUMPER_LINEAR);
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
