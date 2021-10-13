#include "SonicMania.h"

ObjectChemBubble *ChemBubble;

void ChemBubble_Update(void)
{
    RSDK_THIS(ChemBubble);
    StateMachine_Run(entity->state);
}

void ChemBubble_LateUpdate(void) {}

void ChemBubble_StaticUpdate(void) {}

void ChemBubble_Draw(void)
{
    RSDK_THIS(ChemBubble);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void ChemBubble_Create(void *data)
{
    RSDK_THIS(ChemBubble);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible    = true;
        entity->active     = ACTIVE_NORMAL;
        entity->drawOrder  = Zone->drawOrderHigh - 2;
        entity->startPos.x = entity->position.x;
        entity->startPos.y = entity->position.y;
#if RETRO_USE_PLUS
        entity->velocity.y = RSDK.Random(-0x20000, 0, &Zone->randKey);
        entity->shiftY     = RSDK.Random(12, 16, &Zone->randKey);
        entity->shiftY2    = RSDK.Random(9, 10, &Zone->randKey);
#else
        entity->velocity.y = RSDK.Rand(-0x20000, 0);
        entity->shiftY     = RSDK.Rand(12, 16);
        entity->shiftY2    = RSDK.Rand(9, 10);
#endif
        RSDK.SetSpriteAnimation(ChemBubble->aniFrames, RSDK.Rand(1, 3), &entity->animator, true, 0);
        entity->drawFX = FX_SCALE;
        entity->state  = ChemBubble_Unknown1;
    }
}

void ChemBubble_StageLoad(void) { ChemBubble->aniFrames = RSDK.LoadSpriteAnimation("CPZ/ChemPool.bin", SCOPE_STAGE); }

void ChemBubble_Unknown1(void)
{
    RSDK_THIS(ChemBubble);
    entity->scale.x += 0x20;
    entity->scale.y = entity->scale.x;
    if (entity->scale.x == 0x200) {
        entity->drawFX = FX_NONE;
        entity->state  = ChemBubble_Unknown2;
    }
}

void ChemBubble_Unknown2(void)
{
    RSDK_THIS(ChemBubble);
    EntityChemicalPool *parent = (EntityChemicalPool *)entity->parent;

    entity->velocity.y -= 0x2000;
    entity->position.y += entity->velocity.y;
    int32 deform         = (RSDK.Sin256(entity->position.y >> entity->shiftY) << entity->shiftY2) + entity->startPos.x;
    entity->position.x = deform;
    int32 val            = ChemicalPool->table1[deform >> 20];
    int32 pos            = parent->offsetY + (((deform >> 12) & 0xFF) * (ChemicalPool->table1[(deform + 0x100000) >> 20] - val) >> 8) + val + 0x20000;
    if (entity->position.y <= pos) {
        entity->position.y = pos;
        if (entity->animator.animationID == 2) {
            RSDK.SetSpriteAnimation(ChemBubble->aniFrames, 3, &entity->animator, true, 0);
            entity->timer = RSDK.Rand(8, 32);
        }
        else {
            RSDK.SetSpriteAnimation(ChemBubble->aniFrames, 4, &entity->animator, true, 5);
        }
        entity->state = ChemBubble_Unknown3;
    }
}

void ChemBubble_Unknown3(void)
{
    RSDK_THIS(ChemBubble);
    EntityChemicalPool *parent = (EntityChemicalPool *)entity->parent;

    RSDK.ProcessAnimation(&entity->animator);
    int32 val = ChemicalPool->table1[entity->position.x >> 20];
    int32 pos = parent->offsetY + (((entity->position.x >> 12) & 0xFF) * (ChemicalPool->table1[(entity->position.x + 0x100000) >> 20] - val) >> 8) + val
              + 0x20000;

    entity->position.y = pos;
    if (entity->animator.animationID == 4) {
        if (entity->animator.frameID == entity->animator.frameCount - 1)
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
    else if (--entity->timer <= 0) {
        RSDK.SetSpriteAnimation(ChemBubble->aniFrames, 4, &entity->animator, true, 5);
    }
}

void ChemBubble_EditorDraw(void) {}

void ChemBubble_EditorLoad(void) {}

void ChemBubble_Serialize(void) {}
