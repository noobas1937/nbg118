package com.elex.chatservice;

public class Section
{
  private long id;
  private String section_id;
  private String title;
  private String publish_id;

  public Section()
  {
    this.id = -1L;
    this.section_id = "";
    this.publish_id = "";
    this.title = "";
  }

  public Section(long id, String sectionId, String title, String publish_id)
  {
    this.id = id;
    this.section_id = sectionId;
    this.title = title;
    this.publish_id = publish_id;
  }

  public Section(String title, String publish_id)
  {
    this.id = -1L;
    this.title = title;
    this.publish_id = publish_id;
  }

  public Section(String id, String title, String publish_id)
  {
    this.id = -1L;
    this.section_id = id;
    this.title = title;
    this.publish_id = publish_id;
  }

  public String toString()
  {
    return this.title;
  }

  protected String getPublishId() {
    return this.publish_id;
  }

  protected String getTitle() {
    return this.title;
  }

  public void setId(long id) {
    this.id = id;
  }

  public String getSectionId() {
    return this.section_id;
  }
}