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
        TextView  tv = new TextView(this);
        tv.setText( stringFromJNI() );
        setContentView(tv);
    }
    private void callFromJni( String msg){
        Log.i(TAG,"123 "+msg);
    }
    public native String stringFromJNI();
    static {
        System.loadLibrary("hello-jni");
    }


}
