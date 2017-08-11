/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.polkapolka.bluetooth.le;

import android.app.Activity;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.IBinder;
import android.support.design.widget.Snackbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.NumberPicker;
import android.widget.SeekBar;
import android.widget.TextView;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.Scopes;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.Scope;
import com.google.android.gms.fitness.Fitness;
import com.google.android.gms.fitness.data.DataSet;
import com.google.android.gms.fitness.data.Session;
import com.google.android.gms.fitness.request.SessionInsertRequest;
import com.google.android.gms.fitness.request.SessionReadRequest;
import com.google.android.gms.fitness.result.SessionReadResult;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.UUID;
import java.util.concurrent.TimeUnit;

/**
 * For a given BLE device, this Activity provides the user interface to connect, display data,
 * and display GATT services and characteristics supported by the device.  The Activity
 * communicates with {@code BluetoothLeService}, which in turn interacts with the
 * Bluetooth LE API.
 */
public class DeviceControlActivityTemp extends Activity{
    private final static String TAG = DeviceControlActivityTemp.class.getSimpleName();

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";
    private int[] RGBFrame = {0,0,0};
    private TextView isSerial;
    private TextView mConnectionState;
    private TextView mDataField;
    private Button btnSync;
    private Button setAlarm;
    private SeekBar mRed,mGreen,mBlue;
    private String mDeviceName;
    private String mDeviceAddress;
    private NumberPicker bottleAlarm;

    private GoogleApiClient mClient = null;

    public String msg;
    public boolean flag = false, multiple = false;
    public bottleData water = new bottleData();
    public int dataCount =0,count=0;
    public ListView waterLog;

    ArrayList<HashMap<String, String>> rtcLog = new ArrayList<>();
    ArrayList<String> recvdLog = new ArrayList<String>();


    //  private ExpandableListView mGattServicesList;
    private BluetoothLeService mBluetoothLeService;
     private boolean mConnected = false;
    private BluetoothGattCharacteristic characteristicTX;
    private BluetoothGattCharacteristic characteristicRX;


    public final static UUID HM_RX_TX =
            UUID.fromString(SampleGattAttributes.HM_RX_TX);

    private final String LIST_NAME = "NAME";
    private final String LIST_UUID = "UUID";

    // Code to manage Service lifecycle.
    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            mBluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up initialization.
            mBluetoothLeService.connect(mDeviceAddress);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBluetoothLeService = null;
        }
    };

    // Handles various events fired by the Service.
    // ACTION_GATT_CONNECTED: connected to a GATT server.
    // ACTION_GATT_DISCONNECTED: disconnected from a GATT server.
    // ACTION_GATT_SERVICES_DISCOVERED: discovered GATT services.
    // ACTION_DATA_AVAILABLE: received data from the device.  This can be a result of read
    //                        or notification operations.
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                mConnected = true;
                updateConnectionState(R.string.connected);
                invalidateOptionsMenu();
            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                mConnected = false;
                updateConnectionState(R.string.disconnected);
                invalidateOptionsMenu();
                clearUI();
            } else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                // Show all the supported services and characteristics on the user interface.
                displayGattServices(mBluetoothLeService.getSupportedGattServices());
            } else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) {
                displayData(intent.getStringExtra(mBluetoothLeService.EXTRA_DATA));
            }
        }
    };

    private void clearUI() {
        mDataField.setText(R.string.no_data);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.gatt_services_characteristics);

        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        // Sets up UI references.
        ((TextView) findViewById(R.id.device_address)).setText(mDeviceAddress);
         mConnectionState = (TextView) findViewById(R.id.connection_state);
        // is serial present?
        isSerial = (TextView) findViewById(R.id.isSerial);
   
        mDataField = (TextView) findViewById(R.id.data_value);
        mRed = (SeekBar) findViewById(R.id.seekRed);
