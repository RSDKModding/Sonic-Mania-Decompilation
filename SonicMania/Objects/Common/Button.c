// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Button Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectButton *Button;

void Button_Update(void)
{
    RSDK_THIS(Button);

    self->down            = false;
    self->currentlyActive = false;

    switch (self->type) {
        case BUTTON_FLOOR:
            Button_HandleFloor();

            if (Button->hasEggman)
                Button_CheckEggmanCollisions();
            if (Button->hasPhantomRider)
                Button_CheckPRiderCollisions();
            break;

        case BUTTON_ROOF: Button_HandleRoof(); break;
        case BUTTON_RWALL: Button_HandleRWall(); break;
        case BUTTON_LWALL: Button_HandleLWall(); break;

        default: break;
    }

    if (!self->down)
        self->wasActivated = false;

    self->baseAnimator.frameID = self->down + 1;
}

void Button_LateUpdate(void) {}

void Button_StaticUpdate(void) {}

void Button_Draw(void)
{
    RSDK_THIS(Button);

    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    if (self->type >= BUTTON_RWALL)
        drawPos.x -= self->pressPos;
    else
        drawPos.y += self->pressPos;

    RSDK.DrawSprite(&self->buttonAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);
}

void Button_Create(void *data)
{
    RSDK_THIS(Button);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        switch (self->type) {
            case BUTTON_FLOOR:
                self->direction = FLIP_NONE;
                RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->buttonAnimator, true, 0);
                RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->baseAnimator, true, 1);

                self->hitboxButton.left   = Button->hitboxV.left;
                self->hitboxButton.top    = Button->hitboxV.top;
                self->hitboxButton.right  = Button->hitboxV.right;
                self->hitboxButton.bottom = Button->hitboxV.bottom;
                break;

            case BUTTON_ROOF:
                self->direction = FLIP_Y;
                RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->buttonAnimator, true, 0);
                RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->baseAnimator, true, 1);

                self->hitboxButton.left   = Button->hitboxV.left;
                self->hitboxButton.top    = Button->hitboxV.top;
                self->hitboxButton.right  = Button->hitboxV.right;
                self->hitboxButton.bottom = Button->hitboxV.bottom;
                break;

            case BUTTON_RWALL:
                self->direction = FLIP_NONE;
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->buttonAnimator, true, 0);
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->baseAnimator, true, 1);

                self->hitboxButton.left   = Button->hitboxH.left;
                self->hitboxButton.top    = Button->hitboxH.top;
                self->hitboxButton.right  = Button->hitboxH.right;
                self->hitboxButton.bottom = Button->hitboxH.bottom;
                break;

            case BUTTON_LWALL:
                self->direction = FLIP_X;
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->buttonAnimator, true, 0);
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->baseAnimator, true, 1);

                self->hitboxButton.left   = Button->hitboxH.left;
                self->hitboxButton.top    = Button->hitboxH.top;
                self->hitboxButton.right  = Button->hitboxH.right;
                self->hitboxButton.bottom = Button->hitboxH.bottom;
                break;

            default: break;
        }

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = TO_FIXED(32);
        self->updateRange.y = TO_FIXED(32);
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
    }
}

