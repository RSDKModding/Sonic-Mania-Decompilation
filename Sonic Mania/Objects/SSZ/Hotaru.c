#include "SonicMania.h"

ObjectHotaru *Hotaru;

void Hotaru_Update(void)
{
    RSDK_THIS(Hotaru);
    StateMachine_Run(entity->state);
}

void Hotaru_LateUpdate(void) {}

void Hotaru_StaticUpdate(void) {}

void Hotaru_Draw(void)
{
    RSDK_THIS(Hotaru);
    Vector2 drawPos;

    entity->direction = entity->field_88;
    if (entity->field_A2 == 2) {
        entity->inkEffect = INK_ADD;
        entity->alpha     = 0x80;
        drawPos.x         = entity->position.x + entity->offset.x;
        drawPos.y         = entity->offset.y + 0xC0000 + entity->position.y;
        drawPos.x += entity->offset2.x;
        drawPos.y += entity->offset2.y;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        drawPos.x += entity->offset2.x;
        drawPos.y += entity->offset2.y;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        drawPos.x += entity->offset2.x;
        drawPos.y += entity->offset2.y;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->alpha     = 0x100;
        entity->inkEffect = INK_NONE;
    }

    if (!(entity->flags & 1)) {
        drawPos.x = entity->position.x + entity->dist2.x;
        drawPos.y = entity->position.y + entity->dist2.y;
        if (entity->alpha > 0 && (((Zone->timer & 1) << (entity->state == Hotaru_State_Unknown7)) & 0x7FFFFFFF)) {
            entity->inkEffect = INK_ADD;
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

            entity->inkEffect = INK_NONE;
        }
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    }

    if (!(entity->flags & 2)) {
        entity->direction ^= FLIP_X;
        drawPos.x = entity->position.x + entity->offset.x;
        drawPos.y = entity->position.y + entity->offset.y;
        if (entity->alpha > 0 && (((Zone->timer & 1) << (entity->state == Hotaru_State_Unknown7)) & 0x7FFFFFFF)) {
            entity->inkEffect = INK_ADD;
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

            entity->inkEffect = INK_NONE;
        }
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    }
}

void Hotaru_Create(void *data)
{
    RSDK_THIS(Hotaru);
    if (!SceneInfo->inEditor) {
        entity->distX1 <<= 16;
        entity->distX2 <<= 16;
        entity->speed <<= 15;
        entity->offset.x  = -entity->dist.x;
        entity->offset.y  = -entity->dist.y;
        entity->dist2.x   = entity->dist.x;
        entity->dist2.y   = entity->dist.y;
        entity->visible   = true;
        entity->drawOrder = Zone->drawOrderLow;
        entity->active    = ACTIVE_BOUNDS;
        if (entity->distX1 <= entity->distX2)
            entity->updateRange.x = entity->distX2 + 0x400000;
        else
            entity->updateRange.x = entity->distX1 + 0x400000;
        entity->updateRange.y = entity->dist.y + 0x400000;

        entity->drawFX     = FX_ROTATE | FX_FLIP;
        entity->startPos.x = entity->position.x;
        entity->startPos.y = entity->position.y;
        entity->startDir   = entity->direction;
        RSDK.SetSpriteAnimation(Hotaru->aniFrames, entity->quality, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Hotaru->aniFrames, 2, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(Hotaru->aniFrames, 3, &entity->animator3, true, 0);
        entity->state = Hotaru_State_Setup;
    }
}

void Hotaru_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Hotaru.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Hotaru.bin", SCOPE_STAGE);

    Hotaru->hitbox1.top    = -6;
    Hotaru->hitbox1.left   = -6;
    Hotaru->hitbox1.right  = 6;
    Hotaru->hitbox1.bottom = 6;
    Hotaru->hitbox2.top    = -32;
    Hotaru->hitbox2.left   = -32;
    Hotaru->hitbox2.right  = 32;
    Hotaru->hitbox2.bottom = 32;
    Hotaru->hitbox3.top    = -8;
    Hotaru->hitbox3.left   = -8;
    Hotaru->hitbox3.right  = 8;
    Hotaru->hitbox3.bottom = 8;

    DEBUGMODE_ADD_OBJ(Hotaru);
}

void Hotaru_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Hotaru, NULL, entity->position.x, entity->position.y);
}

