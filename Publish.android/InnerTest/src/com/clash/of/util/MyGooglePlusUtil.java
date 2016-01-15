package com.clash.of.util;

import org.nbg.IF.IF;
import org.nbg.util.GameContext;

import android.app.Activity;
import android.content.Intent;
import android.content.IntentSender;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.google.android.gms.MomentUtil;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.common.api.Status;
import com.google.android.gms.games.Games;
import com.google.android.gms.plus.Plus;
import com.google.android.gms.plus.model.people.Person;

public class MyGooglePlusUtil {
	public static native void nativeSetIsLogin(boolean isLogin,String userID);
	public static native void nativeSetGPlusUid(String gUid);
	
	// The core Google+ client.
	public static GoogleApiClient mGoogleApiClient;
	// We can store the connection result from a failed connect()
	// attempt in order to make the application feel a bit more
	// responsive for the user.
	public static ConnectionResult mConnectionResult;
	private static final int REQUEST_CODE_SIGN_IN = 1;
	private static ConnectionListener listener = new ConnectionListener();
	public static void activeApp()
	{
		//初始化GooglePlus Client
		mGoogleApiClient = new GoogleApiClient.Builder(GameContext.getActivityInstance().getApplicationContext())
		.addApi(Plus.API, Plus.PlusOptions.builder()
                .addActivityTypes(MomentUtil.ACTIONS).build())
        .addScope(Plus.SCOPE_PLUS_LOGIN)
		.addConnectionCallbacks(listener)
        .addOnConnectionFailedListener(listener)
//		.setActions("http://schemas.google.com/AddActivity", "http://schemas.google.com/BuyActivity")
//		.setScopes(Scopes.PLUS_LOGIN)  // Space separated list of scopes
		.build();
	}
	
	public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == REQUEST_CODE_SIGN_IN) {
            if (resultCode == Activity.RESULT_OK && !mGoogleApiClient.isConnected()
                    && !mGoogleApiClient.isConnecting()) {
                // This time, connect should succeed.
                mGoogleApiClient.connect();
            }
        }
	}
	
	public static void gpConnect()
	{
		mGoogleApiClient.connect();
	}
	
	public static void gpDisconnect()
	{
		mGoogleApiClient.disconnect();
	}
	
	static class ConnectionListener implements GoogleApiClient.ConnectionCallbacks, 
	GoogleApiClient.OnConnectionFailedListener {

		@Override
		public void onConnectionFailed(ConnectionResult result) {
	        mConnectionResult = result;
		}

		@Override
		public void onConnected(Bundle bundle) {
			Person user = Plus.PeopleApi.getCurrentPerson(mGoogleApiClient);
			String accountName = Games.getCurrentAccountName(mGoogleApiClient);
			if (user != null) {
				accountName = user.getDisplayName();
			}
	        Toast.makeText(GameContext.getActivityInstance(), accountName + " is connected.", Toast.LENGTH_LONG).show();
	        GameContext.getActivityInstance().runOnUiThread(new Runnable() {
				public void run() {
					try {
				        nativeSetIsLogin(true,Games.getCurrentAccountName(mGoogleApiClient));
//				        nativeSetGPlusUid(mGoogleApiClient.getAccountName());
					} catch (Exception e) {
						e.printStackTrace();
						// FIXME: pay fail
						// tracker.reportException(e);
					}
				}
			});
		}

		@Override
		public void onConnectionSuspended(int cause) {
//			if(mGoogleApiClient != null)
//			{
//				mGoogleApiClient.connect();
//			}
		}

	}
	
    public static void GooglePlusLogin() {
        int available = GooglePlayServicesUtil.isGooglePlayServicesAvailable(IF.getInstance());
		if (available != ConnectionResult.SUCCESS) {
			GameContext.getActivityInstance().runOnUiThread(new Runnable() {
				public void run() {
					try {					
						int result = GooglePlayServicesUtil.isGooglePlayServicesAvailable(IF.getInstance());
						GooglePlayServicesUtil.getErrorDialog(result, IF.getInstance(), 0).show();
						nativeSetIsLogin(false,"");
					} catch (Exception e) {
						e.printStackTrace();
						// FIXME: pay fail
						// tracker.reportException(e);
					}
				}
			});
			return;
		}
        try {
        	mConnectionResult.startResolutionForResult(GameContext.getActivityInstance(), REQUEST_CODE_SIGN_IN);
        } catch (IntentSender.SendIntentException e) {
            // Fetch a new result to start.
        	mGoogleApiClient.connect();
        } catch (NullPointerException e) {

        }
        catch(Exception e)
    	{
    		e.printStackTrace();
    	}
	}
	
	public static void GooglePlusLogout(){
        if (mGoogleApiClient.isConnected()) {
            Plus.AccountApi.revokeAccessAndDisconnect(mGoogleApiClient).setResultCallback(
                    new ResultCallback<Status>() {
                        @Override
                        public void onResult(Status status) {
                            if (status.isSuccess()) {
                            } else {
                            }
//                            mGoogleApiClient.reconnect();
                        }
                    }
            );
        }
	}

	public static boolean GooglePlusIsLogin(){
		if (mGoogleApiClient.isConnected()) {
			return true;
		} else {
			return false;
		}
	}
	
	public static void GooglePlusGetUserId(){
		nativeSetGPlusUid(Games.getCurrentAccountName(mGoogleApiClient));
	}
	
	public static void openLeaderBoards(){
		Log.d("COK", "COK openLeaderBoards");
		IF.getInstance().openLeaderBoards();
	}
	
	public static void openAchievements(){
		Log.d("COK", "COK openAchievements");
		IF.getInstance().openAchievements();
	}
	
	public static void submitScore(int score){
		Log.d("COK", "COK submitScore "+score);
		IF.getInstance().submitScore(score);
	}
	
	public static void unlockAchievements(String id){
		Log.d("COK", "COK unlockAchievements "+id);
		IF.getInstance().unlockAchievements(id);
	}
}
