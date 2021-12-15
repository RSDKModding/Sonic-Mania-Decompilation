// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ERZShinobi Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectERZShinobi *ERZShinobi;

void ERZShinobi_Update(void)
{
    RSDK_THIS(ERZShinobi);
    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;
    StateMachine_Run(self->state);
    self->rotStore = (self->rotStore + 8) & 0x1FF;
}

void ERZShinobi_LateUpdate(void) {}

void ERZShinobi_StaticUpdate(void) {}

void ERZShinobi_Draw(void)
{
    RSDK_THIS(ERZShinobi);

    if ((self->invincibilityTimer & 1))
        RSDK.CopyPalette(2, 128, 0, 128, 128);
    self->rotation = self->rotStore;

    for (int32 i = 0; i < 8; ++i) {
        Vector2 drawPos;

        drawPos.x                 = self->spearOffset * RSDK.Sin512(self->rotation) + self->position.x;
        drawPos.y                 = self->position.y - self->spearOffset * RSDK.Cos512(self->rotation);
        self->animator3.frameID = ((self->rotation + 16) >> 5) & 0xF;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        drawPos.x = ((5 * self->spearOffset * RSDK.Sin512(self->rotation)) >> 3) + self->position.x;
        drawPos.y = self->position.y - ((5 * self->spearOffset * RSDK.Cos512(self->rotation)) >> 3);
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->rotation += 64;
    }

    RSDK.DrawSprite(&self->animator1, NULL, false);
    if ((self->invincibilityTimer & 1))
        RSDK.CopyPalette(1, 128, 0, 128, 128);
}

void ERZShinobi_Create(void *data)
{
    RSDK_THIS(ERZShinobi);
    if (!SceneInfo->inEditor) {
        self->visible         = true;
        self->drawFX          = FX_ROTATE | FX_FLIP;
        self->drawOrder       = Zone->drawOrderLow;
        self->active          = ACTIVE_NORMAL;
        self->updateRange.x   = 0x800000;
        self->updateRange.y   = 0x800000;
        self->collisionLayers = Zone->fgLayers;
        self->tileCollisions  = true;
        self->spearOffset     = 0x1600;
        self->state           = ERZShinobi_Unknown3;
        RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &self->animator2, true, 1);
        RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 1, &self->animator3, true, 0);
    }
}

void ERZShinobi_StageLoad(void)
{
    ERZShinobi->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomShinobi.bin", SCOPE_STAGE);

    ERZShinobi->hitbox.left   = -24;
    ERZShinobi->hitbox.top    = -24;
    ERZShinobi->hitbox.right  = 24;
    ERZShinobi->hitbox.bottom = 24;
}

void ERZShinobi_CheckPlayerCollisions(void)
{
    RSDK_THIS(ERZShinobi);

    foreach_active(Player, playerLoop)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(playerLoop, self, &ERZShinobi->hitbox) && Player_CheckBossHit(playerLoop, self)) {
            self->invincibilityTimer = 48;
            RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
            foreach_return;
        }
    }

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    int32 angle  = self->rotStore;
    foreach_active(Player, player)
    {
        self->position.x = storeX + self->spearOffset * RSDK.Sin512(angle);
        self->position.y = storeY - self->spearOffset * RSDK.Cos512(angle);
        if (RSDK.CheckObjectCollisionTouchCircle(player, 0xC0000, self, 0x80000)) {
            Player_CheckHit(player, self);
        }
        angle += 64;
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void ERZShinobi_HandleTileCollisions(void)
{
    RSDK_THIS(ERZShinobi);
    if (self->onGround) {
        self->spearOffset += (4096 - self->spearOffset) >> 3;
        if (!self->prevOnGround) {
            if (self->field_78 > 0) {
                self->velocity.y = -0x30000;
                self->field_78--;
                self->onGround = false;
            }
        }
    }
    else {
        self->velocity.y += 0x3800;
        if (self->velocity.y > 0xC0000)
            self->velocity.y = 0xC0000;
        self->spearOffset += (0x1600 - self->spearOffset) >> 3;
    }

    int32 val               = self->spearOffset / 88;
    self->outerBox.right  = val;
    self->outerBox.bottom = val;
    self->outerBox.left   = -val;
    self->outerBox.top    = -val;
    self->innerBox.left   = 2 - val;
    self->innerBox.right  = val - 2;
    self->innerBox.top    = -val;
    self->innerBox.bottom = val;

    self->prevOnGround = self->onGround;
    RSDK.ProcessTileCollisions(self, &self->outerBox, &self->innerBox);
}

void ERZShinobi_Unknown3(void)
{
    RSDK_THIS(ERZShinobi);
    ERZShinobi_HandleTileCollisions();
    if (++self->timer == 60) {
        self->timer = 0;
        if (self->onGround) {
            self->field_78   = 2;
            self->velocity.y = -0x80000;
            self->onGround   = false;
        }
    }
    ERZShinobi_CheckPlayerCollisions();
}

#if RETRO_INCLUDE_EDITOR
void ERZShinobi_EditorDraw(void)
{
    RSDK_THIS(ERZShinobi);

    self->spearOffset = 0x1600;
    RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &self->animator2, false, 1);
    RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 1, &self->animator3, false, 0);

    ERZShinobi_Draw();
}

void ERZShinobi_EditorLoad(void) { ERZShinobi->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomShinobi.bin", SCOPE_STAGE); }
#endif

void ERZShinobi_Serialize(void) {}
