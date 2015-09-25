//package org.hcg.stac.empire.ad;
//
//import android.app.Activity;
//import android.content.Context;
//import android.util.Log;
//import android.view.View;
//
//import org.hcg.util.CommonUtil;
//import org.hcg.stac.empire.common.constant.CommonConst;
//import org.hcg.stac.empire.publish.IPublishAD;
//import com.tapjoy.TapjoyConnect;
//import com.tapjoy.TapjoyEarnedPointsNotifier;
//import com.tapjoy.TapjoyNotifier;
//import com.tapjoy.TapjoySpendPointsNotifier;
//
///**
// * 
// * @author anychen
// * 
// */
//public class AdTapjoyPublisher implements IPublishAD,
//		TapjoyEarnedPointsNotifier, TapjoyNotifier, TapjoySpendPointsNotifier {
//	private static final String TAG = "Tapjoy";
//	private static final String APP_ID = CommonConst.GOLBAL_TAPJOY_APPID;
//	private static final String APP_SECRET = CommonConst.GOLBAL_TAPJOY_SECRET_KEY;
//	private OnSucessGetPointListener mListener;
//	private boolean isOpened;
//	private boolean isEnable;
//
//	public AdTapjoyPublisher(Context context) {
//		try {
//			if(CommonUtil.isVersion_Global_Free()){
//				TapjoyConnect.requestTapjoyConnect(context, CommonConst.GOLBAL_TAPJOY_APPID, CommonConst.GOLBAL_TAPJOY_SECRET_KEY);
//			}else{
//				TapjoyConnect.requestTapjoyConnect(context, CommonConst.GOLBAL_TAPJOY_APPID, CommonConst.GOLBAL_TAPJOY_SECRET_KEY);
//			}
//			// 设置Tapjoy用户 金币增加时的回调类
//			TapjoyConnect.getTapjoyConnectInstance().setEarnedPointsNotifier(
//					this);
//			isEnable = true;
//		} catch (java.lang.OutOfMemoryError e) {
//			isEnable = false;
//		}
//	}
//
//	@Override
//	public void getUpdatePoints(String currencyName, int pointTotal) {
//		// TODO Auto-generated method stub
//
//	}
//
//	@Override
//	public void getUpdatePointsFailed(String error) {
//		// TODO Auto-generated method stub
//
//	}
//
//	private void spendTapPoints(int amount) {
//		TapjoyConnect.getTapjoyConnectInstance().spendTapPoints(amount, this);
//	}
//
//	/**
//	 * Tapjoy用户 金币增加时的回调方法 在此方法中为我们的用户加上相应的金币
//	 * 
//	 * @param amount
//	 *            所增加的金币/钻石数
//	 */
//	@Override
//	public void earnedTapPoints(final int amount) {
//		if (isEnable && amount > 0) {
//			// 获得钻石或金币
//			if (mListener != null) {
//				mListener.onSucessGetPoint(amount,TAG);
//				spendTapPoints(amount);
//				isOpened = false;
//			}
//		}
//	}
//
//	/**
//	 * Notifier for when spending virtual currency succeeds. 成功使用了：赚到的钻石点数
//	 */
//	@Override
//	public void getSpendPointsResponse(String currencyName, int pointTotal) {
//		Log.d(TAG, "getSpendPointsResponse:" + currencyName + "--"
//				+ pointTotal);
//	}
//
//	@Override
//	public void getSpendPointsResponseFailed(String error) {
//		// DebugLog.d(TAG,"getSpendPointsResponseFailed:"+error);
//	}
//
//	@Override
//	public View createPublishAdBanner(Activity ctx) {
//		return null;
//	}
//
//	@Override
//	public void onCreate(Activity context) {
//		// TODO Auto-generated method stub
//	}
//
//	@Override
//	public void onResume() {
//		if (isEnable && isOpened) {
//			// 当程序从Tapjoy回到我们程序时 调用此方法 此方法会调用earnedTapPoints方法
//			TapjoyConnect.getTapjoyConnectInstance().getTapPoints(this);
//		}
//	}
//
//	@Override
//	public void onDestroy(Activity context) {
//		if (isEnable) {
//			TapjoyConnect.getTapjoyConnectInstance().sendShutDownEvent();
//		}
//	}
//
//	@Override
//	public void showPublishOffersAd(OnSucessGetPointListener listener) {
//		if (isEnable) {
//			mListener = listener;
//			isOpened = true;
//			TapjoyConnect.getTapjoyConnectInstance().showOffers();
//		}
//	}
//
//}