void Button_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MMZ")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("MMZ/Button.bin", SCOPE_STAGE);
        Button->activatePos  = TO_FIXED(8);
        Button->buttonOffset = TO_FIXED(5);
        Button->hitboxOffset = 13;
        Button->unused1      = 5;
    }
    else if (RSDK.CheckSceneFolder("FBZ")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("FBZ/Button.bin", SCOPE_STAGE);
        Button->activatePos  = TO_FIXED(8);
        Button->buttonOffset = TO_FIXED(5);
        Button->hitboxOffset = 13;
        Button->unused1      = 5;
    }
    else if (RSDK.CheckSceneFolder("LRZ1")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("LRZ1/Button.bin", SCOPE_STAGE);
        Button->activatePos  = TO_FIXED(4);
        Button->buttonOffset = TO_FIXED(3);
        Button->hitboxOffset = 7;
        Button->unused1      = 3;
    }
    else if (RSDK.CheckSceneFolder("LRZ2")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("LRZ2/Button.bin", SCOPE_STAGE);
        Button->activatePos  = TO_FIXED(4);
        Button->buttonOffset = TO_FIXED(3);
        Button->hitboxOffset = 7;
        Button->unused1      = 3;
    }
    else if (RSDK.CheckSceneFolder("HCZ")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/Button.bin", SCOPE_STAGE);
        Button->activatePos  = TO_FIXED(3);
        Button->buttonOffset = TO_FIXED(3);
        Button->hitboxOffset = 9;
        Button->unused1      = 6;
    }
    else if (RSDK.CheckSceneFolder("TMZ3")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("TMZ1/Button.bin", SCOPE_STAGE);
        Button->activatePos  = TO_FIXED(3);
        Button->buttonOffset = TO_FIXED(3);
        Button->hitboxOffset = 9;
        Button->unused1      = 6;
    }
    else {
        Button->activatePos  = TO_FIXED(8);
        Button->buttonOffset = TO_FIXED(5);
        Button->hitboxOffset = 13;
        Button->unused1      = 5;
    }

    Button->hitboxH.left   = -3;
    Button->hitboxH.top    = -16;
    Button->hitboxH.right  = 8;
    Button->hitboxH.bottom = 16;

    Button->hitboxV.left   = -16;
    Button->hitboxV.top    = -8;
    Button->hitboxV.right  = 16;
    Button->hitboxV.bottom = 3;

    if (RSDK.FindObject("Eggman"))
        Button->hasEggman = true;

    if (RSDK.FindObject("PhantomRider"))
        Button->hasPhantomRider = true;

    Button->sfxButton = RSDK.GetSfx("Stage/Button.wav");
}

void Button_CheckEggmanCollisions(void)
{
    RSDK_THIS(Button);

    foreach_active(Eggman, eggman)
    {
        self->hitboxButton.top = (Button->activatePos >> 16) - (Button->hitboxOffset & 0xFFFF);
        if (RSDK.CheckObjectCollisionPlatform(self, &self->hitboxButton, eggman, &eggman->hitbox, true)) {
            eggman->onGround = true;
            self->pressPos   = Button->activatePos;

            if (!self->wasActivated) {
                RSDK.PlaySfx(Button->sfxButton, false, 0xFF);
                self->currentlyActive = true;
                self->toggled ^= true;
            }

            self->wasActivated = true;
            self->down         = true;
            self->activated    = true;
        }
    }
}

void Button_CheckPRiderCollisions(void)
{
    RSDK_THIS(Button);

    foreach_active(PhantomRider, rider)
    {
        self->hitboxButton.top = (Button->activatePos >> 16) - (Button->hitboxOffset & 0xFFFF);
        if (RSDK.CheckObjectCollisionPlatform(self, &self->hitboxButton, rider, &rider->hitbox, true)) {
            self->pressPos = Button->activatePos;

            if (!self->wasActivated) {
                RSDK.PlaySfx(Button->sfxButton, false, 0xFF);
                self->currentlyActive = true;
                self->toggled ^= true;
            }

            self->wasActivated = true;
            self->down         = true;
            self->activated    = true;
        }
    }
}

