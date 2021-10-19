#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectTitleEggman *TitleEggman;

void TitleEggman_Update(void)
{
    RSDK_THIS(TitleEggman);
    StateMachine_Run(entity->state);
}

void TitleEggman_LateUpdate(void) {}

void TitleEggman_StaticUpdate(void) {}

void TitleEggman_Draw(void)
{
    RSDK_THIS(TitleEggman);
    if (entity->state == TitleEggman_Unknown1) {
        RSDK.DrawSprite(&entity->animator1, 0, 0);
    }
    else {
        Vector2 drawPos;
        entity->drawFX = FX_NONE;
        int32 angle      = RSDK.Sin256(2 * entity->timer) >> 1;

        for (int32 i = 1; i < 5; ++i) {
            drawPos.x = (i << 8) * RSDK.Sin1024(angle) + entity->position.x;
            drawPos.y = entity->position.y + 0x80000 + (i << 8) * RSDK.Cos1024(angle);
            RSDK.DrawSprite(&entity->data3, &drawPos, false);
        }
        entity->drawFX   = FX_ROTATE;
        drawPos.x        = 0x500 * RSDK.Sin1024(angle) + entity->position.x;
        drawPos.y        = 0x500 * RSDK.Cos1024(angle) + entity->position.y + 0x80000;
        entity->rotation = -(angle >> 2);
        RSDK.DrawSprite(&entity->data4, &drawPos, false);

        entity->drawFX = FX_FLIP;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
}

void TitleEggman_Create(void *data)
{
    RSDK_THIS(TitleEggman);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible   = true;
        entity->drawOrder = 3;
        entity->active    = ACTIVE_NORMAL;
        if (data) {
            RSDK.SetSpriteAnimation(TitleEggman->spriteIndex, 5, &entity->animator1, true, 0);
            entity->state = TitleEggman_Unknown1;
        }
        else {
            RSDK.SetSpriteAnimation(TitleEggman->spriteIndex, 2, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(TitleEggman->spriteIndex, 3, &entity->data3, true, 0);
            RSDK.SetSpriteAnimation(TitleEggman->spriteIndex, 4, &entity->data4, true, 0);
            entity->startPos.x = entity->position.x;
            entity->startPos.y = entity->position.y;
            entity->velocity.x = 0x10000;
            entity->state      = TitleEggman_Unknown2;
        }
    }
}

void TitleEggman_StageLoad(void)
{
    if (API.CheckDLC(DLC_PLUS))
        TitleEggman->spriteIndex = RSDK.LoadSpriteAnimation("Title/PlusLogo.bin", SCOPE_STAGE);
}

void TitleEggman_Unknown1(void)
{
    RSDK_THIS(TitleEggman);
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        RSDK.ResetEntityPtr(entity, 0, 0);
}
void TitleEggman_Unknown2(void)
{
    RSDK_THIS(TitleEggman);
    if (++entity->timer == 120) {
        entity->timer = 0;
        entity->state = TitleEggman_Unknown3;
    }
}
void TitleEggman_Unknown3(void)
{
    RSDK_THIS(TitleEggman);
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;
    entity->startPos.y -= 0x6000;
    entity->position.y = entity->startPos.y + (RSDK.Sin256(4 * entity->timer) << 10);
    ++entity->timer;
    if ((entity->timer & 0xF) == 0)
        RSDK.CreateEntity(TitleEggman->objectID, (void *)1, entity->position.x - 0xE0000, entity->position.y + 0x80000);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->data4);
}

void TitleEggman_EditorDraw(void) {}

void TitleEggman_EditorLoad(void) {}

void TitleEggman_Serialize(void) {}
#endif
