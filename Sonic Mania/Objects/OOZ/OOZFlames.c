#include "SonicMania.h"

ObjectOOZFlames *OOZFlames;

void OOZFlames_Update(void)
{
    RSDK_THIS(OOZFlames);
    RSDK.ProcessAnimation(&entity->animator);
    StateMachine_Run(entity->state);
}

void OOZFlames_LateUpdate(void) {}

void OOZFlames_StaticUpdate(void) {}

void OOZFlames_Draw(void)
{
    RSDK_THIS(OOZFlames);
    Vector2 drawPos;

    drawPos.x = (RSDK_screens[RSDK_sceneInfo->currentScreenID].centerX - 240) << 16;
    for (int32 angle = 0; angle < 0x100; angle += 0x10) {
        drawPos.y = (RSDK.Sin256(4 * (angle + Zone->timer)) << 11) + entity->field_60;
        RSDK.DrawSprite(&entity->animator, &drawPos, true);
        drawPos.x += 0x200000;
    }
}

void OOZFlames_Create(void *data)
{
    RSDK_THIS(OOZFlames);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible   = true;
        entity->drawOrder = Zone->drawOrderHigh;
        entity->active    = ACTIVE_NORMAL;
        entity->drawFX    = FX_SCALE;
        entity->inkEffect = INK_ADD;
        entity->alpha     = 256;
        entity->scale.x   = 512;
        entity->scale.y   = 512;
        entity->field_60  = 0x1400000;
        entity->state     = OOZFlames_Unknown1;
        RSDK.SetSpriteAnimation(OOZFlames->aniFrames, 0, &entity->animator, true, 0);
    }
}

void OOZFlames_StageLoad(void) { OOZFlames->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Flames.bin", SCOPE_STAGE); }

void OOZFlames_Unknown1(void)
{
    RSDK_THIS(OOZFlames);

    if (entity->field_60 > 0xE00000) {
        entity->field_60 -= 0x20000;
    }
    if (++entity->timer == 120) {
        entity->timer = 0;
        entity->state = OOZFlames_Unknown2;
    }
}

void OOZFlames_Unknown2(void)
{
    RSDK_THIS(OOZFlames);

    if (entity->scale.y < 4096) {
        entity->scale.y += 16;
    }
}

void OOZFlames_EditorDraw(void) {}

void OOZFlames_EditorLoad(void) {}

void OOZFlames_Serialize(void) {}
