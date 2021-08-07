#include "SonicMania.h"

ObjectNewspaper *Newspaper;

void Newspaper_Update(void)
{
    RSDK_THIS(Newspaper);
#if RETRO_USE_PLUS
    StateMachine_Run(entity->state);
#else
    Platform_Update();
#endif
}

void Newspaper_LateUpdate(void) {}

void Newspaper_StaticUpdate(void) {}

void Newspaper_Draw(void)
{
    RSDK_THIS(Newspaper);
#if RETRO_USE_PLUS
    RSDK.DrawSprite(&entity->animator, NULL, false);
#else
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
#endif
}

void Newspaper_Create(void *data)
{
    RSDK_THIS(Newspaper);
#if RETRO_USE_PLUS
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        RSDK.SetSpriteAnimation(Newspaper->aniFrames, 1, &entity->animator, true, entity->type);
        switch (entity->type) {
            case 0:
            case 1:
                entity->state     = Newspaper_HandleInteractions;
                entity->drawOrder = Zone->playerDrawLow;
                break;
            case 2:
            case 3: entity->drawOrder = Zone->drawOrderLow; break;
            default: break;
        }

        entity->hitbox.left   = -16;
        entity->hitbox.top    = -15;
        entity->hitbox.right  = 16;
        entity->hitbox.bottom = 16;
    }
#else
    entity->collisionType = 1;
    entity->type          = 5;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Newspaper->aniFrames, 0, &entity->animator, true, 0);
#endif
}

void Newspaper_StageLoad(void)
{
#if RETRO_USE_PLUS
    Newspaper->aniFrames     = RSDK.LoadSpriteAnimation("PSZ1/Newspaper.bin", SCOPE_STAGE);
    Newspaper->sfxPaperStack = RSDK.GetSFX("PSZ/PaperStack.wav");
#else
    if (RSDK.CheckStageFolder("PSZ1"))
        Newspaper->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Newspaper.bin", SCOPE_STAGE);
#endif
}

#if RETRO_USE_PLUS
void Newspaper_HandleInteractions(void)
{
    RSDK_THIS(Newspaper);

    if (entity->timer <= 0) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                EntityDebris *debris =
                    CREATE_ENTITY(Debris, NULL, entity->position.x + RSDK.Rand(-0x80000, 0x80000), entity->position.y + RSDK.Rand(-0x80000, 0x80000));
                debris->state      = Debris_State_Fall;
                debris->gravity    = 0x400;
                debris->velocity.x = RSDK.Rand(0, 0x20000);
                debris->timer      = 240;
                entity->timer      = 2;
                if (debris->position.x < entity->position.x)
                    debris->velocity.x = -debris->velocity.x;
                debris->drawOrder = Zone->drawOrderLow;
                RSDK.SetSpriteAnimation(Newspaper->aniFrames, RSDK.Rand(0, 2) + 2, &debris->animator, true, RSDK.Rand(0, 6));

                if (Newspaper->sfxPaperStack) {
                    RSDK.PlaySFX(Newspaper->sfxPaperStack, false, 255);
                    Newspaper->sfxPaperStack = 0;
                }
            }
        }
    }
    else {
        entity->timer--;
    }
}
#endif

void Newspaper_EditorDraw(void) {}

void Newspaper_EditorLoad(void) {}

void Newspaper_Serialize(void)
{
#if RETRO_USE_PLUS
    RSDK_EDITABLE_VAR(Newspaper, VAR_UINT8, type);
#else
    RSDK_EDITABLE_VAR(Newspaper, VAR_ENUM, node);
    RSDK_EDITABLE_VAR(Newspaper, VAR_ENUM, childCount);
#endif
}
