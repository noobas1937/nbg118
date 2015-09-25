#!/bin/bash

#须配置内容  start

#以下是邮箱的相关设置，jiecao.fm 提供了一个可以直接使用的邮箱账号，请不要去更改邮箱密码，以便他人使用
#更改收件人就可直接使用
#收件人，多个收件人以空格分隔
email_reciver="zhaohongfu@elex-tech.com liruiqi@elex-tech.com"
#发送者邮箱
email_sender=xcodeshell@jiecao.fm
#邮箱用户名
email_username=xcodeshell@jiecao.fm
#邮箱密码
email_password=jiecao.fm1
#smtp服务器地址
email_smtphost=smtp.exmail.qq.com

#可配置内容  end

#fir 相关url
fir_domain="http://fir.im"
fir_lookup_url="$fir_domain/api/v2/app/info"
fir_finish_url="$fir_domain/api/finish"
fir_token="p4qpton4xXry5h7wfaxqfLT3dUVvEZCQsMUn5eSd"

#获取shell文件所在的绝对路径
current_path=$(pwd)
tmp_path=$(dirname $0)
cd $tmp_path
shell_path=$(pwd)
cd $current_path
currdate=`date '+%Y-%m-%d'`

#build文件夹路径
build_path="."

echo "Fetching app infomation from the ipa file..."
#切换到tmp文件夹
cd /tmp
#创建临时文件夹
tmpfoldername="cok_ipa"
if [ -d ./${tmpfoldername} ];then
	rm -rf ${tmpfoldername}
fi
mkdir ${tmpfoldername}

cd ${tmpfoldername}

echo ${tmpfoldername}
echo ${current_path}/package/${currdate}/*.ipa
#拷贝ipa到临时文件夹中
cp ${current_path}/package/${currdate}/IF_TestFlight.ipa ./tmp.zip
#将ipa解压
unzip tmp.zip &>/dev/null
#app文件中Info.plist文件路径
app_infoplist_path=$(pwd)/Payload/*.app/Info.plist
#取版本号
bundleShortVersion=$(/usr/libexec/PlistBuddy -c "print CFBundleShortVersionString" ${app_infoplist_path})
#取build值
bundleVersion=$(/usr/libexec/PlistBuddy -c "print CFBundleVersion" ${app_infoplist_path})
#取bundleIdentifier
bundleIdentifier=$(/usr/libexec/PlistBuddy -c "print CFBundleIdentifier" ${app_infoplist_path})
#取CFBundleName
target_name=$(/usr/libexec/PlistBuddy -c "print CFBundleName" ${app_infoplist_path})
#取CFBundleDisplayName
display_name=$(/usr/libexec/PlistBuddy -c "print CFBundleDisplayName" ${app_infoplist_path})

#删除临时文件夹
cd ..
rm -rf ${tmpfoldername}

#进入到工程build路径下
cd $current_path

#显示名称
ipa_name="${display_name}"

if [ -d ./$target_name ];then
	rm -rf $target_name
fi
mkdir $target_name
#拷贝ipa
cp ./package/${currdate}/IF_TestFlight.ipa ./$target_name/${target_name}.ipa
cd $target_name

echo "=====  bundleIdentifier is $bundleIdentifier  ======"
#到fir api 查询具体的包上传地址
echo "Fetching upload url from fir..."
echo "do fetch $fir_lookup_url/$bundleIdentifier?token=$fir_token"
response=`curl "$fir_lookup_url/$bundleIdentifier?token=$fir_token"`
echo $response
#网络请求结果判断
rtnValue=$?
if [ $rtnValue != 0 ];then
exit 1
fi

fir_publish_url=`ruby -e "require 'json'; iJson = JSON.parse '${response}'; puts iJson['bundle']['pkg']['url']"`
echo $fir_publish_url
fir_publish_key=`ruby -e "require 'json'; iJson = JSON.parse '${response}'; puts iJson['bundle']['pkg']['key']"`
fir_publish_token=`ruby -e "require 'json'; iJson = JSON.parse '${response}'; puts iJson['bundle']['pkg']['token']"`
realShort=`ruby -e "require 'json'; iJson = JSON.parse '${response}'; puts iJson['short']"`
#第一次请求得到的 Appid, 用来最后更新 App 的信息.
fir_app_id=`ruby -e "require 'json'; iJson = JSON.parse '${response}'; puts iJson['id']"`
#上传ipa包，并获取最终的url short name
echo "Uploading the ipa file..."
curl -F file=@${target_name}.ipa -F "key=$fir_publish_key" -F "token=$fir_publish_token" $fir_publish_url
rtnValue=$?
if [ $rtnValue != 0 ];then
exit 1
fi
echo "Finishing uploading and filling in the app information..."
echo "Updateing app info."
response=`curl -X PUT -H "Content-Length: 0" -L "http://fir.im/api/v2/app/$fir_app_id?token=$fir_token&version=$bundleVersion&versionShort=$bundleShortVersion" -v`
realShort=`ruby -e "require 'json'; iJson = JSON.parse '${response}'; puts iJson['short']"`
#拼接完整的下载url，并在浏览器中打开
pulish_url=$fir_domain/$realShort
echo $pulish_url
# open $pulish_url
