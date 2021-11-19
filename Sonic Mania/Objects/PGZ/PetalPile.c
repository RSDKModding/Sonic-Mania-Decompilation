#include "SonicMania.h"

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
    self->drawOrder     = Zone->drawOrderHigh;
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
            self->layerID = RSDK.GetSceneLayerID("FG Low");
        else
            self->layerID = RSDK.GetSceneLayerID("FG High");
    }
    RSDK.SetSpriteAnimation(PetalPile->aniFrames, 0, &self->animator, true, 0);
    self->state = PetalPile_State_Setup;
}

void PetalPile_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1")) {
        PetalPile->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Petal.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("PSZ2")) {
        PetalPile->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Petal.bin", SCOPE_STAGE);
    }

    PetalPile->sfxPetals = RSDK.GetSFX("PSZ/Petals.wav");
}

int32 PetalPile_GetLeafPattern(int32 *patternPtr)
{
    RSDK_THIS(PetalPile);

    int32 count    = PetalPile->patternSize[self->leafPattern];
    int32 *pattern = NULL;
    switch (self->leafPattern) {
        case 0: pattern = PetalPile->pattern1; break;
        case 1: pattern = PetalPile->pattern2; break;
        case 2: pattern = PetalPile->pattern3; break;
        case 3: pattern = PetalPile->pattern4; break;
        case 4: pattern = PetalPile->pattern5; break;
        default: return 0;
    }

    int32 sizeX = maxVal(self->pileSize.x, 0x20000);
    int32 sizeY = maxVal(self->pileSize.y, 0x20000);
    for (int32 i = 0; i < count * 2; i += 2) {
        patternPtr[i + 0] = pattern[i + 0] * (sizeX >> 17);
        patternPtr[i + 1] = pattern[i + 1] * (sizeY >> 17);
    }
    return count;
}

void PetalPile_State_Setup(void)
{
    RSDK_THIS(PetalPile);

    self->hitbox1.left   = -(self->pileSize.x >> 17);
    self->hitbox1.top    = -(self->pileSize.y >> 17);
    self->hitbox1.right  = self->pileSize.x >> 17;
    self->hitbox1.bottom = self->pileSize.y >> 17;

    if (!self->flag) {
        self->state = PetalPile_State_HandleInteractions;
        PetalPile_State_HandleInteractions();
    }
    else {
        self->state = PetalPile_State_Unknown3;
        PetalPile_State_Unknown3();
    }
}

void PetalPile_State_HandleInteractions(void)
{
    RSDK_THIS(PetalPile);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox1)) {
            if (!player->sidekick) {
                if (abs(player->groundVel) >= 0x60000 || player->velocity.y > 0x60000 || player->velocity.y < -0x70000
                    || player->state == Player_State_Spindash || player->state == Player_State_DropDash) {
                    bool32 flag = false;
                    if (player->state == Player_State_Spindash || player->state == Player_State_DropDash || abs(player->groundVel) >= 0x60000)
                        flag = true;
                    self->field_94 = flag * (2 * (player->direction != FLIP_NONE) - 1);

                    if (player->state == Player_State_Spindash || player->state == Player_State_DropDash)
                        self->field_98 = 0xF5555;
                    else
                        self->field_98 = 0xB5555;
                    self->field_8C = player->groundVel >> 1;
                    self->field_84 = player->position.x - self->position.x;
                    self->field_88 = 0;
                    RSDK.PlaySfx(PetalPile->sfxPetals, false, 255);
                    self->state = PetalPile_State_Unknown3;
                    foreach_break;
                }
            }
            if (!self->emitterMode && abs(player->groundVel) < 0x60000 && abs(player->groundVel) > abs(self->maxSpeed.x))
                player->groundVel =
                    player->groundVel - ((abs(player->groundVel) - abs(self->maxSpeed.x)) >> 1) * (((player->groundVel >> 31) & -2) + 1);
        }
    }

    if (self->state != PetalPile_State_Unknown3) {
        Hitbox hitbox;
        hitbox.left   = -8;
        hitbox.top    = -8;
        hitbox.right  = 8;
        hitbox.bottom = 8;
        foreach_active(Explosion, explosion)
        {
            if (RSDK.CheckObjectCollisionTouchBox(self, &self->hitbox1, explosion, &hitbox)) {
                self->field_94 = 0;
                self->field_98 = 0xF5555;
                self->field_84 = explosion->position.x - self->position.x;
                self->field_88 = 0;
                RSDK.PlaySfx(PetalPile->sfxPetals, false, 255);
                self->state = PetalPile_State_Unknown3;
                foreach_break;
            }
        }
    }
}

