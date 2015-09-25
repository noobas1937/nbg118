//package org.hcg.util;
//
//import java.util.UUID;
//
//import org.json.JSONException;
//import org.json.JSONObject;
//
//import android.content.SharedPreferences;
//import android.content.Context;
//import android.util.Log;
//import org.cocos2dx.ext.Device;
//import org.cocos2dx.ext.Tracker;
//import org.cocos2dx.ext.Native;
//
//import org.hcg.IF.Jni;
//import com.google.analytics.tracking.android.EasyTracker;
//import com.google.android.apps.analytics.AdInstallRefMointor;
//import com.xingcloud.analytic.CloudAnalytic;
//import com.xingcloud.analytic.ReportPolicy;
//import com.xingcloud.analytic.user.UserEvent;
//import com.xingcloud.analytic.user.UserField;
//import com.xingcloud.analytic.utils.XTimeStamp;
//import com.xingcloud.analytic.utils.Xutils;
//
//public class CloudAnalysisUitl {
//	
//	private static String mXingCloudAnalysisURL = "http://xa.xingcloud.com/v4/cok/"
//			+ Device.getUid() + "?";
//	
//	public static void init() {
//		try {
//			// 初始化行云统计
//			// 设置APPID
//			CloudAnalytic.instance().setGameAppid(Jni.nativeGetAppId());
//			// 检查广告reffer,注意使用sendVisitAndQuitOutside(true)时，必须手动设置ref并发送
//			AdInstallRefMointor.checkAdInstalledRef(GameContext
//					.getActivityInstance());
//			CloudAnalytic.instance().sendVisitAndQuitOutside(true);
//			CloudAnalytic.instance().setReportUid(
//					GameContext.getActivityInstance(), Device.getUid());
//			CloudAnalytic.instance().setReportPolicy(ReportPolicy.REALTIME,
//					GameContext.getActivityInstance());
//			CloudAnalytic.instance()
//					.onCreate(GameContext.getActivityInstance());
//		} catch (Exception e) {
//			Tracker.reportException(GameContext.getActivityInstance(), e);
//		}
//	}
//
//	public static void xingVist(String appId) {
//		UserField userInfo = new UserField();
//		userInfo.setAppId(appId);
//		userInfo.setEvent(UserEvent.USER_VISIT);
//		// 设置广告安装跟踪
//		String adInstalledInfo = AdInstallRefMointor
//				.checkAdInstalledRef(GameContext.getActivityInstance());
//		Log.d("AdInstall", "AdInstall from--" + adInstalledInfo);
//		if (adInstalledInfo != null) {// 安装通知非空时
//			userInfo.setRef("xafrom=" + adInstalledInfo);
//		} else {
//			userInfo.setRef("xafrom=");
//		}
//
//		JSONObject jsonObject = new JSONObject();
//		try {
//			jsonObject
//					.put("XA_tagname", Xutils.getGameAppVersion(GameContext
//							.getActivityInstance()));
//			jsonObject.put("time", XTimeStamp.getTimeStamp());
//			jsonObject.put("name", Device.getUid());
//			jsonObject.put("clt", "2dx-ad");
//		} catch (JSONException e) {
//			Tracker.reportException(GameContext.getActivityInstance(), e);
//		}
//		userInfo.setJsonVar(jsonObject);
//		userInfo.setUId(Device.getUid());
//		userInfo.setTimestamp(XTimeStamp.getTimeStamp());
//		CloudAnalytic.instance().trackUserEvent(
//				GameContext.getActivityInstance(), userInfo);
//		// 启动时候调用
//		// trackUpdateUser();
//	}
//	
//	public static void trackUpdateUser() {
//		String url = mXingCloudAnalysisURL;
//		url += "update=version,";
//		url += Device.getVersionName();
//		url += "&";
//		url += "update=abflag,";
//		
//		if (Native.nativeGetAbFlag() == null)
//			url += "normal";
//		else
//			url += Native.nativeGetAbFlag();
//		addAnalysisLog(url);
//	}
//	
//	private static void trackAction(String[] actions) {
//	    if (actions == null) {
//	        return;
//	    }
//	    
//	    if (actions.length > 5) {
//	        return;
//	    }
//	    String url = mXingCloudAnalysisURL + "action=";
//	    
//	    int length = actions.length;
//	    for (int i = 0; i < length; i ++) {
//	        url += actions[i];
//	        if (i != length - 1) {
//	            url += ".";
//	        }
//	    }
//        addAnalysisLog(url);
//	}
//	
//	private static void addAnalysisLog(final String xaUrl) {
//		CloudAnalysisManager.addLog(xaUrl);
//	}
//	
//	public static void trackClickEventCheckFirst(String[] actions) {
//        checkIsFirstClickUI(actions);
//        trackAction(actions);
//    }
//	public static void track_ClickEvent(String... action){
//		 trackClickEvent(action);
//	}
//	public static void trackClickEvent(String[] action) {
//
//	    if (action == null) {
//	        return;
//	    }
//	    
//	    // TODO:add type in native
////        if (1 == AppData.getInstance().getSwitchData().getAnalyticType()) {
////            // GA
////            int length = action.length;
////            
////            EasyTracker.getTracker().sendEvent(length > 1 ? action[0] : "Click", 
////                    length > 2 ? action[1] : action[0], 
////                    length > 3 ? action[2] : action[0],
////                    (long) 1);
////        } else if (2 == AppData.getInstance().getSwitchData().getAnalyticType()) {
////            // 行云
//            trackAction(action);
////        } else
////            ;// 不打log
//    }
//	
//	/**
//	 * 重置行云APPID
//	 */
//	public static void resetXingCloudApp() {
//		// 恢复旧的统计ID
//		Log.d("XingCloud", "old_appId="
//				+ GameContext.getGamePublisher().getPublish_XingCould_AppId());
//		CloudAnalytic.instance().setGameAppid(
//				GameContext.getGamePublisher().getPublish_XingCould_AppId());
//		CloudAnalytic.instance().update(GameContext.getActivityInstance());
//	}
//
//	private static boolean isFirstClickUI = true;
//	private static void checkIsFirstClickUI(String[] action) {
//		if (isFirstClickUI) {
//			isFirstClickUI = false;
//			if(CommonUtil.isNewInstallDevice()){
//			    action[action.length - 1] += "_firstclick_newuser";
//			}else{
//                action[action.length - 1] += "_firstclick";
//			}
//            trackClickEvent(action);
//		}
//	}
//
//	private static boolean isFirstEnterMainScene=true;
//	public static void checkIsFirstEnterMainScene(){
//		if(isFirstEnterMainScene){
//			isFirstEnterMainScene=false;
//			if(CommonUtil.isNewInstallDevice()){
//				String[] strs = {"UI_CityView","MainScene","newuser"};
//			    trackClickEvent(strs);
//			}else{
//				String[] strs = {"UI_CityView","MainScene","normal"};
//			    trackClickEvent(strs);
//			}
//		}
//	}
//}
