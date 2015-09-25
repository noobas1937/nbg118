crowdin翻译平台整合、内部作业流程的自动化系统已经搭建完毕。
～～～～～～～～
目前有两部分翻译：
1、游戏内部的翻译。『*.ini 文件』
2、其他翻译。

------
1、针对上述第一种ini文件的翻译，由以下两个工程协作实现：
「COK Localization CN」负责把 「简中 text_zh_CN.ini」 翻译成 「繁中 text_zh_TW.ini」和「英文 text_en.ini」
「COK Localization EN」负责把 「英文 text_en.ini」 翻译成 其他多语言 「*.ini」

2、针对上述第二种翻译，用过程「COK Localization FAQ」实现。文件的上传下载均为手工。

～～～～～～～～
使用方法：
1、『策划』 上传 「简中 text_zh_CN.ini」 文件到 SVN。 ===> 【此时，系统会自动检测该更新】
2、『系统』 把SVN「简中 text_zh_CN.ini」 文件更新到crowdin的CN工程。
3、『译员』 在CN上翻译简体中文。  ===>  【翻译完成、校验完毕后，系统自动从crowdin上下载翻译文件。并上传到SVN。】
4、『系统』 从crowdin CN工程下载「text_en.ini」文件，上传到 crowdin EN 工程；并提交到SVN、mail通知相关人员。
5、『译员』 在EN上翻译。===>  【各语言，翻译完成、校验完毕后，系统自动从crowdin上下载翻译文件。并上传到SVN。】
6、『系统』 从crowdin EN工程下载「text_*.ini」文件，上传到SVN、mail通知相关人员。
7、『策划』 本地更新svn文件夹，获取最新的翻译文件，【确认】后使用。｛注：待系统完善后，可以无需人工确认、直接上线｝
～～～～～～～～

【注】：
如果翻译要求时间比较紧，来不及人工翻译，可以用机器翻译MT（这个需要手工点击MT翻译）。
机器翻译后crowdin系统或许不能通知我们的系统，此时如果没有收到 系统自动发的mail通知，可以手工点击「build project」来触发。
