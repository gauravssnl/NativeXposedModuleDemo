package com.gauravssnl.nativexposedhookdemo;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class Xposed implements IXposedHookLoadPackage {
    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam lpparam) throws Throwable {
        XposedBridge.log("Package :: " + lpparam.packageName);
       if (!lpparam.packageName.contains("com.gauravssnl")) return;
        try {
            XposedBridge.log("Loading native lib");
            System.loadLibrary("nativexposedhookdemo");
            XposedBridge.log("nativexposedhookdemo lib loaded");
        } catch (Exception ex) {
            XposedBridge.log("Exception occurred ::" + ex);
        }
    }
}
