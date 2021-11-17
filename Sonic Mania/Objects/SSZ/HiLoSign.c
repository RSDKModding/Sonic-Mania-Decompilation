#include "SonicMania.h"

ObjectHiLoSign *HiLoSign;

void HiLoSign_Update(void)
{
    RSDK_THIS(HiLoSign);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &HiLoSign->hitboxes[entity->type])) {
            int velocity = 0;
            if (entity->type & 1)
                velocity = player->velocity.x;
            else
                velocity = player->velocity.y;

            int speed = abs((velocity >> 7) + (velocity >> 6));
            if (speed > entity->spinSpeed && speed >= 0xC00) {
                entity->spinSpeed = speed & 0xFFFFFF80;
                entity->active    = ACTIVE_NORMAL;
                entity->state     = HiLoSign_State_Spinning;
                entity->direction = velocity < 0;
            }
        }
    }
    StateMachine_Run(entity->state);
}

void HiLoSign_LateUpdate(void) {}

void HiLoSign_StaticUpdate(void) {}

void HiLoSign_Draw(void)
{
    RSDK_THIS(HiLoSign);
    StateMachine_Run(entity->stateDraw);
}

void HiLoSign_Create(void *data)
{
    RSDK_THIS(HiLoSign);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(HiLoSign->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(HiLoSign->aniFrames, 1, &entity->animator2, true, 0);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->drawFX        = FX_SCALE;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->scale.x       = 0x200;
        entity->scale.y       = 0x200;
        entity->state         = HiLoSign_State_Spinning;

        switch (entity->type) {
            case 0:
                entity->animator2.frameID = 1;
                entity->stateDraw         = HiLoSign_StateDraw_Horizontal;
                break;
            case 1: entity->stateDraw = HiLoSign_StateDraw_Vertical; break;
        }
    }
}

void HiLoSign_StageLoad(void)
{
    HiLoSign->aniFrames          = RSDK.LoadSpriteAnimation("SSZ/HiLoSign.bin", SCOPE_STAGE);
    HiLoSign->hitboxes[0].left   = -24;
    HiLoSign->hitboxes[0].top    = -6;
    HiLoSign->hitboxes[0].right  = 24;
    HiLoSign->hitboxes[0].bottom = 6;
    HiLoSign->hitboxes[1].left   = -6;
    HiLoSign->hitboxes[1].top    = -24;
    HiLoSign->hitboxes[1].right  = 6;
    HiLoSign->hitboxes[1].bottom = 24;
    HiLoSign->sfxSignPost = RSDK.GetSFX("Global/SignPost.wav");
    DEBUGMODE_ADD_OBJ(HiLoSign);
}

void HiLoSign_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(HiLoSign, NULL, entity->position.x, entity->position.y);
}

void HiLoSign_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(HiLoSign->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void HiLoSign_State_Spinning(void)
{
    RSDK_THIS(HiLoSign);

    if (entity->spinSpeed) {
        if (entity->direction)
            entity->angle -= entity->spinSpeed;
        else
            entity->angle += entity->spinSpeed;

        entity->spinSpeed -= 64;
        if (entity->spinSpeed < 256) {
            if (entity->direction) {
                if ((entity->angle & 0xFFFF) < 0x4000)
                    entity->direction ^= 1;
            }
            else if ((entity->angle & 0xFFFF) > 0xC000) {
                entity->direction = FLIP_X;
            }
            entity->spinSpeed = 0;
            entity->state    = HiLoSign_State_Spinning2;
        }
    }

    entity->rotation = (entity->angle >> 8) & 0x1FF;
}

void HiLoSign_State_Spinning2(void)
{
    RSDK_THIS(HiLoSign);

    int prevAng = entity->angle & 0xFFFF0000;
    if (entity->direction)
        entity->angle -= entity->spinSpeed;
    else
        entity->angle += entity->spinSpeed;

    entity->spinSpeed -= 32;
    if ((entity->angle & 0xFFFF0000) != prevAng) {
        entity->active   = ACTIVE_BOUNDS;
        entity->spinSpeed = 0;
        entity->state    = HiLoSign_State_Spinning;
    }
    entity->rotation = (entity->angle >> 8) & 0x1FF;
}

void HiLoSign_StateDraw_Horizontal(void)
{
    RSDK_THIS(HiLoSign);
    Vector2 drawPos;

    drawPos.x       = entity->position.x;
    entity->drawFX  = FX_SCALE;
    entity->scale.y = abs(RSDK.Cos512(entity->rotation)) + 1;
    int scaleY      = abs(RSDK.Sin512(entity->rotation)) + 1;

    switch (entity->rotation >> 7) {
        case 0:
        case 2:
            drawPos.y = entity->position.y + (scaleY << 9);
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
            drawPos.y += -2048 * entity->scale.y - (scaleY << 9);
            break;
        case 1:
        case 3:
            drawPos.y = entity->position.y - (scaleY << 9);
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
            drawPos.y += (scaleY + 4 * (entity->scale.y - 16)) << 9;
            break;
        default: break;
    }
    entity->scale.y = scaleY;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
}

void HiLoSign_StateDraw_Vertical(void)
{
    RSDK_THIS(HiLoSign);
    Vector2 drawPos;

    drawPos.y       = entity->position.y;
    entity->drawFX  = FX_SCALE;
    entity->scale.x = abs(RSDK.Cos512(entity->rotation)) + 1;
    int scaleX      = abs(RSDK.Sin512(entity->rotation)) + 1;

    switch (entity->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x = entity->position.x + (scaleX << 9);
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
            drawPos.x += -0xF00 * entity->scale.x - (scaleX << 9);
            break;
        case 1:
        case 3:
            drawPos.x = entity->position.x - (scaleX << 9);
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
            drawPos.x += ((scaleX - 64) << 9) + 0xF00 * entity->scale.x;
            break;
        default: break;
    }
    entity->scale.x = scaleX;

    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
}

void HiLoSign_EditorDraw(void)
{
    RSDK_THIS(HiLoSign);
    RSDK.SetSpriteAnimation(HiLoSign->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void HiLoSign_EditorLoad(void) { HiLoSign->aniFrames = RSDK.LoadSpriteAnimation("SSZ/HiLoSign.bin", SCOPE_STAGE); }

void HiLoSign_Serialize(void) { RSDK_EDITABLE_VAR(HiLoSign, VAR_UINT8, type); }
