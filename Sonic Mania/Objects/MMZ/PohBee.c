#include "SonicMania.h"

ObjectPohBee *PohBee;

void PohBee_Update(void)
{
    RSDK_THIS(PohBee);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    StateMachine_Run(entity->state);

    PohBee_SetupHitboxes();
    PohBee_CheckPlayerCollisions();
    if (entity->drawOrder != 1 && entity->state != PohBee_State_Setup)
        PohBee_CheckOnScreen();
}

void PohBee_LateUpdate(void) {}

void PohBee_StaticUpdate(void) {}

void PohBee_Draw(void) { PohBee_DrawSprites(); }

void PohBee_Create(void *data)
{
    RSDK_THIS(PohBee);

    entity->visible = true;
    if (entity->planeFilter > 0 && ((uint8)(entity->planeFilter - 1) & 2))
        entity->drawOrder = Zone->drawOrderHigh;
    else
        entity->drawOrder = Zone->drawOrderLow;

    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;
    entity->drawFX        = FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->state         = PohBee_State_Setup;
}

void PohBee_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        PohBee->aniFrames = RSDK.LoadSpriteAnimation("MMZ/PohBee.bin", SCOPE_STAGE);
    PohBee->hitbox.left   = -16;
    PohBee->hitbox.top    = -16;
    PohBee->hitbox.right  = 16;
    PohBee->hitbox.bottom = 16;
    DEBUGMODE_ADD_OBJ(PohBee);
}

void PohBee_DebugSpawn(void)
{
    RSDK_THIS(PohBee);
    EntityPohBee *pohBee = CREATE_ENTITY(PohBee, NULL, entity->position.x, entity->position.y);
    pohBee->direction    = entity->direction;
    pohBee->startDir     = entity->direction;
}

void PohBee_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 2, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);

    RSDK.SetSpriteAnimation(PohBee->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void PohBee_CheckOnScreen(void)
{
    RSDK_THIS(PohBee);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->direction = entity->startDir;
        entity->position  = entity->startPos;
        PohBee_Create(NULL);
    }
}

void PohBee_CheckPlayerCollisions(void)
{
    RSDK_THIS(PohBee);

    foreach_active(Player, player)
    {
        if (entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1)) {
            if (Player_CheckBadnikHit(player, entity, &PohBee->hitbox)) {
                if (entity->drawOrder == 1) {
                    if (Player_CheckBadnikBreak(entity, player, false)) {
                        CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y)->drawOrder = 1;
                        destroyEntity(entity);
                    }
                }
                else {
                    Player_CheckBadnikBreak(entity, player, true);
                }
            }

            for (int32 i = 0; i < entity->spikeCount + 1; ++i) {
                if (Player_CheckCollisionTouch(player, entity, &entity->hitboxes[i])) {
#if RETRO_USE_PLUS
                    int32 storeX       = entity->position.x;
                    int32 storeY       = entity->position.y;
                    entity->position = PohBee_GetSpikePos(i, (entity->drawOrder != 1) + 6);
                    if (!Player_CheckMightyUnspin(768, player, 2, &player->uncurlTimer)) {
#endif
                        Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
                    }
                    entity->position.x = storeX;
                    entity->position.y = storeY;
#endif
                }
            }
        }
    }
}

void PohBee_DrawSprites(void)
{
    RSDK_THIS(PohBee);
    Vector2 drawPos;
    Vector2 drawPos2;

    entity->drawFX = FX_FLIP;

    int32 offsetY   = 16;
    int32 shift = 7;
    if (entity->drawOrder == 1) {
        offsetY   = 15;
        shift = 6;
        entity->drawFX |= FX_SCALE;
    }

    if (entity->animator1.animationID != 1)
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    drawPos2.x = entity->position.x;
    drawPos2.y = entity->position.y + (16 << offsetY);
    entity->drawFX |= FX_ROTATE;

    int32 id = 4;
    for (int32 i = 0; i < 10; ++i) {
        drawPos = drawPos2;
        drawPos.x += (id * RSDK.Cos512(entity->field_70)) << shift;
        drawPos.y += (id * RSDK.Sin512(entity->field_70)) << shift;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);
        id += 8;
    }

    drawPos = PohBee_GetSpikePos(0, shift);
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);
    if (entity->spikeCount == 1) {
        id = 4;
        for (int32 i = 0; i < 4; ++i) {
            drawPos = drawPos2;
            drawPos.x += (id * RSDK.Cos512(entity->field_74)) << shift;
            drawPos.y += (id * RSDK.Sin512(entity->field_74)) << shift;
            RSDK.DrawSprite(&entity->animator3, &drawPos, false);
            id += 8;
        }
        drawPos = PohBee_GetSpikePos(1, shift);
        RSDK.DrawSprite(&entity->animator4, &drawPos, false);
    }
}

