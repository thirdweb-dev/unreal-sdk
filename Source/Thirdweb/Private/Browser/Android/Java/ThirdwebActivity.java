package com.thirdweb.unrealengine;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.browser.customtabs.CustomTabsCallback;

import com.epicgames.unreal.GameActivity;

public class ThirdwebActivity extends Activity {
    private static final String EXTRA_URI = "extra_uri";
    private static final String EXTRA_INTENT_LAUNCHED = "extra_intent_launched";

    private boolean customTabsLaunched = false;
    private CustomTabsController customTabsController;

    public static void startActivity(Activity context, String url) {
        Intent intent = new Intent(context, ThirdwebActivity.class);
        intent.putExtra(EXTRA_URI, Uri.parse(url));
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        context.startActivity(intent);
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (savedInstanceState != null) {
            customTabsLaunched = savedInstanceState.getBoolean(EXTRA_INTENT_LAUNCHED, false);
        }
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Intent resultData = resultCode == RESULT_CANCELED ? new Intent() : data;
        onDeeplinkResult(resultData);
        finish();
    }

    @Override
    protected void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putBoolean(EXTRA_INTENT_LAUNCHED, customTabsLaunched);
    }

    @Override
    protected void onResume() {
        super.onResume();
        Intent authenticationIntent = getIntent();
        if (!customTabsLaunched && authenticationIntent.getExtras() == null) {
            // This activity was launched in an unexpected way
            finish();
            return;
        } else if (!customTabsLaunched) {
            // Haven't launched custom tabs
            customTabsLaunched = true;
            launchCustomTabs();
            return;
        }
        onDeeplinkResult(authenticationIntent);
        finish();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (customTabsController != null) {
            customTabsController.unbindService();
            customTabsController = null;
        }
    }
    
    public native void handleDeepLink(String Deeplink);
    
    public native void handleOnCustomTabsDismissed(String Url);

    private void launchCustomTabs() {
        Bundle extras = getIntent().getExtras();
        final Uri uri = extras.getParcelable(EXTRA_URI);
        customTabsController = new CustomTabsController(this, new CustomTabsCallback() {
            @Override
            public void onNavigationEvent(int navigationEvent, @Nullable Bundle extras) {
                if (navigationEvent == CustomTabsCallback.TAB_HIDDEN/*  && callbackInstance != null */) {
                    // Adding some delay before calling onCustomTabsDismissed as sometimes this gets called 
                    // before the PKCE deeplink is triggered (by 100ms). This means PKCEResponseDelegate will be 
                    // set to null before the SDK can use it to notify the consumer of the PKCE result.
                    final Handler handler = new Handler(Looper.getMainLooper());
                    handler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            GameActivity.onCustomTabsDismissed(uri.toString());
                        }
                    }, 1000);
                }
            }
        });
        customTabsController.bindService();
        customTabsController.launch(uri);
    }

    private void onDeeplinkResult(@Nullable Intent intent) {
        if (intent != null && intent.getData() != null) {
            GameActivity.onDeeplinkResult(intent.getData().toString());
        }
    }

    public interface Callback {
        void onCustomTabsDismissed(String url);
        void onDeeplinkResult(String url);
    }
}