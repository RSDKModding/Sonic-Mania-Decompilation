#include "SonicMania.h"

ObjectFireflies *Fireflies;

void Fireflies_Update(void)
{
    RSDK_THIS(Fireflies);
    StateMachine_Run(entity->state);
}

void Fireflies_LateUpdate(void) {}

void Fireflies_StaticUpdate(void) {}

void Fireflies_Draw(void)
{
    RSDK_THIS(Fireflies);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Fireflies_Create(void *data)
{
    RSDK_THIS(Fireflies);
    if (!SceneInfo->inEditor) {
        entity->active        = ACTIVE_NORMAL;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->visible       = true;
        entity->drawFX        = FX_FLIP;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (data) {
            entity->inkEffect = INK_ADD;
            entity->state     = Fireflies_State_Unknown2;
        }
        else {
            entity->state       = Fireflies_State_Unknown1;
            entity->screenCount = RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT);
        }
    }
}

void Fireflies_StageLoad(void) { Fireflies->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Fireflies.bin", SCOPE_STAGE); }

void Fireflies_State_Unknown1(void)
{
    RSDK_THIS(Fireflies);

    if (entity->timer <= 0) {
        entity->timer = 1;
        if (Fireflies->field_8 < 48) {
            Vector2 pos1, pos2;

            RSDKScreenInfo *screen = &ScreenInfo[entity->screenID];
            int x              = ((screen->width & 0xFFFFFFFE) + 2 * screen->position.x) << 15;
            int y              = ((screen->height & 0xFFFFFFFE) + 2 * screen->position.y) << 15;
#if RETRO_USE_PLUS
            int w = RSDK.Random(-screen->width, screen->width, &Zone->randKey);
            int h = RSDK.Random(-screen->height, screen->height, &Zone->randKey);
#else
            int w               = RSDK.Rand(-screen->width, screen->width);
            int h               = RSDK.Rand(-screen->height, screen->height);
#endif
            pos1.y = y + (h << 16);
            pos1.x = x + (w << 16);
#if RETRO_USE_PLUS
            int type = RSDK.Random(0, 10, &Zone->randKey) > 7;
#else
            int type            = RSDK.Rand(0, 10) > 7;
#endif

            EntityFireflies *fireflies = CREATE_ENTITY(Fireflies, intToVoid(true), pos1.x, pos1.y);
            RSDK.SetSpriteAnimation(Fireflies->aniFrames, type ? 3 : 0, &fireflies->animator, true, 0);
            if (type)
                fireflies->drawOrder = Zone->drawOrderHigh;
            else
                fireflies->drawOrder = 1;

#if RETRO_USE_PLUS
            fireflies->field_84 = RSDK.Random(45, 75, &Zone->randKey);
#else
            fireflies->field_84 = RSDK.Rand(45, 75);
#endif
            fireflies->updateRange.x = 0x800000;
            fireflies->updateRange.y = 0x800000;
            fireflies->active        = ACTIVE_NORMAL;
            fireflies->pos1          = pos1;

#if RETRO_USE_PLUS
            int amplitude = RSDK.Random(32, 128, &Zone->randKey);
            int angle     = RSDK.Random(0, 511, &Zone->randKey);
#else
            int amplitude       = RSDK.Rand(32, 128);
            int angle           = RSDK.Rand(0, 511);
#endif
            int x4 = pos1.x + amplitude * (RSDK.Cos512(angle) << 7);
            int y4 = amplitude * (RSDK.Sin512(angle) << 7) + pos1.y;

#if RETRO_USE_PLUS
            amplitude = RSDK.Random(32, 64, &Zone->randKey);
            angle     = RSDK.Random(0, 511, &Zone->randKey);
#else
            amplitude           = RSDK.Rand(32, 64);
            angle               = RSDK.Rand(0, 511);
#endif
            pos2.x          = pos1.x + amplitude * (RSDK.Cos512(angle) << 7);
            pos2.y          = amplitude * (RSDK.Sin512(angle) << 7) + pos1.y;
            fireflies->pos2 = pos2;

#if RETRO_USE_PLUS
            amplitude = RSDK.Random(32, 64, &Zone->randKey);
            angle     = RSDK.Random(0, 511, &Zone->randKey);
#else
            amplitude           = RSDK.Rand(32, 64);
            angle               = RSDK.Rand(0, 511);
#endif
            fireflies->pos3.x = x4 + amplitude * (RSDK.Cos512(angle) << 7);
            fireflies->pos3.y = amplitude * (RSDK.Sin512(angle) << 7) + y4;

            fireflies->pos4.x = x4;
            fireflies->pos4.y = y4;

            ++Fireflies->field_8;

            entity->screenID = (entity->screenID + 1) % entity->screenCount;
        }
    }
    else {
        entity->timer--;
    }
}

void Fireflies_State_Unknown2(void)
{
    RSDK_THIS(Fireflies);

    int frame = 0, anim = 0, anim2 = 0;
    if (entity->animator.animationID % 3 || entity->animator.frameID != entity->animator.frameCount - 1) {
        if (entity->timer == entity->field_84 - 28) {
            frame = 0;
            anim2 = 5;
            anim  = 2;
            if (entity->drawOrder == Zone->drawOrderHigh)
                anim = anim2;

            RSDK.SetSpriteAnimation(Fireflies->aniFrames, anim, &entity->animator, true, frame);
        }
    }
    else {
        frame = entity->animator.animationID % 3;
        anim  = 1;
        anim2 = 4;
        if (entity->drawOrder == Zone->drawOrderHigh)
            anim = anim2;

        RSDK.SetSpriteAnimation(Fireflies->aniFrames, anim, &entity->animator, true, frame);
    }

    int percent      = (entity->timer << 16) / entity->field_84;
    entity->position = MathHelpers_Unknown5(percent, entity->pos1.x, entity->pos1.y, entity->pos2.x, entity->pos2.y, entity->pos3.x, entity->pos3.y,
                                            entity->pos4.x, entity->pos4.y);
    if (!percent)
        entity->position = entity->pos1;

    if (entity->timer < entity->field_84 - 8) {
        if (entity->alpha < 256)
            entity->alpha += 32;
    }
    else {
        entity->alpha -= 32;
    }
    if (entity->timer >= entity->field_84) {
        destroyEntity(entity);
        --Fireflies->field_8;
    }
    RSDK.ProcessAnimation(&entity->animator);
    ++entity->timer;
}

#if RETRO_INCLUDE_EDITOR
void Fireflies_EditorDraw(void)
{
    RSDK_THIS(Fireflies);
    entity->drawFX = FX_FLIP;

    Fireflies_Draw();
}

void Fireflies_EditorLoad(void) { Fireflies->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Fireflies.bin", SCOPE_STAGE); }
#endif

void Fireflies_Serialize(void) {}
