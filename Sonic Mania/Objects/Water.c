#include "../SonicMania.h"

ObjectWater *Water;

void Water_Update()
{

}

void Water_LateUpdate()
{

}

void Water_StaticUpdate()
{

}

void Water_Draw()
{

}

void Water_Create(void* data)
{

}

void Water_StageLoad()
{
    Water->active           = ACTIVE_ALWAYS;
    Water->spriteIndex      = RSDK.LoadSpriteAnimation("Global/Water.bin", SCOPE_STAGE);
    Water->waterLevel       = 0x7FFFFFFF;
    Water->newWaterLevel    = Water->waterLevel;
    Water->targetWaterLevel = Water->waterLevel;
    Water->field_D4         = 0;
    Water->field_D5         = 0;
    Water->hitbox2.left     = -2;
    Water->hitbox2.top      = -2;
    Water->hitbox2.right    = 2;
    Water->hitbox2.bottom   = 2;
    Water->hitbox.left      = -1;
    Water->hitbox.top       = -1;
    Water->hitbox.right     = 1;
    Water->hitbox.bottom    = 1;
    Water->sfx_Splash       = RSDK.GetSFX("Stage/Splash.wav");
    Water->sfx_Breathe      = RSDK.GetSFX("Stage/Breathe.wav");
    Water->sfx_Warning      = RSDK.GetSFX("Stage/Warning.wav");
    Water->sfx_Drown        = RSDK.GetSFX("Stage/Drown.wav");
    Water->sfx_DrownAlert   = RSDK.GetSFX("Stage/DrownAlert.wav");
    Water->sfx_Skim         = RSDK.GetSFX("HCZ/Skim.wav");
    if (RSDK.CheckStageFolder("HCZ")) {
        Water->wakeSprite      = RSDK.LoadSpriteAnimation("HCZ/Wake.bin", SCOPE_STAGE);
        Water->bigBubbleSprite = RSDK.LoadSpriteAnimation("HCZ/BigBubble.bin", SCOPE_STAGE);
        Water->sfx_WaterLevelL = RSDK.GetSFX("HCZ/WaterLevel_L.wav");
        Water->sfx_WaterLevelR = RSDK.GetSFX("HCZ/WaterLevel_R.wav");
        Water->sfx_DNAGrab     = RSDK.GetSFX("CPZ/DNAGrab.wav");
        Water->sfx_DNABurst    = RSDK.GetSFX("CPZ/DNABurst.wav");
        RSDK.SetSpriteAnimation(Water->wakeSprite, 0, &Water->wakeData, true, 0);
    }
}

void Water_EditorDraw()
{

}

void Water_EditorLoad()
{

}

void Water_Serialize()
{
    RSDK_EDITABLE_VAR(Water, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Water, VAR_UINT8, numDuds);
    RSDK_EDITABLE_VAR(Water, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(Water, VAR_VECTOR2, height);
    RSDK_EDITABLE_VAR(Water, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(Water, VAR_ENUM, buttonTag);
    RSDK_EDITABLE_VAR(Water, VAR_UINT8, r);
    RSDK_EDITABLE_VAR(Water, VAR_UINT8, g);
    RSDK_EDITABLE_VAR(Water, VAR_UINT8, b);
    RSDK_EDITABLE_VAR(Water, VAR_UINT8, priority);
    RSDK_EDITABLE_VAR(Water, VAR_BOOL, destroyOnTrigger);
}

