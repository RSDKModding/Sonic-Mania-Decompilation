#include "SonicMania.h"

ObjectFirework *Firework;

void Firework_Update(void)
{
    RSDK_THIS(Firework);

    if (entity->animator1.animationID < 3) {
        RSDK.ProcessAnimation(&entity->animator1);
    }
    else if (entity->timer == 10) {
        if (entity->animator1.frameID < entity->animator1.frameCount - 1)
            RSDK.ProcessAnimation(&entity->animator1);
        if (entity->animator2.frameID < entity->animator2.frameCount - 1)
            RSDK.ProcessAnimation(&entity->animator2);
    }

    StateMachine_Run(entity->state);
    Firework_HandlePlayerTimers();
}

void Firework_LateUpdate(void) {}

void Firework_StaticUpdate(void)
{

    bool32 flag = false;

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {
        foreach_active(Firework, firework)
        {
            if (firework->field_A0) {
                if (!Firework->playingFlameSfx) {
                    RSDK.PlaySfx(Firework->sfxFlame, true, 255);
                    Firework->playingFlameSfx = true;
                }
                flag = true;
                foreach_break;
            }
        }
    }

    if (!flag) {
        if (Firework->playingFlameSfx) {
            RSDK.StopSFX(Firework->sfxFlame);
            Firework->playingFlameSfx = false;
        }
    }
}

void Firework_Draw(void)
{
    RSDK_THIS(Firework);

    entity->rotation = ((entity->angle >> 1) + 128) & 0x1FF;
    if (entity->state == Firework_State_Unknown4) {
        if (entity->timer > 0)
            RSDK.FillScreen(0xF0F0F0, entity->timer - 128, entity->timer - 256, entity->timer);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, &entity->position, false);
    }
    if (entity->animator1.animationID >= 3) {
        entity->inkEffect = INK_ADD;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->inkEffect = INK_NONE;
    }
}

void Firework_Create(void *data)
{
    RSDK_THIS(Firework);

    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->drawFX        = FX_ROTATE | FX_FLIP;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (!entity->innerRadius)
        entity->innerRadius = 64;
    if (!entity->outerRadius)
        entity->outerRadius = 128;
    if (!entity->distance)
        entity->distance = 512;
    entity->startPos = entity->position;
    entity->state    = Firework_State_Setup;
}

void Firework_StageLoad(void)
{
    Firework->aniFrames      = RSDK.LoadSpriteAnimation("SSZ2/Firework.bin", SCOPE_STAGE);
    Firework->hitbox1.left   = -11;
    Firework->hitbox1.top    = -32;
    Firework->hitbox1.right  = 10;
    Firework->hitbox1.bottom = 15;
    Firework->hitbox2.left   = -10;
    Firework->hitbox2.top    = -10;
    Firework->hitbox2.right  = 10;
    Firework->hitbox2.bottom = 10;
    Firework->sfxRocketJet   = RSDK.GetSFX("Stage/RocketJet.wav");
    Firework->sfxFlame       = RSDK.GetSFX("Stage/Flame2.wav");
    Firework->sfxExplosion2  = RSDK.GetSFX("Stage/Explosion2.wav");
    Firework->sfxExplosion   = RSDK.GetSFX("Stage/Explosion.wav");
    Firework->active         = ACTIVE_ALWAYS;
}

