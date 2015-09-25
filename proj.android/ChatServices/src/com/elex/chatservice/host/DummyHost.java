package com.elex.chatservice.host;

import java.util.Timer;
import java.util.TimerTask;

import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.LanguageItem;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.MathUtil;

public class DummyHost implements IHost {

	@Override
	public void sendMessage(String msg) {
		
	}

	@Override
	public void sendMailMsg(String toName, String title, final String content,
			final String allianceUid, final String uid, boolean isFirstMsg, int type, String sendLocalTime,String targetUid) {
//		TimerTask timerTask = new TimerTask() {
//			@Override
//			public void run() {
//				ServiceInterface.handleChatPush(true,true,ChatActivity.currentChatType,0,UserManager.getInstance().getCurrentUser().mGmod,"",ChatServiceController.getTime(ChatServiceController.getCurrentChannelIndex()), uid, UserManager.getInstance().getCurrentUser().userName,"",allianceUid, content,"",UserManager.getInstance().getCurrentUser().headPic,"","",UserManager.getInstance().getCurrentUser().originalLangStr);
//			}
//		};
//		scheduleTimer(timerTask);
	}

	@Override
	public void sendChatMessage(final String msg, int type,int sendLocalTime) {
//		TimerTask timerTask = new TimerTask() {
//			@Override
//			public void run() {
//				ServiceInterface.handleChatPush(true,true,ChatActivity.currentChatType,0,UserManager.getInstance().getCurrentUser().mGmod,"",ChatServiceController.getTime(ChatServiceController.getCurrentChannelIndex()), UserManager.getInstance().getCurrentUser().uid, UserManager.getInstance().getCurrentUser().userName,"",UserManager.getInstance().getCurrentUser().allianceId, msg,"",UserManager.getInstance().getCurrentUser().headPic,"","",UserManager.getInstance().getCurrentUser().originalLangStr);
//			}
//		};
//		scheduleTimer(timerTask);
	}
	
    private void scheduleTimer(TimerTask timerTask){
    	Timer timer = new Timer();
		timer.schedule(timerTask, MathUtil.randomPow(10, 5000, 3));
    }

	@Override
	public void requestChatMsg(int type) {
		
	}

	@Override
	public void requestMoreMail(String fromUid, String uid, int count) {
		
	}
	
	public String actionAfterResume = null;
	
	@Override
	public void setActionAfterResume(String action, String uid, String name,
			String attachmentId,
			boolean returnToChatAfterPopup) {
		actionAfterResume = action;
		ServiceInterface.flyHint("", "", "选中命令"+actionAfterResume, 0, 0, false);
	}

	@Override
	public void onResume(int chatType) {
		
	}

	@Override
	public void joinAnnounceInvitation(String allianceId) {
		ServiceInterface.onJoinAnnounceInvitationSuccess();
	}

	@Override
	public LanguageItem[] getChatLangArray() {
		return null;
	}

	@Override
	public void unShieldPlayer(String uid, String name) {
		
	}

	@Override
	public void shieldPlayer(String uid) {
		
	}

	@Override
	public void unBanPlayer(String uid) {
		
	}

	@Override
	public void banPlayerByIndex(String uid, int banTimeIndex) {
		
	}

	@Override
	public void onBackPressed() {
		
	}

	@Override
	public void onTextChanged(String msg) {
		
	}

	@Override
	public void set2dxViewHeight(int height, int usableHeightSansKeyboard) {
		
	}

	@Override
	public MsgItem[] getChatInfoArray(int chatInfoNo, String msgType) {
		return null;
	}

	@Override
	public void sendHornMessage(String msg,boolean usePoint,int sendLocalTime) {
		
	}

	@Override
	public int isHornEnough() {
		return 500;
	}

	@Override
	public boolean isCornEnough(int price) {
		return true;
	}

	@Override
	public int getHornBanedTime() {
		return 100;
	}

	@Override
	public void inviteChatRoomMember(String groupId, String memberNameStr,
			String memberUidStr) {
		
	}

	@Override
	public void kickChatRoomMember(String groupId, String memberNameStr,
			String memberUidStr) {
		
	}

	@Override
	public void quitChatRoom(String groupId) {
		
	}

	@Override
	public void modifyChatRoomName(String groupId, String name) {
		
	}

	@Override
	public void sendChatRoomMsg(String groupId, String msg, String sendLocalTime) {
		
	}

	@Override
	public void getChatRoomMsgRecord(String groupId, int start, int count) {
		
	}

	@Override
	public UserInfo[] getUserInfoArray(int index) {
		return null;
	}

	@Override
	public String getCustomHeadPicUrl(String uid, int headPicVer)
	{
		return null;
	}

	@Override
	public String getCustomHeadPic(String customHeadPicUrl)
	{
		return null;
	}

	@Override
	public void getMultiUserInfo(String uidsStr)
	{
	}

	@Override
	public void getAllianceMember() {

	}

	@Override
	public void postCurChannel(int channel) {
		// TODO Auto-generated method stub
	}

	@Override
	public void selectChatRoomMember(String roomName, String memberNameStr,
			String memberUidStr) {
	}

	@Override
	public void callXCApi() {
	}

	@Override
	public void createChatRoom(String memberNameStr,String memberUidStr,String roomName,String contents) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void getMsgBySeqId(int minSeqId, int maxSeqId, int channelType,
			String channelId) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void searchPlayer(String name) {
		// TODO Auto-generated method stub
		System.out.println("searchPlayer: "+name);
	}

	@Override
	public void transportMailInfo(long mailInfo) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public MailData[] getMailDataArray(int mailDataIndex) {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public void deleteSingleMail(int tabType, int type, String mailUid, String fromUid)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void deleteMutiMail(String mailUids, String types)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void testMailCommand()
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void getUpdateMail(String time)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void readMail(String mailUid,int type)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void postUnreadMailNum(int unReadCount)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public String getNameById(String xmlId)
	{
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getPropById(String xmlId, String proName)
	{
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getPointByIndex(int occupyPointId)
	{
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void changeMailListSwitch(boolean isOn)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public String getParseNameAndContent(long mailInfo)
	{
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getPicByType(int type, int value)
	{
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getLang(String key)
	{
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getLang1ByKey(String lang, String key1)
	{
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getLang2ByKey(String lang, String key1, String key2)
	{
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void translateMsgByLua(String originMsg, String targetLang)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void readChatMail(String fromUser, boolean isModMail)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public boolean canTransalteByLua()
	{
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void reportCustomHeadImg(String uid)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public String getLang3ByKey(String lang, String key1, String key2, String key3)
	{
		// TODO Auto-generated method stub
		return null;
	}
}
