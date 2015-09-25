package com.example.sample1;

import java.io.File;
import java.util.ArrayList;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.CookieManager;
import android.webkit.CookieSyncManager;
import android.webkit.WebView;
import android.widget.Button;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageItem;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.TimeManager;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.util.ResUtil;

public class WrapperChatServiceMainActivity extends Activity {
	private Button buttonChannel;
	private Button buttonChat;
	private Button mailChat;
	private Button buttonForum;
	private Button buttonMemberSelectorFragment;
	
	public WrapperChatServiceMainActivity()
	{
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
//		clearWebViewCache();

		setContentView(R.layout.wrapper_chat_service_main);

		ServiceInterface.setIsNewMailListEnable(true);
		ChatServiceController.init(this, true);
		init();

		buttonChannel = (Button)findViewById(ResUtil.getId(this, "id", "button0"));
		buttonChannel.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	showChannelList();
            }
        });
		
		buttonChat = (Button)findViewById(ResUtil.getId(this, "id", "button1"));
		buttonChat.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	showChatFragment();
            }
        });

		mailChat = (Button)findViewById(ResUtil.getId(this, "id", "button3"));
		mailChat.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	showMailFragment();
            }
        });

		buttonForum = (Button)findViewById(ResUtil.getId(this, "id", "button2"));
		buttonForum.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	showForumFragment();
            }
        });

		buttonMemberSelectorFragment = (Button)findViewById(ResUtil.getId(this, "id", "button4"));
		buttonMemberSelectorFragment.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	showMemberSelectorFragment();
            }
        });

