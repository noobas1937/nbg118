package org.nbg.IF.sns;

public interface SNSCallbackListener {
	public void onComplete();

	public void onError(String errMsg);

	public void onCancel();
}
