#include "SonicMania.h"

ObjectHotaruMKII *HotaruMKII;

void HotaruMKII_Update(void)
{
    RSDK_THIS(HotaruMKII);
    StateMachine_Run(entity->state);
}

void HotaruMKII_LateUpdate(void) {}

void HotaruMKII_StaticUpdate(void) {}

void HotaruMKII_Draw(void)
{
    RSDK_THIS(HotaruMKII);
    if (entity->state != HotaruMKII_State_Unknown1) {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        if (entity->state != HotaruMKII_State_Unknown2 && ((Zone->timer & 1) || entity->state == HotaruMKII_State_Unknown5)) {
            int ink           = entity->inkEffect;
            entity->inkEffect = INK_ALPHA;
            RSDK.DrawSprite(&entity->animator2, NULL, false);
            entity->inkEffect = ink;
        }
    }
}

void HotaruMKII_Create(void *data)
{
    RSDK_THIS(HotaruMKII);
    if (!SceneInfo->inEditor) {
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->startPos      = entity->position;
        entity->startDir      = entity->direction;
        entity->updateRange.x = 0x1000000;
        entity->updateRange.y = 0x1000000;
        entity->drawFX        = FX_FLIP;
        if (!entity->triggerSize.x) {
            entity->triggerSize.x = 0x1000000;
            entity->triggerSize.y = 0x1000000;
        }
        entity->hitboxTrigger.right  = entity->triggerSize.x >> 17;
        entity->hitboxTrigger.left   = -entity->hitboxTrigger.right;
        entity->hitboxTrigger.bottom = entity->triggerSize.y >> 17;
        entity->hitboxTrigger.top    = -entity->hitboxTrigger.bottom;
        entity->type                 = voidToInt(data);

        switch (entity->type) {
            default: break;
            case 0:
                entity->active    = ACTIVE_BOUNDS;
                entity->inkEffect = INK_NONE;
                entity->visible   = true;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &entity->animator2, true, 0);
                entity->state = HotaruMKII_State_Setup;
                break;
            case 1:
                entity->active    = ACTIVE_NORMAL;
                entity->inkEffect = INK_ALPHA;
                entity->visible   = true;
                entity->alpha     = 0x80;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &entity->animator1, true, 0);
                entity->state = HotaruMKII_State1_Unknown;
                break;
            case 2:
                --entity->drawOrder;
                entity->active    = ACTIVE_NORMAL;
                entity->inkEffect = INK_ALPHA;
                entity->visible   = true;
                entity->alpha     = 0x80;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 2, &entity->animator1, true, 0);
                entity->state = HotaruMKII_State2_Unknown1;
                break;
        }
    }
}

void HotaruMKII_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/HotaruMKII.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/HotaruMKII.bin", SCOPE_STAGE);

    HotaruMKII->hitbox1.top    = -6;
    HotaruMKII->hitbox1.left   = -6;
    HotaruMKII->hitbox1.right  = 6;
    HotaruMKII->hitbox1.bottom = 6;
    HotaruMKII->hitbox2.top    = -8;
    HotaruMKII->hitbox2.left   = -8;
    HotaruMKII->hitbox2.right  = 8;
    HotaruMKII->hitbox2.bottom = 8;

    HotaruMKII->sfxLaser  = RSDK.GetSFX("SSZ1/HotaruLaser.wav");
    HotaruMKII->sfxAppear = RSDK.GetSFX("SSZ1/HotaruAppear.wav");
    HotaruMKII->sfxFly    = RSDK.GetSFX("SSZ1/HotaruFly.wav");
    HotaruMKII->sfxCharge = RSDK.GetSFX("SSZ1/HotaruCharge.wav");
    DEBUGMODE_ADD_OBJ(HotaruMKII);
}

void HotaruMKII_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityHotaruMKII *hotaruMKII = CREATE_ENTITY(HotaruMKII, NULL, entity->position.x, entity->position.y);
    hotaruMKII->origin           = RSDK.Rand(0, 0x100);
    hotaruMKII->offset1.x        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0x100);
    hotaruMKII->offset1.y        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset2.x        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset2.y        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset3.x        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset3.y        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
}

