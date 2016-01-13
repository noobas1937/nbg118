package com.example.sample1;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import org.apache.commons.lang.StringUtils;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.TimeManager;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.model.db.DBHelper;
import com.elex.chatservice.model.db.DBManager;
import com.elex.chatservice.net.IWebSocketStatusListener;
import com.elex.chatservice.net.WebSocketManager;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.util.gif.GifMovieView;
import com.elex.chatservice.util.gif.GifView;
import com.elex.chatservice.view.ChatFragment;
import com.elex.chatservice.view.ForumFragment;

public class WrapperChatServiceMainActivity extends Activity implements IWebSocketStatusListener {
	private static final String	ACTIVITY_TAG	= "WrapperChatServiceMainActivity";
	private Button buttonChannel;
	private Button buttonChat;
	private Button mailChat;
	private Button buttonForum;
	private Button buttonTranslateOptimization;
	private Button buttonMemberSelectorFragment;
	private Button buttonSerialize;
	private Button buttonConnect;
	private EditText statusLabel;
	private Button buttonSendMessage;
	private GifView	gifView;
	private Button	buttonGif;
	private GifMovieView	gifMovieView;
	
	public WrapperChatServiceMainActivity()
	{
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.wrapper_chat_service_main);

		WebSocketManager.getInstance().setStatusListener(this);
		
//		initDummy();
		initRealDB();

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
		
		buttonTranslateOptimization = (Button)findViewById(ResUtil.getId(this, "id", "button7"));
		buttonTranslateOptimization.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	ServiceInterface.showForumActivity(ForumFragment.WEBVIEW_TYPE_TRANSLATION_OPTIMIZATION,WrapperChatServiceMainActivity.this, null);
            }
        });

		buttonMemberSelectorFragment = (Button)findViewById(ResUtil.getId(this, "id", "button4"));
		buttonMemberSelectorFragment.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	showMemberSelectorFragment();
            }
        });
		
		buttonSerialize = (Button)findViewById(ResUtil.getId(this, "id", "button8"));
		buttonSerialize.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	ChannelManager.getInstance().serialize();
            }
        });
		buttonSerialize.setVisibility(View.GONE);
		
		final WrapperChatServiceMainActivity context = this;
		buttonConnect = (Button)findViewById(ResUtil.getId(this, "id", "button9"));
		buttonConnect.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	WebSocketManager.getInstance().connect();
            }
        });
		buttonConnect.setVisibility(View.GONE);
		
		statusLabel = (EditText)findViewById(ResUtil.getId(this, "id", "wsStatusLabel"));
		statusLabel.setKeyListener(null);
		
		buttonSendMessage = (Button)findViewById(ResUtil.getId(this, "id", "button10"));
		buttonSendMessage.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	WebSocketManager.getInstance().sendUserMsg();
            }
        });
		buttonSendMessage.setVisibility(View.GONE);
		
		buttonGif = (Button)findViewById(R.id.button6);
		buttonGif.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	changeGifAli();
            }
        });
		buttonGif.setVisibility(View.GONE);
		
		gifView = (GifView) findViewById(R.id.gifView);
		// 设置Gif图片源
		gifView.setGifImage(R.drawable.gif);
		// 添加监听器
		gifView.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	if(gifView.isPaused()){
            		gifView.showAnimation();
            	}else{
                	gifView.showCover();	
            	}
            }
        });
		// 设置显示的大小，拉伸或者压缩
		// gifView.setShowDimension(300, 300);
		// 设置加载方式：先加载后显示、边加载边显示、只显示第一帧再显示
		gifView.setGifImageType(GifView.GifImageType.COVER);
		gifView.setVisibility(View.GONE);
		
		gifMovieView = (GifMovieView) findViewById(ResUtil.getId(this, "id", "gifMovieView"));
		gifMovieView.setMovieResource(R.drawable.gif);
		
		// 应该是长度
		System.out.println(gifMovieView.getMovie().duration());
		gifMovieView.setVisibility(View.GONE);
		
