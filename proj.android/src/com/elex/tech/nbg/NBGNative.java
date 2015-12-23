package com.elex.tech.nbg;

public class NBGNative {
	public static native void nativeSetCountryCode(String country);
	public native static void nativeSetPlatformToken(String platformToken);
	public native static void nativeSetPlatformUID(String platformUID);
	public native static void nativeSetGcmRegisterId(String s);
	public native static void nativeSetAdReferrer(String s);
	public native static void nativeSetAppInfo(String packageNames, String versionName, String versionCode);
}
