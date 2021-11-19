#include "SonicMania.h"

ObjectButton *Button;

void Button_Update(void)
{
    RSDK_THIS(Button);

    self->down            = 0;
    self->currentlyActive = false;
    switch (self->type) {
        case BUTTON_FLOOR:
            Button_TypeFloor();
            if (Button->hasEggman)
                Button_CheckEggmanCollisions();
            if (Button->hasPhantomRider)
                Button_CheckPRiderCollisions();
            break;
        case BUTTON_ROOF: Button_TypeRoof(); break;
        case BUTTON_RWALL: Button_TypeRWall(); break;
        case BUTTON_LWALL: Button_TypeLWall(); break;
        default: break;
    }

    if (!self->down)
        self->wasActivated = 0;
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
                self->hitbox.left   = Button->hitboxV.left;
                self->hitbox.top    = Button->hitboxV.top;
                self->hitbox.right  = Button->hitboxV.right;
                self->hitbox.bottom = Button->hitboxV.bottom;
                break;
            case BUTTON_ROOF:
                self->direction = FLIP_Y;
                RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->buttonAnimator, true, 0);
                RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->baseAnimator, true, 1);
                self->hitbox.left   = Button->hitboxV.left;
                self->hitbox.top    = Button->hitboxV.top;
                self->hitbox.right  = Button->hitboxV.right;
                self->hitbox.bottom = Button->hitboxV.bottom;
                break;
            case BUTTON_RWALL:
                self->direction = FLIP_NONE;
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->buttonAnimator, true, 0);
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->baseAnimator, true, 1);
                self->hitbox.left   = Button->hitboxH.left;
                self->hitbox.top    = Button->hitboxH.top;
                self->hitbox.right  = Button->hitboxH.right;
                self->hitbox.bottom = Button->hitboxH.bottom;
                break;
            case BUTTON_LWALL:
                self->direction = FLIP_X;
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->buttonAnimator, true, 0);
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->baseAnimator, true, 1);
                self->hitbox.left   = Button->hitboxH.left;
                self->hitbox.top    = Button->hitboxH.top;
                self->hitbox.right  = Button->hitboxH.right;
                self->hitbox.bottom = Button->hitboxH.bottom;
                break;
            default: break;
        }
        self->currentlyActive = ACTIVE_BOUNDS;
        self->updateRange.x   = 0x200000;
        self->updateRange.y   = 0x200000;
        self->visible         = true;
        self->drawOrder       = Zone->drawOrderLow;
    }
}

