#include "ECServiceCocos2dx.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
jobject parseValueVectorToArrayElva (JNIEnv *env, cocos2d::CCDictionary* metaData) {
    if(metaData == NULL) {
        return NULL;
    }
    const char* arraylist_class_name = "java/util/ArrayList";
    jclass clsArrayList = env->FindClass(arraylist_class_name);
    jmethodID arrayConstructorID = env->GetMethodID (clsArrayList, "<init>", "()V");
    jobject jarrayobj = env->NewObject(clsArrayList, arrayConstructorID);
    jmethodID array_add_method = 0;
    array_add_method = env->GetMethodID(clsArrayList, "add", "(Ljava/lang/Object;)Z");
    
    cocos2d::CCArray* tags = dynamic_cast<cocos2d::CCArray*> (metaData->objectForKey("hs-tags"));
    int j = 0;
    for (j = 0; j < tags->count(); j++) {
        cocos2d::CCString *valStr = dynamic_cast<cocos2d::CCString*> (tags->objectAtIndex(j));
        if (valStr != NULL && valStr->length() > 0) {
            jstring value = env->NewStringUTF(valStr->getCString());
            env->CallBooleanMethod(jarrayobj, array_add_method, value);
        }
    }
    return jarrayobj;
}

jobject parseCCArrayToArrayListElva (JNIEnv *env, cocos2d::CCArray *data) {
    if(data == NULL) {
        return NULL;
    }
    const char* arraylist_class_name = "java/util/ArrayList";
    jclass clsArrayList = env->FindClass(arraylist_class_name);
    jmethodID arrayConstructorID = env->GetMethodID (clsArrayList, "<init>", "()V");
    jobject jarrayobj = env->NewObject(clsArrayList, arrayConstructorID);
    jmethodID array_add_method = 0;
    array_add_method = env->GetMethodID(clsArrayList, "add", "(Ljava/lang/Object;)Z");

    int j = 0;
    for (j = 0; j < data->count(); j++) {
        cocos2d::CCString *valStr = dynamic_cast<cocos2d::CCString*> (data->objectAtIndex(j));
        if (valStr != NULL && valStr->length() > 0) {
            jstring value = env->NewStringUTF(valStr->getCString());
            env->CallBooleanMethod(jarrayobj, array_add_method, value);
        }
    }
    return jarrayobj;
}

jobject parseTagsArrayElva (JNIEnv *env, cocos2d::CCDictionary* metaData) {
    if(metaData == NULL) {
        return NULL;
    }
    return parseCCArrayToArrayListElva(env, dynamic_cast<cocos2d::CCArray*> (metaData->objectForKey("hs-tags")));
}

