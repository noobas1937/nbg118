package org.nbg.stac.empire.sns;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.math.BigDecimal;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Currency;
import java.util.List;

import org.cocos2dx.ext.Device;
import org.cocos2dx.ext.Native;
import org.json.JSONArray;
import org.json.JSONObject;
import org.nbg.IF.IF;
import org.nbg.util.GameContext;

import sfs2x.client.util.Base64.InputStream;

import com.facebook.AccessToken;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.FacebookSdk;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.HttpMethod;
import com.facebook.share.widget.LikeView;
import com.facebook.GraphRequest.GraphJSONObjectCallback;
import com.facebook.appevents.AppEventsConstants;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.internal.WebDialog;
import com.facebook.internal.WebDialog.OnCompleteListener;
import com.facebook.login.*;
import com.facebook.login.widget.LoginButton;
import com.facebook.share.widget.AppInviteDialog;
import com.facebook.share.model.AppInviteContent;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.widget.GameRequestDialog;
import com.facebook.share.model.GameRequestContent;
import com.facebook.share.widget.ShareDialog;
import com.facebook.share.model.ShareLinkContent;
import com.nanigans.android.sdk.NanigansEventManager;

import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.LinearLayout;
import android.widget.Toast;


public class FBUtil {
	private static String TAG="COK_FBUtil";
	private static int REQUEST_ERROR=2;
	private static int REQUEST_CANCEl=3;
	private static int REQUEST_SUCESS=1;
    
    private static boolean _inFeedDialogue = false;
    
    public static String feedName ="";
    public static String feedCaption ="";
    public static String feedLinkDesc ="";
    public static String feedLink ="";
    public static String feedPicURL ="";
    public static int feedType = 0;
    public static String feedRef ="";
    public static int preRequest = 0;
    
    public static String sharePicURL ="";
    public static String sharePicDialog ="";
    public static Bitmap newmap=null;
    
	public native static void nativeSendFriendsList(String list);//ArrayList<FBFriendPoj> list
	public static native void nativeSetIsLogin(boolean isLogin,String userID,String userName);
	public static native void nativeSetRequestResult(int result,String requestID);
	public static native void nativeSetIsFBFan(boolean isFBFan);
    public static native void nativeSetFeedDialogResult(int result);
    public native static void nativeSendInviteFriendsList(String list);//ArrayList<FBFriendPoj> list
    public static native void nativeSetFBUID(String userID);
    public static native void nativeSendRequestFriendsList(String list);
    public static native void nativeSendHeadImgUrl(String headImgUrl);
	
