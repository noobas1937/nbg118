package com.clash.of.kings;

import com.crashlytics.android.Crashlytics;

import io.fabric.sdk.android.Fabric;

import java.util.Locale;

import org.nbg.notifies.LocalNotificationManager;
import org.cocos2dx.ext.Native;
import org.nbg.IF.IF;
import org.nbg.stac.empire.common.activity.CommonActivity;
import org.nbg.stac.empire.pay.PayItemData;
import org.nbg.stac.empire.publish.IPublishChannel;
import org.json.JSONArray;
import org.json.JSONObject;

import android.app.Activity;
import android.app.NativeActivity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.clash.of.util.NotifyUtil;
import com.clash.of.weibo.Constants;
import com.clash.of.weibo.WBUtil;
import com.pingplusplus.android.PingppLog;
import com.pingplusplus.libone.PayActivity;
import com.sina.weibo.sdk.api.TextObject;
import com.sina.weibo.sdk.api.WeiboMultiMessage;
import com.sina.weibo.sdk.api.share.BaseResponse;
import com.sina.weibo.sdk.api.share.IWeiboHandler;
import com.sina.weibo.sdk.api.share.IWeiboShareAPI;
import com.sina.weibo.sdk.api.share.SendMultiMessageToWeiboRequest;
import com.sina.weibo.sdk.api.share.WeiboShareSDK;
import com.sina.weibo.sdk.constant.WBConstants;
import com.stac.empire.publish.PublishImpl;
import com.crashlytics.android.ndk.CrashlyticsNdk;

public class NBG_CN1 extends CommonActivity implements IWeiboHandler.Response {
	private static final String TAG = "COK_CN";
	static PayItemData payItemData;
	public static IWeiboShareAPI mWeiboShareAPI;
	
	@Override
	protected void recordCmd(String cmd){
		SharedPreferences sharedPreferences = this.getSharedPreferences("crashCommand", Context.MODE_PRIVATE);
		Editor editor = sharedPreferences.edit();//获取编辑器
		editor.putString("name", cmd);
		System.out.print(cmd);
		editor.commit();//提交修改
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());
		final SharedPreferences sharedPreferences = this.getSharedPreferences(LocalNotificationManager.CACHE_GAME_UID_KEY, Context.MODE_PRIVATE);
		String gameUid = sharedPreferences.getString("GAMEUID", "");
		
		final SharedPreferences nameSharedPreferences = this.getSharedPreferences("crashCommand", Context.MODE_PRIVATE);
		String name = nameSharedPreferences.getString("name", "");

		Log.d("COK UID", gameUid);
		Crashlytics.setUserIdentifier(gameUid + "_" + name);

		Native.nativeSetFromCountry(Locale.getDefault().getCountry());
		Native.nativeSetParseRegisterId(NotifyUtil.getParseNotifyToken());
		
