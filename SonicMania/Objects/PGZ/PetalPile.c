// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PetalPile Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPetalPile *PetalPile;

void PetalPile_Update(void)
{
    RSDK_THIS(PetalPile);

    StateMachine_Run(self->state);

    if (!self->animator.animationID)
        RSDK.ProcessAnimation(&self->animator);
}

void PetalPile_LateUpdate(void) {}

void PetalPile_StaticUpdate(void) {}

void PetalPile_Draw(void)
{
    RSDK_THIS(PetalPile);
    StateMachine_Run(self->stateDraw);
}

void PetalPile_Create(void *data)
{
    RSDK_THIS(PetalPile);

    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[1];
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x100000;
    self->updateRange.y = 0x100000;

    if (SceneInfo->inEditor) {
        if (!self->maxSpeed.x)
            self->maxSpeed.x = 0x18000;

        if (!self->pileSize.x && !self->pileSize.y) {
            self->pileSize.x = 0x300000;
            self->pileSize.y = 0x80000;
        }
    }
    else {
        if (self->tileLayer)
            self->layerID = RSDK.GetTileLayerID("FG Low");
        else
            self->layerID = RSDK.GetTileLayerID("FG High");
    }

    RSDK.SetSpriteAnimation(PetalPile->aniFrames, 0, &self->animator, true, 0);
    self->state = PetalPile_State_Init;
}

void PetalPile_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        PetalPile->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Petal.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        PetalPile->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Petal.bin", SCOPE_STAGE);

    PetalPile->sfxPetals = RSDK.GetSfx("PSZ/Petals.wav");
}

int32 PetalPile_GetLeafPattern(Vector2 *patternPtr)
{
    RSDK_THIS(PetalPile);

    int32 count    = PetalPile->patternSize[self->leafPattern];
    int32 *pattern = NULL;
    switch (self->leafPattern) {
        case PETALPILE_PATTERN_0: pattern = PetalPile->pattern1; break;
        case PETALPILE_PATTERN_1: pattern = PetalPile->pattern2; break;
        case PETALPILE_PATTERN_2: pattern = PetalPile->pattern3; break;
        case PETALPILE_PATTERN_3: pattern = PetalPile->pattern4; break;
        case PETALPILE_PATTERN_4: pattern = PetalPile->pattern5; break;
        default: return 0;
    }

    int32 sizeX = MAX(self->pileSize.x, 0x20000);
    int32 sizeY = MAX(self->pileSize.y, 0x20000);
    for (int32 i = 0; i < count; ++i) {
        patternPtr[i].x = pattern[(i * 2) + 0] * (sizeX >> 17);
        patternPtr[i].y = pattern[(i * 2) + 1] * (sizeY >> 17);
    }

    return count;
}

void PetalPile_State_Init(void)
{
    RSDK_THIS(PetalPile);

    self->hitbox.left   = -(self->pileSize.x >> 17);
    self->hitbox.top    = -(self->pileSize.y >> 17);
    self->hitbox.right  = self->pileSize.x >> 17;
    self->hitbox.bottom = self->pileSize.y >> 17;

    if (self->noRemoveTiles) {
        self->state = PetalPile_State_SetupEmitter;
        PetalPile_State_SetupEmitter();
    }
    else {
        self->state = PetalPile_State_HandleInteractions;
        PetalPile_State_HandleInteractions();
    }
}

void PetalPile_State_HandleInteractions(void)
{
    RSDK_THIS(PetalPile);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            if (!player->sidekick) {
                if (abs(player->groundVel) >= 0x60000 || player->velocity.y > 0x60000 || player->velocity.y < -0x70000
                    || player->state == Player_State_Spindash || player->state == Player_State_DropDash) {
                    bool32 isFast = false;
                    if (player->state == Player_State_Spindash || player->state == Player_State_DropDash || abs(player->groundVel) >= 0x60000)
                        isFast = true;

                    self->petalDir = isFast * (2 * (player->direction != FLIP_NONE) - 1);

                    if (player->state == Player_State_Spindash || player->state == Player_State_DropDash)
                        self->petalRadius = 0xF5555;
                    else
                        self->petalRadius = 0xB5555;

                    self->petalVel   = player->groundVel >> 1;
                    self->distance.x = player->position.x - self->position.x;
                    self->distance.y = 0;
                    RSDK.PlaySfx(PetalPile->sfxPetals, false, 255);
                    self->state = PetalPile_State_SetupEmitter;

                    foreach_break;
                }
            }

            if (!self->emitterMode && abs(player->groundVel) < 0x60000 && abs(player->groundVel) > abs(self->maxSpeed.x))
                player->groundVel =
                    player->groundVel - ((abs(player->groundVel) - abs(self->maxSpeed.x)) >> 1) * (((player->groundVel >> 31) & -2) + 1);
        }
    }

    if (self->state != PetalPile_State_SetupEmitter) {
        Hitbox hitbox;
        hitbox.left   = -8;
        hitbox.top    = -8;
        hitbox.right  = 8;
        hitbox.bottom = 8;

        foreach_active(Explosion, explosion)
        {
            if (RSDK.CheckObjectCollisionTouchBox(self, &self->hitbox, explosion, &hitbox)) {
                self->petalDir    = 0;
                self->petalRadius = 0xF5555;
                self->distance.x  = explosion->position.x - self->position.x;
                self->distance.y  = 0;
                RSDK.PlaySfx(PetalPile->sfxPetals, false, 255);
                self->state = PetalPile_State_SetupEmitter;

                foreach_break;
            }
        }
    }
}

