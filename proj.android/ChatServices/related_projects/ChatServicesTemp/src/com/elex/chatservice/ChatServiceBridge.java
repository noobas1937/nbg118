/**
 * Copyright (c) 2013-2014 by Helpshift, Inc. All Rights Reserved.
 *
 */
package com.elex.chatservice;

import java.util.ArrayList;
import java.util.HashMap;

import android.app.Activity;
import android.content.Context;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;

public class ChatServiceBridge {
	private static final String TAG = "HSCocosPlugin";
	private static Activity activity;

	public static void alertToRateAppAction(int result) {

	}
	public static native void onBackPressed();
	public static native void onTextChanged(String msg);
	public static void sendMessage(String msg) {

	}

	public static void set2dxViewHeight(int height, int usableHeightSansKeyboard) {

	}

	public static void init(Activity a) {
		ChatServiceBridge.activity = a;
	}

    public static boolean enableChatService = true; 
	private static int baseY = 0;
	private static ChatInputView chatInputView;
	
	public static void initChatInputView(Activity activity)
	{
		if(!enableChatService) return;
		
		ChatServiceBridge.activity = activity;
		try{
			getChatInputView(activity);
			activity.addContentView(chatInputView, new LayoutParams(LayoutParams.FILL_PARENT,LayoutParams.FILL_PARENT));
//		    AndroidBug5497Workaround.assistActivity(chatInputView);
		} catch (Exception e) {
			printException(e);
		}
	}

	public static ChatInputView getChatInputView(Activity activity) {
		System.out.println("getChatInputView");
		if(!enableChatService)
			return null;
		
		int visibility = View.GONE;
		if (chatInputView != null) {
			visibility = chatInputView.getVisibility();
			ViewGroup parent = (ViewGroup) chatInputView.getParent();
			parent.removeView(chatInputView);
		}
		
		chatInputView = new ChatInputView(activity);
		chatInputView.setVisibility(visibility);
		
		return chatInputView;
	}

	public static String getChatInputText(){
		if(!enableChatService)
			return "";
		
		if(chatInputView != null){
			try{
				return chatInputView.getInputText();
			}catch(Exception e)
			{
			    printException(e);
			}
		}
		return "";
	}
	
	public static void disableChatInputView(){
		if(!enableChatService) return;
		
		System.out.println("disableChatInputView");
		setChatInputViewEnabled(false);
	}
	
	public static void enableChatInputView(){
		if(!enableChatService) return;
		
		System.out.println("enableChatInputView");
		setChatInputViewEnabled(true);
	}

	private static void setChatInputViewEnabled(final boolean enabled){
		if(!enableChatService) return;
		
	    activity.runOnUiThread(new Runnable() {
	        @Override
	        public void run() {
	  		  try{
					if(chatInputView != null && chatInputView.getVisibility() == View.VISIBLE){
						chatInputView.setEnabled(enabled);
					}
			  }catch(Exception e)
			  {
				  printException(e);
			  }
	        }
	    });
	}
	
	public static void toggleFullScreen(final boolean enabled){
//		System.out.println("toggleFullScreen: " + enabled);
//		activity.runOnUiThread(new Runnable() {
//			public void run() {
//				try {
////					activity.requestWindowFeature(Window.FEATURE_NO_TITLE);//去掉标题栏
//					if(enabled){
//						activity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN); 
//						activity.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN);
//					}else{
//						activity.getWindow().setFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN, WindowManager.LayoutParams.FLAG_LAYOUT_INSET_DECOR);
//						activity.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN); 
////						activity.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN);
//					}
//				} catch (Exception e) {
//				}
//			}
//		});
	}
	
	public static void showChatInputView() {
		if(!enableChatService) return;
		
		System.out.println("showChatInputView");
		
		if (chatInputView != null) {
			activity.runOnUiThread(new Runnable() {
				public void run() {
					try {
						chatInputView.setVisibility(View.VISIBLE);
					} catch (Exception e) {
						// reportException(Main.getInstance(), e);
					}
				}
			});
		}
	}

