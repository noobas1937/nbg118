package com.elex.chatservice.model;

import java.io.File;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.db.DBHelper;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.util.LogUtil;

import android.content.ContentValues;
import android.database.Cursor;
import android.graphics.BitmapFactory;
import android.provider.BaseColumns;

public class UserInfo implements Cloneable
{
	// 数据库对应
	public int		_id;
	public int		tableVer;
	public String	uid						= "";		// 形如660641254000001
	public String	userName				= "";
	public String	allianceId				= "";
	public String	asn						= "";
	public int		allianceRank			= -1;		// 决定禁言权限，只有本玩家有
	public int		serverId				= -1;		// 国家号，只有本玩家有
	public int		crossFightSrcServerId	= -2;		// 跨服战时的原服id，若为-1表示没有跨服
	public int		type					= 0;		// 玩家类型，尚未使用；会被C++初始化且不赋初值
	public String	headPic					= "";
	public int		headPicVer				= -1;		// 头像id
	public int		mGmod					= -1;		// gm和mod信息，如果为"2"、"4"、"5"表示为mod，如果为“3”表示为gm，
														// 当为mod时名字颜色为(65,119,41)，如果为gm时名字颜色为(33,111,169),否则名字颜色为(111,26,0)
														// mod为"2"、"4"、"5"时对应的图片分别为mod.png、tmod.png、smod.png，gm为3时对应的图片为gm.png，其他的不显示mod和gm
	public int		vipLevel				= -1;		// vip等级，至少为1，由vip points决定，只升不降
	public int		vipEndTime				= 0;		// vip时间，单位为s，有时区，过期则vip暂时失效（等级保留）
	public int		lastUpdateTime			= 0;		// 上次更新时间
	public int		lastChatTime			= 0;		// 上次聊天时间

	// 运行时
	public boolean	isSelected				= false;
	public boolean	isDummy					= false;

	/**
	 * 用于初始化时创建当前玩家，以及代表国家、联盟的UserInfo
	 */
	public UserInfo()
	{

	}

