#include "SonicMania.h"

ObjectPetalPile *PetalPile;

void PetalPile_Update(void)
{
    RSDK_THIS(PetalPile);
    StateMachine_Run(entity->state);
    if (!entity->animator.animationID)
        RSDK.ProcessAnimation(&entity->animator);
}

void PetalPile_LateUpdate(void) {}

void PetalPile_StaticUpdate(void) {}

void PetalPile_Draw(void)
{
    RSDK_THIS(PetalPile);
    StateMachine_Run(entity->stateDraw);
}

void PetalPile_Create(void *data)
{
    RSDK_THIS(PetalPile);
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderHigh;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x100000;
    entity->updateRange.y = 0x100000;
    if (RSDK_sceneInfo->inEditor) {
        if (!entity->maxSpeed.x)
            entity->maxSpeed.x = 0x18000;
        if (!entity->pileSize.x && !entity->pileSize.y) {
            entity->pileSize.x = 0x300000;
            entity->pileSize.y = 0x80000;
        }
    }
    else {
        if (entity->tileLayer)
            entity->layerID = RSDK.GetSceneLayerID("FG Low");
        else
            entity->layerID = RSDK.GetSceneLayerID("FG High");
    }
    RSDK.SetSpriteAnimation(PetalPile->aniFrames, 0, &entity->animator, true, 0);
    entity->state = PetalPile_State_Setup;
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

int PetalPile_GetLeafPattern(int *patternPtr)
{
    RSDK_THIS(PetalPile);

    int count    = PetalPile->patternSize[entity->leafPattern];
    int *pattern = NULL;
    switch (entity->leafPattern) {
        case 0: pattern = PetalPile->pattern1; break;
        case 1: pattern = PetalPile->pattern2; break;
        case 2: pattern = PetalPile->pattern3; break;
        case 3: pattern = PetalPile->pattern4; break;
        case 4: pattern = PetalPile->pattern5; break;
        default: return 0;
    }

    int sizeX = maxVal(entity->pileSize.x, 0x20000);
    int sizeY = maxVal(entity->pileSize.y, 0x20000);
    for (int i = 0; i < count * 2; i += 2) {
        patternPtr[i + 0] = pattern[i + 0] * (sizeX >> 17);
        patternPtr[i + 1] = pattern[i + 1] * (sizeY >> 17);
    }
    return count;
}

void PetalPile_State_Setup(void)
{
    RSDK_THIS(PetalPile);

    entity->hitbox1.left   = -(entity->pileSize.x >> 17);
    entity->hitbox1.top    = -(entity->pileSize.y >> 17);
    entity->hitbox1.right  = entity->pileSize.x >> 17;
    entity->hitbox1.bottom = entity->pileSize.y >> 17;

    if (!entity->flag) {
        entity->state = PetalPile_State_HandleInteractions;
        PetalPile_State_HandleInteractions();
    }
    else {
        entity->state = PetalPile_State_Unknown3;
        PetalPile_State_Unknown3();
    }
}

void PetalPile_State_HandleInteractions(void)
{
    RSDK_THIS(PetalPile);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1)) {
            if (!player->sidekick) {
                if (abs(player->groundVel) >= 0x60000 || player->velocity.y > 0x60000 || player->velocity.y < -0x70000
                    || player->state == Player_State_Spindash || player->state == Player_State_DropDash) {
                    bool32 flag = false;
                    if (player->state == Player_State_Spindash || player->state == Player_State_DropDash || abs(player->groundVel) >= 0x60000)
                        flag = true;
                    entity->field_94 = flag * (2 * (player->direction != FLIP_NONE) - 1);

                    if (player->state == Player_State_Spindash || player->state == Player_State_DropDash)
                        entity->field_98 = 0xF5555;
                    else
                        entity->field_98 = 0xB5555;
                    entity->field_8C = player->groundVel >> 1;
                    entity->field_84 = player->position.x - entity->position.x;
                    entity->field_88 = 0;
                    RSDK.PlaySFX(PetalPile->sfxPetals, false, 255);
                    entity->state = PetalPile_State_Unknown3;
                    foreach_break;
                }
            }
            if (!entity->emitterMode && abs(player->groundVel) < 0x60000 && abs(player->groundVel) > abs(entity->maxSpeed.x))
                player->groundVel =
                    player->groundVel - ((abs(player->groundVel) - abs(entity->maxSpeed.x)) >> 1) * (((player->groundVel >> 31) & -2) + 1);
        }
    }

    if (entity->state != PetalPile_State_Unknown3) {
        Hitbox hitbox;
        hitbox.left   = -8;
        hitbox.left   = -8;
        hitbox.right  = 8;
        hitbox.bottom = 8;
        foreach_active(Explosion, explosion)
        {
            if (RSDK.CheckObjectCollisionTouchBox(entity, &entity->hitbox1, explosion, &hitbox)) {
                entity->field_94 = 0;
                entity->field_98 = 0xF5555;
                entity->field_84 = explosion->position.x - entity->position.x;
                entity->field_88 = 0;
                RSDK.PlaySFX(PetalPile->sfxPetals, false, 255);
                entity->state = PetalPile_State_Unknown3;
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
    int count = PetalPile_GetLeafPattern((int *)buffer);

    int offsetX = 0, offsetY = 0;
    switch (entity->field_94) {
        case 0:
            offsetX = entity->position.x + entity->field_84;
            offsetY = entity->position.y + ((entity->hitbox1.bottom + 32) << 16);
            break;
        case -1:
            offsetX = entity->position.x + ((entity->hitbox1.right + 16) << 16);
            offsetY = entity->position.y + ((entity->hitbox1.bottom + 32) << 16);
            break;
        case 1:
            offsetX = entity->position.x + ((entity->hitbox1.left - 16) << 16);
            offsetY = entity->position.y + ((entity->hitbox1.bottom + 32) << 16);
            break;
    }

    int pos = 0;
    for (int i = 0; i < count; ++i) {
        int val = abs((entity->position.x - offsetX) + buffer[i].x);
        if (pos <= val)
            pos = val;
    }

    for (int i = 0; i < count; ++i) {
        int spawnX = buffer[i].x + entity->position.x;
        int spawnY = buffer[i].y + entity->position.y;

        int angle              = RSDK.ATan2(spawnX - offsetX, spawnY - offsetY);
        EntityPetalPile *petal = CREATE_ENTITY(PetalPile, entity, spawnX, spawnY);
        petal->state           = PetalPile_SetupLeaf;
        petal->stateDraw       = PetalPile_StateDraw_Leaf;

        int val = entity->field_98 >> 1;
        if (entity->field_94) {
            petal->direction = entity->field_94 <= 0;
            petal->field_8C  = entity->field_8C;
            val              = (entity->field_98 >> 8) * ((16 * (abs(spawnX - offsetX) / (pos >> 16))) >> 12);
            petal->timer     = (pos - abs(spawnX - offsetX)) >> 18;
        }
        else {
#if RETRO_USE_PLUS
            petal->direction = RSDK.Random(0, 1, &Zone->randKey);
#else
            petal->direction = RSDK.Rand(0, 1);
#endif
        }

        petal->velStore.x = (val >> 8) * RSDK.Cos256(angle);
        petal->velStore.y = (val >> 9) * RSDK.Sin256(angle) - 0x20000;
        if (entity->tileLayer)
            petal->drawOrder = Zone->drawOrderLow;
        else
            petal->drawOrder = Zone->drawOrderHigh;
    }

    if (entity->emitterMode) {
        entity->state = PetalPile_State_Unknown4;
    }
    else {
        if (!entity->flag) {
            int left   = (entity->position.x >> 16) + entity->hitbox1.left;
            int right  = (entity->position.x >> 16) + entity->hitbox1.right;
            int top    = (entity->position.y >> 16) + entity->hitbox1.top;
            int bottom = (entity->position.y >> 16) + entity->hitbox1.bottom;

            int sizeX = (right >> 4) - (left >> 4);
            int sizeY = (bottom >> 4) - (top >> 4);
            for (int x = 0; x <= sizeX; ++x) {
                for (int y = 0; y <= sizeY; ++y) {
                    RSDK.SetTileInfo(entity->layerID, x + (left >> 4), y + (top >> 4), 0xFFFF);
                }
            }
        }
        destroyEntity(entity);
    }
}

void PetalPile_State_Unknown4(void)
{
    RSDK_THIS(PetalPile);

    bool32 flag = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1)) {
            flag = true;
        }
    }

    if (!flag) {
        entity->state = PetalPile_State_Setup;
    }
}

