#include "SonicMania.h"

ObjectButton *Button;

void Button_Update(void)
{
    RSDK_THIS(Button);

    entity->field_64 = 0;
    entity->activated    = false;
    switch (entity->type) {
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

    if (!entity->field_64)
        entity->field_74 = 0;
    entity->animator2.frameID = entity->field_64 + 1;
}

void Button_LateUpdate(void) {}

void Button_StaticUpdate(void) {}

void Button_Draw(void)
{
    RSDK_THIS(Button);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (entity->type >= 2)
        drawPos.x -= entity->field_78;
    else
        drawPos.y += entity->field_78;
    RSDK.DrawSprite(&entity->data1, &drawPos, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void Button_Create(void *data)
{
    RSDK_THIS(Button);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        switch (entity->type) {
            case 0:
                entity->direction = FLIP_NONE;
                RSDK.SetSpriteAnimation(Button->spriteIndex, 0, &entity->data1, true, 0);
                RSDK.SetSpriteAnimation(Button->spriteIndex, 0, &entity->animator2, true, 1);
                entity->hitbox.left   = Button->hitboxV.left;
                entity->hitbox.top    = Button->hitboxV.top;
                entity->hitbox.right  = Button->hitboxV.right;
                entity->hitbox.bottom = Button->hitboxV.bottom;
                break;
            case 1:
                entity->direction = FLIP_Y;
                RSDK.SetSpriteAnimation(Button->spriteIndex, 0, &entity->data1, true, 0);
                RSDK.SetSpriteAnimation(Button->spriteIndex, 0, &entity->animator2, true, 1);
                entity->hitbox.left   = Button->hitboxV.left;
                entity->hitbox.top    = Button->hitboxV.top;
                entity->hitbox.right  = Button->hitboxV.right;
                entity->hitbox.bottom = Button->hitboxV.bottom;
                break;
            case 2:
                entity->direction = FLIP_NONE;
                RSDK.SetSpriteAnimation(Button->spriteIndex, 1, &entity->data1, true, 0);
                RSDK.SetSpriteAnimation(Button->spriteIndex, 1, &entity->animator2, true, 1);
                entity->hitbox.left   = Button->hitboxH.left;
                entity->hitbox.top    = Button->hitboxH.top;
                entity->hitbox.right  = Button->hitboxH.right;
                entity->hitbox.bottom = Button->hitboxH.bottom;
                break;
            case 3:
                entity->direction = FLIP_X;
                RSDK.SetSpriteAnimation(Button->spriteIndex, 1, &entity->data1, true, 0);
                RSDK.SetSpriteAnimation(Button->spriteIndex, 1, &entity->animator2, true, 1);
                entity->hitbox.left   = Button->hitboxH.left;
                entity->hitbox.top    = Button->hitboxH.top;
                entity->hitbox.right  = Button->hitboxH.right;
                entity->hitbox.bottom = Button->hitboxH.bottom;
                break;
            default: break;
        }
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x200000;
        entity->updateRange.y = 0x200000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
    }
}

void Button_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ")) {
        Button->spriteIndex = RSDK.LoadSpriteAnimation("MMZ/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x80000;
        Button->field_24    = 0x50000;
        Button->field_28    = 13;
        Button->field_2C    = 5;
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Button->spriteIndex = RSDK.LoadSpriteAnimation("FBZ/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x80000;
        Button->field_24    = 0x50000;
        Button->field_28    = 13;
        Button->field_2C    = 5;
    }
    else if (RSDK.CheckStageFolder("LRZ1")) {
        Button->spriteIndex = RSDK.LoadSpriteAnimation("LRZ1/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x40000;
        Button->field_24    = 0x30000;
        Button->field_28    = 7;
        Button->field_2C    = 3;
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        Button->spriteIndex = RSDK.LoadSpriteAnimation("LRZ2/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x40000;
        Button->field_24    = 0x30000;
        Button->field_28    = 7;
        Button->field_2C    = 3;
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Button->spriteIndex = RSDK.LoadSpriteAnimation("HCZ/Button.bin", SCOPE_STAGE);
        Button->field_20    = 0x30000;
        Button->field_24    = 0x30000;
        Button->field_28    = 9;
        Button->field_2C    = 6;
    }
    else if (RSDK.CheckStageFolder("TMZ3")) {
        Button->spriteIndex = RSDK.LoadSpriteAnimation("TMZ1/Button.bin", SCOPE_STAGE);
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
        entity->hitbox.top = (Button->field_20 >> 16) - (Button->field_28 & 0xFFFF);
        if (RSDK.CheckObjectCollisionPlatform(entity, &entity->hitbox, eggman, &eggman->hitbox, true)) {
            eggman->onGround = true;
            entity->field_78 = Button->field_20;
            if (!entity->field_74) {
                RSDK.PlaySfx(Button->sfxButton, 0, 255);
                entity->activated = true;
                entity->field_68 ^= 1;
            }
            entity->field_74 = 1;
            entity->field_64 = 1;
            entity->field_70 = 1;
        }
    }
}

void Button_CheckPRiderCollisions(void)
{
    RSDK_THIS(Button);
    foreach_active(PhantomRider, rider)
    {
        entity->hitbox.top = (Button->field_20 >> 16) - (Button->field_28 & 0xFFFF);
        /*if (RSDK.CheckObjectCollisionPlatform(entity, &entity->hitbox, rider, &rider->hitbox, true)) {
            entity->field_78 = Button->field_20;
            if (!entity->field_74) {
                RSDK.PlaySfx(Button->sfxButton, 0, 255);
                entity->activated = true;
                entity->field_68 ^= 1;
            }
            entity->field_74 = 1;
            entity->field_64 = 1;
            entity->field_70 = 1;
        }*/
    }
}

void Button_TypeFloor(void)
{
    RSDK_THIS(Button);
    int32 val          = entity->field_78;
    entity->field_78 = 0;
    foreach_active(Player, player)
    {
        entity->hitbox.top    = (val >> 16) - (Button->field_28 & 0xFFFF);
        entity->hitbox.bottom = entity->hitbox.top + 32;
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
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 1 || entity->walkOnto) {
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
            entity->hitbox.top -= (val >> 16);
            entity->hitbox.top += (Button->field_20 >> 16);
            int32 val2 = entity->field_78;
            if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox) == 1) {
                entity->field_78 = Button->field_20;
            }
            else {
                entity->position.y -= Button->field_20;
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    entity->field_78     = Button->field_24 + (player->position.y & 0xFFFF0000) + (playerHitbox->bottom << 16) - entity->position.y;
                    if (entity->field_78 <= Button->field_20) {
                        if (entity->field_78 < 0)
                            entity->field_78 = 0;
                    }
                    else {
                        entity->field_78 = Button->field_20;
                    }
                    entity->field_78 &= 0xFFFF0000;
                }
                entity->position.y += Button->field_20;
            }
            if (entity->field_78 == Button->field_20) {
                Player_CheckCollisionBox(player, entity, &entity->hitbox);
                player->angle = 0;
                if (!entity->field_74) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    entity->activated = true;
                    entity->field_68 ^= 1;
                }
                entity->field_74 = 1;
                entity->field_64 = 1;
                entity->field_70 = 1;
            }
            if (val2 > entity->field_78)
                entity->field_78 = val2;
        }
        if (entity->field_78)
            val = entity->field_78;
    }
}
void Button_TypeRoof(void)
{
    RSDK_THIS(Button);
    int32 val          = entity->field_78;
    entity->field_78 = 0;

    foreach_active(Player, player)
    {
        entity->hitbox.top    = -1 - (Button->field_28 & 0xFFFF) - (val >> 16);
        entity->hitbox.bottom = entity->hitbox.top + 32;
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
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 4 || entity->walkOnto) {
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
            entity->hitbox.top += (val >> 16);
            entity->hitbox.top += (Button->field_20 >> 16);
            int32 val2 = entity->field_78;
            if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 4) {
                entity->field_78 = -Button->field_20;
            }
            else {
                entity->position.y += Button->field_20;
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    entity->field_78     = (player->position.y & 0xFFFF0000) + (playerHitbox->top << 16) - Button->field_24 - entity->position.y;
                    if (entity->field_78 >= -Button->field_20) {
                        if (entity->field_78 > 0)
                            entity->field_78 = 0;
                    }
                    else {
                        entity->field_78 = -Button->field_20;
                    }
                    entity->field_78 &= 0xFFFF0000;
                }
                entity->position.y -= Button->field_20;
            }

            if (entity->field_78 == -Button->field_20) {
                if (!entity->field_74) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    entity->activated = true;
                    entity->field_68 ^= 1;
                }
                entity->field_74 = 1;
                entity->field_64 = 1;
                entity->field_70 = 1;
            }
            if (val2 < entity->field_78)
                entity->field_78 = val2;
        }
        if (entity->field_78)
            val = entity->field_78;
    }
}
void Button_TypeRWall(void)
{
    RSDK_THIS(Button);
    int32 val          = entity->field_78;
    entity->field_78 = 0;
    foreach_active(Player, player)
    {
        entity->hitbox.right = (Button->field_28 & 0xFFFF) - (val >> 16) + 1;
        entity->hitbox.left  = entity->hitbox.right - 16;
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

        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 3 || entity->walkOnto) {
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

            entity->hitbox.right += (val >> 16);
            entity->hitbox.right = entity->hitbox.right - (Button->field_20 >> 16) - 1;
            int32 val2             = entity->field_78;
            if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 3) {
                entity->field_78 = Button->field_20;
            }
            else {
                entity->position.x += Button->field_20;
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    entity->field_78     = Button->field_24 - (playerHitbox->left << 16) - (player->position.x & 0xFFFF0000) + entity->position.x;
                    if (entity->field_78 <= Button->field_20) {
                        if (entity->field_78 < 0)
                            entity->field_78 = 0;
                    }
                    else {
                        entity->field_78 = Button->field_20;
                    }
                    entity->field_78 &= 0xFFFF0000;
                }
                entity->position.x -= Button->field_20;
            }

            if (entity->field_78 == Button->field_20) {
                if (!entity->field_74) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    entity->activated = true;
                    entity->field_68 ^= 1;
                }
                entity->field_74 = 1;
                entity->field_64 = 1;
                entity->field_70 = 1;
            }
            if (val2 > entity->field_78)
                entity->field_78 = val2;
        }
        
        if (entity->field_78)
            val = entity->field_78;
    }
}
void Button_TypeLWall(void)
{
    RSDK_THIS(Button);
    int32 val          = entity->field_78;
    entity->field_78 = 0;

    foreach_active(Player, player)
    {
        entity->hitbox.right = (val >> 16) + (Button->field_28 & 0xFFFF);
        entity->hitbox.left  = entity->hitbox.right - 16;

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
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 2 || entity->walkOnto) {
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
            entity->hitbox.right -= (val >> 16);
            entity->hitbox.right -= (Button->field_20 >> 16);
            int32 val2 = entity->field_78;
            if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 2) {
                entity->field_78 = -Button->field_20;
            }
            else {
                entity->position.x -= Button->field_20;
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    entity->field_78     = entity->position.x - (playerHitbox->right << 16) - (player->position.x & 0xFFFF0000) - Button->field_24;
                    if (entity->field_78 >= -Button->field_20) {
                        if (entity->field_78 > 0)
                            entity->field_78 = 0;
                    }
                    else {
                        entity->field_78 = -Button->field_20;
                    }
                    entity->field_78 &= 0xFFFF0000;
                }
                entity->position.x += Button->field_20;
            }
            if (entity->field_78 == -Button->field_20) {
                if (!entity->field_74) {
                    RSDK.PlaySfx(Button->sfxButton, 0, 255);
                    entity->activated = true;
                    entity->field_68 ^= 1;
                }
                entity->field_74 = 1;
                entity->field_64 = 1;
                entity->field_70 = 1;
            }
            if (val2 < entity->field_78)
                entity->field_78 = val2;
        }

        if (entity->field_78)
            val = entity->field_78;
    }

    entity->hitbox.right = (Button->field_28 & 0xFFFF) + (val >> 16);
    entity->hitbox.left  = entity->hitbox.left - 16;
}

void Button_EditorDraw(void) {}

void Button_EditorLoad(void) {}

void Button_Serialize(void)
{
    RSDK_EDITABLE_VAR(Button, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Button, VAR_BOOL, walkOnto);
    RSDK_EDITABLE_VAR(Button, VAR_UINT8, tag);
}
