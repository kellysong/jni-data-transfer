package com.sjl.jnidata;

/**
 * TODO
 *
 * @author Kelly
 * @version 1.0.0
 * @filename JniDataTransfer.java
 * @time 2019/7/8 9:33
 * @copyright(C) 2019 song
 */
public class JniDataTransfer {


    static {
        System.loadLibrary("JniDataTransfer");
    }

    /**
     * 测试基本类型
     *
     * @param b
     * @param i
     * @param c
     * @param d
     * @param str
     * @return
     */
    public static native String testPrimitiveType(byte b, int i, char c, double d, String str);

    /**
     * 获取当前时间戳
     *
     * @return
     */
    public static native long getCurrentTime();

    /**
     * 这个实际用的比较多
     *
     * @param sendData
     * @param resultData
     * @return
     */
    public static native int testBytes(byte[] sendData, byte[] resultData);

    /**
     * 测试传递简单对象
     *
     * @param simple
     * @return
     */
    public static native void testObj(Simple simple);

    /**
     * 测试传递复杂对象
     *
     * @param person
     * @return
     */
    public static native void testComplexObj(Person person);

    /**
     * 测试返回简单对象
     *
     * @return
     */
    public static native Simple testGetObj();

    /**
     * 测试返回复杂对象
     *
     * @return
     */
    public static native Person testGetComplexObj();

    private String name = "java";

    /**
     * C++赋值java变量
     */
    public native void setFieldValue();


    public String getName() {
        return name;
    }

    private JniDataListener jniDataListener;

    /**
     * 回调成功还是异常控制标志
     *
     * @param flag
     */
    public native void setFlag(boolean flag);

    /**
     * C++回调java成功
     */
    public void callSuccess(String msg) {
        if (jniDataListener != null) {
            jniDataListener.onSuccess(msg);
        }
    }

    /**
     * C++回调java异常
     */
    public void callError(Exception e) {
        if (jniDataListener != null) {
            jniDataListener.onError(e);
        }
    }


    public interface JniDataListener {
        void onSuccess(String msg);

        void onError(Exception e);
    }

    public void setJniDataListener(JniDataListener jniDataListener) {
        this.jniDataListener = jniDataListener;
    }
}