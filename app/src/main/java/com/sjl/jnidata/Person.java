package com.sjl.jnidata;

import java.util.List;
import java.util.Map;

/**
 * TODO
 *
 * @author Kelly
 * @version 1.0.0
 * @filename Person.java
 * @time 2019/7/31 11:11
 * @copyright(C) 2019 song
 */
public class Person {
    private String name;//姓名
    private int age;//年龄
    private List<String> label;//个性标签
    private List<Woman> womanList;//女人
    private Map<String,Woman> womanMap;//女人映射

    public Person() {
    }

    /***
     * native层使用
     * @param name
     * @param age
     * @param label
     * @param womanList
     * @param womanMap
     */
    public Person(String name, int age, List<String> label, List<Woman> womanList, Map<String, Woman> womanMap) {
        this.name = name;
        this.age = age;
        this.label = label;
        this.womanList = womanList;
        this.womanMap = womanMap;
    }

    public static class Woman {
        private String name;

        public Woman() {
        }

        public Woman(String name) {
            this.name = name;
        }

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        @Override
        public String toString() {
            return "Woman{" +
                    "name='" + name + '\'' +
                    '}';
        }
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public List<String> getLabel() {
        return label;
    }

    public void setLabel(List<String> label) {
        this.label = label;
    }

    public List<Woman> getWomanList() {
        return womanList;
    }

    public void setWomanList(List<Woman> womanList) {
        this.womanList = womanList;
    }

    public Map<String, Woman> getWomanMap() {
        return womanMap;
    }

    public void setWomanMap(Map<String, Woman> womanMap) {
        this.womanMap = womanMap;
    }

    @Override
    public String toString() {
        return "Person{" +
                "name='" + name + '\'' +
                ", age=" + age +
                ", label=" + label +
                ", womanList=" + womanList +
                ", womanMap=" + womanMap +
                '}';
    }
}
