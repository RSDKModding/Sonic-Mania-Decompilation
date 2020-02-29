#ifndef OBJ_CHEMBUBBLE_H
#define OBJ_CHEMBUBBLE_H

//Object Class
class ChemBubble : Object {

};

//Entity Class
class EntityChemBubble : Entity {

};

//Entity Functions
void ChemBubble_Update();
void ChemBubble_EarlyUpdate();
void ChemBubble_LateUpdate();
void ChemBubble_Draw();
void ChemBubble_Setup(void* subtype);
void ChemBubble_StageLoad();
void ChemBubble_GetAttributes();

#endif //!OBJ_CHEMBUBBLE_H
