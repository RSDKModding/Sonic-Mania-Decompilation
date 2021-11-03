#include "SonicMania.h"

ObjectDango *Dango;

void Dango_Update(void)
{
    RSDK_THIS(Dango);
    StateMachine_Run(entity->state);

    if (entity->timer2 > 0)
        entity->timer2--;
}

void Dango_LateUpdate(void) {}

void Dango_StaticUpdate(void) {}

void Dango_Draw(void)
{
    RSDK_THIS(Dango);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Dango_Create(void *data)
{
    RSDK_THIS(Dango);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &entity->animator, true, 0);
    entity->state = Dango_State_Setup;
}

void Dango_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Dango.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Dango.bin", SCOPE_STAGE);

    Dango->hitbox1.top    = -14;
    Dango->hitbox1.left   = -14;
    Dango->hitbox1.right  = 14;
    Dango->hitbox1.bottom = 14;
    Dango->hitbox2.top    = -128;
    Dango->hitbox2.left   = -112;
    Dango->hitbox2.right  = -18;
    Dango->hitbox2.bottom = 128;
    Dango->sfxBumper      = RSDK.GetSFX("Stage/Bumper3.wav");
    DEBUGMODE_ADD_OBJ(Dango);
}

void Dango_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityDango *dango = CREATE_ENTITY(Dango, NULL, entity->position.x, entity->position.y);
    RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &dango->animator, true, 0);
    dango->state = Dango_State_Unknown5;
    if (dango->direction == FLIP_NONE)
        dango->groundVel = -0x20000;
    else
        dango->groundVel = 0x20000;
}

void Dango_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Dango->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Dango_CheckOnScreen(void)
{
    RSDK_THIS(Dango);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->direction  = entity->startDir;
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        Dango_Create(NULL);
    }
}

void Dango_CheckPlayerCollisions(void)
{
    RSDK_THIS(Dango);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Dango->hitbox1)) {
            int anim    = player->playerAnimator.animationID;
            bool32 flag = anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH;
#if RETRO_USE_PLUS
            if (player->characterID == ID_MIGHTY)
                flag |= anim == ANI_CROUCH || player->jumpAbilityTimer > 1;
            flag &= entity->state == Dango_State_Unknown4 && player->state != Player_State_MightyHammerDrop;
#else
            flag &= entity->state == Dango_State_Unknown4;
#endif

            if (flag) {
                if (anim != ANI_JUMP)
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                RSDK.PlaySfx(Dango->sfxBumper, false, 255);

                if (player->velocity.x <= 0) {
                    if (entity->groundVel > player->velocity.x) {
                        if (entity->groundVel > -0x20000)
                            entity->groundVel = -0x20000;
                    }
                }
                else if (entity->groundVel < player->velocity.x) {
                    entity->groundVel = player->velocity.x;
                    if (entity->groundVel < 0x20000)
                        entity->groundVel = 0x20000;
                }
                entity->velocity.x = entity->groundVel;
#if RETRO_USE_PLUS
                if (!(player->characterID == ID_MIGHTY && player->jumpAbilityTimer > 1)) {
#endif
                    player->groundVel  = (player->position.x - entity->position.x) >> 2;
                    player->velocity.x = player->groundVel;
                    player->velocity.y = (player->position.y - entity->position.y) >> 2;
                    player->onGround   = false;
#if RETRO_USE_PLUS
                }
#endif

                if ((entity->position.x > player->position.x && entity->velocity.x < 0)
                    || (entity->position.x < player->position.x && entity->velocity.x > 0)) {
                    entity->groundVel  = -entity->groundVel;
                    entity->velocity.x = -entity->velocity.x;
                }
            }
            else {
                Player_CheckBadnikBreak(entity, player, true);
            }
        }
    }
}

bool32 Dango_HandleTileCollisions(StateMachine(nextState), uint8 anim)
{
    RSDK_THIS(Dango);

    bool32 flag = false;
    int storeX  = entity->position.x;
    int storeY  = entity->position.y;
    entity->position.x += entity->groundVel;

    bool32 collided = false;
    if (entity->groundVel <= 0)
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_RWALL, 0, Dango->hitbox1.left << 16, 0, 4);
    else
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_LWALL, 0, Dango->hitbox1.right << 16, 0, 4);

    if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, 2)) {
        uint16 tile   = RSDK.GetTileInfo(Zone->fgLow, entity->position.x >> 16, (entity->position.y + 0xD0000) >> 16);
        entity->angle = RSDK.GetTileAngle(tile, 0, 0);
        if (entity->groundVel <= 0) {
            if (entity->angle < 0x80 && entity->angle > 0x10)
                flag = true;
            else
                entity->velocity.x = (entity->groundVel >> 8) * RSDK.Cos256(entity->angle);
        }
        else {
            if ((uint32)(entity->angle - 0x81) <= 0x6E)
                flag = true;
            else
                entity->velocity.x = (entity->groundVel >> 8) * RSDK.Cos256(entity->angle);
        }
    }
    else {
        flag = true;
    }

    if (collided)
        flag = true;
    if (entity->flag)
        flag = false;
    if (flag) {
        entity->position.x = storeX;
        entity->position.y = storeY;
        RSDK.SetSpriteAnimation(Dango->aniFrames, anim, &entity->animator, true, 0);
        entity->state = nextState;
        entity->flag  = true;
    }
    return flag;
}

