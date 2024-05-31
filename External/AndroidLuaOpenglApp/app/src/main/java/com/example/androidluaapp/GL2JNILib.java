package com.example.androidluaapp;

public class GL2JNILib {



    /**
     * @param width the current view width
     * @param height the current view height
     */
    public static native void init(int width, int height);
    public static native void step();
}