void Button_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("MMZ/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x80000;
        Button->buttonOffset = 0x50000;
        Button->hitboxOffset = 13;
        Button->unused1      = 5;
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("FBZ/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x80000;
        Button->buttonOffset = 0x50000;
        Button->hitboxOffset = 13;
        Button->unused1      = 5;
    }
    else if (RSDK.CheckStageFolder("LRZ1")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("LRZ1/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x40000;
        Button->buttonOffset = 0x30000;
        Button->hitboxOffset = 7;
        Button->unused1      = 3;
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("LRZ2/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x40000;
        Button->buttonOffset = 0x30000;
        Button->hitboxOffset = 7;
        Button->unused1      = 3;
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x30000;
        Button->buttonOffset = 0x30000;
        Button->hitboxOffset = 9;
        Button->unused1      = 6;
    }
    else if (RSDK.CheckStageFolder("TMZ3")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("TMZ1/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x30000;
        Button->buttonOffset = 0x30000;
        Button->hitboxOffset = 9;
        Button->unused1      = 6;
    }
    else {
        Button->activatePos  = 0x80000;
        Button->buttonOffset = 0x50000;
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
    if (RSDK.GetObjectIDByName("Eggman"))
        Button->hasEggman = true;
    if (RSDK.GetObjectIDByName("PhantomRider"))
        Button->hasPhantomRider = true;
    Button->sfxButton = RSDK.GetSFX("Stage/Button.wav");
}

void Button_CheckEggmanCollisions(void)
{
    RSDK_THIS(Button);
    foreach_active(Eggman, eggman)
    {
        self->hitbox.top = (Button->activatePos >> 16) - (Button->hitboxOffset & 0xFFFF);
        if (RSDK.CheckObjectCollisionPlatform(self, &self->hitbox, eggman, &eggman->hitbox, true)) {
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
        self->hitbox.top = (Button->activatePos >> 16) - (Button->hitboxOffset & 0xFFFF);
        if (RSDK.CheckObjectCollisionPlatform(self, &self->hitbox, rider, &rider->hitbox, true)) {
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

void Button_TypeFloor(void)
{
    RSDK_THIS(Button);
    int32 val      = self->pressPos;
    self->pressPos = 0;
    foreach_active(Player, player)
    {
        self->hitbox.top     = (val >> 16) - (Button->hitboxOffset & 0xFFFF);
        self->hitbox.bottom  = self->hitbox.top + 32;
        int32 playerX        = player->position.x;
        int32 playerY        = player->position.y;
        int32 xVel           = player->velocity.x;
        int32 yVel           = player->velocity.y;
        int32 vel            = player->groundVel;
        bool32 grounded      = player->onGround;
        bool32 groundedStore = player->groundedStore;
        void *nextGState     = player->nextGroundState;
        void *nextAState     = player->nextAirState;
        void *state          = player->state;
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == 1 || self->walkOnto) {
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
            self->hitbox.top -= (val >> 16);
            self->hitbox.top += (Button->activatePos >> 16);
            int32 val2 = self->pressPos;
            if (Player_CheckCollisionPlatform(player, self, &self->hitbox) == 1) {
                self->pressPos = Button->activatePos;
            }
            else {
                self->position.y -= Button->activatePos;
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->pressPos       = Button->buttonOffset + (player->position.y & 0xFFFF0000) + (playerHitbox->bottom << 16) - self->position.y;
                    if (self->pressPos <= Button->activatePos) {
                        if (self->pressPos < 0)
                            self->pressPos = 0;
                    }
                    else {
                        self->pressPos = Button->activatePos;
                    }
                    self->pressPos &= 0xFFFF0000;
                }
                self->position.y += Button->activatePos;
            }
            if (self->pressPos == Button->activatePos) {
                Player_CheckCollisionBox(player, self, &self->hitbox);
                player->angle = 0;
                if (!self->wasActivated) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    self->currentlyActive = true;
                    self->toggled ^= true;
                }
                self->wasActivated = true;
                self->down         = true;
                self->activated    = true;
            }
            if (val2 > self->pressPos)
                self->pressPos = val2;
        }
        if (self->pressPos)
            val = self->pressPos;
    }
}
void Button_TypeRoof(void)
{
    RSDK_THIS(Button);
    int32 val      = self->pressPos;
    self->pressPos = 0;

    foreach_active(Player, player)
    {
        self->hitbox.top     = -1 - (Button->hitboxOffset & 0xFFFF) - (val >> 16);
        self->hitbox.bottom  = self->hitbox.top + 32;
        int32 playerX        = player->position.x;
        int32 playerY        = player->position.y;
        int32 xVel           = player->velocity.x;
        int32 yVel           = player->velocity.y;
        int32 vel            = player->groundVel;
        bool32 grounded      = player->onGround;
        bool32 groundedStore = player->groundedStore;
        void *nextGState     = player->nextGroundState;
        void *nextAState     = player->nextAirState;
        void *state          = player->state;
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == 4 || self->walkOnto) {
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
            self->hitbox.top += (val >> 16);
            self->hitbox.top += (Button->activatePos >> 16);
            int32 val2 = self->pressPos;
            if (Player_CheckCollisionBox(player, self, &self->hitbox) == 4) {
                self->pressPos = -Button->activatePos;
            }
            else {
                self->position.y += Button->activatePos;
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->pressPos       = (player->position.y & 0xFFFF0000) + (playerHitbox->top << 16) - Button->buttonOffset - self->position.y;
                    if (self->pressPos >= -Button->activatePos) {
                        if (self->pressPos > 0)
                            self->pressPos = 0;
                    }
                    else {
                        self->pressPos = -Button->activatePos;
                    }
                    self->pressPos &= 0xFFFF0000;
                }
                self->position.y -= Button->activatePos;
            }

            if (self->pressPos == -Button->activatePos) {
                if (!self->wasActivated) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    self->currentlyActive = true;
                    self->toggled ^= true;
                }
                self->wasActivated = true;
                self->down         = true;
                self->activated    = true;
            }
            if (val2 < self->pressPos)
                self->pressPos = val2;
        }
        if (self->pressPos)
            val = self->pressPos;
    }
}
void Button_TypeRWall(void)
{
    RSDK_THIS(Button);
    int32 val      = self->pressPos;
    self->pressPos = 0;
    foreach_active(Player, player)
    {
        self->hitbox.right   = (Button->hitboxOffset & 0xFFFF) - (val >> 16) + 1;
        self->hitbox.left    = self->hitbox.right - 16;
        int32 playerX        = player->position.x;
        int32 playerY        = player->position.y;
        int32 xVel           = player->velocity.x;
        int32 yVel           = player->velocity.y;
        int32 vel            = player->groundVel;
        bool32 grounded      = player->onGround;
        bool32 groundedStore = player->groundedStore;
        void *nextGState     = player->nextGroundState;
        void *nextAState     = player->nextAirState;
        void *state          = player->state;

        if (Player_CheckCollisionBox(player, self, &self->hitbox) == 3 || self->walkOnto) {
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

            self->hitbox.right += (val >> 16);
            self->hitbox.right = self->hitbox.right - (Button->activatePos >> 16) - 1;
            int32 val2         = self->pressPos;
            if (Player_CheckCollisionBox(player, self, &self->hitbox) == 3) {
                self->pressPos = Button->activatePos;
            }
            else {
                self->position.x += Button->activatePos;
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->pressPos       = Button->buttonOffset - (playerHitbox->left << 16) - (player->position.x & 0xFFFF0000) + self->position.x;
                    if (self->pressPos <= Button->activatePos) {
                        if (self->pressPos < 0)
                            self->pressPos = 0;
                    }
                    else {
                        self->pressPos = Button->activatePos;
                    }
                    self->pressPos &= 0xFFFF0000;
                }
                self->position.x -= Button->activatePos;
            }

            if (self->pressPos == Button->activatePos) {
                if (!self->wasActivated) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    self->currentlyActive = true;
                    self->toggled ^= true;
                }
                self->wasActivated = true;
                self->down         = true;
                self->activated    = true;
            }
            if (val2 > self->pressPos)
                self->pressPos = val2;
        }

        if (self->pressPos)
            val = self->pressPos;
    }
}
void Button_TypeLWall(void)
{
    RSDK_THIS(Button);
    int32 val      = self->pressPos;
    self->pressPos = 0;

    foreach_active(Player, player)
    {
        self->hitbox.right = (val >> 16) + (Button->hitboxOffset & 0xFFFF);
        self->hitbox.left  = self->hitbox.right - 16;

        int32 playerX        = player->position.x;
        int32 playerY        = player->position.y;
        int32 xVel           = player->velocity.x;
        int32 yVel           = player->velocity.y;
        int32 vel            = player->groundVel;
        bool32 grounded      = player->onGround;
        bool32 groundedStore = player->groundedStore;
        void *nextGState     = player->nextGroundState;
        void *nextAState     = player->nextAirState;
        void *state          = player->state;
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == 2 || self->walkOnto) {
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
            self->hitbox.right -= (val >> 16);
            self->hitbox.right -= (Button->activatePos >> 16);
            int32 val2 = self->pressPos;
            if (Player_CheckCollisionBox(player, self, &self->hitbox) == 2) {
                self->pressPos = -Button->activatePos;
            }
            else {
                self->position.x -= Button->activatePos;
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->pressPos       = self->position.x - (playerHitbox->right << 16) - (player->position.x & 0xFFFF0000) - Button->buttonOffset;
                    if (self->pressPos >= -Button->activatePos) {
                        if (self->pressPos > 0)
                            self->pressPos = 0;
                    }
                    else {
                        self->pressPos = -Button->activatePos;
                    }
                    self->pressPos &= 0xFFFF0000;
                }
                self->position.x += Button->activatePos;
            }
            if (self->pressPos == -Button->activatePos) {
                if (!self->wasActivated) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    self->currentlyActive = true;
                    self->toggled ^= true;
                }
                self->wasActivated = true;
                self->down         = true;
                self->activated    = true;
            }
            if (val2 < self->pressPos)
                self->pressPos = val2;
        }

        if (self->pressPos)
            val = self->pressPos;
    }

    self->hitbox.right = (Button->hitboxOffset & 0xFFFF) + (val >> 16);
    self->hitbox.left  = self->hitbox.left - 16;
}

