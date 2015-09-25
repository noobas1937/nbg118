package com.elex.chatservice;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentTransaction;
import android.view.Window;
import android.view.WindowManager;

import com.elex.chatservice.util.AttachmentUtil;
import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSRes;

public final class ChatActivity extends FragmentActivity {
	public static final String TAG = "HelpShiftDebug";
	private HSApiData data;
	private HSStorage storage;
	private Bundle bundle;
	private int fragmentHolderId;
	private FragmentTransaction ft;
	
	public void onCreate(Bundle savedInstanceState) {
		System.out.println("ChatActivity onCreate");
		super.onCreate(savedInstanceState);
		this.data = new HSApiData(this);
		this.storage = this.data.storage;

		Bundle extras = getIntent().getExtras();
//		String activeConversation = this.storage.getActiveConversation();

		requestWindowFeature(Window.FEATURE_NO_TITLE);

		// 等效于getWindow().addFlags(1024);
		// 会导致输入法把整个屏幕挤上去，helpshift的会话界面也不是全屏的（FAQ是全屏）
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

//		System.out.println("ChatActivity 1");
		setContentView(HSRes.getId(this, "layout", "cs__chat_activity"));

		this.bundle = new Bundle(extras);
//		String chatLaunchSource = extras.getString("chatLaunchSource");

//		System.out.println("ChatActivity 2");
		this.fragmentHolderId = HSRes.getId(this, "id", "cs__chat_activity_fragment_holder");
		this.ft = getSupportFragmentManager().beginTransaction();
		
//		AndroidBug5497Workaround.assistActivity(this);
		
//		System.out.println("ChatActivity 3");
		showMessagesFragment();
	}

	private void showMessagesFragment() {
		System.out.println("ChatActivity showMessagesFragment");
		this.ft.add(this.fragmentHolderId, Fragment.instantiate(this, ChatInputFragment.class.getName(), this.bundle));

		this.ft.commit();
	}

	protected void onResume() {
//		Bundle extras = getIntent().getExtras();
//		String activeConversation = this.storage.getActiveConversation();
//		String archivedConversation = this.storage.getArchivedConversation();

		// HSActivityUtil.restoreFullscreen(this);
		// if ((extras.getBoolean("newConversation")) ||
		// ((activeConversation.equals("")) &&
		// (archivedConversation.equals(""))))
		// {
		// HSActivityUtil.restoreFullscreen(this);
		// }
		// else{
		// HSActivityUtil.forceNotFullscreen(this);
		// }

//		this.storage.setIsConversationShowing(Boolean.valueOf(true));
		
		super.onResume();
	}

	protected void onPause() {
//		Bundle extras = getIntent().getExtras();
//		String activeConversation = this.storage.getActiveConversation();
//		String archivedConversation = this.storage.getArchivedConversation();
//
//		if ((extras.getBoolean("newConversation"))
//				|| ((activeConversation.equals("")) && (archivedConversation
//						.equals("")))) {
//			if (!AttachmentUtil.isSelectingAttachment()) {
//				Intent returnIntent = new Intent();
//				setResult(-1, returnIntent);
//				finish();
//			}
//		}
//		// HSActivityUtil.restoreFullscreen(this);
//		if (extras != null) {
//			Boolean isRoot = Boolean.valueOf(extras.getBoolean("isRoot"));
//			if ((isRoot.booleanValue() == true) && (isFinishing() == true)) {
//				HSActivityUtil.sessionEnding();
//			}
//		}
		super.onPause();
	}

	public void onBackPressed() {
		Intent returnIntent = new Intent();
		setResult(RESULT_OK, returnIntent);
		super.onBackPressed();
//		finish();
		System.out.println("ChatActivity onBackPressed()");
	}

	public void onDestroy() {
//		this.storage.setIsConversationShowing(Boolean.valueOf(false));
		super.onDestroy();

		System.out.println("ChatActivity onDestroy()");
	}
}