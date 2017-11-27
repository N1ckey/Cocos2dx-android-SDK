//
//  ECServiceCocos2dx.h
//  ElvaChatService Cocos2dx SDK
//

#ifndef ECServiceCocos2dx_h
#define ECServiceCocos2dx_h

#include "cocos2d.h"
#include <string>
#define ES_WITH_TAGS_MATCHING "withTagsMatching"

using namespace std;

class ECServiceCocos2dx
{
public:
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
public:
    static void init(string appKey,string domain,string appId);
	static void showElva(string playerName,string playerUid,int serverId,string playerParseId,string showConversationFlag);
    static void showElva(string playerName,string playerUid,int serverId,string playerParseId,string showConversationFlag,cocos2d::ValueMap& config);
    static void showSingleFAQ(string faqId);
    static void showSingleFAQ(string faqId,cocos2d::ValueMap& config);
    static void showFAQSection(string sectionPublishId);
    static void showFAQSection(string sectionPublishId,cocos2d::ValueMap& config);
    static void showFAQs();
    static void showFAQs(cocos2d::ValueMap& config);
    static void setName(string game_name);
    static void registerDeviceToken(string deviceToken);
    static void setFcmToken(string deviceToken);
    static void setUserId(string playerUid);//use self-service, should call before showFAQ
    static void setServerId(int serverId);//use self-service, should call before showFAQ
    static void setUserName(string playerName);//call before service, after init
    static void showConversation(string playerUid,int serverId);
    static void showConversation(string playerUid,int serverId,cocos2d::ValueMap& config);
    static bool setSDKLanguage(const char* locale);
    static void useDevice();
    static void setEvaluateStar(int star);//评价默认星星数量
	static void showElvaOP(string playerName,string playerUid,int serverId,string playerParseId,string showConversationFlag,cocos2d::ValueMap& config);
    static void showElvaOP(string playerName,string playerUid,int serverId,string playerParseId,string showConversationFlag,cocos2d::ValueMap& config,int defaultTabIndex);
    static void showVIPChat(string webAppId);
    
};

#endif
