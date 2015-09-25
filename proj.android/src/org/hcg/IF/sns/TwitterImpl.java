package org.hcg.IF.sns;

import android.app.Activity;

import com.nostra13.socialsharing.common.AuthListener;
import com.nostra13.socialsharing.facebook.FacebookFacade;
import com.nostra13.socialsharing.twitter.TwitterFacade;
import org.hcg.IF.sns.SNSCallbackListener;

public class TwitterImpl implements ISimpleSocial {
	public static final String TWITTER_CONSUMER_KEY = "44veknrIKvs6xpDEwpDkQ";
	public static final String TWITTER_CONSUMER_SECRET = "aRbIx8n1PpLCsk1cI8Uy3I9oXKSpshkavnCirxyTeU";
	public static final String TWITTER_ACESS_TOKEN = "359835025-nGubesXzWZziFT2Ddg635ayF9DOcEbaKvk0S6F8x";
	public static final String TWITTER_ACESS_TOKEN_SECRET = "7EzdoZjRXBs8K3tbb47MZ8lGDEdAsoPISS5JtwYlx8";
	private TwitterFacade twitter;
	private TwitterEventObserver twitterEventObserver;
	private SNSCallbackListener listener;
	public TwitterImpl(Activity mainActivity) {
		twitter = new TwitterFacade(mainActivity, TWITTER_CONSUMER_KEY,
				TWITTER_CONSUMER_SECRET);
		twitterEventObserver = TwitterEventObserver.newInstance();
	}

	@Override
	public void registerListener(Activity mainActivity) {
		twitterEventObserver.registerListeners(mainActivity);
		if (!twitter.isAuthorized()) {
			twitter.authorize();
		}
	}

	@Override
	public void unRegisterListener(Activity mainActivity) {
		twitterEventObserver.unregisterListeners();
	}

	@Override
	public void postMessage(final String message, String link, String linkName,
			String linkDescription) {
		if (twitter.isAuthorized()) {
			publishMessage(message);
		} else {
			// Start authentication dialog and publish message after successful
			// authentication
			twitter.authorize(new AuthListener() {
				@Override
				public void onAuthSucceed() {
					publishMessage(message); 
				}

				@Override
				public void onAuthFail(String error) { // Do nothing
				}
			});
		}

	}

	@Override
	public void postImage(String title, byte[] imgae) {
		throw new java.lang.RuntimeException("unsupport postImage by Twitter");
	}

	@Override
	public void setCallbackListener(SNSCallbackListener listener) {
		this.listener=listener;
	}
	
	private void publishMessage(String message){
		twitter.publishMessage(message);
		twitter.logout();
		if(listener!=null){
			listener.onComplete();
		}
	}
}