void Hotaru_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Hotaru->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Hotaru_CheckPlayerCollisions(void)
{
    RSDK_THIS(Hotaru);

    int storeX = entity->position.x;
    int storeY = entity->position.y;

    foreach_active(Player, player)
    {
        if (!(entity->flags & 1)) {
            entity->position.x += entity->dist2.x;
            entity->position.y += entity->dist2.y;
            if (Player_CheckBadnikTouch(player, entity, &Hotaru->hitbox1) && Player_CheckBadnikBreak(entity, player, false)) {
                entity->flags |= 1;
                entity->field_A2 = 0;
            }
            entity->position.x = storeX;
            entity->position.y = storeY;
        }
        if (!(entity->flags & 2)) {
            entity->position.x += entity->offset.x;
            entity->position.y += entity->offset.y;
            if (Player_CheckBadnikTouch(player, entity, &Hotaru->hitbox1) && Player_CheckBadnikBreak(&entity, player, false)) {
                entity->flags |= 2;
                entity->field_A2 = 0;
            }
            entity->position.x = storeX;
            entity->position.y = storeY;
        }
    }
}

void Hotaru_CheckOnScreen(void)
{
    RSDK_THIS(Hotaru);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->speed >>= 15;
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->distX1     = (entity->distX1 >> 16);
        entity->distX2     = (entity->distX2 >> 16);
        entity->direction  = entity->startDir;
        Hotaru_Create(NULL);
    }
}

void Hotaru_State_Setup(void)
{
    RSDK_THIS(Hotaru);

    entity->active   = ACTIVE_NORMAL;
    entity->field_88 = 0;
    entity->alpha    = 0;
    entity->field_A2 = 0;
    entity->state    = Hotaru_State_Unknown1;
    Hotaru_State_Unknown1();
}

void Hotaru_State_Unknown1(void)
{
    RSDK_THIS(Hotaru);

    if (entity->field_88) {
        entity->offset.x -= entity->speed;
        entity->dist2.x += entity->speed;
        if (entity->dist2.x >= entity->distX2) {
            entity->offset.x = -entity->distX2;
            entity->dist2.x  = entity->distX2;
            entity->field_88 = 0;
        }
    }
    else {
        entity->offset.x += entity->speed;
        entity->dist2.x -= entity->speed;
        if (entity->offset.x >= entity->distX1) {
            entity->offset.x = entity->distX1;
            entity->dist2.x  = -entity->distX1;
            entity->field_88 = 1;
        }
    }
    Hotaru_CheckPlayerCollisions();
    int storeX = entity->position.x;
    int storeY = entity->position.y;

    foreach_active(Player, player)
    {
        bool32 flag = false;
        if (!(entity->flags & 1)) {
            entity->position.x += entity->dist2.x;
            entity->position.y += entity->dist2.y;
            if (Player_CheckCollisionTouch(player, entity, &Hotaru->hitbox2)) {
                flag = true;
            }
        }
        entity->position.x = storeX;
        entity->position.y = storeY;

        if (!(entity->flags & 2) && !flag) {
            entity->position.x += entity->offset.x;
            entity->position.y += entity->offset.y;
            if (Player_CheckCollisionTouch(player, entity, &Hotaru->hitbox2)) {
                flag = true;
            }
        }

        if (flag) {
            entity->playerPtr = (Entity *)player;
            if (player->sidekick)
                entity->screenID = 0;
            else
                entity->screenID = player->camera->screenID;

            entity->position.x = storeX;
            entity->position.y = storeY;
            entity->state      = Hotaru_State_Unknown2;
            foreach_break;
        }

        entity->position.x = storeX;
        entity->position.y = storeY;
    }
}

void Hotaru_State_Unknown2(void)
{
    RSDK_THIS(Hotaru);
    entity->state = Hotaru_State_Unknown3;
    Hotaru_State_Unknown3();
}

void Hotaru_State_Unknown3(void)
{
    RSDK_THIS(Hotaru);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;
    RSDKScreenInfo *screen   = &ScreenInfo[entity->screenID];

    int screenX = (screen->position.x + screen->centerX) << 16;
    int screenY = (screen->position.y + screen->centerY) << 16;

    if (entity->position.x <= screenX) {
        entity->position.x += player->velocity.x + 0x20000;
        if (entity->position.x > screenX)
            entity->position.x = screenX;
    }
    else {
        entity->position.x -= player->velocity.x - 0x20000;
        if (entity->position.x < screenX)
            entity->position.x = screenX;
    }

    if (entity->position.y <= screenY) {
        entity->position.y += player->velocity.y + 0x20000;
        if (entity->position.y > screenY)
            entity->position.y = screenY;
    }
    else {
        entity->position.y -= player->velocity.y - 0x20000;
        if (entity->position.y < screenY)
            entity->position.y = screenY;
    }
    if (entity->position.x == screenX && entity->position.y == screenY) {
        entity->timer = 100;
        entity->state = Hotaru_State_Unknown4;
    }

    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOnScreen();
}

