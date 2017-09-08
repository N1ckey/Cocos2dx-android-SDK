### SDK Folder and File Structure Description

1. Folder **elvachatservice** contains the SDK core files, which should be imported to your target project completely. 
2. Folder **interface/cocos** contains .cpp source to interface AIHELP SDK with your cocos project code. Please import ECServiceCocos2dx.h、ECServiceCocos2dx.cpp to the **Classes** folder of your project.
3. Folder **support-jars** contains all the common libraries that AIHELP SDK depends on build. Please include them all if your project doesn’t have any of them already. If your project already has some of the libraries, you only need to include others. However, if your existing library has a lower version than version 23, please update it using the newer version we have provided here. If your existing library with the same name has a higher version, you don’t need to replace it. 
4. ReleaseNote.md is the release notes.
5. README.md is the [SDK Integration Guide](https://github.com/AI-HELP/cocos-android-SDK/blob/master/README.md)