	public UserInfo(Cursor c)
	{
		try{
			_id = c.getInt(c.getColumnIndex(BaseColumns._ID));
			tableVer = c.getInt(c.getColumnIndex(DBDefinition.COLUMN_TABLE_VER));
			uid = c.getString(c.getColumnIndex(DBDefinition.USER_COLUMN_USER_ID));
			userName = c.getString(c.getColumnIndex(DBDefinition.USER_COLUMN_NICK_NAME));
			allianceId = c.getString(c.getColumnIndex(DBDefinition.USER_COLUMN_ALLIANCE_ID));
			asn = c.getString(c.getColumnIndex(DBDefinition.USER_COLUMN_ALLIANCE_NAME));
			allianceRank = c.getInt(c.getColumnIndex(DBDefinition.USER_COLUMN_ALLIANCE_RANK));
			serverId = c.getInt(c.getColumnIndex(DBDefinition.USER_COLUMN_SERVER_ID));
			crossFightSrcServerId = c.getInt(c.getColumnIndex(DBDefinition.USER_CROSS_FIGHT_SRC_SERVER_ID));
			type = c.getInt(c.getColumnIndex(DBDefinition.USER_COLUMN_TYPE));
			headPic = c.getString(c.getColumnIndex(DBDefinition.USER_COLUMN_HEAD_PIC));
			headPicVer = c.getInt(c.getColumnIndex(DBDefinition.USER_COLUMN_CUSTOM_HEAD_PIC));
			mGmod = c.getInt(c.getColumnIndex(DBDefinition.USER_COLUMN_PRIVILEGE));
			vipLevel = c.getInt(c.getColumnIndex(DBDefinition.USER_COLUMN_VIP_LEVEL));
			vipEndTime = c.getInt(c.getColumnIndex(DBDefinition.USER_COLUMN_VIP_END_TIME));
			lastUpdateTime = c.getInt(c.getColumnIndex(DBDefinition.USER_COLUMN_LAST_UPDATE_TIME));
			lastChatTime = c.getInt(c.getColumnIndex(DBDefinition.USER_COLUMN_LAST_CHAT_TIME));
			initNullField();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	public ContentValues getContentValues()
	{
		ContentValues cv = new ContentValues();
		cv.put(DBDefinition.COLUMN_TABLE_VER, DBHelper.CURRENT_DATABASE_VERSION);
		cv.put(DBDefinition.USER_COLUMN_USER_ID, uid);
		cv.put(DBDefinition.USER_COLUMN_NICK_NAME, userName);
		cv.put(DBDefinition.USER_COLUMN_ALLIANCE_ID, allianceId);
		cv.put(DBDefinition.USER_COLUMN_ALLIANCE_NAME, asn);
		cv.put(DBDefinition.USER_COLUMN_ALLIANCE_RANK, allianceRank);
		cv.put(DBDefinition.USER_COLUMN_SERVER_ID, serverId);
		cv.put(DBDefinition.USER_CROSS_FIGHT_SRC_SERVER_ID, crossFightSrcServerId);
		cv.put(DBDefinition.USER_COLUMN_TYPE, type);
		cv.put(DBDefinition.USER_COLUMN_HEAD_PIC, headPic);
		cv.put(DBDefinition.USER_COLUMN_CUSTOM_HEAD_PIC, headPicVer);
		cv.put(DBDefinition.USER_COLUMN_PRIVILEGE, mGmod);
		cv.put(DBDefinition.USER_COLUMN_VIP_LEVEL, vipLevel);
		cv.put(DBDefinition.USER_COLUMN_VIP_END_TIME, vipEndTime);
		cv.put(DBDefinition.USER_COLUMN_LAST_UPDATE_TIME, lastUpdateTime);
		cv.put(DBDefinition.USER_COLUMN_LAST_CHAT_TIME, lastChatTime);
		return cv;
	}

	/**
	 * 用于wrapper假消息
	 */
    public UserInfo(
	  			int gmod,
	  			int allianceRank,
	  			int headPicVer,
	  			int vipLv,
	  			String uidStr,
	  			String nameStr,
	  			String asnStr,
	  			String headPicStr,
	  			int lastUpdateTime)
	{
    	System.out.println(" >>>>> wrapper假消息");
		this.vipLevel = vipLv;
		this.vipEndTime=TimeManager.getInstance().getCurrentTime() + 60;
		this.userName = nameStr;
		this.headPic = headPicStr;
		this.uid = uidStr;
		this.asn = asnStr;
		this.mGmod = gmod;
		this.allianceRank = allianceRank;
//		System.out.println("UserInfo() allianceId = " + allianceIdStr);
		this.headPicVer=headPicVer;
		this.lastUpdateTime = lastUpdateTime;
	}

	/**
	 * 用于接受到的消息时，在本地找不到用户信息，临时创建dummy UserInfo
	 */
    public UserInfo(String uidStr)
	{
		System.out.println("create dummy user, uid: " + uidStr);
		uid = uidStr;
		headPic = "g026";
		userName = uid;
		isDummy = true;
	}

	/**
	 * 用于聊天室成员选择列表
	 */
//	public UserInfo(String name, String headPic, boolean isSelected)
//	{
//		this.userName = name;
//		this.headPic = headPic;
//		this.isSelected = isSelected;
//	}

	@Override
	public boolean equals(Object o)
	{
		if (o == null || getClass() != o.getClass())
		{
			return false;
		}
		if (o == this)
		{
			return true;
		}

		UserInfo user = (UserInfo) o;
		boolean result;
		result = uid.equals(user.uid) &&
				userName.equals(user.userName) &&
				allianceId.equals(user.allianceId) &&
				asn.equals(user.asn) &&
				allianceRank == user.allianceRank &&
				serverId == user.serverId &&
				crossFightSrcServerId == user.crossFightSrcServerId &&
				type == user.type &&
				headPic.equals(user.headPic) &&
				headPicVer == user.headPicVer &&
				mGmod == user.mGmod &&
				vipLevel == user.vipLevel &&
				vipEndTime == user.vipEndTime &&
				lastUpdateTime == user.lastUpdateTime &&
				lastChatTime == user.lastChatTime;

		if(!result){
			// asn 和 headPic、 headPicVer 可能会不等
//			System.out.println("UserInfo not equals: target = " + o);
//			System.out.println("UserInfo not equals: self   = " + this);
		}
		return result;
	}

	@Override
	public Object clone()
	{
		UserInfo o = null;
		try
		{
			o = (UserInfo) super.clone();
		}
		catch (CloneNotSupportedException e)
		{
			e.printStackTrace();
		}
		return o;
	}

	public String getVipInfo()
	{
        int dtime = vipEndTime - TimeManager.getInstance().getCurrentTime();
        String vipInfo= (vipLevel > 0 && dtime > 0) ? LanguageManager.getLangByKey(LanguageKeys.VIP_INFO, String.valueOf(vipLevel)) : "";
		return vipInfo;
	}
	
	public boolean isCustomHeadImage()
	{
		if(isDummy)
			return false;
		return headPicVer>0 && headPicVer<1000000;
	}

	// 自定义头像网络URL
	public String getCustomHeadPicUrl()
	{
		String result = ChatServiceController.getInstance().host.getCustomHeadPicUrl(uid, headPicVer);
		// System.out.println("getCustomHeadPicUrl(): uid = " + uid + " headPicVer = " + headPicVer + " result = " + result);
		return result;
	}

	// 自定义头像本地路径
	public String getCustomHeadPic()
	{
		String result = ChatServiceController.getInstance().host.getCustomHeadPic(getCustomHeadPicUrl());
		// System.out.println("getCustomHeadPic(): result = " + result);
		return result;
	}

	public int getHeadPicVer() {
		return headPicVer;
	}

	// 自定义头像是否存在
	public boolean isCustomHeadPicExist()
	{
		String path=getCustomHeadPic();
		String fileName=path;
//		System.out.println("isCustomHeadPicExist(): getCustomHeadPic() = " +path);
		File file = new File(fileName);
		if(file.exists()){
			return true;
		}
		
		if(!path.endsWith(".png") && !path.endsWith(".jpg"))
		{
			fileName=path+".png";
			file = new File(fileName);
			if(file.exists()){
				return true;
			}
			else{
				fileName=path+".jpg";
				file = new File(fileName);
				if(file.exists()){
					return true;
				}
			}
		}
		else
		{
			file = new File(path);
			if(file.exists()){
				return true;
			}
		}
		return false;
	}
	
	/**
	 * C++创建的对象可能没有默认值赋值，需要补上
	 */
	public void initNullField()
	{
		if (userName == null)
		{
			userName = "";
		}
		if (allianceId == null)
		{
			allianceId = "";
		}
		if (asn == null)
		{
			asn = "";
		}
		if (headPic == null)
		{
			headPic = "";
		}
	}
	
	/**
	 * 判断是否是dummy user，特征为：
	 * 不是频道user，且privilege（或AllianceRank、ServerId、CustomHeadPic、VipLevel）为-1
	 * dummy user有两种来源：
	 * 1.故意创建的临时dummy user
	 * 2.以前曾将dummy user存入了db
	 */
	public boolean isValid()
	{
		boolean result = isChannelUser() || this.mGmod != -1;
//		if(!result){
//			System.out.println("result is not valid :" + uid);
//		}
		return result;
	}
	
	public boolean isChannelUser()
	{
		return this.uid.contains("@");
	}
}