jobject parseConfigDictionaryElva (JNIEnv* env, cocos2d::CCDictionary *config) {
    if(config == NULL) {
        return NULL;
    }
    const char* hashmap_class_name = "java/util/HashMap";
    jclass clsHashMap = env->FindClass(hashmap_class_name);
    jmethodID constructorID = env->GetMethodID (clsHashMap, "<init>", "()V");

    jobject jmapobj = env->NewObject(clsHashMap, constructorID);

    jmethodID map_put_method = 0;
    map_put_method = env->GetMethodID(clsHashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    jstring data;
    
    
    jstring key = env->NewStringUTF("showConversationFlag");
    cocos2d::CCString* showConversationFlag = dynamic_cast<cocos2d::CCString*>(config->objectForKey("showConversationFlag"));
    if (showConversationFlag != NULL && showConversationFlag->length() > 0) {
        data = env->NewStringUTF(showConversationFlag->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF("hs-custom-metadata");
    cocos2d::CCDictionary* metaData = dynamic_cast<cocos2d::CCDictionary*>(config->objectForKey("hs-custom-metadata"));
    if (metaData!= NULL) {
        jobject metaMap = env->NewObject(clsHashMap, constructorID);
        cocos2d::CCArray *keys = metaData->allKeys();
        int i = 0;
        for(i = 0; i < keys->count(); i++) {
            cocos2d::CCString* keyStr = dynamic_cast<cocos2d::CCString*> (keys->objectAtIndex(i));
            if (keyStr != NULL && keyStr->length() > 0) {
                jstring key = env->NewStringUTF(keyStr->getCString());
                cocos2d::CCString *valStr = dynamic_cast<cocos2d::CCString*> (metaData->objectForKey(keyStr->getCString()));
                if(valStr != NULL && valStr->length() > 0) {
                    jstring value = env->NewStringUTF(valStr->getCString());
                    env->CallObjectMethod(metaMap, map_put_method, key, value);
                } else if (valStr == NULL) {
                    env->CallObjectMethod(metaMap, map_put_method, key, parseTagsArrayElva(env, metaData));
                }
            }
        }
        env->CallObjectMethod(jmapobj, map_put_method, key, metaMap);
    }
    
    return jmapobj;
}

/*
展示聊天主界面，不带config
*/
void ECServiceCocos2dx::showElva(string playerName,string playerUid,int serverId,string playerParseId,string showConversationFlag){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showElvaChatServiceFrom2dx"
                                                , "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V")
       )
    {
        return;
    }
    else
    {
        jstring npcName = minfo.env->NewStringUTF("AIHelp");
        jstring userName = minfo.env->NewStringUTF(playerName.c_str());
        jstring uid = minfo.env->NewStringUTF(playerUid.c_str());
        jstring parseId = minfo.env->NewStringUTF(playerParseId.c_str());
        jstring showConversationFlag = minfo.env->NewStringUTF(showConversationFlag.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,npcName
                                        ,userName
                                        ,uid
                                        ,parseId
                                        ,(jint)serverId
                                        ,showConversationFlag
                                        );
        minfo.env->DeleteLocalRef(npcName);
        minfo.env->DeleteLocalRef(userName);
        minfo.env->DeleteLocalRef(uid);
        minfo.env->DeleteLocalRef(parseId);
        minfo.env->DeleteLocalRef(showConversationFlag);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}


/*
展示聊天主界面，带config
*/
void ECServiceCocos2dx::showElva(string playerName,string playerUid,int serverId,string playerParseId,string showConversationFlag,cocos2d::CCDictionary *config){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showElvaChatServiceFrom2dx"
                                                , "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/util/HashMap;)V")
       )
    {
        return;
    }
    else
    {
        jstring npcName = minfo.env->NewStringUTF("AIHelp");
        jstring userName = minfo.env->NewStringUTF(playerName.c_str());
        jstring uid = minfo.env->NewStringUTF(playerUid.c_str());
        jstring parseId = minfo.env->NewStringUTF(playerParseId.c_str());
        jstring showConversationFlag = minfo.env->NewStringUTF(showConversationFlag.c_str());
        jobject hashMap = parseConfigDictionaryElva(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,npcName
                                        ,userName
                                        ,uid
                                        ,parseId
                                        ,(jint)serverId
                                        ,showConversationFlag
                                        ,hashMap
                                        );
        minfo.env->DeleteLocalRef(npcName);
        minfo.env->DeleteLocalRef(userName);
        minfo.env->DeleteLocalRef(uid);
        minfo.env->DeleteLocalRef(parseId);
        minfo.env->DeleteLocalRef(showConversationFlag);
        minfo.env->DeleteLocalRef(hashMap);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}


/*登录不上直接显示FAQ 不带config*/
void ECServiceCocos2dx::showSingleFAQ(string faqId){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showFAQ"
                                                , "(Ljava/lang/String;)V")
       )
    {
        return;
    }
    else
    {
        jstring jFaqId = minfo.env->NewStringUTF(faqId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jFaqId
                               			);
        minfo.env->DeleteLocalRef(jFaqId);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

/*登录不上直接显示FAQ 带config*/
void ECServiceCocos2dx::showSingleFAQ(string faqId,cocos2d::CCDictionary *config){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showFAQ"
                                                , "(Ljava/lang/String;Ljava/util/HashMap;)V")
       )
    {
        return;
    }
    else
    {
        jstring jFaqId = minfo.env->NewStringUTF(faqId.c_str());
        jobject hashMap = parseConfigDictionaryElva(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jFaqId
                                        ,hashMap
                                        );
        minfo.env->DeleteLocalRef(jFaqId);
        minfo.env->DeleteLocalRef(hashMap);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

/*用户自行选择使用 FAQ List 不带config*/
void ECServiceCocos2dx::showFAQs(){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showFAQList"
                                                ,"()V")
       )
    {
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        );
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

/*用户自行选择使用 FAQ List 带config*/
void ECServiceCocos2dx::showFAQs(cocos2d::CCDictionary *config){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showFAQList"
                                                ,"(Ljava/util/HashMap;)V")
       )
    {
        return;
    }
    else
    {
        jobject hashMap = parseConfigDictionaryElva(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,hashMap
                                        );
        minfo.env->DeleteLocalRef(hashMap);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

/*初始化 在主activity的onCreate中调用
            a:当前运行的action
            appKey:注册后由我方提供
            domain:注册后我方提供域名
            appId:注册后由我方提供
*/
void ECServiceCocos2dx::init(string appKey,string domain,string appId) {
       cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"init"
                                                ,"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")
       )
    {
        return;
    }
    else
    {
        jstring jAppSecret = minfo.env->NewStringUTF(appKey.c_str());
        jstring jDomain = minfo.env->NewStringUTF(domain.c_str());
        jstring jAppId = minfo.env->NewStringUTF(appId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jAppSecret
                                        ,jDomain
                                        ,jAppId
                                        );
        minfo.env->DeleteLocalRef(jAppSecret);
        minfo.env->DeleteLocalRef(jDomain);
        minfo.env->DeleteLocalRef(jAppId);
        minfo.env->DeleteLocalRef(minfo.classID);
    } 
}

