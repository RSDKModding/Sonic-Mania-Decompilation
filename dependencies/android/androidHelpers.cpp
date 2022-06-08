#include <RSDK/Core/RetroEngine.hpp>
#include <main.hpp>

bool32 firstlaunch = true;

JNIEXPORT void jnifuncN(nativeOnPause) {
    // set focus stuff here
}
JNIEXPORT void jnifuncN(nativeOnResume) {

}
JNIEXPORT void jnifunc(nativeOnStart, jstring basepath) {
    char buffer[0x200];
    strcpy(buffer, env->GetStringUTFChars((jstring)basepath, NULL));
    RSDK::SKU::SetUserFileCallbacks(buffer, NULL, NULL);

}
JNIEXPORT void jnifuncN(nativeOnStop) {
    // set some sort of exit flag in renderdev
}

JNIEXPORT void jnifunc(nativeSetSurface, jobject surface) {
    if (surface) {
        RSDK::RenderDevice::window = ANativeWindow_fromSurface(env, surface);
        if (firstlaunch) {
            firstlaunch = false;
            RSDK_main(0, NULL, (void *)RSDK::LinkGameLogic);
        }
    } else {
        ANativeWindow_release(RSDK::RenderDevice::window);
    }
}