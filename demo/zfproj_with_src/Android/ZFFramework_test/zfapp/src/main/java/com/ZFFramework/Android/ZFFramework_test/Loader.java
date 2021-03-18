package com.ZFFramework.Android.ZFFramework_test;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import com.ZFFramework.Android.ZF_impl.ZFMainEntry;

public class Loader extends Activity {
    static {
        System.loadLibrary("ZFFramework_test");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(BuildConfig.DEBUG) {
            ZFMainEntry.debugMode(true);
        }
    }
    private boolean _firstTime = true;
    @Override
    protected void onResume() {
        super.onResume();
        if(_firstTime) {
            _firstTime = false;
            Intent intent = new Intent(Loader.this, ZFMainEntry.class);
            startActivity(intent);
            Loader.this.finish();
        }
    }
}

