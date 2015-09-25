package com.elex.chatservice.model.db;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import org.apache.commons.lang.StringUtils;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.model.ChannelManager;
import com.elex.chatservice.model.ChatChannel;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.LogUtil;

public class DBManager
{
	// id有标准column名：BaseColumns._ID
	public static final String	TABEL_PERSON	= "person";

	private DBHelper			helper;
	private SQLiteDatabase		db;

	private static DBManager	instance;

	public static DBManager getInstance()
	{
		if(instance == null){
//			System.out.println("DBManager getInstance(): null");
			instance = new DBManager();
		}
		return instance;
	}

	/**
	 * 因为getWritableDatabase内部调用了mContext.openOrCreateDatabase(mName, 0, mFactory);
	 * 所以要确保context已初始化，可以把实例化DBManager的步骤放在Activity的onCreate里。
	 *
	 * @param context
	 */
	private DBManager()
	{
	}
	
	public void initDB(Context context)
	{
//		return;
		System.out.println("DBManager.initDB(): context = " + context);
		// 可能有异常：android.database.sqlite.SQLiteException: not an error (code 0): Could not open the database in read/write mode
		try
		{
			helper = new DBHelper(context);
			// 触发getDatabaseLocked，再调用onCreate, onUpgrade and/or onOpen
			db = helper.getWritableDatabase();
			isInited = true;
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	/**
	 * close database
	 */
	public void closeDB()
	{
		if (!isDBAvailable()) return;
		
		try{
			if(helper != null){
				helper.close();	
				helper = null;
			}
			if(db != null){
				if(db.isOpen()) db.close();	
				db = null;
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	private static boolean isInited = false;
	private static boolean tracked = false;
	public boolean isDBAvailable()
	{
		if(!isInited) return false;
		
		boolean result = db != null && db.isOpen();
		if(!result && !tracked){
			if(db == null){
				LogUtil.trackMessage("database is unavailable (db is null)", "", "");
			}else{
				LogUtil.trackMessage("database is unavailable (db is not open)", "", "");
			}
			tracked = true;
		}
		return result;
	}

	/**
	 * @return 可能为null
	 */
	public UserInfo getUser(String userID)
	{
//		System.out.println("DBManager getUser(): uid = " + userID);
		if (StringUtils.isEmpty(userID) || !isDBAvailable()) return null;

		UserInfo result = null;
		Cursor c = null;
		try
		{
			String sql = String.format(	Locale.US, "SELECT * FROM %s WHERE %s = '%s'",
										DBDefinition.TABEL_USER,
										DBDefinition.USER_COLUMN_USER_ID,
										userID);
			c = db.rawQuery(sql, null);
			if (!c.moveToFirst())
			{
				return null;
			}
			result = new UserInfo(c);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}
		return result;
	}

	public void insertUser(UserInfo user)
	{
//		System.out.println("DBManager insertUser(): uid = " + user.uid + " name = " + user.userName);
		if (user == null || !isDBAvailable()) return;

		try
		{
			db.insert(DBDefinition.TABEL_USER, null, user.getContentValues());
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	/**
	 * 存在则update，否则插入
	 */
	public void updateUser(UserInfo user)
	{
//		System.out.println("DBManager updateUser(): uid = " + user.uid + " name = " + user.userName);
		if (user == null || !isDBAvailable()) return;

		try
		{
			if(getUser(user.uid) != null){
				String where = String.format(Locale.US, "%s = '%s'", DBDefinition.USER_COLUMN_USER_ID, user.uid);
				db.update(DBDefinition.TABEL_USER, user.getContentValues(), where, null);
			}else{
				insertUser(user);
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	public void updateMyMessageStatus(MsgItem msg, ChatTable chatTable)
	{
//		System.out.println("DBManager updateMessage():" + " channelID = " + chatTable.channelID + " table = "
//				+ chatTable.getTableName() + " " + msg);
		if (msg == null || StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return;

		try
		{
			String where = String.format(Locale.US, "%s = %s AND %s = '%s'", 
			                             DBDefinition.CHAT_COLUMN_LOCAL_SEND_TIME, msg.sendLocalTime,
			                             DBDefinition.CHAT_COLUMN_USER_ID, msg.uid);
			db.update(chatTable.getTableNameAndCreate(), msg.getContentValues(), where, null);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	public void updateMessage(MsgItem msg, ChatTable chatTable)
	{
//		System.out.println("DBManager updateMessage():" + " channelID = " + chatTable.channelID + " table = "
//				+ chatTable.getTableName() + " " + msg);
		if (msg == null || StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return;

		try
		{
			String where = "";
			if (msg.channelType != DBDefinition.CHANNEL_TYPE_USER)
			{
				where = String.format(Locale.US, "%s = %s AND %s = '%s'", DBDefinition.CHAT_COLUMN_SEQUENCE_ID, msg.sequenceId,
						DBDefinition.CHAT_COLUMN_USER_ID, msg.uid);
			}
			else
			{
				where = String.format(Locale.US, "%s = '%s' AND %s = '%s'", DBDefinition.CHAT_COLUMN_MAIL_ID, msg.mailId,
						DBDefinition.CHAT_COLUMN_USER_ID, msg.uid);
			}

			db.update(chatTable.getTableNameAndCreate(), msg.getContentValues(), where, null);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	public void insertMessages(ArrayList<MsgItem> msgs, ChatTable chatTable)
	{
		insertMessages((MsgItem[]) msgs.toArray(new MsgItem[0]), chatTable);
	}
	
	public void insertMailData(MailData mailData, ChatChannel channel)
	{
		mailData.channelId = mailData.getChannelId();
		System.out.println("DBManager insertMailData(): mailData type = " +mailData.getType() + "channelID:"+channel.channelID);
		if (!isDBAvailable()) return;
		
		try
		{
			// 极少量 Fatal Exception: android.database.sqlite.SQLiteDiskIOException disk I/O error (code 1034)
			db.beginTransaction();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
			return;
		}
		
		try
		{
			try{
				if (!isMailDataExists(mailData.getUid()))
				{
//					System.out.println("DBManager insertMessages(): actual insert");
					db.insert(DBDefinition.TABEL_MAIL, DBDefinition.MAIL_ID, mailData.getContentValues());
					checkChannel(mailData, channel);
				}
			}
			catch (Exception e)
			{
				LogUtil.trackMessage("mailData is not Valid", "uid=" + mailData.getUid(), "");
				LogUtil.printException(e);
			}
			db.setTransactionSuccessful();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			try
			{
				// 可能出异常：cannot commit - no transaction is active (code 1)
				db.endTransaction();
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
	}
	
	public void updateMail(MailData mailData,ChatChannel channel)
	{
//		System.out.println("DBManager updateMessage():" + " channelID = " + chatTable.channelID + " table = "
//				+ chatTable.getTableName() + " " + msg);
		if (mailData == null || !isDBAvailable()) return;

		try
		{
			String where = String.format(Locale.US, "%s = '%s'", DBDefinition.MAIL_ID, mailData.getUid());

			db.update(DBDefinition.TABEL_MAIL, mailData.getContentValues(), where, null);
			checkChannel(mailData, channel);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}
	
	public void updateMail(MailData mailData)
	{
//		System.out.println("DBManager updateMessage():" + " channelID = " + chatTable.channelID + " table = "
//				+ chatTable.getTableName() + " " + msg);
		if (mailData == null || !isDBAvailable()) return;

		try
		{
			String where = String.format(Locale.US, "%s = '%s'", DBDefinition.MAIL_ID, mailData.getUid());

			db.update(DBDefinition.TABEL_MAIL, mailData.getContentValues(), where, null);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}
	
	/**
	 * 邮件现在有表，但没有插入，应该是因为seqId不符合约束条件
	 */
	public void insertMessages(MsgItem[] msgs, ChatTable chatTable)
	{
		System.out.println("DBManager insertMessages(): length = " + msgs.length + " channelID = " + chatTable.channelID
				+ " table = " + chatTable.getTableNameAndCreate());
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return;

		String tableName = chatTable.getTableNameAndCreate();

		try
		{
			// 极少量 Fatal Exception: android.database.sqlite.SQLiteDiskIOException disk I/O error (code 1034)
			db.beginTransaction();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
			return;
		}
		
		try
		{
			for (MsgItem msg : msgs)
			{
//				System.out.println("insertMessages msg:"+msg+" mailId:"+msg.mailId+" chatTable.channelType:"+chatTable.channelType);
				try{
					if ((chatTable.channelType != DBDefinition.CHANNEL_TYPE_USER && !isMsgExists(chatTable, msg.sequenceId)) || 
							(chatTable.channelType == DBDefinition.CHANNEL_TYPE_USER && !isUserMailExists(chatTable, msg.mailId)))
					{
//						System.out.println("DBManager insertMessages(): actual insert to " + tableName + " " + LogUtil.typeToString(msg));
						db.insert(tableName, null, msg.getContentValues());
						checkChannel(msg, chatTable);
					}
				}
				catch (Exception e)
				{
					LogUtil.trackMessage("MsgItem is not Valid", "sequenceId=" + msg.sequenceId + " channelType="
							+ msg.channelType, "");
					LogUtil.printException(e);
				}
			}
			db.setTransactionSuccessful();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			try
			{
				// 可能出异常：cannot commit - no transaction is active (code 1)
				db.endTransaction();
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
	}
	
	private void checkChannel(MsgItem msg, ChatTable chatTable)
	{
		ChatChannel channel = ChannelManager.getInstance().getChannel(chatTable);
		if(channel == null) return;
		
		if(msg.isNewMsg && !msg.isSelfMsg && !ChatServiceController.isInTheSameChannel(chatTable.channelID))
		{
			channel.unreadCount++;
			updateChannel(channel);
		}
		
		if(channel.channelType != DBDefinition.CHANNEL_TYPE_USER)
		{
//			System.out.println("msg sequenceId:"+msg.sequenceId);
			if(msg.sequenceId <= 0) return;
			
			if(channel.dbMinSeqId == -1){
				channel.dbMinSeqId = msg.sequenceId;
			}
			if(channel.dbMaxSeqId == -1){
				channel.dbMaxSeqId = msg.sequenceId;
			}
			if(msg.sequenceId < channel.dbMinSeqId)
			{
				channel.dbMinSeqId = msg.sequenceId;
			}
			if(msg.sequenceId > channel.dbMaxSeqId)
			{
				channel.dbMaxSeqId = msg.sequenceId;
			}
			
			if(msg.createTime > channel.latestTime || (msg.createTime == channel.latestTime && StringUtils.isNotEmpty(channel.latestId) &&
					msg.sequenceId > Integer.parseInt(channel.latestId)))
			{
				channel.latestTime = msg.createTime;
				channel.latestId = ""+msg.sequenceId;
			}
			updateChannel(channel);
		}
		else
		{
			if(msg.createTime > channel.latestTime)
			{
				channel.latestTime = msg.createTime;
				channel.latestId=msg.mailId;
				updateChannel(channel);
			}
		}
	}
	
	private void checkChannel(MailData mailData, ChatChannel channel)
	{
		if(channel==null)
			return;
		
		if (mailData.isUnread() && !ChatServiceController.isInTheSameChannel(mailData.getChannelId()))
		{
			channel.unreadCount++;
		}
		
		if(mailData.getCreateTime() > channel.latestTime)
		{
			channel.latestTime = mailData.getCreateTime();
			channel.latestId=mailData.getUid();
		}
		updateChannel(channel);
	}

	public void updateChannel(ChatChannel channel)
	{
//		System.out.println("DBManager updateChannel(): " + channel.getChatTable().getChannelName());
		if (StringUtils.isEmpty(channel.channelID) || !isDBAvailable()) return;

		try
		{
			if(!isChannelExists(channel.channelID))
				insertChannel(channel);
			String where = String.format(Locale.US, "%s = '%s'", DBDefinition.CHANNEL_CHANNEL_ID, channel.channelID);
			db.update(DBDefinition.TABEL_CHANNEL, channel.getContentValues(), where, null);
//			channel.refreshRenderData();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}
	
	public ArrayList<ChatChannel> getAllChannel()
	{
//		System.out.println("DBManager getAllChannel()");
		ArrayList<ChatChannel> result = new ArrayList<ChatChannel>();
		
		if (!isDBAvailable() || !isTableExists(DBDefinition.TABEL_CHANNEL)) return result;
		
		Cursor c = null;
		try
		{
			String sql = String.format(Locale.US, "SELECT * FROM %s", DBDefinition.TABEL_CHANNEL);
			c = db.rawQuery(sql, null);
			if(c!=null)
			{
				while (c.moveToNext())
				{
					ChatChannel channel = new ChatChannel(c);
					if(channel!=null && !channel.channelID.equals(""))
						result.add(channel);
				}
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return result;
	}

	public ChatChannel getChannel(ChatTable chatTable)
	{
//		System.out.println("DBManager getChannel(): " + chatTable.getChannelName());
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return null;
		
		ChatChannel result = null;
		Cursor c = null;
		try
		{
			String sql = String.format(Locale.US, "SELECT * FROM %s WHERE %s = '%s'",
										DBDefinition.TABEL_CHANNEL,
										DBDefinition.CHANNEL_CHANNEL_ID,
										chatTable.channelID);
			c = db.rawQuery(sql, null);

			if (!c.moveToFirst())
			{
				return null;
			}
			
			result = new ChatChannel(c);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return result;
	}
	
	public void deleteChannel(ChatTable chatTable)
	{
		System.out.println("DBManager deleteChannel(): channelID = " + chatTable.channelID);
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return;

		db.beginTransaction();
		try
		{
			if(chatTable.channelType != DBDefinition.CHANNEL_TYPE_CHATROOM){
				String where = String.format(Locale.US, "%s = %d AND %s = '%s'", 
											DBDefinition.CHANNEL_TYPE, chatTable.channelType,
											DBDefinition.CHANNEL_CHANNEL_ID, chatTable.channelID);
				db.delete(DBDefinition.TABEL_CHANNEL, where, null);
			}
			
			db.execSQL("DROP TABLE IF EXISTS '" + chatTable.getTableName() + "'");
			
			db.setTransactionSuccessful();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			try
			{
				db.endTransaction();
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
	}
	
	public void deleteSysMailChannel(ChatTable chatTable)
	{
		System.out.println("DBManager deleteChannel(): channelID = " + chatTable.channelID);
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return;

		db.beginTransaction();
		try
		{
			String where = String.format(Locale.US, "%s = %d AND %s = '%s'", 
										DBDefinition.CHANNEL_TYPE, chatTable.channelType,
										DBDefinition.CHANNEL_CHANNEL_ID, chatTable.channelID);
			db.delete(DBDefinition.TABEL_CHANNEL, where, null);
			
			String where2 = String.format(Locale.US, "%s = '%s' AND %s != %d AND %s != %d", 
										DBDefinition.MAIL_CHANNEL_ID, chatTable.channelID,
										DBDefinition.MAIL_REWARD_STATUS, 0,
										DBDefinition.MAIL_SAVE_FLAG, 1);
			db.delete(DBDefinition.TABEL_MAIL, where2, null);
			
			db.setTransactionSuccessful();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			try
			{
				// 可能出异常：Fatal Exception: android.database.sqlite.SQLiteFullException database or disk is full (code 13)
				db.endTransaction();
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
	}
	
	//只对DB进行操作
	public void deleteSysMailFromDB(String mailId)
	{
		System.out.println("DBManager deleteSysMailFromDB(): mailId = " + mailId);
		if (StringUtils.isEmpty(mailId) || !isDBAvailable()) return;
		db.beginTransaction();
		try
		{
			String where = String.format(Locale.US, "%s = '%s'", 
					DBDefinition.MAIL_ID, mailId);
			db.delete(DBDefinition.TABEL_MAIL, where, null);
			
			db.setTransactionSuccessful();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			try
			{
				db.endTransaction();
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
	}
	
	public void deleteSysMail(ChatChannel channel, String mailId)
	{
		System.out.println("DBManager deleteSysMail(): mailId = " + mailId);
		if (StringUtils.isEmpty(mailId) || !isDBAvailable()) return;

		db.beginTransaction();
		try
		{
			String where2 = String.format(Locale.US, "%s = '%s'", 
					DBDefinition.MAIL_ID, mailId);
			db.delete(DBDefinition.TABEL_MAIL, where2, null);
			
			if(channel.latestId.equals(mailId) && channel.mailDataList!=null && channel.mailDataList.size()>0)
			{
				String latestMailId="";
				int latestMailCreateTime=0;
				for(int i=0;i<channel.mailDataList.size();i++)
				{
					MailData mailData=channel.mailDataList.get(i);
					if(mailData.getCreateTime()>latestMailCreateTime)
					{
						latestMailCreateTime=mailData.getCreateTime();
						latestMailId=mailData.getUid();
					}
				}
				if(!latestMailId.equals("") && latestMailCreateTime!=0)
				{
					channel.latestId=latestMailId;
					channel.latestTime=latestMailCreateTime;
					String where = String.format(Locale.US, "%s = '%s'", DBDefinition.CHANNEL_CHANNEL_ID, channel.channelID);
					db.update(DBDefinition.TABEL_CHANNEL, channel.getContentValues(), where, null);
					channel.refreshRenderData();
				}
			}
			
			db.setTransactionSuccessful();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			try
			{
				db.endTransaction();
			}
			catch (Exception e)
			{
				LogUtil.printException(e);
			}
		}
	}

	public boolean isMsgExists(ChatTable chatTable, int seqId)
	{
//		System.out.println("DBManager isMsgExists(): " + tableName);
		if (chatTable == null || StringUtils.isEmpty(chatTable.getTableNameAndCreate()) || !isDBAvailable()) return false;

		int count = 0;
		Cursor c = null;
		try
		{
			String sql = String.format(	Locale.US, "SELECT * FROM %s WHERE %s = %d",
			                           	chatTable.getTableNameAndCreate(),
										DBDefinition.CHAT_COLUMN_SEQUENCE_ID,
										seqId);
			c = db.rawQuery(sql, null);
			if (!c.moveToFirst())
			{
				return false;
			}
			count = c.getInt(0);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return count > 0;
	}
	
	
	
	public boolean isChannelExists(String channelID)
	{
		if (StringUtils.isEmpty(channelID) || !isDBAvailable()) return false;

		int count = 0;
		Cursor c = null;
		try
		{
			String sql = String.format(	Locale.US, "SELECT * FROM %s WHERE %s = '%s'",
			                           	DBDefinition.TABEL_CHANNEL,
										DBDefinition.CHANNEL_CHANNEL_ID,
										channelID);
			c = db.rawQuery(sql, null);
			if (!c.moveToFirst())
			{
				return false;
			}
			count = c.getInt(0);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return count > 0;
	}
	
	public boolean isUserMailExists(ChatTable chatTable, String mailId)
	{
//		System.out.println("DBManager isMsgExists(): " + tableName);
		if (chatTable == null || StringUtils.isEmpty(chatTable.getTableNameAndCreate()) || !isDBAvailable()) return false;

		int count = 0;
		Cursor c = null;
		try
		{
			String sql = String.format(	Locale.US, "SELECT * FROM %s WHERE %s = '%s'",
			                           	chatTable.getTableNameAndCreate(),
										DBDefinition.CHAT_COLUMN_MAIL_ID,
										mailId);
			c = db.rawQuery(sql, null);
			if (!c.moveToFirst())
			{
				return false;
			}
			count = c.getInt(0);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return count > 0;
	}
	
	public boolean isMailDataExists(String mailId)
	{
//		System.out.println("DBManager isMsgExists(): " + tableName);
		if (!isDBAvailable()) return false;

		int count = 0;
		Cursor c = null;
		try
		{
			String sql = String.format(	Locale.US, "SELECT * FROM %s WHERE %s = '%s'",
										DBDefinition.TABEL_MAIL,
										DBDefinition.MAIL_ID,
										mailId);
			c = db.rawQuery(sql, null);
			if (!c.moveToFirst())
			{
				return false;
			}
			count = c.getInt(0);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return count > 0;
	}
	
	public String getSystemMailLatestId()
	{
		if (!isDBAvailable()) 
			return "";
		String latestId = "";
		Cursor c = null;
		try{
			//不一定只有一条，可加"LIMIT 1" "ORDER BY %s DESC LIMIT 1"限制条数
			//可限制UNIQUE ON CONFLICT IGNORE
			String sql = String.format(Locale.US, "SELECT * FROM %s WHERE %s = (SELECT %s(%s) FROM %s)",
					DBDefinition.TABEL_MAIL,
					DBDefinition.MAIL_CREATE_TIME,
					"max",
					DBDefinition.MAIL_CREATE_TIME,
					DBDefinition.TABEL_MAIL);
			c = db.rawQuery(sql, null);
			if (c == null || (c!=null && !c.moveToFirst()))
			{
				return latestId;
			}
			latestId = c.getString(c.getColumnIndex(DBDefinition.MAIL_ID));
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}
		return latestId;
	}
	
	public long getSystemMailLatestModifyTime()
	{
		if (!isDBAvailable()) 
			return -1;
		long latestModifyTime = -1;
		Cursor c = null;
		try{
			String sql = String.format(Locale.US, "SELECT * FROM %s WHERE %s = (SELECT %s(%s) FROM %s)",
					DBDefinition.TABEL_CHANNEL,
					DBDefinition.CHANNEL_LATEST_MODIFY_TIME,
					"max",
					DBDefinition.CHANNEL_LATEST_MODIFY_TIME,
					DBDefinition.TABEL_CHANNEL);
			c = db.rawQuery(sql, null);
			if (c == null || (c!=null && !c.moveToFirst()))
			{
				return latestModifyTime;
			}
			latestModifyTime = c.getLong(c.getColumnIndex(DBDefinition.CHANNEL_LATEST_MODIFY_TIME));
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}
		return latestModifyTime;
	}
	
	public int getChatLatestTime(String chatTableName)
	{
		if (!isDBAvailable()) 
			return 0;
		if(StringUtils.isEmpty(chatTableName))
			return 0;
		int latestTime = 0;
		Cursor c = null;
		try{
			String sql = String.format(Locale.US, "SELECT max(%s),%s FROM %s",
					DBDefinition.CHAT_COLUMN_CREATE_TIME,
					DBDefinition.CHAT_COLUMN_CREATE_TIME,
					chatTableName);
			c = db.rawQuery(sql, null);
			if (c == null || (c!=null && !c.moveToFirst()))
			{
				return latestTime;
			}
			if(c.getColumnIndex(DBDefinition.CHAT_COLUMN_CREATE_TIME)>=0)
				latestTime = c.getInt(c.getColumnIndex(DBDefinition.CHAT_COLUMN_CREATE_TIME));
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}
		return latestTime;
	}
	
	public int getSysMailChannelLatestTime(String channelId)
	{
		if (!isDBAvailable()) 
			return 0;
		int latestTime = 0;
		Cursor c = null;
		try{
			String where = getSqlByChannelId(channelId);
			if(StringUtils.isEmpty(where))
				return 0;
			String sql = String.format(Locale.US, "SELECT max(%s),%s FROM %s",
					DBDefinition.MAIL_CREATE_TIME,
					DBDefinition.MAIL_CREATE_TIME,
					DBDefinition.TABEL_MAIL,
					where);
			c = db.rawQuery(sql, null);
			if (c == null || (c!=null && !c.moveToFirst()))
			{
				return latestTime;
			}
			if(c.getColumnIndex(DBDefinition.MAIL_CREATE_TIME)>=0)
				latestTime = c.getInt(c.getColumnIndex(DBDefinition.MAIL_CREATE_TIME));
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}
		return latestTime;
	}
	
	public String getSysMailChannelLatestId(String channelId)
	{
		if (!isDBAvailable()) 
			return "";
		String latestId = "";
		Cursor c = null;
		try{
			String where = getSqlByChannelId(channelId);
			if(StringUtils.isEmpty(where))
				return "";
			
			String sql = String.format(Locale.US, "SELECT max(%s),%s FROM %s %s",
					DBDefinition.MAIL_CREATE_TIME,
					DBDefinition.MAIL_ID,
					DBDefinition.TABEL_MAIL,
					where);
			c = db.rawQuery(sql, null);
			if (c == null || (c!=null && !c.moveToFirst()))
			{
				return latestId;
			}
			latestId = c.getString(c.getColumnIndex(DBDefinition.MAIL_ID));
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}
		return latestId;
	} 
	
	public String getLatestId(String chatTable)
	{
		if (StringUtils.isEmpty(chatTable) || !isDBAvailable()) return "";
		
		String latestId = "";
		Cursor c = null;
		try{
			//不一定只有一条，可加"LIMIT 1" "ORDER BY %s DESC LIMIT 1"限制条数
			//可限制UNIQUE ON CONFLICT IGNORE
			String sql = String.format(Locale.US, "SELECT * FROM %s WHERE %s = (SELECT %s(%s) FROM %s)",
			                           	chatTable,
										DBDefinition.MAIL_CREATE_TIME,
										"max",
										DBDefinition.MAIL_CREATE_TIME,
										chatTable);
			c = db.rawQuery(sql, null);
			if (!c.moveToFirst())
			{
				return latestId;
			}
			latestId = c.getString(c.getColumnIndex(DBDefinition.MAIL_ID));
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}
		return latestId;
	}
	
	public int getMarginalSequenceNumber(String chatTable, boolean isUpper)
	{
		if (StringUtils.isEmpty(chatTable) || !isDBAvailable()) return 0;
		
		int sequenceNumber = 0;
		Cursor c = null;
		try{
			//不一定只有一条，可加"LIMIT 1" "ORDER BY %s DESC LIMIT 1"限制条数
			//可限制UNIQUE ON CONFLICT IGNORE
			String sql = String.format(Locale.US, "SELECT * FROM %s WHERE %s = (SELECT %s(%s) FROM %s)",
			                           	chatTable,
										DBDefinition.CHAT_COLUMN_SEQUENCE_ID,
										isUpper ? "max" : "min",
										DBDefinition.CHAT_COLUMN_SEQUENCE_ID,
										chatTable);
			c = db.rawQuery(sql, null);
			if (!c.moveToFirst())
			{
				return sequenceNumber;
			}
			sequenceNumber = c.getInt(c.getColumnIndex(DBDefinition.CHAT_COLUMN_SEQUENCE_ID));
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}
		return sequenceNumber;
	}
	
	public MailData getSysMailByID(String mailID)
	{
		MailData mailData = null;
		if (StringUtils.isEmpty(mailID) || !isDBAvailable()) return mailData;

		Cursor c = null;
		try
		{
			String sql = String.format(	Locale.US, "SELECT * FROM %s WHERE %s = '%s'",
			                           	DBDefinition.TABEL_MAIL,
										DBDefinition.MAIL_ID,
										mailID);
			c = db.rawQuery(sql, null);
			if (!c.moveToFirst())
			{
				return mailData;
			}else{
				mailData = new MailData(c);
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return mailData;
	}

	public MsgItem getUserMailByID(ChatTable chatTable, String mailID)
	{
		MsgItem msg = null;
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return msg;

		Cursor c = null;
		try
		{
			String sql = String.format(	Locale.US, "SELECT * FROM %s WHERE %s = '%s'",
			                           	chatTable.getTableNameAndCreate(),
										DBDefinition.CHAT_COLUMN_MAIL_ID,
										mailID);
			c = db.rawQuery(sql, null);
			if (!c.moveToFirst())
			{
				return msg;
			}else{
				msg = new MsgItem(c);
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return msg;
	}

	public boolean isUserMailExistByType(ChatTable chatTable, int type)
	{
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return false;

		boolean result = false;
		Cursor c = null;
		try
		{
			String sql = String.format(	Locale.US, "SELECT * FROM %s WHERE %s = %d LIMIT 1",
			                           	chatTable.getTableNameAndCreate(),
										DBDefinition.CHAT_COLUMN_TYPE,
										type);
			c = db.rawQuery(sql, null);
			if (c.moveToFirst())
			{
				result =  true;
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return result;
	}
	
	public boolean isUserMailExistDifferentType(ChatTable chatTable, int type)
	{
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return false;

		boolean result = false;
		Cursor c = null;
		try
		{
			String sql = String.format(	Locale.US, "SELECT * FROM %s WHERE %s <> %d LIMIT 1",
			                           	chatTable.getTableNameAndCreate(),
										DBDefinition.CHAT_COLUMN_TYPE,
										type);
			c = db.rawQuery(sql, null);
			if (c.moveToFirst())
			{
				result =  true;
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return result;
	}
	
	public MsgItem getChatBySequeueId(ChatTable chatTable, int seqId)
	{
		MsgItem msg = null;
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable() || seqId<1) return msg;

		Cursor c = null;
		try
		{
			String sql = String.format(	Locale.US, "SELECT * FROM %s WHERE %s = %d",
			                           	chatTable.getTableNameAndCreate(),
										DBDefinition.CHAT_COLUMN_SEQUENCE_ID,
										seqId);
			c = db.rawQuery(sql, null);
			if (!c.moveToFirst())
			{
				return msg;
			}else{
				msg = new MsgItem(c);
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return msg;
	}
	
	/**
	 * @param createTime 获取<此时间的邮件，需>0才有效
	 * @param countLimit 数量限制。至多返回该数量，需>0才有效，若<=0则不限数量
	 */
	public List<MsgItem> getUserMailByTime(ChatTable chatTable, int createTime, int countLimit)
	{
		ArrayList<MsgItem> msgs = new ArrayList<MsgItem>();
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return msgs;

		Cursor c = null;
		try
		{
			c = queryUserMailByCreateTime(chatTable, createTime, countLimit);
			while (c != null && c.moveToNext())
			{
				MsgItem msg = new MsgItem(c);
				msgs.add(msg);
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return msgs;
	}
	
	private Cursor queryUserMailByCreateTime(ChatTable chatTable, int createTime, int countLimit)
	{
		if (!isDBAvailable()) 
			return null;
		String sql="";
		if(createTime > 0)
		{
			sql = String.format(Locale.US, "SELECT * FROM %s WHERE %s < %d ORDER BY %s DESC",
			                   	chatTable.getTableNameAndCreate(),
								DBDefinition.CHAT_COLUMN_CREATE_TIME,
								createTime,
								DBDefinition.CHAT_COLUMN_CREATE_TIME);
		}
		else
		{
			sql = String.format(Locale.US, "SELECT * FROM %s ORDER BY %s DESC",
			                   	chatTable.getTableNameAndCreate(),
								DBDefinition.CHAT_COLUMN_CREATE_TIME);
		} 
		
		if(countLimit > 0){
			sql += " LIMIT " + countLimit;
		}
		
		Cursor c = db.rawQuery(sql, null);
		return c;
	}

	public List<MsgItem> getChatMsgBySection(ChatTable chatTable, int upperId, int lowerId)
	{
		ArrayList<MsgItem> msgs = new ArrayList<MsgItem>();
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return msgs;

		Cursor c = null;
		try
		{
			c = queryChatBySection(chatTable, upperId, lowerId);
			while (c != null && c.moveToNext())
			{
				MsgItem msg = new MsgItem(c);
				msgs.add(msg);
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return msgs;
	}

	private Cursor queryChatBySection(ChatTable chatTable, int upperId, int lowerId)
	{
		if (!isDBAvailable()) 
			return null;
		int minId = Math.min(upperId, lowerId);
		int maxId = Math.max(upperId, lowerId);
		String sql = String.format(Locale.US, "SELECT * FROM %s WHERE %s >= %d AND %s <= %d ORDER BY %s DESC",
		                           	chatTable.getTableNameAndCreate(),
									DBDefinition.CHAT_COLUMN_SEQUENCE_ID,
									minId,
									DBDefinition.CHAT_COLUMN_SEQUENCE_ID,
									maxId,
									DBDefinition.CHAT_COLUMN_SEQUENCE_ID);
		Cursor c = db.rawQuery(sql, null);
		return c;
	}
	
	public ArrayList<MailData> getSysMailByTime(ChatTable chatTable, int createTime)
	{
		ArrayList<MailData> mails = new ArrayList<MailData>();
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return mails;

		Cursor c = null;
		try
		{
			if(ChatServiceController.isNewMailUIEnable)
				c = queryMailByCreateTime(chatTable.channelID,createTime);
			else
				c = queryMailByCreateTime(chatTable,createTime);
			
			while (c != null && c.moveToNext())
			{
				MailData mail = new MailData(c);
				mails.add(mail);
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}

		return mails;
	}
	
	private Cursor queryMailByCreateTime(ChatTable chatTable,int createTime)
	{
		if (!isDBAvailable()) 
			return null;
		String sql="";
		
		if(createTime==-1)
		{
			if(chatTable.isChannelType())
			{
				int type = chatTable.getMailTypeByChannelId();
				if(type>0)
				{
					sql=String.format(Locale.US, "SELECT * FROM %s WHERE %s='%s' OR %s=%d ORDER BY %s DESC",
							DBDefinition.TABEL_MAIL,
							DBDefinition.MAIL_CHANNEL_ID,
							chatTable.channelID,
							DBDefinition.MAIL_TYPE,
							type,
							DBDefinition.MAIL_CREATE_TIME);
				}
			}
			else
			{
				sql=String.format(Locale.US, "SELECT * FROM %s WHERE %s='%s' AND %s<>%d AND %s<>%d AND %s<>%d ORDER BY %s DESC",
						DBDefinition.TABEL_MAIL,
						DBDefinition.MAIL_CHANNEL_ID,
						chatTable.channelID,
						DBDefinition.MAIL_TYPE,
						MailManager.MAIL_RESOURCE,
						DBDefinition.MAIL_TYPE,
						MailManager.MAIL_ATTACKMONSTER,
						DBDefinition.MAIL_TYPE,
						MailManager.MAIL_RESOURCE_HELP,
						DBDefinition.MAIL_CREATE_TIME);
			}
			
		}
		else
		{
			if(chatTable.isChannelType())
			{
				int type = chatTable.getMailTypeByChannelId();
				if(type>0)
				{
					sql=String.format(Locale.US, "SELECT * FROM %s WHERE ( %s='%s' OR %s=%d ) AND %s < %d ORDER BY %s DESC",
							DBDefinition.TABEL_MAIL,
							DBDefinition.MAIL_CHANNEL_ID,
							chatTable.channelID,
							DBDefinition.MAIL_TYPE,
							type,
							DBDefinition.MAIL_CREATE_TIME,
							createTime,
							DBDefinition.MAIL_CREATE_TIME);
				}
			}
			else
			{
				sql=String.format(Locale.US, "SELECT * FROM %s WHERE %s='%s' AND %s<>%d AND %s<>%d AND %s<>%d AND %s < %d ORDER BY %s DESC",
						DBDefinition.TABEL_MAIL,
						DBDefinition.MAIL_CHANNEL_ID,
						chatTable.channelID,
						DBDefinition.MAIL_TYPE,
						MailManager.MAIL_RESOURCE,
						DBDefinition.MAIL_TYPE,
						MailManager.MAIL_RESOURCE_HELP,
						DBDefinition.MAIL_TYPE,
						MailManager.MAIL_ATTACKMONSTER,
						DBDefinition.MAIL_CREATE_TIME,
						createTime,
						DBDefinition.MAIL_CREATE_TIME);
			}
			
		}
		Cursor c = db.rawQuery(sql, null);
		return c;
	}
	
	private String getSqlByChannelId(String channelId)
	{
		String sql="";
		List<Integer> typeArray = MailManager.getInstance().getChannelTypeArrayByChannel(channelId);
		if(typeArray==null || typeArray.size()<=0)
			return ""; 
		String temp = "";
		
		String[] specailTitle = {
				"114111", 
				"105726",
				"105727",
				"105728",
				"105729",
				"105730"
		};
		
		String allianceSpecialSql = "";
		String systemSpecialSql = "";
		for(int i=0;i<specailTitle.length;i++)
		{
			if(i>0)
			{
				allianceSpecialSql += " OR ";
				systemSpecialSql += " AND ";
			}
			allianceSpecialSql += DBDefinition.MAIL_TITLE + " = " +specailTitle[i];
			systemSpecialSql += DBDefinition.MAIL_TITLE + " <> " +specailTitle[i];
		}
		if(StringUtils.isNotEmpty(systemSpecialSql))
			systemSpecialSql += (" AND " + DBDefinition.MAIL_TITLE + " <> '137460'");
		else
			systemSpecialSql += (DBDefinition.MAIL_TITLE + " <> '137460'");
//		System.out.println("allianceSpecialSql:"+allianceSpecialSql);
//		System.out.println("systemSpecialSql:"+systemSpecialSql);
		
		for (int i = 0; i < typeArray.size(); i++)
		{
			int type = typeArray.get(i).intValue();
			if(type>0)
			{
				if(i>0)
					temp += " OR ";
				if(channelId.equals(MailManager.CHANNELID_ALLIANCE) && type == MailManager.MAIL_SYSTEM && StringUtils.isNotEmpty(allianceSpecialSql))
					temp += "(" + DBDefinition.MAIL_TYPE + " = " + type +" AND (" + allianceSpecialSql + "))";
				else if(channelId.equals(MailManager.CHANNELID_SYSTEM) && type == MailManager.MAIL_SYSTEM && StringUtils.isNotEmpty(systemSpecialSql))
					temp += "(" + DBDefinition.MAIL_TYPE + " = " + type +" AND (" + systemSpecialSql + "))";
				else
					temp += DBDefinition.MAIL_TYPE + " = " + type;
			}
		}
		if(channelId.equals(MailManager.CHANNELID_FIGHT))
		{
			if(StringUtils.isNotEmpty(temp))
				temp += (" OR (" + DBDefinition.MAIL_TYPE + " = " + MailManager.MAIL_SYSTEM + " AND " + DBDefinition.MAIL_TITLE + " = '137460')");
			else
				temp += (DBDefinition.MAIL_TYPE + " = " + MailManager.MAIL_SYSTEM + " AND " + DBDefinition.MAIL_TITLE + " = '137460'");
		}
		if(StringUtils.isNotEmpty(temp))
		{
			temp += ")";
			sql = " WHERE (" + temp;
		}
		return sql;
	}
	
	private Cursor queryMailByCreateTime(String channelId,int createTime)
	{
		if (!isDBAvailable()) 
			return null;
		if(StringUtils.isEmpty(channelId) || StringUtils.isEmpty(getSqlByChannelId(channelId)))
			return null;
		String sql="SELECT * FROM " + DBDefinition.TABEL_MAIL + getSqlByChannelId(channelId);
		if(createTime == -1)
		{
			sql += " ORDER BY " + DBDefinition.MAIL_CREATE_TIME +" DESC";
		}
		else
		{
			sql += " AND " + DBDefinition.MAIL_CREATE_TIME + " < " + createTime + "ORDER BY " + DBDefinition.MAIL_CREATE_TIME +" DESC";
		}
		Cursor c = db.rawQuery(sql, null);
		return c;
	}

	public void prepareChatTable(ChatTable chatTable)
	{
		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return;
		
		if (!isTableExists(chatTable.getTableName()))
		{
			System.out.println("DBManager prepareChatTable(): " + chatTable.getChannelName());
			if(chatTable.channelType == DBDefinition.CHANNEL_TYPE_ALLIANCE){
				System.out.println("USER_TYPE_ALLIANCE");
			}
			if(chatTable.channelType != DBDefinition.CHANNEL_TYPE_OFFICIAL)
				createChatTable(chatTable.getTableName());
			else
				createMailTable();

			// 邮件时不应该插入频道user，会与玩家user冲突
			if(chatTable.channelType != DBDefinition.CHANNEL_TYPE_OFFICIAL){
				UserInfo user = new UserInfo();
				user.uid = chatTable.getChannelName();
				insertUser(user);
			}
			
			if(getChannel(chatTable) == null){
				ChatChannel channel = ChannelManager.getInstance().getChannel(chatTable);
				insertChannel(channel);
			}
		}
	}

	public void insertChannel(ChatChannel channel)
	{
		if (channel == null || !isDBAvailable()) return;
		System.out.println("DBManager insertChannel(): ");

		try
		{
			db.insert(DBDefinition.TABEL_CHANNEL, null, channel.getContentValues());
//			channel.refreshRenderData();
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	public boolean isTableExists(String tableName)
	{
		if (StringUtils.isEmpty(tableName) || !isDBAvailable()) return false;

		int count = 0;
		Cursor c = null;
		try
		{
			String sql = String.format(Locale.US, "SELECT COUNT(*) FROM %s WHERE type = '%s' AND name = '%s'",
										DBDefinition.TABLE_SQLITE_MASTER,
										"table",
										tableName);
			c = db.rawQuery(sql, null);
			if (!c.moveToFirst())
			{
				return false;
			}
			count = c.getInt(0);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
		finally
		{
			if (c != null) c.close();
		}
		return count > 0;
	}

	private void createChatTable(String tableName)
	{
		if (StringUtils.isEmpty(tableName) || !isDBAvailable()) return;
		
		try
		{
			db.execSQL(DBDefinition.CREATE_TABLE_CHAT.replace(DBDefinition.CHAT_TABLE_NAME_PLACEHOLDER, tableName));
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}
	
	private void createMailTable()
	{
		if (!isDBAvailable())
			return;

		try {
			db.execSQL(DBDefinition.CREATE_TABEL_MAIL);
		} catch (Exception e) {
			LogUtil.printException(e);
		}
	}

	public static String sqliteEscape(String keyWord)
	{
		keyWord = keyWord.replace("/", "//");
		keyWord = keyWord.replace("'", "''");
		keyWord = keyWord.replace("[", "/[");
		keyWord = keyWord.replace("]", "/]");
		keyWord = keyWord.replace("%", "/%");
		keyWord = keyWord.replace("&", "/&");
		keyWord = keyWord.replace("_", "/_");
		keyWord = keyWord.replace("(", "/(");
		keyWord = keyWord.replace(")", "/)");
		return keyWord;
	}

//	public List<MsgItem> getLatestMessages(ChatTable chatTable)
//	{
//		ArrayList<MsgItem> msgs = new ArrayList<MsgItem>();
//		if (StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return msgs;
//
//		Cursor c = null;
//		try
//		{
//			String userId = chatTable.channelID + DBDefinition.getPostfixForType(chatTable.channelType);
//			String md5TableId = MathUtil.md5(userId);
//			c = queryLatestChat(md5TableId, 20);
//			while (c != null && c.moveToNext())
//			{
//				MsgItem msg = new MsgItem(c);
//				msgs.add(msg);
//			}
//		}
//		catch (Exception e)
//		{
//			LogUtil.printException(e);
//		}
//		finally
//		{
//			if (c != null) c.close();
//		}
//
//		return msgs;
//	}
//
//	private Cursor queryLatestChat(String md5TableId, int count)
//	{
//		String sql = String.format(Locale.US, "SELECT * FROM %s ORDER BY %s DESC LIMIT %d",
//									DBDefinition.chatTableId2Name(md5TableId),
//									DBDefinition.CHAT_COLUMN_CREATE_TIME,
//									count);
//		Cursor c = db.rawQuery(sql, null);
//		return c;
//	}
	
//	public boolean isMsgExists(String tableName, MsgItem msg)
//	{
////		System.out.println("DBManager isMsgExists(): " + tableName);
//		if (StringUtils.isEmpty(tableName) || msg == null || !isDBAvailable()) return false;
//
//		int count = 0;
//		Cursor c = null;
//		try
//		{
//			String sql = String.format(	Locale.US,
//										"SELECT * FROM %s WHERE %s = %d AND %s = %d AND %s = '%s'",
//										tableName,
//										DBDefinition.CHAT_COLUMN_SEQUENCE_ID,
//										msg.sequenceId,
//										DBDefinition.CHAT_COLUMN_CREATE_TIME,
//										msg.createTime,
//										DBDefinition.CHAT_COLUMN_USER_ID,
//										msg.uid);
//			c = db.rawQuery(sql, null);
//			if (!c.moveToFirst())
//			{
//				return false;
//			}
//			count = c.getInt(0);
//		}
//		catch (Exception e)
//		{
//			LogUtil.printException(e);
//		}
//		finally
//		{
//			if (c != null) c.close();
//		}
//
//		return count > 0;
//	}

	// 改成从channel表取
	// 需遍历。不可直接从sqlite_sequence表获取，因为它是服务器端的，且由于缓存区数量有限，在本地不一定以0开头、不一定连续
//	public ArrayList<GetNewMsgParam> getAllMaxSequenceNumber()
//	{
//		ArrayList<GetNewMsgParam> params = new ArrayList<GetNewMsgParam>();
//		ArrayList<String> chatTables = getChatTableNames();
//		for (String table : chatTables)
//		{
//			GetNewMsgParam param = new GetNewMsgParam();
//			param.chatTableName = table;
//			param.dbMaxSeqId = getMarginalSequenceNumber(table, true);
//			params.add(param);
//		}
//		return params;
//	}

//	public MsgItem getMessage(MsgItem msg, ChatTable chatTable)
//	{
////		System.out.println("DBManager getMessage(): msg = " + msg.msg + " channelID = " + chatTable.channelID);
//		if (msg == null || StringUtils.isEmpty(chatTable.channelID) || !isDBAvailable()) return null;
//		
//		MsgItem result = null;
//		Cursor c = null;
//		try
//		{
//			String where = String.format(Locale.US, "%s = %s AND %s = '%s'", 
//										DBDefinition.CHAT_COLUMN_LOCAL_SEND_TIME, msg.sendLocalTime,
//										DBDefinition.CHAT_COLUMN_USER_ID, msg.uid);
//
//			String sql = String.format(Locale.US, "SELECT * FROM %s WHERE %s", chatTable.getTableNameAndCreate(), where);
//			c = db.rawQuery(sql, null);
//
//			if (!c.moveToFirst())
//			{
//				return null;
//			}
//			
//			result = new MsgItem(c);
//		}
//		catch (Exception e)
//		{
//			LogUtil.printException(e);
//		}
//		finally
//		{
//			if (c != null) c.close();
//		}
//
//		return result;
//	}

//	private ArrayList<String> getChatTableNames()
//	{
//		ArrayList<String> result = new ArrayList<String>();
//		if (!isDBAvailable()) return result;
//
//		Cursor c = null;
//		try
//		{
//			String sql = String.format(	"SELECT * FROM %s WHERE type = '%s' AND name LIKE '%s%%'",
//										DBDefinition.TABLE_SQLITE_MASTER,
//										"table",
//										DBDefinition.TABEL_CHAT);
//			c = db.rawQuery(sql, null);
//			while (c.moveToNext())
//			{
//				result.add(c.getString(c.getColumnIndex("name")));
//			}
//		}
//		catch (Exception e)
//		{
//			LogUtil.printException(e);
//		}
//		finally
//		{
//			if (c != null) c.close();
//		}
//		return result;
//	}
}