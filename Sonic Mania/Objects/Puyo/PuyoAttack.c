#include "SonicMania.h"

ObjectPuyoAttack *PuyoAttack;

void PuyoAttack_Update(void)
{
    RSDK_THIS(PuyoAttack);

    if (entity->delay > 0) {
        entity->delay--;
        if (entity->delay)
            return;
        entity->visible = true;
    }

    RSDK.ProcessAnimation(&entity->animator);
    int angle = RSDK.ATan2((entity->targetPos.x - entity->position.x) >> 16, (entity->targetPos.y - entity->position.y) >> 16);
    int rot   = 2 * angle;
    int val   = rot - entity->rotation;

    if (abs(rot - entity->rotation) >= abs(val - 0x200)) {
        if (abs(val - 0x200) < abs(val + 0x200))
            entity->rotation += ((val - 512) >> 3);
        else
            entity->rotation += ((val + 512) >> 3);
    }
    else if (abs(rot - entity->rotation) >= abs(val + 0x200))
        entity->rotation += ((val + 512) >> 3);
    else
        entity->rotation += (val >> 3);

    entity->rotation &= 0x1FF;
    entity->position.x += entity->radius * RSDK.Cos512(entity->rotation);
    entity->position.y += entity->radius * RSDK.Sin512(entity->rotation);
    if (entity->radius < 1024)
        entity->radius += 8;
    if (entity->position.y < entity->targetPos.y) {
        // PuyoMatch_Unknown1(entity->playerID, entity->field_64);
        destroyEntity(entity);
    }
}

void PuyoAttack_LateUpdate(void) {}

void PuyoAttack_StaticUpdate(void) {}

void PuyoAttack_Draw(void)
{
    RSDK_THIS(PuyoAttack);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PuyoAttack_Create(void *data)
{
    RSDK_THIS(PuyoAttack);
    if (!RSDK_sceneInfo->inEditor) {
        entity->delay     = 16;
        entity->radius    = 512;
        entity->drawOrder = Zone->hudDrawOrder;
        entity->playerID  = voidToInt(data);
        if (!voidToInt(data))
            entity->rotation = 64;
        else
            entity->rotation = 320;
        entity->active = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(PuyoAttack->aniFrames, entity->playerID ^ 1, &entity->animator, true, 0);
    }
}

void PuyoAttack_StageLoad(void)
{
    PuyoAttack->aniFrames = RSDK.LoadSpriteAnimation("Puyo/Combos.bin", SCOPE_STAGE);
    PuyoAttack->sfxAttack = RSDK.GetSFX("Puyo/Attack.wav");
}

#if RETRO_INCLUDE_EDITOR
void PuyoAttack_EditorDraw(void) {}

void PuyoAttack_EditorLoad(void) {}
#endif

void PuyoAttack_Serialize(void) {}