void ECServiceCocos2dx::setName(string game_name){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"setName"
                                                ,"(Ljava/lang/String;)V")
       )
    {
        return;
    }
    else
    {
        jstring jGame_name = minfo.env->NewStringUTF(game_name.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jGame_name
                                        );
        minfo.env->DeleteLocalRef(jGame_name);
        minfo.env->DeleteLocalRef(minfo.classID);
    } 
}

//void ECServiceCocos2dx::setFcmToken(string deviceToken){
//    if(deviceToken=="") {
//        return;
//    }
//
//    cocos2d::JniMethodInfo minfo;
//    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
//                                                ,"setFcmToken"
//                                                ,"(Ljava/lang/String;)V")
//       )
//    {
//        return;
//    }
//    else
//    {
//        jstring jdeviceToken = minfo.env->NewStringUTF(deviceToken.c_str());
//        minfo.env->CallStaticVoidMethod(minfo.classID
//                                        ,minfo.methodID
//                                        ,jdeviceToken
//                                        );
//        minfo.env->DeleteLocalRef(jdeviceToken);
//        minfo.env->DeleteLocalRef(minfo.classID);
//    }
//}

void ECServiceCocos2dx::registerDeviceToken(string deviceToken,bool isVIP){
	if(deviceToken=="") {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"registerDeviceToken"
                                                ,"(Ljava/lang/String;Z)V")
       )
    {
        return;
    }
    else
    {
        jstring jdeviceToken = minfo.env->NewStringUTF(deviceToken.c_str());
        jboolean jisVip = isVIP ? JNI_TRUE : JNI_FALSE;
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jdeviceToken
                                        ,jisVip
                                        );
        minfo.env->DeleteLocalRef(jdeviceToken);
        minfo.env->DeleteLocalRef(minfo.classID);
    } 
}

void ECServiceCocos2dx::setUserId(string playerUid){
    if(playerUid=="") {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"setUserId"
                                                ,"(Ljava/lang/String;)V")
       )
    {
        return;
    }
    else
    {
        jstring jplayerUid = minfo.env->NewStringUTF(playerUid.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jplayerUid
                                        );
        minfo.env->DeleteLocalRef(jplayerUid);
        minfo.env->DeleteLocalRef(minfo.classID);
    } 
}

void ECServiceCocos2dx::setUserName(string playerName){
    if(playerName=="") {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"setUserName"
                                                ,"(Ljava/lang/String;)V")
       )
    {
        return;
    }
    else
    {
        jstring jplayerName = minfo.env->NewStringUTF(playerName.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jplayerName
                                        );
        minfo.env->DeleteLocalRef(jplayerName);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ECServiceCocos2dx::setServerId(int serverId){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"setServerId"
                                                ,"(I)V")
       )
    {
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,(jint)serverId
                                        );
        minfo.env->DeleteLocalRef(minfo.classID);
    } 
}

/*VIP_CHAT*/
void ECServiceCocos2dx::showConversation(string playerUid,int serverId){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showConversation"
                                                ,"(Ljava/lang/String;I)V")
       )
    {
        return;
    }
    else
    {
        jstring jplayerUid = minfo.env->NewStringUTF(playerUid.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jplayerUid
                                        ,(jint)serverId
                                        );
        minfo.env->DeleteLocalRef(jplayerUid);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

/*VIP_CHAT*/
void ECServiceCocos2dx::showConversation(string playerUid,int serverId,cocos2d::CCDictionary *config){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showConversation"
                                                ,"(Ljava/lang/String;ILjava/util/HashMap;)V")
       )
    {
        return;
    }
    else
    {
        jstring jplayerUid = minfo.env->NewStringUTF(playerUid.c_str());
        jobject hashMap = parseConfigDictionaryElva(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jplayerUid
                                        ,(jint)serverId
                                        ,hashMap
                                        );
        minfo.env->DeleteLocalRef(jplayerUid);
        minfo.env->DeleteLocalRef(hashMap);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ECServiceCocos2dx::showFAQSection(string sectionPublishId){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showFAQSection"
                                                , "(Ljava/lang/String;)V")
       )
    {
        return;
    }
    else
    {
        jstring jSectionPublishId = minfo.env->NewStringUTF(sectionPublishId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jSectionPublishId
                                        );
        minfo.env->DeleteLocalRef(jSectionPublishId);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ECServiceCocos2dx::showFAQSection(string sectionPublishId,cocos2d::CCDictionary *config){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showFAQSection"
                                                , "(Ljava/lang/String;Ljava/util/HashMap;)V")
       )
    {
        return;
    }
    else
    {
        jstring jSectionPublishId = minfo.env->NewStringUTF(sectionPublishId.c_str());
        jobject hashMap = parseConfigDictionaryElva(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jSectionPublishId
                                        ,hashMap
                                        );
        minfo.env->DeleteLocalRef(jSectionPublishId);
        minfo.env->DeleteLocalRef(hashMap);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ECServiceCocos2dx::setSDKLanguage(const char *locale) {
    if(locale != NULL || strlen(locale) != 0) {
        cocos2d::JniMethodInfo minfo;
        bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                                "net/aihelp/chatservice/ElvaChatServiceHelper",
                                                                "setSDKLanguage",
                                                                "(Ljava/lang/String;)V");
        if(hasMethod) {
            jstring idStr = minfo.env->NewStringUTF(locale);
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, idStr);
            minfo.env->DeleteLocalRef(idStr);
            minfo.env->DeleteLocalRef(minfo.classID);
        }
    }    
}