void PetalPile_State_Unknown3(void)
{
    RSDK_THIS(PetalPile);

    Vector2 buffer[0x100];
    memset(buffer, 0, sizeof(buffer));
    int32 count = PetalPile_GetLeafPattern((int32 *)buffer);

    int32 offsetX = 0, offsetY = 0;
    switch (self->field_94) {
        case 0:
            offsetX = self->position.x + self->field_84;
            offsetY = self->position.y + ((self->hitbox1.bottom + 32) << 16);
            break;
        case -1:
            offsetX = self->position.x + ((self->hitbox1.right + 16) << 16);
            offsetY = self->position.y + ((self->hitbox1.bottom + 32) << 16);
            break;
        case 1:
            offsetX = self->position.x + ((self->hitbox1.left - 16) << 16);
            offsetY = self->position.y + ((self->hitbox1.bottom + 32) << 16);
            break;
    }

    int32 pos = 0;
    for (int32 i = 0; i < count; ++i) {
        int32 val = abs((self->position.x - offsetX) + buffer[i].x);
        if (pos <= val)
            pos = val;
    }

    for (int32 i = 0; i < count; ++i) {
        int32 spawnX = buffer[i].x + self->position.x;
        int32 spawnY = buffer[i].y + self->position.y;

        int32 angle              = RSDK.ATan2(spawnX - offsetX, spawnY - offsetY);
        EntityPetalPile *petal = CREATE_ENTITY(PetalPile, self, spawnX, spawnY);
        petal->state           = PetalPile_SetupLeaf;
        petal->stateDraw       = PetalPile_StateDraw_Leaf;

        int32 val = self->field_98 >> 1;
        if (self->field_94) {
            petal->direction = self->field_94 <= 0;
            petal->field_8C  = self->field_8C;
            val              = (self->field_98 >> 8) * ((16 * (abs(spawnX - offsetX) / (pos >> 16))) >> 12);
            petal->timer     = (pos - abs(spawnX - offsetX)) >> 18;
        }
        else {
#if RETRO_USE_PLUS
            petal->direction = RSDK.RandSeeded(0, 1, &Zone->randSeed);
#else
            petal->direction = RSDK.Rand(0, 1);
#endif
        }

        petal->velStore.x = (val >> 8) * RSDK.Cos256(angle);
        petal->velStore.y = (val >> 9) * RSDK.Sin256(angle) - 0x20000;
        if (self->tileLayer)
            petal->drawOrder = Zone->drawOrderLow;
        else
            petal->drawOrder = Zone->drawOrderHigh;
    }

    if (self->emitterMode) {
        self->state = PetalPile_State_Unknown4;
    }
    else {
        if (!self->flag) {
            int32 left   = (self->position.x >> 16) + self->hitbox1.left;
            int32 right  = (self->position.x >> 16) + self->hitbox1.right;
            int32 top    = (self->position.y >> 16) + self->hitbox1.top;
            int32 bottom = (self->position.y >> 16) + self->hitbox1.bottom;

            int32 sizeX = (right >> 4) - (left >> 4);
            int32 sizeY = (bottom >> 4) - (top >> 4);
            for (int32 x = 0; x <= sizeX; ++x) {
                for (int32 y = 0; y <= sizeY; ++y) {
                    RSDK.SetTileInfo(self->layerID, x + (left >> 4), y + (top >> 4), 0xFFFF);
                }
            }
        }
        destroyEntity(self);
    }
}

void PetalPile_State_Unknown4(void)
{
    RSDK_THIS(PetalPile);

    bool32 flag = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox1)) {
            flag = true;
        }
    }

    if (!flag) {
        self->state = PetalPile_State_Setup;
    }
}