void PetalPile_SetupLeaf(void)
{
    RSDK_THIS(PetalPile);
    entity->hitbox1.left   = -1;
    entity->hitbox1.top    = -1;
    entity->hitbox1.right  = 1;
    entity->hitbox1.bottom = 1;
    entity->active         = ACTIVE_NORMAL;
    entity->updateRange.x  = 0x10000;
    entity->updateRange.y  = 0x10000;
    RSDK.SetSpriteAnimation(PetalPile->aniFrames, 0, &entity->animator, true, 0);
    entity->state = PetalPile_State_Unknown5;
    PetalPile_State_Unknown5();
}

void PetalPile_State_Unknown5(void)
{
    RSDK_THIS(PetalPile);

    if (entity->timer <= 0) {
        entity->state    = PetalPile_State_Unknown6;
        entity->timer    = 0;
        entity->velocity = entity->velStore;
    }
    else {
        entity->timer--;
    }

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void PetalPile_State_Unknown6(void)
{
    RSDK_THIS(PetalPile);

    if (entity->field_8C > 0) {
        entity->field_8C -= 4096;
        if (entity->field_8C < 0)
            entity->field_8C = 0;
    }
    else if (entity->field_8C < 0) {
        entity->field_8C += 0x1000;
        if (entity->field_8C > 0)
            entity->field_8C = 0;
    }
    entity->position.x += entity->field_8C;

    entity->velocity.y += 0x4000;
    if (entity->velocity.x <= 0)
        entity->velocity.x += minVal(abs(entity->velocity.x), 0x8000);
    else
        entity->velocity.x -= minVal(abs(entity->velocity.x), 0x8000);
    if (entity->velocity.y > 0)
        entity->velocity.y = 0;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (++entity->timer > 2) {
#if RETRO_USE_PLUS
        if (RSDK.Random(0, 10, &Zone->randKey) > 6)
#else
        if (RSDK.Rand(0, 10) > 6)
#endif
            entity->direction = entity->direction == FLIP_NONE;
        entity->timer = 0;
    }
    if (entity->velocity.y >= 0 && !entity->velocity.x) {
#if RETRO_USE_PLUS
        entity->field_9C = RSDK.Random(0, 255, &Zone->randKey);
#else
        entity->field_9C = RSDK.Rand(0, 255);
#endif
        entity->state = PetalPile_State_Unknown7;
    }

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void PetalPile_State_Unknown7(void)
{
    RSDK_THIS(PetalPile);

    if (entity->field_8C > 0) {
        entity->field_8C -= 4096;
        if (entity->field_8C < 0)
            entity->field_8C = 0;
    }
    else if (entity->field_8C < 0) {
        entity->field_8C += 0x1000;
        if (entity->field_8C > 0)
            entity->field_8C = 0;
    }
    entity->position.x += entity->field_8C;
    entity->velocity.y += 0x4000;
    if (entity->velocity.y > 0x10000)
        entity->velocity.y = 0x10000;
    int val = RSDK.Sin256(4 * entity->field_9C) << 8;
    ++entity->timer;
    entity->position.x += val;
    entity->velocity.x = val;
    entity->position.y += entity->velocity.y;
    if (entity->timer > 3) {
#if RETRO_USE_PLUS
        if (RSDK.Random(0, 10, &Zone->randKey) > 6)
#else
        if (RSDK.Rand(0, 10) > 6)
#endif
            entity->direction = entity->direction == FLIP_NONE;
        entity->timer = 0;
    }

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);

    entity->field_9C++;
}

void PetalPile_StateDraw_Leaf(void)
{
    RSDK_THIS(PetalPile);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PetalPile_EditorDraw(void) {}

void PetalPile_EditorLoad(void) {}

void PetalPile_Serialize(void)
{
    RSDK_EDITABLE_VAR(PetalPile, VAR_INT8, leafPattern);
    RSDK_EDITABLE_VAR(PetalPile, VAR_INT8, tileLayer);
    RSDK_EDITABLE_VAR(PetalPile, VAR_VECTOR2, pileSize);
    RSDK_EDITABLE_VAR(PetalPile, VAR_VECTOR2, maxSpeed);
    RSDK_EDITABLE_VAR(PetalPile, VAR_BOOL, emitterMode);
}