void Firework_CheckPlayerCollisions(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);

        if (!((1 << playerID) & entity->activePlayers)) {

            if (player->state != Player_State_None && !entity->playerTimers[playerID]
                && Player_CheckCollisionTouch(player, entity, &Firework->hitbox1)) {
                player->tileCollisions = false;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_PULLEYHOLD, &player->playerAnimator, false, 0);
                player->state = Player_State_None;
                entity->activePlayers |= 1 << playerID;
                RSDK.PlaySfx(Player->sfx_Grab, false, 255);
                player->direction = player->position.x > entity->position.x;
                if (player->sidekick == false)
                    entity->activated = true;
            }
        }

        if ((1 << playerID) & entity->activePlayers) {
            if (player->state == Player_State_None) {
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->angle      = entity->angle >> 1;

                if (player->direction == FLIP_NONE)
                    player->rotation = ((entity->angle >> 1) + 0xC0) % 511;
                else
                    player->rotation = ((entity->angle >> 1) + 0x444) % 511;

                player->position.x = entity->position.x;
                player->position.y = entity->position.y;

                int power = -16;
                if (player->direction == FLIP_NONE)
                    power = 16;

                player->position.x += power * (RSDK.Sin1024(entity->angle) << 6);
                player->position.y -= power * (RSDK.Cos1024(entity->angle) << 6);
                player->position.x += -0x100 * RSDK.Cos1024(entity->angle);
                player->position.y -= -0x100 * RSDK.Sin1024(entity->angle);
            }
            else {
                entity->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void Firework_HandlePlayerTimers(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (entity->playerTimers[playerID])
            entity->playerTimers[playerID]--;
    }
}

void Firework_RemovePlayers(bool32 hurt)
{
    foreach_active(Player, player)
    {
        RSDK.GetEntityID(player);
        Firework_HandlePlayerRemoval(player, hurt);
    }
}

void Firework_HandlePlayerRemoval(void *p, bool32 hurt)
{
    RSDK_THIS(Firework);
    EntityPlayer *player = (EntityPlayer *)p;

    int playerID = RSDK.GetEntityID(player);
    if ((1 << playerID) & entity->activePlayers) {
        player->state                  = Player_State_Air;
        player->onGround               = false;
        player->tileCollisions         = true;
        player->velocity               = entity->velocity;
        entity->playerTimers[playerID] = 30;
        if (hurt) {
            bool32 isSidekick = player->sidekick;
            player->sidekick  = true;
            Player_Hit(player);
            player->sidekick = isSidekick;
        }
        else {
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
        }
        entity->activePlayers &= ~(1 << playerID);
    }
}

void Firework_HandlePlayerJump(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        if ((1 << RSDK.GetEntityID(player)) & entity->activePlayers && player->jumpPress) {
            int playerID = RSDK.GetEntityID(player);

            if ((1 << playerID) & entity->activePlayers) {
                player->state                  = Player_State_Air;
                player->onGround               = false;
                player->tileCollisions         = true;
                player->velocity.x             = entity->velocity.x;
                player->velocity.y             = entity->velocity.y;
                entity->playerTimers[playerID] = 30;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                entity->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void Firework_HandlePlayerControl(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        if (!player->sidekick && (1 << RSDK.GetEntityID(player)) & entity->activePlayers) {
            if (player->left) {
                --entity->angle;
            }
            else if (player->right) {
                ++entity->angle;
            }
        }
    }
}

void Firework_HandleMoveDir(void)
{
    RSDK_THIS(Firework);

    if (entity->field_88 < 0x80000)
        entity->field_88 += 0x2000;
    if (entity->field_88 > 0x80000)
        entity->field_88 = 0x80000;

    entity->velocity.x = (entity->field_88 >> 10) * RSDK.Cos1024(entity->angle);
    entity->velocity.y = (entity->field_88 >> 10) * RSDK.Sin1024(entity->angle);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    int rx = abs(entity->field_98.x - entity->position.x) >> 16;
    int ry = abs(entity->field_98.y - entity->position.y) >> 16;
    entity->field_8C += MathHelpers_SquareRoot(rx * rx + ry * ry);
    entity->field_98.x = entity->position.x;
    entity->field_98.y = entity->position.y;
}

void Firework_HandleDebrisSpawn(void)
{
    RSDK_THIS(Firework);

    if (!(entity->timer % 5)) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_LightningSpark, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->position.x -= 0x600 * RSDK.Cos1024(entity->angle);
        debris->position.y -= 0x600 * RSDK.Sin1024(entity->angle);
        debris->drawOrder = Zone->drawOrderLow;
        debris->timer     = 52;
    }
}

void Firework_HandleTileCollisions(void)
{
    RSDK_THIS(Firework);

    bool32 collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_LWALL, 0, Firework->hitbox2.right << 13, 0, 4);
    collided |= RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, Firework->hitbox2.top << 13, 4);
    collided |= RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_RWALL, 0, Firework->hitbox2.left << 13, 0, 4);

    if (collided) {
        Camera_ShakeScreen(0, 0, 4);
        Firework_HandleRideEnd(true);
    }
}

