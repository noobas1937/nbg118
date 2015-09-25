package com.elex.chatservice;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class HelpshiftDb extends SQLiteOpenHelper
{
  protected static final String TAG = "HelpShiftDebug";
  private static HelpshiftDb instance = null;
  private static SQLiteDatabase dbInstance;
  private static final String DATABASE_NAME = "__hs__db";
  private static final int DATABASE_VERSION = 1;

  private HelpshiftDb(Context context)
  {
    super(context, "__hs__db", null, 1);
    getWritableDatabase();
  }

  protected static HelpshiftDb getInstance(Context c) {
    if (instance == null) {
      instance = new HelpshiftDb(c);
    }
    return instance;
  }

  public SQLiteDatabase getWritableDatabase()
  {
    if (dbInstance == null) {
      dbInstance = super.getWritableDatabase();
    }
    return dbInstance;
  }

  public void onCreate(SQLiteDatabase db)
  {
    HSSectionDb.onCreateDb(db);
    HSFaqDb.onCreateDb(db);
  }

  public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion)
  {
    Log.w(HelpshiftDb.class.getName(), "Upgrading database from version " + oldVersion + " to " + newVersion + ", which will destroy all old data");

    HSSectionDb.onUpgradeDb(db, oldVersion, newVersion);
    HSFaqDb.onUpgradeDb(db, oldVersion, newVersion);
    onCreate(db);
  }

  protected void clearFaqsDb()
  {
    HSFaqDb.onDestroyDb(dbInstance);
    HSSectionDb.onDestroyDb(dbInstance);
    onCreate(dbInstance);
  }

  public static synchronized SQLiteDatabase getDb() {
    if (instance != null) {
      return instance.getWritableDatabase();
    }
    return null;
  }
}