package com.elex.chatservice.image;

import android.graphics.Bitmap;

public interface ImageLoaderListener
{
	void onImageLoaded(String url, Bitmap bitmap);
}
