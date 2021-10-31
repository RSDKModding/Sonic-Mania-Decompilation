#include "SonicMania.h"

ObjectJellygnite *Jellygnite;

void Jellygnite_Update(void)
{
    RSDK_THIS(Jellygnite);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator2);

    StateMachine_Run(entity->state);

    if (entity->state != Jellygnite_State_Setup && entity->objectID == Jellygnite->objectID) {
        if (entity->field_66 > 0)
            entity->field_66--;
        Jellygnite_CheckPlayerCollisions();

        if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
            entity->position       = entity->startPos;
            entity->direction      = entity->startDir;
            entity->visible        = false;
            entity->timer          = 0;
            entity->angle          = 0;
            entity->field_68       = 0;
            entity->field_6C       = 0;
            entity->grabbedPlayer  = 0;
            entity->shakeTimer     = 0;
            entity->shakeCount     = 0;
            entity->lastShakeFlags = 0;
            Jellygnite_Create(NULL);
        }
    }
}

void Jellygnite_LateUpdate(void) {}

void Jellygnite_StaticUpdate(void)
{
    foreach_active(Jellygnite, jellygnite) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(jellygnite)); }
}

void Jellygnite_Draw(void)
{
    RSDK_THIS(Jellygnite);
    Jellygnite_DrawPart1();
    Jellygnite_DrawPart2();
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Jellygnite_Create(void *data)
{
    RSDK_THIS(Jellygnite);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->startDir      = entity->direction;
    entity->drawFX        = FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->state         = Jellygnite_State_Setup;
}

void Jellygnite_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HPZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Jellygnite.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("HCZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Jellygnite.bin", SCOPE_STAGE);

    Jellygnite->hitbox.left   = -14;
    Jellygnite->hitbox.top    = -14;
    Jellygnite->hitbox.right  = 14;
    Jellygnite->hitbox.bottom = 14;
    Jellygnite->sfxGrab       = RSDK.GetSFX("Global/Grab.wav");
    Jellygnite->sfxElectrify  = RSDK.GetSFX("Stage/Electrify2.wav");
    DEBUGMODE_ADD_OBJ(Jellygnite);
}

void Jellygnite_DebugSpawn(void)
{
    RSDK_THIS(Jellygnite);
    EntityJellygnite *jellygnite = CREATE_ENTITY(Jellygnite, NULL, entity->position.x, entity->position.y);
    jellygnite->direction        = entity->direction;
    jellygnite->startDir         = entity->direction;
}

void Jellygnite_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Jellygnite_SetupAnimations(uint8 id)
{
    RSDK_THIS(Jellygnite);

    switch (id) {
        case 0:
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 3, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 5, &entity->animator3, true, 0);
            break;
        case 1:
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 1, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 3, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 5, &entity->animator3, true, 0);
            break;
        case 2:
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 2, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 4, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 6, &entity->animator3, true, 0);
            break;
    }

    entity->id = id;
}

void Jellygnite_CheckPlayerCollisions(void)
{
    RSDK_THIS(Jellygnite);

    foreach_active(Player, player)
    {
        if (player != (EntityPlayer *)entity->grabbedPlayer) {
            if (Player_CheckCollisionTouch(player, entity, &Jellygnite->hitbox) && entity->state == Jellygnite_State_Unknown1
                && player->position.y >= entity->position.y) {
                entity->state = Jellygnite_State_Unknown2;
                Jellygnite_SetupAnimations(1);
                entity->grabbedPlayer = (Entity *)player;
                if (Water) {
                    EntityWater *bubble = Water_GetPlayerBubble(player);
                    if (bubble) {
                        bubble->timer = 0;
                        Water_HCZBubbleBurst(bubble, false);
                    }
                }
                entity->isPermanent = true;
                RSDK.PlaySfx(Jellygnite->sfxGrab, false, 255);
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->groundVel       = 0;
                player->state           = Player_State_None;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
                player->onGround        = false;
                player->direction       = entity->direction;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, true, 0);
                player->playerAnimator.animationSpeed = 0;
                player->direction                     = entity->direction ^ 1;
            }

            if (player != (EntityPlayer *)entity->grabbedPlayer && player->position.y < entity->position.y
                && Player_CheckBadnikTouch(player, entity, &Jellygnite->hitbox) && Player_CheckBadnikBreak(entity, player, false)) {
                EntityPlayer *playerPtr = (EntityPlayer *)entity->grabbedPlayer;
                if (playerPtr)
                    playerPtr->state = Player_State_Air;
                destroyEntity(entity);
            }
        }
    }
}

void Jellygnite_HandlePlayerStruggle(void)
{
    RSDK_THIS(Jellygnite);
    EntityPlayer *player = (EntityPlayer *)entity->grabbedPlayer;

    if (player) {
        if (entity->lastShakeFlags) {
            if (!--entity->shakeTimer) {
                entity->shakeCount     = 0;
                entity->lastShakeFlags = 0;
            }
            uint8 flags = 0;
            if (player->left)
                flags = 1;
            if (player->right)
                flags |= 2;
            if (flags && flags != 3 && flags != entity->lastShakeFlags) {
                entity->lastShakeFlags = flags;
                if (++entity->shakeCount >= 4) {
                    player->state         = Player_State_Air;
                    entity->field_66      = 16;
                    entity->grabbedPlayer = NULL;
                }
                else {
                    entity->shakeTimer = 64;
                }
            }
        }
        else {
            if (player->left) {
                entity->lastShakeFlags = 1;
                entity->shakeTimer     = 32;
            }
            if (player->right) {
                entity->lastShakeFlags |= 2;
                entity->shakeTimer = 32;
            }
        }
        player->position.x = entity->position.x;
        player->position.y = entity->position.y + 0xC0000;
    }
}

