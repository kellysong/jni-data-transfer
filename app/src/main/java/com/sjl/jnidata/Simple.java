package com.sjl.jnidata;

/**
 * TODO
 *
 * @author Kelly
 * @version 1.0.0
 * @filename Simple.java
 * @time 2019/7/31 11:32
 * @copyright(C) 2019 song
 */
public class Simple {
    public Simple() {
    }

    /**
     * native层使用
     * @param a
     * @param b
     */
    public Simple(String a, int b) {
        this.a = a;
        this.b = b;
    }

    private String a;
    private int b;

    public String getA() {
        return a;
    }

    public void setA(String a) {
        this.a = a;
    }

    public int getB() {
        return b;
    }

    public void setB(int b) {
        this.b = b;
    }

    @Override
    public String toString() {
        return "Simple{" +
                "a='" + a + '\'' +
                ", b=" + b +
                '}';
    }
}
