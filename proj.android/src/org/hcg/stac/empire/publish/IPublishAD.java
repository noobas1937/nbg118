package org.hcg.stac.empire.publish;

import android.app.Activity;
import android.view.View;

public interface IPublishAD {
   
	/**
	 * 显示积分墙
	 * 
	 */
	public void showPublishOffersAd(OnSucessGetPointListener listener);

	/**
	 * 加广告条
	 * 
	 * @param ctx
	 * @param layout
	 * @param isBottom
	 */
	public View createPublishAdBanner(Activity ctx);
    
	public void onCreate(Activity context);
	public void onResume();
	public void onDestroy(Activity context);
	
	public static interface OnSucessGetPointListener{
		public void onSucessGetPoint(int value,String tag);
	}
}
