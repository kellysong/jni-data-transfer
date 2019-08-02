package com.sjl.jnidata;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * jni数据传递测试示例
 *
 * @author song
 */
public class MainActivity extends AppCompatActivity {
    private TextView textView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        textView = findViewById(R.id.tv_content);
    }


    public void testBasic(View view) {
        String hello = JniDataTransfer.testPrimitiveType((byte) 0x1, 2, '3', 4.5, "hello world");
        showMsg(hello);
    }

    private void showMsg(String msg) {
        textView.setText(msg);
    }

    public void getCurrentTime(View view) {
        long currentTime = JniDataTransfer.getCurrentTime();
        showMsg("native时间戳：" + currentTime + ",java时间戳：" + System.currentTimeMillis());
    }

    public void testByteArr(View view) {
        byte[] sendData = new String("hello world").getBytes();
        byte[] resultData = new byte[32];
        int length = JniDataTransfer.testBytes(sendData, resultData);
        if (length > 0) {
            showMsg(new String(resultData, 0, length));
        }
    }

    public void testObj(View view) {
        Simple simple = new Simple();
        simple.setA("李四");
        simple.setB(99);
        JniDataTransfer.testObj(simple);
    }

    /**
     * 测试传递复杂对象
     *
     * @param view
     */
    public void testComplexObj(View view) {
        Person person = new Person();
        person.setName("李四");
        person.setAge(99);

        List<String> label = Arrays.asList("唱", "跳", "rap", "篮球");
        List<Person.Woman> womanList = new ArrayList<>();
        Person.Woman woman1 = new Person.Woman();
        woman1.setName("恭喜");
        Person.Woman woman2 = new Person.Woman();
        woman2.setName("发财");
        womanList.add(woman1);
        womanList.add(woman2);
        Map<String, Person.Woman> womanMap = new HashMap<>();
        for (Person.Woman woman : womanList) {
            womanMap.put(person.getName() + ":" + woman.getName(), woman);
        }

        person.setLabel(label);
        person.setWomanList(womanList);
        person.setWomanMap(womanMap);
        JniDataTransfer.testComplexObj(person);
    }

    public void testGetObj(View view) {
        Simple simple = JniDataTransfer.testGetObj();
        if (simple != null) {
            showMsg(simple.toString());
        }
    }

    public void testGetComplexObj(View view) {
        Person testGetComplexObj = JniDataTransfer.testGetComplexObj();
        if (testGetComplexObj != null) {
            showMsg(testGetComplexObj.toString());
        }
    }


    public void setFieldValue(View view) {
        JniDataTransfer jniDataTransfer = new JniDataTransfer();
        String name1 = jniDataTransfer.getName();
        jniDataTransfer.setFieldValue();
        String name2 = jniDataTransfer.getName();
        showMsg("设置前：" + name1 + "\n设置后：" + name2);
    }

    boolean callbackFlag = true;


    public void callbackJava(View view) {
        JniDataTransfer jniDataTransfer = new JniDataTransfer();
        //设置监听
        jniDataTransfer.setJniDataListener(new JniDataTransfer.JniDataListener() {
            @Override
            public void onSuccess(String msg) {
                showMsg("回调成功:" + msg + "");
                callbackFlag = false;
            }

            @Override
            public void onError(Exception e) {
                showMsg("回调异常:" + e.getMessage());
                callbackFlag = true;
            }
        });

        jniDataTransfer.setFlag(callbackFlag);


    }
}
