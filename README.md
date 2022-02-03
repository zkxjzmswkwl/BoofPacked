Example Java implementation with JNA.

External.java

```java
package software.carter.wilt;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.ptr.PointerByReference;

public interface BoofPack extends Library {
    BoofPack INSTANCE = (BoofPack) Native.load("BoofPacked", BoofPack.class);

    void move_mouse_rel(int x, int y);

    boolean async_keystate(int key);

    String get_active_window_title();

    // You can pass integers as dwords.
    void read_proc_mem(String title, /* DWORD */int start_loc, /* DWORD */int out);

    PointerByReference get_hwnd_by_title(String windowTitle);

    String get_title_by_hwnd(PointerByReference hwnd);
}
```

Main.java

```java
import com.sun.jna.ptr.PointerByReference;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        PointerByReference spotifyHwnd = BoofPack.INSTANCE.get_hwnd_by_title("Spotify Premium");

        /**
         * As you listen, Spotify's window title changes.
         * You only need to snag the HWND (in Java, PointerByReference) once.
         * This example requires the would-be user to pause Spotify playback before running.
         * After running, they're free to resume playback with the ability to snag the window title
         * until process exit.
        */
        for ( ;; ) {
            System.out.println(BoofPack.INSTANCE.get_title_by_hwnd(spotifyHwnd));
            Thread.sleep(200);
        }
    }
}
```