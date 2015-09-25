package com.elex.chatservice.host;

import com.elex.chatservice.model.LanguageItem;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.mail.MailData;

public interface IHost {
	public void sendMessage(String msg);
	public void sendMailMsg(String toName,String  title,String content,String allianceUid,String uid,boolean isFirstMsg,int type,String sendLocalTime,String targetUid);
	public void requestMoreMail(String fromUid,String uid,int count);
	public void sendChatMessage(String msg,int type,int sendLocalTime);
	public void requestChatMsg(int type);
	public void setActionAfterResume(String action, String uid, String name, String attachmentId, boolean returnToChatAfterPopup);
	public void onResume(int chatType);
	public void joinAnnounceInvitation(String allianceId);
	// 返回一个ChatLanguage数组  
    public LanguageItem[] getChatLangArray(); 
 // 返回一个聊天消息数组  
    public MsgItem[] getChatInfoArray(int chatInfoNo,String msgType);
    public MailData[] getMailDataArray(int mailDataIndex);

    //解除屏蔽玩家
    public void unShieldPlayer(String uid,String name);
    //屏蔽玩家
    public void shieldPlayer(String uid);
    //解除禁言玩家
    public void unBanPlayer(String uid);
    //禁言玩家
    public void banPlayerByIndex(String uid,int banTimeIndex);
    //通知cocos2d-x当前在哪个频道
    public void postCurChannel(int channel);

    //点击google翻译行云打点
    public void callXCApi();
	public void onBackPressed();
	public void onTextChanged(String msg);
	public void set2dxViewHeight(int height,int usableHeightSansKeyboard);
	
	public void sendHornMessage(String msg,boolean usePoint,int sendLocalTime);
	
	//是否有足够的item，如果足够返回0，否则返回所需金币数
	public int isHornEnough();
	public boolean isCornEnough(int price);
	public int getHornBanedTime();
	
	public UserInfo[] getUserInfoArray(int index);
	//创建群聊
	public void createChatRoom(String memberNameStr,String memberUidStr,String roomName,String contents);
	//邀请加入群聊
	public void inviteChatRoomMember(String groupId,String memberNameStr,String memberUidStr);
	//将玩家移除群聊
	public void kickChatRoomMember(String groupId,String memberNameStr,String memberUidStr);
	//退出群聊
	public void quitChatRoom(String groupId);
	//修改群聊名称
	public void modifyChatRoomName(String groupId,String name);
	//发送群聊消息
	public void sendChatRoomMsg(String groupId,String msg,String sendLocalTime);
	//获取群聊消息记录
	public void getChatRoomMsgRecord(String groupId,int start,int count);

	// 自定义头像网络URL
	public String getCustomHeadPicUrl(String uid, int headPicVer);
	// 自定义头像本地路径
	public String getCustomHeadPic(String customHeadPicUrl);
	public void getMultiUserInfo(String uidsStr);
	public void getAllianceMember();
	public void selectChatRoomMember(String roomName,String memberNameStr,String memberUidStr);
	
	//获取消息记录（聊天和聊天室）
	public void getMsgBySeqId(int minSeqId, int maxSeqId, int channelType, String channelId);
	
	public void searchPlayer(String name);
	
	public void transportMailInfo(long mailInfo);
	public void deleteSingleMail(int tabType,int type,String mailUid,String fromUid);
	public void deleteMutiMail(String mailUids,String types);
	public void readMail(String mailUid,int type);
	public void readChatMail(String fromUser,boolean isModMail);
	
	public void testMailCommand();
	public void getUpdateMail(String time);
	public void postUnreadMailNum(int unReadCount);
	
	public String getNameById(String xmlId);
	public String getPropById(String xmlId,String proName);
	public String getPointByIndex(int occupyPointId);
	public String getPicByType(int type,int value);
	public void changeMailListSwitch(boolean isOn);
	public String getParseNameAndContent(long mailInfo);
	public String getLang(String lang);
	public String getLang1ByKey(String lang,String key1);
	public String getLang2ByKey(String lang,String key1,String key2);
	public String getLang3ByKey(String lang,String key1,String key2,String key3);
	public void translateMsgByLua(String originMsg,String targetLang);
	public boolean canTransalteByLua();
	public void reportCustomHeadImg(String uid);
}
