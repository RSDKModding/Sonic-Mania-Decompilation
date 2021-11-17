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
    if (entity->state == TitleEggman_State_Dust) {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
    else {
        Vector2 drawPos;
        entity->drawFX = FX_NONE;
        int32 angle      = RSDK.Sin256(2 * entity->timer) >> 1;

        for (int32 i = 1; i < 5; ++i) {
            drawPos.x = (i << 8) * RSDK.Sin1024(angle) + entity->position.x;
            drawPos.y = entity->position.y + 0x80000 + (i << 8) * RSDK.Cos1024(angle);
            RSDK.DrawSprite(&entity->animator3, &drawPos, false);
        }
        entity->drawFX   = FX_ROTATE;
        drawPos.x        = 0x500 * RSDK.Sin1024(angle) + entity->position.x;
        drawPos.y        = 0x500 * RSDK.Cos1024(angle) + entity->position.y + 0x80000;
        entity->rotation = -(angle >> 2);
        RSDK.DrawSprite(&entity->animator4, &drawPos, false);

        entity->drawFX = FX_FLIP;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
}

void TitleEggman_Create(void *data)
{
    RSDK_THIS(TitleEggman);
    if (!SceneInfo->inEditor) {
        entity->visible   = true;
        entity->drawOrder = 3;
        entity->active    = ACTIVE_NORMAL;
        if (data) {
            RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 5, &entity->animator1, true, 0);
            entity->state = TitleEggman_State_Dust;
        }
        else {
            RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 2, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 3, &entity->animator3, true, 0);
            RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 4, &entity->animator4, true, 0);
            entity->startPos.x = entity->position.x;
            entity->startPos.y = entity->position.y;
            entity->velocity.x = 0x10000;
            entity->state      = TitleEggman_State_Wait;
        }
    }
}

void TitleEggman_StageLoad(void)
{
    if (API.CheckDLC(DLC_PLUS))
        TitleEggman->aniFrames = RSDK.LoadSpriteAnimation("Title/PlusLogo.bin", SCOPE_STAGE);
}

void TitleEggman_State_Dust(void)
{
    RSDK_THIS(TitleEggman);
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        destroyEntity(entity);
}
void TitleEggman_State_Wait(void)
{
    RSDK_THIS(TitleEggman);
    if (++entity->timer == 120) {
        entity->timer = 0;
        entity->state = TitleEggman_State_Move;
    }
}
void TitleEggman_State_Move(void)
{
    RSDK_THIS(TitleEggman);
    entity->position.x += entity->velocity.x;
    entity->startPos.y -= 0x6000;
    entity->position.y = entity->startPos.y + (RSDK.Sin256(4 * entity->timer) << 10);
    ++entity->timer;
    if (!(entity->timer & 0xF))
        CREATE_ENTITY(TitleEggman, intToVoid(1), entity->position.x - 0xE0000, entity->position.y + 0x80000);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator4);
}

#if RETRO_INCLUDE_EDITOR
void TitleEggman_EditorDraw(void)
{
    RSDK_THIS(TitleEggman);
    RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 2, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 3, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 4, &entity->animator4, true, 0);
    entity->startPos.x = entity->position.x;
    entity->startPos.y = entity->position.y;

    TitleEggman_Draw();
}

void TitleEggman_EditorLoad(void) { TitleEggman->aniFrames = RSDK.LoadSpriteAnimation("Title/PlusLogo.bin", SCOPE_STAGE); }
#endif

void TitleEggman_Serialize(void) {}
#endif
