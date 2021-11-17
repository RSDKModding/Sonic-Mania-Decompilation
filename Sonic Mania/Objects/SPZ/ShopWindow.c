#include "SonicMania.h"

ObjectShopWindow *ShopWindow;

void ShopWindow_Update(void)
{
    RSDK_THIS(ShopWindow);
    StateMachine_Run(entity->state);

    if (entity->silhouette) {
        foreach_active(Player, player)
        {
            EntityShield *shield = RSDK_GET_ENTITY((Player->playerCount + RSDK.GetEntityID(player)), Shield);
            if (shield->objectID == Shield->objectID && shield->type == SHIELD_BLUE) {
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox2))
                    shield->flag = Zone->timer & 1;
            }
        }
    }
}

void ShopWindow_LateUpdate(void) {}

void ShopWindow_StaticUpdate(void) {}

void ShopWindow_Draw(void)
{
    RSDK_THIS(ShopWindow);
    StateMachine_Run(entity->stateDraw);
}

void ShopWindow_Create(void *data)
{
    RSDK_THIS(ShopWindow);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible = true;
        if (data) {
            entity->active        = ACTIVE_NORMAL;
            entity->drawOrder     = Zone->drawOrderLow;
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;
            entity->drawFX        = FX_FLIP;
            entity->scale.x       = 0x200;
            entity->scale.y       = 0x200;
            entity->state         = ShopWindow_State_Shard;
            entity->stateDraw     = ShopWindow_StateDraw_Shard;
        }
        else {
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = entity->size.x;
            entity->updateRange.y = entity->size.y;
            if (!entity->silhouette)
                entity->drawOrder = Zone->drawOrderLow;
            else
                entity->drawOrder = Zone->drawOrderHigh;

            entity->inkEffect = INK_ADD;
            entity->alpha     = 0x80;
            entity->size.x >>= 0x10;
            entity->size.y >>= 0x10;
            if (entity->shatter) {
                entity->hitbox.top          = -entity->size.y;
                entity->hitbox.bottom       = entity->size.y;
                entity->hitbox.left         = -(entity->size.x >> 1);
                entity->hitbox.right        = entity->size.x >> 1;
                entity->windowHitbox.top    = -(entity->size.y >> 1);
                entity->windowHitbox.right  = entity->size.x;
                entity->windowHitbox.left   = -entity->size.x;
                entity->windowHitbox.bottom = entity->size.y >> 1;
                entity->state               = ShopWindow_State_Silhouette;
            }
            entity->stateDraw      = ShopWindow_StateDraw_Normal;
            entity->hitbox2.top    = -8 - entity->size.y;
            entity->hitbox2.right  = entity->size.x + 8;
            entity->hitbox2.bottom = entity->size.y + 8;
            entity->hitbox2.left   = -8 - entity->size.x;
            RSDK.SetSpriteAnimation(ShopWindow->aniFrames, 0, &entity->animator, false, 0);
        }
        
        foreach_all(CircleBumper, bumper) {
            if (RSDK.CheckObjectCollisionTouchBox(bumper, &CircleBumper->hitbox, entity, &entity->hitbox2))
                bumper->drawOrder = Zone->drawOrderLow;
        }
    }
}

void ShopWindow_StageLoad(void)
{
    ShopWindow->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/ShopWindow.bin", SCOPE_STAGE);
    RSDK.SetPaletteMask(RSDK.GetPaletteEntry(0, 253));
    ShopWindow->sfxShatter = RSDK.GetSFX("Stage/WindowShatter.wav");
}

void ShopWindow_State_Shard(void)
{
    RSDK_THIS(ShopWindow);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    entity->field_90.x += entity->field_88.x;
    entity->field_90.y += entity->field_88.y;
    entity->scale.x = RSDK.Cos512(entity->field_90.x);
    entity->scale.y = RSDK.Cos512(entity->field_90.y);

    RSDK.ProcessAnimation(&entity->animator);
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void ShopWindow_State_Shattered(void)
{
    RSDK_THIS(ShopWindow);
    if (++entity->animator.animationTimer == 2) {
        int32 cntY = (entity->size.y >> 3) + 1;
        int32 cntX = (entity->size.x >> 3) + 1;

        int32 posY = entity->position.y - (entity->size.y << 16) + 0x80000;
        for (int32 y = 0; y < cntY; ++y) {
            int32 posX = entity->position.x - (entity->size.x << 16) + 0x80000;
            for (int32 x = 0; x < cntX; ++x) {
                EntityShopWindow *shard = (EntityShopWindow *)RSDK.CreateEntity(ShopWindow->objectID, (void *)1, posX + ((RSDK.Rand(0, 8) - 4) << 16),
                                                                                posY + ((RSDK.Rand(0, 8) - 4) << 16));
                if (RSDK.Rand(0, 3) == 1) {
                    RSDK.SetSpriteAnimation(ShopWindow->aniFrames, 3, &shard->animator, 0, RSDK.Rand(0, 18));
                }
                else {
                    RSDK.SetSpriteAnimation(ShopWindow->aniFrames, 2, &shard->animator, 0, RSDK.Rand(0, 21));
                }
                shard->field_88.x = RSDK.Rand(1, 16);
                shard->field_88.y = RSDK.Rand(1, 16);
                shard->field_90.x = RSDK.Rand(0, 512);
                shard->field_90.y = RSDK.Rand(0, 512);
                shard->velocity.x = RSDK.Rand(-6, 6) << 16;
                posX += 0x100000;
                shard->velocity.y = RSDK.Rand(-6, 2) << 16;
            }
            posY += 0x100000;
        }
        RSDK.PlaySfx(ShopWindow->sfxShatter, false, 0xFF);
        destroyEntity(entity);
    }
}

void ShopWindow_State_Silhouette(void)
{
    RSDK_THIS(ShopWindow);
    if (entity->shatter) {

        foreach_active(Player, player)
        {
            if (abs(player->velocity.x) >= 0xA0000) {
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                    entity->stateDraw = ShopWindow_StateDraw_Shattered;
                    entity->state     = ShopWindow_State_Shattered;
                }
            }

            if (abs(player->velocity.y) >= 0xA0000) {
                if (Player_CheckCollisionTouch(player, entity, &entity->windowHitbox)) {
                    entity->stateDraw = ShopWindow_StateDraw_Shattered;
                    entity->state     = ShopWindow_State_Shattered;
                }
            }
        }
    }
}

