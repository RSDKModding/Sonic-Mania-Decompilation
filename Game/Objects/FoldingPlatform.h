#ifndef OBJ_FOLDINGPLATFORM_H
#define OBJ_FOLDINGPLATFORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectFoldingPlatform : public Object {
public:

};

//Entity Class
class EntityFoldingPlatform : public Entity {
public:

};

//Object Entity
ObjectFoldingPlatform FoldingPlatform;

//Entity Functions
void FoldingPlatform_Update();
void FoldingPlatform_EarlyUpdate();
void FoldingPlatform_LateUpdate();
void FoldingPlatform_Draw();
void FoldingPlatform_Setup(void* subtype);
void FoldingPlatform_StageLoad();
void FoldingPlatform_GetAttributes();

#endif //!OBJ_FOLDINGPLATFORM_H
