public class App {
    // This declares that the static `hello` method will be provided
    // a native library.
    private static native String jniCall(String libName, String arg, String constantArg);

    static {
        // This actually loads the shared object that we'll be creating.
        // The actual location of the .so or .dll may differ based on your
        // platform.
        System.out.println("java.library.path=" + System.getProperty("java.library.path"));
        System.loadLibrary("caller");
    }

    // The rest is just regular ol' Java!
    public static void main(String[] args) {
        String output1 = App.jniCall("./f1.so","a","b");
        System.out.println(output1);
        String output2 = App.jniCall("./f2.so","a","b");
        System.out.println(output2);
    }
}
