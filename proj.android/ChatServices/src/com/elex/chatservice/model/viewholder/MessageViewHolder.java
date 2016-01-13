package com.elex.chatservice.model.viewholder;

import java.util.Timer;
import java.util.TimerTask;

import com.elex.chatservice.model.MsgItem;

import android.view.View;

public class MessageViewHolder
{
	public Timer sendTimer;
	public TimerTask sendTimerTask;
	public MsgItem currentMsgItem;
	
	public View findViewById(View convertView,int id)
	{
		return null;
	}
	
	public void stopSendTimer()
	{
		if (sendTimer != null)
		{
			if (sendTimerTask != null)
			{
				sendTimerTask.cancel();
			}
			sendTimer.cancel();
			sendTimer.purge();
		}
	}

	public void removeSendTimer()
	{
		stopSendTimer();
		sendTimer = null;
		sendTimerTask = null;
	}
}