void PetalPile_State_SetupEmitter(void)
{
    RSDK_THIS(PetalPile);

    Vector2 pattern[0x100];
    memset(pattern, 0, sizeof(pattern));
    int32 count = PetalPile_GetLeafPattern(pattern);

    int32 offsetX = 0, offsetY = 0;
    switch (self->petalDir) {
        case -1:
            offsetX = self->position.x + ((self->hitbox.right + 16) << 16);
            offsetY = self->position.y + ((self->hitbox.bottom + 32) << 16);
            break;

        case 0:
            offsetX = self->position.x + self->distance.x;
            offsetY = self->position.y + ((self->hitbox.bottom + 32) << 16);
            break;

        case 1:
            offsetX = self->position.x + ((self->hitbox.left - 16) << 16);
            offsetY = self->position.y + ((self->hitbox.bottom + 32) << 16);
            break;
    }

    int32 pos = 0;
    for (int32 i = 0; i < count; ++i) pos = MAX(pos, abs((self->position.x - offsetX) + pattern[i].x));

    for (int32 i = 0; i < count; ++i) {
        int32 spawnX = pattern[i].x + self->position.x;
        int32 spawnY = pattern[i].y + self->position.y;

        int32 angle            = RSDK.ATan2(spawnX - offsetX, spawnY - offsetY);
        EntityPetalPile *petal = CREATE_ENTITY(PetalPile, self, spawnX, spawnY);
        petal->state           = PetalPile_StateLeaf_Setup;
        petal->stateDraw       = PetalPile_Draw_Leaf;

        int32 radius = self->petalRadius >> 1;
        if (self->petalDir) {
            petal->direction = self->petalDir <= 0;
            petal->petalVel  = self->petalVel;
            radius           = (self->petalRadius >> 8) * ((16 * (abs(spawnX - offsetX) / (pos >> 16))) >> 12);
            petal->timer     = (pos - abs(spawnX - offsetX)) >> 18;
        }
        else {
            petal->direction = ZONE_RAND(FLIP_NONE, FLIP_X);
        }

        petal->velStore.x = (radius >> 8) * RSDK.Cos256(angle);
        petal->velStore.y = (radius >> 9) * RSDK.Sin256(angle) - 0x20000;
        petal->drawGroup  = self->tileLayer ? Zone->objectDrawGroup[0] : Zone->objectDrawGroup[1];
    }

    if (self->emitterMode) {
        self->state = PetalPile_State_Emitter;
    }
    else {
        if (!self->noRemoveTiles) {
            int32 left   = (self->position.x >> 16) + self->hitbox.left;
            int32 right  = (self->position.x >> 16) + self->hitbox.right;
            int32 top    = (self->position.y >> 16) + self->hitbox.top;
            int32 bottom = (self->position.y >> 16) + self->hitbox.bottom;

            int32 sizeX = (right >> 4) - (left >> 4);
            int32 sizeY = (bottom >> 4) - (top >> 4);
            for (int32 x = 0; x <= sizeX; ++x) {
                for (int32 y = 0; y <= sizeY; ++y) {
                    RSDK.SetTile(self->layerID, x + (left >> 4), y + (top >> 4), -1);
                }
            }
        }

        destroyEntity(self);
    }
}

void PetalPile_State_Emitter(void)
{
    RSDK_THIS(PetalPile);

    bool32 collided = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox))
            collided = true;
    }

    if (!collided)
        self->state = PetalPile_State_Init;
}