Vector2 PohBee_GetSpikePos(uint8 spikeID, uint8 shift)
{
    RSDK_THIS(PohBee);

    Vector2 pos = entity->position;

    int32 y = entity->position.y;
    if (entity->drawOrder == 1)
        y += 0x100000;
    else
        y += 0x80000;


    switch (spikeID) {
        case 0:
            pos.x += ((84 * RSDK.Cos512(entity->field_70)) << shift);
            pos.y = y + ((84 * RSDK.Sin512(entity->field_70)) << shift);
            break;
        case 1:
            pos.x += ((36 * RSDK.Cos512(entity->field_74)) << shift);
            pos.y = y + ((4 * (9 * RSDK.Sin512(entity->field_74))) << shift);
            break;
    }

    return pos;
}

void PohBee_SetupHitboxes(void)
{
    RSDK_THIS(PohBee);

    for (int32 i = 0; i < 2; ++i) {
        Vector2 pos = PohBee_GetSpikePos(i, 7);
        int32 x       = (pos.x - entity->position.x) >> 16;
        int32 y       = (pos.y - entity->position.y) >> 16;

        Hitbox hitbox;
        hitbox.left   = x - 12;
        hitbox.right  = x + 12;
        hitbox.bottom = y + 12;
        if (entity->direction == FLIP_X) {
            hitbox.left  = 12 - x;
            hitbox.right = -hitbox.right;
        }
        hitbox.top          = y - 12;
        entity->hitboxes[i] = hitbox;
    }
}

void PohBee_State_Setup(void)
{
    RSDK_THIS(PohBee);

    if (entity->drawOrder != 1)
        entity->active = ACTIVE_NORMAL;
    entity->position  = entity->startPos;
    entity->direction = entity->startDir;
    entity->field_70  = 128;
    entity->field_74  = 172;
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 2, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 3, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 4, &entity->animator4, true, 0);
    entity->state = PohBee_State_Move;
    PohBee_State_Move();
}

void PohBee_State_Move(void)
{
    RSDK_THIS(PohBee);

    int32 distX = abs(entity->position.x - entity->startPos.x);

    int32 move = (2 * (entity->direction != FLIP_NONE) - 1) << 15;
    if (entity->animator1.animationID == 1) {
        switch (entity->animator1.frameID) {
            case 1: move >>= 1; break;
            case 2: move = 0; break;
            case 3: move = (-0x8000 * (2 * (entity->direction != FLIP_NONE) - 1)) >> 1; break;
            case 4: move = -0x8000 * (2 * (entity->direction != FLIP_NONE) - 1); break;
            default: break;
        }
    }
    entity->position.x += move;

    if (distX < entity->amplitude.x) {
        entity->field_7C = 0;
    }
    else if (!entity->animator1.animationID && !entity->field_7C) {
        RSDK.SetSpriteAnimation(PohBee->aniFrames, 1, &entity->animator1, true, 0);
        entity->field_7C = 1;
    }
    if (entity->animator1.animationID == 1 && entity->animator1.frameID == entity->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(PohBee->aniFrames, 0, &entity->animator1, true, 0);
        entity->direction = entity->direction == FLIP_NONE;
    }

    entity->angle      = (entity->angle + 8) & 0x1FF;
    entity->position.y = entity->startPos.y + (RSDK.Sin512(entity->angle) << 7) * (entity->amplitude.y >> 16);

    entity->field_68 = (entity->field_68 + 9) & 0x7FF;
    entity->field_70 = (112 * ((RSDK.Sin512(entity->field_68 >> 2) >> 2) + 136)) >> 7;

    entity->field_6C = (entity->field_6C + 10) & 0x7FF;
    entity->field_74 = (112 * ((RSDK.Sin512(entity->field_6C >> 2) >> 2) + 144)) >> 7;
}

void PohBee_EditorDraw(void) { PohBee_DrawSprites(); }

void PohBee_EditorLoad(void)
{
    PohBee->aniFrames = RSDK.LoadSpriteAnimation("MMZ/PohBee.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PohBee, planeFilter);
    RSDK_ENUM_VAR(PLANEFILTER_NONE);
    RSDK_ENUM_VAR(PLANEFILTER_A);
    RSDK_ENUM_VAR(PLANEFILTER_B);
}

void PohBee_Serialize(void)
{
    RSDK_EDITABLE_VAR(PohBee, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PohBee, VAR_UINT8, spikeCount);
    RSDK_EDITABLE_VAR(PohBee, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(PohBee, VAR_ENUM, planeFilter);
}