void HotaruMKII_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void HotaruMKII_CheckPlayerCollisions(void)
{
    RSDK_THIS(HotaruMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &HotaruMKII->hitbox1))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void HotaruMKII_CheckOnScreen(void)
{
    RSDK_THIS(HotaruMKII);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        entity->offsetID   = 0;
        HotaruMKII_Create(NULL);
    }
}

void HotaruMKII_HandleDistances(void *p)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)p;

    if (Player_CheckValidState(player)) {
        int distX = entity->distance.x;
        switch (entity->offsetID) {
            case 0:
                entity->distance.x = entity->offset1.x;
                entity->distance.y = entity->offset1.y;
                break;
            case 1:
                entity->distance.x = entity->offset2.x;
                entity->distance.y = entity->offset2.y;
                break;
            case 2:
                entity->distance.x = entity->offset3.x;
                entity->distance.y = entity->offset3.y;
                break;
            case 3:
                entity->distance.x = 0;
                entity->distance.y = 0;
                break;
            default: break;
        }

        entity->distance.x &= 0xFFFF0000;
        entity->distance.y &= 0xFFFF0000;
        if (!entity->distance.x && !entity->distance.y) {
            entity->distance.x = distX;
            entity->distance.y = 0xB0010000;
        }
        int angle =
            RSDK.ATan2(player->position.x + entity->distance.x - entity->position.x, entity->distance.y + player->position.y - entity->position.y);

        entity->field_6C = 0x300 * RSDK.Cos256(angle);
        entity->field_70 = 0x300 * RSDK.Sin256(angle);
        entity->velocity.x += entity->field_6C;
        entity->velocity.y += entity->field_70;
        ++entity->offsetID;
    }
    else {
        entity->playerPtr  = NULL;
        entity->offsetID   = 4;
        entity->velocity.x = 0;
        entity->velocity.y = -0x30000;
        entity->state      = HotaruMKII_State_Unknown6;
    }
}

void HotaruMKII_State_Setup(void)
{
    RSDK_THIS(HotaruMKII);

    entity->active     = ACTIVE_NORMAL;
    entity->childCount = 0;
    entity->state      = HotaruMKII_State_Unknown1;
    HotaruMKII_State_Unknown1();
}

void HotaruMKII_State_Unknown1(void)
{
    RSDK_THIS(HotaruMKII);

    bool32 flag = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitboxTrigger)) {
            entity->playerPtr = (Entity *)player;
            if (!player->sidekick) {
                flag         = true;
                int screenID = 0;
                if (player->camera)
                    screenID = player->camera->screenID;

                entity->position.x = RSDK.Cos256(entity->origin) << 17;
                entity->position.y = RSDK.Sin256(entity->origin) << 17;
                RSDKScreenInfo *screen = &ScreenInfo[screenID];

                if (entity->position.x > (screen->width & 0xFFFFFFFE) << 15)
                    entity->position.x = (screen->width & 0xFFFFFFFE) << 15;

                if (entity->position.x < -0x8000 * (screen->width & 0xFFFFFFFE))
                    entity->position.x = -0x8000 * (screen->width & 0xFFFFFFFE);

                if (entity->position.y > (screen->height & 0xFFFFFFFE) << 15)
                    entity->position.y = (screen->height & 0xFFFFFFFE) << 15;

                if (entity->position.y < -0x8000 * (screen->height & 0xFFFFFFFE))
                    entity->position.y = -0x8000 * (screen->height & 0xFFFFFFFE);

                if (entity->position.x >= 0)
                    entity->position.x += 0x100000;
                else
                    entity->position.x -= 0x100000;

                if (entity->position.y >= 0)
                    entity->position.y += 0x100000;
                else
                    entity->position.y -= 0x100000;

                entity->position.x += ((screen->width & 0xFFFFFFFE) + 2 * screen->position.x) << 15;
                entity->position.y += ((screen->height & 0xFFFFFFFE) + 2 * screen->position.y) << 15;
                HotaruMKII_HandleDistances(player);
                entity->inkEffect = INK_ALPHA;
                entity->visible   = true;
                entity->alpha     = 128;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &entity->animator1, true, 0);
                RSDK.PlaySfx(HotaruMKII->sfxFly, false, 255);
                entity->state = HotaruMKII_State_Unknown2;
                foreach_break;
            }
        }
    }

    if (!flag)
        HotaruMKII_CheckOnScreen();
}