void ECServiceCocos2dx::useDevice(){
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "net/aihelp/chatservice/ElvaChatServiceHelper",
                                                             "useDevice",
                                                             "()V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ECServiceCocos2dx::setEvaluateStar(int star){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"setEvaluateStar"
                                                ,"(I)V")
       )
    {
        return;
    }
    else
    {
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,(jint)star
                                        );
        minfo.env->DeleteLocalRef(minfo.classID);
    } 
}

/*
展示运营主界面，带config
*/
void ECServiceCocos2dx::showElvaOP(string playerName,string playerUid,int serverId,string playerParseId,string showConversationFlag,cocos2d::CCDictionary *config){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showElvaChatServiceOPFrom2dx"
                                                , "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/util/HashMap;)V")
       )
    {
        return;
    }
    else
    {
        jstring npcName = minfo.env->NewStringUTF("AIHelp");
        jstring userName = minfo.env->NewStringUTF(playerName.c_str());
        jstring uid = minfo.env->NewStringUTF(playerUid.c_str());
        jstring parseId = minfo.env->NewStringUTF(playerParseId.c_str());
        jstring showConversationFlag = minfo.env->NewStringUTF(showConversationFlag.c_str());
        jobject hashMap = parseConfigDictionaryElva(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,npcName
                                        ,userName
                                        ,uid
                                        ,parseId
                                        ,(jint)serverId
                                        ,showConversationFlag
                                        ,hashMap
                                        );
        minfo.env->DeleteLocalRef(npcName);
        minfo.env->DeleteLocalRef(userName);
        minfo.env->DeleteLocalRef(uid);
        minfo.env->DeleteLocalRef(parseId);
        minfo.env->DeleteLocalRef(showConversationFlag);
        minfo.env->DeleteLocalRef(hashMap);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

/*
展示运营主界面，带index
*/
void ECServiceCocos2dx::showElvaOP(string playerName,string playerUid,int serverId,string playerParseId,string showConversationFlag,cocos2d::CCDictionary *config,int defaultTabIndex){
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showElvaChatServiceOPFrom2dx"
                                                , "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/util/HashMap;I)V")
       )
    {
        return;
    }
    else
    {
        jstring npcName = minfo.env->NewStringUTF("AIHelp");
        jstring userName = minfo.env->NewStringUTF(playerName.c_str());
        jstring uid = minfo.env->NewStringUTF(playerUid.c_str());
        jstring parseId = minfo.env->NewStringUTF(playerParseId.c_str());
        jstring showConversationFlag = minfo.env->NewStringUTF(showConversationFlag.c_str());
        jobject hashMap = parseConfigDictionaryElva(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,npcName
                                        ,userName
                                        ,uid
                                        ,parseId
                                        ,(jint)serverId
                                        ,showConversationFlag
                                        ,hashMap
										,defaultTabIndex
                                        );
        minfo.env->DeleteLocalRef(npcName);
        minfo.env->DeleteLocalRef(userName);
        minfo.env->DeleteLocalRef(uid);
        minfo.env->DeleteLocalRef(parseId);
        minfo.env->DeleteLocalRef(showConversationFlag);
        minfo.env->DeleteLocalRef(hashMap);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void ECServiceCocos2dx::showVIPChat(string webAppId)
{
    if(webAppId=="") {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    if(!cocos2d::JniHelper::getStaticMethodInfo(minfo,"net/aihelp/chatservice/ElvaChatServiceHelper"
                                                ,"showVIPChat"
                                                ,"(Ljava/lang/String;)V")
       )
    {
        return;
    }
    else
    {
        jstring jwebAppId = minfo.env->NewStringUTF(webAppId.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID
                                        ,minfo.methodID
                                        ,jwebAppId
                                        );
        minfo.env->DeleteLocalRef(jwebAppId);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

#endif
