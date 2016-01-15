package com.google.android.gms;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.util.Log;
import android.widget.Toast;
import org.cocos2dx.ext.Native;

import org.nbg.IF.R;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.SignInButton;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.Player;
import com.google.example.games.basegameutils.GameHelper;
import com.google.example.games.basegameutils.GameHelper.GameHelperListener;
import org.nbg.IF.IF;
import org.nbg.util.CommonUtil;

public class GoogleGameServiceHelper extends GameHelper implements
		GameHelperListener {
	private static String TAG = "GameHelper";

	private boolean DEBUG_BUILD = true;
	private SignInButton mSignInButton;
	boolean mSetupDone = false;
	boolean mIsFirstLogin;
	private boolean tryOpenAchievements = false;
	private boolean tryOpenLeaderBoards = false;
	public GoogleGameServiceHelper(Activity activity) {
		super(activity,1);
		init();
	}

	private void init() {

		// enable debug logs (if applicable)
		if (DEBUG_BUILD) {
			enableDebugLog(true);
		}
		mIsFirstLogin=true;
		mSignInButton = new SignInButton(this.getContext());
		if (!this.mSetupDone) {
			setup(this);
			mSetupDone = true;
		}
	}
	
	private void clearOpenPanelCache(){
		tryOpenAchievements = false;
		tryOpenLeaderBoards = false;
	}

	private void handleSignInSucceeded() {
		// 通知已经连接上
		JSONObject obj = getJsonObjLoiginfo();
		try {
			obj.put("msgId", "login_sucess_google");
		} catch (JSONException e) {
			e.printStackTrace();
		}
		Native.postNotification("onResponsed3rdPlatform", obj.toString());
		//打开相关面板
		if(tryOpenAchievements){
			openAchievements();
		}else if(tryOpenLeaderBoards){
			openLeaderBoards();
		}
	}

	private void handleSignInFailed() {

		JSONObject obj = getJsonObjLoiginfo();
		try {
			obj.put("msgId", "login_failed_google");
		} catch (JSONException e) {
			e.printStackTrace();
		}
		Native.postNotification("onResponsed3rdPlatform", obj.toString());
		clearOpenPanelCache();
	}

	public boolean isGooglePlayServicesAvailable() {
		int available = GooglePlayServicesUtil
				.isGooglePlayServicesAvailable(this.getContext());
		return available == ConnectionResult.SUCCESS;
	}

	private void handleUnSupportedGooglePlayServices() {
		JSONObject obj = getJsonObjLoiginfo();
		try {
			obj.put("msgId", "login_failed_google");
			obj.put("reason", "unsupported");

		} catch (JSONException e) {
			e.printStackTrace();
		}
		((Activity)this.getContext()).runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Toast.makeText(getContext(), R.string.plus_generic_error,
						Toast.LENGTH_LONG).show();

			}
		});
		Native.postNotification("onResponsed3rdPlatform", obj.toString());
		clearOpenPanelCache();

	}
	public String getLoginInfo_GoogleGameService() {
		JSONObject obj = getJsonObjLoiginfo();
		return obj.toString();
	}
	private static String NULL_ACCOUNT="";
	private JSONObject getJsonObjLoiginfo() {
		JSONObject obj = new JSONObject();
		try {

			if (this.isSignedIn()) {
				
				// Set the greeting appropriately on main menu
				Player p = Games.Players.getCurrentPlayer(getApiClient());

				String displayName;
				if (p == null) {
					Log.w(TAG, "mGamesClient.getCurrentPlayer() is NULL!");
					displayName = "???";
				} else {
					displayName = p.getDisplayName();
				}
				
				// 基于权限问题，去除获取账号参数处理
				// 此处需要：android.permission.GET_ACCOUNTS权限
//				if (!CommonUtil.isVersion_AOE2()) {
					try {
						String accountName = Games.getCurrentAccountName(getApiClient());
						// String accountName =
						// Games.getCurrentAccountName(getApiClient());//chenjianming368@gmail.com
						if (accountName != null) {
							obj.put("userName", accountName);
						} else {
							obj.put("userName", NULL_ACCOUNT);
						}
					} catch (Exception e) {
					}
//				} else {
//					obj.put("userName", NULL_ACCOUNT);
//				}


				if (p != null) {
					String userId = p.getPlayerId();
					obj.put("platform", "google+");
					obj.put("displayName", displayName);
					obj.put("userId", userId);
					Log.i(TAG, "SignedIn: DisplayName:" + displayName + " userName:" + userId);
				} else {
					Log.i(TAG, "SignedIn: Error:");
				}

				obj.put("token", "");

				obj.put("is_connect", true);
			} else {
				obj.put("platform", "google+");
				obj.put("is_connect", false);
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return obj;
	}

	/***
	 * 登录方法
	 */
	public void signIn() {
		Log.i(TAG, "call login()");
		if (!isGooglePlayServicesAvailable()) {
			handleUnSupportedGooglePlayServices();
			return;
		}

		if (this.isSignedIn()) {
			this.signOut();
			if(mIsFirstLogin){
				this.mSignInButton.performClick();
				this.beginUserInitiatedSignIn();
				mIsFirstLogin=false;
			}
			return;
		}
		mIsFirstLogin=false;
		this.mSignInButton.performClick();
		this.beginUserInitiatedSignIn();
	}

	@Override
	public void signOut() {
		super.signOut();

		// 无法监听登出是否成功，这里全部默认登出成功
		JSONObject obj = getJsonObjLoiginfo();
		try {
			obj.put("msgId", "logout_sucess_google");
		} catch (JSONException e) {
			e.printStackTrace();
		}

		Native.postNotification("onResponsed3rdPlatform", obj.toString());
		clearOpenPanelCache();
	}

	public void openAchievements() {
		clearOpenPanelCache();
		if (this.getApiClient().isConnected()) {
			IF.getInstance().startActivityForResult(Games.Achievements.getAchievementsIntent(getApiClient()),3502);
		}else{
			//尝试登录
			tryOpenAchievements = true;
			signIn();
		}
	}
	
	public void openLeaderBoards() {
		clearOpenPanelCache();
		if (this.getApiClient().isConnected()) {
			IF.getInstance().startActivityForResult(Games.Leaderboards.getAllLeaderboardsIntent(getApiClient()),3602);
		}else{
			//尝试登录
			tryOpenLeaderBoards = true;
			signIn();
		}
	}
	
	public void submitScore(int score) {
		if (this.getApiClient().isConnected()) {
			Log.d("COK", "COK submitScore success");
			Games.Leaderboards.submitScore(getApiClient(), "CgkIoYzEkosJEAIQCQ", score);
		}else{
			Log.d("COK", "COK submitScore fail");
		}
	}

	public void unlockAchievements(String id) {
		if (this.getApiClient().isConnected()) {
			// unlock the "Trivial Victory" achievement.
			Log.d("COK", "COK unlockAchievements success "+id);
			Games.Achievements.unlock(getApiClient(), id);
//			Games.Achievements.unlock(getGamesClient(), getAchievemnetId(id));
		}else{
			Log.d("COK", "COK unlockAchievements fail");
		}
	}

	@Override
	public void onSignInFailed() {
		Log.i(TAG, "onSignInFailed");
		handleSignInFailed();
	}

	@Override
	public void onSignInSucceeded() {
		Log.i(TAG, "onSignInSucceeded");
		handleSignInSucceeded();

	}
}
