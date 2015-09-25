package com.elex.chatservice;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import java.util.ArrayList;
import java.util.List;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class HSSectionDataSource
{
  private static final String TAG = "HelpShiftDebug";
  private HSSectionDb sectionDb;
  private HSFaqDataSource faqDataSource;
  private String[] allColumns = { "_id", "__hs__section_id", "__hs__publish_id", "__hs__title" };

  protected HSSectionDataSource(Context context)
  {
    this.sectionDb = new HSSectionDb(context);
    this.faqDataSource = new HSFaqDataSource(context);
  }

  protected Section createSection(Section newSection) {
    ContentValues values = new ContentValues();
    values.put("__hs__title", newSection.getTitle());
    values.put("__hs__publish_id", newSection.getPublishId());
    values.put("__hs__section_id", newSection.getSectionId());
    long insertId = HelpshiftDb.getDb().insert("__hs__sections", null, values);
    newSection.setId(insertId);
    return newSection;
  }

  protected void storeSections(JSONArray sections)
  {
    try {
      HelpshiftDb.getDb().beginTransaction();
      for (int i = 0; i < sections.length(); i++) {
        JSONObject section = sections.getJSONObject(i);
        JSONArray faqs = section.getJSONArray("faqs");
        String sectionTitle = section.getString("title");
        String sectionPublishId = section.getString("publish_id");
        String sectionId = section.getString("id");
        Section newSection = new Section(sectionId, sectionTitle, sectionPublishId);
        createSection(newSection);

        HelpshiftDb.getDb().beginTransaction();
        for (int j = 0; j < faqs.length(); j++) {
          JSONObject faq = faqs.getJSONObject(j);
          Faq newFaq = new Faq(-1L, faq.getString("id"), faq.getString("publish_id"), sectionPublishId, faq.getString("title"), faq.getString("body"), 0, Boolean.valueOf(faq.getString("is_rtl") == "true"));

          this.faqDataSource.createFaq(newFaq);
        }

        HelpshiftDb.getDb().setTransactionSuccessful();
        HelpshiftDb.getDb().endTransaction();

        HelpshiftDb.getDb().yieldIfContendedSafely();
      }
      HelpshiftDb.getDb().setTransactionSuccessful();
    } catch (JSONException e) {
      Log.d("HelpShiftDebug", e.toString(), e);
    }
    finally {
      HelpshiftDb.getDb().endTransaction();
    }
  }

  protected List<Section> getAllSections() {
    List sections = new ArrayList();

    Cursor cursor = HelpshiftDb.getDb().query("__hs__sections", this.allColumns, null, null, null, null, null);
    try
    {
      cursor.moveToFirst();
      while (!cursor.isAfterLast()) {
        Section section = cursorToSection(cursor);
        sections.add(section);
        cursor.moveToNext();
      }
    } finally {
      cursor.close();
    }
    return sections;
  }

  protected Section getSection(String publishId) {
    if ((publishId == null) || (publishId.equals(""))) {
      return new Section();
    }
    Section section = null;

    Cursor cursor = HelpshiftDb.getDb().query("__hs__sections", this.allColumns, "__hs__publish_id = ?", new String[] { publishId }, null, null, null);
    try
    {
      cursor.moveToFirst();
      if (!cursor.isAfterLast())
        section = cursorToSection(cursor);
    }
    finally {
      cursor.close();
    }
    return section;
  }

  private Section cursorToSection(Cursor cursor) {
    Section section = new Section(cursor.getLong(0), cursor.getString(1), cursor.getString(3), cursor.getString(2));

    return section;
  }

  protected void clearSectionsData() {
    this.sectionDb.getHsDb().clearFaqsDb();
  }
}