package com.google.android.gms;

/**
 * GooglePlus代理接口
 */
public interface IGooglePlusActivityer {
	public static final int REQUEST_CODE_SIGN_IN_GOOGLE_PLUS = 11000;
	public static final int REQUEST_CODE_RECOVER_FROM_AUTH_ERROR = 11001;
	public static final int REQUEST_CODE_RECOVER_FROM_PLAY_SERVICES_ERROR=1002;
	public void setOnResponseListener_GooglePlus(OnResponseListener_GooglePlus listener);

}
