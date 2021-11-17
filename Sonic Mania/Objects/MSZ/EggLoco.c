#include "SonicMania.h"

ObjectEggLoco *EggLoco;

void EggLoco_Update(void)
{
    RSDK_THIS(EggLoco);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator10);
    RSDK.ProcessAnimation(&entity->animator7);
    entity->field_60 = (entity->field_60 + 16) & 0x1FF;

    StateMachine_Run(entity->state);

    if (!(Zone->timer & 0x3F)) {
        RSDK.PlaySfx(EggLoco->sfxSmoke, false, 255);
        CREATE_ENTITY(LocoSmoke, NULL, entity->position.x + 0x450000, entity->position.y - 0x480000);
    }
}

void EggLoco_LateUpdate(void) {}

void EggLoco_StaticUpdate(void) {}

void EggLoco_Draw(void)
{
    RSDK_THIS(EggLoco);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y - 2 * entity->field_64;
    RSDK.DrawSprite(&entity->animator2, 0, false);
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    drawPos.x                  = entity->position.x;
    drawPos.x                  = entity->position.x - 0x2D0000;
    drawPos.y                  = entity->position.y - 0x3F0000;
    entity->drawFX             = FX_ROTATE;
    entity->rotation           = -(entity->field_64 >> 15);
    entity->animator12.frameID = 0;
    RSDK.DrawSprite(&entity->animator12, &drawPos, false);

    drawPos.x                  = entity->position.x - 0x5F0000;
    drawPos.y                  = entity->field_64 + entity->position.y - 0x3F0000;
    entity->drawFX             = FX_NONE;
    entity->animator12.frameID = 1;
    RSDK.DrawSprite(&entity->animator12, &drawPos, false);

    entity->animator12.frameID = 2;
    RSDK.DrawSprite(&entity->animator12, &drawPos, false);
    RSDK.DrawSprite(&entity->animator2, 0, false);
    RSDK.DrawSprite(&entity->animator1, 0, false);
    RSDK.DrawSprite(&entity->animator3, 0, false);

    drawPos.x        = entity->position.x - 0x680000;
    drawPos.y        = entity->position.y + 0x380000;
    entity->rotation = entity->field_60;
    entity->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&entity->animator5, &drawPos, false);

    entity->drawFX = FX_NONE;
    RSDK.DrawSprite(&entity->animator6, &drawPos, false);

    drawPos.x += 0x4A0000;
    entity->drawFX   = FX_ROTATE;
    entity->rotation = entity->field_60 + 256;
    RSDK.DrawSprite(&entity->animator5, &drawPos, false);

    entity->drawFX = FX_NONE;
    RSDK.DrawSprite(&entity->animator6, &drawPos, false);

    drawPos.x = entity->position.x + 0x160000;
    drawPos.y = entity->position.y + 0x4B0000;
    RSDK.DrawSprite(&entity->animator7, &drawPos, false);

    drawPos.x += 0x3E0000;
    RSDK.DrawSprite(&entity->animator7, &drawPos, false);

    drawPos.x                 = (2304 * RSDK.Cos512(entity->field_60) + entity->position.x - 0x680000) + 0x4C0000;
    drawPos.y                 = ((9 * RSDK.Sin512(entity->field_60)) << 8) + entity->position.y + 0x380000;
    entity->animator8.frameID = 1;
    entity->drawFX            = FX_ROTATE;
    entity->rotation          = RSDK.Sin512(-entity->field_60) / 18 - 4;
    RSDK.DrawSprite(&entity->animator8, &drawPos, false);

    entity->drawFX = FX_NONE;
    drawPos.x -= 0x4C0000;
    entity->animator8.frameID = 0;
    RSDK.DrawSprite(&entity->animator8, &drawPos, false);

    entity->animator11.frameID = 0;
    drawPos.x                  = entity->position.x + ((RSDK.Cos512(entity->field_60) + 256) << 11);
    drawPos.y                  = entity->position.y;
    RSDK.DrawSprite(&entity->animator11, &drawPos, false);

    entity->animator11.frameID = 2;
    RSDK.DrawSprite(&entity->animator11, &drawPos, false);

    entity->animator11.frameID = 1;
    RSDK.DrawSprite(&entity->animator11, 0, false);
    RSDK.DrawSprite(&entity->animator9, 0, false);
    RSDK.DrawSprite(&entity->animator10, 0, false);
}

