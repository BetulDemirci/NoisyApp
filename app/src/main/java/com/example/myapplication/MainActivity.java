package com.example.myapplication;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import static android.content.ContentValues.TAG;

public class MainActivity extends Activity
        implements ActivityCompat.OnRequestPermissionsResultCallback {

    String[] appPermissions = {
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE
    };
    private static final int PERMISSON_CODE = 1240;
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("rnnoise_demo");
    }

    static AssetManager AssetManager;
    private Button record;
    static boolean isPlayingAsset = false;



    @TargetApi(17)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if(checkAndRequesttPermission()){

            initApp();
        }

    }



    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */


    public static native boolean rnnoise_demo(AssetManager inputF, String inputFName);

    public void initApp(){
        AssetManager = getAssets();

        //final String filePath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)+"/input.pcm";
        final String filePath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)+"/";
        Log.i("FILE_PATH", filePath);

        boolean what = rnnoise_demo(AssetManager, filePath);

        Toast.makeText(MainActivity.this, "RESULT : "+what, Toast.LENGTH_SHORT).show();

   }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if(requestCode == PERMISSON_CODE){

            HashMap<String, Integer> permissionResult = new HashMap<>();
            int deniedCount = 0;

            for(int i=0; i<grantResults.length; i++){
                if(grantResults[i] == PackageManager.PERMISSION_DENIED){
                    permissionResult.put(permissions[i], grantResults[i]);
                    deniedCount++;
                }
            }

            if(deniedCount == 0){
                initApp();
            }
        }
    }


   private boolean checkAndRequesttPermission(){

       List<String> listPermissions = new ArrayList<>();
       for(String perm : appPermissions){
           if(ContextCompat.checkSelfPermission(this, perm) != PackageManager.PERMISSION_GRANTED){
               listPermissions.add(perm);
           }
       }

       if(!listPermissions.isEmpty()){
           ActivityCompat.requestPermissions(this,
               listPermissions.toArray(new String[listPermissions.size()]),
               PERMISSON_CODE
           );
           return false;
       }
       return true;
    }
}
