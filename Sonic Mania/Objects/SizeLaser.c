#include "../SonicMania.h"

ObjectSizeLaser *SizeLaser;

void SizeLaser_Update()
{

}

void SizeLaser_LateUpdate()
{

}

void SizeLaser_StaticUpdate()
{

}

void SizeLaser_Draw()
{
    EntitySizeLaser *entity = (EntitySizeLaser *)RSDK_sceneInfo->entity;
    RSDK.DrawSprite(&entity->animData1, 0, 0);
    //if (entity->state == SizeLaser_Unknown10)
    //    RSDK.DrawSprite(&entity->animData2, &entity->storedPos, 0);
}

void SizeLaser_Create(void* data)
{
    EntitySizeLaser *entity             = (EntitySizeLaser *)RSDK_sceneInfo->entity;
    RSDK_sceneInfo->entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        int type              = (int)(size_t)data;
        entity->visible       = true;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (!type) {
            entity->active    = ACTIVE_BOUNDS;
            entity->drawOrder = Zone->drawOrderLow + 1;

            //entity->state = SizeLaser_Unknown8;
            switch (entity->orientation) {
                case 0:
                    entity->direction = FLIP_NONE;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type, &entity->animData1, true, 0);
                    break;
                case 1:
                    entity->direction = FLIP_NONE;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 1, &entity->animData1, true, 0);
                    break;
                case 2:
                    entity->direction = FLIP_X;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 1, &entity->animData1, true, 0);
                    break;
            }
        }
        else {
            entity->active      = ACTIVE_NORMAL;
            entity->drawOrder   = Zone->drawOrderLow;
            entity->orientation = (type - 1) >> 2;
            entity->type        = (type - 1) & 1;

            //entity->state = SizeLaser_Unknown9;
            switch (entity->orientation) {
                default:
                case 0:
                    entity->velocity.y = 0x40000;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * (entity->type + 2), &entity->animData1, true, 0);
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * (entity->type + 4), &entity->animData2, true, 0);
                    break;
                case 1:
                    entity->velocity.x = 0x40000;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 5, &entity->animData1, true, 0);
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 9, &entity->animData2, true, 0);
                    break;
                case 2:
                    entity->direction  = FLIP_X;
                    entity->velocity.x = -0x40000;
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 5, &entity->animData1, true, 0);
                    RSDK.SetSpriteAnimation(SizeLaser->spriteIndex, 2 * entity->type + 9, &entity->animData2, true, 0);
                    break;
            }
        }
    }
}

void SizeLaser_StageLoad()
{
    SizeLaser->spriteIndex = RSDK.LoadSpriteAnimation("MMZ/SizeLaser.bin", SCOPE_STAGE);
    switch (globals->playerID & 0xFF) {
        case ID_SONIC: SizeLaser->sonicIndex = RSDK.LoadSpriteAnimation("Players/ChibiSonic.bin", SCOPE_STAGE); break;
        case ID_TAILS:
            SizeLaser->tailsIndex      = RSDK.LoadSpriteAnimation("Players/ChibiTails.bin", SCOPE_STAGE);
            SizeLaser->tailSpriteIndex = RSDK.LoadSpriteAnimation("Players/CTailSprite.bin", SCOPE_STAGE);
            break;
        case ID_KNUCKLES: SizeLaser->knuxIndex = RSDK.LoadSpriteAnimation("Players/ChibiKnux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
        case ID_MIGHTY: SizeLaser->mightyIndex = RSDK.LoadSpriteAnimation("Players/ChibiMighty.bin", SCOPE_STAGE); break;
        case ID_RAY: SizeLaser->rayIndex = RSDK.LoadSpriteAnimation("Players/ChibiRay.bin", SCOPE_STAGE); break;
#endif
        default: break;
    }

    switch ((globals->playerID >> 8) & 0xFF) {
        case ID_SONIC: SizeLaser->sonicIndex = RSDK.LoadSpriteAnimation("Players/ChibiSonic.bin", SCOPE_STAGE); break;
        case ID_TAILS:
            SizeLaser->tailsIndex      = RSDK.LoadSpriteAnimation("Players/ChibiTails.bin", SCOPE_STAGE);
            SizeLaser->tailSpriteIndex = RSDK.LoadSpriteAnimation("Players/CTailSprite.bin", SCOPE_STAGE);
            break;
        case ID_KNUCKLES: SizeLaser->knuxIndex = RSDK.LoadSpriteAnimation("Players/ChibiKnux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
        case ID_MIGHTY: SizeLaser->mightyIndex = RSDK.LoadSpriteAnimation("Players/ChibiMighty.bin", SCOPE_STAGE); break;
        case ID_RAY: SizeLaser->rayIndex = RSDK.LoadSpriteAnimation("Players/ChibiRay.bin", SCOPE_STAGE); break;
#endif
        default: break;
    }

    SizeLaser->hitbox.left   = -4;
    SizeLaser->hitbox.top    = -3;
    SizeLaser->hitbox.right  = 4;
    SizeLaser->hitbox.bottom = 4;
    SizeLaser->sfx_Shrink2   = RSDK.GetSFX("MMZ/Shrink2.wav");
    SizeLaser->sfx_Grow2     = RSDK.GetSFX("MMZ/Grow2.wav");

    Soundboard_LoadSFX("MMZ/SizeLaser.wav", 1, (void*)SizeLaser_SizeChangeSFXCheck, NULL);
}

bool32 SizeLaser_SizeChangeSFXCheck()
{
    Entity *entity = NULL;

    int cnt = 0;
    while (RSDK.GetActiveEntities(SizeLaser->objectID, &entity)) {
        if (entity->activeScreens)
            ++cnt;
    }
    return cnt > 0;
}

void SizeLaser_EditorDraw()
{

}

void SizeLaser_EditorLoad()
{

}

void SizeLaser_Serialize()
{
    RSDK_EDITABLE_VAR(SizeLaser, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(SizeLaser, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(SizeLaser, VAR_ENUM, extend);
}

