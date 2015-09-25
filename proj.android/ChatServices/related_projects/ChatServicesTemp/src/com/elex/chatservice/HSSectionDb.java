package com.elex.chatservice;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;

public class HSSectionDb
{
  private static final String TAG = "HelpShiftDebug";
  private HelpshiftDb hsDb;
  protected static final String TABLE_NAME = "__hs__sections";
  protected static final String COLUMN_ID = "_id";
  protected static final String COLUMN_SECTION_ID = "__hs__section_id";
  protected static final String COLUMN_PUBLISH_ID = "__hs__publish_id";
  protected static final String COLUMN_TITLE = "__hs__title";
  private static final String DROP_TABLE = "drop table __hs__sections;";
  private static final String CREATE_TABLE = "create table __hs__sections (_id integer primary key autoincrement, __hs__section_id text not null, __hs__publish_id integer not null, __hs__title text not null);";

  protected HSSectionDb(Context context)
  {
    this.hsDb = HelpshiftDb.getInstance(context);
  }

  protected SQLiteDatabase getWritableDatabase() {
    return this.hsDb.getWritableDatabase();
  }

  protected static void onCreateDb(SQLiteDatabase db) {
    db.execSQL("create table __hs__sections (_id integer primary key autoincrement, __hs__section_id text not null, __hs__publish_id integer not null, __hs__title text not null);");
  }

  protected static void onDestroyDb(SQLiteDatabase db) {
    db.execSQL("drop table __hs__sections;");
  }

  protected static void onUpgradeDb(SQLiteDatabase db, int oldVersion, int newVersion) {
    Log.w(HSSectionDb.class.getName(), "Upgrading database from version " + oldVersion + " to " + newVersion + ", which will destroy all old data");

    db.execSQL("DROP TABLE IF EXISTS __hs__sections");
  }

  protected HelpshiftDb getHsDb() {
    return this.hsDb;
  }
}