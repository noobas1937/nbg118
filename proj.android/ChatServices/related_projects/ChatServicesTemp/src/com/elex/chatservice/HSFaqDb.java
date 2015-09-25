package com.elex.chatservice;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;

public class HSFaqDb
{
  public static final String TAG = "HelpShiftDebug";
  private HelpshiftDb hsDb;
  public static final String TABLE_NAME = "__hs__faqs";
  public static final String COLUMN_ID = "_id";
  public static final String COLUMN_QID = "__hs__qid";
  public static final String COLUMN_PUBLISH_ID = "__hs__publish_id";
  public static final String COLUMN_SECTION_ID = "__hs__section_id";
  public static final String COLUMN_TITLE = "__hs__title";
  public static final String COLUMN_BODY = "__hs__body";
  public static final String COLUMN_IS_HELPFUL = "__hs__is_helpful";
  public static final String COLUMN_IS_RTL = "__hs__is_rtl";
  private static final String DROP_TABLE = "drop table __hs__faqs;";
  private static final String CREATE_TABLE = "create table __hs__faqs (_id integer primary key autoincrement, __hs__qid text not null, __hs__publish_id text not null, __hs__section_id text not null, __hs__title text not null, __hs__body text not null, __hs__is_helpful integer, __hs__is_rtl integer, FOREIGN KEY(__hs__section_id) REFERENCES __hs__sections (_id));";

  protected HSFaqDb(Context context)
  {
    this.hsDb = HelpshiftDb.getInstance(context);
  }

  protected SQLiteDatabase getWritableDatabase() {
    return this.hsDb.getWritableDatabase();
  }

  protected static void onCreateDb(SQLiteDatabase db) {
    db.execSQL("create table __hs__faqs (_id integer primary key autoincrement, __hs__qid text not null, __hs__publish_id text not null, __hs__section_id text not null, __hs__title text not null, __hs__body text not null, __hs__is_helpful integer, __hs__is_rtl integer, FOREIGN KEY(__hs__section_id) REFERENCES __hs__sections (_id));");
  }

  protected static void onDestroyDb(SQLiteDatabase db) {
    db.execSQL("drop table __hs__faqs;");
  }

  protected static void onUpgradeDb(SQLiteDatabase db, int oldVersion, int newVersion) {
    Log.w(HSFaqDb.class.getName(), "Upgrading database from version " + oldVersion + " to " + newVersion + ", which will destroy all old data");

    db.execSQL("DROP TABLE IF EXISTS __hs__faqs");
  }

  protected HelpshiftDb getHsDb() {
    return this.hsDb;
  }
}