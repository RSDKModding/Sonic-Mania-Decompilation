#include "../SonicMania.h"

ObjectSpinSign *SpinSign;

void SpinSign_Update(void)
{
    RSDK_THIS(SpinSign);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &SpinSign->hitboxes[entity->type])) {
            int vel = 0;
            if (entity->type & 1)
                vel = player->velocity.x;
            else
                vel = player->velocity.y;

            int val = (vel >> 7) + (vel >> 6);
            if (abs(val) > entity->timer && val >= 0xC00) {
                entity->timer     = val & -0x80;
                entity->active    = ACTIVE_NORMAL;
                entity->state     = SpinSign_Unknown1;
                entity->direction = vel < 0;
            }
        }
    }
    StateMachine_Run(entity->state);
}

void SpinSign_LateUpdate(void) {}

void SpinSign_StaticUpdate(void) {}

void SpinSign_Draw(void)
{
    RSDK_THIS(SpinSign);
    StateMachine_Run(entity->stateDraw);
}

void SpinSign_Create(void *data)
{
    RSDK_THIS(SpinSign);
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SpinSign->aniFrames, 3, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(SpinSign->aniFrames, 4, &entity->animator3, true, 0);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->drawFX        = FX_SCALE;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->scale.x       = 0x200;
        entity->scale.y       = 0x200;
        entity->state         = SpinSign_Unknown1;
        switch (entity->type) {
            case 0:
                entity->animator3.frameID = 1;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 0, &entity->animator2, true, 0);
                entity->stateDraw = SpinSign_Unknown3;
                break;
            case 1:
                entity->animator3.frameID = 0;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 0, &entity->animator2, true, 0);
                entity->stateDraw = SpinSign_Unknown4;
                break;
            case 2:
                entity->animator3.frameID = 1;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 5, &entity->animator2, true, 0);
                entity->stateDraw = SpinSign_Unknown5;
                break;
            case 3:
                entity->animator3.frameID = 2;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 5, &entity->animator2, true, 0);
                entity->stateDraw = SpinSign_Unknown6;
                break;
            default: break;
        }
    }
}

void SpinSign_StageLoad(void)
{
    SpinSign->aniFrames          = RSDK.LoadSpriteAnimation("SPZ1/SpinSign.bin", SCOPE_STAGE);
    SpinSign->hitboxes[0].left   = -24;
    SpinSign->hitboxes[0].top    = -6;
    SpinSign->hitboxes[0].right  = 24;
    SpinSign->hitboxes[0].bottom = 6;
    SpinSign->hitboxes[1].left   = -6;
    SpinSign->hitboxes[1].top    = -24;
    SpinSign->hitboxes[1].right  = 6;
    SpinSign->hitboxes[1].bottom = 24;
    SpinSign->hitboxes[2].left   = -24;
    SpinSign->hitboxes[2].top    = -6;
    SpinSign->hitboxes[2].right  = 24;
    SpinSign->hitboxes[2].bottom = 6;
    SpinSign->hitboxes[3].left   = -6;
    SpinSign->hitboxes[3].top    = -40;
    SpinSign->hitboxes[3].right  = 6;
    SpinSign->hitboxes[3].bottom = 40;
    DEBUGMODE_ADD_OBJ(SpinSign);
    SpinSign->sfxSignPost = RSDK.GetSFX("Global/SignPost.wav");
}

void SpinSign_DebugSpawn(void)
{
    RSDK_THIS(SpinSign);
    RSDK.CreateEntity(SpinSign->objectID, 0, entity->position.x, entity->position.y);
}
void SpinSign_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 4, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, 0);
    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 3, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, 0);
    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 4, &DebugMode->animator, true, 2);
    RSDK.DrawSprite(&DebugMode->animator, 0, 0);
}

void SpinSign_Unknown1(void)
{
    RSDK_THIS(SpinSign);

    if (entity->timer) {
        if (entity->direction)
            entity->angle -= entity->timer;
        else
            entity->angle += entity->timer;

        entity->timer -= 0x40;
        if (entity->timer < 256) {
            if (entity->direction) {
                if (entity->angle < 0x4000)
                    entity->direction ^= FLIP_X;
            }
            else if (entity->angle > 0xC000) {
                entity->direction = FLIP_X;
            }
            entity->timer = 0;
            entity->state = SpinSign_Unknown2;
        }
    }
    entity->rotation = (entity->angle >> 8) & 0x1FF;
}
void SpinSign_Unknown2(void)
{
    RSDK_THIS(SpinSign);

    int ang = entity->angle & 0xFFFF0000;
    if (entity->direction)
        entity->angle -= entity->timer;
    else
        entity->angle += entity->timer;

    entity->timer -= 0x20;
    if ((entity->angle & 0xFFFF0000) != ang) {
        entity->active = ACTIVE_BOUNDS;
        entity->timer  = 0;
        entity->state  = SpinSign_Unknown1;
    }
    entity->rotation = (entity->angle >> 8) & 0x1FF;
}