//		LanguageConfiger.initFromINIInBackground(this);
	}

	public void onGifClick(View v) {
		GifMovieView gif = (GifMovieView) v;
		gif.setPaused(!gif.isPaused());
	}

	private int gifIndex = 100;
	private int gifIndexAli = 1;
	private int gifCountAli = 50;
	private void changeGifAli()
	{
		int id = ResUtil.getId(this, "drawable", "ali" + Integer.toString(gifIndexAli));
//    	gifView.setGifImage(id);
    	gifMovieView.setMovieResource(id);
    	gifIndexAli = gifIndexAli % gifCountAli + 1;
	}
	private void changeGifWechat()
	{
    	gifView.setGifImage(ResUtil.getId(this, "drawable", "wechat" + Integer.toString(gifIndex)));
    	gifIndex++;
    	if(gifIndex >= 200) gifIndex -= 100;
	}
    
    protected void showChannelList() {
		ServiceInterface.showChannelListActivity(WrapperChatServiceMainActivity.this, false, -1, null, false);
	}
	
	protected void showChatFragment() {
		ServiceInterface.showChatActivity(WrapperChatServiceMainActivity.this, DBDefinition.CHANNEL_TYPE_COUNTRY, false);
	}

	protected void showMailFragment() {
		ServiceInterface.showChatActivity(WrapperChatServiceMainActivity.this, DBDefinition.CHANNEL_TYPE_USER, false);
	}

	protected void showMemberSelectorFragment() {
//		ActivityManager activityManager = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
//		System.out.println("getMemoryClass: " + activityManager.getMemoryClass());
//		System.out.println("getLargeMemoryClass: " + activityManager.getLargeMemoryClass());
		ChatServiceController.isCreateChatRoom = true;
		ServiceInterface.showMemberSelectorActivity(WrapperChatServiceMainActivity.this,true);
	}

	protected void showForumFragment() {
		// "http://f.elex.com/forums/4-Announcements"
		ServiceInterface.showForumActivity(ForumFragment.WEBVIEW_TYPE_FORFUM,WrapperChatServiceMainActivity.this, null);
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

	private void initRealDB()
	{
		ChatServiceController.init(this, true);
		initBaseInfo();
		copyDBFile();

//		DBManager.initDatabase(false, false);
		DBManager.getInstance().initDB(this);

		UserInfo me = UserManager.getInstance().getCurrentUser();
		ServiceInterface.setPlayerInfo(me.serverId, TimeManager.getInstance().getCurrentTime(), me.mGmod, me.headPicVer, me.userName, me.uid, me.headPic, me.vipLevel, me.vipEndTime, me.lastUpdateTime, me.crossFightSrcServerId);
		ServiceInterface.setPlayerAllianceInfo(me.asn, me.allianceId, me.allianceRank, true);
		
		LanguageConfiger.initLanguage(this);
		initAllianceMembers(UserManager.getInstance().getCurrentUser().allianceId); //需要语言包才能正确解析
		
		ServiceInterface.handleGetNewMailMsg(ChannelManager.getInstance().getSimulateReturnChannelInfo());
//		ChannelManager.deserialize();
	}

	private void initAllianceMembers(String allianceId)
	{
		if(StringUtils.isNotEmpty(allianceId)){
			ArrayList<UserInfo> members = DBManager.getInstance().getAllianceMembers(allianceId);
			String uidStr = "";
			String lastUpdateTimeStr = "";
			for (int i = 0; i < members.size(); i++)
			{
				if(i > 0){
					uidStr += "_";
					lastUpdateTimeStr += "_";
				}
				uidStr += members.get(i).uid;
				lastUpdateTimeStr += members.get(i).lastUpdateTime;
			}
			ServiceInterface.notifyUserUids(uidStr, lastUpdateTimeStr, UserManager.NOTIFY_USERINFO_TYPE_ALLIANCE);
		}
	}
	
	public final String dummyDBUser = USER_HUCHAO_532;
	public static final String USER_ZY_INNER = "1380131787000001"; //内网包，个人邮件适量，系统邮件900
	public static final String USER_XU_INNER = "1380625871000001";
	public static final String USER_HUCHAO_532 = "909504798000489";
	public static final String USER_MAIL_TEMP = "51568477000000";
	public static final String USER_XUTEST_BETA = "1385363567000001";

	public static final String USER_ZY532_Major = "913608715000047";
	public static final String USER_ZY532_Minor = "1480348070000532";
	
	public static final String USER_MOD = "424596561000011"; //mod，个人邮件很多，系统邮件5
	public static final String USER_MAIL_OVER_500 = "195242040000007"; //个人邮件超过500，系统邮件230
	
	private void copyDBFile() {
	    AssetManager assetManager = getAssets();
	    String sourceFile = "database/" + dummyDBUser + ".db";
        InputStream in = null;
        OutputStream out = null;
        try {
          in = assetManager.open(sourceFile);
          File outFile = new File(DBHelper.getDBFileAbsolutePath(this));
          out = new FileOutputStream(outFile);
          copyFile(in, out);
        } catch(IOException e) {
            Log.e("tag", "Failed to copy asset file: " + sourceFile, e);
        }     
        finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    // NOOP
                }
            }
            if (out != null) {
                try {
                    out.close();
                } catch (IOException e) {
                    // NOOP
                }
            }
        }
	}
	private void copyFile(InputStream in, OutputStream out) throws IOException {
	    byte[] buffer = new byte[1024];
	    int read;
	    while((read = in.read(buffer)) != -1){
	      out.write(buffer, 0, read);
	    }
	}
	
	private void initBaseInfo()
	{
		ServiceInterface.setIsNewMailListEnable(true);
		ConfigManager.enableCustomHeadImg = true;
		UserManager.getInstance().setCurrentUserId(dummyDBUser); // 1380131787000001 379997000002
	}
	
	private void initDummy() {
		ChatServiceController.init(this, true);
		initBaseInfo();
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
//		ServiceInterface.setPlayerAllianceInfo("zhe", "allianceIdX", 2, true);
		LanguageConfiger.initLanguage(this);
		
		initDummyUserAndMsg();
	}

	private void initDummyUserAndMsg()
	{
		UserInfo[] userInfos = getDummyUsers();
		for (int i = 0; i < userInfos.length; i++)
		{
			UserManager.getInstance().addUser(userInfos[i]);
		}
		UserInfo cuser = UserManager.getInstance().getCurrentUser();

		MsgItem[] msgs = getDummyMsgs(userInfos, cuser);
		for (int i = 0; i < msgs.length; i++)
		{
			if(!msgs[i].isRedPackageMessage())
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

//		TimeManager.getInstance().setServerBaseTime(Math.round(System.currentTimeMillis() / 1000));
	}

	private UserInfo[] getDummyUsers()
	{
		UserInfo[] userInfos = {
			new UserInfo(5, 0, 0, 7, "131762465000002", "Ned", "Winterfell", "g045", TimeManager.getInstance().getCurrentTime()),
			new UserInfo(1, 0, 0, 0, "131762465000003", "Jemmy", "King`s Landing", "g008", TimeManager.getInstance().getCurrentTime()) ,
			new UserInfo(5, 0, 0, 1, "131762465000004", "Imp", "Casterly Rock", "g044", TimeManager.getInstance().getCurrentTime()) ,
			new UserInfo(11, 0, 0, 10, "131762465000005", "John Snow", "Winterfell", "g043", TimeManager.getInstance().getCurrentTime()) };
		return userInfos;
	}
	private MsgItem[] getDummyMsgs(UserInfo userInfos[], UserInfo cuser)
	{
		MsgItem[] msgs = {
				new MsgItem(1, true, false, 0, 100, userInfos[0].uid, "我要退出联盟", "","中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(2, true, true, 0, 3, cuser.uid, "In order to use the Software and related services on www.cok.com, or call the number 13825462145. You must first agree to this License Agreement. android@cok.com.", "", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(3, true, false, 0, 12, userInfos[1].uid, "快来拆红包1", "快来拆红包1", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(4, false, true, 0, 12, cuser.uid, "快来拆红包2", "快来拆红包2", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(5, true, false, 0, 12, userInfos[2].uid, "快来拆红包3", "快来拆红包3", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(6, false, true, 0, 7, cuser.uid, "3|134054|105392|[{\"name\":\"sfds|f|t\"}]", "3|王者之剑", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(7, true, true, 0, 12, cuser.uid, "快来拆红包4", "快来拆红包4", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(8, false, false, 0, 6, cuser.uid, "集合攻打此反倒是坐标200:341", "集合攻打此坐标0:341", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(9, true, false, 0, 13, userInfos[3].uid, "5", "8", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(10, false, true, 0, 0, cuser.uid, "6", "9", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(11, true, false, 0, 0, userInfos[3].uid, "dsfdsfewrwerfds", "发生的范德萨范德萨", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(12, false, true, 0, 0, cuser.uid, "玩儿玩儿玩儿", "发生的福尔沃特V大是", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(13, true, false, 0, 0, userInfos[3].uid, "威尔额外热温热温热污染️", "发生的服务而额外任务而", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(14, false, true, 0, 0, cuser.uid, "💪", "9", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(15, true, false, 0, 0, userInfos[3].uid, "😤", "8", "中文",TimeManager.getInstance().getCurrentTime()),
				new MsgItem(16, false, true, 0, 0, cuser.uid, "😳", "9", "中文",TimeManager.getInstance().getCurrentTime()) 
				
//				new MsgItem(1, true, false, 0, 100, userInfos[0].uid, "1", "","中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(2, true, true, 0, 6, cuser.uid, "2", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(3, true, false, 0, 12, userInfos[1].uid, "3", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(4, false, true, 0, 12, cuser.uid, "4", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(5, true, false, 0, 12, userInfos[2].uid, "5", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(6, false, true, 0, 13, cuser.uid, "6", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(7, true, true, 0, 12, cuser.uid, "7", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(8, false, false, 0, 6, cuser.uid, "8", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(9, true, false, 0, 13, userInfos[3].uid, "9", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(10, false, true, 0, 0, cuser.uid, "10", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(11, true, false, 0, 0, userInfos[3].uid, "11", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(12, false, true, 0, 0, cuser.uid, "12", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(13, true, false, 0, 0, userInfos[3].uid, "13️", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(14, false, true, 0, 0, cuser.uid, "14", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(15, true, false, 0, 0, userInfos[3].uid, "15", "", "中文",TimeManager.getInstance().getCurrentTime()),
//				new MsgItem(16, false, true, 0, 0, cuser.uid, "16", "", "中文",TimeManager.getInstance().getCurrentTime())
				};
		msgs[2].sendState = 1;
//		msgs[2].createTime = TimeManager.getInstance().getCurrentTime() - 24*60*60 + 60;
		msgs[3].sendState = 0;
		msgs[4].sendState = 2;
		msgs[6].sendState = 3;
		msgs[2].attachmentId = "fsdfwerwwr_1";
		msgs[3].attachmentId = "32423dsfsrwr_1";
		msgs[4].attachmentId = "34235dsas_1";
		msgs[6].attachmentId = "fsdf324235werwwr_1";
		return msgs;
	}

	private int streamCount = 0;
	private int statusCount = 0;
	@Override
	public void onConsoleOutput(String message)
	{
		refreshStatus(message);
	}

	@Override
	public void onStremInput()
	{
		streamCount++;
		this.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				buttonConnect.setText("连接websocket" + " [" + streamCount + "]");
			}
		});
	}
	
	private void refreshStatus(final String message)
	{
		this.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				statusCount++;
				statusLabel.setText(statusLabel.getText() + (statusLabel.getText().length() > 0 ? "\n" : "") + "[" + statusCount + "] "
						+ message);
				statusLabel.setSelection(statusLabel.getText().length(), statusLabel.getText().length());
			}
		});
	}

	@Override
	public void onStatus(String message)
	{
		ChatFragment.setConnectionStatus(message);
	}
}