#if RETRO_INCLUDE_EDITOR
void Button_EditorDraw(void)
{
    RSDK_THIS(Button);
    Vector2 drawPos;

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

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (self->type >= BUTTON_RWALL)
        drawPos.x -= self->pressPos;
    else
        drawPos.y += self->pressPos;
    RSDK.DrawSprite(&self->buttonAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);
}

void Button_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("MMZ/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x80000;
        Button->buttonOffset = 0x50000;
        Button->hitboxOffset = 13;
        Button->unused1      = 5;
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("FBZ/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x80000;
        Button->buttonOffset = 0x50000;
        Button->hitboxOffset = 13;
        Button->unused1      = 5;
    }
    else if (RSDK.CheckStageFolder("LRZ1")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("LRZ1/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x40000;
        Button->buttonOffset = 0x30000;
        Button->hitboxOffset = 7;
        Button->unused1      = 3;
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("LRZ2/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x40000;
        Button->buttonOffset = 0x30000;
        Button->hitboxOffset = 7;
        Button->unused1      = 3;
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x30000;
        Button->buttonOffset = 0x30000;
        Button->hitboxOffset = 9;
        Button->unused1      = 6;
    }
    else if (RSDK.CheckStageFolder("TMZ3")) {
        Button->aniFrames    = RSDK.LoadSpriteAnimation("TMZ1/Button.bin", SCOPE_STAGE);
        Button->activatePos  = 0x30000;
        Button->buttonOffset = 0x30000;
        Button->hitboxOffset = 9;
        Button->unused1      = 6;
    }
    else {
        Button->activatePos  = 0x80000;
        Button->buttonOffset = 0x50000;
        Button->hitboxOffset = 13;
        Button->unused1      = 5;
    }

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
