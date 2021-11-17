#include "SonicMania.h"

ObjectKabasira *Kabasira;

void Kabasira_Update(void)
{
    RSDK_THIS(Kabasira);
    StateMachine_Run(entity->state);
}

void Kabasira_LateUpdate(void) {}

void Kabasira_StaticUpdate(void) {}

void Kabasira_Draw(void)
{
    RSDK_THIS(Kabasira);

    if (entity->state == Kabasira_State2_Unknown) {
        entity->drawFX = FX_SCALE | FX_FLIP;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.ProcessAnimation(&entity->animator2);
    }
    else {
        int speed = (2 * (entity->direction == FLIP_NONE) - 1);
        int angle = entity->angle + 72 * speed;
        if (angle < 0)
            angle = ((-1 - angle) & -0x200) + angle + 512;
        angle &= 0x1FF;

        int alpha    = 64;
        int angleVel = 24 * speed;

        for (int i = 0; i < 3; ++i) {
            Kabasira_DrawSegment(angle, alpha);
            alpha += 64;
            angle -= angleVel;
            if (angle < 0)
                angle = ((-1 - angle) & -0x200) + angle + 0x200;
            angle &= 0x1FF;
        }

        Kabasira_DrawSegment(angle, alpha);
        entity->angle = angle;
    }
}

void Kabasira_Create(void *data)
{
    RSDK_THIS(Kabasira);

    entity->inkEffect = INK_ALPHA;
    entity->drawFX    = FX_FLIP | FX_ROTATE | FX_SCALE;
    entity->scale.x   = 0x200;
    entity->scale.y   = 0x200;
    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->startPos      = entity->position;
        entity->startDir      = entity->direction;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Kabasira->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Kabasira->aniFrames, 1, &entity->animator2, true, 0);
        entity->animator2.frameCount = 3;
        if (data) {
            entity->alpha = 128;
            entity->state = Kabasira_State2_Unknown;
        }
        else {
            entity->animator2.frameCount = 3;
            entity->animator2.loopIndex  = 0;
            entity->state                = Kabasira_State_Setup;
        }
    }
}

void Kabasira_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Kabasira->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Kabasira.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Kabasira->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Kabasira.bin", SCOPE_STAGE);

    Kabasira->hitbox.left   = -6;
    Kabasira->hitbox.top    = -6;
    Kabasira->hitbox.right  = 6;
    Kabasira->hitbox.bottom = 6;
    Kabasira->checkRange.x  = 0x60000;
    Kabasira->checkRange.y  = 0x60000;
    Kabasira->sfxPon        = RSDK.GetSFX("Stage/Pon.wav");
    Kabasira->sfxExplosion2 = RSDK.GetSFX("Stage/Explosion2.wav");
    DEBUGMODE_ADD_OBJ(Kabasira);
}

void Kabasira_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Kabasira, NULL, entity->position.x, entity->position.y);
}

void Kabasira_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Kabasira->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

bool32 Kabasira_HandleAnimations(uint8 angle)
{
    RSDK_THIS(Kabasira);

    int rotation = 2 * angle;
    int offset   = entity->animator2.frameID % 3;
    int val      = 0;
    switch (rotation >> 7) {
        case 0:
        case 2: val = (rotation >> 5) & 3; break;
        case 1:
        case 3: val = 3 - ((rotation >> 5) & 3); break;
    }

    int startFrame               = 3 * val;
    entity->animator2.loopIndex  = startFrame;
    entity->animator2.frameID    = offset + startFrame;
    entity->animator2.frameCount = startFrame + 3;
    return rotation > 0x80 && rotation < 0x180;
}

