// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Crane Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCrane *Crane;

void Crane_Update(void)
{
    RSDK_THIS(Crane);

    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->frontAnimator);
    RSDK.ProcessAnimation(&self->backAnimator);
}

void Crane_LateUpdate(void) {}

void Crane_StaticUpdate(void)
{
    foreach_active(Crane, crane) { RSDK.AddDrawListRef(Zone->playerDrawGroup[0] + 1, RSDK.GetEntitySlot(crane)); }
}

void Crane_Draw(void)
{
    RSDK_THIS(Crane);

    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        SpriteFrame *frame = RSDK.GetFrame(Crane->aniFrames, 4, 0);
        int32 height       = (self->position.y - self->startPos.y) >> 16;
        frame->sprY        = 230 - height;
        frame->height      = height;
        frame->pivotY      = -16 - height;
        RSDK.DrawSprite(&self->chainAnimator, NULL, false);
        RSDK.DrawSprite(&self->backAnimator, NULL, false);
    }
    else {
        RSDK.DrawSprite(&self->frontAnimator, NULL, false);
    }
}

void Crane_Create(void *data)
{
    RSDK_THIS(Crane);

    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawFX        = FX_FLIP;

    // Frame 3???? that doesn't exist
    // Note: it *does* for anim 0 & 1 though, likely where the cause stems from
    RSDK.SetSpriteAnimation(Crane->aniFrames, 2, &self->frontAnimator, true, 3);
    RSDK.SetSpriteAnimation(Crane->aniFrames, 3, &self->backAnimator, true, 3);
    RSDK.SetSpriteAnimation(Crane->aniFrames, 4, &self->chainAnimator, true, 0);
    self->state = Crane_State_Init;
}

void Crane_StageLoad(void)
{
    Crane->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Crane.bin", SCOPE_STAGE);

    Crane->hitboxUnused.left   = -8;
    Crane->hitboxUnused.top    = -8;
    Crane->hitboxUnused.right  = 8;
    Crane->hitboxUnused.bottom = 8;

    Crane->hitboxRange.left   = -8;
    Crane->hitboxRange.top    = 0;
    Crane->hitboxRange.right  = 8;
    Crane->hitboxRange.bottom = 128;

    Crane->hitboxGrab.left   = -8;
    Crane->hitboxGrab.top    = 0;
    Crane->hitboxGrab.right  = 8;
    Crane->hitboxGrab.bottom = 16;

    Crane->active = ACTIVE_ALWAYS;

    Crane->sfxGrab = RSDK.GetSfx("Global/Grab.wav");

    DEBUGMODE_ADD_OBJ(Crane);
}

void Crane_DebugSpawn(void)
{
    RSDK_THIS(Crane);

    EntityCrane *crane = CREATE_ENTITY(Crane, NULL, self->position.x, self->position.y);
    crane->direction   = self->direction;
    crane->startDir    = self->direction;
}

void Crane_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Crane->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Crane_HandlePlayerInteractions(void)
{
    RSDK_THIS(Crane);

    foreach_active(Player, player)
    {
        if (!self->grabbedPlayer) {
            if ((self->playerType != CRANE_PLAYER1_ONLY || !player->sidekick) && (self->playerType != CRANE_PLAYER2_ONLY || player->sidekick)) {
                if (Player_CheckCollisionTouch(player, self, &Crane->hitboxGrab)) {
                    RSDK.PlaySfx(Crane->sfxGrab, false, 255);
                    self->state       = Crane_State_RiseUp;
                    self->isPermanent = true;
                    RSDK.SetSpriteAnimation(Crane->aniFrames, 0, &self->frontAnimator, true, 0);
                    RSDK.SetSpriteAnimation(Crane->aniFrames, 1, &self->backAnimator, true, 0);
                    self->grabbedPlayer     = player;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->groundVel       = 0;
                    player->state           = Player_State_Static;
                    player->nextGroundState = StateMachine_None;
                    player->nextAirState    = StateMachine_None;
                    player->onGround        = false;
                    player->direction       = self->direction;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, true, 0);
                    player->animator.speed = 0;
                }
            }
        }
    }
}

void Crane_CheckOffScreen(void)
{
    RSDK_THIS(Crane);

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        int32 x          = self->position.x;
        int32 y          = self->position.y;
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        if (RSDK.CheckOnScreen(self, &self->updateRange)) {
            self->position.x = x;
            self->position.y = y;
        }
        else {
            self->grabbedPlayer = NULL;
            self->delay         = 0;
            self->timer         = 0;
            self->direction     = self->startDir;
            self->visible       = false;
            self->state         = Crane_State_CheckOffScreen;
        }
    }
}

