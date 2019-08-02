#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/time.h>
#include <com_sjl_jnidata_JniDataTransfer.h>
#include <android/log.h>
#include <iostream>

using namespace std;


#define GET_ARRAY_LEN(array, len) { len = sizeof(array)/sizeof(array[0]);}

//日志LOGI("user info----name:%s, age:%d, sex:%s.", "xxx", 18, "男");
static const char *TAG = "simple_logger_jni";
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGW(fmt, args...) __android_log_print(ANDROID_LOG_WARN, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

void checkNull(jobject obj, char *msg);

class NullException : public exception {
public:
    const char *what() const _GABIXX_NOEXCEPT {
        if (msg == NULL) {
            return "NullException";
        }
        return msg;
    }

public:
    NullException(char const *msg) {
        this->msg = msg;
    }

    NullException() {
    }

public:
    const char *msg;
};

void checkNull(jobject obj, char const *msg) {
    if (obj == NULL) {
        NullException a(msg);
        throw a;
    }
}


/*
 * 带参数且有返回值
 */
JNIEXPORT jstring JNICALL Java_com_sjl_jnidata_JniDataTransfer_testPrimitiveType
        (JNIEnv *env, jclass clz, jbyte b, jint i, jchar c, jdouble d, jstring str) {
    int index = (int) (b + i + c + d);
    LOGI("index=%d", index);
    const char *strContent = env->GetStringUTFChars(str, JNI_FALSE);
    env->ReleaseStringUTFChars(str, strContent);
    return env->NewStringUTF(strContent);
}

/**
 * 获取当前时间
 * @return
 */
long long getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long ts = (long long) tv.tv_sec * 1000 + tv.tv_usec / 1000;//64位，不加在64位系统会溢出，导致时间不准
    return ts;
}


/*
 * 无参数且有返回值
 */
JNIEXPORT jlong JNICALL Java_com_sjl_jnidata_JniDataTransfer_getCurrentTime
        (JNIEnv *env, jclass clz) {
    long long time = getCurrentTime();
    return time;
}

/*
 *用byte[]接收数据
 */
JNIEXPORT jint JNICALL Java_com_sjl_jnidata_JniDataTransfer_testBytes
        (JNIEnv *env, jclass clz, jbyteArray sendData, jbyteArray resultData) {
    jbyte *send = env->GetByteArrayElements(sendData, NULL);
    int nLength = env->GetArrayLength(sendData);//字节数组长度
    jbyte *result = env->GetByteArrayElements(resultData, NULL);
    int nLength2 = env->GetArrayLength(resultData);//字节数组长度
    if (nLength > nLength2) {
        env->ReleaseByteArrayElements(sendData, send, 0);//释放
        env->ReleaseByteArrayElements(resultData, result, 0);//释放
        return -1;
    }
    for (int i = 0; i < nLength; i++) {
        result[i] = send[i];
    }
    env->ReleaseByteArrayElements(sendData, send, 0);//释放
    env->ReleaseByteArrayElements(resultData, result, 0);//释放
    return nLength;
}

/*
 * 传递简单对象
 */
JNIEXPORT void JNICALL Java_com_sjl_jnidata_JniDataTransfer_testObj
        (JNIEnv *env, jclass clz, jobject obj) {
    jclass simple_cls = env->GetObjectClass(obj);

    if (simple_cls == NULL) {
        LOGW("GetObjectClass failed");
        return;
    }
    //获得属性字段
    jfieldID field_a = env->GetFieldID(simple_cls, "a", "Ljava/lang/String;");

    jfieldID field_b = env->GetFieldID(simple_cls, "b", "I");
    //获得属性值
    jstring a = (jstring) env->GetObjectField(obj, field_a);
    jint b = env->GetIntField(obj, field_b);
    const char *c_name = env->GetStringUTFChars(a, NULL);
    //释放引用
    env->ReleaseStringUTFChars(a, c_name);
    LOGI("print simple obj:a=%s,b=%d", c_name, b);
}


/*
 *
 *
 * 传递复杂对象
 */
