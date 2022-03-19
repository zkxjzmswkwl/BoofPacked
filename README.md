Example Java implementation with JNA.

External.java

```java
package software.carter.wilt;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.ptr.PointerByReference;

public interface BoofPack extends Library {
    BoofPack INSTANCE = (BoofPack) Native.load("BoofPacked", BoofPack.class);

    void moveMouseRel(int x, int y);

    boolean asyncKeystate(int key);

    String getActiveWindowTitle();

    // You can pass integers as dwords.
    void readProcMem(String title, /* DWORD */int start_loc, /* DWORD */int out);

    PointerByReference getHWNDByTitle(String windowTitle);

    String getTitleByHWND(PointerByReference hwnd);

    // See example for this
    int hashXXH32(String to_hash);

    // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos
    void setWindowLoc(PointerByReference hwnd, int x, int y, int cx, int cy);

    // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtexta
    void setWindowText(PointerByReference hwnd, String text);

    // https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa
    void spawnProcess(String executablePath);

    // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setfocus
    void setFocusedWindow(PointerByReference hwnd);
}
```

Main.java

```java
import com.sun.jna.ptr.PointerByReference;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        PointerByReference spotifyHwnd = BoofPack.INSTANCE.getHWNDByTitle("Spotify Premium");

        /**
         * As you listen, Spotify's window title changes.
         * You only need to snag the HWND (in Java, PointerByReference) once.
         * This example requires the would-be user to pause Spotify playback before running.
         * After running, they're free to resume playback with the ability to snag the window title
         * until process exit.
        */
        for ( ;; ) {
            System.out.println(BoofPack.INSTANCE.getTitleByHWND(spotifyHwnd));
            Thread.sleep(200);
        }
    }
}
```

What about unsigned integers?
```java
String hashMe = "hash me";
byte[] bytes = ByteBuffer.allocate(4).putInt(BoofPack.INSTANCE.hashXXH32(hashMe)).array();
int unprocessedHash = ByteBuffer.wrap(bytes).getInt();
long processedHash = Integer.toUnsignedLong(hashedStr);
```