void Button_HandleFloor(void)
{
    RSDK_THIS(Button);

    int32 startPressPos = self->pressPos;
    self->pressPos      = 0;

    foreach_active(Player, player)
    {
        self->hitboxButton.top    = (startPressPos >> 16) - (Button->hitboxOffset & 0xFFFF);
        self->hitboxButton.bottom = self->hitboxButton.top + 32;

        int32 playerX            = player->position.x;
        int32 playerY            = player->position.y;
        int32 xVel               = player->velocity.x;
        int32 yVel               = player->velocity.y;
        int32 vel                = player->groundVel;
        bool32 grounded          = player->onGround;
        bool32 groundedStore     = player->groundedStore;
        StateMachine(nextGState) = player->nextGroundState;
        StateMachine(nextAState) = player->nextAirState;
        StateMachine(state)      = player->state;

        if (Player_CheckCollisionBox(player, self, &self->hitboxButton) == C_TOP || self->walkOnto) {
            player->position.x      = playerX;
            player->position.y      = playerY;
            player->velocity.x      = xVel;
            player->velocity.y      = yVel;
            player->groundVel       = vel;
            player->onGround        = grounded;
            player->groundedStore   = groundedStore;
            player->nextGroundState = nextGState;
            player->nextAirState    = nextAState;
            player->state           = state;

            self->hitboxButton.top -= (startPressPos >> 16);
            self->hitboxButton.top += (Button->activatePos >> 16);
            int32 newPressPos = self->pressPos;

            if (Player_CheckCollisionPlatform(player, self, &self->hitboxButton) == C_TOP) {
                self->pressPos = Button->activatePos;
            }
            else {
                self->position.y -= Button->activatePos;
                if (Player_CheckCollisionTouch(player, self, &self->hitboxButton)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->pressPos       = Button->buttonOffset + (player->position.y & 0xFFFF0000) + (playerHitbox->bottom << 16) - self->position.y;
                    self->pressPos       = CLAMP(self->pressPos, 0, Button->activatePos) & 0xFFFF0000;
                }

                self->position.y += Button->activatePos;
            }

            if (self->pressPos == Button->activatePos) {
                Player_CheckCollisionBox(player, self, &self->hitboxButton);
                player->angle = 0;
                if (!self->wasActivated) {
                    RSDK.PlaySfx(Button->sfxButton, false, 255);
                    self->currentlyActive = true;
                    self->toggled ^= true;
                }
                self->wasActivated = true;
                self->down         = true;
                self->activated    = true;
            }

            if (newPressPos > self->pressPos)
                self->pressPos = newPressPos;
        }

        if (self->pressPos)
            startPressPos = self->pressPos;
    }
}
void Button_HandleRoof(void)
{
    RSDK_THIS(Button);

    int32 startPressPos = self->pressPos;
    self->pressPos      = 0;

    foreach_active(Player, player)
    {
        self->hitboxButton.top    = -1 - (Button->hitboxOffset & 0xFFFF) - (startPressPos >> 16);
        self->hitboxButton.bottom = self->hitboxButton.top + 32;

        int32 playerX            = player->position.x;
        int32 playerY            = player->position.y;
        int32 xVel               = player->velocity.x;
        int32 yVel               = player->velocity.y;
        int32 vel                = player->groundVel;
        bool32 grounded          = player->onGround;
        bool32 groundedStore     = player->groundedStore;
        StateMachine(nextGState) = player->nextGroundState;
        StateMachine(nextAState) = player->nextAirState;
        StateMachine(state)      = player->state;

        if (Player_CheckCollisionBox(player, self, &self->hitboxButton) == C_BOTTOM || self->walkOnto) {
            player->position.x      = playerX;
            player->position.y      = playerY;
            player->velocity.x      = xVel;
            player->velocity.y      = yVel;
            player->groundVel       = vel;
            player->onGround        = grounded;
            player->groundedStore   = groundedStore;
            player->nextGroundState = nextGState;
            player->nextAirState    = nextAState;
            player->state           = state;

            self->hitboxButton.top += (startPressPos >> 16);
            self->hitboxButton.top += (Button->activatePos >> 16);
            int32 newPressPos = self->pressPos;

            if (Player_CheckCollisionBox(player, self, &self->hitboxButton) == C_BOTTOM) {
                self->pressPos = -Button->activatePos;
            }
            else {
                self->position.y += Button->activatePos;
                if (Player_CheckCollisionTouch(player, self, &self->hitboxButton)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->pressPos       = (player->position.y & 0xFFFF0000) + (playerHitbox->top << 16) - Button->buttonOffset - self->position.y;
                    self->pressPos       = CLAMP(self->pressPos, -Button->activatePos, 0) & 0xFFFF0000;
                }

                self->position.y -= Button->activatePos;
            }

            if (self->pressPos == -Button->activatePos) {
                if (!self->wasActivated) {
                    RSDK.PlaySfx(Button->sfxButton, false, 255);
                    self->currentlyActive = true;
                    self->toggled ^= true;
                }

                self->wasActivated = true;
                self->down         = true;
                self->activated    = true;
            }

            if (newPressPos < self->pressPos)
                self->pressPos = newPressPos;
        }

        if (self->pressPos)
            startPressPos = self->pressPos;
    }
}
void Button_HandleRWall(void)
{
    RSDK_THIS(Button);

    int32 startPressPos = self->pressPos;
    self->pressPos      = 0;

    foreach_active(Player, player)
    {
        self->hitboxButton.right = (Button->hitboxOffset & 0xFFFF) - (startPressPos >> 16) + 1;
        self->hitboxButton.left  = self->hitboxButton.right - 16;

        int32 playerX            = player->position.x;
        int32 playerY            = player->position.y;
        int32 xVel               = player->velocity.x;
        int32 yVel               = player->velocity.y;
        int32 vel                = player->groundVel;
        bool32 grounded          = player->onGround;
        bool32 groundedStore     = player->groundedStore;
        StateMachine(nextGState) = player->nextGroundState;
        StateMachine(nextAState) = player->nextAirState;
        StateMachine(state)      = player->state;

        if (Player_CheckCollisionBox(player, self, &self->hitboxButton) == C_RIGHT || self->walkOnto) {
            player->position.x      = playerX;
            player->position.y      = playerY;
            player->velocity.x      = xVel;
            player->velocity.y      = yVel;
            player->groundVel       = vel;
            player->onGround        = grounded;
            player->groundedStore   = groundedStore;
            player->nextGroundState = nextGState;
            player->nextAirState    = nextAState;
            player->state           = state;

            self->hitboxButton.right += (startPressPos >> 16);
            self->hitboxButton.right = self->hitboxButton.right - (Button->activatePos >> 16) - 1;
            int32 newPressPos        = self->pressPos;

            if (Player_CheckCollisionBox(player, self, &self->hitboxButton) == C_RIGHT) {
                self->pressPos = Button->activatePos;
            }
            else {
                self->position.x += Button->activatePos;
                if (Player_CheckCollisionTouch(player, self, &self->hitboxButton)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->pressPos       = Button->buttonOffset - (playerHitbox->left << 16) - (player->position.x & 0xFFFF0000) + self->position.x;
                    self->pressPos       = CLAMP(self->pressPos, 0, Button->activatePos) & 0xFFFF0000;
                }
                self->position.x -= Button->activatePos;
            }

            if (self->pressPos == Button->activatePos) {
                if (!self->wasActivated) {
                    RSDK.PlaySfx(Button->sfxButton, false, 255);
                    self->currentlyActive = true;
                    self->toggled ^= true;
                }

                self->wasActivated = true;
                self->down         = true;
                self->activated    = true;
            }

            if (newPressPos > self->pressPos)
                self->pressPos = newPressPos;
        }

        if (self->pressPos)
            startPressPos = self->pressPos;
    }
}
void Button_HandleLWall(void)
{
    RSDK_THIS(Button);
    int32 startPressPos = self->pressPos;
    self->pressPos      = 0;

    foreach_active(Player, player)
    {
        self->hitboxButton.right = (startPressPos >> 16) + (Button->hitboxOffset & 0xFFFF);
        self->hitboxButton.left  = self->hitboxButton.right - 16;

        int32 playerX            = player->position.x;
        int32 playerY            = player->position.y;
        int32 xVel               = player->velocity.x;
        int32 yVel               = player->velocity.y;
        int32 vel                = player->groundVel;
        bool32 grounded          = player->onGround;
        bool32 groundedStore     = player->groundedStore;
        StateMachine(nextGState) = player->nextGroundState;
        StateMachine(nextAState) = player->nextAirState;
        StateMachine(state)      = player->state;

        if (Player_CheckCollisionBox(player, self, &self->hitboxButton) == C_LEFT || self->walkOnto) {
            player->position.x      = playerX;
            player->position.y      = playerY;
            player->velocity.x      = xVel;
            player->velocity.y      = yVel;
            player->groundVel       = vel;
            player->onGround        = grounded;
            player->groundedStore   = groundedStore;
            player->nextGroundState = nextGState;
            player->nextAirState    = nextAState;
            player->state           = state;

            self->hitboxButton.right -= (startPressPos >> 16);
            self->hitboxButton.right -= (Button->activatePos >> 16);
            int32 newPressPos = self->pressPos;

            if (Player_CheckCollisionBox(player, self, &self->hitboxButton) == C_LEFT) {
                self->pressPos = -Button->activatePos;
            }
            else {
                self->position.x -= Button->activatePos;
                if (Player_CheckCollisionTouch(player, self, &self->hitboxButton)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->pressPos       = self->position.x - (playerHitbox->right << 16) - (player->position.x & 0xFFFF0000) - Button->buttonOffset;
                    self->pressPos       = CLAMP(self->pressPos, -Button->activatePos, 0) & 0xFFFF0000;
                }

                self->position.x += Button->activatePos;
            }

            if (self->pressPos == -Button->activatePos) {
                if (!self->wasActivated) {
                    RSDK.PlaySfx(Button->sfxButton, false, 255);
                    self->currentlyActive = true;
                    self->toggled ^= true;
                }
                self->wasActivated = true;
                self->down         = true;
                self->activated    = true;
            }

            if (newPressPos < self->pressPos)
                self->pressPos = newPressPos;
        }

        if (self->pressPos)
            startPressPos = self->pressPos;
    }

    self->hitboxButton.right = (Button->hitboxOffset & 0xFFFF) + (startPressPos >> 16);
    self->hitboxButton.left  = self->hitboxButton.left - 16;
}

