package com.example.androidcppapplication;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class AssetUtils {

    // 定义日志标签
    private static final String TAG = "AssetUtils";

    // 拷贝 assets/dll/ 下的所有 DLL 文件到应用的文件目录
    public static void copyDllsFromAssets(Context context) {
        AssetManager assetManager = context.getAssets();

        try {
            // 获取 assets/dll/ 目录下的所有文件名
            String[] files = assetManager.list("dll");
            if (files != null) {
                for (String fileName : files) {
                    if (fileName.endsWith(".dll")) { // 只拷贝 .so 文件
                        copyFileFromAssets(assetManager, context.getFilesDir(), "dll/" + fileName, fileName);
                    }
                }
            }
        } catch (IOException e) {
            Log.e(TAG, "Error reading assets/dll directory", e);
        }
    }

    // 拷贝单个文件从 assets 到目标目录
    private static void copyFileFromAssets(AssetManager assetManager, File outDir, String assetPath, String outputFileName) throws IOException {
        InputStream in = assetManager.open(assetPath);
        File outFile = new File(outDir, outputFileName);

        // 如果目标文件夹不存在，创建它
        if (!outDir.exists()) {
            outDir.mkdirs();
        }

        try (OutputStream out = new FileOutputStream(outFile)) {
            byte[] buffer = new byte[1024];
            int length;
            while ((length = in.read(buffer)) > 0) {
                out.write(buffer, 0, length);
            }
        }

        in.close();

        Log.i(TAG, "File copied to: " + outFile.getAbsolutePath());
    }
}