		NotifyUtil.trackAppOpened(getIntent());
        mWeiboShareAPI = WeiboShareSDK.createWeiboAPI(this, Constants.APP_KEY);
        mWeiboShareAPI.registerApp();
        WBUtil.init(this);
        String autoDownloadFlag = Native.nativeGetConfig("isAutoDownloadOn");
        Log.d("autoDownloadFlag", autoDownloadFlag);
		if(isWifi(IF.getInstance()) && !autoDownloadFlag.equals("0")){
			new UpdateManager(IF.getInstance()).checkUpdate();
		}
	}
	
	private static boolean isWifi(Context mContext) {  
	    ConnectivityManager connectivityManager = (ConnectivityManager) mContext  
	            .getSystemService(Context.CONNECTIVITY_SERVICE);  
	    NetworkInfo activeNetInfo = connectivityManager.getActiveNetworkInfo();  
	    if (activeNetInfo != null  
	            && activeNetInfo.getType() == ConnectivityManager.TYPE_WIFI) {  
	        return true;  
	    }  
	    return false;
	}
	
	@Override
	public void onResume() {
		super.onResume();
	}
	
	@Override
	protected void onPause() {
		super.onPause();
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy(); 
	}
	
	@Override
	public IPublishChannel getPublishImpl() {
		return new PublishImpl();
	}
	
	public static void doLogin() {
	}
	
	public static void doPay(PayItemData payItemData) {
		NBG_CN1.payItemData = payItemData;
		int price = (int)(Math.ceil(payItemData.getItemPrice()*100));
		Log.d(TAG, "cok pay clicked");
		String productName = String.format("%d金币礼包", payItemData.getItemCoin());
		if("9006".equals(payItemData.getItemId()))
			productName = "月卡";
		String orderId = String.format("%s_%d_%s_%d", 
				payItemData.getUserId(), // UID   20
				Native.nativeGetServerId(), // 服务器ID 2+1
				payItemData.getItemId(), // xmlId 4+1
				System.currentTimeMillis() // 当前时间 13+1
				);
		String productId = "";
		switch (price){
			case 499:
				productId = "3000";
				break;
			case 999:
				productId = "6800";
				break;
			case 1999:
				productId = "12800";
				break;
			case 2499:
				productId = "16300";
				break;
			case 4999:
				productId = "32800";
				break;
			case 9999:
				productId = "64800";
				break;
			case 99:
				productId = "600";
				break;
			case 99999:
				productId = "649800";
				break;
			default:
				return;
		}
//		StringBuilder sb = new StringBuilder("http://pay.337.com/payment/mobiledo?");
//		sb.append("currency=CNY");
//		sb.append("&elex_mobile=true");
//		sb.append("&amount="+productId);
//		sb.append("&payment_method=alipay");
//		sb.append("&description="+productName);
//		sb.append("&vamount="+String.valueOf(payItemData.getItemCoin()));
//		sb.append("&role="+payItemData.getUserId());
//		sb.append("&custom_data="+orderId);
//		sb.append("&uid="+payItemData.getUserId());
//		sb.append("&product_id="+payItemData.getItemId());
//		sb.append("&app_key=cok@elex337_zh_1");
//		sb.append("&phone=");
//		sb.append("&channel=alipay");
//		sb.append("&country=CN");
//		String url = sb.toString();
//		
//		Native.nativeShowWaitInterface();
//		
//		DisplayMetrics dm = new DisplayMetrics();
//		IF.getInstance().getWindowManager().getDefaultDisplay().getMetrics(dm);
//		IF.getInstance().displayWebView(0, 0, dm.widthPixels, dm.heightPixels);
//		IF.getInstance().updateURL(url);
		// 构建账单json对象
		String serverUrl = "http://s1.cok.elexapp.com:8080/gameservice/paymentpingpprequest";
		JSONObject bill = new JSONObject();
		try {
			JSONObject displayItem = new JSONObject();
			displayItem.put("name", "物品");
			JSONArray billList = new JSONArray();
			billList.put(productName);
			displayItem.put("contents", billList);
			JSONArray display = new JSONArray();
			display.put(displayItem);
			bill.put("order_no", orderId);
			bill.put("amount", productId);
			bill.put("display", display);
//			bill.put("extras", extras);// 该字段选填
		} catch (Exception e) {
			e.printStackTrace();
		}
		PayActivity.SHOW_CHANNEL_WECHAT = true;
//		PayActivity.SHOW_CHANNEL_UPMP = true;
		PayActivity.SHOW_CHANNEL_BFB = true;
		PayActivity.SHOW_CHANNEL_ALIPAY = true;
		// PingPP日志开关
		PingppLog.DEBUG = true;

		// 发起支付
		PayActivity.CONNECT_TIMEOUT = 10000;//（单位毫秒）
	    PayActivity.READ_TIMEOUT = 10000;
		PayActivity.CallPayActivity(IF.getInstance(), bill.toString(), serverUrl);

	}
	
	public static void share(String shareText){
		WeiboMultiMessage weiboMessage = new WeiboMultiMessage();
		TextObject textObject = new TextObject();
		textObject.text = shareText;
		SendMultiMessageToWeiboRequest request = new SendMultiMessageToWeiboRequest();
		request.transaction = String.valueOf(System.currentTimeMillis());
		request.multiMessage = weiboMessage;
		mWeiboShareAPI.sendRequest(IF.getInstance(), request);
	}
	
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
	    if (requestCode == PayActivity.PAYACTIVITY_REQUEST_CODE) {
	        if (resultCode == PayActivity.PAYACTIVITY_RESULT_CODE) {
	        	if("pay_successed".equals(data.getExtras().getString("result"))){
		            Toast.makeText(this, "支付成功请等待服务器处理", Toast.LENGTH_LONG).show();
	        	}
//	            Toast.makeText(this, data.getExtras().getString("result"), Toast.LENGTH_LONG).show();
	        }else if(resultCode == Activity.RESULT_CANCELED){
	            //pressed back
//	            Toast.makeText(this, "支付取消", Toast.LENGTH_LONG).show();
//	            Toast.makeText(this, data.getExtras().getString("result"), Toast.LENGTH_LONG).show();
	        }
	    }
        // SSO 授权回调
        // 重要：发起 SSO 登陆的 Activity 必须重写 onActivityResults
        if (WBUtil.mSsoHandler != null) {
        	WBUtil.mSsoHandler.authorizeCallBack(requestCode, resultCode, data);
        }
	}
	
	@Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        // 从当前应用唤起微博并进行分享后，返回到当前应用时，需要在此处调用该函数
        // 来接收微博客户端返回的数据；执行成功，返回 true，并调用
        // {@link IWeiboHandler.Response#onResponse}；失败返回 false，不调用上述回调
        mWeiboShareAPI.handleWeiboResponse(intent, this);
    }

    /**
     * 接收微客户端博请求的数据。
     * 当微博客户端唤起当前应用并进行分享时，该方法被调用。
     * 
     * @param baseRequest 微博请求数据对象
     * @see {@link IWeiboShareAPI#handleWeiboRequest}
     */
    @Override
    public void onResponse(BaseResponse baseResp) {
        switch (baseResp.errCode) {
        case WBConstants.ErrorCode.ERR_OK:
            break;
        case WBConstants.ErrorCode.ERR_CANCEL:
            break;
        case WBConstants.ErrorCode.ERR_FAIL:
        	Toast.makeText(this, "登陆失败", Toast.LENGTH_LONG).show();
            break;
        }
	}

}
