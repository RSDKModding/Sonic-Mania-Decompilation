#include "RetroEngine.hpp"

ObjectDefaultObject DefaultObject;

void DefaultObject_Update()
{
    //if (Key_Up) {
    //    if (unkY > 0)
    //        unkY -= 4;
    //}
    //else if (Key_Down) {
    //    unkY += 4;
    //}
    //if (Key_Left) {
    //    if (unkX > 0)
    //        unkX -= 4;
    //}
    //else if (Key_Right) {
    //    unkX += 4;
    //}
}

void DefaultObject_Create(void* data)
{
    EntityDefaultObject *entity = (EntityDefaultObject *)sceneInfo.entity;
    entity->priority            = ACTIVE_ALWAYS;
}

