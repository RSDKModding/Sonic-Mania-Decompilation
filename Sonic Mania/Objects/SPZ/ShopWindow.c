// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ShopWindow Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectShopWindow *ShopWindow;

void ShopWindow_Update(void)
{
    RSDK_THIS(ShopWindow);
    StateMachine_Run(self->state);

    if (self->silhouette) {
        foreach_active(Player, player)
        {
            EntityShield *shield = RSDK_GET_ENTITY((Player->playerCount + RSDK.GetEntityID(player)), Shield);
            if (shield->objectID == Shield->objectID && shield->type == SHIELD_BLUE) {
                if (Player_CheckCollisionTouch(player, self, &self->hitboxItem))
                    shield->forceVisible = Zone->timer & 1;
            }
        }
    }
}

void ShopWindow_LateUpdate(void) {}

void ShopWindow_StaticUpdate(void) {}

void ShopWindow_Draw(void)
{
    RSDK_THIS(ShopWindow);
    StateMachine_Run(self->stateDraw);
}

void ShopWindow_Create(void *data)
{
    RSDK_THIS(ShopWindow);
    if (!SceneInfo->inEditor) {
        self->visible = true;
        if (data) {
            self->active        = ACTIVE_NORMAL;
            self->drawOrder     = Zone->drawOrderLow;
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            self->drawFX        = FX_FLIP;
            self->scale.x       = 0x200;
            self->scale.y       = 0x200;
            self->state         = ShopWindow_State_Shard;
            self->stateDraw     = ShopWindow_Draw_Shard;
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = self->size.x;
            self->updateRange.y = self->size.y;
            if (!self->silhouette)
                self->drawOrder = Zone->drawOrderLow;
            else
                self->drawOrder = Zone->drawOrderHigh;

            self->inkEffect = INK_ADD;
            self->alpha     = 0x80;
            self->size.x >>= 0x10;
            self->size.y >>= 0x10;
            if (self->shatter) {
                self->hitboxWindowX.left   = -(self->size.x >> 1);
                self->hitboxWindowX.top    = -self->size.y;
                self->hitboxWindowX.right  = self->size.x >> 1;
                self->hitboxWindowX.bottom = self->size.y;

                self->hitboxWindowY.left   = -self->size.x;
                self->hitboxWindowY.top    = -(self->size.y >> 1);
                self->hitboxWindowY.right  = self->size.x;
                self->hitboxWindowY.bottom = self->size.y >> 1;

                self->state = ShopWindow_State_Silhouette;
            }
            self->stateDraw         = ShopWindow_Draw_Normal;
            self->hitboxItem.top    = -8 - self->size.y;
            self->hitboxItem.right  = self->size.x + 8;
            self->hitboxItem.bottom = self->size.y + 8;
            self->hitboxItem.left   = -8 - self->size.x;
            RSDK.SetSpriteAnimation(ShopWindow->aniFrames, 0, &self->animator, false, 0);
        }

        foreach_all(CircleBumper, bumper)
        {
            if (RSDK.CheckObjectCollisionTouchBox(bumper, &CircleBumper->hitbox, self, &self->hitboxItem))
                bumper->drawOrder = Zone->drawOrderLow;
        }
    }
}

void ShopWindow_StageLoad(void)
{
    ShopWindow->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/ShopWindow.bin", SCOPE_STAGE);

    RSDK.SetPaletteMask(RSDK.GetPaletteEntry(0, 253));

    ShopWindow->sfxShatter = RSDK.GetSfx("Stage/WindowShatter.wav");
}