void Crane_State_CheckOffScreen(void)
{
    RSDK_THIS(Crane);

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->state = Crane_State_Init;
        Crane_Create(NULL);
    }
}

void Crane_State_Init(void)
{
    RSDK_THIS(Crane);

    self->active = ACTIVE_NORMAL;
    if (!self->startType) {
        self->state = Crane_State_CheckForPlayers;
        Crane_State_CheckForPlayers();
    }
    else {
        self->position.y += 0x400000;
        self->timer = -1;
        self->state = Crane_State_CheckGrab;

        Crane_HandlePlayerInteractions();
        Crane_CheckOffScreen();
    }
}

void Crane_State_CheckForPlayers(void)
{
    RSDK_THIS(Crane);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Crane->hitboxRange)) {
            self->timer = 32;
            self->state = Crane_State_LowerToGrab;
        }
    }

    Crane_CheckOffScreen();
}

void Crane_State_LowerToGrab(void)
{
    RSDK_THIS(Crane);

    Crane_HandlePlayerInteractions();
    Crane_CheckOffScreen();

    --self->timer;
    if (self->state == Crane_State_LowerToGrab) {
        if (self->timer >= 0)
            self->position.y += 0x20000;
        else
            self->state = Crane_State_CheckGrab;
    }
}

void Crane_State_CheckGrab(void)
{
    Crane_HandlePlayerInteractions();
    Crane_CheckOffScreen();
}

void Crane_State_RiseUp(void)
{
    RSDK_THIS(Crane);
    if (++self->timer < 32) {
        self->position.y -= 0x20000;

        EntityPlayer *player = self->grabbedPlayer;
        if (player) {
            if (Player_CheckValidState(player)) {
                player->animator.speed = 0;
                player->position.x     = self->position.x;
                player->position.y     = self->position.y;
                if (self->direction) {
                    player->position.x += 0x20000;
                    player->position.y += 0xE0000;
                }
                else {
                    player->position.x -= 0x20000;
                    player->position.y += 0xE0000;
                }
            }
            else {
                self->grabbedPlayer = NULL;
            }
        }
    }
    else {
        self->delay      = 1;
        self->timer      = 16;
        self->velocity.x = 0;
        self->state      = Crane_State_ToDest1stHalf;
    }

    Crane_CheckOffScreen();
}

void Crane_State_ToDest1stHalf(void)
{
    RSDK_THIS(Crane);

    self->velocity.x += 0x1000;
    if (self->direction == FLIP_NONE) {
        self->position.x += self->velocity.x;
        if (self->position.x >= self->startPos.x + (self->distance << 15))
            self->state = Crane_State_ToDest2ndHalf;
    }
    else {
        self->position.x -= self->velocity.x;
        if (self->position.x <= self->startPos.x - (self->distance << 15))
            self->state = Crane_State_ToDest2ndHalf;
    }

    EntityPlayer *player = self->grabbedPlayer;
    if (player) {
        if (Player_CheckValidState(player)) {
            player->animator.speed = 0;
            player->position.x     = self->position.x;
            player->position.y     = self->position.y;
            if (self->direction) {
                player->position.x += 0x20000;
                player->position.y += 0xE0000;
            }
            else {
                player->position.x -= 0x20000;
                player->position.y += 0xE0000;
            }
        }
        else {
            self->grabbedPlayer = NULL;
        }
    }

    Crane_CheckOffScreen();
}

void Crane_State_ToDest2ndHalf(void)
{
    RSDK_THIS(Crane);

    EntityPlayer *player = self->grabbedPlayer;

    self->velocity.x -= 0x1000;
    if (self->direction == FLIP_NONE)
        self->position.x += self->velocity.x;
    else
        self->position.x -= self->velocity.x;

    if (self->velocity.x <= 0) {
        if (player) {
            if (Player_CheckValidState(player))
                player->state = Player_State_Air;
            else
                self->grabbedPlayer = NULL;
        }

        RSDK.SetSpriteAnimation(Crane->aniFrames, 2, &self->frontAnimator, true, 0);
        RSDK.SetSpriteAnimation(Crane->aniFrames, 3, &self->backAnimator, true, 0);
        self->delay       = 30;
        self->state       = Crane_State_DropDelay;
        self->isPermanent = false;
    }

    if (player) {
        if (Player_CheckValidState(player)) {
            player->animator.speed = 0;
            player->position.x     = self->position.x;
            player->position.y     = self->position.y;
            if (self->direction) {
                player->position.x += 0x20000;
                player->position.y += 0xE0000;
            }
            else {
                player->position.x -= 0x20000;
                player->position.y += 0xE0000;
            }
        }
        else {
            self->grabbedPlayer = NULL;
        }
    }

    Crane_CheckOffScreen();
}

