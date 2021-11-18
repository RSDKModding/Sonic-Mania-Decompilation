#include "SonicMania.h"

ObjectButton *Button;

void Button_Update(void)
{
    RSDK_THIS(Button);

    self->field_64 = 0;
    self->activated    = false;
    switch (self->type) {
        case 0:
            Button_TypeFloor();
            if (Button->hasEggman)
                Button_CheckEggmanCollisions();
            if (Button->hasPhantomRider)
                Button_CheckPRiderCollisions();
            break;
        case 1: Button_TypeRoof(); break;
        case 2: Button_TypeRWall(); break;
        case 3: Button_TypeLWall(); break;
        default: break;
    }

    if (!self->field_64)
        self->field_74 = 0;
    self->animator2.frameID = self->field_64 + 1;
}

void Button_LateUpdate(void) {}

void Button_StaticUpdate(void) {}

void Button_Draw(void)
{
    RSDK_THIS(Button);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (self->type >= 2)
        drawPos.x -= self->field_78;
    else
        drawPos.y += self->field_78;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void Button_Create(void *data)
{
    RSDK_THIS(Button);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        switch (self->type) {
            case 0:
                self->direction = FLIP_NONE;
                RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->animator2, true, 1);
                self->hitbox.left   = Button->hitboxV.left;
                self->hitbox.top    = Button->hitboxV.top;
                self->hitbox.right  = Button->hitboxV.right;
                self->hitbox.bottom = Button->hitboxV.bottom;
                break;
            case 1:
                self->direction = FLIP_Y;
                RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->animator2, true, 1);
                self->hitbox.left   = Button->hitboxV.left;
                self->hitbox.top    = Button->hitboxV.top;
                self->hitbox.right  = Button->hitboxV.right;
                self->hitbox.bottom = Button->hitboxV.bottom;
                break;
            case 2:
                self->direction = FLIP_NONE;
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->animator2, true, 1);
                self->hitbox.left   = Button->hitboxH.left;
                self->hitbox.top    = Button->hitboxH.top;
                self->hitbox.right  = Button->hitboxH.right;
                self->hitbox.bottom = Button->hitboxH.bottom;
                break;
            case 3:
                self->direction = FLIP_X;
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->animator2, true, 1);
                self->hitbox.left   = Button->hitboxH.left;
                self->hitbox.top    = Button->hitboxH.top;
                self->hitbox.right  = Button->hitboxH.right;
                self->hitbox.bottom = Button->hitboxH.bottom;
                break;
            default: break;
        }
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x200000;
        self->updateRange.y = 0x200000;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
    }
}

void Button_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x80000;
        Button->field_24    = 0x50000;
        Button->field_28    = 13;
        Button->field_2C    = 5;
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x80000;
        Button->field_24    = 0x50000;
        Button->field_28    = 13;
        Button->field_2C    = 5;
    }
    else if (RSDK.CheckStageFolder("LRZ1")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x40000;
        Button->field_24    = 0x30000;
        Button->field_28    = 7;
        Button->field_2C    = 3;
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x40000;
        Button->field_24    = 0x30000;
        Button->field_28    = 7;
        Button->field_2C    = 3;
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x30000;
        Button->field_24    = 0x30000;
        Button->field_28    = 9;
        Button->field_2C    = 6;
    }
    else if (RSDK.CheckStageFolder("TMZ3")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x30000;
        Button->field_24    = 0x30000;
        Button->field_28    = 9;
        Button->field_2C    = 6;
    }
    else {
        Button->field_20 = 0x80000;
        Button->field_24 = 0x50000;
        Button->field_28 = 13;
        Button->field_2C = 5;
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
        self->hitbox.top = (Button->field_20 >> 16) - (Button->field_28 & 0xFFFF);
        if (RSDK.CheckObjectCollisionPlatform(self, &self->hitbox, eggman, &eggman->hitbox, true)) {
            eggman->onGround = true;
            self->field_78 = Button->field_20;
            if (!self->field_74) {
                RSDK.PlaySfx(Button->sfxButton, false, 0xFF);
                self->activated = true;
                self->field_68 ^= 1;
            }
            self->field_74 = 1;
            self->field_64 = 1;
            self->field_70 = 1;
        }
    }
}

void Button_CheckPRiderCollisions(void)
{
    RSDK_THIS(Button);
    foreach_active(PhantomRider, rider)
    {
        self->hitbox.top = (Button->field_20 >> 16) - (Button->field_28 & 0xFFFF);
        if (RSDK.CheckObjectCollisionPlatform(self, &self->hitbox, rider, &rider->hitbox, true)) {
            self->field_78 = Button->field_20;
            if (!self->field_74) {
                RSDK.PlaySfx(Button->sfxButton, false, 0xFF);
                self->activated = true;
                self->field_68 ^= 1;
            }
            self->field_74 = 1;
            self->field_64 = 1;
            self->field_70 = 1;
        }
    }
}

