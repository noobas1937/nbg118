/**
 * Copyright (c) 2014-2015 by Helpshift, Inc. All Rights Reserved.
 *
 */
package com.helpshift;

import java.io.File;

class HelpshiftSessionDelegate implements Helpshift.HelpshiftDelegate {

  /**
   * Delegate method which is called when Helpshift session begins.
   */
  @Override
  public void helpshiftSessionBegan () {
    HelpshiftBridge.helpshiftSessionBegan();
  }

  /**
   * Delegate method which is called when Helpshift session ends.
   */
  @Override
  public void helpshiftSessionEnded () {
    HelpshiftBridge.helpshiftSessionEnded();
  }

  /**
   * Delegate method which is called when user starts a new conversation
   *
   * @param newConversationMessage
   */
  @Override
  public void newConversationStarted (String newConversationMessage) {
    HelpshiftBridge.newConversationStarted(newConversationMessage);
  }

  /**
   * Delegate method which is called when user sends a new message
   *
   * @param newMessage
   */
  @Override
  public void userRepliedToConversation (String newMessage) {
    HelpshiftBridge.userRepliedToConversation(newMessage);
  }

  /**
   * Delegate method which is called when user completes the customer
   * satisfaction survey
   *
   * @param rating
   * @param feedback
   */
  @Override
  public void userCompletedCustomerSatisfactionSurvey (int rating, String feedback) {
    HelpshiftBridge.userCompletedCustomerSatisfactionSurvey(rating, feedback);
  }

  /**
   * Optional delegate method that is called when the user taps an downloaded attachment file to view it.
   *
   * @param attachmentFile
   */
  @Override
  public void displayAttachmentFile (File attachmentFile) {
    // No-op
  }

}
