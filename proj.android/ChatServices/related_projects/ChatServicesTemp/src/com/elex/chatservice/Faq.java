package com.elex.chatservice;

final class Faq
{
  private long id;
  private String qId;
  private String title;
  private String publish_id;
  private String type;
  private String section_publish_id;
  private String body;
  private int is_helpful;
  private Boolean is_rtl;

  public Faq()
  {
    this.title = "";
    this.publish_id = "";
    this.type = "";
    this.body = "";
    this.section_publish_id = "";
    this.is_helpful = 0;
    this.is_rtl = Boolean.valueOf(false);
  }

  public Faq(String title, String publish_id, String type)
  {
    this.title = title;
    this.publish_id = publish_id;
    this.type = type;
  }

  public Faq(long id, String qId, String publish_id, String sectionId, String title, String body, int isHelpful, Boolean isRtl)
  {
    this.id = id;
    this.qId = qId;
    this.title = title;
    this.publish_id = publish_id;
    this.type = "faq";
    this.section_publish_id = sectionId;
    this.body = body;
    this.is_helpful = isHelpful;
    this.is_rtl = isRtl;
  }

  public String toString()
  {
    return this.title;
  }

  protected String getId() {
    return this.qId;
  }

  protected String getPublishId() {
    return this.publish_id;
  }

  protected String getSectionPublishId() {
    return this.section_publish_id;
  }

  protected String getType() {
    return this.type;
  }
  protected String getTitle() {
    return this.title;
  }

  protected String getBody() {
    return this.body;
  }

  protected int getIsHelpful() {
    return this.is_helpful;
  }

  protected Boolean getIsRtl() {
    return this.is_rtl;
  }

  public void setId(long id) {
    this.id = id;
  }
}