void Crane_State_DropDelay(void)
{
    RSDK_THIS(Crane);

    if (--self->delay <= 0)
        self->state = Crane_State_ToStart1stHalf;

    Crane_HandlePlayerInteractions();
    Crane_CheckOffScreen();
}

void Crane_State_ToStart1stHalf(void)
{
    RSDK_THIS(Crane);

    self->velocity.x -= 0x1000;
    if (self->direction) {
        self->position.x -= self->velocity.x;
        if (self->position.x >= self->startPos.x - (self->distance << 15))
            self->state = Crane_State_ToStart2ndHalf;
    }
    else {
        self->position.x += self->velocity.x;
        if (self->position.x <= self->startPos.x + (self->distance << 15))
            self->state = Crane_State_ToStart2ndHalf;
    }

    Crane_CheckOffScreen();
}

void Crane_State_ToStart2ndHalf(void)
{
    RSDK_THIS(Crane);
    self->velocity.x += 0x1000;

    if (self->direction)
        self->position.x -= self->velocity.x;
    else
        self->position.x += self->velocity.x;

    if (self->velocity.x >= 0) {
        if (self->startType) {
            self->timer         = 32;
            self->state         = Crane_State_LowerToGrab;
            self->grabbedPlayer = NULL;
        }
        else {
            self->state         = Crane_State_CheckForPlayers;
            self->grabbedPlayer = NULL;
        }
    }

    Crane_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void Crane_EditorDraw(void)
{
    RSDK_THIS(Crane);

    RSDK.SetSpriteAnimation(Crane->aniFrames, 2, &self->frontAnimator, true, 2);
    RSDK.SetSpriteAnimation(Crane->aniFrames, 3, &self->backAnimator, true, 2);
    RSDK.SetSpriteAnimation(Crane->aniFrames, 4, &self->chainAnimator, true, 0);

    Vector2 drawPos = self->position;
    if (self->startType)
        drawPos.y += 0x400000;

    RSDK.DrawSprite(&self->chainAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->backAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->frontAnimator, &drawPos, false);

    if (showGizmos()) {
        self->inkEffect = INK_BLEND;

        uint8 mode       = 0;
        self->velocity.x = 0;
        while (true) {
            switch (mode) {
                default: break;
                case 0:
                    self->velocity.x += 0x1000;
                    if (self->direction == FLIP_NONE) {
                        drawPos.x += self->velocity.x;
                        if (drawPos.x >= self->position.x + (self->distance << 15))
                            mode = 1;
                    }
                    else {
                        drawPos.x -= self->velocity.x;
                        if (drawPos.x <= self->position.x - (self->distance << 15))
                            mode = 1;
                    }
                    break;

                case 1:
                    self->velocity.x -= 0x1000;
                    if (self->direction == FLIP_NONE)
                        drawPos.x += self->velocity.x;
                    else
                        drawPos.x -= self->velocity.x;

                    if (self->velocity.x <= 0)
                        mode = 2;
                    break;
            }

            if (mode > 1)
                break;
        }

        RSDK.DrawSprite(&self->chainAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->backAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->frontAnimator, &drawPos, false);

        DrawHelpers_DrawArrow(self->position.x, self->position.y, drawPos.x, drawPos.y, 0x00FF00, INK_NONE, 0xFF);

        self->inkEffect = INK_NONE;
    }
}

void Crane_EditorLoad(void)
{
    Crane->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Crane.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Crane, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);

    RSDK_ACTIVE_VAR(Crane, startType);
    RSDK_ENUM_VAR("Up", CRANE_START_UP);
    RSDK_ENUM_VAR("Down", CRANE_START_DOWN);

    RSDK_ACTIVE_VAR(Crane, playerType);
    RSDK_ENUM_VAR("Player Only", CRANE_PLAYER1_ONLY);
    RSDK_ENUM_VAR("Sidekick Only", CRANE_PLAYER2_ONLY);
    RSDK_ENUM_VAR("Any Player", CRANE_PLAYER_ALL);
}
#endif

void Crane_Serialize(void)
{
    RSDK_EDITABLE_VAR(Crane, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Crane, VAR_UINT16, distance);
    RSDK_EDITABLE_VAR(Crane, VAR_UINT8, startType);
    RSDK_EDITABLE_VAR(Crane, VAR_UINT8, playerType);
}
