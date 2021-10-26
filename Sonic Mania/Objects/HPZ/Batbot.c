#include "SonicMania.h"

ObjectBatbot *Batbot = NULL;

void Batbot_Update(void)
{
    RSDK_THIS(Batbot);
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.animationID == 1)
        entity->direction = entity->field_88 ^ Batbot->dirFlag[entity->animator1.frameID];

    StateMachine_Run(entity->state);

    Batbot_HandlePlayerInteractions();
    if (entity->state != Batbot_State_Setup) {
        if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, 0) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
            entity->direction = entity->startDir;
            entity->position  = entity->startPos;
            Batbot_Create(NULL);
        }
    }
}

void Batbot_LateUpdate(void) {}

void Batbot_StaticUpdate(void) {}

void Batbot_Draw(void)
{
    RSDK_THIS(Batbot);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    if (entity->flag)
        RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void Batbot_Create(void *data)
{
    RSDK_THIS(Batbot);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;
    entity->drawFX        = FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->state         = Batbot_State_Setup;
}

void Batbot_StageLoad(void)
{
    //if (RSDK.CheckStageFolder("HPZ"))
        Batbot->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Batbot.bin", SCOPE_STAGE);
    Batbot->hitboxBadnik.left   = -16;
    Batbot->hitboxBadnik.top    = -12;
    Batbot->hitboxBadnik.right  = 16;
    Batbot->hitboxBadnik.bottom = 12;
    Batbot->hitbox2.left        = -96;
    Batbot->hitbox2.top         = -64;
    Batbot->hitbox2.right       = 96;
    Batbot->hitbox2.bottom      = 128;
    Batbot->hitbox3.left        = -80;
    Batbot->hitbox3.top         = -64;
    Batbot->hitbox3.right       = 80;
    Batbot->hitbox3.bottom      = 96;
    Batbot->hitboxPlayer.left   = 0;
    Batbot->hitboxPlayer.top    = 0;
    Batbot->hitboxPlayer.right  = 0;
    Batbot->hitboxPlayer.bottom = 0;
    Batbot->dirFlag[0]          = 0;
    Batbot->dirFlag[1]          = 0;
    Batbot->dirFlag[2]          = 0;
    Batbot->dirFlag[3]          = 1;
    Batbot->dirFlag[4]          = 1;
    DEBUGMODE_ADD_OBJ(Batbot);
}

void Batbot_DebugSpawn(void)
{
    RSDK_THIS(Batbot);
    EntityBatbot *batBot = CREATE_ENTITY(Batbot, NULL, entity->position.x, entity->position.y);
    batBot->direction    = entity->direction;
    batBot->startDir     = entity->direction;
}

void Batbot_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &DebugMode->animator, true, 3);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Batbot_HandlePlayerInteractions(void)
{
    RSDK_THIS(Batbot);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Batbot->hitboxBadnik))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Batbot_State_Setup(void)
{
    RSDK_THIS(Batbot);
    entity->timer    = 0;
    entity->field_90 = 0;
    entity->field_6C = entity->position.y;
    entity->active   = ACTIVE_NORMAL;
    entity->flag     = true;
    RSDK.SetSpriteAnimation(Batbot->aniFrames, 3, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &entity->animator1, true, 0);
    entity->state = Batbot_State_Idle;
    Batbot_State_Idle();
}

void Batbot_State_Idle(void)
{
    RSDK_THIS(Batbot);

    entity->field_68   = (entity->field_68 + 8) & 0x1FF;
    entity->position.y = (RSDK.Sin512(entity->field_68) << 9) + entity->field_6C;
    bool32 flag        = false;

    foreach_active(Player, player)
    {
        if (RSDK.CheckObjectCollisionTouchBox(player, &Batbot->hitboxPlayer, entity, &Batbot->hitbox2)) {
            RSDK.SetSpriteAnimation(Batbot->aniFrames, 1, &entity->animator1, false, 0);
            flag = true;
        }
        if (RSDK.CheckObjectCollisionTouchBox(player, &Batbot->hitboxPlayer, entity, &Batbot->hitbox3)) {
            entity->playerPtr = (Entity *)player;
            entity->state     = Batbot_State_Attack;
        }
    }

    if (!flag)
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &entity->animator1, false, 0);
}

void Batbot_State_Attack(void)
{
    RSDK_THIS(Batbot);
    if (entity->field_68) {
        entity->field_68   = (entity->field_68 + 8) & 0x1FF;
        entity->position.y = (RSDK.Sin512(entity->field_68) << 9) + entity->field_6C;
    }
    if (++entity->timer == 20) {
        entity->timer      = 0;
        entity->posUnknown = entity->position;
        if (entity->playerPtr->position.x <= entity->position.x) {
            entity->posUnknown.x -= 0x500000;
            entity->swoopAngle = 0;
            entity->field_88   = 0;
            entity->state      = Batbot_State_SwoopRight;
        }
        else {
            entity->posUnknown.x += 0x500000;
            entity->swoopAngle = 256;
            entity->field_88   = 1;
            entity->state      = Batbot_State_SwoopLeft;
        }
        entity->flag = false;
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 2, &entity->animator1, true, 0);
    }
}

void Batbot_State_SwoopRight(void)
{
    RSDK_THIS(Batbot);
    entity->swoopAngle += 4;
    entity->position.x = entity->posUnknown.x + 0x2800 * RSDK.Cos512(entity->swoopAngle);
    entity->position.y = entity->posUnknown.y + 0x2800 * RSDK.Sin512(entity->swoopAngle);
    if (entity->swoopAngle == 256) {
        entity->flag = true;
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &entity->animator1, true, 0);
        entity->state = Batbot_State_Idle;
    }
}

void Batbot_State_SwoopLeft(void)
{
    RSDK_THIS(Batbot);
    entity->swoopAngle -= 4;
    entity->position.x = entity->posUnknown.x + 0x2800 * RSDK.Cos512(entity->swoopAngle);
    entity->position.y = entity->posUnknown.y + 0x2800 * RSDK.Sin512(entity->swoopAngle);
    if (entity->swoopAngle == 0) {
        entity->flag = true;
        RSDK.SetSpriteAnimation(Batbot->aniFrames, 0, &entity->animator1, true, 0);
        entity->state = Batbot_State_Idle;
    }
}

#if RETRO_INCLUDE_EDITOR
void Batbot_EditorDraw(void) {}

void Batbot_EditorLoad(void) {}
#endif

void Batbot_Serialize(void) { RSDK_EDITABLE_VAR(Batbot, VAR_UINT8, direction); }
