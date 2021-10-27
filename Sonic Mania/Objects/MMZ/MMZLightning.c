#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectMMZLightning *MMZLightning;

void MMZLightning_Update(void)
{
    RSDK_THIS(MMZLightning);
    StateMachine_Run(entity->state);
}

void MMZLightning_LateUpdate(void) {}

void MMZLightning_StaticUpdate(void) {}

void MMZLightning_Draw(void)
{
    RSDK_THIS(MMZLightning);

    RSDK.SetActivePalette(1, 0, RSDK_screens->height);

    RSDK.DrawSprite(&entity->animator, 0, true);

    RSDK.SetActivePalette(0, 0, RSDK_screens->height);
}

void MMZLightning_Create(void *data)
{
    RSDK_THIS(MMZLightning);

    if (!RSDK_sceneInfo->inEditor) {
        entity->active = ACTIVE_NORMAL;
        entity->state  = (Type_StateMachine)data;
    }
}

void MMZLightning_StageLoad(void) { MMZLightning->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Lightning.bin", SCOPE_STAGE); }

void MMZLightning_Unknown1(void)
{
    RSDK_THIS(MMZLightning);
    if (!entity->timer)
        RSDK.SetLimitedFade(0, 0, 5, 192, 0, 255);
    if (++entity->timer == 4) {
        entity->timer = 0;
        entity->state = MMZLightning_Unknown2;
    }
}

void MMZLightning_Unknown2(void)
{
    RSDK_THIS(MMZLightning);
    RSDK.SetLimitedFade(0, 4, 1, entity->timer, 0, 255);

    entity->timer += 4;
    if (entity->timer > 256)
        destroyEntity(entity);
}

void MMZLightning_Unknown3(void)
{
    RSDK_THIS(MMZLightning);
    RSDK.SetSpriteAnimation(MMZLightning->aniFrames, 0, &entity->animator, true, 0);
    entity->position.x = (RSDK_screens->centerX + RSDK.Rand(-140, 140)) << 16;
    entity->position.y = RSDK.Rand(0x200000, 0x400000);
    entity->visible    = true;
    entity->drawFX     = FX_FLIP;
    entity->direction  = RSDK.Rand(0, 2);
    entity->alpha      = 512;
    entity->inkEffect  = INK_ADD;
    entity->drawOrder  = 1;
    entity->state      = MMZLightning_Unknown4;
}

void MMZLightning_Unknown4(void)
{
    RSDK_THIS(MMZLightning);
    RSDK.ProcessAnimation(&entity->animator);

    if (entity->animator.frameID == entity->animator.frameCount - 1)
        entity->state = MMZLightning_Unknown5;
}

void MMZLightning_Unknown5(void)
{
    RSDK_THIS(MMZLightning);

    if (entity->alpha > 0)
        entity->alpha -= 8;
    else
        destroyEntity(entity);
}

void MMZLightning_Unknown6(void)
{
    RSDK_THIS(MMZLightning);
    RSDK.SetSpriteAnimation(MMZLightning->aniFrames, 1, &entity->animator, true, RSDK.Rand(0, 4));
    entity->position.x = (RSDK_screens->centerX + RSDK.Rand(-64, 64)) << 16;
    entity->position.y = RSDK.Rand(0x400000, 0x600000);
    entity->visible    = true;
    entity->drawFX     = FX_FLIP;
    entity->direction  = RSDK.Rand(0, 4);
    entity->alpha      = 320;
    entity->inkEffect  = INK_ADD;
    entity->drawOrder  = 1;
    entity->state      = MMZLightning_Unknown7;
}

void MMZLightning_Unknown7(void)
{
    RSDK_THIS(MMZLightning);

    if (entity->alpha > 0)
        entity->alpha -= 0x20;
    else
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void MMZLightning_EditorDraw(void) {}

void MMZLightning_EditorLoad(void) {}
#endif

void MMZLightning_Serialize(void) {}
#endif