void Hotaru_State_Unknown4(void)
{
    RSDK_THIS(Hotaru);
    RSDKScreenInfo *screen = &ScreenInfo[entity->screenID];

    entity->position.x = (screen->position.x + screen->centerX) << 16;
    entity->position.y = (screen->position.y + screen->centerY) << 16;
    if (++entity->rotation == 128) {
        entity->rotation          = 0;
        entity->animator1.frameID = 1;
        entity->state             = Hotaru_State_Unknown5;
    }
    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOnScreen();
}

void Hotaru_State_Unknown5(void)
{
    RSDK_THIS(Hotaru);

    if (entity->alpha >= 128) {
        Hitbox hitbox;
        if (entity->offset.x >= entity->dist2.x) {
            hitbox.left  = entity->dist2.x >> 16;
            hitbox.right = entity->offset.x >> 16;
        }
        else {
            hitbox.left  = entity->offset.x >> 16;
            hitbox.right = entity->dist2.x >> 16;
        }

        if (entity->offset.y >= entity->dist2.y) {
            hitbox.top    = entity->dist2.y >> 16;
            hitbox.bottom = entity->offset.y >> 16;
        }
        else {
            hitbox.top    = entity->offset.y >> 16;
            hitbox.bottom = entity->dist2.y >> 16;
        }

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                entity->field_A2 = 1;
            }
        }
    }

    entity->alpha += 2;
    if (entity->alpha == 256) {
        entity->timer = 128;
        if (!entity->flags && !entity->quality) {
            ++entity->field_A2;
            entity->offset2.x = (entity->dist2.x - entity->offset.x) >> 2;
            entity->offset2.y = (entity->dist2.y - entity->offset.y) >> 2;
        }
        entity->state = Hotaru_State_Unknown6;
    }
    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOnScreen();
}

void Hotaru_State_Unknown6(void)
{
    RSDK_THIS(Hotaru);

    RSDK.ProcessAnimation(&entity->animator3);
    if (--entity->timer <= 0) {
        entity->field_A2 = 0;
        entity->alpha    = 192;
        entity->state    = Hotaru_State_Unknown7;
    }
    Hotaru_CheckPlayerCollisions();

    if (entity->field_A2 == 2) {
        int storeX = entity->position.x;
        int storeY = entity->position.y;

        foreach_active(Player, player)
        {
            entity->position.x += entity->offset.x;
            entity->position.y += entity->offset.y;
            for (int i = 0; i < 3; ++i) {
                entity->position.x += entity->offset2.x;
                entity->position.y += entity->offset2.y;
                if (Player_CheckCollisionTouch(player, entity, &Hotaru->hitbox3)) {
                    Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
                }
            }
            entity->position.x = storeX;
            entity->position.y = storeY;
        }
    }
    Hotaru_CheckOnScreen();
}

void Hotaru_State_Unknown7(void)
{
    RSDK_THIS(Hotaru);
    entity->position.y -= 0x20000;
    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void Hotaru_EditorDraw(void)
{
    RSDK_THIS(Hotaru);

    entity->drawOrder = Zone->drawOrderLow;
    entity->active    = ACTIVE_BOUNDS;
    if (entity->distX1 <= entity->distX2)
        entity->updateRange.x = (entity->distX2 << 16) + 0x400000;
    else
        entity->updateRange.x = (entity->distX1 << 16) + 0x400000;
    entity->updateRange.y = entity->dist.y + 0x400000;

    entity->drawFX     = FX_ROTATE | FX_FLIP;
    entity->offset.x   = -entity->dist.x;
    entity->offset.y   = -entity->dist.y;
    entity->dist2.x    = entity->dist.x;
    entity->dist2.y    = entity->dist.y;
    RSDK.SetSpriteAnimation(Hotaru->aniFrames, entity->quality, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(Hotaru->aniFrames, 2, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(Hotaru->aniFrames, 3, &entity->animator3, true, 0);

    Hotaru_Draw();
}

void Hotaru_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Hotaru.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Hotaru.bin", SCOPE_STAGE);
}
#endif

void Hotaru_Serialize(void)
{
    RSDK_EDITABLE_VAR(Hotaru, VAR_UINT8, quality);
    RSDK_EDITABLE_VAR(Hotaru, VAR_VECTOR2, dist);
    RSDK_EDITABLE_VAR(Hotaru, VAR_ENUM, distX1);
    RSDK_EDITABLE_VAR(Hotaru, VAR_ENUM, distX2);
    RSDK_EDITABLE_VAR(Hotaru, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(Hotaru, VAR_UINT8, preset);
}
