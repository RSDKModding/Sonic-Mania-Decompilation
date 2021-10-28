#include "SonicMania.h"

ObjectVultron *Vultron;

void Vultron_Update(void)
{
    RSDK_THIS(Vultron);
    StateMachine_Run(entity->state);
}

void Vultron_LateUpdate(void) {}

void Vultron_StaticUpdate(void) {}

void Vultron_Draw(void)
{
    RSDK_THIS(Vultron);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void Vultron_Create(void *data)
{
    RSDK_THIS(Vultron);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible   = true;
        entity->drawOrder = Zone->drawOrderLow;
        entity->drawFX    = FX_FLIP;
        if (data)
            entity->type = voidToInt(data);
        entity->active = ACTIVE_BOUNDS;

        switch (entity->type) {
            case 0:
                entity->updateRange.y = 0x800000;
                entity->updateRange.x = (entity->dist + 16) << 19;
                entity->startPos      = entity->position;
                entity->startDir      = entity->direction;
                RSDK.SetSpriteAnimation(Vultron->aniFrames, 0, &entity->animator1, true, 0);
                entity->state = Vultron_State_Setup;
                break;
            case 1:
                entity->updateRange.x  = 0x800000;
                entity->updateRange.y  = 0x800000;
                entity->drawFX         = FX_ROTATE;
                entity->rotation       = 192 * (RSDK_sceneInfo->createSlot & 1) + 320;
                entity->hitbox.left  = -12;
                entity->hitbox.top  = -8;
                entity->hitbox.right = 12;
                entity->hitbox.bottom = 8;
                RSDK.SetSpriteAnimation(Vultron->aniFrames, 1, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(Vultron->aniFrames, 2, &entity->animator2, true, 0);
                entity->state = Vultron_State2_Unknown;
                break;
        }
    }
}

void Vultron_StageLoad(void)
{
    Vultron->sfxVultron = RSDK.GetSFX("MSZ/Vultron.wav");
    if (RSDK.CheckStageFolder("MSZ"))
        Vultron->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Vultron.bin", SCOPE_STAGE);

    Vultron->hitbox.left  = 0;
    Vultron->hitbox.top  = -64;
    Vultron->hitbox.right = 256;
    Vultron->hitbox.bottom = 128;
    DEBUGMODE_ADD_OBJ(Vultron);
}

void Vultron_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    EntityVultron *vultron = CREATE_ENTITY(Vultron, NULL, entity->position.x, entity->position.y);
    vultron->direction     = entity->direction;
    vultron->startDir      = entity->direction;
    vultron->dist          = 64;
    vultron->updateRange.x = 0x2800000;
}

void Vultron_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Vultron->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Vultron_CheckPlayerCollisions(void)
{
    RSDK_THIS(Vultron);
    Hitbox *hitbox = RSDK.GetHitbox(&entity->animator1, 0);
    int left       = (hitbox->left << 16) + (((hitbox->right - hitbox->left) << 15) & 0xFFFF0000);
    int top        = (hitbox->top << 16) + (((hitbox->bottom - hitbox->top) << 15) & 0xFFFF0000);
    if (entity->direction == FLIP_X)
        left = -left;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, hitbox)) {
            entity->position.x += left;
            entity->position.y += top;
            if (!Player_CheckBadnikBreak(entity, player, true)) {
                entity->position.x -= left;
                entity->position.y -= top;
            }
        }
    }
}

void Vultron_CheckOnScreen(void)
{
    RSDK_THIS(Vultron);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        Vultron_Create(NULL);
    }
}

void Vultron_State_Setup(void)
{
    RSDK_THIS(Vultron);
    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = 0;
    entity->state      = Vultron_State_Unknown1;
    Vultron_State_Unknown1();
}

