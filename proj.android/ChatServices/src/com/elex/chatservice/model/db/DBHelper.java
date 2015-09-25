package com.elex.chatservice.model.db;

import java.io.File;

import org.apache.commons.lang.StringUtils;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.os.Environment;
import android.provider.BaseColumns;

import com.elex.chatservice.model.MsgItem;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.util.LogUtil;

/**
 * 创建初始表
 * 管理版本号、升级表
 *
 * @author zhouyu
 */
public class DBHelper extends SQLiteOpenHelper
{
	private static final String	DATABASE_NAME							= "chat_service.db";
	private static final int	VERSION_BASIS							= 1;
	private static final int	VERSION_ADD_CROSS_FIGHT_SRC_SERVER_ID	= 2;						// 6.11提交内网，外网尚未发布过
	private static final int	VERSION_ADD_MAIL_TABLE					= 3;
	private static final int	VERSION_ADD_TITLE_AND_SUMMARY			= 4;
	private static final int	VERSION_ADD_PARSE_VERSION				= 5;

	public static final int		CURRENT_DATABASE_VERSION				= VERSION_ADD_PARSE_VERSION;
	
	public DBHelper(Context context)
	{
		// CursorFactory设置为null,使用默认值
		super(context, getDBPath(context), null, CURRENT_DATABASE_VERSION);
	}

	private static String getDBPath(Context context)
	{
		String result = null;
		if (isSDcardAvaiable())
		{
			String user = StringUtils.isEmpty(UserManager.getInstance().getCurrentUserId()) ? "unknownUser" : UserManager.getInstance()
					.getCurrentUserId();
			// SD卡应用目录（卸载会删除）：context.getExternalFilesDir(null) + File.separator + user + "/database/"
			String directory = Environment.getExternalStorageDirectory() + "/data/data/" + context.getPackageName()
					+ File.separator + user	+ "/database/";
			if(prepareDirectory(directory)){
				result = directory + DATABASE_NAME;
			}
		}

		if(StringUtils.isEmpty(result))
		{
			result = DATABASE_NAME;
		}
		System.out.println("Database location: " + result);
		return result;
	}

	// S4上，有user时，要手动创建目录才行
	private static boolean prepareDirectory(String path)
	{
		File file = new File(path);
		if (file.exists() && file.isDirectory()){
			return true;
		}else{
			boolean result = file.mkdirs();
//			System.out.println("mkdirs:[" + path + "] success:" + result);
			return result;
		}
	}

	// 数据库第一次被创建时onCreate会被调用
	@Override
	public void onCreate(SQLiteDatabase db)
	{
		createBasicTable(db, DBDefinition.TABEL_USER, DBDefinition.CREATE_TABEL_USER);
		createBasicTable(db, DBDefinition.TABEL_CHANNEL, DBDefinition.CREATE_TABEL_CHANNEL);
		createBasicTable(db, DBDefinition.TABEL_MAIL, DBDefinition.CREATE_TABEL_MAIL);
	}