	public static void hideChatInputView() {
		if(!enableChatService) return;
		
		System.out.println("hideChatInputView");
		
		if (chatInputView != null) {
			activity.runOnUiThread(new Runnable() {
				public void run() {
					try {
						chatInputView.setVisibility(View.GONE);
						chatInputView.hideKeyboard();
					} catch (Exception e) {
						// reportException(Main.getInstance(), e);
					}
				}
			});
		}
	}

	public static void setSendButtonText(final String text) {
		if(!enableChatService) return;
		
		System.out.println("setSendButtonText: "+text);
		
		if (chatInputView != null) {
			activity.runOnUiThread(new Runnable() {
				public void run() {
					try {
						chatInputView.setSendButtonText(text);
					} catch (Exception e) {
						// reportException(Main.getInstance(), e);
					}
				}
			});
		}
	}

	public static void setEditTextHintText(final String hint) {
		if(!enableChatService) return;
		
		System.out.println("setEditTextHintText: "+hint);
		
		if (chatInputView != null) {
			activity.runOnUiThread(new Runnable() {
				public void run() {
					try {
						chatInputView.setEditTextHintText(hint);
					} catch (Exception e) {
						// reportException(Main.getInstance(), e);
					}
				}
			});
		}
	}

	private static HashMap parseConfigDictionary(HashMap config) {
		if (config == null) {
			return new HashMap();
		}
		if (config.get("enableInAppNotification") != null) {
			if (config.get("enableInAppNotification").equals("yes")) {
				config.put("enableInAppNotification", true);
			} else {
				config.put("enableInAppNotification", false);
			}
		}

		if (config.get("enableContactUs") != null) {
			if (config.get("enableContactUs").equals("yes")
					|| config.get("enableContactUs").equals("always")) {
				config.put("enableContactUs",
						Helpshift.ENABLE_CONTACT_US.ALWAYS);
			} else if (config.get("enableContactUs").equals("no")
					|| config.get("enableContactUs").equals("never")) {
				config.put("enableContactUs", Helpshift.ENABLE_CONTACT_US.NEVER);
			} else if (config.get("enableContactUs").equals(
					"after_viewing_faqs")) {
				config.put("enableContactUs",
						Helpshift.ENABLE_CONTACT_US.AFTER_VIEWING_FAQS);
			}
		}

		if (config.get("gotoConversationAfterContactUs") != null) {
			if (config.get("gotoConversationAfterContactUs").equals("yes")) {
				config.put("gotoConversationAfterContactUs", true);
			} else {
				config.put("gotoConversationAfterContactUs", false);
			}
		}

		if (config.get("requireEmail") != null) {
			if (config.get("requireEmail").equals("yes")) {
				config.put("requireEmail", true);
			} else {
				config.put("requireEmail", false);
			}
		}

		HashMap customMeta = (HashMap) (config
				.get(Helpshift.HSCustomMetadataKey));
		if (customMeta != null) {
			ArrayList<String> tags = (ArrayList<String>) customMeta
					.get(Helpshift.HSTagsKey);
			if (tags != null && tags.size() > 0) {
				String[] tagsArray = (String[]) tags.toArray(new String[tags
						.size()]);
				customMeta.put(Helpshift.HSTagsKey, tagsArray);
			}
		}
		return config;
	}

	public static void install(Activity a, String apiKey, String domain,
			String appId) {
		HashMap<String, Object> config = new HashMap<String, Object>();
		config.put("sdkType", "cocos2dx");
		ChatServiceBridge.install(a, apiKey, domain, appId, config);
	}

	public static void install(Activity a, String apiKey, String domain,
			String appId, HashMap config) {
		ChatServiceBridge.activity = a;
		HashMap parsedConfig = parseConfigDictionary(config);
		parsedConfig.put("sdkType", "cocos2dx");
		Helpshift.install(ChatServiceBridge.activity.getApplication(), apiKey,
				domain, appId, parsedConfig);
	}