bool32 Jellygnite_CheckInWater(void *p)
{
    RSDK_THIS(Jellygnite);
    EntityPlayer *player = (EntityPlayer *)p;

    if (player->position.y > Water->waterLevel)
        return true;

    foreach_active(Water, water)
    {
        if (water->type == 1) {
            if (Player_CheckCollisionTouch(player, entity, &water->hitbox)
                && RSDK.CheckObjectCollisionTouchBox(entity, &Jellygnite->hitbox, water, &water->hitbox)) {
                return true;
            }
        }
    }
    return false;
}

void Jellygnite_DrawPart1(void)
{
    RSDK_THIS(Jellygnite);
    Vector2 drawPos;

    int angle = entity->angle & 0x1FF;
    int y     = entity->position.y + 0x70000;

    for (int i = 0; i < 4; ++i) {
        drawPos.x = entity->position.x + (RSDK.Cos512(angle) << 9);
        drawPos.y = y + (RSDK.Sin512(angle) << 8);
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        angle = (angle + 32) & 0x1FF;
        y += 0x60000;
    }
}

void Jellygnite_DrawPart2(void)
{
    RSDK_THIS(Jellygnite);
    Vector2 drawPos;

    int x   = 0;
    int y   = 0;
    int ang = entity->field_68;
    for (int i = 0; i < 4; ++i) {
        int angle = (ang >> 7) & 0x1FF;
        x += 0x312 * RSDK.Sin512(angle);
        y += 0x312 * RSDK.Cos512(angle);

        drawPos.x = x + entity->position.x - 0xD0000;
        drawPos.y = y + entity->position.y + 0x10000;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        drawPos.x = -x;
        drawPos.x = entity->position.x + 0xD0000 - x;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        ang <<= 1;
    }
}

void Jellygnite_State_Setup(void)
{
    RSDK_THIS(Jellygnite);
    if (entity->position.y >= Water->waterLevel) {
        entity->active         = ACTIVE_NORMAL;
        entity->timer          = 0;
        entity->angle          = 0;
        entity->field_68       = 0;
        entity->field_6C       = 0;
        entity->grabbedPlayer  = 0;
        entity->shakeTimer     = 0;
        entity->shakeCount     = 0;
        entity->lastShakeFlags = 0;
        Jellygnite_SetupAnimations(0);
        entity->state = Jellygnite_State_Unknown1;
        Jellygnite_State_Unknown1();
    }
    else {
        destroyEntity(entity);
    }
}

void Jellygnite_State_Unknown1(void)
{
    RSDK_THIS(Jellygnite);

    entity->angle      = (entity->angle + 4) & 0x1FF;
    entity->field_68   = RSDK.Sin512(entity->angle) << 1;
    entity->field_6C   = (entity->field_6C + 1) & 0x1FF;
    entity->position.x = (RSDK.Sin512(entity->field_6C) << 11) + entity->startPos.x;

    EntityPlayer *playerPtr = NULL;
    int distance            = 0x7FFFFFFF;
    foreach_active(Player, player)
    {
        if (abs(entity->position.x - player->position.x) < distance) {
            playerPtr = player;
            distance  = abs(entity->position.x - player->position.x);
        }
    }

    if (playerPtr) {
        if (Jellygnite_CheckInWater(playerPtr)) {
            if (entity->position.y <= playerPtr->position.y - 0x200000) {
                entity->velocity.y += 0x800;
                if (entity->velocity.y >= 0xC000)
                    entity->velocity.y = 0xC000;
            }
            else {
                entity->velocity.y -= 0x800;
                if (entity->velocity.y <= -0xC000)
                    entity->velocity.y = -0xC000;
            }
        }
        else {
            entity->velocity.y >>= 1;
        }
    }

    entity->position.y += entity->velocity.y;
    if (entity->position.y - 0x100000 < Water->waterLevel && entity->velocity.y < 0) {
        entity->position.y = entity->position.y - entity->velocity.y;
        entity->velocity.y = -entity->velocity.y;
    }
}

void Jellygnite_State_Unknown2(void)
{
    RSDK_THIS(Jellygnite);
    if (entity->field_68 >= 0x600) {
        entity->state = Jellygnite_State_Unknown3;
        Jellygnite_SetupAnimations(2);
        RSDK.PlaySfx(Jellygnite->sfxElectrify, false, 255);
    }
    else {
        entity->field_68 += 128;
    }

    EntityPlayer *player = (EntityPlayer *)entity->grabbedPlayer;
    player->position.x   = entity->position.x;
    player->position.y   = entity->position.y + 0xC0000;
}

void Jellygnite_State_Unknown3(void)
{
    RSDK_THIS(Jellygnite);
    Jellygnite_HandlePlayerStruggle();

    if (++entity->timer == 60) {
        EntityPlayer *player = (EntityPlayer *)entity->grabbedPlayer;
        if (player && player->state == Player_State_None) {
            Player_CheckHit(player, entity);
            if (player->state != Player_State_Hit && Player_CheckValidState(player))
                player->state = Player_State_Air;
            entity->grabbedPlayer = NULL;
        }
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void Jellygnite_EditorDraw(void)
{
    RSDK_THIS(Jellygnite);
    Jellygnite_SetupAnimations(0);

    Jellygnite_Draw();
}

void Jellygnite_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("HPZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Jellygnite.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("HCZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Jellygnite.bin", SCOPE_STAGE);
}
#endif

void Jellygnite_Serialize(void) { RSDK_EDITABLE_VAR(Jellygnite, VAR_UINT8, direction); }