void Dango_State_Setup(void)
{
    RSDK_THIS(Dango);

    entity->active = ACTIVE_NORMAL;
    if (entity->direction == FLIP_NONE)
        entity->groundVel = -0x6000;
    else
        entity->groundVel = 0x6000;
    entity->velocity.y = 0;
    entity->state      = Dango_State_Unknown1;
    Dango_State_Unknown1();
}

void Dango_State_Unknown1(void)
{
    RSDK_THIS(Dango);

    if (RSDK.ObjectTileCollision(RSDK_sceneInfo->entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, false)) {
        RSDK.ProcessAnimation(&entity->animator);
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
        if (!Dango_HandleTileCollisions(Dango_State_Unknown6, 5) && entity->timer2 <= 0) {
            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, entity, &Dango->hitbox2)) {
                    RSDK.SetSpriteAnimation(Dango->aniFrames, 3, &entity->animator, true, 0);
                    entity->state = Dango_State_Unknown3;
                }
            }
        }
    }
    else {
        entity->state = Dango_State_Unknown2;
        Dango_State_Unknown2();
    }
}

void Dango_State_Unknown6(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &entity->animator, true, 0);
        entity->flag       = false;
        entity->groundVel  = -entity->groundVel;
        entity->velocity.x = -entity->velocity.x;
        entity->state      = Dango_State_Unknown1;
        entity->direction ^= FLIP_X;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
    }
}

void Dango_State_Unknown2(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->groundVel;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, true)) {
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &entity->animator, true, 0);
        entity->state = Dango_State_Unknown1;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
    }
}

void Dango_State_Unknown3(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &entity->animator, true, 0);
        entity->state = Dango_State_Unknown4;
        if (entity->direction == FLIP_NONE)
            entity->groundVel = -0x20000;
        else
            entity->groundVel = 0x20000;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
    }
}

void Dango_State_Unknown4(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&entity->animator);
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, false)) {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
        Dango_HandleTileCollisions(Dango_State_Unknown7, 4);
    }
    else {
        entity->state = Dango_State_Unknown5;
        Dango_State_Unknown5();
    }
}

void Dango_State_Unknown5(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->groundVel;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, true)) {
        entity->velocity.y = 0;
        entity->state      = Dango_State_Unknown4;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
    }
}

void Dango_State_Unknown7(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &entity->animator, true, 0);
        entity->flag   = false;
        entity->timer2 = 30;
        if (entity->direction == FLIP_NONE)
            entity->groundVel = -0x6000;
        else
            entity->groundVel = 0x6000;
        entity->state = Dango_State_Unknown1;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
    }
}

#if RETRO_USE_PLUS
void Dango_StateTaunt_Unknown1(void)
{
    RSDK_THIS(Dango);

    if (++entity->timer == 180) {
        entity->timer     = 0;
        entity->groundVel = 0x60000;
        entity->state     = Dango_StateTaunt_Unknown2;
        RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &entity->animator, true, 0);
    }
}

void Dango_StateTaunt_Unknown2(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->groundVel;
    RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xE0000, 14);

    foreach_active(PhantomRuby, ruby)
    {
        if (abs(entity->position.x - ruby->position.x) < 0x120000) {
            RSDK.PlaySfx(Dango->sfxBumper, false, 255);
            entity->state    = Dango_StateTaunt_Unknown3;
            ruby->state      = PhantomRuby_Unknown6;
            ruby->velocity.x = entity->groundVel;
            ruby->velocity.y = -0x80000;
            RSDK.SetSpriteAnimation(Dango->aniFrames, 4, &entity->animator, true, 0);
        }
    }
}

void Dango_StateTaunt_Unknown3(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&entity->animator);
    entity->groundVel -= entity->groundVel >> 2;
    entity->position.x += entity->groundVel;
    if (entity->groundVel < 0x8000) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 6, &entity->animator, true, 0);
        entity->state = Dango_StateTaunt_Unknown4;
    }
}

void Dango_StateTaunt_Unknown4(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1)
        ++entity->timer;
    if (entity->timer == 32) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 5, &entity->animator, true, 0);
        entity->timer = 0;
        entity->state = Dango_StateTaunt_Unknown5;
    }
}

void Dango_StateTaunt_Unknown5(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 3, &entity->animator, true, 0);
        entity->direction = FLIP_NONE;
        entity->state     = Dango_StateTaunt_Unknown6;
    }
}

void Dango_StateTaunt_Unknown6(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.animationID == 3) {
        if (entity->animator.frameID == entity->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &entity->animator, true, 0);
    }
    else {
        entity->position.x -= 0x40000;
    }

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}
#endif

#if RETRO_INCLUDE_EDITOR
void Dango_EditorDraw(void) { Dango_Draw(); }

void Dango_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Dango.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Dango.bin", SCOPE_STAGE);
}
#endif

void Dango_Serialize(void) {}