void JNICALL Java_com_sjl_jnidata_JniDataTransfer_testComplexObj
        (JNIEnv *env, jclass clz, jobject obj) {
    jclass complex_cls = env->GetObjectClass(obj);
    if (complex_cls == NULL) {
        LOGW("GetObjectClass failed");
        return;
    }
    //获得属性字段
    jfieldID field_name = env->GetFieldID(complex_cls, "name", "Ljava/lang/String;");
    jfieldID field_age = env->GetFieldID(complex_cls, "age", "I");
    jfieldID field_label = env->GetFieldID(complex_cls, "label", "Ljava/util/List;");
    jfieldID field_woman = env->GetFieldID(complex_cls, "womanList", "Ljava/util/List;");
    jfieldID field_womanMap = env->GetFieldID(complex_cls, "womanMap", "Ljava/util/Map;");

    //特别注意：如果java传过来对象字段没有赋值，Get***Field是空的

    //捕获异常
    try {
        //获得属性值
        jstring name = (jstring) env->GetObjectField(obj, field_name);
        jint age = env->GetIntField(obj, field_age);
        const char *c_name = env->GetStringUTFChars(name, NULL);
        //释放引用
        env->ReleaseStringUTFChars(name, c_name);

        LOGI("=========print complex obj start:");

        LOGI("name=%s,age=%d", c_name, age);

        //处理label List<String>
        jobject labelObj = (jobject) env->GetObjectField(obj, field_label);
        checkNull(labelObj, "labelObj is null.");
        //获取List class
        jclass labelCls = env->GetObjectClass(labelObj);

        //获取List的MethodID
        jmethodID label_get = env->GetMethodID(labelCls, "get", "(I)Ljava/lang/Object;");
        jmethodID label_size = env->GetMethodID(labelCls, "size", "()I");

        int labelSize = env->CallIntMethod(labelObj, label_size);
        for (int i = 0; i < labelSize; i++) {
            jobject label_obj = env->CallObjectMethod(labelObj, label_get, i);
            jstring str = (jstring) label_obj;

            const char *name = env->GetStringUTFChars(str, NULL);//拿到标签名字
            if (name == NULL) {
                continue;
            }
            env->ReleaseStringUTFChars(str, name);
            LOGI("print label list 第%d个标签：name=%s", i + 1, name);
        }

        //处理woman List<Woman>
        jobject womanObj = env->GetObjectField(obj, field_woman);
        checkNull(womanObj, "womanObj is null.");

        //获取List class
        jclass womanCls = env->GetObjectClass(womanObj);

        //获取List的MethodID
        jmethodID woman_get_id = env->GetMethodID(womanCls, "get", "(I)Ljava/lang/Object;");
        jmethodID woman_size_id = env->GetMethodID(womanCls, "size", "()I");

        int womanSize = env->CallIntMethod(womanObj, woman_size_id);
        for (int i = 0; i < womanSize; i++) {
            //通过get方法获取
            jobject woman_obj = env->CallObjectMethod(womanObj, woman_get_id, i);
            jclass woman_cls = env->GetObjectClass(woman_obj);

            jmethodID nameId = env->GetMethodID(woman_cls, "getName", "()Ljava/lang/String;");
            jstring nameStr = (jstring) env->CallObjectMethod(woman_obj, nameId);
            const char *name = env->GetStringUTFChars(nameStr, NULL);//拿到名字
            if (name == NULL) {
                continue;
            }
            env->ReleaseStringUTFChars(nameStr, name);
            env->DeleteLocalRef(woman_obj);
            env->DeleteLocalRef(nameStr);
            LOGI("print woman list 第%d个女人：name=%s", i + 1, name);
        }

        jobject womanMapObj = env->GetObjectField(obj, field_womanMap);
        checkNull(womanMapObj, "womanMapObj is null.");
        //这样也可以获取
        jclass womanMapCls = env->FindClass("java/util/Map");
        jmethodID methodID_womanMap = env->GetMethodID(womanMapCls, "size", "()I");

        int womanMapSize = env->CallIntMethod(womanMapObj, methodID_womanMap);//map
        //使用iterator遍历
        if (womanMapSize > 0) {
            jmethodID entrySetMID = env->GetMethodID(womanMapCls, "entrySet", "()Ljava/util/Set;");
            jobject setObj = env->CallObjectMethod(womanMapObj, entrySetMID);

            jclass setClass = env->FindClass("java/util/Set");

            jmethodID iteratorMID = env->GetMethodID(setClass, "iterator",
                                                     "()Ljava/util/Iterator;");
            jobject iteratorObj = env->CallObjectMethod(setObj, iteratorMID);
            jclass iteratorClz = env->FindClass("java/util/Iterator");
            jmethodID hasNextMID = env->GetMethodID(iteratorClz, "hasNext", "()Z");
            jmethodID nextMID = env->GetMethodID(iteratorClz, "next", "()Ljava/lang/Object;");
            //内部类使用$符号表示
            jclass entryClass = env->FindClass("java/util/Map$Entry");
            jmethodID getKeyMID = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
            jmethodID getValueMID = env->GetMethodID(entryClass, "getValue",
                                                     "()Ljava/lang/Object;");
            while (env->CallBooleanMethod(iteratorObj, hasNextMID)) {
                jobject entryObj = env->CallObjectMethod(iteratorObj,
                                                         nextMID);//这个对应 Map.Entry<String, Person.Woman>
                jstring key = (jstring) env->CallObjectMethod(entryObj, getKeyMID);
                if (key == NULL) {
                    continue;
                }
                const char *keyStr = env->GetStringUTFChars(key, NULL);

                jobject woman_obj = env->CallObjectMethod(entryObj, getValueMID);
                if (woman_obj == NULL) {
                    continue;
                }
                jclass label_cls = env->GetObjectClass(woman_obj);

                jmethodID nameId = env->GetMethodID(label_cls, "getName", "()Ljava/lang/String;");
                jstring name = (jstring) env->CallObjectMethod(woman_obj, nameId);
                const char *nameStr = env->GetStringUTFChars(name, NULL);//拿到名字
                // 释放UTF字符串资源
                env->ReleaseStringUTFChars(key, keyStr);
                env->ReleaseStringUTFChars(name, nameStr);
                // 释放JNI局部引用资源
                env->DeleteLocalRef(entryObj);
                env->DeleteLocalRef(woman_obj);
                env->DeleteLocalRef(key);
                LOGI("print woman map 我与女人的映射关系：key=%s,name=%s", keyStr, nameStr);
            }
        }
        env->DeleteLocalRef(labelObj);
        env->DeleteLocalRef(womanObj);
        env->DeleteLocalRef(womanMapObj);
        LOGI("=========print complex obj end");
    } catch (NullException err) {
        LOGE("print complex obj error:%s", err.what());
    } catch (exception err) {//其它异常
        LOGE("print complex obj error:%s", err.what());
    }
}

