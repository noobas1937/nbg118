package com.elex.chatservice.util;

public interface IAnalyticTracker
{
    public void trackException(String exceptionType, String funcInfo, String cause, String message);
    public void transportMail(String jsonStr);
    public String getParseFromCocos2dx(String jsonStr);
}