void Vultron_State_Unknown1(void)
{
    RSDK_THIS(Vultron);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Vultron->hitbox)) {
            entity->velocity.y = -0xD800;
            if (entity->direction == FLIP_NONE)
                entity->velocity.x = 0x8000;
            else
                entity->velocity.x = -0x8000;
            entity->storeY = entity->position.y;
            RSDK.PlaySfx(Vultron->sfxVultron, false, 255);
            entity->state = Vultron_State_Unknown2;
        }
    }

    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State_Unknown2(void)
{
    RSDK_THIS(Vultron);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == 5) {
        RSDK.SetSpriteAnimation(Vultron->aniFrames, 2, &entity->animator2, true, 0);
        entity->state = Vultron_State_Unknown3;
        entity->velocity.x *= 8;
    }
    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State_Unknown3(void)
{
    RSDK_THIS(Vultron);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x1800;

    if (entity->velocity.y <= 0) {
        entity->distance   = entity->dist;
        entity->velocity.y = 0;
        entity->state      = Vultron_State_Unknown4;
    }
    RSDK.ProcessAnimation(&entity->animator2);
    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State_Unknown4(void)
{
    RSDK_THIS(Vultron);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (!--entity->distance) {
        RSDK.SetSpriteAnimation(Vultron->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        entity->state = Vultron_State_Unknown5;
    }
    RSDK.ProcessAnimation(&entity->animator2);
    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State_Unknown5(void)
{
    RSDK_THIS(Vultron);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->direction)
        entity->velocity.x += 0x1800;
    else
        entity->velocity.y -= 0x1800;

    entity->velocity.y -= 0x4000;
    if (entity->velocity.y < -0x38000) {
        entity->velocity.y = -0x38000;
        entity->state      = Vultron_State_Unknown6;
    }
    RSDK.ProcessAnimation(&entity->animator1);
    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State_Unknown6(void)
{
    RSDK_THIS(Vultron);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->direction) {
        entity->velocity.x += 0x1800;
        if (entity->velocity.x > 0)
            entity->velocity.x = 0;
    }
    else {
        entity->velocity.x -= 0x1800;
        if (entity->velocity.x < 0)
            entity->velocity.x = 0;
    }
    entity->velocity.y += 0x1800;
    if (entity->velocity.y > -0xC800) {
        entity->direction  = entity->direction ^ 1;
        entity->position.y = entity->storeY;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = 0x8000;
        else
            entity->velocity.x = -0x8000;
        entity->velocity.y = -0xC800;
        RSDK.SetSpriteAnimation(Vultron->aniFrames, 0, &entity->animator1, true, 0);
        entity->state = Vultron_State_Unknown2;
    }
    RSDK.ProcessAnimation(&entity->animator1);
    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State2_Unknown(void)
{
    RSDK_THIS(Vultron);
    EntityPlayer *playerPtr = Player_GetNearestPlayerX();
    RSDK.ProcessAnimation(&entity->animator2);
    int angle = RSDK.ATan2((playerPtr->position.x - entity->position.x) >> 16, (playerPtr->position.y - entity->position.y) >> 16);

    int rot = 2 * angle - entity->rotation;

    if (abs(2 * angle - entity->rotation) >= abs(rot - 0x200)) {
        if (abs(rot - 0x200) < abs(rot + 0x200)) {
            entity->rotation += ((rot - 0x200) >> 5);
        }
        else {
            entity->rotation += ((rot + 0x200) >> 5);
        }
    }
    else {
        if (abs(2 * angle - entity->rotation) < abs(rot + 0x200)) {
            entity->rotation += (rot >> 5);
        }
        else {
            entity->rotation += ((rot + 0x200) >> 5);
        }
    }

    entity->rotation &= 0x1FF;
    entity->position.x += RSDK.Cos512(entity->rotation) << 9;
    entity->position.y += RSDK.Sin512(entity->rotation) << 9;
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &entity->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

#if RETRO_INCLUDE_EDITOR
void Vultron_EditorDraw(void)
{
    RSDK_THIS(Vultron);
    switch (entity->type) {
        case 0:
            entity->drawFX = FX_NONE;
            RSDK.SetSpriteAnimation(Vultron->aniFrames, 0, &entity->animator1, false, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 2, &entity->animator2, true, 0);
            break;
        case 1:
            entity->drawFX   = FX_ROTATE;
            entity->rotation = 192 * (RSDK_sceneInfo->createSlot & 1) + 320;
            RSDK.SetSpriteAnimation(Vultron->aniFrames, 1, &entity->animator1, false, 0);
            RSDK.SetSpriteAnimation(Vultron->aniFrames, 2, &entity->animator2, false, 0);
            break;
    }

    Vultron_Draw();
}

void Vultron_EditorLoad(void) { Vultron->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Vultron.bin", SCOPE_STAGE); }
#endif

void Vultron_Serialize(void)
{
    RSDK_EDITABLE_VAR(Vultron, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Vultron, VAR_UINT16, dist);
    RSDK_EDITABLE_VAR(Vultron, VAR_UINT8, direction);
}