//        mGreen = (SeekBar) findViewById(R.id.seekGreen);
//        mBlue = (SeekBar) findViewById(R.id.seekBlue);

        btnSync = (Button) findViewById(R.id.btn_sync);
        setAlarm = (Button) findViewById(R.id.btn_Alarm);

        waterLog = (ListView) findViewById(R.id.rtcList);
        bottleAlarm = (NumberPicker) findViewById(R.id.bottleAlarm) ;

        readSeek(mRed,0);
//        readSeek(mGreen,1);
//        readSeek(mBlue,2);
     
        getActionBar().setTitle(mDeviceName);
        getActionBar().setDisplayHomeAsUpEnabled(true);
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);

    }


    @Override
    protected void onResume() {
        super.onResume();
        registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
        if (mBluetoothLeService != null) {
            final boolean result = mBluetoothLeService.connect(mDeviceAddress);
            Log.d(TAG, "Connect request result=" + result);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(mGattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(mServiceConnection);
        mBluetoothLeService = null;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.gatt_services, menu);
        if (mConnected) {
            menu.findItem(R.id.menu_connect).setVisible(false);
            menu.findItem(R.id.menu_disconnect).setVisible(true);
        } else {
            menu.findItem(R.id.menu_connect).setVisible(true);
            menu.findItem(R.id.menu_disconnect).setVisible(false);
        }
        return true;
    }

    public void getRTCdata(View view){
        String str = "*sync#";
        final byte[] tx = str.getBytes();
        if (mConnected) {
            characteristicTX.setValue(tx);
            mBluetoothLeService.writeCharacteristic(characteristicTX);
            mBluetoothLeService.setCharacteristicNotification(characteristicRX, true);
        }
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch(item.getItemId()) {
            case R.id.menu_connect:
                mBluetoothLeService.connect(mDeviceAddress);
                return true;
            case R.id.menu_disconnect:
                mBluetoothLeService.disconnect();
                return true;
            case android.R.id.home:
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void updateConnectionState(final int resourceId) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mConnectionState.setText(resourceId);
            }
        });
    }

    private void displayData(String data) {

        if(!flag)
            msg= "";

        if(data.charAt(0) == '*')
        {
            Log.w("blueData:", "start bit");
            Log.w("blueData:", "len:"+data.length());
            Log.w("blueData:","count:"+count+" datacount:"+dataCount);
            if(count==0)
                recvdLog.clear();


            if(data.charAt((data.length()-1)) == '+')
            {
                Log.w("blueData:", "more bit");
                flag = true;
                msg +=data.substring(1,data.length()-1);
                Log.w("blueData:",msg);
                respond("*");

            }

            if(data.charAt((data.length()-1)) == '#')
            {
                Log.w("blueData:", "stop bit");
                flag = false;
                msg+=data.substring(1,data.length()-1);
                mDataField.setText(msg);
                recvdLog.add(count,getWater(msg));
                count++;
                Log.w("blueData:","count:"+count+" datacount:"+dataCount);
                if(count == dataCount)
                {
                    Log.w("blueData","count reached!");
                    ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(this,R.layout.activity_rtclist,recvdLog);
                    waterLog.setAdapter(arrayAdapter);
                    count =0;
                    dataCount=0;
                }
                getWater(msg);
            }
        }

        else if(data.charAt(0)=='$'){
            Log.w("blueData","data count:"+data.charAt(1));
            dataCount = Character.getNumericValue(data.charAt(1));
            count =0;
        }

        else if(data.charAt(0)=='%')
            if(data.charAt((data.length()-1)) == '#')
                Log.w("blueCmd",data);
        else
            Log.w("blueData", "unknown data");
    }

    // Demonstrates how to iterate through the supported GATT Services/Characteristics.
    // In this sample, we populate the data structure that is bound to the ExpandableListView
    // on the UI.
    private void displayGattServices(List<BluetoothGattService> gattServices) {
        if (gattServices == null) return;
        String uuid = null;
        String unknownServiceString = getResources().getString(R.string.unknown_service);
        ArrayList<HashMap<String, String>> gattServiceData = new ArrayList<HashMap<String, String>>();

 
        // Loops through available GATT Services.
        for (BluetoothGattService gattService : gattServices) {
            HashMap<String, String> currentServiceData = new HashMap<String, String>();
            uuid = gattService.getUuid().toString();
            currentServiceData.put(
                    LIST_NAME, SampleGattAttributes.lookup(uuid, unknownServiceString));
            
            // If the service exists for HM 10 Serial, say so.
            if(SampleGattAttributes.lookup(uuid, unknownServiceString) == "HM 10 Serial") { isSerial.setText("Yes, serial :-)"); } else {  isSerial.setText("No, serial :-("); } 
            currentServiceData.put(LIST_UUID, uuid);
            gattServiceData.add(currentServiceData);

     		// get characteristic when UUID matches RX/TX UUID
    		 characteristicTX = gattService.getCharacteristic(BluetoothLeService.UUID_HM_RX_TX);
    		 characteristicRX = gattService.getCharacteristic(BluetoothLeService.UUID_HM_RX_TX);
        }
        
    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothLeService.ACTION_DATA_AVAILABLE);
        return intentFilter;
    }
    
    private void readSeek(SeekBar seekBar,final int pos) {
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener(){ 
        	@Override 
        	   public void onProgressChanged(SeekBar seekBar, int progress, 
        	     boolean fromUser) { 
        			RGBFrame[pos]=progress;
        		} 

        	   @Override 
        	   public void onStartTrackingTouch(SeekBar seekBar) { 
        	    // TODO Auto-generated method stub 
        	   } 

        	   @Override 
        	   public void onStopTrackingTouch(SeekBar seekBar) { 
        	    // TODO Auto-generated method stub 
              		makeChange();
        	   } 
        });
    }
    // on change of bars write char 
    private void makeChange() {
    	 String str = RGBFrame[0] + "," + RGBFrame[1] + "," + RGBFrame[2] + "\n";
         Log.d(TAG, "Sending result=" + str);
		 final byte[] tx = str.getBytes();
		 if(mConnected) {
		    characteristicTX.setValue(tx);
			mBluetoothLeService.writeCharacteristic(characteristicTX);
			mBluetoothLeService.setCharacteristicNotification(characteristicRX,true);
		 }

    }

    private  void respond(String cmd)
    {
        String msg="null";
        if(cmd == "*")
        {
            msg="*";
            msg += "ack#";
            Log.w("blueData:", "command:"+cmd+"and msg:"+msg);
        }
        final byte [] tx = msg.getBytes();
        if(mConnected)
        {
            characteristicTX.setValue(tx);
            mBluetoothLeService.writeCharacteristic(characteristicTX);
            mBluetoothLeService.setCharacteristicNotification(characteristicTX,true);
        }
    }

    private String getWater(String data)
    {
        int pt = data.indexOf('T');
        String display =Integer.toString(count+1)+')';

        try {
            String date = data.substring(0, pt);
            int oldPt = pt;
            pt = data.indexOf('W');
            String time = data.substring(oldPt+1,pt);
            String newTime= "";

            for(int i =0;i<time.length();i++)
                if(time.charAt(i)=='\\' || time.charAt(i)== '/')
                    newTime+=':';
                else
                    newTime+=time.charAt(i);

//            water.day = Integer.parseInt(date.substring(0,2));
//            water.month = Integer.parseInt(date.substring(3,5));
//            water.year = Integer.parseInt(date.substring(6,pt));
            Log.w("blueData",date+" "+newTime);
            int pos = data.indexOf('/',pt+1);
            String temp ="";
            temp+=data.charAt(pos+1);
            Log.w("blueData","flow:"+temp);
            String flow = Integer.parseInt(temp)==1?"removed":"added";
            display += date+" "+newTime+" "+ data.substring(pt+1,pos)+"ml "+flow+"\n";
            Log.w("rtcData",display);


        }
        catch(Exception e){
            Log.w("blueData", "point"+pt);
            Log.w("exception",e);

        }
        return display;

    }




}