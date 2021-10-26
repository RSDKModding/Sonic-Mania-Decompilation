#include "SonicMania.h"

ObjectPinata *Pinata;

void Pinata_Update(void)
{
    RSDK_THIS(Pinata);
    StateMachine_Run(entity->state);
}

void Pinata_LateUpdate(void) {}

void Pinata_StaticUpdate(void) {}

void Pinata_Draw(void)
{
    RSDK_THIS(Pinata);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Pinata_Create(void *data)
{
    RSDK_THIS(Pinata);

    if (entity->priority)
        entity->drawOrder = Zone->drawOrderLow;
    else
        entity->drawOrder = Zone->drawOrderHigh;
    entity->visible       = true;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    entity->state         = Pinata_State_CheckPlayerCollisions;
    RSDK.SetSpriteAnimation(Pinata->aniFrames, 3, &entity->animator, true, 0);
}

void Pinata_StageLoad(void)
{
    Pinata->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/Pinata.bin", SCOPE_STAGE);
    Pinata->hitbox.left   = -12;
    Pinata->hitbox.top    = -16;
    Pinata->hitbox.right  = 12;
    Pinata->hitbox.bottom = 16;
    Pinata->sfxPinata     = RSDK.GetSFX("MSZ/Pinata.wav");
    DEBUGMODE_ADD_OBJ(Pinata);
}

void Pinata_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Pinata->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Pinata_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Pinata, NULL, entity->position.x, entity->position.y);
}

void Pinata_State_CheckPlayerCollisions(void)
{
    RSDK_THIS(Pinata);
    RSDK.ProcessAnimation(&entity->animator);

    foreach_active(Player, player)
    {
        if (player->playerAnimator.animationID != ANI_HURT && Player_CheckBadnikTouch(player, entity, &Pinata->hitbox)) {
            RSDK.PlaySfx(Pinata->sfxPinata, false, 255);

#if RETRO_USE_PLUS
            if (player->state != Player_State_MightyHammerDrop) {
#endif
                if (player->state == Player_State_FlyCarried)
                    RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

                int anim = player->playerAnimator.animationID;
                if (anim != ANI_FLY && anim != ANI_FLYLIFTTIRED) {
                    if (player->state != Player_State_TailsFlight) {
                        if (player->state != Player_State_DropDash)
                            player->state = Player_State_Air;

                        if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                            player->playerAnimator.animationID = ANI_WALK;
                    }
                }
                if (player->playerAnimator.animationID != ANI_FLY)
                    player->jumpAbility = 0;
                if (player->velocity.y > -0x80000)
                    player->velocity.y = -0x80000;
                player->onGround       = false;
                player->tileCollisions = true;
#if RETRO_USE_PLUS
            }
#endif
            CREATE_ENTITY(ScoreBonus, NULL, entity->position.x, entity->position.y)->animator.frameID = 16;
            Player_GiveScore(player, 10);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), entity->position.x, entity->position.y - 0x100000)->drawOrder =
                Zone->drawOrderHigh;

            for (int i = 0; i < 6; ++i) {
                int x                = entity->position.x + RSDK.Rand(-0x80000, 0x80000);
                int y                = entity->position.y + RSDK.Rand(-0x80000, 0x80000);
                EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x, y);
                debris->state        = Debris_State_Fall;
                debris->gravity      = 0x4000;
                debris->velocity.x   = RSDK.Rand(0, 0x20000);
                if (debris->position.x < entity->position.x)
                    debris->velocity.x = -debris->velocity.x;
                debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                debris->drawFX     = FX_FLIP;
                debris->direction  = i & 3;
                debris->drawOrder  = Zone->drawOrderHigh;
                RSDK.SetSpriteAnimation(Pinata->aniFrames, 0, &debris->animator, true, RSDK.Rand(0, 4));
            }

            entity->state   = Pinata_State_Respawn;
            entity->visible = false;
            entity->active  = ACTIVE_NORMAL;
        }
    }
}

void Pinata_State_Respawn(void)
{
    RSDK_THIS(Pinata);
    Vector2 range;

    range.x = 0x1000000;
    range.y = 0x1000000;

    if (!RSDK.CheckOnScreen(entity, &range)) {
        entity->state   = Pinata_State_CheckPlayerCollisions;
        entity->visible = true;
        entity->active  = ACTIVE_BOUNDS;
    }
}

void Pinata_EditorDraw(void) { Pinata_Draw(); }

void Pinata_EditorLoad(void) { Pinata->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Pinata.bin", SCOPE_STAGE); }

void Pinata_Serialize(void)
{
    RSDK_EDITABLE_VAR(Pinata, VAR_BOOL, respawn);
    RSDK_EDITABLE_VAR(Pinata, VAR_ENUM, priority);
}
