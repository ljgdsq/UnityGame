package com.example.androidluaapp;

import android.os.Bundle;

import com.google.android.material.snackbar.Snackbar;

import androidx.appcompat.app.AppCompatActivity;

import android.view.View;

import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

import com.example.androidluaapp.databinding.ActivityGl2JniactivityBinding;

public class GL2JNIActivity extends AppCompatActivity {
    static {
        System.loadLibrary("androidluaapp");
    }
    private GL2JNIView mView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mView=new GL2JNIView(this);

        setContentView(mView);
    }
}