package com.decomp.rsdkv5;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.os.Environment;
import android.view.Surface;
import android.view.SurfaceHolder;

import java.io.File;

public class RSDKv5 extends Activity implements SurfaceHolder.Callback {
    @Override
    protected void onStart() {
        super.onStart();
        nativeOnStart(getBasePath());
    }

    @Override
    protected void onPause() {
        super.onPause();
        nativeOnPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        nativeOnResume();
    }


    protected void onStop() {
        super.onStop();
        nativeOnStop();
    }

    public String getBasePath() {
        Context c = getApplicationContext();
        requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE}, 0);
        String p = Environment.getExternalStorageDirectory().getAbsolutePath() + "/RSDK/v5";
        //getExternalStorageDirectory is deprecated. I do not care.
        new File(p).mkdirs();
        return p + "/";
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int format, int w, int h) {
        nativeSetSurface(surfaceHolder.getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        nativeSetSurface(null);
    }


    // NATIVE START
    static {
        System.loadLibrary("RSDK");
    }
    // RENDER
    public static native void nativeOnStart(String basepath);
    public static native void nativeOnResume();
    public static native void nativeOnPause();
    public static native void nativeOnStop();
    public static native void nativeSetSurface(Surface surface);
    // INPUT
    // AUDIO
}
