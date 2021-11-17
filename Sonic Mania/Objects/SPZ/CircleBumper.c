#include "SonicMania.h"

ObjectCircleBumper *CircleBumper;

void CircleBumper_Update(void)
{
    RSDK_THIS(CircleBumper);
    StateMachine_Run(entity->stateMove);
    StateMachine_Run(entity->stateCollide);
}

void CircleBumper_LateUpdate(void) {}

void CircleBumper_StaticUpdate(void) {}

void CircleBumper_Draw(void)
{
    RSDK_THIS(CircleBumper);
    RSDK.DrawSprite(&entity->animator, &entity->originPos, false);
}

void CircleBumper_Create(void *data)
{
    RSDK_THIS(CircleBumper);
    entity->visible     = true;
    entity->drawOrder   = Zone->drawOrderHigh;
    entity->originPos.x = entity->position.x;
    entity->originPos.y = entity->position.y;
    entity->active      = ACTIVE_BOUNDS;
    if (!RSDK_sceneInfo->inEditor) {
        entity->amplitude.x >>= 10;
        entity->amplitude.y >>= 10;
    }

    switch (entity->type) {
        case 0:
        default:
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;
            RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &entity->animator, true, 0);
            entity->stateMove = CircleBumper_Unknown4;
            break;
        case 1:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x1000) << 10;
            entity->stateMove     = CircleBumper_Unknown5;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x1000) << 10;
            break;
        case 2:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x1000) << 10;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x1000) << 10;
            RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &entity->animator, true, 0);
            entity->stateMove = CircleBumper_Unknown6;
            break;
        case 3:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x2000) << 9;
            entity->stateMove     = CircleBumper_Unknown8;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 9;
            break;
        case 4:
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;
            entity->active        = ACTIVE_NEVER;
            entity->stateMove     = CircleBumper_Unknown7;
            break;
    }

    RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &entity->animator, true, 0);
    entity->stateCollide = CircleBumper_Unknown2;
    if (!entity->hitCount)
        entity->hitCount = 5;
}

void CircleBumper_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1")) 
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SPZ2")) 
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("Blueprint")) 
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/CircleBumper.bin", SCOPE_STAGE);

    CircleBumper->hitbox.left   = -7;
    CircleBumper->hitbox.top    = -6;
    CircleBumper->hitbox.right  = 7;
    CircleBumper->hitbox.bottom = 6;
    CircleBumper->sfxBumper     = RSDK.GetSFX("Stage/Bumper.wav");

    DEBUGMODE_ADD_OBJ(CircleBumper);
}

void CircleBumper_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void CircleBumper_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(CircleBumper, NULL, entity->position.x, entity->position.y);
}

void CircleBumper_Unknown1(void)
{
    RSDK_THIS(CircleBumper);
    int32 storeX         = entity->position.x;
    int32 storeY         = entity->position.y;
    entity->position.x = entity->originPos.x;
    entity->position.y = entity->originPos.y;
    foreach_active(Player, player)
    {
        if (player->playerAnimator.animationID != ANI_HURT && Player_CheckBadnikTouch(player, entity, &CircleBumper->hitbox)) {
            entity->animator.frameID = 0;
            entity->stateCollide     = CircleBumper_Unknown3;
            RSDK.PlaySfx(CircleBumper->sfxBumper, 0, 255);
            entity->active = ACTIVE_NORMAL;
            int32 angle      = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
            int32 xVel       = 0x700 * RSDK.Cos256(angle);
            int32 yVel       = 0x700 * RSDK.Sin256(angle);
            if (player->state == Player_State_FlyCarried)
                RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
            }
            else {
#endif
                int32 anim = player->playerAnimator.animationID;
                if (anim != ANI_FLY && anim != ANI_FLYLIFTTIRED && player->state != Player_State_TailsFlight) {
                    if (player->state != Player_State_DropDash)
                        player->state = Player_State_Air;
                    if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                        player->playerAnimator.animationID = ANI_WALK;
                }
#if RETRO_USE_PLUS
            }
#endif
            if (player->playerAnimator.animationID != ANI_FLY) {
                player->velocity.x  = xVel;
                player->groundVel   = xVel;
                player->jumpAbility = 0;
            }
            player->velocity.y     = yVel;
            player->onGround       = false;
            player->tileCollisions = true;
            if (entity->hitCount) {
                EntityScoreBonus *bonus = (EntityScoreBonus *)RSDK.CreateEntity(ScoreBonus->objectID, NULL, entity->position.x, entity->position.y);
                bonus->animator.frameID     = 16;
                Player_GiveScore(player, 10);
                --entity->hitCount;
            }
        }
    }
    entity->position.x = storeX;
    entity->position.y = storeY;
}