void Firework_HandleRideEnd(bool32 hurt)
{
    RSDK_THIS(Firework);

    entity->field_A0 = false;
    if (hurt) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(Firework->sfxExplosion2, false, 255);
        Firework_RemovePlayers(true);
        entity->state = Firework_State_Unknown4;
    }
    else {
        int angle = 0;

        for (int i = 0; i < 8; ++i) {
            int x                    = (RSDK.Cos1024(angle) << 6) * entity->innerRadius + entity->position.x;
            int y                    = (RSDK.Sin1024(angle) << 6) * entity->innerRadius + entity->position.y;
            EntityFirework *firework = CREATE_ENTITY(Firework, NULL, x, y);
            firework->velocity.x     = 48 * RSDK.Cos1024(angle);
            firework->velocity.y     = 48 * RSDK.Sin1024(angle);
            firework->state          = Firework_State_Unknown5;
            firework->angle          = angle;
            firework->field_7C       = 0;

            x                    = (RSDK.Cos1024(angle) << 6) * entity->outerRadius + entity->position.x;
            y                    = (RSDK.Sin1024(angle) << 6) * entity->outerRadius + entity->position.y;
            firework             = CREATE_ENTITY(Firework, NULL, x, y);
            firework->velocity.x = 32 * RSDK.Cos1024(angle);
            firework->velocity.y = 32 * RSDK.Sin1024(angle);
            firework->angle      = angle;
            firework->state      = Firework_State_Unknown5;
            firework->field_7C   = 1;
            angle += 0x80;
        }

        entity->timer     = 512;
        entity->drawOrder = Zone->hudDrawOrder;
        RSDK.PlaySfx(Firework->sfxExplosion, false, 255);
        Firework_RemovePlayers(false);
        entity->state = Firework_State_Unknown4;
    }
}

void Firework_CheckOnScreen(void)
{
    RSDK_THIS(Firework);

    if (entity->state == Firework_State_Unknown4) {
        if (entity->timer <= 0) {
            if (!RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
                entity->position.x = entity->startPos.x;
                entity->position.y = entity->startPos.y;
                entity->state      = Firework_State_ResetOnScreen;
                entity->visible    = false;
                entity->direction  = FLIP_NONE;
            }
        }
    }
    else {
        if (!RSDK.CheckOnScreen(entity, NULL)) {
            if (!RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
                entity->position.x = entity->startPos.x;
                entity->position.y = entity->startPos.y;
                entity->state      = Firework_State_ResetOnScreen;
                entity->visible    = false;
                entity->direction  = FLIP_NONE;
            }
        }
    }
}

void Firework_HandleAnimations(int a1, int16 angle, Animator *animator1, Animator *animator2, uint8 *direction)
{
    RSDK_THIS(Firework);

    uint16 ang = angle & 0x3FF;
    int anim   = 4 * (a1 != 0) + 3;

    if (ang - 65 > 0x37F) {
        if (direction)
            *direction = FLIP_NONE;
        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim, animator1, true, 0);
    }
    else if (ang <= 0xC0) {
        if (direction)
            *direction = FLIP_Y;

        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, animator1, true, 0);
    }
    else if (ang <= 0x140) {
        if (direction)
            *direction = FLIP_Y;
        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 1, animator1, true, 0);
    }
    else if (ang <= 0x1C0) {
        if (direction)
            *direction = FLIP_XY;
        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, animator1, true, 0);
    }
    else if (ang <= 0x240) {
        if (direction)
            *direction = FLIP_X;

        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim, animator1, true, 0);
    }
    else if (ang <= 0x2C0) {
        if (direction)
            *direction = FLIP_X;
        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, animator1, true, 0);
    }
    else if (ang > 0x340) {
        if (direction)
            *direction = FLIP_NONE;
        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, animator1, true, 0);
    }
    else if (direction)
        *direction = FLIP_NONE;
    else if (animator1)
        RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 1, animator1, true, 0);

    if (animator2)
        RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 3, animator2, true, 0);
}

