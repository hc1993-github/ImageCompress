package com.hc.image;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.TextView;

import com.hc.image.databinding.ActivityMainBinding;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
    public void compress(View view) {
        File src = new File(Environment.getExternalStorageDirectory(),"src.jpg");
        Bitmap bitmap = BitmapFactory.decodeFile(src.getAbsolutePath());
        File out = new File(Environment.getExternalStorageDirectory(),"new.jpg");
        compress(bitmap,out.getAbsolutePath());
    }
    public native void compress(Bitmap bitmap,String path);
}