void EggLoco_Create(void *data)
{
    RSDK_THIS(EggLoco);
    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0xA00000;
        entity->updateRange.y = 0x600000;
        entity->state         = EggLoco_State_Unknown1;
        entity->timer         = 60;
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 0, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 6, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 1, &entity->animator5, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 1, &entity->animator6, true, 1);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 2, &entity->animator7, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 3, &entity->animator8, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 5, &entity->animator9, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 7, &entity->animator10, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 4, &entity->animator11, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 10, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 12, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 13, &entity->animator12, true, 0);
    }
}

void EggLoco_StageLoad(void)
{
    EggLoco->aniFrames  = RSDK.LoadSpriteAnimation("MSZ/Train.bin", SCOPE_STAGE);
    EggLoco->sfxSmoke   = RSDK.GetSFX("MSZ/LocoSmoke.wav");
    EggLoco->sfxWhistle = RSDK.GetSFX("MSZ/LocoWhistle.wav");
    Soundboard_LoadSFX("MSZ/LocoChugga.wav", true, EggLoco_CheckCB, EggLoco_UpdateCB);
}

bool32 EggLoco_CheckCB(void)
{
    int32 count   = 0;
    int32 screenX = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
    int32 screenY = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;
    foreach_all(EggLoco, eggLoco)
    {
        int32 distX = abs(screenX - eggLoco->position.x);
        int32 distY = abs(screenY - eggLoco->position.y);
        int32 rad   = MathHelpers_SquareRoot((distY >> 16) * (distY >> 16) + (distX >> 16) * (distX >> 16));
        if (rad <= 840)
            count++;
    }
    return count > 0;
}

void EggLoco_UpdateCB(int32 sfx)
{
    int32 screenX = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
    int32 screenY = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;
    foreach_all(EggLoco, eggLoco)
    {
        int32 distX = abs(screenX - eggLoco->position.x);
        int32 distY = abs(screenY - eggLoco->position.y);
        int32 vol   = minVal(MathHelpers_SquareRoot((distX >> 16) * (distX >> 16) + (distY >> 16) * (distY >> 16)), 840);
        RSDK.SetChannelAttributes(Soundboard->sfxChannel[sfx], 1.0 - (vol / 840.0), 0.0, 1.0);
        foreach_break;
    }
}

void EggLoco_State_Unknown1(void)
{
    RSDK_THIS(EggLoco);

    if (entity->timer <= 0) {
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 11, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 12, &entity->animator2, true, 0);
        entity->state = EggLoco_State_Unknown2;
    }
    else {
        entity->timer--;
    }
}

void EggLoco_State_Unknown2(void)
{
    RSDK_THIS(EggLoco);
    if (++entity->timer == 4) {
        RSDK.PlaySfx(EggLoco->sfxWhistle, false, 255);
        entity->timer = 0;
        entity->state = EggLoco_State_Unknown3;
    }
}

void EggLoco_State_Unknown3(void)
{
    RSDK_THIS(EggLoco);

    RSDK.ProcessAnimation(&entity->animator2);
    entity->field_64 = (RSDK.GetFrameID(&entity->animator2) - 97) << 16;
    if (entity->field_64 >= 0x10000 && !(Zone->timer & 3)) {
        EntityDust *dust = CREATE_ENTITY(Dust, NULL, entity->position.x - 0x240000, entity->position.y - 0x4E0000);
        dust->state      = Dust_State_EggLoco;
        dust->velocity.x = 0x20000;
        dust->velocity.y = -0x18000;
        dust->drawOrder  = Zone->drawOrderLow;
    }
    if (entity->animator1.animationID == 11 && entity->animator1.frameID == entity->animator1.frameCount - 1)
        RSDK.SetSpriteAnimation(EggLoco->aniFrames, 10, &entity->animator1, true, 0);

    if (entity->animator2.frameID == entity->animator2.frameCount - 1) {
        entity->timer = RSDK.Rand(60, 180);
        entity->state = EggLoco_State_Unknown1;
    }
}

#if RETRO_INCLUDE_EDITOR
void EggLoco_EditorDraw(void) {}

void EggLoco_EditorLoad(void) {}
#endif

void EggLoco_Serialize(void) {}
