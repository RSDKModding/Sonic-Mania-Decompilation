#ifndef OBJ_WALLCRAWL_H
#define OBJ_WALLCRAWL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWallCrawl;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWallCrawl;

// Object Entity
extern ObjectWallCrawl *WallCrawl;

// Standard Entity Events
void WallCrawl_Update(void);
void WallCrawl_LateUpdate(void);
void WallCrawl_StaticUpdate(void);
void WallCrawl_Draw(void);
void WallCrawl_Create(void* data);
void WallCrawl_StageLoad(void);
void WallCrawl_EditorDraw(void);
void WallCrawl_EditorLoad(void);
void WallCrawl_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_WALLCRAWL_H