void HotaruMKII_State_Unknown6(void)
{
    RSDK_THIS(HotaruMKII);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    HotaruMKII_CheckOnScreen();
}

void HotaruMKII_State_Unknown2(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (player && !Player_CheckValidState(player)) {
        HotaruMKII_HandleDistances(player);
    }
    else {
        if ((Zone->timer & 7) == 0) {
            EntityHotaruMKII *child = CREATE_ENTITY(HotaruMKII, intToVoid(1), entity->position.x, entity->position.y);
            child->playerPtr        = entity->playerPtr;
            child->distance.x       = entity->position.x - player->position.x;
            child->distance.y       = entity->position.y - player->position.y;
        }

        int flags = 0;
        if (entity->field_6C < 0) {
            int x = player->position.x + entity->distance.x;
            if (entity->position.x <= x) {
                flags              = 1;
                entity->position.x = x;
            }
        }
        if (entity->field_6C > 0) {
            int x = player->position.x + entity->distance.x;
            if (entity->position.x >= x) {
                flags              = 1;
                entity->position.x = x;
            }
        }

        if (entity->field_70 < 0) {
            int y = player->position.y + entity->distance.y;
            if (entity->position.y <= y) {
                flags++;
                entity->position.y = y;
            }
        }
        if (entity->field_70 > 0) {
            int y = player->position.y + entity->distance.y;
            if (entity->position.y >= y) {
                flags++;
                entity->position.x = y;
            }
        }

        if (flags == 2) {
            entity->inkEffect = INK_NONE;
            entity->alpha     = 0;
            RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 0, &entity->animator1, true, 0);
            entity->timer = 60;
            RSDK.PlaySfx(HotaruMKII->sfxAppear, false, 255);
            entity->state = HotaruMKII_State_Unknown3;
        }
        else {
            RSDK.ProcessAnimation(&entity->animator1);
            int angle          = RSDK.ATan2(player->position.x + entity->distance.x - entity->position.x,
                                   player->position.y + entity->distance.y - entity->position.y);
            entity->field_6C   = 0x300 * RSDK.Cos256(angle);
            entity->velocity.x = 0x300 * RSDK.Cos256(angle) + player->velocity.x;
            entity->field_70   = 0x300 * RSDK.Sin256(angle);
            entity->velocity.y = 0x300 * RSDK.Sin256(angle) + player->velocity.y;
        }
        HotaruMKII_CheckOnScreen();
    }
}

void HotaruMKII_State_Unknown3(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    RSDK.ProcessAnimation(&entity->animator3);
    if (Player_CheckValidState(player)) {
        entity->position.x = player->position.x + entity->distance.x;
        entity->position.y = player->position.y + entity->distance.y;
        if (--entity->timer <= 0) {
            RSDK.PlaySfx(HotaruMKII->sfxCharge, false, 255);
            entity->state = HotaruMKII_State_Unknown4;
        }
        HotaruMKII_CheckOnScreen();
    }
    else {
        HotaruMKII_HandleDistances(player);
    }
}

void HotaruMKII_State_Unknown4(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    RSDK.ProcessAnimation(&entity->animator2);
    if (Player_CheckValidState(player)) {
        entity->position.x = player->position.x + entity->distance.x;
        entity->position.y = player->position.y + entity->distance.y;
        entity->alpha += 2;
        if (entity->alpha == 0x100) {
            entity->timer     = 90;
            entity->alpha     = 256;
            entity->inkEffect = INK_ALPHA;
            entity->state     = HotaruMKII_State_Unknown5;
        }
        HotaruMKII_CheckPlayerCollisions();
        HotaruMKII_CheckOnScreen();
    }
    else {
        HotaruMKII_HandleDistances(player);
    }
}

