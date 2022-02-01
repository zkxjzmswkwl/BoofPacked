Example Java implementation with JNA.

```
import com.sun.jna.Library;
import com.sun.jna.Native;

public interface External extends Library{
    External INSTANCE = (External)Native.load("BoofPacked", External.class);
    void move_mouse_rel(int x, int y);
    boolean async_keystate(int key);
    String get_active_window_title();
    // You can pass integers as dwords.
    void read_proc_mem(String title, /*DWORD*/int start_loc, /*DWORD*/int out);
}
```

```
public class Main
{
    public static void main(String[] args) throws InterruptedException
    {
        for ( ;; ) {
            System.out.println(External.INSTANCE.get_active_window_title());
            Thread.sleep(20);
        }
    }
}
```