void PetalPile_SetupLeaf(void)
{
    RSDK_THIS(PetalPile);
    self->hitbox1.left   = -1;
    self->hitbox1.top    = -1;
    self->hitbox1.right  = 1;
    self->hitbox1.bottom = 1;
    self->active         = ACTIVE_NORMAL;
    self->updateRange.x  = 0x10000;
    self->updateRange.y  = 0x10000;
    RSDK.SetSpriteAnimation(PetalPile->aniFrames, 0, &self->animator, true, 0);
    self->state = PetalPile_State_Unknown5;
    PetalPile_State_Unknown5();
}

void PetalPile_State_Unknown5(void)
{
    RSDK_THIS(PetalPile);

    if (self->timer <= 0) {
        self->state    = PetalPile_State_Unknown6;
        self->timer    = 0;
        self->velocity = self->velStore;
    }
    else {
        self->timer--;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void PetalPile_State_Unknown6(void)
{
    RSDK_THIS(PetalPile);

    if (self->field_8C > 0) {
        self->field_8C -= 4096;
        if (self->field_8C < 0)
            self->field_8C = 0;
    }
    else if (self->field_8C < 0) {
        self->field_8C += 0x1000;
        if (self->field_8C > 0)
            self->field_8C = 0;
    }
    self->position.x += self->field_8C;

    self->velocity.y += 0x4000;
    if (self->velocity.x <= 0)
        self->velocity.x += minVal(abs(self->velocity.x), 0x8000);
    else
        self->velocity.x -= minVal(abs(self->velocity.x), 0x8000);
    if (self->velocity.y > 0)
        self->velocity.y = 0;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (++self->timer > 2) {
#if RETRO_USE_PLUS
        if (RSDK.RandSeeded(0, 10, &Zone->randSeed) > 6)
#else
        if (RSDK.Rand(0, 10) > 6)
#endif
            self->direction = self->direction == FLIP_NONE;
        self->timer = 0;
    }
    if (self->velocity.y >= 0 && !self->velocity.x) {
#if RETRO_USE_PLUS
        self->field_9C = RSDK.RandSeeded(0, 255, &Zone->randSeed);
#else
        self->field_9C = RSDK.Rand(0, 255);
#endif
        self->state = PetalPile_State_Unknown7;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void PetalPile_State_Unknown7(void)
{
    RSDK_THIS(PetalPile);

    if (self->field_8C > 0) {
        self->field_8C -= 4096;
        if (self->field_8C < 0)
            self->field_8C = 0;
    }
    else if (self->field_8C < 0) {
        self->field_8C += 0x1000;
        if (self->field_8C > 0)
            self->field_8C = 0;
    }
    self->position.x += self->field_8C;
    self->velocity.y += 0x4000;
    if (self->velocity.y > 0x10000)
        self->velocity.y = 0x10000;
    int32 val = RSDK.Sin256(4 * self->field_9C) << 8;
    ++self->timer;
    self->position.x += val;
    self->velocity.x = val;
    self->position.y += self->velocity.y;
    if (self->timer > 3) {
#if RETRO_USE_PLUS
        if (RSDK.RandSeeded(0, 10, &Zone->randSeed) > 6)
#else
        if (RSDK.Rand(0, 10) > 6)
#endif
            self->direction = self->direction == FLIP_NONE;
        self->timer = 0;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);

    self->field_9C++;
}

void PetalPile_StateDraw_Leaf(void)
{
    RSDK_THIS(PetalPile);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void PetalPile_EditorDraw(void) {}

void PetalPile_EditorLoad(void) {}
#endif

void PetalPile_Serialize(void)
{
    RSDK_EDITABLE_VAR(PetalPile, VAR_INT8, leafPattern);
    RSDK_EDITABLE_VAR(PetalPile, VAR_INT8, tileLayer);
    RSDK_EDITABLE_VAR(PetalPile, VAR_VECTOR2, pileSize);
    RSDK_EDITABLE_VAR(PetalPile, VAR_VECTOR2, maxSpeed);
    RSDK_EDITABLE_VAR(PetalPile, VAR_BOOL, emitterMode);
}