void HotaruMKII_State_Unknown5(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    RSDK.ProcessAnimation(&entity->animator1);
    if (Player_CheckValidState(player)) {
        if (--entity->timer > 0) {
            if (entity->timer <= 60) {
                if (entity->timer == 60) {
                    RSDK.PlaySfx(HotaruMKII->sfxLaser, false, 255);
                    RSDK.StopSFX(HotaruMKII->sfxCharge);
                }
                entity->alpha -= 3;
                if (!(entity->timer & 3)) {
                    EntityHotaruMKII *child = CREATE_ENTITY(HotaruMKII, intToVoid(2), entity->position.x, entity->position.y + 0xE0000);
                    if (!entity->childCount) {
                        entity->childCount       = 1;
                        child->animator1.frameID = 1;
                    }
                    if (entity->childCount == 1)
                        child->childCount = 1;
                    if (++entity->childCount == 3)
                        entity->childCount = 1;
                }
            }
            RSDK.ProcessAnimation(&entity->animator2);
            HotaruMKII_CheckPlayerCollisions();
            HotaruMKII_CheckOnScreen();
        }
        else {
            HotaruMKII_HandleDistances(player);
            entity->childCount = 0;
            entity->alpha      = 128;
            entity->inkEffect  = INK_ALPHA;
            RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &entity->animator1, true, 0);
            entity->state = HotaruMKII_State_Unknown2;
        }
    }
    else {
        HotaruMKII_HandleDistances(player);
    }
}

void HotaruMKII_State1_Unknown(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    entity->position.x = player->position.x + entity->distance.x;
    entity->position.y = player->position.y + entity->distance.y;
    RSDK.ProcessAnimation(&entity->animator1);
    entity->alpha -= 4;
    if (entity->alpha <= 0)
        destroyEntity(entity);
}

void HotaruMKII_State2_Unknown1(void)
{
    RSDK_THIS(HotaruMKII);

    entity->position.y += 0x40000;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &HotaruMKII->hitbox2)) {
            Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
        }
    }

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
        if (entity->childCount == 1) {
            ++entity->drawOrder;
            entity->position.y += 0x80000;
            entity->inkEffect = INK_NONE;
            RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 3, &entity->animator1, true, 0);
            entity->state = HotaruMKII_State2_Unknown2;
        }
        else {
            destroyEntity(entity);
        }
    }
}

void HotaruMKII_State2_Unknown2(void)
{
    RSDK_THIS(HotaruMKII);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID >= entity->animator1.frameCount - 1)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void HotaruMKII_EditorDraw(void)
{
    RSDK_THIS(HotaruMKII);
    entity->drawOrder     = Zone->drawOrderHigh;
    entity->updateRange.x = 0x1000000;
    entity->updateRange.y = 0x1000000;
    entity->drawFX        = FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->inkEffect     = INK_NONE;
    entity->visible       = true;
    RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 0, &entity->animator1, false, 0);

    RSDK.DrawSprite(&entity->animator1, NULL, false);

    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->triggerSize.x >> 1;
    drawPos.y -= entity->triggerSize.y >> 1;
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x + entity->triggerSize.x, drawPos.y, 0x00FF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, entity->triggerSize.y + drawPos.y, drawPos.x + entity->triggerSize.x, entity->triggerSize.y + drawPos.y, 0x00FF00, 0,
                  INK_NONE, false);
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x, drawPos.y + entity->triggerSize.y, 0x00FF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + entity->triggerSize.x, drawPos.y, drawPos.x + entity->triggerSize.x, drawPos.y + entity->triggerSize.y, 0x00FF00, 0,
                  INK_NONE, false);
}

void HotaruMKII_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/HotaruMKII.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/HotaruMKII.bin", SCOPE_STAGE);
}
#endif

void HotaruMKII_Serialize(void)
{
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_UINT8, origin);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, offset1);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, offset2);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, offset3);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, triggerSize);
}
