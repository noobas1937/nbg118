package org.hcg.IF.sns;

import android.app.Activity;

public interface ISimpleSocial {
	public void registerListener(Activity mainActivity);

	public void unRegisterListener(Activity mainActivity);

	public void postMessage(String message, String link, String linkName,
			String linkDescription);

	public void postImage(String title, byte[] imgae);

	public void setCallbackListener(SNSCallbackListener listener);
}
