package com.google.android.gms;

import android.content.Intent;

public interface OnResponseListener_GooglePlus {

	public boolean handleActivityResult(int requestCode, int resultCode,
			Intent data);
}