#if GAME_INCLUDE_EDITOR
void Button_EditorDraw(void)
{
    RSDK_THIS(Button);

    switch (self->type) {
        case BUTTON_FLOOR:
            self->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->buttonAnimator, true, 0);
            RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->baseAnimator, true, 1);
            break;

        case BUTTON_ROOF:
            self->direction = FLIP_Y;
            RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->buttonAnimator, true, 0);
            RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->baseAnimator, true, 1);
            break;

        case BUTTON_RWALL:
            self->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->buttonAnimator, true, 0);
            RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->baseAnimator, true, 1);
            break;

        case BUTTON_LWALL:
            self->direction = FLIP_X;
            RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->buttonAnimator, true, 0);
            RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->baseAnimator, true, 1);
            break;

        default: break;
    }

    RSDK.DrawSprite(&self->buttonAnimator, NULL, false);
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);
}

void Button_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("MMZ"))
        Button->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Button.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("FBZ"))
        Button->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Button.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ1"))
        Button->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Button.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2"))
        Button->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Button.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("HCZ"))
        Button->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Button.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("TMZ3"))
        Button->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Button.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Button, type);
    RSDK_ENUM_VAR("Floor", BUTTON_FLOOR);
    RSDK_ENUM_VAR("Ceiling", BUTTON_ROOF);
    RSDK_ENUM_VAR("Right Wall", BUTTON_RWALL);
    RSDK_ENUM_VAR("Left Wall", BUTTON_LWALL);
}
#endif

void Button_Serialize(void)
{
    RSDK_EDITABLE_VAR(Button, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Button, VAR_BOOL, walkOnto);
    RSDK_EDITABLE_VAR(Button, VAR_UINT8, tag);
}