void Button_TypeFloor(void)
{
    RSDK_THIS(Button);
    int32 val          = self->field_78;
    self->field_78 = 0;
    foreach_active(Player, player)
    {
        self->hitbox.top    = (val >> 16) - (Button->field_28 & 0xFFFF);
        self->hitbox.bottom = self->hitbox.top + 32;
        int32 playerX           = player->position.x;
        int32 playerY           = player->position.y;
        int32 xVel              = player->velocity.x;
        int32 yVel              = player->velocity.y;
        int32 vel               = player->groundVel;
        bool32 grounded       = player->onGround;
        bool32 groundedStore  = player->groundedStore;
        void *nextGState      = player->nextGroundState;
        void *nextAState      = player->nextAirState;
        void *state           = player->state;
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
            self->hitbox.top += (Button->field_20 >> 16);
            int32 val2 = self->field_78;
            if (Player_CheckCollisionPlatform(player, self, &self->hitbox) == 1) {
                self->field_78 = Button->field_20;
            }
            else {
                self->position.y -= Button->field_20;
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->field_78     = Button->field_24 + (player->position.y & 0xFFFF0000) + (playerHitbox->bottom << 16) - self->position.y;
                    if (self->field_78 <= Button->field_20) {
                        if (self->field_78 < 0)
                            self->field_78 = 0;
                    }
                    else {
                        self->field_78 = Button->field_20;
                    }
                    self->field_78 &= 0xFFFF0000;
                }
                self->position.y += Button->field_20;
            }
            if (self->field_78 == Button->field_20) {
                Player_CheckCollisionBox(player, self, &self->hitbox);
                player->angle = 0;
                if (!self->field_74) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    self->activated = true;
                    self->field_68 ^= 1;
                }
                self->field_74 = 1;
                self->field_64 = 1;
                self->field_70 = 1;
            }
            if (val2 > self->field_78)
                self->field_78 = val2;
        }
        if (self->field_78)
            val = self->field_78;
    }
}
void Button_TypeRoof(void)
{
    RSDK_THIS(Button);
    int32 val          = self->field_78;
    self->field_78 = 0;

    foreach_active(Player, player)
    {
        self->hitbox.top    = -1 - (Button->field_28 & 0xFFFF) - (val >> 16);
        self->hitbox.bottom = self->hitbox.top + 32;
        int32 playerX           = player->position.x;
        int32 playerY           = player->position.y;
        int32 xVel              = player->velocity.x;
        int32 yVel              = player->velocity.y;
        int32 vel               = player->groundVel;
        bool32 grounded       = player->onGround;
        bool32 groundedStore  = player->groundedStore;
        void *nextGState      = player->nextGroundState;
        void *nextAState      = player->nextAirState;
        void *state           = player->state;
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
            self->hitbox.top += (Button->field_20 >> 16);
            int32 val2 = self->field_78;
            if (Player_CheckCollisionBox(player, self, &self->hitbox) == 4) {
                self->field_78 = -Button->field_20;
            }
            else {
                self->position.y += Button->field_20;
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->field_78     = (player->position.y & 0xFFFF0000) + (playerHitbox->top << 16) - Button->field_24 - self->position.y;
                    if (self->field_78 >= -Button->field_20) {
                        if (self->field_78 > 0)
                            self->field_78 = 0;
                    }
                    else {
                        self->field_78 = -Button->field_20;
                    }
                    self->field_78 &= 0xFFFF0000;
                }
                self->position.y -= Button->field_20;
            }

            if (self->field_78 == -Button->field_20) {
                if (!self->field_74) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    self->activated = true;
                    self->field_68 ^= 1;
                }
                self->field_74 = 1;
                self->field_64 = 1;
                self->field_70 = 1;
            }
            if (val2 < self->field_78)
                self->field_78 = val2;
        }
        if (self->field_78)
            val = self->field_78;
    }
}
void Button_TypeRWall(void)
{
    RSDK_THIS(Button);
    int32 val          = self->field_78;
    self->field_78 = 0;
    foreach_active(Player, player)
    {
        self->hitbox.right = (Button->field_28 & 0xFFFF) - (val >> 16) + 1;
        self->hitbox.left  = self->hitbox.right - 16;
        int32 playerX          = player->position.x;
        int32 playerY          = player->position.y;
        int32 xVel             = player->velocity.x;
        int32 yVel             = player->velocity.y;
        int32 vel              = player->groundVel;
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
            self->hitbox.right = self->hitbox.right - (Button->field_20 >> 16) - 1;
            int32 val2             = self->field_78;
            if (Player_CheckCollisionBox(player, self, &self->hitbox) == 3) {
                self->field_78 = Button->field_20;
            }
            else {
                self->position.x += Button->field_20;
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->field_78     = Button->field_24 - (playerHitbox->left << 16) - (player->position.x & 0xFFFF0000) + self->position.x;
                    if (self->field_78 <= Button->field_20) {
                        if (self->field_78 < 0)
                            self->field_78 = 0;
                    }
                    else {
                        self->field_78 = Button->field_20;
                    }
                    self->field_78 &= 0xFFFF0000;
                }
                self->position.x -= Button->field_20;
            }

            if (self->field_78 == Button->field_20) {
                if (!self->field_74) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    self->activated = true;
                    self->field_68 ^= 1;
                }
                self->field_74 = 1;
                self->field_64 = 1;
                self->field_70 = 1;
            }
            if (val2 > self->field_78)
                self->field_78 = val2;
        }
        
        if (self->field_78)
            val = self->field_78;
    }
}
void Button_TypeLWall(void)
{
    RSDK_THIS(Button);
    int32 val          = self->field_78;
    self->field_78 = 0;

    foreach_active(Player, player)
    {
        self->hitbox.right = (val >> 16) + (Button->field_28 & 0xFFFF);
        self->hitbox.left  = self->hitbox.right - 16;

        int32 playerX          = player->position.x;
        int32 playerY          = player->position.y;
        int32 xVel             = player->velocity.x;
        int32 yVel             = player->velocity.y;
        int32 vel              = player->groundVel;
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
            self->hitbox.right -= (Button->field_20 >> 16);
            int32 val2 = self->field_78;
            if (Player_CheckCollisionBox(player, self, &self->hitbox) == 2) {
                self->field_78 = -Button->field_20;
            }
            else {
                self->position.x -= Button->field_20;
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    self->field_78     = self->position.x - (playerHitbox->right << 16) - (player->position.x & 0xFFFF0000) - Button->field_24;
                    if (self->field_78 >= -Button->field_20) {
                        if (self->field_78 > 0)
                            self->field_78 = 0;
                    }
                    else {
                        self->field_78 = -Button->field_20;
                    }
                    self->field_78 &= 0xFFFF0000;
                }
                self->position.x += Button->field_20;
            }
            if (self->field_78 == -Button->field_20) {
                if (!self->field_74) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    self->activated = true;
                    self->field_68 ^= 1;
                }
                self->field_74 = 1;
                self->field_64 = 1;
                self->field_70 = 1;
            }
            if (val2 < self->field_78)
                self->field_78 = val2;
        }

        if (self->field_78)
            val = self->field_78;
    }

    self->hitbox.right = (Button->field_28 & 0xFFFF) + (val >> 16);
    self->hitbox.left  = self->hitbox.left - 16;
}