void PetalPile_StateLeaf_Setup(void)
{
    RSDK_THIS(PetalPile);

    self->hitbox.left   = -1;
    self->hitbox.top    = -1;
    self->hitbox.right  = 1;
    self->hitbox.bottom = 1;

    self->active        = ACTIVE_NORMAL;
    self->updateRange.x = 0x10000;
    self->updateRange.y = 0x10000;
    RSDK.SetSpriteAnimation(PetalPile->aniFrames, 0, &self->animator, true, 0);

    self->state = PetalPile_StateLeaf_Delay;
    PetalPile_StateLeaf_Delay();
}

void PetalPile_StateLeaf_Delay(void)
{
    RSDK_THIS(PetalPile);

    if (self->timer <= 0) {
        self->state    = PetalPile_StateLeaf_HandleVelocity;
        self->timer    = 0;
        self->velocity = self->velStore;
    }
    else {
        self->timer--;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void PetalPile_StateLeaf_HandleVelocity(void)
{
    RSDK_THIS(PetalPile);

    if (self->petalVel > 0) {
        self->petalVel -= 4096;
        if (self->petalVel < 0)
            self->petalVel = 0;
    }
    else if (self->petalVel < 0) {
        self->petalVel += 0x1000;
        if (self->petalVel > 0)
            self->petalVel = 0;
    }

    self->position.x += self->petalVel;

    if (self->velocity.x <= 0)
        self->velocity.x += MIN(abs(self->velocity.x), 0x8000);
    else
        self->velocity.x -= MIN(abs(self->velocity.x), 0x8000);
    self->velocity.y += 0x4000;

    if (self->velocity.y > 0)
        self->velocity.y = 0;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (++self->timer > 2) {
        if (ZONE_RAND(0, 10) > 6)
            self->direction = self->direction == FLIP_NONE;

        self->timer = 0;
    }

    if (self->velocity.y >= 0 && !self->velocity.x) {
        self->petalOffset = ZONE_RAND(0, 255);
        self->state       = PetalPile_StateLeaf_Fall;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void PetalPile_StateLeaf_Fall(void)
{
    RSDK_THIS(PetalPile);

    if (self->petalVel > 0) {
        self->petalVel -= 4096;
        if (self->petalVel < 0)
            self->petalVel = 0;
    }
    else if (self->petalVel < 0) {
        self->petalVel += 0x1000;
        if (self->petalVel > 0)
            self->petalVel = 0;
    }

    self->position.x += self->petalVel;

    self->velocity.y += 0x4000;
    if (self->velocity.y > 0x10000)
        self->velocity.y = 0x10000;

    self->velocity.x = RSDK.Sin256(4 * self->petalOffset) << 8;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    ++self->timer;
    if (self->timer > 3) {
        if (ZONE_RAND(0, 10) > 6)
            self->direction = self->direction == FLIP_NONE;

        self->timer = 0;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);

    self->petalOffset++;
}

void PetalPile_Draw_Leaf(void)
{
    RSDK_THIS(PetalPile);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

#if GAME_INCLUDE_EDITOR
void PetalPile_EditorDraw(void)
{
    RSDK_THIS(PetalPile);

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->pileSize.x, self->pileSize.y, 0xFFFF00);
}

void PetalPile_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        PetalPile->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Petal.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        PetalPile->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Petal.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PetalPile, leafPattern);
    RSDK_ENUM_VAR("Pattern 0", PETALPILE_PATTERN_0);
    RSDK_ENUM_VAR("Pattern 1", PETALPILE_PATTERN_1);
    RSDK_ENUM_VAR("Pattern 2", PETALPILE_PATTERN_2);
    RSDK_ENUM_VAR("Pattern 3", PETALPILE_PATTERN_3);
    RSDK_ENUM_VAR("Pattern 4", PETALPILE_PATTERN_4);

    RSDK_ACTIVE_VAR(PetalPile, tileLayer);
    RSDK_ENUM_VAR("FG High", PETALPILE_FGHIGH);
    RSDK_ENUM_VAR("FG Low", PETALPILE_FGLOW);
}
#endif

void PetalPile_Serialize(void)
{
    RSDK_EDITABLE_VAR(PetalPile, VAR_INT8, leafPattern);
    RSDK_EDITABLE_VAR(PetalPile, VAR_INT8, tileLayer);
    RSDK_EDITABLE_VAR(PetalPile, VAR_VECTOR2, pileSize);
    RSDK_EDITABLE_VAR(PetalPile, VAR_VECTOR2, maxSpeed);
    RSDK_EDITABLE_VAR(PetalPile, VAR_BOOL, emitterMode);
}
