package org.nbg.IF.sns;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;

import org.nbg.IF.IF;
import org.nbg.stac.empire.sns.simple.FacebookImpl;
import org.nbg.util.CommonUtil;
import org.nbg.util.MarketUtil;

/**
 * 社区分享辅助类
 * 
 * @author anychen
 * 
 */
public class SNSHelper {
	public static final String TAG = "share-sns";
	public static final String SNS_FACEBOOK = "facebook";
	public static final String SNS_TWITTER = "twitter";
	public static final String SNS_RENREN = "renren";
	public static final String SNS_TENCENT = "tencent";
	public static final String SNS_SINA = "sina";
	public static final String SNS_GOOGLE_PLAY = "market";
	public static final String SNS_EMAIL = "email";
	public static final String SNS_OTHER = "UI_Other";
	/**
	 * 分享消息
	 */
	public static final int SHARE_NEWS_EVENT = 2;

	// SocialAuth Component
	// private SocialAuthAdapter mGlobalAdapter;
	private Context mContext;
	private SNSCallbackListener mCallbackListener;
	private String mPostFeedMsg;
	FacebookImpl mFacebookImpl;
	TwitterImpl mTwitterImpl;

	private static final String SHARE_PIC_URI = "invite_share.jpg";

	public SNSHelper(Context ctx) {
		this.mContext = ctx;
		// mGlobalAdapter = new SocialAuthAdapter(new ResponseListener());
		// mGlobalAdapter.addProvider(Provider.FACEBOOK, R.drawable.facebook);
		// mGlobalAdapter.addProvider(Provider.TWITTER, R.drawable.twitter);
	}

	public void postFeed(String msg, String pltSns) {

	}

	public void postFeed_sina(String msg) {
		final String msg1 = msg;
//		IF.getInstance().runOnUiThread(new Runnable() {
//			public void run() {
//				// 使用友盟分享实现
//				UMSnsService.shareToSina(mContext, CommonUtil.getAssetBitmap(SHARE_PIC_URI), msg1,
//						new UmSocialCallbackListener());
//			}
//		});
	}

	public void postFeed_tenc(String msg) {
		final String msg1 = msg;
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				// 使用友盟分享实现
//				UMSnsService.shareToTenc(mContext, CommonUtil.getAssetBitmap(SHARE_PIC_URI), msg1,
//						new UmSocialCallbackListener());
			}
		});
	}

	public void postFeed_renr(String msg) {
		// 使用友盟分享实现
		final String msg1 = msg;
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
//				UMSnsService.shareToRenr(mContext, CommonUtil.getAssetBitmap(SHARE_PIC_URI), msg1,
//						new UmSocialCallbackListener());
			}
		});
	}

	public void postFeed_playStore() {
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				MarketUtil.openAppsInMarket(IF.getInstance(), -1);
			}
		});
	}

	public void postFeed_facebook(String msg) {
		final String msg1 = msg;
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				mPostFeedMsg = msg1;
				if (mFacebookImpl == null) {
					mFacebookImpl = new FacebookImpl((Activity) mContext);
					mFacebookImpl.registerListener((Activity) mContext);
				}
				mFacebookImpl.postImage(mPostFeedMsg, CommonUtil.getAssetBitmap(SHARE_PIC_URI));
			}
		});
	}

	public void postFeed_twitter(String msg) {
		mPostFeedMsg = msg;
		if (mTwitterImpl == null) {
			mTwitterImpl = new TwitterImpl((Activity) mContext);
			mTwitterImpl.registerListener((Activity) mContext);
		}
		mTwitterImpl.postMessage(msg, null, null, null);
	}

	public void postFeed_email(String msg, String subject) {
		Intent mailIntent = new Intent(android.content.Intent.ACTION_SENDTO);
		mailIntent.setType("image/*");
		mailIntent.setData(Uri.parse("mailto:"));
		mailIntent.putExtra(android.content.Intent.EXTRA_SUBJECT, subject);
		mailIntent.putExtra(android.content.Intent.EXTRA_TEXT, msg);
		mailIntent.putExtra(Intent.EXTRA_STREAM,
				CommonUtil.getAssetsBitmapForUri(SHARE_PIC_URI));
		mContext.startActivity(mailIntent);
	}

	public void postFeed_other(String msg, String imgUri, String sendTile) {
		Intent picMessageIntent = new Intent(android.content.Intent.ACTION_SEND);
		picMessageIntent.setType("image/*");
		picMessageIntent.putExtra(Intent.EXTRA_STREAM,
				CommonUtil.getAssetsBitmapForUri(imgUri));
		picMessageIntent.putExtra(android.content.Intent.EXTRA_TEXT, msg);
		mContext.startActivity(Intent.createChooser(picMessageIntent, sendTile));
	}

	public SNSCallbackListener getCallbackListener() {
		return mCallbackListener;
	}

	public void setCallbackListener(SNSCallbackListener mCallbackListener) {
		this.mCallbackListener = mCallbackListener;
	}

	public void release() {
		if (mFacebookImpl != null) {
			mFacebookImpl.unRegisterListener((Activity) mContext);
		}
		if (mTwitterImpl != null) {
			mTwitterImpl.unRegisterListener((Activity) mContext);
		}
	}

//	private final class UmSocialCallbackListener implements
//	DataSendCallbackListener {
//		@Override
//		public void onDataSendFinished(RETURN_STATUS return_status,
//				SHARE_TO paramSHARE_TO) {
//			Log.d(TAG, "onDataSendFinished:" + return_status.toString());
//			// * UPDATED, 发送成功
//			// * REPEATED, 重复发送（新浪有重发判断）
//			// * FILE_TO_LARGE, 图片大小超过限制，现在的上限是2M（只检查byte维度的大小）
//			// * NETWORK_UNAVAILABLE, 网络问题，发送失败
//			// * SEND_TIME_EXTENDS_LIMIT, 发送次数超过单位时间的限制
//			if (return_status == RETURN_STATUS.UPDATED) {
//				if (mCallbackListener != null) {
//					mCallbackListener.onComplete();
//				}
//			} else {
//				if (mCallbackListener != null) {
//					mCallbackListener.onError("");
//				}
//			}
//
//		}
//
//		@Override
//		public void onDataSendFailedWithException(UMSNSException ex,
//				SHARE_TO pSHARE_TO) {
//			Log.d(TAG, "onDataSendFailedWithException:" + ex.toString());
//
//		}
//	}

}
