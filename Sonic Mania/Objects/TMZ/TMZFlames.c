#include "SonicMania.h"

ObjectTMZFlames *TMZFlames;

void TMZFlames_Update(void)
{
    RSDK_THIS(TMZFlames);

    entity->active = ACTIVE_NORMAL;
    RSDK.ProcessAnimation(&entity->animator);
    StateMachine_Run(entity->state);
}

void TMZFlames_LateUpdate(void) {}

void TMZFlames_StaticUpdate(void) {}

void TMZFlames_Draw(void)
{
    RSDK_THIS(TMZFlames);
    Vector2 drawPos;

    drawPos.y = (RSDK_screens[RSDK_sceneInfo->currentScreenID].centerY - 112) << 16;
    for (int i = 0; i < 0x80; i += 0x10) {
        drawPos.x = (RSDK.Sin256(4 * (i + Zone->timer)) << 11) + entity->offset;
        RSDK.DrawSprite(&entity->animator, &drawPos, true);
        drawPos.y += 0x200000;
    }
}

void TMZFlames_Create(void *data)
{
    RSDK_THIS(TMZFlames);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->drawFX        = FX_SCALE;
        entity->inkEffect     = INK_ALPHA;
        entity->alpha         = 0x100;
        entity->scale.x       = 0x400;
        entity->scale.y       = 0x200;
        entity->offset        = -0x800000;
        entity->state         = TMZFlames_Unknown1;
        RSDK.SetSpriteAnimation(TMZFlames->aniFrames, 0, &entity->animator, true, 0);
    }
}

void TMZFlames_StageLoad(void) { TMZFlames->aniFrames = RSDK.LoadSpriteAnimation("Phantom/Flames.bin", SCOPE_STAGE); }

void TMZFlames_Unknown1(void)
{
    RSDK_THIS(TMZFlames);
    if (++entity->timer == 240) {
        entity->timer = 0;
        entity->state = TMZFlames_Unknown2;
    }
}

void TMZFlames_Unknown2(void)
{
    RSDK_THIS(TMZFlames);

    if (entity->offset < 0)
        entity->offset += 0x40000;

    if (++entity->timer == 260) {
        entity->timer = 0;
        entity->state = TMZFlames_Unknown3;
    }
}

void TMZFlames_Unknown3(void)
{
    RSDK_THIS(TMZFlames);

    if (entity->scale.x < 0x1000)
        entity->scale.x += 16;
}

void TMZFlames_EditorDraw(void) {}

void TMZFlames_EditorLoad(void) {}

void TMZFlames_Serialize(void) {}
