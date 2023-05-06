// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LightBulb Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLightBulb *LightBulb;

void LightBulb_Update(void)
{
    RSDK_THIS(LightBulb);

    StateMachine_Run(self->state);
}

void LightBulb_LateUpdate(void) {}

void LightBulb_StaticUpdate(void) {}

void LightBulb_Draw(void)
{
    RSDK_THIS(LightBulb);

    self->alpha            = 0x200;
    self->inkEffect        = INK_SUB;
    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->inkEffect        = INK_ADD;
    self->alpha            = (RSDK.Sin256(4 * Zone->timer) >> 2) + 192;
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->alpha            = 0x200;
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void LightBulb_Create(void *data)
{
    RSDK_THIS(LightBulb);

    self->drawGroup     = (self->priority != LIGHTBULB_PRIO_HIGH) ? Zone->objectDrawGroup[0] : Zone->objectDrawGroup[1];
    self->visible       = true;
    self->alpha         = 0x100;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    self->state         = LightBulb_State_CheckPlayerCollisions;
    RSDK.SetSpriteAnimation(LightBulb->aniFrames, 0, &self->animator, true, 0);
}

void LightBulb_StageLoad(void)
{
    LightBulb->aniFrames = RSDK.LoadSpriteAnimation("MSZ/LightBulb.bin", SCOPE_STAGE);

    LightBulb->hitboxLightBulb.left   = -12;
    LightBulb->hitboxLightBulb.top    = -12;
    LightBulb->hitboxLightBulb.right  = 12;
    LightBulb->hitboxLightBulb.bottom = 12;

    LightBulb->sfxBulbPop = RSDK.GetSfx("Stage/BulbPop.wav");
    DEBUGMODE_ADD_OBJ(LightBulb);
}

void LightBulb_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(LightBulb, NULL, self->position.x, self->position.y);
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
        if (player->animator.animationID != ANI_HURT && Player_CheckCollisionTouch(player, self, &LightBulb->hitboxLightBulb)) {
            RSDK.PlaySfx(LightBulb->sfxBulbPop, false, 255);

#if MANIA_USE_PLUS
            if (player->state != Player_State_MightyHammerDrop) {
#endif
                if (player->state == Player_State_FlyCarried)
                    RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

                int32 anim = player->animator.animationID;
                if (anim != ANI_FLY && anim != ANI_FLY_LIFT_TIRED) {
                    if (player->state != Player_State_TailsFlight) {
                        if (player->state != Player_State_DropDash)
                            player->state = Player_State_Air;

                        if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                            player->animator.animationID = ANI_WALK;
                    }
                }

                if (player->animator.animationID != ANI_FLY)
                    player->applyJumpCap = false;

                if (player->velocity.y > -0x80000)
                    player->velocity.y = -0x80000;

                player->onGround       = false;
                player->tileCollisions = TILECOLLISION_DOWN;
#if MANIA_USE_PLUS
            }
#endif
            CREATE_ENTITY(ScoreBonus, NULL, self->position.x, self->position.y)->animator.frameID = 16;
            Player_GiveScore(player, 10);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), self->position.x, self->position.y - 0x100000)->drawGroup =
                Zone->objectDrawGroup[1];

            for (int32 i = 0; i < 6; ++i) {
                int32 x              = self->position.x + RSDK.Rand(-0x80000, 0x80000);
                int32 y              = self->position.y + RSDK.Rand(-0x80000, 0x80000);
                EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x, y);

                debris->state           = Debris_State_Fall;
                debris->gravityStrength = 0x4000;
                debris->velocity.x      = RSDK.Rand(0, 0x20000);
                if (debris->position.x < self->position.x)
                    debris->velocity.x = -debris->velocity.x;

                debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                debris->drawFX     = FX_FLIP;
                debris->direction  = i & 3;
                debris->drawGroup  = Zone->objectDrawGroup[1];
                RSDK.SetSpriteAnimation(ItemBox->aniFrames, 6, &debris->animator, true, RSDK.Rand(0, 4));
            }

            self->state   = LightBulb_State_Destroyed;
            self->visible = false;
            self->active  = ACTIVE_NORMAL;
        }
    }
}

void LightBulb_State_Destroyed(void)
{
    RSDK_THIS(LightBulb);

    Vector2 range = { TO_FIXED(256), TO_FIXED(256) };
    if (!RSDK.CheckOnScreen(self, &range)) {
        self->state   = LightBulb_State_CheckPlayerCollisions;
        self->visible = true;
        self->active  = ACTIVE_BOUNDS;
    }
}

#if GAME_INCLUDE_EDITOR
void LightBulb_EditorDraw(void) { LightBulb_Draw(); }

void LightBulb_EditorLoad(void)
{
    LightBulb->aniFrames = RSDK.LoadSpriteAnimation("MSZ/LightBulb.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(LightBulb, priority);
    RSDK_ENUM_VAR("High", LIGHTBULB_PRIO_HIGH);
    RSDK_ENUM_VAR("Low", LIGHTBULB_PRIO_LOW);
}
#endif

void LightBulb_Serialize(void)
{
    RSDK_EDITABLE_VAR(LightBulb, VAR_BOOL, respawn);
    RSDK_EDITABLE_VAR(LightBulb, VAR_ENUM, priority);
}
