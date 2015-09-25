package org.hcg.stac.empire.ad;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.util.Log;
import android.view.View;

import org.hcg.util.GameContext;
import com.sponsorpay.sdk.android.SponsorPay;
import com.sponsorpay.sdk.android.publisher.SponsorPayPublisher;
import com.sponsorpay.sdk.android.publisher.currency.CurrencyServerAbstractResponse;
import com.sponsorpay.sdk.android.publisher.currency.CurrencyServerDeltaOfCoinsResponse;
import com.sponsorpay.sdk.android.publisher.currency.SPCurrencyServerListener;
import org.hcg.stac.empire.publish.IPublishAD;
import org.hcg.stac.empire.publish.IPublishAD.OnSucessGetPointListener;

public class AdSponsorPayPublisher implements IPublishAD {
	//private static String mUserId = "sponsor_android";
	private static String TAG = "SponsorPay";
	private static final Boolean mShouldStayOpen = false;
	
	private OnSucessGetPointListener mSuccessGetPointListerner;

	public AdSponsorPayPublisher() {
		mSuccessGetPointListerner = null;
//		mUserId = CommonUtil.getUid();
	}

	public View createPublishAdBanner1(Activity ctx) {
		// TODO Auto-generated method stub
		return null;
	}

	public OnSucessGetPointListener getSuccessGetPointListerner() {
		return mSuccessGetPointListerner;
	}

	@Override
	public void onCreate(Activity context) {
		AdSponsorPay.onStartAction();
	}

	@Override
	public void onResume() {

		try {
			SPCurrencyServerListener requestListener = new SPCurrencyServerListener() {

				@Override
				public void onSPCurrencyServerError(
						CurrencyServerAbstractResponse response) {
					// Log.d(TAG,
					// "Request or Response Error: " + response.getErrorType());
				}

				@Override
				public void onSPCurrencyDeltaReceived(
						CurrencyServerDeltaOfCoinsResponse response) {
					// Log.d(TAG,
					// "Response From Currency Server. Delta of Coins: " +
					// String.valueOf(response.getDeltaOfCoins()) +
					// ", Latest Transaction Id: " +
					// response.getLatestTransactionId());
					// Log.d(TAG,"listener = "+ mSuccessGetPointListerner);
					SharedPreferences perference = GameContext
							.getActivityInstance().getSharedPreferences(
									"SponsorPay", Context.MODE_PRIVATE);
					if (response.getDeltaOfCoins() > 0.0f) {
						Editor editor = perference.edit();
						editor.putInt("currencyValue",
								(int) response.getDeltaOfCoins());
						editor.commit();
					}
					if (mSuccessGetPointListerner != null) {
						int value = perference.getInt("currencyValue", 0);
						if (value > 0) {
							mSuccessGetPointListerner.onSucessGetPoint(value,TAG);
							Editor editor1 = perference.edit();
							editor1.putInt("currencyValue", 0);
							editor1.commit();
						}
					}
				}
			};
			SponsorPayPublisher.requestNewCoins(
					GameContext.getActivityInstance(), requestListener);
		} catch (RuntimeException ex) {

			Log.e(TAG, "SponsorPay SDK Exception: ", ex);
		}
	}

	@Override
	public void onDestroy(Activity context) {
		// TODO Auto-generated method stub
	}

	@Override
	public void showPublishOffersAd(OnSucessGetPointListener listener) {
		mSuccessGetPointListerner = listener;
		try {
			GameContext.getActivityInstance().startActivityForResult(
					SponsorPayPublisher.getIntentForOfferWallActivity(
							GameContext.getActivityInstance(), mShouldStayOpen,
							"coins", null),
					SponsorPayPublisher.DEFAULT_OFFERWALL_REQUEST_CODE);
		} catch (RuntimeException ex) {
			Log.e(TAG, "SponsorPay SDK Exception: ", ex);
		}

	}

	@Override
	public View createPublishAdBanner(Activity ctx) {
		// TODO Auto-generated method stub
		return null;
	}
}
