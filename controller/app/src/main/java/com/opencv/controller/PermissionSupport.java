package com.opencv.controller;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.util.ArrayList;
import java.util.List;

public class PermissionSupport {
    private String TAG = "openCV:PermissionSupport";
    private String[] permissions = {
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.CAMERA
    };
    private List<String> permissionList;
    private final int MULTIPLE_PERMISSIONS = 1023;


    private Context context;
    private Activity activity;

    public PermissionSupport(Activity _activity, Context _context) {
        this.activity = _activity;
        this.context = _context;
    }

    public void requestPermission() {
        ActivityCompat.requestPermissions(activity,
                permissionList.toArray(new String[permissionList.size()]),
                MULTIPLE_PERMISSIONS);
    }

    public boolean checkPermission() {
        int result;

        permissionList = new ArrayList<>();
        for (String pm : permissions) {
            result = ContextCompat.checkSelfPermission(context, pm);
            if (result != PackageManager.PERMISSION_GRANTED) {
                permissionList.add(pm);
            }
        }

        Log.d(TAG, "permissionCheck : " + permissionList.size());

        if (!permissionList.isEmpty()) {
            return false;
        }

        return true;
    }

    public boolean permissionResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == MULTIPLE_PERMISSIONS && (grantResults.length > 0)) {
            MainActivity mainActivity = new MainActivity();
            for (int i = 0; i < grantResults.length; i++) {
                Log.d(TAG, "result:" + permissions[i] + ", " + grantResults[i]);
                if (permissions[i].equals(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                        && grantResults[i] == PackageManager.PERMISSION_GRANTED) {
                    //mkdirs here lol
                    mainActivity.mkdirs();
                } else if ((permissions[i].equals(Manifest.permission.CAMERA)
                        && grantResults[i] == PackageManager.PERMISSION_GRANTED)) {
                }
                if (grantResults[i] == -1) {
                    return false;
                }
            }
        }
        return true;
    }
}
