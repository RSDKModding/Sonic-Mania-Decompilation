#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectSSZEggman *SSZEggman;

void SSZEggman_Update(void)
{
    RSDK_THIS(SSZEggman);
    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animator1);
}

void SSZEggman_LateUpdate(void) {}

void SSZEggman_StaticUpdate(void) {}

void SSZEggman_Draw(void)
{
    RSDK_THIS(SSZEggman);

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x += entity->offset.x;
    drawPos.y += entity->offset.y;

    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator4, NULL, false);
}

void SSZEggman_Create(void *data)
{
    RSDK_THIS(SSZEggman);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->drawFX        = FX_FLIP;
        entity->alpha         = 64;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 2, &entity->animator1, true, 0);
        if (data) {
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 6, &entity->animator4, true, 0);
        }
        else {
            entity->offset.y = -0x100000;
            entity->state    = SSZEggman_Unknown1;
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 4, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 5, &entity->animator3, true, 0);
        }
    }
}

void SSZEggman_StageLoad(void) { SSZEggman->aniFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanSSZ.bin", SCOPE_STAGE); }

void SSZEggman_Unknown1(void)
{
    RSDK_THIS(SSZEggman);

    foreach_all(PhantomRuby, ruby) { entity->ruby = ruby; }
    entity->rubyPos.x = entity->position.x;
    entity->rubyPos.y = entity->position.y;
    entity->state     = SSZEggman_Unknown2;
}

void SSZEggman_Unknown2(void)
{
    RSDK_THIS(SSZEggman);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 9) + entity->rubyPos.y) & 0xFFFF0000;

    if (entity->ruby) {
        entity->ruby->position.x = entity->position.x - 0xC0000;
        entity->ruby->position.y = entity->position.y - 0x100000;
    }

    foreach_active(MetalSonic, metal)
    {
        /*if (metal->state == MetalSonic_Unknown33) {
            entity->speed = -0x28000;
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 3, &entity->animator1, true, 0);
            entity->state = SSZEggman_Unknown3;
            if (entity->ruby) {
                entity->ruby->state      = PhantomRuby_Unknown6;
                entity->ruby->velocity.x = -0x40000;
                entity->ruby->velocity.y = -0x40000;
            }
        }*/
    }
}

void SSZEggman_Unknown3(void)
{
    RSDK_THIS(SSZEggman);

    entity->speed += 0x3800;
    entity->offset.y += entity->speed;

    if (entity->offset.y >= 0) {
        entity->offset.y = 0;
        entity->speed    = 0;
        RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 0, &entity->animator1, true, 0);
        entity->state = SSZEggman_Unknown4;
    }

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 9) + entity->rubyPos.y) & 0xFFFF0000;
}

void SSZEggman_Unknown4(void)
{
    RSDK_THIS(SSZEggman);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 9) + entity->rubyPos.y) & 0xFFFF0000;

    if (++entity->timer == 120)
        RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 1, &entity->animator1, true, 0);
    if (entity->timer == 180) {
        entity->timer     = 0;
        entity->direction = FLIP_X;
        entity->active    = ACTIVE_NORMAL;
        entity->state     = SSZEggman_Unknown5;
    }

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 9) + entity->rubyPos.y) & 0xFFFF0000;
}

void SSZEggman_Unknown5(void)
{
    RSDK_THIS(SSZEggman);

    entity->velocity.x += 0x1000;
    entity->position.x += entity->velocity.x;
    entity->rubyPos.y -= 0x8000;
    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 9) + entity->rubyPos.y) & 0xFFFF0000;

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void SSZEggman_EditorDraw(void)
{
    RSDK_THIS(SSZEggman);
    RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 2, &entity->animator1, true, 0);
    entity->offset.y = -0x100000;
    RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 4, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 5, &entity->animator3, true, 0);

    SSZEggman_Draw();
}

void SSZEggman_EditorLoad(void) { SSZEggman->aniFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanSSZ.bin", SCOPE_STAGE); }
#endif

void SSZEggman_Serialize(void) {}
#endif
