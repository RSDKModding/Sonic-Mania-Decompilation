#include "SonicMania.h"

ObjectReagent *Reagent;

void Reagent_Update(void)
{
    RSDK_THIS(Reagent);
    StateMachine_Run(entity->state);
}

void Reagent_LateUpdate(void) {}

void Reagent_StaticUpdate(void) {}

void Reagent_Draw(void)
{
    RSDK_THIS(Reagent);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Reagent_Create(void *data)
{
    RSDK_THIS(Reagent);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x2000000;
        entity->inkEffect     = INK_ALPHA;
        entity->alpha         = 0xC0;
#if RETRO_USE_PLUS
        entity->velocity.x = RSDK.Random(-0xC000, 0xC000, &Zone->randKey);
#else
        entity->velocity.x = RSDK.Rand(-0xC000, 0xC000);
#endif
        entity->type          = voidToInt(data);
        entity->state         = Reagent_Unknown1;

        switch (entity->type) {
            case 0:
                entity->r = 0;
                entity->g = 8;
                entity->b = 192;
                break;
            case 1:
                entity->r = 24;
                entity->g = 144;
                entity->b = 0;
                break;
            case 2:
                entity->r = 0;
                entity->g = 128;
                entity->b = 176;
                break;
        }
#if RETRO_USE_PLUS
        RSDK.SetSpriteAnimation(Reagent->aniFrames, entity->type + 1, &entity->animator, true, RSDK.Random(0, 2, &Zone->randKey));
#else
        RSDK.SetSpriteAnimation(Reagent->aniFrames, entity->type + 1, &entity->animator, true, RSDK.Rand(0, 2));
#endif
    }
}

void Reagent_StageLoad(void)
{
    Reagent->aniFrames     = RSDK.LoadSpriteAnimation("CPZ/Syringe.bin", SCOPE_STAGE);
    Reagent->hitbox.left   = -0x10;
    Reagent->hitbox.top    = -0x02;
    Reagent->hitbox.right  = 0x20;
    Reagent->hitbox.bottom = 0x02;
    Reagent->sfxLand       = RSDK.GetSFX("Puyo/Land.wav");
}

void Reagent_Unknown1(void)
{
    RSDK_THIS(Reagent);
    entity->position.x += entity->velocity.x;
    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;

    foreach_all(ChemicalPool, chemPool)
    {
        if (RSDK.CheckObjectCollisionTouchBox(entity, &Reagent->hitbox, chemPool, &chemPool->hitbox)) {
            entity->velocity.y >>= 2;
            entity->startX   = entity->position.x;
            entity->state    = Reagent_Unknown2;
            chemPool->active = ACTIVE_NORMAL;
            ChemicalPool_ChangeState(chemPool, entity->type, entity->r, entity->g, entity->b);
            RSDK.PlaySFX(Reagent->sfxLand, 0, 255);
        }
    }

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void Reagent_Unknown2(void)
{
    RSDK_THIS(Reagent);
    entity->velocity.y -= 0x1800;
    entity->alpha -= 4;
    ++entity->timer;
    entity->position.y += entity->velocity.y;
    entity->position.x = (RSDK.Sin256(entity->timer) << 10) + entity->startX;
    if (entity->timer == 64)
        destroyEntity(entity);
}

void Reagent_EditorDraw(void) {}

void Reagent_EditorLoad(void) {}

void Reagent_Serialize(void) {}
