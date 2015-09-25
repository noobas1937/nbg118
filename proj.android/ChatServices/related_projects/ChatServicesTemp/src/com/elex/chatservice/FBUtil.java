package com.elex.chatservice;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Currency;
import java.util.List;

import org.json.JSONArray;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.FrameLayout.LayoutParams;
import android.widget.Toast;

import com.elex.chatservice.ChatInputView;

public class FBUtil {
//	private static String TAG="FBUtil";
//	private static int REQUEST_ERROR=2;
//	private static int REQUEST_CANCEl=3;
//	private static int REQUEST_SUCESS=1;
//    
//    private static boolean _inFeedDialogue = false;
//	
//	public native static void nativeSendFriendsList(String list);//ArrayList<FBFriendPoj> list
//	public static native void nativeSetIsLogin(boolean isLogin,String userID,String userName);
//	public static native void nativeSetRequestResult(int result);
//	public static native void nativeSetIsFBFan(boolean isFBFan);
//    public static native void nativeSetFeedDialogResult(int result);
//    public native static void nativeSendInviteFriendsList(String list);//ArrayList<FBFriendPoj> list
//	
//	public static boolean Login() {
//		LoginButton loginButton = new LoginButton(IF.getInstance());
//		loginButton.setReadPermissions(Arrays.asList("public_profile","user_friends"));
////		loginButton.LoginOnClick();
////		loginButton.callOnClick();
//		boolean flag = true;
//		try {
//			Log.d("fb", "Login");
//			Method loginOnClickMethod = LoginButton.class.getMethod("LoginOnClick", null);
//			loginOnClickMethod.invoke(loginButton);
//			Log.d("fb", "Login later");
//		} catch (Exception e){
//			Method callOnClickMethod;
//			Log.d("fb", "Login exception1"+e.getMessage());
//			try {
//				callOnClickMethod = LoginButton.class.getMethod("callOnClick", null);
//				callOnClickMethod.invoke(loginButton);
//			} catch (Exception e1) {
//				Log.d("fb", "Login exception2"+e1.getMessage());
//				flag = false;
//			}
//		}
//		return flag;
////	}
//	private static int baseY = 0;
//	private static ChatInputView chatInputView;
//	public static void showChatInputView(int x, int y){
//		System.out.println("showChatInputView");
//		baseY += 5;
//		DisplayMetrics dm = new DisplayMetrics();
//		IF.getInstance().getWindowManager().getDefaultDisplay().getMetrics(dm);
//		final float posX = 0;
//		final float posY = dm.heightPixels * (baseY - 4) / 100;
//		System.out.println("chatInputView = "+chatInputView);
//		if(chatInputView != null){
//			IF.getInstance().runOnUiThread(new Runnable() {
//				public void run() {
//					try {
////						chatInputView.setX(posX);
////						chatInputView.setY(posY);
//						android.widget.RelativeLayout.LayoutParams layoutParams = chatInputView.getContainerViewLayoutParams();
//						layoutParams.topMargin = (int)posY;
//						chatInputView.setContainerViewLayoutParams(layoutParams);
//						chatInputView.setVisibility(View.VISIBLE);
//					} catch (Exception e) {
////						reportException(Main.getInstance(), e);
//					}
//				}
//			});
//		}
//	}
//	public static void hideChatInputView(){
//		System.out.println("hideChatInputView");
//		if(chatInputView != null){
//			IF.getInstance().runOnUiThread(new Runnable() {
//				public void run() {
//					try {
////						chatInputView.setX(0);
////						chatInputView.setY(0);
//						chatInputView.setVisibility(View.GONE);
//					} catch (Exception e) {
////						reportException(Main.getInstance(), e);
//					}
//				}
//			});
//		}
//	}
//	
//	public static ChatInputView getChatInputView(Context context){
//		System.out.println("getChatInputView");
//		int visibility = View.GONE;
//		System.out.println("chatInputView = "+chatInputView);
//		if(chatInputView != null){
//			visibility = chatInputView.getVisibility();
//			ViewGroup parent = (ViewGroup)chatInputView.getParent();
//			parent.removeView(chatInputView);
//		}
////		if(chatInputView == null){
//			chatInputView = new ChatInputView(context);
////			chatInputView.setObjectId("https://www.facebook.com/Clash.Of.Kings.Game");
//			chatInputView.setVisibility(visibility);
////			chatInputView.setForegroundColor(0xFF602800);
////			chatInputView.getLikeButton().setOnClickListener(new OnClickListener() {
////	            @Override
////	            public void onClick(View v) {
////	            	chatInputView.toggleLike();
////	            	Log.d("fb", "fblike after click");
////	            }
////	        });
////		}
//		return chatInputView;
//	}
}
