package com.elex.chatservice;

public abstract interface HSFaqSyncStatusEvents
{
  public abstract void faqsUpdated();

  public abstract void searchIndexesUpdated();
}