#include "SonicMania.h"

ObjectThoughtBubble *ThoughtBubble;

void ThoughtBubble_Update(void)
{
    RSDK_THIS(ThoughtBubble);
    StateMachine_Run(entity->state);
    entity->drawPos1.x = entity->position.x + 0x280000;
    entity->drawPos1.y = ((RSDK.Sin256(-4 * Zone->timer) + 0x1A00) << 9) + entity->position.y;
    entity->drawPos2.x = entity->position.x + 0x180000;
    entity->drawPos2.y = ((RSDK.Sin256(4 * Zone->timer) + 0xB00) << 10) + entity->position.y;
}

void ThoughtBubble_LateUpdate(void) {}

void ThoughtBubble_StaticUpdate(void) {}

void ThoughtBubble_Draw(void)
{
    RSDK_THIS(ThoughtBubble);
    switch (entity->type) {
        case 0: RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false); break;
        case 1:
            RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);
            RSDK.DrawSprite(&entity->animator4, &entity->drawPos2, false);
            break;
        case 2:
            RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);
            RSDK.DrawSprite(&entity->animator4, &entity->drawPos2, false);
            RSDK.DrawSprite(&entity->animator1, NULL, false);
            break;
        case 3:
            RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);
            RSDK.DrawSprite(&entity->animator4, &entity->drawPos2, false);
            RSDK.DrawSprite(&entity->animator1, NULL, false);
            RSDK.DrawSprite(&entity->animator2, NULL, false);
            break;
        default: break;
    }
}

void ThoughtBubble_Create(void *data)
{
    RSDK_THIS(ThoughtBubble);
    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->state         = ThoughtBubble_Unknown1;
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &entity->animator4, true, 3);
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 3, &entity->animator2, true, 0);
    }
}

void ThoughtBubble_StageLoad(void)
{
    ThoughtBubble->aniFrames  = RSDK.LoadSpriteAnimation("LRZ3/ThoughtBubble.bin", SCOPE_STAGE);
    ThoughtBubble->dustFrames = RSDK.LoadSpriteAnimation("Global/Dust.bin", SCOPE_STAGE);
}

void ThoughtBubble_Unknown1(void)
{
    RSDK_THIS(ThoughtBubble);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    if (++entity->timer == 8) {
        entity->timer = 0;
        if (++entity->type == 2)
            entity->state = ThoughtBubble_Unknown2;
    }
}
void ThoughtBubble_Unknown2(void)
{
    RSDK_THIS(ThoughtBubble);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 2, &entity->animator1, true, 0);
        entity->state = ThoughtBubble_Unknown3;
        ++entity->type;
    }
}
void ThoughtBubble_Unknown3(void)
{
    RSDK_THIS(ThoughtBubble);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    if (++entity->timer == 80) {
        entity->timer = 0;
        if (entity->animator2.animationID >= 6) {
            entity->state = ThoughtBubble_Unknown4;
        }
        else {
            entity->animator2.animationID++;
            RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, entity->animator2.animationID, &entity->animator2, true, 0);
        }
    }
}
void ThoughtBubble_Unknown4(void)
{
    RSDK_THIS(ThoughtBubble);
    entity->type = 1;
    for (int32 i = 0; i < 8; ++i) {
        EntityDebris *debris =
            CREATE_ENTITY(Debris, NULL, entity->position.x + RSDK.Rand(-0x180000, 0x180000), entity->position.y + RSDK.Rand(-0x100000, 0x100000));
        debris->state        = Debris_State_Move;
        debris->velocity.x   = RSDK.Rand(-0x20000, 0x20000);
        debris->velocity.y   = RSDK.Rand(-0x20000, 0x20000);
        debris->drawFX       = FX_SCALE | FX_FLIP;
        debris->direction    = i & 3;
        int32 scale            = RSDK.Rand(0x200, 0x400);
        debris->scale.x      = scale;
        debris->scale.y      = scale;
        debris->drawOrder    = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(ThoughtBubble->dustFrames, 0, &debris->animator, true, RSDK.Rand(0, 4));
    }
    entity->state = ThoughtBubble_Unknown5;
}
void ThoughtBubble_Unknown5(void)
{
    RSDK_THIS(ThoughtBubble);
    if (++entity->timer == 8) {
        entity->timer = 0;
        if (--entity->type < 0)
            destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void ThoughtBubble_EditorDraw(void)
{
    RSDK_THIS(ThoughtBubble);
    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 0, &entity->animator4, true, 3);
    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 1, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(ThoughtBubble->aniFrames, 3, &entity->animator2, true, 0);

    entity->type = 3;

    entity->drawPos1.x = entity->position.x + 0x280000;
    entity->drawPos1.y = ((RSDK.Sin256(-4) + 0x1A00) << 9) + entity->position.y;
    entity->drawPos2.x = entity->position.x + 0x180000;
    entity->drawPos2.y = ((RSDK.Sin256(4) + 0xB00) << 10) + entity->position.y;

    ThoughtBubble_Draw();
}

void ThoughtBubble_EditorLoad(void) {}
#endif

void ThoughtBubble_Serialize(void) {}