void SpinSign_Unknown3(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    entity->drawFX = FX_SCALE;
    drawPos.x      = entity->position.x;

    Animator *animator = NULL;
    if (entity->rotation <= 128 || entity->rotation >= 384) {
        animator = &entity->animator1;
    }
    else {
        animator = &entity->animator2;
    }
    animator->frameID = (Zone->timer >> 4) & 1;

    entity->scale.y = abs(RSDK.Cos512(entity->rotation)) + 1;
    int scale       = abs(RSDK.Sin512(entity->rotation)) + 1;

    switch (entity->rotation >> 7) {
        case 0:
        case 2:
            drawPos.y = entity->position.y + (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.y += -0xF00 * entity->scale.y - (scale << 9);
            break;
        case 1:
        case 3:
            drawPos.y = entity->position.y - (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.y += ((scale - 64) << 9) + 0xF00 * entity->scale.y;
            break;
        default: break;
    }
    entity->scale.y           = scale;
    entity->animator3.frameID = (animator->frameID + 2) ^ (entity->rotation < 256);
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
}
void SpinSign_Unknown4(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    drawPos.y      = entity->position.y;
    entity->drawFX = FX_SCALE;

    Animator *animator = NULL;
    if (entity->rotation <= 128 || entity->rotation >= 384)
        animator = &entity->animator1;
    else
        animator = &entity->animator2;

    animator->frameID = (Zone->timer >> 4) & 1;

    entity->scale.x   = abs(RSDK.Cos512(entity->rotation)) + 1;
    int scale = abs(RSDK.Sin512(entity->rotation)) + 1;

    switch (entity->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x = entity->position.x + (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.x += -0xF00 * entity->scale.x - (scale << 9);
            break;
        case 1:
        case 3:
            drawPos.x = entity->position.x - (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.x += ((scale - 64) << 9) + 0xF00 * entity->scale.x;
            break;
        default: break;
    }
    entity->scale.x           = scale;
    entity->animator3.frameID = animator->frameID ^ (entity->rotation < 256);
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
}
void SpinSign_Unknown5(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    drawPos.x                 = entity->position.x;
    entity->drawFX            = FX_SCALE;
    entity->animator2.frameID = entity->rotation <= 128 || entity->rotation >= 384;

    entity->scale.y = abs(RSDK.Cos512(entity->rotation)) + 1;
    int scale       = abs(RSDK.Sin512(entity->rotation)) + 1;

    switch (entity->rotation >> 7) {
        case 0:
        case 2:
            drawPos.y = entity->position.y + (scale << 9);
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
            drawPos.y += -0x1400 * entity->scale.y - (scale << 9);
            break;
        case 1:
        case 3:
            drawPos.y = entity->position.y - (scale << 9);
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
            drawPos.y += (scale + 2 * (5 * entity->scale.y - 32)) << 9;
            break;
        default: break;
    }
    entity->scale.y           = scale;
    entity->animator3.frameID = entity->animator2.frameID + 6;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
}
void SpinSign_Unknown6(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    drawPos.y                 = entity->position.y;
    entity->drawFX            = FX_SCALE;
    entity->animator2.frameID = entity->rotation <= 128 || entity->rotation >= 384;

    entity->scale.x = abs(RSDK.Cos512(entity->rotation)) + 1;
    int scale       = abs(RSDK.Sin512(entity->rotation)) + 1;

    switch (entity->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x = entity->position.x + (scale << 9);
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
            drawPos.x += -0xC00 * entity->scale.x - (scale << 9);
            break;
        case 1:
        case 3:
            drawPos.x = entity->position.x - (scale << 9);
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
            drawPos.x += (scale + 2 * (3 * entity->scale.x - 32)) << 9;
            break;
        default: break;
    }
    entity->scale.x           = scale;
    entity->animator3.frameID = entity->animator2.frameID + 4;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
}

void SpinSign_EditorDraw(void) {}

void SpinSign_EditorLoad(void) {}

void SpinSign_Serialize(void) { RSDK_EDITABLE_VAR(SpinSign, VAR_UINT8, type); }
