package myapplication.com.textlcd;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.EditText;

public class TextLCDActivity extends AppCompatActivity {
    /** Called when the activity is first created. */
    static {
        System.loadLibrary("textlcd");
    } // JNI Library load     
    // JNI Interface library function
    public native int TextLCDOut(String str, String str2);
    public native int IOCtlClear();
    public native int IOCtlReturnHome();
    public native int IOCtlDisplay(boolean bOn);
    public native int IOCtlCursor(boolean bOn);
    public native int IOCtlBlink(boolean bOn);
    int ret;
    boolean disp, cursor, blink;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        disp = true;
        cursor = false;
        blink = false;
        IOCtlClear();
        IOCtlReturnHome();
        IOCtlDisplay(disp);
        IOCtlCursor(cursor);
        IOCtlBlink(blink);
        ret = TextLCDOut("   Chung-Ang    ", "   University   ");
        // Output Text
        final EditText edit1 = (EditText) findViewById(R.id.edit1);
        final EditText edit2 = (EditText) findViewById(R.id.edit2);
        final Button ButWrite = (Button) findViewById(R.id.ButWrite);
        final Button ButClear = (Button) findViewById(R.id.ButClear);
        ButWrite.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                final String str = edit1.getText().toString();
                final String str2 = edit2.getText().toString();
                ret = TextLCDOut(str, str2);
            }
        });
        ButClear.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                IOCtlClear();
            }
        });
    }
}
