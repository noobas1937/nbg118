//
//  ChatServiceMessageListener.h
//  IF
//
//  Created by 周雨 on 15-1-5.
//
//

#ifndef IF_ChatServiceMessageListener_h
#define IF_ChatServiceMessageListener_h

#include <string>
using namespace std;

class ChatServiceMessageListener
{
public:
	virtual void sendMessage(string msg){};
	virtual void onTextChanged(string msg){};
	virtual void onBackPressed(){};
	virtual void setHeightFromNative(int h, int usableHeightSansKeyboard){};
};


#endif
