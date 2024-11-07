package com.example.androidcppapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.example.androidcppapplication.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'androidcppapplication' library on application startup.
    static {
        System.loadLibrary("androidcppapplication");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
        AssetUtils.copyDllsFromAssets(this);
        String filesDirPath = getFilesDir().getAbsolutePath();
        loadMono(filesDirPath);
    }

    /**
     * A native method that is implemented by the 'androidcppapplication' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native void  loadMono(String assemblyPath);
}