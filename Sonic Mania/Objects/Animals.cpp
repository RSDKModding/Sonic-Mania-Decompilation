#include "../SonicMania.hpp"

ObjectAnimals *Animals;

void Animals_Update()
{

}

void Animals_LateUpdate()
{

}

void Animals_StaticUpdate()
{

}

void Animals_Draw()
{

}

void Animals_Create(void* data)
{

}

void Animals_StageLoad()
{
    Animals->spriteIndex = RSDK.LoadAnimation("Global/Animals.bin", SCOPE_STAGE);
    if (RSDK.GetObjectIDByName("Platform"))
        Animals->hasPlatform = true;
    if (RSDK.GetObjectIDByName("Bridge"))
        Animals->hasBridge = true;
}

void Animals_EditorDraw()
{

}

void Animals_EditorLoad()
{

}

void Animals_Serialize()
{
    RSDK_EDITABLE_VAR(Animals, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Animals, VAR_ENUM, behaviour);
}

