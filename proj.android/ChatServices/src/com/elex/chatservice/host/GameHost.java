package com.elex.chatservice.host;

public class GameHost implements IHost
{
	public native void sendMessage(String msg);

	public native void sendMailMsg(String toName, String title, String content, String allianceUid, String uid, boolean isFirstMsg,
			int type, String sendLocalTime, String targetUid);

	public native void requestMoreMail(String fromUid, String uid, int count);

	public native void sendChatMessage(String msg, int type, String sendLocalTime);

	public native void setActionAfterResume(String action, String uid, String name, String attachmentId, boolean returnToChatAfterPopup);

	public native void onResume(int chatType);

	public native void joinAnnounceInvitation(String allianceId);

	/**
	 * 返回一个ChatLanguage数组
	 */
	public native Object[] getChatLangArray();

	/**
	 * 返回一个聊天消息数组
	 */
	public native Object[] getChatInfoArray(int chatInfoNo, String msgType);

	/**
	 * 返回邮件数据数组
	 */
	public native Object[] getMailDataArray(int mailDataIndex);

	/**
	 * 解除屏蔽玩家
	 */
	public native void unShieldPlayer(String uid, String name);

	/**
	 * 屏蔽玩家
	 */
	public native void shieldPlayer(String uid);

	/**
	 * 解除禁言玩家
	 */
	public native void unBanPlayer(String uid);

	/**
	 * 禁言玩家
	 */
	public native void banPlayerByIndex(String uid, int banTimeIndex);

	/**
	 * 解除禁言玩家喇叭消息
	 */
	public native void unBanPlayerNotice(String uid);

	/**
	 * 禁言玩家喇叭消息
	 */
	public native void banPlayerNoticeByIndex(String uid, int banTimeIndex);
	/**
	 * 通知cocos2d-x当前在哪个频道
	 */
	public native void postCurChannel(int channel);

	public native void onBackPressed();

	public native void onTextChanged(String msg);

	public native void set2dxViewHeight(int height, int usableHeightSansKeyboard);

	public native void sendHornMessage(String msg, boolean usePoint, String sendLocalTime);

	public native int isHornEnough();

	public native boolean isCornEnough(int price);

	public native int getHornBanedTime();

	public native Object[] getUserInfoArray(int index);

	/**
	 * 创建群聊
	 */
	public native void createChatRoom(String memberNameStr, String memberUidStr, String roomName, String contents);

	/**
	 * 邀请加入群聊
	 */
	public native void inviteChatRoomMember(String groupId, String memberNameStr, String memberUidStr);

	/**
	 * 将玩家移除群聊
	 */
	public native void kickChatRoomMember(String groupId, String memberNameStr, String memberUidStr);

	/**
	 * 退出群聊
	 */
	public native void quitChatRoom(String groupId);

	/**
	 * 修改群聊名称
	 */
	public native void modifyChatRoomName(String groupId, String name);

	/**
	 * 发送群聊消息
	 */
	public native void sendChatRoomMsg(String groupId, String msg, String sendLocalTime);

	/**
	 * 获取群聊消息记录
	 */
	public native void getChatRoomMsgRecord(String groupId, int start, int count);

	public native String getCustomHeadPicUrl(String uid, int headPicVer);

	public native String getCustomHeadPic(String customHeadPicUrl);

	public native void getMultiUserInfo(String uidsStr);

	public native void getAllianceMember();

	public native void selectChatRoomMember(String roomName, String memberNameStr, String memberUidStr);

	/**
	 * 点击google翻译行云打点
	 */
	public native void callXCApi();

	public native void getMsgBySeqId(int minSeqId, int maxSeqId, int channelType, String channelId);

	public native void searchPlayer(String name);

	/**
	 * 将mail数据传递给C++端
	 */
	public native void transportMailInfo(long mailInfo, boolean isShowDetectMail);

	public native void deleteSingleMail(int tabType, int type, String mailUid, String fromUid);

	public native void deleteMutiMail(String mailUids, String types);

	public native void rewardMutiMail(String mailUids, String types);

	public native void readDialogMail(int type, boolean isModMail, String types);

	public native void testMailCommand();

	public native void getUpdateMail(String time);

	public native void readMail(String mailUid, int type);

	public native void readMutiMail(String mailUids);

	public native void postUnreadMailNum(int unReadCount);

	public native String getNameById(String xmlId);

	public native String getPropById(String xmlId, String proName);

	public native String getPointByIndex(int occupyPointId);

	public native String getPointByMapTypeAndIndex(int occupyPointId, int serverType);

	public native void changeMailListSwitch(boolean isOn);

	public native String getParseNameAndContent(long mailInfo);

	public native String getPicByType(int type, int value);

	public native String getLang(String lang);

	public native String getLang1ByKey(String lang, String key1);

	public native String getLang2ByKey(String lang, String key1, String key2);

	public native String getLang3ByKey(String lang, String key1, String key2, String key3);

	public native void translateMsgByLua(String originMsg, String targetLang);

	public native void readChatMail(String fromUser, boolean isModMail);

	public native boolean canTransalteByLua();

	public native void reportCustomHeadImg(String uid);

	public native void reportPlayerChatContent(String uid, String msg);

	public native void translateOptimize(String method, String originalLang, String userLang, String msg, String translationMsg);

	public native void postDetectMailInfo(String jsonStr);

	public native void changeNickName();

	public native void postDeletedDetectMailInfo(String jsonStr);

	public native void postChangedDetectMailInfo(String jsonStr);

	public native void postNotifyMailPopup();

	public native void showDetectMailFromAndroid(String mailUid);

	public native void getRedPackageStatus(String attachment);

	public native void postFriendLatestMail(String jsonStr);

	public native void completeInitDatabase();
	
	public native void postChatLatestInfo(String chatJson);
}