	public static boolean Login() {
		if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "Login" + ": fail due to facebookEnabled false");
			return false;
		}
        boolean flag = true;
        try {
        	Collection<String> permissions = Arrays.asList("public_profile", "user_friends", "publish_actions");
            LoginManager.getInstance().logInWithPermissions(IF.getInstance(), permissions);
        }catch (Exception e){
            Log.d(TAG, "Login exception"+e.getMessage());
            flag = false;
        }
        return flag;
	}
	
	private static LikeView likeView;
	public static void showLike(){
		pressLikeButton();
	}
	
	public static void createLikeButton(Context context){
		if(likeView == null){
			likeView = new LikeView(context);
			likeView.setObjectIdAndType("https://www.facebook.com/Clash.Of.Kings.Game",null);
			likeView.setVisibility(View.GONE);
			likeView.setForegroundColor(0xFF602800);
		}
	}
	public static boolean pressLikeButton(){
		if(likeView != null){
	    	likeView.toggleLike();
	    	return true;
		}
		return false;
	}
	public static void hideLike(){
		if(likeView != null){
			IF.getInstance().runOnUiThread(new Runnable() {
				public void run() {
					try {
//						likeView.setX(0);
//						likeView.setY(0);
						likeView.setVisibility(View.GONE);
					} catch (Exception e) {
//						reportException(Main.getInstance(), e);
					}
				}
			});
		}
	}
	public static LikeView getLikeView(Context context){
//		int visibility = View.GONE;
//		if(likeView != null){
//			visibility = likeView.getVisibility();
//			ViewGroup parent = (ViewGroup)likeView.getParent();
//			parent.removeView(likeView);
//		}
//		if(likeView == null){
//			likeView = new LikeView(context);
//			likeView.setObjectIdAndType("https://www.facebook.com/Clash.Of.Kings.Game",null);
//			likeView.setVisibility(visibility);
//			likeView.setForegroundColor(0xFF602800);
//			//不需要额外监听点击事件
//			likeView.likeButton.setOnClickListener(new OnClickListener() {
//	            @Override
//	            public void onClick(View v) {
//	            	likeView.toggleLike();
//	            	Log.d(TAG, "fblike after click");
//	            }
//	        });
//		}
		return likeView;
	}
	
	private void sendRequestDialog() {
		if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "sendRequestDialog" + ": fail due to facebookEnabled false");
			return;
		}
	    Bundle params = new Bundle();
	    params.putString("message", "Learn how to make your Android apps social");
	    WebDialog requestsDialog = (
	        new WebDialog.Builder(IF.getInstance(),
	            "apprequest",
	            params))
	            .setOnCompleteListener(new OnCompleteListener() {

	                @Override
	                public void onComplete(Bundle values,
	                    FacebookException error) {
	                    if (error != null) {
	                        if (error instanceof FacebookOperationCanceledException) {
	                            Toast.makeText(IF.getInstance().getApplicationContext(), 
	                                "Request cancelled", 
	                                Toast.LENGTH_SHORT).show();
	                        } else {
	                            Toast.makeText(IF.getInstance().getApplicationContext(), 
	                                "Network Error", 
	                                Toast.LENGTH_SHORT).show();
	                        }
	                    } else {
	                        final String requestId = values.getString("request");
	                        if (requestId != null) {
	                            Toast.makeText(IF.getInstance().getApplicationContext(), 
	                                "Request sent",  
	                                Toast.LENGTH_SHORT).show();
	                        } else {
	                            Toast.makeText(IF.getInstance().getApplicationContext(), 
	                                "Request cancelled", 
	                                Toast.LENGTH_SHORT).show();
	                        }
	                    }   
	                }

	            })
	            .build();
	    requestsDialog.show();
	}

	/**
	 * Logout From Facebook 
	 */
	public static void callFacebookLogout() {
		if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "callFacebookLogout" + ": fail due to facebookEnabled false");
			return;
		}
		LoginManager.getInstance().logOut();
	}
	
	public static void getFriendsList() {
		Log.d(TAG, " static getFriendsList");
		if (isLogin()) {
			makeFriendsRequest();
		}
	}
	
	public static void inviteFriends() {
		Log.d(TAG, " call inviteFriends");
		if (isLogin()) {
			Log.d(TAG, "makeInviteFriendsRequest");
			makeInviteFriendsRequest();
		}
	}
	
	public static void getAppRequestList() {
		Log.d(TAG, " static getAppRequestList");
		if (isLogin()) {
			getAppRequestFriends();
		}
	}
	
	public static void requestForMeInfo() {
		Log.d(TAG, " call requestForMeInfo");
		if (isLogin()) {
			Log.d(TAG, "makeRequestMe");
			makeRequestMe();
		}
	}
	
	public static void appInvite(final String inviteLink) {
		Log.d(TAG, " call appInvite");
		if (isLogin()) {
			IF.getInstance().runOnUiThread(new Runnable() {
				@Override
				public void run() {
					Log.d(TAG, "run appInvite");
					// TODO Auto-generated method stub
					if (AppInviteDialog.canShow()) {
					    AppInviteContent content = new AppInviteContent.Builder()
					                .setApplinkUrl(inviteLink)
					                .build();
					    AppInviteDialog.show(IF.getInstance(), content);
					}
				}
			});
		}else{
			Log.d(TAG, "facebook not login");
		}
	}
	
	public static void showFansWall(){
		Intent it = new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.facebook.com/Clash.of.Kings.fans"));  
        IF.getInstance().startActivity(it); 
	}
	
	private static void makeFriendsRequest() {
		// Make an API call to get user data and define a
		// new callback to handle the response.
		Log.d(TAG,"fb makeFriendsRequest");
		IF.getInstance().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				GraphRequest request = GraphRequest.newMyInstallFriendsRequest(AccessToken.getCurrentAccessToken(),
						new GraphRequest.GraphJSONArrayCallback() {

							@Override
							public void onCompleted(JSONArray result, GraphResponse response) {
//								ArrayList<FBFriendPoj> fbAllFriendPojs=new ArrayList<FBFriendPoj>();
//								//List<FBFriendPoj> fbSelectedFriendPojs = new ArrayList<FBFriendPoj>();
//								if (users != null && users.size() > 0) {
//									for (int i = 0; i < users.size(); i++) {
//										FBFriendPoj poj = new FBFriendPoj();
//										poj.setUserid(users.get(i).getId());
//										poj.setUsername(users.get(i).getName());
//										fbAllFriendPojs.add(poj);
//									}
//								}
//								Log.d(TAG,"fb friends token "+session.getAccessToken());
//								Log.d(TAG,"fb friends count "+fbAllFriendPojs.size());
//								Log.d(TAG,"fb friends data "+users.toString());
//								nativeSendFriendsList(fbAllFriendPojs);
								try {
									Log.d(TAG,"fb makeFriendsRequest response "+response.toString());
									if(response!=null && response.getError()!=null){
										Log.d(TAG,"fb makeFriendsRequest error ");
									}else if(result!=null){
										JSONArray data = result;
										Log.d(TAG,"fb friends count "+data.length());
										if (data.length() > 0) {
											nativeSendFriendsList(data.toString());
										}
									}
								} catch (Exception e) {
									e.printStackTrace();
								}

//								sendFriendRequest(fbAllFriendPojs,"test invite");
							}
						});
				request.executeAsync();
			}
		});
		
	}
	
	private static void makeInviteFriendsRequest() {
		// Make an API call to get user data and define a
		// new callback to handle the response.
		IF.getInstance().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				Log.d(TAG,"fb makeInviteFriendsRequest run");
				GraphRequest request = GraphRequest.newMyFriendsRequest(AccessToken.getCurrentAccessToken(),
						new GraphRequest.GraphJSONArrayCallback() {//Callback

							@Override
							public void onCompleted(JSONArray result, GraphResponse response) {
								ArrayList<FBFriendPoj> fbAllFriendPojs=new ArrayList<FBFriendPoj>();
								Log.d(TAG,"fb makeInviteFriendsRequest fbAllFriendPojs");
//								if (users != null && users.size() > 0) {
//									for (int i = 0; i < users.size(); i++) {
//										FBFriendPoj poj = new FBFriendPoj();
//										poj.setUserid(users.get(i).getId());
//										poj.setUsername(users.get(i).getName());
//										fbAllFriendPojs.add(poj);
//									}
//								}

								try {
									Log.d(TAG,"fb Invite friends response "+response.toString());
									if(response!=null && response.getError()!=null){
										//todo
										Log.d(TAG,"fb Invite error ");
									}else if(result!=null){
										JSONArray data = result;
										Log.d(TAG,"fb Invite friends count "+data.length());
										if (data.length() > 0) {
											nativeSendInviteFriendsList(data.toString());
										}
									}
								} catch (Exception e) {
									e.printStackTrace();
								}

								Log.d(TAG,"fb Invite friends token "+AccessToken.getCurrentAccessToken());
								//Log.d(TAG,"fb Invite friends data "+users.toString());
								
//								sendFriendRequest(fbAllFriendPojs,"test invite");
							}
						});
				request.executeAsync();
			}
		});
	}
	
	private static void getAppRequestFriends() {
		// Make an API call to get user data and define a
		// new callback to handle the response.
		IF.getInstance().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				Log.d(TAG,"fb getAppRequestFriends run");
				GraphRequest request = GraphRequest.newAppRequestFriendsRequest(AccessToken.getCurrentAccessToken(),
						new GraphRequest.GraphJSONArrayCallback() {//Callback
							@Override
							public void onCompleted(JSONArray result, GraphResponse response) {
								try {
									Log.d(TAG,"fb getAppRequestFriends response "+response.toString());
									if(response!=null && response.getError()!=null){
										//todo
										Log.d(TAG,"fb AppRequestFriends error ");
									}else if(result!=null){
										JSONArray data = result;
										Log.d(TAG,"fb AppRequestFriends friends count "+data.length());
										if (data.length() > 0) {
											nativeSendRequestFriendsList(data.toString());
										}
									}
								} catch (Exception e) {
									e.printStackTrace();
								}
							}
						});
				request.executeAsync();
			}
		});
	}
	
	public static void sendFriendRequest(final ArrayList<FBFriendPoj> friends,final String msg,final String ftype) {
		if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "isLogin" + ": fail due to facebookEnabled false");
			return;
		}
		IF.getInstance().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				if(!isLogin()){
					Log.d(TAG,"facebook not login");
					return;
				}
				Log.d(TAG,"fb sendFriendRequest ftype="+ftype);
				//params.putString("frictionless", "1"); //Turn on frictionless
				List<String> ids = new ArrayList<String>();
				for (int i = 0; i < friends.size(); i++) {
					ids.add(friends.get(i).getUserid());
				}
				GameRequestDialog gameRequestDialog = new GameRequestDialog(IF.getInstance());
		        gameRequestDialog.registerCallback(
		        		IF.getInstance().getCallbackManager(),
		                new FacebookCallback<GameRequestDialog.Result>() {
		                    @Override
		                    public void onCancel() {
		                        Log.d(TAG, "Canceled");
		                    }

		                    @Override
		                    public void onError(FacebookException error) {
		                        Log.d(TAG, String.format("Error: %s", error.toString()));
		                    }

		                    @Override
		                    public void onSuccess(GameRequestDialog.Result result) {
		                        Log.d(TAG, "Success!");
		                        Log.d(TAG, "fb "+result.getRequestId());;
								final String reqId = result.getRequestId();
								Log.d(TAG,"fb friends invite request id= "+reqId);
								if (reqId != null && !reqId.equals("")) {
									nativeSetRequestResult(REQUEST_SUCESS,reqId);
								}
		                    }
		                });		        
				String useIds = TextUtils.join(",", ids);
				if(ftype!=null && ftype.equals("1")){
					GameRequestContent content1 = new GameRequestContent.Builder()
	                .setMessage(msg)
	                .setObjectId("739067812873207")
					.setActionType(GameRequestContent.ActionType.SEND)
					.setTo(useIds)
	                .build();
					if (GameRequestDialog.canShow()) {
						gameRequestDialog.show(IF.getInstance(), content1);
					}
					Log.d(TAG,"fb params send object_id=739067812873207");
				}else if(ftype!=null && ftype.equals("2")){
					GameRequestContent content2 = new GameRequestContent.Builder()
	                .setMessage(msg)
	                .setObjectId("426169294209352")
					.setActionType(GameRequestContent.ActionType.ASKFOR)
					.setTo(useIds)
	                .build();
					if (GameRequestDialog.canShow()) {
						gameRequestDialog.show(IF.getInstance(), content2);
					}
					Log.d(TAG,"fb params askfor object_id=426169294209352");
				}else if(friends.size()<=0){
					GameRequestContent content3 = new GameRequestContent.Builder()
	                .setMessage(msg)
	                .build();
					if (GameRequestDialog.canShow()) {
						gameRequestDialog.show(IF.getInstance(), content3);
					}
					Log.d(TAG,"fb filters =app_non_users");
				}else{
					GameRequestContent content4 = new GameRequestContent.Builder()
	                .setMessage(msg)
	                .setTo(useIds)
	                .build();
					if (GameRequestDialog.canShow()) {
						gameRequestDialog.show(IF.getInstance(), content4);
					}
					Log.d(TAG,"fb else send");
				}

				Log.d(TAG,"fb send ids ="+ids);
				//showDialogWithoutNotificationBar("apprequests", params);
			}
		});		
	}
	
	private static void showDialogWithoutNotificationBar(String action, Bundle params) {
		if(!isLogin()){
			return;
		}
		WebDialog requestsDialog = (new WebDialog.Builder(IF.getInstance(),
				"apprequest", params)).setOnCompleteListener(
				new OnCompleteListener() {

					@Override
					public void onComplete(Bundle values,
							FacebookException error) {
						if (error != null) {
							Log.d(TAG,"fb friends invite error "+error.toString());

							if (error instanceof FacebookOperationCanceledException) {
								//showToast(R.string.fb_invite_send_cancel);
								nativeSetRequestResult(REQUEST_CANCEl,"");
							} else {
								//showToast(R.string.fb_invite_send_error);
								nativeSetRequestResult(REQUEST_ERROR,"");
							}
						} else {
							Log.d(TAG,"fb friends invite result "+values.toString());
							final String reqId = values.getString("request");
							Log.d(TAG,"fb friends invite request id= "+reqId);
							if (reqId != null && !reqId.equals("")) {
								//showToast(R.string.fb_invite_send_sucess);
								nativeSetRequestResult(REQUEST_SUCESS,reqId);
								//updateView();
							} else {
								//showToast(R.string.fb_invite_send_cancel);
								nativeSetRequestResult(REQUEST_CANCEl,reqId);
							}
						}
					}
				}).build();
		requestsDialog.show();
	}
	public static boolean isLogin(){
		if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "isLogin" + ": fail due to facebookEnabled false");
			return false;
		}
		if(AccessToken.getCurrentAccessToken() == null){
			Log.d(TAG, "fb not login");
			return false;
		}else if(AccessToken.getCurrentAccessToken().isExpired()){
			Log.d(TAG, "fb token expired");
			return false;
		}
		return true;
	}

	public static boolean facebookHasGranted(final String msg){
		if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "facebookHasGranted" + ": fail due to facebookEnabled false");
			return false;
		}
		boolean flag = false;
		if(isLogin() && AccessToken.getCurrentAccessToken().getPermissions().contains(msg)){
			flag = true;
		}
		return flag;
	}
	
	public static void facebookPostAction(final String action,final String actionType,final String actionObject){
		Log.d(TAG, "fb facebookPostAction");
    	if(!isLogin()){
    		Log.d(TAG,"fb facebookPostAction not login ");
			return;
		}
        final Bundle params = new Bundle();
	    params.putString(actionType, actionObject);
	    params.putString("fb:explicitly_shared", "true");
	    String keyType = Integer.toString(feedType);
	    IF.getInstance().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				try
				{
					String actionLink = "me/clash-of-kings:";
					actionLink = actionLink + action;
					Log.d(TAG,"fb postAction  actionLink="+actionLink);
					Log.d(TAG,"fb postAction  actionType="+actionType);
					Log.d(TAG,"fb postAction  actionObject="+actionObject);
					final GraphRequest request = new GraphRequest(AccessToken.getCurrentAccessToken(), actionLink, params,
							HttpMethod.POST, new GraphRequest.Callback() {
								public void onCompleted(GraphResponse response) {
									try {
										Log.d(TAG,"fb postAction response"+response.toString());
									} catch (Exception e) {
										e.printStackTrace();
									}
									Log.d(TAG,"fb postAction  end");
								}
							});
					request.executeAsync();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	public static void facebookPostMethod(final String method,final String objectType,final String objectValue,final String actions,final String actionsValue,final String ref){
		Log.d(TAG, "fb facebookPfacebookPostMethodostAction");
    	if(!isLogin()){
    		Log.d(TAG,"fb facebookPostMethod not login ");
			return;
		}
        final Bundle params = new Bundle();
	    params.putString(objectType, objectValue);
	    if(actions!=null && !actions.equals("")){
	    	params.putString(actions, actionsValue);
	    }
	    params.putString("fb:explicitly_shared", "true");
	    params.putString("caption", "Clash of Kings");
	    params.putString("ref", ref);
	    IF.getInstance().runOnUiThread(new Runnable() {			
			@Override
			public void run() {
				try
				{
			    	String methodLink = "me/";
			    	methodLink = methodLink + method;
					Log.d(TAG,"fb facebookPostMethod  method="+methodLink);
					Log.d(TAG,"fb facebookPostMethod  objectType="+objectType);
					Log.d(TAG,"fb facebookPostMethod  objectValue="+objectValue);
					final GraphRequest request = new GraphRequest(AccessToken.getCurrentAccessToken(), methodLink, params,
							HttpMethod.POST, new GraphRequest.Callback() {
								public void onCompleted(GraphResponse response) {
									try {
										Log.d(TAG,"fb facebookPostMethod response"+response.toString());
									} catch (Exception e) {
										e.printStackTrace();
									}
									Log.d(TAG,"fb facebookPostMethod  end");
								}
							});
					request.executeAsync();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	public static void checkIsGameFans_FB() {
//		Session session = Session.getActiveSession();
//
//		if (session != null && session.isOpened()) {
//			String fqlQuery = "SELECT profile_section,type,page_id FROM page_fan WHERE uid = me() and page_id=1429242977292633";
//			Bundle params = new Bundle();
//			params.putString("q", fqlQuery);
//
//			final GraphRequest request = new GraphRequest(session, "/fql", params,
//					HttpMethod.GET, new Request.Callback() {
//						public void onCompleted(Response response) {
//							try {
//								JSONArray data = (JSONArray) response
//										.getGraphObject().getInnerJSONObject()
//										.get("data");
//								if (data.length() > 0) {
//									nativeSetIsFBFan(true);
//								}
//							} catch (Exception e) {
//								e.printStackTrace();
//							}
//							Log.i(TAG, "Result: " + response.toString());
//						}
//					});
//			//Request.executeBatchAsync(request);
//			IF.getInstance().runOnGLThread(new Runnable() {
//				@Override
//				public void run() {
//					Request.executeBatchAndWait(request);
//				}
//			});
//		} else {
//		}
	}
	
	
	public static void showToast(final String msg){
		IF.getInstance().runOnGLThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				Toast.makeText(IF.getInstance(), msg, Toast.LENGTH_LONG).show();
			}
		});
	}
	
	public static void showToast(final int resId){
		final String msg=IF.getInstance().getString(resId);
		IF.getInstance().runOnGLThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				Toast.makeText(IF.getInstance(), msg, Toast.LENGTH_LONG).show();
			}
		});
	}
    
	public  static void rePublishFeedDialog(){
		Log.d(TAG, "fb rePublishFeedDialog execute feed");
		publishFeedDialog(feedName,feedCaption,feedLink,feedLinkDesc,feedPicURL,feedType,feedRef);
	}
			
	
    public static void publishFeedDialog(String name,String caption, String link,
                                         String linkDescription,String pictureUrl,final int fType,String ref){
        feedName = name;
        feedCaption = caption;
        feedLinkDesc = linkDescription;
        feedLink = link;
        feedPicURL = pictureUrl;
        feedType = fType;
        feedRef = ref;
        Log.d(TAG,"fb publishFeedDialog start");
    	if(_inFeedDialogue){
			return;
		}
		if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "publishFeedDialog" + ": fail due to facebookEnabled false");
			return;
		}
    	if(!isLogin()){
    		Log.d(TAG,"fb publishFeedDialog not login ");
    		preRequest = 5;
    		Login();
			return;
		}
    	if(!AccessToken.getCurrentAccessToken().getPermissions().contains("publish_actions")){
    		Log.d(TAG,"fb publishFeedDialog have no permission publish_actions");
    	}
		Log.d(TAG,"fb publishFeedDialog ");
		_inFeedDialogue = true;
        final Bundle params = new Bundle();
	    params.putString("name", name);
	    params.putString("caption", caption);
	    params.putString("description", linkDescription);
	    params.putString("link", link);
	    params.putString("ref", ref);
	    params.putString("picture", pictureUrl);
	    String keyType = Integer.toString(feedType);
	    Log.d(TAG,"fb push feed feedType="+keyType);
	    IF.getInstance().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				try
				{
				    if((feedType==1 && AccessToken.getCurrentAccessToken().getPermissions().contains("publish_actions")) || fType==2){
				    	Log.d(TAG,"fb push feed feedType=");
						final GraphRequest request = new GraphRequest(AccessToken.getCurrentAccessToken(), "me/feed", params,
								HttpMethod.POST, new GraphRequest.Callback() {
									public void onCompleted(GraphResponse response) {
										_inFeedDialogue = false;
										try {
											Log.d(TAG,"fb push feed response"+response.toString());
											if(response!=null && response.getError()==null){
												if(fType==1){
													nativeSetFeedDialogResult(REQUEST_SUCESS);
												}
											}
										} catch (Exception e) {
											e.printStackTrace();
										}
										Log.d(TAG,"fb push feed end");
									}
								});
						request.executeAsync();
				    }else{
				    	Log.d(TAG,"fb execute ShareDialog");
				    	_inFeedDialogue = false;
				    	ShareDialog shareDialog = new ShareDialog(IF.getInstance());
				    	shareDialog.registerCallback(
				        		IF.getInstance().getCallbackManager(),
				                new FacebookCallback<ShareDialog.Result>() {
				                    @Override
				                    public void onCancel() {
				                        Log.d(TAG, "fb Canceled");
				                    }

				                    @Override
				                    public void onError(FacebookException error) {
				                        Log.d(TAG, String.format("fb Error: %s", error.toString()));
				                    }

				                    @Override
				                    public void onSuccess(ShareDialog.Result result) {
				                        Log.d(TAG, "fb Success!"+result);
				                        //nativeSetRequestResult(REQUEST_SUCESS,reqId);
				                    }
				                });	
				    	ShareLinkContent content = new ShareLinkContent.Builder()
		                .setContentDescription(feedLinkDesc)
				    	.setContentTitle(feedCaption)
				    	.setImageUrl(Uri.parse(feedPicURL))
				    	.setContentUrl(Uri.parse(feedLink))
				    	.setRef(feedRef)
		                .build();
						if (shareDialog.canShow(content)) {
							Log.d(TAG,"fb execute ShareDialog show");
							shareDialog.show(IF.getInstance(), content);
							if(feedType==1){
								nativeSetFeedDialogResult(REQUEST_SUCESS);
							}
						}
						// Invoke the dialog
//				    	WebDialog feedDialog = (
//	                                            new WebDialog.Builder(GameContext.getActivityInstance(),
//	                                                                            "feed",
//	                                                                            params))
//	                    .setOnCompleteListener(new OnCompleteListener() {
//	                        
//	                        @Override
//	                        public void onComplete(Bundle values,
//	                                               FacebookException error) {
//	                            _inFeedDialogue = false;
//	                            if (error == null) {
//	                                final String postId = values.getString("post_id");
//	                                if (postId != null) {
//	                                    nativeSetFeedDialogResult(REQUEST_SUCESS);
//	                                } else {
//	                                    // User clicked the Cancel button
//	                                    //nativeSetFeedDialogResult(REQUEST_CANCEl);
//	                                }
//	                            } else if (error instanceof FacebookOperationCanceledException) {
//	                                // User clicked the "x" button
//	                                //nativeSetFeedDialogResult(REQUEST_CANCEl);
//	                            } else {
//	                                //nativeSetFeedDialogResult(REQUEST_ERROR);
//	                            }
//	                        }
//	                        
//	                    })
//	                    .build();
//				    	feedDialog.show();	
				    }
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	    preRequest = 0;
    }
     
     private static Bitmap downloadPhoto(String picUrl) {
    	 Log.d(TAG,"downloadPhoto");
 		try {
 			URL url = new URL(picUrl);
 			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
 			conn.setRequestMethod("GET");
 			conn.setConnectTimeout(5 * 1000);
 			conn.connect();
 			java.io.InputStream in = conn.getInputStream();
 			ByteArrayOutputStream bos = new ByteArrayOutputStream();
 			byte[] buffer = new byte[1024];
 			int len = 0;
 			while ((len = in.read(buffer)) != -1) {
 				bos.write(buffer, 0, len);
 			}
 			byte[] dataImage = bos.toByteArray();
 			bos.close();
 			in.close();
 			Bitmap bitmap = BitmapFactory.decodeByteArray(dataImage, 0,
 					dataImage.length);
 			return bitmap;
 		} catch (Exception e) {
 			// TODO Auto-generated catch block
 			e.printStackTrace();
 		}
 		return null;
 	}

    public static void SharePhotos(String url,String dialog){
        sharePicURL = url;
        sharePicDialog = dialog;
        Log.d(TAG,"fb SharePhotos start");
        if(!IF.getInstance().facebookEnabled){
            Log.d(TAG, "SharePhotos" + ": fail due to facebookEnabled false");
            return;
        }
        newmap = downloadPhoto(sharePicURL);
        if(newmap==null){
        	Log.d(TAG, "SharePhotos err : newmap is null");
        	return;
        }
//        Log.d(TAG,"newmap.getWidth()=="+newmap.getWidth()+",newmap.getHeight()=="+newmap.getHeight());
	    IF.getInstance().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Log.d(TAG,"fb download pic url===>"+sharePicURL);
		        try
		        {
		            Log.d(TAG,"fb execute SharePhotos");
		            ShareDialog shareDialog = new ShareDialog(IF.getInstance());
		            shareDialog.registerCallback(
		                                         IF.getInstance().getCallbackManager(),
		                                         new FacebookCallback<ShareDialog.Result>() {
		                                             @Override
		                                             public void onCancel() {
		                                                 Log.d(TAG, "Canceled");
		                                             }
		                                             @Override
		                                             public void onError(FacebookException error) {
		                                                 Log.d(TAG, String.format("Error: %s", error.toString()));
		                                             }
		                                             @Override
		                                             public void onSuccess(ShareDialog.Result result) {
		                                                 Log.d(TAG, "Success!"+result);
		                                                 //nativeSetRequestResult(REQUEST_SUCESS,reqId);
		                                             }
		                                         });
		            SharePhoto photo = new SharePhoto.Builder()
		            	.setBitmap(newmap)
		            	.build();
		            SharePhotoContent content = new SharePhotoContent.Builder()
		            	.addPhoto(photo)
		            	.build();
		      
	            	if (shareDialog.canShow(content)) {
		                Log.d(TAG,"fb execute ShareDialog show");
		                shareDialog.show(IF.getInstance(), content);
		            }else{
		            	//没安装app
		            	new  AlertDialog.Builder(IF.getContext()).setMessage(sharePicDialog).show(); 
//		    	    	Toast.makeText(IF.getContext(),sharePicDialog, Toast.LENGTH_LONG).show();
		    	    	Native.postNotification("fb_app_not_install", null);
		    	    	Log.d(TAG,"fb app not install");
		    	    	return;
		    	    }
		        } catch (Exception e) {
		            e.printStackTrace();
		        }
			}
	    });
    }
    
    public static void fbEventCompletedRegistration(){
		if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "fbEventCompletedRegistration" + ": fail due to facebookEnabled false");
			return;
		}
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
		logger.logEvent(AppEventsConstants.EVENT_NAME_COMPLETED_REGISTRATION);
    }
    
    public static void fbEventCompletedTutorial(){
		if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "fbEventCompletedTutorial" + ": fail due to facebookEnabled false");
			return;
		}
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
		logger.logEvent(AppEventsConstants.EVENT_NAME_COMPLETED_TUTORIAL);
    }
    
    public static void fbEventAchievedLevel(int level){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "fbEventAchievedLevel" + ": fail due to facebookEnabled false");
			return;
		}
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString(AppEventsConstants.EVENT_PARAM_LEVEL, String.valueOf(level));
		logger.logEvent(AppEventsConstants.EVENT_NAME_ACHIEVED_LEVEL,parameters);
    }
    
    public static void fbEventPurchase(String cost,String itemId){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "fbEventPurchase" + ": fail due to facebookEnabled false");
			return;
		}
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
		logger.logPurchase(BigDecimal.valueOf(Float.parseFloat(cost)), Currency.getInstance("USD"));
		NanigansEventManager.getInstance().trackPurchase(Float.parseFloat(cost)*100, String.valueOf(cost), 1D);
    }
	
    public static void appPurchaseItem(int type,int user_level, int castle_level,int item_id,String item_name,int item_price,int item_count,int spend,String entrance){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appPurchaseItem" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appPurchaseItem");
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("type", String.valueOf(type));
    	parameters.putString("user_level", String.valueOf(user_level));
    	parameters.putString("castle_level", String.valueOf(castle_level));
    	parameters.putString("item_id", String.valueOf(item_id));
    	parameters.putString("item_name", item_name);
    	parameters.putString("item_price", String.valueOf(item_price));
    	parameters.putString("item_count", String.valueOf(item_count));
    	parameters.putString("spend", String.valueOf(spend));
    	parameters.putString("entrance", entrance);
    	logger.logEvent("Purchase_Item",spend,parameters);
    }
    
    public static void appEventShareLog(String realm,int user_level, int castle_level,String share_content){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appEventShareLog" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appEventShareLog");
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("realm", realm);
    	parameters.putString("user_level", String.valueOf(user_level));
    	parameters.putString("castle_level", String.valueOf(castle_level));
    	parameters.putString("share_content", share_content);
    	logger.logEvent("Share",parameters);
    }
    
    public static void appEventFBEntrance(String entrance){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appEventFBEntrance" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appEventFBEntrance");
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("FacebookEntrance", entrance);
    	logger.logEvent("FacebookEntrance",parameters);
    }
    
    public static void appEventSpeedUp(int user_level, int castle_level,int type,int spend){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appEventSpeedUp" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appEventSpeedUp");
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("user_level", String.valueOf(user_level));
    	parameters.putString("castle_level", String.valueOf(castle_level));
    	parameters.putString("type", String.valueOf(type));
    	parameters.putString("spend", String.valueOf(spend));
    	logger.logEvent("SpeedUp",parameters);
    }
    
    public static void appEventGiftPackage(String entracnce,String name,int id,int user_castle, int user_level){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appEventGiftPackage" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appEventGiftPackage");
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("package_entracnce", entracnce);
    	parameters.putString("package_name", name);
    	parameters.putString("package_id", String.valueOf(id));
    	parameters.putString("user_level", String.valueOf(user_level));
    	parameters.putString("user_castle", String.valueOf(user_castle));
    	logger.logEvent("appEventGiftPackage",parameters);
    }
    
    public static void appEventHireWorker(int user_castle,int user_level){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appEventHireWorker" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appEventHireWorker");
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("user_level", String.valueOf(user_level));
    	parameters.putString("user_castle", String.valueOf(user_castle));
    	logger.logEvent("HireWorker",parameters);
    }
    
    public static void appEventLevelUp(int user_level){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appEventLevelUp" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appEventLevelUp");
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("user_level_up", String.valueOf(user_level));
    	logger.logEvent("LevelUp",parameters);
    }
    
    public static void appEventSdCardUnAvable(String type){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appEventSdCardUnAvable" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appEventSdCardUnAvable");
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("type", type);
    	logger.logEvent("SdcardUnavalbe",parameters);
    }

    public static void appEventException(String exceptionType, String funcInfo, String cause, String message){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appEventException" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appEventException " + exceptionType + " " + funcInfo);
    	System.out.println("appEventException: " + exceptionType + " " + funcInfo + " " + cause + " " + message);
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("function", Device.getVersionName() + " " + Device.getVersionCode() + " " + funcInfo);
    	parameters.putString("cause", cause);
    	parameters.putString("message", message);
    	logger.logEvent("Exception_" + exceptionType,parameters);
    }
    
    public static void appEventAllianceHonorExchange(String name,int id,int user_castle, int user_level,int rank){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appEventAllianceHonorExchange" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appEventGiftPackage");
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("alliance_item_name", name);
    	parameters.putString("alliance_item_id", String.valueOf(id));
    	parameters.putString("user_level", String.valueOf(user_level));
    	parameters.putString("user_castle", String.valueOf(user_castle));
    	parameters.putString("user_alliance_level", String.valueOf(rank));
    	logger.logEvent("AllianceHonorExchange",parameters);
    }
    
    public static void appEventAllianceScoreUsage(String name,int id,int rank){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appEventAllianceScoreUsage" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appEventAllianceScoreUsage");
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("alliance_item_name", name);
    	parameters.putString("alliance_item_id", String.valueOf(id));
    	parameters.putString("user_alliance_level", String.valueOf(rank));
    	logger.logEvent("AllianceScoreUsage",parameters);
    }
    
    public static void appEventCityEffect(int cityLv,int playerLv){
    	if(!IF.getInstance().facebookEnabled){
			Log.d(TAG, "appEventCityEffect" + ": fail due to facebookEnabled false");
			return;
		}
    	Log.d(TAG, "appEventCityEffect");
    	AppEventsLogger logger = IF.getInstance().getAppEventsLogger();
    	Bundle parameters = new Bundle();
    	parameters.putString("user_castle", String.valueOf(cityLv));
    	parameters.putString("user_level", String.valueOf(playerLv));
    	logger.logEvent("cityEffect",parameters);
    }
    
	private static void makeRequestMe() {
		IF.getInstance().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				Log.d(TAG,"fb makeRequestMe run");
				GraphRequest request = GraphRequest.newMeRequest(AccessToken.getCurrentAccessToken(),
						new GraphRequest.GraphJSONObjectCallback() {//Callback
							@Override
	                        public void onCompleted(JSONObject me,  GraphResponse response) {
	                            if (response.getError() != null) {
	                      
	                            }else{
	                            	Log.d(TAG,"fb makeRequestMe id"+me.optString("id"));
	                            	nativeSetFBUID(me.optString("id"));
	                            }
	                        }
						});
				request.executeAsync();
			}
		});
	}
	/*
	private static final List<String> PERMISSIONS = Arrays
			.asList("publish_actions");
	public static int GET_FRIEND_LIST = 3;
	public static int USERINFO = 4;
	public static int LOGIN = 0;
	private static String m_name;
	private static String m_caption;
	private static String m_description;
	private static String m_picUrl;	

	private static void login(int func) {
		Session session = Session.getActiveSession();
		if (session == null) {
			session = new Session(Main.getInstance());
			Session.setActiveSession(session);
			if (session.getState().equals(SessionState.CREATED_TOKEN_LOADED)) {
				Session.OpenRequest request = new Session.OpenRequest(
						Main.getInstance());
				request.setPermissions(PERMISSIONS);
				request.setRequestCode(func);
				session.openForPublish(request);
			}
		}
		if (!session.isOpened() && !session.isClosed()) {
			Session.OpenRequest request = new Session.OpenRequest(
					Main.getInstance());
			request.setPermissions(PERMISSIONS);
			request.setRequestCode(func);
			session.openForPublish(request);
		} else {
			Session.openActiveSession(Main.getInstance(), true, null);
			if (func == LOGIN) {
				
			} else if (func == GET_FRIEND_LIST) {
				getFriendList();
			} else if (func == USERINFO) {
				getUserInfo();
			}
		}
	}

	public static void Login() {
		Session session = Session.getActiveSession();
		if (session == null || !session.isOpened()) {
			login(LOGIN);
			return;
		}

	}

	public static void getUserInfo() {
		Session session = Session.getActiveSession();
		if (session == null || !session.isOpened()) {
			login(USERINFO);
			return;
		}
		Request.executeMeRequestAsync(session, new Request.GraphUserCallback() {
			@Override
			public void onCompleted(GraphUser user, Response response) {
				if (user != null) {
					nativeSetFBUid(user.getId());
					Toast.makeText(Main.getInstance(), "UID binded",
							Toast.LENGTH_SHORT).show();
				}
			}
		});
	}

	public static void getFriendList() {
		Session session = Session.getActiveSession();
		if (session == null || !session.isOpened()) {
			login(GET_FRIEND_LIST);
			return;
		}
		Request request = Request.newMyFriendsRequest(session,
				new Request.GraphUserListCallback() {

					@Override
					public void onCompleted(List<GraphUser> users,
							Response response) {
						List<FBFriend> friends=new ArrayList<FBUtil.FBFriend>();
						if (users != null && users.size() > 0) {
							for (int i = 0; i < users.size(); i++) {
								FBFriend poj = new FBUtil().new FBFriend();
								poj.setId(users.get(i).getId());
								poj.setName(users.get(i).getName());
								friends.add(poj);
							}
						}
						nativeSendFriendsList(friends);
						if (response.getError() != null) {
							// Handle errors, will do so later.
						}
					}
				});
		request.executeAsync();
	}
	*/
	
}