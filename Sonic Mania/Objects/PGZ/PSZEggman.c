#include "SonicMania.h"

ObjectPSZEggman *PSZEggman;

void PSZEggman_Update(void)
{
    RSDK_THIS(PSZEggman);
    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animator);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
}

void PSZEggman_LateUpdate(void) {}

void PSZEggman_StaticUpdate(void) {}

void PSZEggman_Draw(void)
{
    RSDK_THIS(PSZEggman);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);

    entity->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&entity->animator4, NULL, false);

    entity->inkEffect = INK_NONE;
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void PSZEggman_Create(void *data)
{
    RSDK_THIS(PSZEggman);
    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->drawPos.x     = entity->position.x - 0x180000;
        entity->drawPos.y     = entity->position.y + 0x10000;
        entity->alpha         = 64;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 1, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 2, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(PSZEggman->aniFrames, 0, &entity->animator, true, 0);
    }
}

void PSZEggman_StageLoad(void)
{
    PSZEggman->controlFrames = RSDK.LoadSpriteAnimation("PSZ2/ControlPanel.bin", SCOPE_STAGE);
    PSZEggman->aniFrames     = RSDK.LoadSpriteAnimation("Eggman/EggmanPSZ.bin", SCOPE_STAGE);
}

void PSZEggman_Unknown1(void)
{
    RSDK_THIS(PSZEggman);
    if (entity->animator.animationTimer == 1) {
        if (entity->ruby) {
            entity->ruby->position.x += 0x10000;
            ++entity->timer;
        }
        else if (entity->animator.frameID == 7) {
            entity->ruby = CREATE_ENTITY(PhantomRuby, NULL, entity->drawPos.x - 0x100000, entity->drawPos.y - 0x20000);
        }
    }
    if (entity->timer == 6) {
        entity->ruby->startPos.x = entity->ruby->position.x;
        entity->ruby->startPos.y = entity->ruby->position.y;
        entity->ruby->state      = PhantomRuby_Unknown5;
        entity->state            = StateMachine_None;
    }
}

#if RETRO_INCLUDE_EDITOR
void PSZEggman_EditorDraw(void) {}

void PSZEggman_EditorLoad(void) {}
#endif

void PSZEggman_Serialize(void) {}
