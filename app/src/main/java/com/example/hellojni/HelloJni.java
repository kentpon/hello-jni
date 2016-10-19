package com.example.hellojni;
import android.app.Activity;
import android.util.Log;
import android.widget.TextView;
import android.os.Bundle;
public class HelloJni extends Activity
{
    private final static String TAG = "HelloJni";
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        Log.i(TAG,"onCreate");
        int[] ints = intArrayFromJni();
        Foo foo = fooFromJni();

        TextView tv = new TextView(this);
        String t = stringFromJNI() + "\n" + intArrayToString(ints) + "\n" + foo;
        tv.setText(t);
        setContentView(tv);
    }
    private void callFromJni( String msg) {
        Log.i(TAG,"callFromJni " + msg);
    }
    private boolean intArrayCallFromJni(int[] ints) {
        Log.i(TAG, "intArrayFromJni " + intArrayToString(ints));
        return true;
    }
    private Foo fooCallFromJni(Foo foo) {
        Log.i(TAG, "fooFromJni " + foo);
        foo.num = 999;
        return  foo;
    }
    public native String stringFromJNI();
    public native int[] intArrayFromJni();
    public native Foo fooFromJni();
    static {
        System.loadLibrary("hello-jni");
    }

    public static class Foo {
        public String name;
        public int num;
        public Foo() {
            name = "foo";
            num = -1;
        }
        @Override
        public String toString() {
            return "Foo name:"+name+" num:"+num;
        }
    }

    public String intArrayToString(int[] ints) {
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("[");
        for (int i = 0; i<ints.length; i++) {
            if(i != 0) stringBuilder.append(", ");
            stringBuilder.append(ints[i]);
        }
        stringBuilder.append("]");
        return stringBuilder.toString();
    }
}