void Firework_State_Setup(void)
{
    RSDK_THIS(Firework);

    entity->position.x    = entity->startPos.x;
    entity->position.y    = entity->startPos.y;
    entity->field_98.x    = entity->startPos.x;
    entity->field_98.y    = entity->startPos.y;
    entity->visible       = true;
    entity->activePlayers = 0;
    entity->rotation      = 0;
    entity->angle         = 768;
    entity->timer         = 0;
    entity->activated     = false;
    entity->field_88      = 0;
    entity->field_8C      = 0;
    entity->drawOrder     = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(Firework->aniFrames, 0, &entity->animator1, true, 0);
    entity->state = Firework_State_Unknown1;
}

void Firework_State_Unknown1(void)
{
    RSDK_THIS(Firework);

    Firework_CheckPlayerCollisions();
    if (entity->activated) {
        RSDK.PlaySfx(Firework->sfxRocketJet, false, 255);
        RSDK.SetSpriteAnimation(Firework->aniFrames, 1, &entity->animator1, true, 0);
        entity->timer = 30;
        entity->state = Firework_State_Unknown2;
    }
}

void Firework_State_Unknown2(void)
{
    RSDK_THIS(Firework);

    if (entity->timer <= 0) {
        entity->field_A0 = true;
        RSDK.SetSpriteAnimation(Firework->aniFrames, 2, &entity->animator1, true, 0);
        entity->field_98.x = entity->position.x;
        entity->field_98.y = entity->position.y;
        entity->timer      = 0;
        entity->state      = Firework_State_Unknown3;
    }
    else {
        entity->timer--;
    }
    Firework_CheckPlayerCollisions();
    Firework_HandlePlayerJump();
}

void Firework_State_Unknown3(void)
{
    RSDK_THIS(Firework);

    Firework_HandleMoveDir();
    Firework_HandleDebrisSpawn();
    Firework_CheckPlayerCollisions();
    Firework_HandlePlayerJump();
    Firework_HandlePlayerControl();
    if (entity->field_8C > entity->distance)
        Firework_HandleRideEnd(false);
    Firework_HandleTileCollisions();
    Firework_CheckOnScreen();
    ++entity->timer;
}

void Firework_State_Unknown4(void)
{
    RSDK_THIS(Firework);

    if (entity->timer <= 0) {
        entity->activePlayers = 0;
        Firework_CheckOnScreen();
        entity->visible = false;
    }
    else {
        entity->timer -= 0x40;
    }
}

void Firework_State_ResetOnScreen(void)
{
    RSDK_THIS(Firework);

    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->state = Firework_State_Setup;
        Firework_Create(NULL);
    }
}

void Firework_State_Unknown5(void)
{
    RSDK_THIS(Firework);

    entity->visible  = true;
    entity->rotation = 0;
    entity->timer    = 0;
    entity->state    = Firework_State_Unknown6;
}

void Firework_State_Unknown6(void)
{
    RSDK_THIS(Firework);

    if (entity->timer >= 10) {
        if (entity->timer == 10) {
            entity->position.x += entity->velocity.x;
            entity->position.y += entity->velocity.y;
            if (entity->animator1.frameID == entity->animator1.frameCount - 1)
                destroyEntity(entity);
        }
    }
    else {
        Firework_HandleAnimations(entity->field_7C, entity->angle, &entity->animator1, &entity->animator2, &entity->direction);
        ++entity->timer;
    }
}

#if RETRO_INCLUDE_EDITOR
void Firework_EditorDraw(void)
{
    RSDK_THIS(Firework);
    RSDK.SetSpriteAnimation(Firework->aniFrames, 0, &entity->animator1, false, 0);

    Firework_Draw();
}

void Firework_EditorLoad(void) { Firework->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Firework.bin", SCOPE_STAGE); }
#endif

void Firework_Serialize(void)
{
    RSDK_EDITABLE_VAR(Firework, VAR_INT32, distance);
    RSDK_EDITABLE_VAR(Firework, VAR_INT32, innerRadius);
    RSDK_EDITABLE_VAR(Firework, VAR_INT32, outerRadius);
}
