#include "SonicMania.h"

ObjectStegway *Stegway = NULL;

void Stegway_Update(void)
{
    RSDK_THIS(Stegway);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    if (entity->dustFlag)
        RSDK.ProcessAnimation(&entity->animator3);

    StateMachine_Run(entity->state);

    Stegway_HandlePlayerInteractions();
    if (entity->state != Stegway_State_Setup)
        Stegway_CheckOnScreen();
}

void Stegway_LateUpdate(void) {}

void Stegway_StaticUpdate(void) {}

void Stegway_Draw(void)
{
    RSDK_THIS(Stegway);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    if (entity->dustFlag)
        RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void Stegway_Create(void *data)
{
    RSDK_THIS(Stegway);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;
    entity->drawFX        = FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->state         = Stegway_State_Setup;
}

void Stegway_StageLoad(void)
{
    //if (RSDK.CheckStageFolder("HPZ"))
        Stegway->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Stegway.bin", SCOPE_STAGE);
    Stegway->hitboxBadnik.left   = -20;
    Stegway->hitboxBadnik.top    = -14;
    Stegway->hitboxBadnik.right  = 20;
    Stegway->hitboxBadnik.bottom = 14;
    Stegway->hitbox2.left        = -96;
    Stegway->hitbox2.top         = -32;
    Stegway->hitbox2.right       = 0;
    Stegway->hitbox2.bottom      = 14;
    Stegway->sfxRev              = RSDK.GetSFX("Stage/Rev.wav");
    Stegway->sfxRelease          = RSDK.GetSFX("Global/Release.wav");
    DEBUGMODE_ADD_OBJ(Stegway);
}

void Stegway_DebugSpawn(void)
{
    RSDK_THIS(Stegway);
    EntityStegway *stegway = CREATE_ENTITY(Stegway, NULL, entity->position.x, entity->position.y);
    stegway->direction     = entity->direction;
    stegway->startDir      = entity->direction;
}

void Stegway_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Stegway_CheckOnScreen(void)
{
    RSDK_THIS(Stegway);
    if (!RSDK.CheckOnScreen(entity, 0) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->direction = entity->startDir;
        entity->position  = entity->startPos;
        Stegway_Create(NULL);
    }
}

void Stegway_HandlePlayerInteractions(void)
{
    RSDK_THIS(Stegway);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Stegway->hitboxBadnik))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Stegway_SetupAnims(char type, bool32 force)
{
    RSDK_THIS(Stegway);
    switch (type) {
        case 0:
            if (force || entity->animator1.animationID)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &entity->animator1, true, 0);
            entity->animator1.animationSpeed = 1;
            entity->animator2.animationSpeed = 6;
            break;
        case 1:
            if (force || entity->animator1.animationID != 1)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 1, &entity->animator1, true, 0);
            entity->animator2.animationSpeed = 16;
            break;
        case 2:
            if (force || entity->animator1.animationID)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &entity->animator1, true, 0);
            if (entity->animator1.frameID == 2)
                entity->animator1.frameID = 1;
            entity->animator1.animationSpeed = 0;
            entity->animator2.animationSpeed = 24;
            break;
        case 3:
            if (force || entity->animator1.animationID)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &entity->animator1, true, 0);
            if (entity->animator1.frameID == 2)
                entity->animator1.frameID = 1;
            entity->animator1.animationSpeed = 0;
            entity->animator2.animationSpeed = 0;
            break;
        default: break;
    }
}

void Stegway_State_Setup(void)
{
    RSDK_THIS(Stegway);

    entity->active = ACTIVE_NORMAL;
    if (entity->direction == FLIP_NONE)
        entity->velocity.x = -0x4000;
    else
        entity->velocity.x = 0x4000;
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 2, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 3, &entity->animator3, true, 0);
    Stegway_SetupAnims(0, true);
    entity->state = Stegway_Unknown6;
    Stegway_Unknown6();
}

void Stegway_Unknown6(void)
{
    RSDK_THIS(Stegway);

    entity->position.x += entity->velocity.x;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Stegway->hitbox2)) {
            entity->state = Stegway_Unknown8;
            Stegway_SetupAnims(3, false);
        }
    }

    bool32 collided = false;
    if (entity->direction)
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0xC0000, 0x100000, 8);
    else
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0xC0000, 0x100000, 8);
    if (!collided) {
        entity->state = Stegway_Unknown7;
        Stegway_SetupAnims(3, false);
        entity->dustFlag = false;
        entity->flag     = false;
    }
    Stegway_CheckOnScreen();
}

void Stegway_Unknown7(void)
{
    RSDK_THIS(Stegway);

    if (entity->timer >= 29) {
        entity->timer = 0;
        entity->state = Stegway_Unknown6;
        Stegway_SetupAnims(0, false);

        entity->direction ^= FLIP_X;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x4000;
        else
            entity->velocity.x = 0x4000;
    }
    else {
        entity->timer++;
    }
}

void Stegway_Unknown8(void)
{
    RSDK_THIS(Stegway);
    if (++entity->timer == 8) {
        entity->timer = 0;
        entity->state = Stegway_Unknown9;
        Stegway_SetupAnims(2, false);
        RSDK.PlaySFX(Stegway->sfxRev, false, 255);
    }
}

void Stegway_Unknown9(void)
{
    RSDK_THIS(Stegway);
    if (++entity->timer == 32) {
        entity->timer = 0;
        entity->state = Stegway_Unknown10;
        EntityDust *dust =
            CREATE_ENTITY(Dust, entity, entity->position.x - 0xA0000 * (2 * (entity->direction != FLIP_NONE) - 1), entity->position.y + 0x100000);
        RSDK.SetSpriteAnimation(Dust->spriteIndex, 2, &dust->animator, true, 0);
        dust->state     = Dust_State_DropDash;
        dust->direction = 1 - entity->direction;
        dust->drawOrder = entity->drawOrder;
        Stegway_SetupAnims(1, false);
        entity->dustFlag = true;
        entity->velocity.x *= 12;
        RSDK.PlaySFX(Stegway->sfxRelease, false, 255);
    }
}

void Stegway_Unknown10(void)
{
    RSDK_THIS(Stegway);

    entity->position.x += entity->velocity.x;
    int val = 2 * (entity->direction != FLIP_NONE) - 1;

    if (!entity->flag) {
        int storeX = entity->position.x;
        int storeY = entity->position.y;
        if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, val << 22, 0x100000, 8))
            entity->flag = true;
        entity->position.x = storeX;
        entity->position.y = storeY;
    }

    bool32 collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0xC0000 * val, 0x100000, 8);

    if (entity->flag) {
        if (entity->velocity.x * val >= 0x4000) {
            entity->velocity.x -= 0xA00 * val;
            if (val * entity->velocity.x < 0x4000) {
                entity->velocity.x = val << 14;
                entity->state      = Stegway_Unknown6;
                Stegway_SetupAnims(0, false);
                entity->animator1.frameID = 0;
                entity->dustFlag          = false;
                entity->flag              = false;
            }
        }
    }

    if (!collided) {
        entity->state = Stegway_Unknown7;
        Stegway_SetupAnims(3, false);
        entity->dustFlag = false;
        entity->flag     = false;
    }
}

void Stegway_EditorDraw(void) {}

void Stegway_EditorLoad(void) {}

void Stegway_Serialize(void) { RSDK_EDITABLE_VAR(Stegway, VAR_UINT8, direction); }