	private void createBasicTable(SQLiteDatabase db, String tableName, String tableDef)
	{
		try
		{
			if(!isTableExists(db, tableName)){
				System.out.println("createBasicTable: " + tableName);
				db.execSQL(tableDef);
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}

	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion)
	{
		System.out.println("onUpgrade() oldVersion:" + oldVersion + " newVersion:" + newVersion);
		
		db.beginTransaction();
		try
		{
			// we want apply all following updates, so no break statement here...
			switch (oldVersion)
			{
				case VERSION_BASIS:
					if(!existsColumnInTable(db, DBDefinition.TABEL_USER, DBDefinition.USER_CROSS_FIGHT_SRC_SERVER_ID)){
						db.execSQL("ALTER TABLE " + DBDefinition.TABEL_USER + " ADD "
								+ DBDefinition.USER_CROSS_FIGHT_SRC_SERVER_ID + " INTEGER DEFAULT -2");
					}
					upgradeTableVersion(db, DBDefinition.TABEL_USER, VERSION_ADD_CROSS_FIGHT_SRC_SERVER_ID);
				
				// TODO 如果升级失败的话，保证插入数据不出错、不会出现脏数据
				case VERSION_ADD_CROSS_FIGHT_SRC_SERVER_ID:
					// 扩展channel表
					if(!existsColumnInTable(db, DBDefinition.TABEL_CHANNEL, DBDefinition.CHANNEL_UNREAD_COUNT)){
						db.execSQL("ALTER TABLE " + DBDefinition.TABEL_CHANNEL + " ADD "
								+ DBDefinition.CHANNEL_UNREAD_COUNT + " INTEGER DEFAULT 0");
						db.execSQL("ALTER TABLE " + DBDefinition.TABEL_CHANNEL + " ADD "
								+ DBDefinition.CHANNEL_LATEST_ID + " TEXT");
						db.execSQL("ALTER TABLE " + DBDefinition.TABEL_CHANNEL + " ADD "
								+ DBDefinition.CHANNEL_LATEST_TIME + " INTEGER DEFAULT -1");
						db.execSQL("ALTER TABLE " + DBDefinition.TABEL_CHANNEL + " ADD "
								+ DBDefinition.CHANNEL_LATEST_MODIFY_TIME + " INTEGER DEFAULT -1");
						db.execSQL("ALTER TABLE " + DBDefinition.TABEL_CHANNEL + " ADD "
								+ DBDefinition.CHANNEL_SETTINGS + " TEXT");
					}
					upgradeTableVersion(db, DBDefinition.TABEL_CHANNEL, VERSION_ADD_MAIL_TABLE);
					
					// 新增mail表
					createBasicTable(db, DBDefinition.TABEL_MAIL, DBDefinition.CREATE_TABEL_MAIL);
					
					// 重建chat表
					recreateAllChatTables(db);
				
				case VERSION_ADD_MAIL_TABLE:
					// 扩展mail表
					if(!existsColumnInTable(db, DBDefinition.TABEL_MAIL, DBDefinition.MAIL_TITLE_TEXT)){
						db.execSQL("ALTER TABLE " + DBDefinition.TABEL_MAIL + " ADD "
								+ DBDefinition.MAIL_TITLE_TEXT + " TEXT");
						db.execSQL("ALTER TABLE " + DBDefinition.TABEL_MAIL + " ADD "
								+ DBDefinition.MAIL_SUMMARY + " TEXT");
						db.execSQL("ALTER TABLE " + DBDefinition.TABEL_MAIL + " ADD "
								+ DBDefinition.MAIL_LANGUAGE + " TEXT");
					}
					upgradeTableVersion(db, DBDefinition.TABEL_MAIL, VERSION_ADD_TITLE_AND_SUMMARY);
					
				case VERSION_ADD_TITLE_AND_SUMMARY:
					// 扩展mail表
					if(!existsColumnInTable(db, DBDefinition.TABEL_MAIL, DBDefinition.PARSE_VERSION)){
						db.execSQL("ALTER TABLE " + DBDefinition.TABEL_MAIL + " ADD "
								+ DBDefinition.PARSE_VERSION + " INTEGER DEFAULT -1");
					}
					upgradeTableVersion(db, DBDefinition.TABEL_MAIL, VERSION_ADD_PARSE_VERSION);
					
				case VERSION_ADD_PARSE_VERSION:
			}
			db.setTransactionSuccessful();
		}
		catch (Exception e)
		{
			// 若字段已存在会有异常 SQLiteException: duplicate column name: CrossFightSrcServerId (code 1)
			LogUtil.printException(e);
		}
		finally
		{
			db.endTransaction();
		}
	}

	// 修改表的版本号
	public void upgradeTableVersion(SQLiteDatabase db, String tableName, int newVersion)
	{
		System.out.println("upgradeTableVersion() tableName:" + tableName + " newVersion:" + newVersion);
		ContentValues cv = new ContentValues();
		cv.put(DBDefinition.COLUMN_TABLE_VER, newVersion);
		db.update(tableName, cv, null, null);
	}
	
	private void recreateAllChatTables(SQLiteDatabase db)
	{
		String sql = String.format(	"SELECT * FROM %s WHERE type = '%s' AND name LIKE '%s%%'",
									DBDefinition.TABLE_SQLITE_MASTER,
									"table",
									DBDefinition.TABEL_CHAT);
		Cursor cursor = db.rawQuery(sql, null);
		while (cursor != null && cursor.moveToNext())
		{
			String tableName = cursor.getString(cursor.getColumnIndex("name"));
			recreateChatTable(db, tableName);
		}
	}

	private void recreateChatTable(SQLiteDatabase db, String tableName)
	{
		String columns = DBDefinition.CHAT_COLUMN_SEQUENCE_ID + ","
				+ DBDefinition.CHAT_COLUMN_USER_ID + ","
				+ DBDefinition.CHAT_COLUMN_CHANNEL_TYPE + ","
				+ DBDefinition.CHAT_COLUMN_CREATE_TIME + ","
				+ DBDefinition.CHAT_COLUMN_LOCAL_SEND_TIME + ","
				+ DBDefinition.CHAT_COLUMN_TYPE + ","
				+ DBDefinition.CHAT_COLUMN_MSG + ","
				+ DBDefinition.CHAT_COLUMN_TRANSLATION + ","
				+ DBDefinition.CHAT_COLUMN_ORIGINAL_LANGUAGE + ","
				+ DBDefinition.CHAT_COLUMN_TRANSLATED_LANGUAGE + ","
				+ DBDefinition.CHAT_COLUMN_STATUS + ","
				+ DBDefinition.CHAT_COLUMN_ATTACHMENT_ID + ","
				+ DBDefinition.CHAT_COLUMN_MEDIA;
		
		db.execSQL("ALTER TABLE " + tableName + " RENAME TO TempOldTable");
		db.execSQL(DBDefinition.CREATE_TABLE_CHAT.replace(DBDefinition.CHAT_TABLE_NAME_PLACEHOLDER, tableName));
		db.execSQL("INSERT INTO " + tableName + "(" + columns + ") SELECT " + columns + " FROM TempOldTable");
		db.execSQL("DROP TABLE TempOldTable");
		
//		ALTER TABLE {tableName} RENAME TO TempOldTable;
//		CREATE TABLE {tableName} (name TEXT, COLNew {type} DEFAULT {defaultValue}, qty INTEGER, rate REAL);
//		INSERT INTO {tableName} (name, qty, rate) SELECT name, qty, rate FROM TempOldTable;
//		DROP TABLE TempOldTable;
	}

	public boolean isTableExists(SQLiteDatabase db, String tableName)
	{
	    if (StringUtils.isEmpty(tableName) || db == null || !db.isOpen())
	    {
	        return false;
	    }

		int count = 0;
		Cursor cursor = null;
		try
		{
			cursor = db.rawQuery("SELECT COUNT(*) FROM " + DBDefinition.TABLE_SQLITE_MASTER + " WHERE type = ? AND name = ?",
					new String[] { "table", tableName });
			if (!cursor.moveToFirst())
			{
				return false;
			}
		    count = cursor.getInt(0);
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}finally {
	        if (cursor != null)
	        	cursor.close();
	    }
	    return count > 0;
	}
	
	private boolean existsColumnInTable(SQLiteDatabase db, String tableName, String columnToCheck) {
	    if (StringUtils.isEmpty(tableName) || StringUtils.isEmpty(columnToCheck) || db == null || !db.isOpen())
	    {
	        return false;
	    }
	    boolean result = false;
	    Cursor c = null;
	    try{
	        // query 1 row
	    	c  = db.rawQuery( "SELECT * FROM " + tableName + " LIMIT 0", null );

	        // 即使没有数据，mCursor.getColumnIndex也可用
	        // getColumnIndex gives us the index (0 to ...) of the column - otherwise we get a -1
	        if(c != null && c.getColumnIndex(columnToCheck) != -1){
	        	// 没有数据时，mCursor.close会异常
	        	result = true;
	        }
	    }catch (Exception e){
	        //something went wrong. Missing the database? The table?
			LogUtil.printException(e);
	    }finally {
	        if (c != null)
	        	c.close();
	    }
	    return result;
	}
	
	public static boolean isSDcardAvaiable() {
		String sdCardState = Environment.getExternalStorageState();
		return sdCardState.equals(Environment.MEDIA_MOUNTED);
	}
}
