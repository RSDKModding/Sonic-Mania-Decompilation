#include "../SonicMania.h"

ObjectSeltzerWater *SeltzerWater;

void SeltzerWater_Update(void)
{
    RSDK_THIS(SeltzerWater);
    StateMachine_Run(entity->state);

    if (entity->scale.x < 512) {
        entity->scale.x += 16;
        entity->scale.y = entity->scale.x;
        if (entity->scale.x == 512)
            entity->drawFX = FX_NONE;
    }
}

void SeltzerWater_LateUpdate(void) {}

void SeltzerWater_StaticUpdate(void) {}

void SeltzerWater_Draw(void)
{
    RSDK_THIS(SeltzerWater);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void SeltzerWater_Create(void *data)
{
    RSDK_THIS(SeltzerWater);
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SeltzerWater->aniFrames, 4, &entity->animator, true, RSDK.Rand(0, 8));
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x100000;
        entity->updateRange.y = 0x100000;
        entity->visible       = 1;
        entity->drawFX        = FX_SCALE;
        entity->scale.x       = 0x80;
        entity->scale.y       = 0x80;
        entity->field_5C      = RSDK.Rand(0, 256);
        entity->state         = SeltzerWater_Unknown1;
    }
}

void SeltzerWater_StageLoad(void) { SeltzerWater->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Seltzer.bin", SCOPE_STAGE); }

void SeltzerWater_Unknown1(void)
{
    RSDK_THIS(SeltzerWater);
    EntityPlatformNode *node = RSDK_GET_ENTITY(entity->nodeSlot, PlatformNode);
    RSDK.ProcessAnimation(&entity->animator);
    if (node->objectID == PlatformNode->objectID) {
        int x = (entity->position.x - node->position.x) >> 16;
        int y = (entity->position.y - node->position.y) >> 16;

        entity->angle      = RSDK.ATan2(x, y);
        entity->velocity.x = -(entity->field_64 * RSDK.Cos256(entity->angle));
        entity->velocity.y = 16 * RSDK.Sin256(entity->field_5C) - entity->field_64 * RSDK.Sin256(entity->angle);
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        if (x * x + y * y < 80)
            ++entity->nodeSlot;
        entity->field_5C++;
    }
    else {
        entity->state           = SeltzerWater_Unknown2;
        entity->gravityStrength = 0x3800;
    }
}

void SeltzerWater_Unknown2(void)
{
    RSDK_THIS(SeltzerWater);
    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += entity->gravityStrength;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x60000, true)) {
        RSDK.SetSpriteAnimation(SeltzerWater->aniFrames, 5, &entity->animator, true, 0);
        entity->state = SeltzerWater_Unknown3;
    }
    else {
        if (!RSDK.CheckOnScreen(entity, NULL))
            destroyEntity(entity);
    }
}

void SeltzerWater_Unknown3(void)
{
    RSDK_THIS(SeltzerWater);
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1)
        destroyEntity(entity);
}

void SeltzerWater_EditorDraw(void) {}

void SeltzerWater_EditorLoad(void) {}

void SeltzerWater_Serialize(void) {}
