package org.nbg.stac.empire.sns.simple;

import android.app.Activity;

import org.nbg.IF.sns.FacebookEventObserver;
import org.nbg.IF.sns.ISimpleSocial;
import org.nbg.IF.sns.SNSCallbackListener;

import com.nostra13.socialsharing.common.AuthListener;
import com.nostra13.socialsharing.facebook.FacebookFacade;

public class FacebookImpl implements ISimpleSocial {
	public static final String FACEBOOK_APP_ID = "1670702669839400";
	private static final String FACEBOOK_APP_SECRET = "5e5dc4c34e5d0cd97442122478ba72eb";
	private FacebookFacade facebook;
	private FacebookEventObserver facebookEventObserver;
	private SNSCallbackListener listener;

	public FacebookImpl(Activity mainActivity) {
		facebook = new FacebookFacade(mainActivity, FACEBOOK_APP_ID);
		facebookEventObserver = FacebookEventObserver.newInstance();
	}

	private void publishImage(String caption, byte[] bitmapdata) {
		facebook.publishImage(bitmapdata, caption);
	}

	@Override
	public void registerListener(Activity mainActivity) {
		facebookEventObserver.registerListeners(mainActivity);
		if (!facebook.isAuthorized()) {
			facebook.authorize();
		}
	}

	@Override
	public void unRegisterListener(Activity mainActivity) {
		facebookEventObserver.unregisterListeners();
	}

	@Override
	public void postMessage(final String message, final String link,
			final String linkName, final String linkDescription) {
		if (facebook.isAuthorized()) {
			facebook.publishMessage(message, link, linkName, linkDescription,
					null, null);
			facebook.logout();
		} else {
			// Start authentication dialog and publish message after successful
			// authentication
			facebook.authorize(new AuthListener() {
				@Override
				public void onAuthSucceed() {
					facebook.publishMessage(message, link, linkName,
							linkDescription, null, null);
					facebook.logout();
				}

				@Override
				public void onAuthFail(String error) { // Do noting
				}
			});
		}
	}

	@Override
	public void postImage(final String caption, final byte[] imgae) {
		if (facebook.isAuthorized()) {
			publishImage(caption, imgae);
		} else {
			// Start authentication dialog and publish image after successful
			// authentication
			facebook.authorize(new AuthListener() {
				@Override
				public void onAuthSucceed() {
					publishImage(caption, imgae);
				}

				@Override
				public void onAuthFail(String error) { // Do noting
				}
			});
		}
	}

	@Override
	public void setCallbackListener(SNSCallbackListener listener) {
		this.listener = listener;
	}
}
