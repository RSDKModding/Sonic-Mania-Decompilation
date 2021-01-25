#ifndef SPRITE_H
#define SPRITE_H


enum Scopes {
	SCOPE_NONE,
	SCOPE_GLOBAL,
	SCOPE_STAGE,
};

struct EntityAnimationData {
    void *framePtrs;
    int frameID;
    short animationID;
    short prevAnimationID;
    short animationSpeed;
    short animationTimer;
    short frameDelay;
    short frameCount;
    byte loopIndex;
    byte rotationFlag;
};
#endif // SPRITE_H