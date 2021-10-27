#include "SonicMania.h"

ObjectTMZAlert *TMZAlert;

void TMZAlert_Update(void)
{
    RSDK_THIS(TMZAlert);
    StateMachine_Run(entity->state);
}

void TMZAlert_LateUpdate(void) {}

void TMZAlert_StaticUpdate(void) {}

void TMZAlert_Draw(void)
{
    RSDK_THIS(TMZAlert);
    entity->drawFX           = FX_NONE;
    entity->inkEffect        = INK_NONE;
    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, NULL, false);
    if (entity->scale.y != 0x200)
        entity->drawFX = FX_SCALE;
    entity->inkEffect        = INK_ALPHA;
    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void TMZAlert_Create(void *data)
{
    RSDK_THIS(TMZAlert);
    if (!RSDK_sceneInfo->inEditor) {
        entity->inkEffect     = INK_ALPHA;
        entity->visible       = true;
        entity->drawFX        = FX_SCALE;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->scale.x       = 0x200;
        entity->alpha         = 16 * (RSDK.Rand(-32, -8) - 16);
        RSDK.SetSpriteAnimation(TMZAlert->aniFrames, 0, &entity->animator, true, 0);
    }
}

void TMZAlert_StageLoad(void) { TMZAlert->aniFrames = RSDK.LoadSpriteAnimation("Phantom/AlertScreen.bin", SCOPE_STAGE); }

void TMZAlert_Unknown1(void)
{
    RSDK_THIS(TMZAlert);

    if (entity->alpha >= 256) {
        entity->drawFX = FX_NONE;
        entity->state  = TMZAlert_Unknown2;
    }
    else {
        entity->alpha += 32;
        if (entity->alpha > 256)
            entity->alpha = 256;
        entity->scale.y = 2 * entity->alpha;
    }
}
void TMZAlert_Unknown2(void)
{
    RSDK_THIS(TMZAlert);
    entity->alpha = (RSDK.Cos256(8 * ++entity->timer) >> 2) + 224;
    if (!(entity->timer & 0xF)) {
        RSDK.SetSpriteAnimation(TMZAlert->aniFrames, 1, &entity->animator2, true, RSDK.Rand(0, 2));
    }
    if (entity->timer == 320) {
        entity->timer = 0;
        entity->state = TMZAlert_Unknown3;
    }
}
void TMZAlert_Unknown3(void)
{
    RSDK_THIS(TMZAlert);
    if (entity->alpha <= 16) {
        entity->state = StateMachine_None;
    }
    else {
        entity->alpha -= 8;
    }
}

#if RETRO_INCLUDE_EDITOR
void TMZAlert_EditorDraw(void) {}

void TMZAlert_EditorLoad(void) {}
#endif

void TMZAlert_Serialize(void) {}
