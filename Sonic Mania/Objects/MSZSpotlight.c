#include "../SonicMania.h"

ObjectMSZSpotlight *MSZSpotlight;

void MSZSpotlight_Update(void)
{

}

void MSZSpotlight_LateUpdate(void)
{

}

void MSZSpotlight_StaticUpdate(void)
{

}

void MSZSpotlight_Draw(void)
{

}

void MSZSpotlight_Create(void* data)
{

}

void MSZSpotlight_StageLoad(void)
{
    MSZSpotlight->spriteIndex = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);
    RSDK_ACTIVE_VAR(MSZSpotlight, color);
    RSDK.AddVarEnumValue("(nil)");
    RSDK.AddVarEnumValue("Green");
    RSDK.AddVarEnumValue("Blue");
}

void MSZSpotlight_EditorDraw(void)
{

}

void MSZSpotlight_EditorLoad(void)
{

}

void MSZSpotlight_Serialize(void) { RSDK_EDITABLE_VAR(MSZSpotlight, VAR_UINT8, color); }

