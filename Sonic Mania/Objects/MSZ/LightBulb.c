#include "SonicMania.h"

ObjectLightBulb *LightBulb;

void LightBulb_Update(void)
{
    RSDK_THIS(LightBulb);
    StateMachine_Run(entity->state);
}

void LightBulb_LateUpdate(void) {}

void LightBulb_StaticUpdate(void) {}

void LightBulb_Draw(void)
{
    RSDK_THIS(LightBulb);

    entity->alpha            = 0x200;
    entity->inkEffect        = INK_SUB;
    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, NULL, false);

    entity->inkEffect        = INK_ADD;
    entity->alpha            = (RSDK.Sin256(4 * Zone->timer) >> 2) + 192;
    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, NULL, false);

    entity->alpha            = 0x200;
    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void LightBulb_Create(void *data)
{
    RSDK_THIS(LightBulb);
    if (entity->priority)
        entity->drawOrder = Zone->drawOrderLow;
    else
        entity->drawOrder = Zone->drawOrderHigh;
    entity->visible       = true;
    entity->alpha         = 256;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    entity->state         = LightBulb_State_CheckPlayerCollisions;
    RSDK.SetSpriteAnimation(LightBulb->aniFrames, 0, &entity->animator, true, 0);
}

void LightBulb_StageLoad(void)
{
    LightBulb->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/LightBulb.bin", SCOPE_STAGE);
    LightBulb->hitbox.left   = -12;
    LightBulb->hitbox.top    = -12;
    LightBulb->hitbox.right  = 12;
    LightBulb->hitbox.bottom = 12;
    LightBulb->sfxBulbPop    = RSDK.GetSFX("Stage/BulbPop.wav");
    DEBUGMODE_ADD_OBJ(LightBulb);
}

void LightBulb_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(LightBulb, NULL, entity->position.x, entity->position.y);
}

void LightBulb_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(LightBulb->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void LightBulb_State_CheckPlayerCollisions(void)
{
    RSDK_THIS(LightBulb);

    foreach_active(Player, player)
    {
        if (player->playerAnimator.animationID != ANI_HURT && Player_CheckCollisionTouch(player, entity, &LightBulb->hitbox)) {
            RSDK.PlaySfx(LightBulb->sfxBulbPop, false, 255);

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
                RSDK.SetSpriteAnimation(ItemBox->aniFrames, 6, &debris->animator, true, RSDK.Rand(0, 4));
            }

            entity->state   = LightBulb_State_Destroyed;
            entity->visible = false;
            entity->active  = ACTIVE_NORMAL;
        }
    }
}

void LightBulb_State_Destroyed(void)
{
    RSDK_THIS(LightBulb);
    Vector2 range;

    range.x = 0x1000000;
    range.y = 0x1000000;

    if (!RSDK.CheckOnScreen(entity, &range)) {
        entity->state   = LightBulb_State_CheckPlayerCollisions;
        entity->visible = true;
        entity->active  = ACTIVE_BOUNDS;
    }
}

#if RETRO_INCLUDE_EDITOR
void LightBulb_EditorDraw(void) { LightBulb_Draw(); }

void LightBulb_EditorLoad(void) { LightBulb->aniFrames = RSDK.LoadSpriteAnimation("MSZ/LightBulb.bin", SCOPE_STAGE); }
#endif

void LightBulb_Serialize(void)
{
    RSDK_EDITABLE_VAR(LightBulb, VAR_BOOL, respawn);
    RSDK_EDITABLE_VAR(LightBulb, VAR_ENUM, priority);
}
