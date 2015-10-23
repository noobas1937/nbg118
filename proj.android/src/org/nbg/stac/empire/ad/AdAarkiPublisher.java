package org.nbg.stac.empire.ad;
//package com.stac.empire.ad;
//
//import android.app.Activity;
//import android.view.View;
//
//import com.aarki.AarkiContact;
//import com.aarki.AarkiOfferActivity;
//import com.aarki.AarkiUserBalance;
//import com.aarki.AarkiUserBalance.Status;
//import com.stac.empire.publish.IPublishAD;
//import com.stac.empire.publish.IPublishAD.OnSucessGetPointListener;
//
//public class AdAarkiPublisher implements IPublishAD {
//	Activity mContext;
//	private static final String PLACEMENT_ID = "F27D233463F41ED4AA";
//	private static final String CLIENT_SECURITY_KEY = "EDGpp7Z6rLSKq0xBYhAoZFKAAf7Q";
//	private static final String APP_ID = "A0B893893C80CDBFOU";
//	private static final String LOG_TAG = "Aarki";
//	private OnSucessGetPointListener mSuccessGetPointListerner;
//	@Override
//	public void showPublishOffersAd(final OnSucessGetPointListener listener) {
//		mSuccessGetPointListerner=listener;
//		AarkiOfferActivity.launch(mContext, PLACEMENT_ID);
//	}
//
//	@Override
//	public View createPublishAdBanner(Activity ctx) {
//		// TODO Auto-generated method stub
//		return null;
//	}
//
//	@Override
//	public void onCreate(Activity context) {
//		mContext = context;
//		AarkiContact.registerApp(context, CLIENT_SECURITY_KEY, APP_ID);
//	}
//
//	@Override
//	public void onResume() {
//		AarkiUserBalance.check(PLACEMENT_ID, new AarkiUserBalance.Listener() {
//
//			@Override
//			public void onFinished(final Status status, final Integer balance) {
//				if (status == AarkiUserBalance.Status.OK) {
//					// Update stats
//					if (balance > 0&&mSuccessGetPointListerner!=null) {
//						
//						 // Try subtracting item cost from the user's balance
//				        AarkiUserBalance.add(PLACEMENT_ID, -balance, new AarkiUserBalance.Listener() {
//				            
//				            @Override
//				            public void onFinished(final Status status, final Integer balance) {
//				                if (status == AarkiUserBalance.Status.OK) {
//				                	mSuccessGetPointListerner.onSucessGetPoint(balance);
//				                }    
//				            }
//				        });
// 
//					}
//				}
//			}
//
//		});
//	}
//
//	@Override
//	public void onDestroy(Activity context) {
//		// TODO Auto-generated method stub
//
//	}
//
//}
