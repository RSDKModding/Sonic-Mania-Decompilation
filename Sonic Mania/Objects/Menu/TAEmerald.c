#include "SonicMania.h"

ObjectTAEmerald *TAEmerald;

void TAEmerald_Update(void)
{
    RSDK_THIS(TAEmerald);
    StateMachine_Run(entity->state);
}

void TAEmerald_LateUpdate(void) {}

void TAEmerald_StaticUpdate(void) {}

void TAEmerald_Draw(void)
{
    RSDK_THIS(TAEmerald);
    RSDK.SetActivePalette(3, 0, RSDK_screens->height);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void TAEmerald_Create(void *data)
{
    RSDK_THIS(TAEmerald);
    if (!RSDK_sceneInfo->inEditor) {
        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->angle         = 16 * entity->color;
        entity->visible       = true;
        entity->drawOrder     = 1;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->state         = TAEmerald_Unknown1;
        RSDK.SetSpriteAnimation(TAEmerald->aniFrames, 7, &entity->animator, true, entity->color);
        if (SaveGame->saveRAM) {
            if (((1 << entity->color) & SaveGame->saveRAM->chaosEmeralds) > 0)
                entity->visible = false;
        }
    }
}

void TAEmerald_StageLoad(void) { TAEmerald->aniFrames = RSDK.LoadSpriteAnimation("Special/Results.bin", SCOPE_STAGE); }

void TAEmerald_Unknown1(void)
{
    RSDK_THIS(TAEmerald);
    entity->position.y = (RSDK.Sin256(RSDK_sceneInfo->entity->angle) << 11) + entity->startPos.y;
    entity->angle      = (entity->angle + 4);
}

void TAEmerald_Unknown2(void)
{
    RSDK_THIS(TAEmerald);

    if (entity->timer > 0) {
        entity->timer--;
    }
    else {
        if (entity->direction) {
            entity->angle += 2;
            if (entity->angle >= 0) {
                entity->direction = FLIP_NONE;
                entity->state     = StateMachine_None;
            }
        }
        else {
            entity->angle -= 2;
            if (entity->angle <= -128) {
                entity->direction = FLIP_X;
                entity->state     = StateMachine_None;
            }
        }

        entity->rotation   = 4 * entity->angle;
        entity->position.x = 0x4800 * RSDK.Cos256(entity->angle) + entity->startPos.x;
        entity->position.y = 0x4800 * RSDK.Sin256(entity->angle) + entity->startPos.y;
    }
}

#if RETRO_INCLUDE_EDITOR
void TAEmerald_EditorDraw(void) {}

void TAEmerald_EditorLoad(void) {}
#endif

void TAEmerald_Serialize(void) { RSDK_EDITABLE_VAR(TAEmerald, VAR_UINT8, color); }