void Kabasira_DrawSegment(int32 angle, int32 alpha)
{
    RSDK_THIS(Kabasira);
    Vector2 drawPos;

    int rotation = 2 * angle;
    drawPos.x    = 0xA00 * RSDK.Sin512(rotation) + entity->startPos.x;
    drawPos.y    = (RSDK.Cos512(angle) << 13) + entity->startPos.y;
    RSDK.SetSpriteAnimation(Kabasira->aniFrames, 0, &entity->animator1, true, (11 - rotation % 512 / 42 % 12));

    int scale = 0;
    if ((uint32)(entity->animator1.frameID - 1) > 5) {
        entity->scale.x = 0x200;
        entity->scale.y = 0x200;
    }
    else {
        entity->scale.x = ((abs(2 * entity->animator1.frameID - 7) << 5) / 5) + 480;
        entity->scale.y = entity->scale.x;
    }

    bool32 dir      = Kabasira_HandleAnimations(angle);
    int storedDir   = entity->direction;
    int storedAlpha = entity->alpha;
    if (entity->angle < 256) {
        entity->direction = FLIP_NONE;
        entity->alpha     = alpha;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        entity->direction = dir;
        alpha >>= 1;
        entity->alpha = alpha;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    }
    else {
        entity->direction = dir;
        entity->alpha     = alpha >> 1;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->direction = FLIP_NONE;
        entity->alpha     = alpha;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    }

    entity->alpha     = storedAlpha;
    entity->direction = storedDir;
}

void Kabasira_CheckPlayerCollisions(void)
{
    RSDK_THIS(Kabasira);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Kabasira->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Kabasira_State_Setup(void)
{
    RSDK_THIS(Kabasira);

    entity->active = ACTIVE_NORMAL;
    entity->angle  = 0;
    entity->timer  = 0;
    entity->state  = Kabasira_State_Unknown1;
    Kabasira_State_Unknown1();
}

void Kabasira_State_Unknown1(void)
{
    RSDK_THIS(Kabasira);

    RSDK.ProcessAnimation(&entity->animator2);

    entity->angle += 2;
    if (entity->timer > 0) {
        entity->timer--;
    }
    else {
        entity->timer = 240;
        if (RSDK.CheckOnScreen(entity, &Kabasira->checkRange)) {
            EntityPlayer *player = Player_GetNearestPlayer();
            if (player) {
                RSDK.PlaySfx(Kabasira->sfxPon, false, 255);
                EntityKabasira *child = CREATE_ENTITY(Kabasira, intToVoid(true), entity->position.x, entity->position.y);
                int angle             = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                child->velocity.x     = RSDK.Cos256(angle) << 9;
                child->velocity.y     = RSDK.Sin256(angle) << 9;
                child->direction      = player->position.x < entity->position.x;
            }
        }
    }

    entity->position.y = (RSDK.Cos512(entity->angle) << 13) + entity->startPos.y;
    entity->position.x = 0xA00 * RSDK.Sin512(2 * entity->angle) + entity->startPos.x;
    Kabasira_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Kabasira_Create(NULL);
    }
}

void Kabasira_State2_Unknown(void)
{
    RSDK_THIS(Kabasira);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Kabasira->hitbox)) {
            if (Player_CheckAttacking(player, entity)) {
                CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
                RSDK.PlaySfx(Kabasira->sfxExplosion2, false, 255);
                destroyEntity(entity);
                foreach_break;
            }
            else
                Player_CheckHit(player, entity);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Kabasira_EditorDraw(void)
{
    RSDK_THIS(Kabasira);
    RSDK.SetSpriteAnimation(Kabasira->aniFrames, 0, &entity->animator1, false, 0);
    RSDK.SetSpriteAnimation(Kabasira->aniFrames, 1, &entity->animator2, false, 0);
    entity->startPos = entity->position;
    entity->startDir = entity->direction;

    Kabasira_Draw();
}

void Kabasira_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Kabasira->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Kabasira.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Kabasira->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Kabasira.bin", SCOPE_STAGE);
}
#endif

void Kabasira_Serialize(void) { RSDK_EDITABLE_VAR(Kabasira, VAR_UINT8, direction); }
