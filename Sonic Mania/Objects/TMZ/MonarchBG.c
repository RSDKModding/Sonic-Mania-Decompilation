#include "SonicMania.h"

ObjectMonarchBG *MonarchBG;

void MonarchBG_Update(void)
{
    RSDK_THIS(MonarchBG);
    StateMachine_Run(entity->state);
}

void MonarchBG_LateUpdate(void) {}

void MonarchBG_StaticUpdate(void) {}

void MonarchBG_Draw(void)
{
    RSDK_THIS(MonarchBG);
    Vector2 drawPos;
    
    drawPos.x         = entity->position.x - ((entity->position.x - ((RSDK_screens->position.x + RSDK_screens->centerX) << 16)) >> 1);
    drawPos.y         = entity->position.y - 208 * ((entity->position.y - ((RSDK_screens->position.y + RSDK_screens->centerY) << 16)) >> 8);
    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animatorTop, &drawPos, false);
    RSDK.DrawSprite(&entity->animatorBottom, &drawPos, false);
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animatorTop, &drawPos, false);
    RSDK.DrawSprite(&entity->animatorBottom, &drawPos, false);
}

void MonarchBG_Create(void *data)
{
    RSDK_THIS(MonarchBG);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawFX        = FX_FLIP;
        entity->drawOrder     = 1;
        entity->scale.x       = 0xE00;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->position.x    = 0x8000000;
        entity->position.y    = 0xC000000;
        RSDK.SetSpriteAnimation(MonarchBG->aniFrames, 0, &entity->animatorTop, true, 0);
        RSDK.SetSpriteAnimation(MonarchBG->aniFrames, 1, &entity->animatorBottom, true, 0);
    }
}

void MonarchBG_StageLoad(void)
{
    MonarchBG->aniFrames  = RSDK.LoadSpriteAnimation("TMZ1/MonarchBG.bin", SCOPE_STAGE);
    MonarchBG->meshIndex  = RSDK.LoadMesh("TMZ/MonarchBG.bin", SCOPE_STAGE);
    MonarchBG->sceneIndex = RSDK.Create3DScene("View:Monarch", 0x2000, SCOPE_STAGE);

    RSDK.SetDiffuseColour(MonarchBG->sceneIndex, 64, 0, 64);
    RSDK.SetDiffuseIntensity(MonarchBG->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularIntensity(MonarchBG->sceneIndex, 16, 16, 16);
}

#if RETRO_INCLUDE_EDITOR
void MonarchBG_EditorDraw(void)
{
    RSDK_THIS(MonarchBG);
    entity->position.x    = 0x8000000;
    entity->position.y    = 0xC000000;
    RSDK.SetSpriteAnimation(MonarchBG->aniFrames, 0, &entity->animatorTop, true, 0);
    RSDK.SetSpriteAnimation(MonarchBG->aniFrames, 1, &entity->animatorBottom, true, 0);

    MonarchBG_Draw();
}

void MonarchBG_EditorLoad(void) { MonarchBG->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MonarchBG.bin", SCOPE_STAGE); }
#endif

void MonarchBG_Serialize(void) {}
