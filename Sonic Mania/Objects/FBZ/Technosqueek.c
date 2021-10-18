#include "SonicMania.h"

ObjectTechnosqueek *Technosqueek;

void Technosqueek_Update(void)
{
    RSDK_THIS(Technosqueek);
    StateMachine_Run(entity->state);
}

void Technosqueek_LateUpdate(void) {}

void Technosqueek_StaticUpdate(void) {}

void Technosqueek_Draw(void)
{
    RSDK_THIS(Technosqueek);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (entity->type) {
        if (entity->direction & FLIP_Y)
            drawPos.y = entity->position.y - entity->field_6C;
        else
            drawPos.y = entity->field_6C + entity->position.y;
    }
    else {
        if (entity->direction & FLIP_X)
            drawPos.x = entity->position.x - entity->field_6C;
        else
            drawPos.x = entity->field_6C + entity->position.x;
    }
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Technosqueek_Create(void *data)
{
    RSDK_THIS(Technosqueek);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->startDir      = entity->direction;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (!entity->type) {
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 2, &entity->animator2, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 3, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 5, &entity->animator2, true, 0);
        }
        entity->state = Technosqueek_State_Unknown1;
        entity->distance *= 2;
    }
}

void Technosqueek_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        Technosqueek->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Technosqueek.bin", SCOPE_STAGE);
    Technosqueek->hitbox.left   = -4;
    Technosqueek->hitbox.top    = -4;
    Technosqueek->hitbox.right  = 4;
    Technosqueek->hitbox.bottom = 4;
    DEBUGMODE_ADD_OBJ(Technosqueek);
}

void Technosqueek_DebugSpawn(void)
{
    RSDK_THIS(Technosqueek);
    CREATE_ENTITY(Technosqueek, NULL, entity->position.x, entity->position.y);
}

void Technosqueek_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Technosqueek_HandlePlayerInteractions(void)
{
    RSDK_THIS(Technosqueek);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Technosqueek->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Technosqueek_CheckOnScreen(void)
{
    RSDK_THIS(Technosqueek);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->distance >>= 1;
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Technosqueek_Create(NULL);
    }
}

void Technosqueek_State_Unknown1(void)
{
    RSDK_THIS(Technosqueek);

    entity->active = ACTIVE_NORMAL;
    bool32 flag    = false;
    if (entity->type)
        flag = !(entity->direction & FLIP_Y);
    else
        flag = !(entity->direction & FLIP_X);
    if (flag) {
        entity->field_64  = 0x2000;
        entity->field_68  = entity->speed << 14;
        entity->groundVel = -entity->field_68;
    }
    else {
        entity->groundVel = entity->speed << 14;
        entity->field_64  = -0x2000;
        entity->field_68  = -entity->groundVel;
    }
    entity->field_60 = entity->distance >> 1;
    entity->flag     = true;
    entity->field_6C = 0;
    if (entity->type) {
        entity->state = Technosqueek_State_Unknown4;
        Technosqueek_State_Unknown4();
    }
    else {
        entity->state = Technosqueek_State_Unknown2;
        Technosqueek_State_Unknown2();
    }
}

void Technosqueek_State_Unknown2(void)
{
    RSDK_THIS(Technosqueek);

    RSDK_sceneInfo->entity->position.x += entity->groundVel;
    if (entity->field_60) {
        entity->field_60--;
    }
    else {
        entity->groundVel += entity->field_64;
        if (entity->groundVel == entity->field_68) {
            entity->field_60 = entity->distance;
            entity->field_68 = -entity->field_68;
            entity->field_64 = -entity->field_64;
            entity->flag     = true;
        }
    }

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->flag) {
        if (abs(entity->groundVel) <= 0x10000 && entity->animator1.animationID != 1)
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 1, &entity->animator1, true, 0);
        if (entity->animator1.frameID == 1) {
            entity->field_6C = -0x80000;
        }
        else if (entity->animator1.frameID == 2) {
            entity->field_6C = -0x120000;
        }
    }
    else {
        if (entity->field_6C < 0) {
            entity->field_6C += abs(entity->groundVel - abs(entity->field_64));
        }
        else {
            entity->field_6C = 0;
        }
    }
    if (!entity->groundVel)
        entity->state = Technosqueek_State_Unknown3;
    Technosqueek_HandlePlayerInteractions();
    Technosqueek_CheckOnScreen();
}

void Technosqueek_State_Unknown3(void)
{
    RSDK_THIS(Technosqueek);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        entity->flag = false;
        RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 0, &entity->animator1, true, 0);
        entity->state = Technosqueek_State_Unknown2;
        entity->direction ^= 1u;
    }
    Technosqueek_HandlePlayerInteractions();
    Technosqueek_CheckOnScreen();
}

void Technosqueek_State_Unknown4(void)
{
    RSDK_THIS(Technosqueek);

    entity->position.y += entity->groundVel;
    if (entity->field_60) {
        entity->field_60--;
    }
    else {
        entity->groundVel += entity->field_64;
        if (entity->groundVel == entity->field_68) {
            entity->field_60 = entity->distance;
            entity->field_68 = -entity->field_68;
            entity->field_64 = -entity->field_64;
            entity->flag     = true;
        }
    }

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->flag) {
        if (abs(entity->groundVel) <= 0x10000 && entity->animator1.animationID != 4)
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 4, &entity->animator1, true, 0);
        if (entity->animator1.frameID == 1) {
            entity->field_6C = -0x80000;
        }
        else if (entity->animator1.frameID == 2) {
            entity->field_6C = -0x120000;
        }
    }
    else {
        if (entity->field_6C < 0) {
            entity->field_6C += abs(entity->groundVel - abs(entity->field_64));
        }
        else {
            entity->field_6C = 0;
        }
    }
    if (!entity->groundVel)
        entity->state = Technosqueek_State_Unknown5;
    Technosqueek_HandlePlayerInteractions();
    Technosqueek_CheckOnScreen();
}

void Technosqueek_State_Unknown5(void)
{
    RSDK_THIS(Technosqueek);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        entity->flag = false;
        RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 3, &entity->animator1, true, 0);
        entity->state = Technosqueek_State_Unknown4;
        entity->direction ^= FLIP_Y;
    }
    Technosqueek_HandlePlayerInteractions();
    Technosqueek_CheckOnScreen();
}

void Technosqueek_State_Unknown6(void)
{
    RSDK_THIS(Technosqueek);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8)) {
        entity->startPos.x = entity->position.x;
        entity->startPos.y = entity->position.y;
        entity->startDir   = entity->direction;
        entity->velocity.y = 0;
        entity->speed      = 16;
        entity->state      = Technosqueek_State_Unknown1;
    }
    RSDK.ProcessAnimation(&entity->animator2);
    Technosqueek_HandlePlayerInteractions();
}

void Technosqueek_EditorDraw(void) {}

void Technosqueek_EditorLoad(void) {}

void Technosqueek_Serialize(void)
{
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT8, speed);
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT16, distance);
}
