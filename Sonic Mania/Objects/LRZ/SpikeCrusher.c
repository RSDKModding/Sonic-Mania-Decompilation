#include "SonicMania.h"

ObjectSpikeCrusher *SpikeCrusher;

void SpikeCrusher_Update(void) { Platform_Update(); }

void SpikeCrusher_LateUpdate(void) {}

void SpikeCrusher_StaticUpdate(void) {}

void SpikeCrusher_Draw(void)
{
    RSDK_THIS(SpikeCrusher);

    RSDK.DrawSprite(&entity->animator2, &entity->drawPos, false);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void SpikeCrusher_Create(void *data)
{
    RSDK_THIS(SpikeCrusher);

    entity->collision = PLATFORM_C_6;
    Platform_Create(NULL);
    if (!RSDK_sceneInfo->inEditor) {
        entity->drawOrder = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &entity->animator, true, 4);
        RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &entity->animator2, true, 5);

        while (!RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x7FFF0000, true)) {
            entity->position.y += 0x10000;
        }

        entity->updateRange.x = 0x800000;
        entity->updateRange.y = entity->position.y - entity->centerPos.y + 0x200000;
        if (entity->startDir == 1)
            entity->drawPos.y = entity->position.y;
        entity->position.y = entity->centerPos.y;
        entity->state      = SpikeCrusher_State_Unknown1;
    }
}

void SpikeCrusher_StageLoad(void)
{
    SpikeCrusher->sfxHuff   = RSDK.GetSFX("Stage/Huff.wav");
    SpikeCrusher->sfxImpact = RSDK.GetSFX("Stage/Impact4.wav");
}

void SpikeCrusher_CheckOnScreen(void)
{
    RSDK_THIS(SpikeCrusher);

    if (!RSDK.CheckPosOnScreen(&entity->drawPos, &entity->updateRange) && !RSDK.CheckPosOnScreen(&entity->centerPos, &entity->updateRange)) {
        entity->drawPos.x     = entity->centerPos.x;
        entity->drawPos.y     = entity->centerPos.y;
        entity->position.x    = entity->centerPos.x;
        entity->position.y    = entity->centerPos.y;
        entity->velocity.y    = 0;
        entity->collapseDelay = 0;
        entity->active        = ACTIVE_BOUNDS;
        SpikeCrusher_Create(NULL);
    }
}

void SpikeCrusher_State_Unknown1(void)
{
    RSDK_THIS(SpikeCrusher);

    entity->active = ACTIVE_NORMAL;
    if (entity->startDir == 0)
        entity->state = SpikeCrusher_State_Unknown2;
    else
        entity->state = SpikeCrusher_State_Unknown6;
}

void SpikeCrusher_State_Unknown2(void)
{
    RSDK_THIS(SpikeCrusher);

    entity->drawPos.y += entity->velocity.y;

    int storeX = entity->position.x;
    int storeY = entity->position.y;
    entity->velocity.y += 0x3800;
    if (entity->collapseDelay < 8 && (entity->collapseDelay & 1))
        entity->animator.frameID = entity->collapseDelay >> 1;
    else
        entity->animator.frameID = 4;
    entity->collapseDelay++;
    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x200000, true)) {
        if (entity->activeScreens)
            RSDK.PlaySfx(SpikeCrusher->sfxImpact, false, 255);
        entity->collapseDelay = 0;
        entity->velocity.y    = 0;
        entity->state         = SpikeCrusher_State_Unknown3;
    }
    entity->drawPos.x  = entity->position.x;
    entity->drawPos.y  = entity->position.y;
    entity->position.x = storeX;
    entity->position.y = storeY;
    SpikeCrusher_CheckOnScreen();
}

void SpikeCrusher_State_Unknown3(void)
{
    RSDK_THIS(SpikeCrusher);

    entity->collapseDelay += 2;
    entity->drawPos.y += 0x20000;
    entity->animator2.frameID = entity->collapseDelay + 5;
    if (entity->collapseDelay >= 6) {
        entity->collapseDelay = 15;
        entity->drawPos.y     = entity->drawPos.y + 0x8000;
        entity->state         = SpikeCrusher_State_Unknown4;
    }
    SpikeCrusher_CheckOnScreen();
}

void SpikeCrusher_State_Unknown4(void)
{
    RSDK_THIS(SpikeCrusher);

    --entity->collapseDelay;
    entity->drawPos.y -= 0x8000;

    int frame = (entity->collapseDelay >> 1) + 4;
    if (frame < 5)
        frame = 5;
    entity->animator2.frameID = frame;
    if (entity->collapseDelay <= 0)
        entity->state = SpikeCrusher_State_Unknown5;
    SpikeCrusher_CheckOnScreen();
}

void SpikeCrusher_State_Unknown5(void)
{
    RSDK_THIS(SpikeCrusher);

    if (entity->collapseDelay & 1)
        entity->animator.frameID = 3 - (entity->collapseDelay >> 1);
    else
        entity->animator.frameID = 4;
    entity->collapseDelay++;

    if (entity->collapseDelay >= 8) {
        entity->collapseDelay = 0;
        entity->state         = SpikeCrusher_State_Unknown6;
    }
    SpikeCrusher_CheckOnScreen();
}

void SpikeCrusher_State_Unknown6(void)
{
    RSDK_THIS(SpikeCrusher);

    entity->drawPos.y -= 0x10000;
    entity->animator.frameID = 4 * (!(entity->collapseDelay & 1));
    if (entity->activeScreens && !(entity->collapseDelay & 0x1F))
        RSDK.PlaySfx(SpikeCrusher->sfxHuff, false, 255);
    ++entity->collapseDelay;
    if (entity->drawPos.y <= entity->centerPos.y) {
        entity->collapseDelay = 0;
        entity->drawPos.y     = entity->centerPos.y;
        entity->state         = SpikeCrusher_State_Unknown2;
    }
    entity->velocity.y = -0x10000;
    SpikeCrusher_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void SpikeCrusher_EditorDraw(void)
{
    RSDK_THIS(SpikeCrusher);
    entity->drawPos = entity->position;
    RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &entity->animator, true, 4);
    RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &entity->animator2, true, 5);

    SpikeCrusher_Draw();
}

void SpikeCrusher_EditorLoad(void) {}
#endif

void SpikeCrusher_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpikeCrusher, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(SpikeCrusher, VAR_ENUM, startDir);
}