void ShopWindow_State_Shard(void)
{
    RSDK_THIS(ShopWindow);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->shardScale.x += self->scaleSpeed.x;
    self->shardScale.y += self->scaleSpeed.y;
    self->scale.x = RSDK.Cos512(self->shardScale.x);
    self->scale.y = RSDK.Cos512(self->shardScale.y);

    RSDK.ProcessAnimation(&self->animator);
    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void ShopWindow_State_Shattered(void)
{
    RSDK_THIS(ShopWindow);
    if (++self->animator.timer == 2) {
        int32 cntY = (self->size.y >> 3) + 1;
        int32 cntX = (self->size.x >> 3) + 1;

        int32 posY = self->position.y - (self->size.y << 16) + 0x80000;
        for (int32 y = 0; y < cntY; ++y) {
            int32 posX = self->position.x - (self->size.x << 16) + 0x80000;
            for (int32 x = 0; x < cntX; ++x) {
                EntityShopWindow *shard =
                    CREATE_ENTITY(ShopWindow, intToVoid(true), posX + ((RSDK.Rand(0, 8) - 4) << 16), posY + ((RSDK.Rand(0, 8) - 4) << 16));

                if (RSDK.Rand(0, 3) == 1) 
                    RSDK.SetSpriteAnimation(ShopWindow->aniFrames, 3, &shard->animator, 0, RSDK.Rand(0, 18));
                else 
                    RSDK.SetSpriteAnimation(ShopWindow->aniFrames, 2, &shard->animator, 0, RSDK.Rand(0, 21));

                shard->scaleSpeed.x = RSDK.Rand(1, 16);
                shard->scaleSpeed.y = RSDK.Rand(1, 16);
                shard->shardScale.x = RSDK.Rand(0, 512);
                shard->shardScale.y = RSDK.Rand(0, 512);
                shard->velocity.x   = RSDK.Rand(-6, 6) << 16;
                posX += 0x100000;
                shard->velocity.y = RSDK.Rand(-6, 2) << 16;
            }
            posY += 0x100000;
        }
        RSDK.PlaySfx(ShopWindow->sfxShatter, false, 0xFF);
        destroyEntity(self);
    }
}

void ShopWindow_State_Silhouette(void)
{
    RSDK_THIS(ShopWindow);
    if (self->shatter) {

        foreach_active(Player, player)
        {
            if (abs(player->velocity.x) >= 0xA0000) {
                if (Player_CheckCollisionTouch(player, self, &self->hitboxWindowX)) {
                    self->stateDraw = ShopWindow_Draw_Shattered;
                    self->state     = ShopWindow_State_Shattered;
                }
            }

            if (abs(player->velocity.y) >= 0xA0000) {
                if (Player_CheckCollisionTouch(player, self, &self->hitboxWindowY)) {
                    self->stateDraw = ShopWindow_Draw_Shattered;
                    self->state     = ShopWindow_State_Shattered;
                }
            }
        }
    }
}

void ShopWindow_Draw_Normal(void)
{
    RSDK_THIS(ShopWindow);
    Vector2 drawPos;
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    int32 x = (self->position.x >> 0x10) - screen->position.x - self->size.x;
    int32 y = (self->position.y >> 0x10) - screen->position.y - self->size.y;
    RSDK.SetClipBounds(SceneInfo->currentScreenID, x, y, x + 2 * self->size.x, y + 2 * self->size.y + 1);

    drawPos.x = (x - (uint8)(x + (screen->position.x >> 1))) << 16;
    drawPos.y = (y - (uint8)(y + (screen->position.y >> 1))) << 16;
    if (self->silhouette)
        RSDK.DrawRect(x, y, 2 * self->size.x, 2 * self->size.y, 0x100068, 255, INK_UNMASKED, true);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, &drawPos, true);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, true);

    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, true);

    self->animator.frameID = 3;
    RSDK.DrawSprite(&self->animator, &drawPos, true);

    if (!self->shatter) {
        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
    }
    else {
        drawPos.x              = self->position.x - (self->size.x << 16) + 0x180000;
        drawPos.y              = self->position.y + ((self->size.y - 24) << 16);
        self->inkEffect        = INK_NONE;
        self->animator.frameID = 4;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->inkEffect = INK_ADD;
        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
    }
}

void ShopWindow_Draw_Shard(void)
{
    RSDK_THIS(ShopWindow);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void ShopWindow_Draw_Shattered(void)
{
    RSDK_THIS(ShopWindow);
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];
    RSDK.DrawRect((self->position.x >> 0x10) - screen->position.x - self->size.x, (self->position.y >> 0x10) - screen->position.y - self->size.y,
                  2 * self->size.x, 2 * self->size.y, 0xF0F0F0, 255, 0, true);
}

#if RETRO_INCLUDE_EDITOR
void ShopWindow_EditorDraw(void)
{
    RSDK_THIS(ShopWindow);
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = self->size.x;
    self->updateRange.y = self->size.y;
    if (!self->silhouette)
        self->drawOrder = Zone->drawOrderLow;
    else
        self->drawOrder = Zone->drawOrderHigh;

    self->inkEffect = INK_ADD;
    self->alpha     = 0x80;
    RSDK.SetSpriteAnimation(ShopWindow->aniFrames, 0, &self->animator, false, 0);

    Vector2 size;
    size.x = self->size.x >> 16;
    size.y = self->size.y >> 16;

    Vector2 drawPos;
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    int32 x = (self->position.x >> 0x10) - screen->position.x - size.x;
    int32 y = (self->position.y >> 0x10) - screen->position.y - size.y;
    RSDK.SetClipBounds(SceneInfo->currentScreenID, x, y, x + 2 * size.x, y + 2 * size.y + 1);

    drawPos.x = (x + screen->position.x) << 16;
    drawPos.y = (y + screen->position.y) << 16;

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 3;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    if (!self->shatter) {
        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
    }
    else {
        drawPos.x              = self->position.x - (size.x << 16) + 0x180000;
        drawPos.y              = self->position.y + ((size.y - 24) << 16);
        self->inkEffect        = INK_NONE;
        self->animator.frameID = 4;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->inkEffect = INK_ADD;
        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
    }

    DrawHelpers_DrawRectOutline(0xFFFF00, self->position.x, self->position.y, self->size.x * 2, self->size.y * 2);
}

void ShopWindow_EditorLoad(void) { ShopWindow->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/ShopWindow.bin", SCOPE_STAGE); }
#endif

void ShopWindow_Serialize(void)
{
    RSDK_EDITABLE_VAR(ShopWindow, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(ShopWindow, VAR_BOOL, shatter);
    RSDK_EDITABLE_VAR(ShopWindow, VAR_BOOL, silhouette);
}