void ShopWindow_StateDraw_Normal(void)
{
    RSDK_THIS(ShopWindow);
    Vector2 drawPos;
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];

    int32 x = (entity->position.x >> 0x10) - screen->position.x - entity->size.x;
    int32 y = (entity->position.y >> 0x10) - screen->position.y - entity->size.y;
    RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, x, y, x + 2 * entity->size.x, y + 2 * entity->size.y + 1);

    drawPos.x = (x - (uint8)(x + (screen->position.x >> 1))) << 16;
    drawPos.y = (y - (uint8)(y + (screen->position.y >> 1))) << 16;
    if (entity->silhouette)
        RSDK.DrawRect(x, y, 2 * entity->size.x, 2 * entity->size.y, 0x100068, 255, INK_UNMASKED, true);

    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, &drawPos, true);

    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, &drawPos, true);

    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &drawPos, true);

    entity->animator.frameID = 3;
    RSDK.DrawSprite(&entity->animator, &drawPos, true);

    if (!entity->shatter) {
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
    }
    else {
        drawPos.x            = entity->position.x - (entity->size.x << 16) + 0x180000;
        drawPos.y            = entity->position.y + ((entity->size.y - 24) << 16);
        entity->inkEffect    = INK_NONE;
        entity->animator.frameID = 4;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        entity->inkEffect = INK_ADD;
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
    }
}

void ShopWindow_StateDraw_Shard(void)
{
    RSDK_THIS(ShopWindow);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void ShopWindow_StateDraw_Shattered(void)
{
    RSDK_THIS(ShopWindow);
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];
    RSDK.DrawRect((entity->position.x >> 0x10) - screen->position.x - entity->size.x,
                  (entity->position.y >> 0x10) - screen->position.y - entity->size.y, 2 * entity->size.x, 2 * entity->size.y, 0xF0F0F0, 255, 0, true);
}

#if RETRO_INCLUDE_EDITOR
void ShopWindow_EditorDraw(void)
{
    RSDK_THIS(ShopWindow);
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = entity->size.x;
    entity->updateRange.y = entity->size.y;
    if (!entity->silhouette)
        entity->drawOrder = Zone->drawOrderLow;
    else
        entity->drawOrder = Zone->drawOrderHigh;

    entity->inkEffect = INK_ADD;
    entity->alpha     = 0x80;
    RSDK.SetSpriteAnimation(ShopWindow->aniFrames, 0, &entity->animator, false, 0);

    Vector2 size;
    size.x = entity->size.x >> 16;
    size.y = entity->size.y >> 16;

    Vector2 drawPos;
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];

    int32 x = (entity->position.x >> 0x10) - screen->position.x - size.x;
    int32 y = (entity->position.y >> 0x10) - screen->position.y - size.y;
    RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, x, y, x + 2 * size.x, y + 2 * size.y + 1);

    drawPos.x = (x + screen->position.x) << 16;
    drawPos.y = (y + screen->position.y) << 16;

    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 3;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    if (!entity->shatter) {
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
    }
    else {
        drawPos.x                = entity->position.x - (size.x << 16) + 0x180000;
        drawPos.y                = entity->position.y + ((size.y - 24) << 16);
        entity->inkEffect        = INK_NONE;
        entity->animator.frameID = 4;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        entity->inkEffect = INK_ADD;
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
    }

    DrawHelpers_DrawRectOutline(0xFFFF00, entity->position.x, entity->position.y, entity->size.x * 2, entity->size.y * 2);
}

void ShopWindow_EditorLoad(void) { ShopWindow->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/ShopWindow.bin", SCOPE_STAGE); }
#endif

void ShopWindow_Serialize(void)
{
    RSDK_EDITABLE_VAR(ShopWindow, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(ShopWindow, VAR_BOOL, shatter);
    RSDK_EDITABLE_VAR(ShopWindow, VAR_BOOL, silhouette);
}
