#include "SonicMania.h"

ObjectBladePole *BladePole;

void BladePole_Update(void)
{
    RSDK_THIS(BladePole);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    StateMachine_Run(entity->state);
}

void BladePole_LateUpdate(void) {}

void BladePole_StaticUpdate(void) {}

void BladePole_Draw(void) { BladePole_DrawSprites(); }

void BladePole_Create(void *data)
{
    RSDK_THIS(BladePole);
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(BladePole->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(BladePole->aniFrames, 1, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(BladePole->aniFrames, 1, &entity->animator3, true, 0);
    BladePole->hitbox2.top    = 20;
    BladePole->hitbox2.bottom = BladePole->hitbox2.top + 24;
    BladePole->hitbox2.left   = -24;
    BladePole->hitbox2.right  = 24;
    BladePole->hitbox1.top    = -32;
    BladePole->hitbox1.bottom = BladePole->hitbox1.top + 24;
    BladePole->hitbox1.left   = -24;
    BladePole->hitbox1.right  = 24;
    entity->state             = BladePole_Unknown4;
}

void BladePole_StageLoad(void) { BladePole->aniFrames = RSDK.LoadSpriteAnimation("MMZ/BladePole.bin", SCOPE_STAGE); }

void BladePole_DrawSprites(void)
{
    RSDK_THIS(BladePole);
    Vector2 drawPos;

    entity->direction = FLIP_NONE;
    drawPos.x         = entity->position.x;
    drawPos.y         = entity->position.y;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    drawPos.y -= 0xC0000;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    drawPos.y -= 0xC0000;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y - 0x180000;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    if (SceneInfo->inEditor) {
        drawPos.y += 0x180000;
    }
    else {
        entity->direction = FLIP_Y;
        drawPos.y += 0x3C0000;
    }

    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
    entity->direction = FLIP_NONE;
}

bool32 BladePole_SetAnimation(Animator *animator)
{
    RSDK_THIS(BladePole);

    switch (animator->animationID) {
        case 1: RSDK.SetSpriteAnimation(BladePole->aniFrames, 2, animator, true, 0); break;
        case 2:
            if (animator->frameID == animator->frameCount - 1)
                RSDK.SetSpriteAnimation(BladePole->aniFrames, 3, animator, true, 0);
            break;
        case 3:
            if (entity->timer >= 74 && !animator->frameID)
                RSDK.SetSpriteAnimation(BladePole->aniFrames, 4, animator, true, 0);
            break;
        case 4:
            if (animator->frameID == animator->frameCount - 1) {
                RSDK.SetSpriteAnimation(BladePole->aniFrames, 1, animator, true, 0);
                return true;
            }
            break;
        default: break;
    }
    return false;
}

void BladePole_CheckPlayerCollisions(Hitbox *hitbox)
{
    RSDK_THIS(BladePole);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, hitbox)) {
            int32 storeX         = entity->position.x;
            int32 storeY         = entity->position.y;
            entity->position.y = ((BladePole->hitbox2.bottom - BladePole->hitbox2.top) << 15) + storeY;
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(1024, player, 2, &player->uncurlTimer)) {
#endif
                Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
            }
#endif
            entity->position.x = storeX;
            entity->position.y = storeY;
        }
    }
}

void BladePole_Unknown4(void)
{
    RSDK_THIS(BladePole);

    if (BladePole_SetAnimation(&entity->animator2)) {
        entity->state = BladePole_Unknown5;
        entity->timer = 0;
    }
    if (entity->animator2.animationID == 3)
        BladePole_CheckPlayerCollisions(&BladePole->hitbox1);
    ++entity->timer;
}

void BladePole_Unknown5(void)
{
    RSDK_THIS(BladePole);

    if (BladePole_SetAnimation(&entity->animator3)) {
        entity->state = BladePole_Unknown4;
        entity->timer = 0;
    }
    if (entity->animator3.animationID == 3)
        BladePole_CheckPlayerCollisions(&BladePole->hitbox2);
    ++entity->timer;
}

#if RETRO_INCLUDE_EDITOR
void BladePole_EditorDraw(void) { BladePole_DrawSprites(); }

void BladePole_EditorLoad(void) { BladePole->aniFrames = RSDK.LoadSpriteAnimation("MMZ/BladePole.bin", SCOPE_STAGE); }
#endif

void BladePole_Serialize(void) {}
