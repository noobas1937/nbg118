package com.elex.chatservice.view;

import org.apache.commons.lang.StringUtils;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnTouchListener;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;

import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.ServiceInterface;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.util.CompatibleApiUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.view.actionbar.ActionBarFragment;

public class WriteMailFragment extends ActionBarFragment
{
	private ImageButton	sendMailButton;
	private ImageView	addPeopleButton;
	private EditText	recieverEditText;
	private EditText	contentEditText;
	private TextView	recieverLabel;
	private TextView	contentLabel;

	private String		roomName	= "";
	private String		memberUids	= "";
	private String		memberNames	= "";

	public WriteMailFragment()
	{
	}

	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		((WriteMailActivity) getActivity()).fragment = this;
	}

	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
	{
		this.activity = ((WriteMailActivity) getActivity());

		
		return inflater.inflate(ResUtil.getId(this, "layout", "cs__write_mail_fragment"), container, false);
	}
	
	@Override
	public void onResume()
	{
		super.onResume();
		
		if (StringUtils.isNotEmpty(getWriteMailActivity().roomName)) roomName = getWriteMailActivity().roomName;
		if (StringUtils.isNotEmpty(getWriteMailActivity().memberUids)) memberUids = getWriteMailActivity().memberUids;
		if (StringUtils.isNotEmpty(getWriteMailActivity().memberNames))
		{
			memberNames = getWriteMailActivity().memberNames;
			recieverEditText.setText(memberNames);
		}
	}
	
	public WriteMailActivity getWriteMailActivity()
	{
		return (WriteMailActivity) activity;
	}

	public void onViewCreated(View view, Bundle savedInstanceState)
	{
		super.onViewCreated(view, savedInstanceState);

		getTitleLabel().setText(LanguageManager.getLangByKey(LanguageKeys.TITLE_MAIL));
		
		sendMailButton = (ImageButton) view.findViewById(R.id.sendMailButton);
		addPeopleButton = (ImageView) view.findViewById(R.id.addPeopleButton);
		recieverEditText = (EditText) view.findViewById(R.id.recieverEditText);
		contentEditText = (EditText) view.findViewById(R.id.contentEditText);
		recieverLabel = (TextView) view.findViewById(R.id.recieverLabel);
		contentLabel = (TextView) view.findViewById(R.id.contentLabel);

		recieverLabel.setText(LanguageManager.getLangByKey(LanguageKeys.WRITE_MAIL_RECIEVER));
		contentLabel.setText(LanguageManager.getLangByKey(LanguageKeys.WRITE_MAIL_CONTENT));
		
		refreshSendButton();

		recieverEditText.addTextChangedListener(new TextWatcher()
		{
			@Override
			public void afterTextChanged(Editable s)
			{
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count, int after)
			{
			}

			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count)
			{
				refreshSendButton();
			}
		});

		contentEditText.addTextChangedListener(new TextWatcher()
		{
			@Override
			public void afterTextChanged(Editable s)
			{
				contentEditText.post(new Runnable()
				{
					@Override
					public void run()
					{
						// refreshWordCount();
					}
				});
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count, int after)
			{
			}

			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count)
			{
				refreshSendButton();
			}
		});

		addPeopleButton.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				ChatServiceController.isCreateChatRoom=true;
				ServiceInterface.showMemberSelectorActivity(activity, true);
			}
		});
		
		sendMailButton.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View view)
			{
				sendMail();
			}
		});
		
		fragmentLayout.setOnTouchListener(new OnTouchListener() {
			@SuppressLint("ClickableViewAccessibility")
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				hideSoftKeyBoard();
				return false;
			}
		});
	}

	private void refreshSendButton()
	{
		if (recieverEditText.getText().length() == 0 || contentEditText.getText().length() == 0)
		{
			sendMailButton.setEnabled(false);
			CompatibleApiUtil.getInstance().setButtonAlpha(sendMailButton, false);
		}
		else
		{
			sendMailButton.setEnabled(true);
			CompatibleApiUtil.getInstance().setButtonAlpha(sendMailButton, true);
		}
	}
	
	private void sendMail()
	{
		if (memberUids.equals(""))
		{
			String content = contentEditText.getText().toString();
			String title = content;
			if (content.length() > 30) title = content.substring(0, 29);

			String allianceMailId = "";
			// 如果填自己的名字则发联盟邮件
			if (recieverEditText.getText().toString().equals(UserManager.getInstance().getCurrentUser().userName)
					&& StringUtils.isNotEmpty(UserManager.getInstance().getCurrentUser().allianceId))
			{
				allianceMailId = UserManager.getInstance().getCurrentUser().allianceId;
			}
			ChatServiceController.getInstance().host.sendMailMsg(	recieverEditText.getText().toString(),
																	title,
																	content,
																	allianceMailId,
																	"",
																	false,
																	0,
																	"",
																	"");
		}
		else
		{
			ChatServiceController.getInstance().host.createChatRoom(memberNames, 
			                                                        memberUids, 
			                                                        roomName, 
			                                                        contentEditText.getText().toString());
		}
		activity.exitActivity();
	}
}