/*
 *返回简单对象
 */
JNIEXPORT jobject JNICALL Java_com_sjl_jnidata_JniDataTransfer_testGetObj
        (JNIEnv *env, jclass clz) {
    jclass simpleClz = env->FindClass("com/sjl/jnidata/Simple");
    //获得构造函数,函数名为 <init> 返回类型必须为 void 即 V
    jmethodID constructorMID = env->GetMethodID(simpleClz, "<init>", "(Ljava/lang/String;I)V");
    char buff[100] = {0};
    char *pos = buff;
    int strLen = sprintf(pos, "%s:", "hello world from c++");
    pos += strLen;
    sprintf(pos, "%lli", getCurrentTime());
    jstring str = env->NewStringUTF(buff);
    jobject object = env->NewObject(simpleClz, constructorMID, str, 123456);
    env->DeleteLocalRef(str);
    return object;
}


/*
 * 返回复杂对象
 */
JNIEXPORT jobject JNICALL Java_com_sjl_jnidata_JniDataTransfer_testGetComplexObj
        (JNIEnv *env, jclass clz) {

    jclass personClz = env->FindClass("com/sjl/jnidata/Person");
    //获得构造函数,函数名为 <init> 返回类型必须为 void 即 V
    //如果不通过构造器赋值，可以选择set赋值
    jmethodID constructorMID = env->GetMethodID(personClz, "<init>",
                                                "(Ljava/lang/String;ILjava/util/List;Ljava/util/List;Ljava/util/Map;)V");
    jstring p_name = env->NewStringUTF("李四");
    jint p_age = 26;


    jclass list_cls = env->FindClass("java/util/ArrayList");

    jmethodID listConstructorMID = env->GetMethodID(list_cls, "<init>", "()V");

    //处理List<Label>
    jobject label_list_obj = env->NewObject(list_cls, listConstructorMID);
    jmethodID label_list_add = env->GetMethodID(list_cls, "add", "(Ljava/lang/Object;)Z");
    env->CallBooleanMethod(label_list_obj, label_list_add, env->NewStringUTF("唱"));
    env->CallBooleanMethod(label_list_obj, label_list_add, env->NewStringUTF("跳"));
    env->CallBooleanMethod(label_list_obj, label_list_add, env->NewStringUTF("rap"));
    env->CallBooleanMethod(label_list_obj, label_list_add, env->NewStringUTF("篮球"));

    //处理List<Woman>
    jobject woman_list_obj = env->NewObject(list_cls, listConstructorMID);
    jmethodID woman_list_add = env->GetMethodID(list_cls, "add", "(Ljava/lang/Object;)Z");

    jclass woman_cls = env->FindClass("com/sjl/jnidata/Person$Woman");//获得Woman类
    jmethodID woman_constructor = env->GetMethodID(woman_cls, "<init>", "(Ljava/lang/String;)V");

    jstring woman1 = env->NewStringUTF("恭喜");
    jobject woman_obj1 = env->NewObject(woman_cls, woman_constructor, woman1);
    env->CallBooleanMethod(woman_list_obj, woman_list_add, woman_obj1);
    jstring woman2 = env->NewStringUTF("发财");
    jobject woman_obj2 = env->NewObject(woman_cls, woman_constructor, woman2);
    env->CallBooleanMethod(woman_list_obj, woman_list_add, woman_obj2);
    //处理Map<String,Woman>
    jclass map_cls = env->FindClass("java/util/HashMap");
    jmethodID mapConstructorMID = env->GetMethodID(map_cls, "<init>", "()V");
    jobject woman_map_obj = env->NewObject(map_cls, mapConstructorMID);
    jmethodID woman_map_put = env->GetMethodID(map_cls, "put",
                                               "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    env->CallObjectMethod(woman_map_obj, woman_map_put, env->NewStringUTF("李四:恭喜"), woman_obj1);
    env->CallObjectMethod(woman_map_obj, woman_map_put, env->NewStringUTF("李四:发财"), woman_obj2);

    jobject object = env->NewObject(personClz, constructorMID, p_name, p_age, label_list_obj,
                                    woman_list_obj, woman_map_obj);
    env->DeleteLocalRef(woman1);
    env->DeleteLocalRef(woman2);
    env->DeleteLocalRef(label_list_obj);
    env->DeleteLocalRef(woman_list_obj);
    env->DeleteLocalRef(woman_map_obj);
    return object;
}

/*
 * C++修改java字段值
 */
JNIEXPORT void JNICALL Java_com_sjl_jnidata_JniDataTransfer_setFieldValue
        (JNIEnv *env, jobject obj) {//如果是静态方法，第二个参数是class
    jclass cls = env->GetObjectClass(obj);
    jfieldID nameFieldId = env->GetFieldID(cls, "name", "Ljava/lang/String;");
    char new_name[] = "C++设置java字段值成功";
    jstring cName = env->NewStringUTF(new_name);
    env->SetObjectField(obj, nameFieldId, cName);//修改对象实例值
}

/*
 * C++回调java
 */
JNIEXPORT void JNICALL Java_com_sjl_jnidata_JniDataTransfer_setFlag
        (JNIEnv *env, jobject obj, jboolean b) {
    jclass cls = env->GetObjectClass(obj);
    jmethodID callSuccessFieldId = env->GetMethodID(cls, "callSuccess", "(Ljava/lang/String;)V");
    jmethodID callErrorFieldId = env->GetMethodID(cls, "callError", "(Ljava/lang/Exception;)V");
    if (b) {
        jstring msg = env->NewStringUTF("来自C++回调，成功");
        //回调成功方法，并传递参数
        env->CallVoidMethod(obj , callSuccessFieldId , msg);
    } else {
        jstring msg = env->NewStringUTF("来自C++回调，异常");
        jclass exceptionClz = env->FindClass("java/lang/Exception");
        jmethodID constructorMID = env->GetMethodID(exceptionClz, "<init>", "(Ljava/lang/String;)V");
        jobject object = env->NewObject(exceptionClz, constructorMID, msg);
        env->CallVoidMethod(obj, callErrorFieldId , object);
    }
}