void CircleBumper_Unknown2(void) { CircleBumper_Unknown1(); }

void CircleBumper_Unknown3(void)
{
    RSDK_THIS(CircleBumper);
    CircleBumper_Unknown1();
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        entity->animator.frameID = 0;
        entity->active           = ACTIVE_BOUNDS;
        entity->stateCollide     = CircleBumper_Unknown2;
    }
}

void CircleBumper_Unknown4(void)
{
    RSDK_THIS(CircleBumper);
    entity->originPos.x = entity->position.x;
    entity->originPos.y = entity->position.y;
}

void CircleBumper_Unknown5(void)
{
    RSDK_THIS(CircleBumper);
    entity->originPos.x = entity->amplitude.x * RSDK.Sin1024(entity->speed * Zone->timer) + entity->position.x;
    entity->originPos.y = entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer) + entity->position.y;
}

void CircleBumper_Unknown6(void)
{
    RSDK_THIS(CircleBumper);
    entity->originPos.x = entity->amplitude.x * RSDK.Cos1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->position.x;
    entity->originPos.y = entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->position.y;
}

void CircleBumper_Unknown7(void)
{
    RSDK_THIS(CircleBumper);
    entity->originPos.x += RSDK_sceneInfo->entity->velocity.x;
    entity->originPos.y += entity->velocity.y;
    Entity *ent = RSDK.GetEntityByID(entity->speed);

    if (entity->velocity.x <= 0) {
        if (entity->originPos.x < ent->position.x)
            entity->originPos.x = ent->position.x;
    }
    else if (entity->originPos.x > ent->position.x) {
        entity->originPos.x = ent->position.x;
    }

    if (entity->velocity.y <= 0) {
        if (entity->originPos.y < ent->position.y)
            entity->originPos.y = ent->position.y;
    }
    else if (entity->originPos.y > ent->position.y) {
        entity->originPos.y = ent->position.y;
    }
}

void CircleBumper_Unknown8(void)
{
    RSDK_THIS(CircleBumper);
    int32 val = Zone->timer << 7;
    if (((val >> 16) & 1) == RSDK_sceneInfo->entity->direction) {
        entity->originPos.x = entity->position.x + (val * entity->amplitude.x >> 6) - (entity->amplitude.x << 15);
        entity->originPos.y = entity->position.y + (val * entity->amplitude.y >> 6) - (entity->amplitude.y << 15);
    }
    else {
        entity->originPos.x = entity->position.x + (entity->amplitude.x << 15) - (val * entity->amplitude.x >> 6);
        entity->originPos.y = entity->position.y + (entity->amplitude.y << 15) - (val * entity->amplitude.y >> 6);
    }
}

#if RETRO_INCLUDE_EDITOR
void CircleBumper_EditorDraw(void)
{
    RSDK_THIS(CircleBumper);

    switch (entity->type) {
        case 0:
        default:
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;
            break;
        case 1:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x1000) << 10;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x1000) << 10;
            break;
        case 2:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x1000) << 10;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x1000) << 10;
            break;
        case 3:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x2000) << 9;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 9;
            break;
        case 4:
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;
            break;
    }
    RSDK.SetSpriteAnimation(CircleBumper->aniFrames, 0, &entity->animator, true, 0);
    entity->originPos = entity->position;

    CircleBumper_Draw();
}

void CircleBumper_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SPZ2"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/CircleBumper.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("Blueprint"))
        CircleBumper->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/CircleBumper.bin", SCOPE_STAGE);
}
#endif

void CircleBumper_Serialize(void)
{
    RSDK_EDITABLE_VAR(CircleBumper, VAR_ENUM, hitCount);
    RSDK_EDITABLE_VAR(CircleBumper, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(CircleBumper, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(CircleBumper, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(CircleBumper, VAR_INT32, angle);
}