void Button_EditorDraw(void)
{
    RSDK_THIS(Button);
    Vector2 drawPos;

    switch (self->type) {
        case 0:
            self->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->animator2, true, 1);
            break;
        case 1:
            self->direction = FLIP_Y;
            RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Button->aniFrames, 0, &self->animator2, true, 1);
            break;
        case 2:
            self->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->animator2, true, 1);
            break;
        case 3:
            self->direction = FLIP_X;
            RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Button->aniFrames, 1, &self->animator2, true, 1);
            break;
        default: break;
    }

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (self->type >= 2)
        drawPos.x -= self->field_78;
    else
        drawPos.y += self->field_78;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void Button_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x80000;
        Button->field_24    = 0x50000;
        Button->field_28    = 13;
        Button->field_2C    = 5;
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x80000;
        Button->field_24    = 0x50000;
        Button->field_28    = 13;
        Button->field_2C    = 5;
    }
    else if (RSDK.CheckStageFolder("LRZ1")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x40000;
        Button->field_24    = 0x30000;
        Button->field_28    = 7;
        Button->field_2C    = 3;
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x40000;
        Button->field_24    = 0x30000;
        Button->field_28    = 7;
        Button->field_2C    = 3;
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x30000;
        Button->field_24    = 0x30000;
        Button->field_28    = 9;
        Button->field_2C    = 6;
    }
    else if (RSDK.CheckStageFolder("TMZ3")) {
        Button->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x30000;
        Button->field_24    = 0x30000;
        Button->field_28    = 9;
        Button->field_2C    = 6;
    }
    else {
        Button->field_20 = 0x80000;
        Button->field_24 = 0x50000;
        Button->field_28 = 13;
        Button->field_2C = 5;
    }
}

void Button_Serialize(void)
{
    RSDK_EDITABLE_VAR(Button, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Button, VAR_BOOL, walkOnto);
    RSDK_EDITABLE_VAR(Button, VAR_UINT8, tag);
}
