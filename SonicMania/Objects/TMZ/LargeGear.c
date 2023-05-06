// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LargeGear Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLargeGear *LargeGear;

void LargeGear_Update(void)
{
    RSDK_THIS(LargeGear);

    self->angle = self->rotOffset + self->rotSpeed * Zone->timer;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        int32 id        = 0;
        int32 storedID  = -1;
        bool32 collided = false;

        for (int32 angle = 0x180; angle < 0x380; angle += 0x40) {
            int32 ang = angle + self->angle;
            int32 x   = (storeX + 0x2400 * RSDK.Cos512(ang)) & 0xFFFF0000;
            int32 y   = (storeY + 0x2400 * RSDK.Sin512(ang)) & 0xFFFF0000;

            if (((1 << playerID) & self->activePlayers) && self->playerIDs[playerID] == id) {
                player->position.x += x - self->positions[id].x;
                player->position.y += y - self->positions[id].y;
            }

            self->position.x = x;
            self->position.y = y;

            if (Player_CheckCollisionBox(player, self, &LargeGear->hitboxTooth) == C_TOP) {
                collided = true;
                storedID = id;
            }

            self->positions[id].x   = x;
            self->positions[id++].y = y;
        }

        self->position.x = storeX;
        self->position.y = storeY;

        if (Player_CheckCollisionBox(player, self, &LargeGear->hitboxBase) == C_TOP) {
            collided = true;
            storedID = -1;
            self->activePlayers |= 1 << playerID;
        }
        else {
            if (collided)
                self->activePlayers |= 1 << playerID;
            else
                self->activePlayers &= ~(1 << playerID);
        }

        self->playerIDs[playerID] = storedID;
        if (collided) {
            Hitbox *playerHitbox = Player_GetHitbox(player);

            RSDK.ObjectTileCollision(player, player->collisionLayers, CMODE_RWALL, self->collisionPlane, playerHitbox->left << 16, 0, true);
            RSDK.ObjectTileCollision(player, player->collisionLayers, CMODE_LWALL, self->collisionPlane, playerHitbox->right << 16, 0, true);
            RSDK.ObjectTileCollision(player, player->collisionLayers, CMODE_FLOOR, self->collisionPlane, playerHitbox->bottom << 16, 0, true);
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;

    RSDK.ProcessAnimation(&self->baseAnimator);
}

void LargeGear_LateUpdate(void) {}

void LargeGear_StaticUpdate(void) {}

void LargeGear_Draw(void)
{
    RSDK_THIS(LargeGear);

    for (int32 i = 0; i < 0x200; i += 0x40) {
        self->rotation = i + self->angle;

        RSDK.SetSpriteAnimation(LargeGear->aniFrames, 1, &self->toothAnimator, true, 7 - abs(RSDK.Sin512(self->rotation) / 73));

        RSDK.DrawSprite(&self->toothAnimator, NULL, false);
        RSDK.Cos512(self->rotation + 0x180);
        RSDK.Sin512(self->rotation + 0x180);
        self->rotation = 0;
    }

    self->rotation = 0;
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);

    self->rotation = self->angle;
    RSDK.DrawSprite(&self->centerAnimator, NULL, false);

    self->rotation = 0;
    RSDK.DrawSprite(&self->axleAnimator, NULL, false);

    for (int32 i = 0; i < 0x200; i += 0x80) {
        int32 angle = i + self->angle;

        Vector2 drawPos = self->position;

        drawPos.x += 0x1300 * RSDK.Cos512(angle);
        drawPos.y += 0x1300 * RSDK.Sin512(angle);
        RSDK.DrawSprite(&self->rivetAnimator, &drawPos, false);
    }
}

void LargeGear_Create(void *data)
{
    RSDK_THIS(LargeGear);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->origin        = self->position;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(LargeGear->aniFrames, 0, &self->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(LargeGear->aniFrames, 1, &self->toothAnimator, true, 0);
    RSDK.SetSpriteAnimation(LargeGear->aniFrames, 2, &self->axleAnimator, true, 0);
    RSDK.SetSpriteAnimation(LargeGear->aniFrames, 3, &self->centerAnimator, true, 0);
    RSDK.SetSpriteAnimation(LargeGear->aniFrames, 4, &self->rivetAnimator, true, 0);

    self->angle = self->rotOffset;
}

void LargeGear_StageLoad(void)
{
    LargeGear->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/LargeGear.bin", SCOPE_STAGE);

    LargeGear->hitboxTooth.left   = -10;
    LargeGear->hitboxTooth.top    = -10;
    LargeGear->hitboxTooth.right  = 10;
    LargeGear->hitboxTooth.bottom = 10;

    LargeGear->hitboxBase.left   = -8;
    LargeGear->hitboxBase.top    = -64;
    LargeGear->hitboxBase.right  = 8;
    LargeGear->hitboxBase.bottom = 64;
}

#if GAME_INCLUDE_EDITOR
void LargeGear_EditorDraw(void)
{
    RSDK_THIS(LargeGear);

    self->origin = self->position;
    self->angle  = self->rotOffset;

    LargeGear_Draw();
}

void LargeGear_EditorLoad(void) { LargeGear->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/LargeGear.bin", SCOPE_STAGE); }
#endif

void LargeGear_Serialize(void)
{
    RSDK_EDITABLE_VAR(LargeGear, VAR_INT32, rotSpeed);
    RSDK_EDITABLE_VAR(LargeGear, VAR_INT32, rotOffset);
}