	public static void showConversation() {
		try {
			System.out.println("showConversation void");
			ChatServiceBridge.showConversation(new HashMap());
		} catch (Exception e) {
			printException(e);
		}
	}

	public static void printException(Exception e) {
		System.out.println(e.getMessage());

		if (e.getCause() != null)
			System.out.println("cause:" + e.getCause().getMessage());

		e.printStackTrace();

		StackTraceElement[] stack = e.getStackTrace();
		if (stack != null)
			System.out.println("stack:");
		for (int i = 0; i < stack.length; i++) {
			StackTraceElement stackTraceElement = stack[i];
			System.out.println(stackTraceElement.toString());
		}
	}

	public static void showConversation(HashMap config) {
		try {
			System.out.println("showConversation");
			Helpshift.showConversation(ChatServiceBridge.activity,
					parseConfigDictionary(config));
		} catch (Exception e) {
			printException(e);
		}
	}

	public static void showChatActivity() {
		try {
			System.out.println("ChatServiceBridge showChatActivity()");
			Helpshift.showChatActivity(ChatServiceBridge.activity);
		} catch (Exception e) {
			printException(e);
		}
	}

	public static void showFAQSection(String sectionPublishId) {
		ChatServiceBridge.showFAQSection(sectionPublishId, new HashMap());
	}

	public static void showFAQSection(String sectionPublishId, HashMap config) {
		Helpshift.showFAQSection(ChatServiceBridge.activity, sectionPublishId,
				parseConfigDictionary(config));
	}

	public static void showSingleFAQ(String questionPublishId) {
		ChatServiceBridge.showSingleFAQ(questionPublishId, new HashMap());
	}

	public static void showSingleFAQ(String questionPublishId, HashMap config) {
		Helpshift.showSingleFAQ(ChatServiceBridge.activity, questionPublishId,
				parseConfigDictionary(config));
	}

	public static void showFAQs() {
		try {
			System.out.println("showFAQs void");
			ChatServiceBridge.showFAQs(new HashMap());
		} catch (Exception e) {
			printException(e);
		}
	}

	public static void showFAQs(HashMap config) {
		try {
			System.out.println("showFAQs");
			Helpshift.showFAQs(ChatServiceBridge.activity,
					parseConfigDictionary(config));
		} catch (Exception e) {
			printException(e);
		}
	}

	public static void setNameAndEmail(String name, String email) {
		Helpshift.setNameAndEmail(name, email);
	}

	public static void setUserIdentifier(String identifier) {
		Helpshift.setUserIdentifier(identifier);
	}

	public static void registerDeviceToken(String deviceToken) {
		Helpshift.registerDeviceToken(ChatServiceBridge.activity, deviceToken);
	}

	public static void leaveBreadCrumb(String breadCrumb) {
		Helpshift.leaveBreadCrumb(breadCrumb);
	}

	public static void clearBreadCrumbs() {
		Helpshift.clearBreadCrumbs();
	}

	public static Integer getNotificationCount() {
		return Helpshift.getNotificationCount();
	}

	public static void handlePush(String issueId) {
		Helpshift.handlePush(ChatServiceBridge.activity, issueId);
	}

	public static void showAlertToRateApp(String url) {
		HSAlertToRateAppListener actionListener = new HSAlertToRateAppListener() {
			@Override
			public void onAction(Helpshift.HS_RATE_ALERT action) {
				int msg = 0;
				switch (action) {
				case CLOSE:
					msg = 0;
					break;
				case FEEDBACK:
					msg = 1;
					break;
				case SUCCESS:
					msg = 2;
					break;
				case FAIL:
					msg = 3;
					break;
				}
				alertToRateAppAction(msg);
			}
		};
		Helpshift.showAlertToRateApp(url, actionListener);
	}
}
