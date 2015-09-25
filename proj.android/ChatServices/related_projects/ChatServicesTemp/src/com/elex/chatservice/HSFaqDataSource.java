package com.elex.chatservice;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import java.util.ArrayList;
import java.util.List;

public class HSFaqDataSource
{
  public static final String TAG = "HelpShiftDebug";
  private HSFaqDb faqDb;
  private String[] allColumns = { "_id", "__hs__qid", "__hs__publish_id", "__hs__section_id", "__hs__title", "__hs__body", "__hs__is_helpful", "__hs__is_rtl" };

  private String[] allUiColumns = { "__hs__qid", "__hs__publish_id", "__hs__section_id", "__hs__title" };

  protected HSFaqDataSource(Context context)
  {
    this.faqDb = new HSFaqDb(context);
  }

  protected int removeFromDb(Faq newFaq) {
    return HelpshiftDb.getDb().delete("__hs__faqs", "__hs__qid = ?", new String[] { newFaq.getId() });
  }

  protected String doesExist(Faq faq)
  {
    String id = "";
    Cursor cursor = HelpshiftDb.getDb().query("__hs__faqs", this.allColumns, "__hs__qid = ?", new String[] { faq.getId() }, null, null, null);
    try
    {
      cursor.moveToFirst();
      if (!cursor.isAfterLast())
        id = faq.getId();
    }
    finally
    {
      cursor.close();
    }
    return id;
  }

  protected Faq createFaq(Faq newFaq) {
    ContentValues values = new ContentValues();
    values.put("__hs__title", newFaq.getTitle());
    values.put("__hs__publish_id", newFaq.getPublishId());
    values.put("__hs__section_id", newFaq.getSectionPublishId());
    values.put("__hs__body", newFaq.getBody());
    values.put("__hs__is_helpful", Integer.valueOf(newFaq.getIsHelpful()));
    values.put("__hs__is_rtl", newFaq.getIsRtl());

    long insertId = 0L;
    String faqId = doesExist(newFaq);
    if (faqId.length() == 0) {
      values.put("__hs__qid", newFaq.getId());
      insertId = HelpshiftDb.getDb().insert("__hs__faqs", null, values);
    } else {
      insertId = HelpshiftDb.getDb().update("__hs__faqs", values, "__hs__qid = ?", new String[] { faqId });
    }

    newFaq.setId(insertId);
    return newFaq;
  }

  protected List<Faq> getFaqsDataForSection(String sectionPubId) {
    if ((sectionPubId == null) || (sectionPubId.equals(""))) {
      return new ArrayList();
    }

    List faqs = new ArrayList();

    Cursor cursor = HelpshiftDb.getDb().query("__hs__faqs", this.allColumns, "__hs__section_id = ?", new String[] { sectionPubId }, null, null, null);
    try
    {
      cursor.moveToFirst();
      while (!cursor.isAfterLast()) {
        Faq faq = cursorToFaq(cursor);
        faqs.add(faq);
        cursor.moveToNext();
      }
    }
    finally {
      cursor.close();
    }
    return faqs;
  }

  protected List<Faq> getFaqsForSection(String sectionPubId) {
    if ((sectionPubId == null) || (sectionPubId.equals(""))) {
      return new ArrayList();
    }

    List faqs = new ArrayList();

    Cursor cursor = HelpshiftDb.getDb().query("__hs__faqs", this.allUiColumns, "__hs__section_id = ?", new String[] { sectionPubId }, null, null, null);
    try
    {
      cursor.moveToFirst();
      while (!cursor.isAfterLast()) {
        Faq faq = cursorToFaqForUi(cursor);
        faqs.add(faq);
        cursor.moveToNext();
      }
    }
    finally {
      cursor.close();
    }
    return faqs;
  }

  protected Faq getFaq(String publishId) {
    if ((publishId == null) || (publishId.equals(""))) {
      return new Faq();
    }

    Faq faq = null;
    Cursor cursor = HelpshiftDb.getDb().query("__hs__faqs", this.allColumns, "__hs__publish_id = ?", new String[] { publishId }, null, null, null);
    try
    {
      cursor.moveToFirst();
      if (!cursor.isAfterLast())
        faq = cursorToFaq(cursor);
    }
    finally {
      cursor.close();
    }
    return faq;
  }

  protected int setIsHelpful(String faqId, Boolean state)
  {
    if ((faqId == null) || (faqId.equals(""))) {
      return 0;
    }

    ContentValues values = new ContentValues();
    int isHelpful = state.booleanValue() == true ? 1 : -1;
    values.put("__hs__is_helpful", Integer.valueOf(isHelpful));

    return HelpshiftDb.getDb().update("__hs__faqs", values, "__hs__qid = ?", new String[] { faqId });
  }

  private Faq cursorToFaq(Cursor cursor)
  {
    Faq faq = new Faq(cursor.getLong(0), cursor.getString(1), cursor.getString(2), cursor.getString(3), cursor.getString(4), cursor.getString(5), cursor.getInt(6), Boolean.valueOf(cursor.getInt(7) == 1));

    return faq;
  }

  private Faq cursorToFaqForUi(Cursor cursor) {
    Faq faq = new Faq(0L, cursor.getString(0), cursor.getString(1), cursor.getString(2), cursor.getString(3), "", 0, Boolean.valueOf(false));

    return faq;
  }

  protected void clearFaqData() {
    this.faqDb.getHsDb().clearFaqsDb();
  }
}