//		showMemberSelectorFragment();
		
		ArrayList<String> fechingUids = new ArrayList<String>();
		fechingUids.add("1");
		fechingUids.add("try");
		System.out.println("fechingUids.contains(1)" + fechingUids.contains("1"));
		System.out.println("fechingUids.contains(try)" + fechingUids.contains(new String("try")));
		fechingUids.remove(new String("try"));
		fechingUids.remove(new String("1"));
		System.out.println("fechingUids.size()" + fechingUids.size());
	}

    private static final String TAG = WrapperChatServiceMainActivity.class.getSimpleName();
    private static final String APP_CACAHE_DIRNAME = "/webcache";
    
	private void showChannelList() {
		ServiceInterface.showChannelListActivity(WrapperChatServiceMainActivity.this, false, -1, null, false);
	}
	
	private void showChatFragment() {
		ServiceInterface.showChatActivity(WrapperChatServiceMainActivity.this, DBDefinition.CHANNEL_TYPE_COUNTRY, false);
	}

	private void showMailFragment() {
		ServiceInterface.showChatActivity(WrapperChatServiceMainActivity.this, DBDefinition.CHANNEL_TYPE_USER, false);
	}

	private void showMemberSelectorFragment() {
//		ActivityManager activityManager = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
//		System.out.println("getMemoryClass: " + activityManager.getMemoryClass());
//		System.out.println("getLargeMemoryClass: " + activityManager.getLargeMemoryClass());
		ServiceInterface.showMemberSelectorActivity(WrapperChatServiceMainActivity.this,true);
	}

	private boolean target = true;
	private void showForumFragment() {
//		target = !target;
//		if(target){
//			ServiceInterface.showChatActivity(WrapperChatServiceMainActivity.this, false, "http://f.elex.com/forums/4-Announcements");
//		}else{
			ServiceInterface.showForumActivity(WrapperChatServiceMainActivity.this, null);
//		}
	}

	@Override
	protected void onResume() {
		super.onResume();
		ChatServiceController.setCurrentActivity(null);
	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	protected void onStop() {
		super.onStop();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		
		// 只能关闭其它task的一个activity
//		android.os.Process.killProcess(android.os.Process.myPid());

//		ServiceInterface.clearStack();
//		int currentVersion = android.os.Build.VERSION.SDK_INT;
//		if (currentVersion > android.os.Build.VERSION_CODES.ECLAIR_MR1)
//		{
//			Intent startMain = new Intent(Intent.ACTION_MAIN);
//			startMain.addCategory(Intent.CATEGORY_HOME);
//			startMain.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//			startActivity(startMain);
//			System.exit(0);
//		}
//		else
//		{
//			// android2.1
//			ActivityManager am = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
//			am.restartPackage(getPackageName());
//		}
	}

	public void init() {
		UserManager.getInstance().setCurrentUserId("379997000002");
		DBManager.getInstance().initDB(this);
		ServiceInterface.setPlayerInfo(	1,
										TimeManager.getInstance().getCurrentTime(),
										2,
										0,
										"zhou",
										"379997000002",
										"g044",
										4,
										TimeManager.getInstance().getCurrentLocalTime() + 60,
										TimeManager.getInstance().getCurrentTime(),
										-1);
		ServiceInterface.setPlayerAllianceInfo("zhe", "allianceIdX", 2, true);
		ConfigManager.getInstance().gameLang = "en";

		LanguageItem[] chatLangArray={
				new LanguageItem("E100068","您未加入联盟，暂时无法使用联盟聊天频道"),
				new LanguageItem("115020","加入"),
				new LanguageItem("105207","禁言"),
				new LanguageItem("105209","已禁言"),
				new LanguageItem("105210","是否禁言：{0}？"),
				new LanguageItem("105300","国家"),
				new LanguageItem("105302","发送"),
				new LanguageItem("105304","复制"),
				new LanguageItem("105307","您发布的聊天消息过于频繁，请稍等！"),
				new LanguageItem("105308","发送邮件"),
				new LanguageItem("105309","查看玩家"),
				new LanguageItem("105312","屏蔽"),
				new LanguageItem("105313","是否要屏蔽{0}，屏蔽后你将不会收到该玩家的任何聊天信息和邮件，但是你可以通过设置来解除对该玩家的屏蔽。"),
				new LanguageItem("105315","解除屏蔽"),
				new LanguageItem("105316","聊天"),
				new LanguageItem("105321","由{0}翻译"),
				new LanguageItem("105322","原文"),
				new LanguageItem("105502","下滑加载更多"),
				new LanguageItem("105602","联盟"),
				new LanguageItem("108584","邀请加入联盟"),
				new LanguageItem("115922","屏蔽该玩家留言"),
				new LanguageItem("115923","屏蔽该联盟留言"),
				new LanguageItem("115925","是否要屏蔽{0}，屏蔽后该玩家将无法对您的联盟进行留言，但是你可以通过联盟管理来解除对该玩家的屏蔽。"),
				new LanguageItem("115926","是否要屏蔽{0}，屏蔽后该联盟将无法对您的联盟进行留言，但是你可以通过联盟管理来解除对该联盟的屏蔽。"),
				new LanguageItem("115929","联盟留言"),
				new LanguageItem("115181","系统"),
				new LanguageItem("115182","我邀请了{0}加入我们的联盟，希望他能和我们一起并肩作战。"),
				new LanguageItem("115281","查看战报"),
				new LanguageItem("115282","抱歉，该战报已无法查看"),
				new LanguageItem("115168","立即加入联盟获得金币"),
				new LanguageItem("115169","{0}金币"),
				new LanguageItem("115170","通过邮件发送"),
				new LanguageItem("105326","翻译"),
				new LanguageItem("105327","下拉可加载更多"),
				new LanguageItem("105328","松开载入更多"),
				new LanguageItem("105324","服务器即将在{0}分后停机更新"),
				new LanguageItem("105325","服务器即将在{0}秒后停机更新"),
				new LanguageItem("115068","立即加入"),
				new LanguageItem("confirm","确定"),
				new LanguageItem("cancel_btn_label","取消"),
				new LanguageItem("114110","加载中"),
				new LanguageItem("104932","刷新"),
				new LanguageItem("105564","全体联盟成员"),
				new LanguageItem("101205", "邮件"),
				new LanguageItem("105329", "论坛"),
				new LanguageItem("105522", "侦察战报"),
				new LanguageItem("105591", "小时"),
				new LanguageItem("105330", "是否重发此消息?"),
				new LanguageItem("105332", "发送王国公告将消耗1个 {0}！"),
				new LanguageItem("104371", "号角"),
				new LanguageItem("105333", "领主大人，您的 {0} 不足，花费一些金币即可发送王国公告！"),
				new LanguageItem("104912", "领主大人，您的金币不足，赶快去购买一些吧！"),
				new LanguageItem("105331", "公告"),
				new LanguageItem("103001", "VIP{0}"),
				new LanguageItem("105352", "{0}条新消息"),
				new LanguageItem("105353", "以下为新消息"),
				new LanguageItem("105369", "以下是最近{0}条新消息"),
				new LanguageItem("105384", "信息"),
				new LanguageItem("105519", "战斗报告"),
				new LanguageItem("103731", "列王的纷争游戏工作室"),
				new LanguageItem("132000", "联系MOD"),
				new LanguageItem("108523", "删除"),
				new LanguageItem("105569", "公告"),
				new LanguageItem("105516", "资源采集报告"),
				new LanguageItem("114121", "资源帮助报告"),
				new LanguageItem("103715", "怪物"),
				new LanguageItem("111660", "我刚刚在铁匠铺在成功的锻造出了{0}"),
				new LanguageItem("138039", "战场"),
				new LanguageItem("105504", "内容"),
				new LanguageItem("105505", "收件人")
				};
		LanguageManager.initChatLanguage(chatLangArray);
		
		UserInfo[] userInfos = getDummyUsers();
		for (int i = 0; i < userInfos.length; i++)
		{
			UserManager.getInstance().addUser(userInfos[i]);
		}
		UserInfo cuser = UserManager.getInstance().getCurrentUser();

		MsgItem[] msgs = getDummyMsgs(userInfos, cuser);
		for (int i = 0; i < msgs.length; i++)
		{
			msgs[i].sendState = MsgItem.SEND_SUCCESS;
			ChannelManager.getInstance().countryChannel.msgList.add(msgs[i]);
		}
		DBManager.getInstance().insertMessages(msgs, ChannelManager.getInstance().countryChannel.getChatTable());
		
		ChatChannel chatRoom = ChannelManager.getInstance().addDummyChannel(DBDefinition.CHANNEL_TYPE_CHATROOM, "dummyChatRoom");
		for (int i = 0; i < userInfos.length; i++)
		{
			chatRoom.memberUidArray.add(userInfos[i].uid);
		}
		chatRoom.latestTime = TimeManager.getInstance().getCurrentTime();
		MsgItem[] msgs2 = getDummyMsgs(userInfos, cuser);
		for (int i = 0; i < msgs2.length; i++)
		{
			msgs2[i].sendState = MsgItem.SEND_SUCCESS;
			chatRoom.msgList.add(msgs2[i]);
		}
		
//		ChannelManager.getInstance().countryChannel.msgList.get(0).user = UserManager.getInstance().getCurrentUser();
//		ChannelManager.getInstance().countryChannel.msgList.get(4).user = UserManager.getInstance().getCurrentUser();
//		System.out.println(LogUtil.typeToString(ChannelManager.getInstance().countryChannel.msgList.get(0)));
//		System.out.println(LogUtil.compareObjects(new Object[]{ChannelManager.getInstance().countryChannel.msgList.get(0), ChannelManager.getInstance().countryChannel.msgList.get(3), ChannelManager.getInstance().countryChannel.msgList.get(4)}));
//		System.out.println(LogUtil.typeToString(UserManager.getInstance().getCurrentUser()));
//		System.out.println(LogUtil.compareObjects(new Object[]{UserManager.getInstance().getCurrentUser(), UserManager.getInstance().getCurrentUser()}));
		
//		ChatServiceController.currentChatType=0;
//		ChatServiceController.isInMailDialog=true;

		TimeManager.getInstance().setServerBaseTime(Math.round(System.currentTimeMillis() / 1000));
	}

	private UserInfo[] getDummyUsers()
	{
		UserInfo[] userInfos = {
			new UserInfo(5, 0, 0, 7, "131762465000002", "Ned", "Winterfell", "g045", TimeManager.getInstance().getCurrentTime()),
			new UserInfo(1, 0, 0, 0, "131762465000003", "Jemmy", "King`s Landing", "g008", TimeManager.getInstance().getCurrentTime()) ,
			new UserInfo(5, 0, 0, 1, "131762465000004", "Imp", "Casterly Rock", "g044", TimeManager.getInstance().getCurrentTime()) ,
			new UserInfo(1, 0, 0, 10, "131762465000005", "John Snow", "Winterfell", "g043", TimeManager.getInstance().getCurrentTime()) };
		return userInfos;
	}
	private MsgItem[] getDummyMsgs(UserInfo userInfos[], UserInfo cuser)
	{
		MsgItem[] msgs = {
				new MsgItem(1, true, false, 0, 100, userInfos[0].uid, "我要退出联盟", "","中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(2, true, true, 0, 0, cuser.uid, "In order to use the Software and related services on www.cok.com, or call the number 13825462145. You must first agree to this License Agreement. android@cok.com.", "", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(3, true, false, 0, 4, userInfos[1].uid, "集合攻打此坐标123:341 123:341 123:341 123:341，5分钟后开始", "", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(4, false, true, 0, 2, cuser.uid, "集合攻打此坐标", "", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(5, true, false, 0, 100, userInfos[2].uid, "我要退出联盟", "集合攻打此坐标", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(6, false, true, 0, 7, cuser.uid, "3|王者之剑", "3|王者之剑", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(7, true, false, 0, 4, userInfos[3].uid, "集合攻打此坐标123:341，5分钟后开始", "范德萨发的", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(8, false, true, 0, 3, cuser.uid, "集合攻打此反倒是坐标200:341，5分钟后开始", "集合攻打此坐标0:341，5分钟后开始", "中文",TimeManager.getInstance().getCurrentTime()) };
		return msgs;
	}

    /**
     * 清除WebView缓存
     */
	public void clearWebViewCache()
	{
		WebView webView = new WebView(this);
		webView.clearCache(true);
		webView.clearHistory();
		CookieSyncManager.createInstance(this);
		CookieSyncManager.getInstance().startSync();
		CookieManager.getInstance().removeSessionCookie();

		// 清理Webview缓存数据库
		try
		{
			deleteDatabase("webview.db");
			deleteDatabase("webviewCache.db");
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}

		// WebView 缓存文件
		File appCacheDir = new File(getFilesDir().getAbsolutePath() + APP_CACAHE_DIRNAME);
		Log.e(TAG, "appCacheDir path=" + appCacheDir.getAbsolutePath());

		File webviewCacheDir = new File(getCacheDir().getAbsolutePath() + "/webviewCache");
		Log.e(TAG, "webviewCacheDir path=" + webviewCacheDir.getAbsolutePath());

		// 删除webview 缓存目录
		if (webviewCacheDir.exists())
		{
			deleteFile(webviewCacheDir);
		}
		// 删除webview 缓存 缓存目录
		if (appCacheDir.exists())
		{
			deleteFile(appCacheDir);
		}
	}

	/**
	 * 递归删除 文件/文件夹
	 *
	 * @param file
	 */
	public void deleteFile(File file)
	{
		Log.i(TAG, "delete file path=" + file.getAbsolutePath());
		if (file.exists())
		{
			if (file.isFile())
			{
				file.delete();
			}
			else if (file.isDirectory())
			{
				File files[] = file.listFiles();
				for (int i = 0; i < files.length; i++)
				{
					deleteFile(files[i]);
				}
			}
			file.delete();
		}
		else
		{
			Log.e(TAG, "delete file no exists " + file.getAbsolutePath());
		}
	}
}
