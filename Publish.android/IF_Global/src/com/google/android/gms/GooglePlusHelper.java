package com.google.android.gms;

import org.cocos2dx.ext.Native;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.IntentSender;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.google.android.gms.auth.GoogleAuthException;
import com.google.android.gms.auth.GoogleAuthUtil;
import com.google.android.gms.auth.GooglePlayServicesAvailabilityException;
import com.google.android.gms.auth.UserRecoverableAuthException;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.SignInButton;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.plus.Plus;
import com.google.android.gms.plus.model.people.Person;
import com.google.android.gms.plus.model.people.Person.Name;
import com.nbg.gp.R;

public class GooglePlusHelper implements GoogleApiClient.ConnectionCallbacks,
	GoogleApiClient.OnConnectionFailedListener, OnResponseListener_GooglePlus {
	private static String TAG = "GooglePlus";
	private static GooglePlusHelper instance;

	private GoogleApiClient mGoogleApiClient;
	private SignInButton mSignInButton;

	private ConnectionResult mConnectionResult;
	private Activity mParentActivity;
	private String mToken;
	private boolean isRequestLoginFlag;
	private boolean isRequestLogoutFlag;
	private int fetchTokenCount;
	private GooglePlusHelper(Activity act) {
		mParentActivity = act;
		mGoogleApiClient = new GoogleApiClient.Builder(mParentActivity)
        .addApi(Plus.API, Plus.PlusOptions.builder()
                .addActivityTypes(MomentUtil.ACTIONS).build())
        .addScope(Plus.SCOPE_PLUS_LOGIN)
        .addConnectionCallbacks(this)
        .addOnConnectionFailedListener(this)
        .build();

		boolean flag=mGoogleApiClient.isConnectionFailedListenerRegistered(this);

		mSignInButton = new SignInButton(mParentActivity);
		((IGooglePlusActivityer) mParentActivity)
				.setOnResponseListener_GooglePlus(this);
	}

	public static void newInstance(Activity ctx) {
		if (ctx == null) {
			throw new RuntimeException("please set parent Activity..");
		}
		instance = new GooglePlusHelper(ctx);
	}

	public static GooglePlusHelper getInstance() {
		if (instance == null) {
			throw new RuntimeException(
					"please call newInstance(Activity ctx) first");
		}
		return instance;
	}

	public void onStart() {
		mToken = null;
		mGoogleApiClient.connect();
		Log.i(TAG, "onStart()");

	}

	public void onStop() {
		mToken = null;
		mGoogleApiClient.disconnect();
		Log.i(TAG, "onStop()");

	}

	public boolean isGooglePlayServicesAvailable() {
		int available = GooglePlayServicesUtil
				.isGooglePlayServicesAvailable(mParentActivity);
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
		this.mParentActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Toast.makeText(mParentActivity, R.string.plus_generic_error,
						Toast.LENGTH_LONG).show();

			}
		});
		Native.postNotification("onResponsed3rdPlatform", obj.toString());

	}

	public void login() {
		Log.i(TAG, "call login()");
		if (!isGooglePlayServicesAvailable()) {
			handleUnSupportedGooglePlayServices();
			return;
		}
		if (isConnected()) {
			this.logout();
			return;
		}
		isRequestLoginFlag = true;
		mSignInButton.performClick();
		showProgressBar();
		try {
			if (mConnectionResult != null) {
				mConnectionResult.startResolutionForResult(mParentActivity,
						IGooglePlusActivityer.REQUEST_CODE_SIGN_IN_GOOGLE_PLUS);
			} else {
				mGoogleApiClient.connect();
			}
		} catch (IntentSender.SendIntentException e) {
			// Fetch a new result to start.
			mGoogleApiClient.connect();
		}
		
	}

	public void logout() {
		if (!isGooglePlayServicesAvailable()) {
			handleUnSupportedGooglePlayServices();
			return;
		}
		Log.i(TAG, "call logout()");
		isRequestLogoutFlag = true;
		mToken = null;
        if (mGoogleApiClient.isConnected()) {
            Plus.AccountApi.clearDefaultAccount(mGoogleApiClient);
            mGoogleApiClient.disconnect();
//            mGoogleApiClient.reconnect();
        }
	}

	public boolean isConnected() {
		return mGoogleApiClient != null && mGoogleApiClient.isConnected();
	}

	public String getLoginInfo_GooglePlus() {
		JSONObject obj = getJsonObjLoiginfo();
		return obj.toString();
	}

	private JSONObject getJsonObjLoiginfo() {
		JSONObject obj = new JSONObject();
		try {

			if (isConnected()) {
				String accountName = Games.getCurrentAccountName(mGoogleApiClient);// chenjianming368@gmail.com
				if (accountName != null) {
					obj.put("userName", accountName);
				}
				Person person = Plus.PeopleApi.getCurrentPerson(mGoogleApiClient);
				if (person != null) {
					String displayName = person.getDisplayName();// Jianming
																	// Chen
					// 115325605126759956912
					String userId = person.getId();
					// {"familyName":"Chen","givenName":"Jianming"}
					Name name = person.getName();
					obj.put("platform", "google+");
					obj.put("displayName", displayName);
					obj.put("userId", userId);
					Log.i(TAG, "userName:" + accountName + " userId:" + userId
							+ " Name:" + name.toString());
				} else {
					Log.i(TAG, "userName:" + accountName);
				}
				if (mToken != null)
					obj.put("token", mToken);
				else {
					obj.put("token", "");
				}
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

	@Override
	public void onConnectionFailed(ConnectionResult result) {
		Log.i(TAG, "onConnectionFailed");

		mConnectionResult = result;
		if (isRequestLoginFlag) {
			isRequestLoginFlag = false;// 每次请求只响应一次回复
			JSONObject obj = getJsonObjLoiginfo();
			try {
				obj.put("msgId", "login_failed_google");
			} catch (JSONException e) {
				e.printStackTrace();
			}
			Native.postNotification("onResponsed3rdPlatform", obj.toString());
			closeProgressBar();
		} else if (this.isRequestLogoutFlag) {
			isRequestLogoutFlag = false;
			JSONObject obj = getJsonObjLoiginfo();
			try {
				obj.put("msgId", "logout_sucess_google");
			} catch (JSONException e) {
				e.printStackTrace();
			}
			Native.postNotification("onResponsed3rdPlatform", obj.toString());
		}

	}

	@Override
	public void onConnected(Bundle connectionHint) {
		String currentPersonName = Plus.PeopleApi.getCurrentPerson(mGoogleApiClient) != null ? Plus.PeopleApi.getCurrentPerson(mGoogleApiClient).getDisplayName() : "unknown_person";
		Log.i(TAG, "onConnected:" + currentPersonName);

		if (this.mToken == null) {
			this.fetchTokenInBackground();
		}
		if (isRequestLoginFlag) {
			isRequestLoginFlag = false;// 每次请求只响应一次回复
			// 通知已经连接上
			JSONObject obj = getJsonObjLoiginfo();
			try {
				obj.put("msgId", "login_sucess_google");
			} catch (JSONException e) {
				e.printStackTrace();
			}
			Native.postNotification("onResponsed3rdPlatform", obj.toString());
			closeProgressBar();

		}
	}

	@Override
	public void onConnectionSuspended(int cause) {
		Log.i(TAG, "onDisConnected");
		try{
			mGoogleApiClient.connect();
		}catch(Exception e){
			e.printStackTrace();
		}
		if (isRequestLoginFlag || isRequestLogoutFlag) {
			isRequestLogoutFlag = false;
			isRequestLoginFlag = false;// 每次请求只响应一次回复
			JSONObject obj = getJsonObjLoiginfo();
			try {
				obj.put("msgId", "logout_sucess_google");
			} catch (JSONException e) {
				e.printStackTrace();
			}
			Native.postNotification("onResponsed3rdPlatform", obj.toString());
			closeProgressBar();

		}
	}

	@Override
	public boolean handleActivityResult(int requestCode, int resultCode,
			Intent data) {
		Log.i(TAG, "handleActivityResult requestCode:" + requestCode
				+ " resultCode:" + resultCode);

		if (requestCode == IGooglePlusActivityer.REQUEST_CODE_SIGN_IN_GOOGLE_PLUS) {
			if (resultCode == Activity.RESULT_OK && !mGoogleApiClient.isConnected()
					&& !mGoogleApiClient.isConnecting()) {
				// This time, connect should succeed.
				mGoogleApiClient.connect();
			}else if(resultCode==Activity.RESULT_CANCELED){//被用户取消了
				//通知取消
				JSONObject obj = getJsonObjLoiginfo();
				try {
					obj.put("msgId", "login_canceled_google");
				} catch (JSONException e) {
					e.printStackTrace();
				}
				Native.postNotification("onResponsed3rdPlatform", obj.toString());
				closeProgressBar();

			}
		} else if (requestCode == IGooglePlusActivityer.REQUEST_CODE_RECOVER_FROM_AUTH_ERROR) {
			handleAuthorizeResult(resultCode, data);
		}
		return false;
	}

	private void handleAuthorizeResult(int resultCode, Intent data) {
		if (data == null) {
			show("Unknown error, click the button again");
			return;
		}
		if (resultCode == Activity.RESULT_OK) {
			this.fetchTokenInBackground();
			return;
		}
		if (resultCode == Activity.RESULT_CANCELED) {
			show("User rejected authorization.");
			return;
		}
		show("Unknown error, click the button again");
	}

	public void show(final String message) {
		this.mParentActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(mParentActivity, message, Toast.LENGTH_LONG);
			}
		});
	}

	private void fetchTokenInBackground() {
		if(fetchTokenCount>2){
			return;
		}
		fetchTokenCount++;
		new Thread(new Runnable() {

			@Override
			public void run() {
				// 取Token
				mToken = fetchToken();
				if (mToken != null) {
					Native.postNotification("getToken_sucess_google",
							getLoginInfo_GooglePlus());

				}
			}
		}).start();
	}

	protected String fetchToken() {
		try {
			return GoogleAuthUtil.getToken(this.mParentActivity,
					Games.getCurrentAccountName(mGoogleApiClient),
					"oauth2:https://www.googleapis.com/auth/userinfo.profile");
		} catch (GooglePlayServicesAvailabilityException playEx) {
			playEx.printStackTrace();
			// GooglePlayServices.apk is either old, disabled, or not present.
			showErrorDialog(playEx.getConnectionStatusCode());
		} catch (UserRecoverableAuthException userRecoverableException) {
			userRecoverableException.printStackTrace();
			// Unable to authenticate, but the user can fix this.
			// Forward the user to the appropriate activity.
			this.mParentActivity.startActivityForResult(
					userRecoverableException.getIntent(),
					IGooglePlusActivityer.REQUEST_CODE_RECOVER_FROM_AUTH_ERROR);
		} catch (GoogleAuthException fatalException) {
			fatalException.printStackTrace();
			show("Unrecoverable error " + fatalException.getMessage());
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * This method is a hook for background threads and async tasks that need to
	 * launch a dialog. It does this by launching a runnable under the UI
	 * thread.
	 */
	public void showErrorDialog(final int code) {
		this.mParentActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Dialog d = GooglePlayServicesUtil
						.getErrorDialog(
								code,
								mParentActivity,
								IGooglePlusActivityer.REQUEST_CODE_RECOVER_FROM_PLAY_SERVICES_ERROR);
				d.show();
			}
		});
	}
	
	ProgressDialog mPrgDlg;

	private void showProgressBar() {
		this.mParentActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mPrgDlg = null;
				mPrgDlg=ProgressDialog.show(mParentActivity, "Google+", "Please Wait...");
				mPrgDlg.setCanceledOnTouchOutside(false);
				mPrgDlg.setIcon(R.drawable.common_signin_btn_icon_focus_dark);
			}
		});
	}

	private void closeProgressBar() {

		this.mParentActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (mPrgDlg != null) {
					mPrgDlg.cancel();
				}
				mPrgDlg = null;

			}
		});

	}
}
