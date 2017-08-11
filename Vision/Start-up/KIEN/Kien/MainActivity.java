package test.espclient;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;


import android.content.Context;
import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends Activity {

    TextView textResponse;
    EditText editTextAddress, editTextPort;
    Button buttonConnect, buttonClear,buttonDiscon , buttonSendMsg;

    EditText welcomeMsg;

    Socket socket;

    boolean socketStatus = false;

    MyClientTask myClientTask;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editTextAddress = (EditText) findViewById(R.id.address);
        editTextPort = (EditText) findViewById(R.id.port);
        buttonConnect = (Button) findViewById(R.id.connect);
        buttonClear = (Button) findViewById(R.id.clear);
        buttonDiscon = (Button) findViewById(R.id.closeSocket);
        buttonSendMsg = (Button) findViewById(R.id.sendMsg);
        textResponse = (TextView) findViewById(R.id.response);

        welcomeMsg = (EditText)findViewById(R.id.welcomemsg);

        buttonConnect.setOnClickListener(buttonConnectOnClickListener);

        buttonDiscon.setOnClickListener(buttonDisconnectOnCLickListener);

        buttonSendMsg.setOnClickListener(sendMessage);

        buttonClear.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                textResponse.setText("");
            }
        });
    }

    public static class SingleToast {

        private static Toast mToast;

        public static void show(Context context, String text, int duration) {
            if (mToast != null) mToast.cancel();
            mToast = Toast.makeText(context, text, duration);
            mToast.show();
        }
    }

    OnClickListener buttonConnectOnClickListener = new OnClickListener() {

        @Override
        public void onClick(View arg0) {
      if(socketStatus)
          Toast.makeText(MainActivity.this,"Already connected!", Toast.LENGTH_LONG).show();
      else {
          socket = null;
          String address = editTextAddress.getText().toString();
          int port = Integer.parseInt(editTextPort.getText().toString());
          String tMsg = welcomeMsg.getText().toString();

          if (address == null || port == 0)
              Toast.makeText(MainActivity.this, "Please enter valid address/port", Toast.LENGTH_LONG).show();

          else {
              Toast.makeText(MainActivity.this,"Connection Success!",Toast.LENGTH_SHORT).show();

              myClientTask = new MyClientTask(address,port,tMsg);
                          myClientTask.execute();

          } //else when no active socket conn. and credentials are validated.


      } //else when already active socket conn.
        }
    };

    OnClickListener buttonDisconnectOnCLickListener = new OnClickListener() {
        @Override
        public void onClick(View v) {
            if (!socketStatus)
                Toast.makeText(MainActivity.this, "SOCKET Already Closed!!", Toast.LENGTH_SHORT).show();
            else {
                try {
                    onDisconnect();
                    if(myClientTask.isCancelled()) {
                        socket.close();
                        Toast.makeText(MainActivity.this, "Socket Closed!", Toast.LENGTH_SHORT).show();
                        socketStatus = false;
                    }
                    else
                    {
                        Toast.makeText(MainActivity.this, "Couldn't Disconnect! Pls try again!", Toast.LENGTH_SHORT).show();
                        socketStatus = true;
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    Toast.makeText(MainActivity.this,e.toString(), Toast.LENGTH_SHORT).show();

                }
            }
        }
    };

    OnClickListener sendMessage = new OnClickListener() {
        @Override
        public void onClick(View v) {
            String msg = welcomeMsg.getText().toString();
            if(msg.equals(""))
            {
                Toast.makeText(MainActivity.this, "Message is empty!!!", Toast.LENGTH_SHORT).show();
            }
            else if(!socketStatus)
            {
                Toast.makeText(MainActivity.this, "Please Establish Socket Connection first!", Toast.LENGTH_SHORT).show();
            }
            else
            {
                sendESP toServer  = new sendESP(msg);
                toServer.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);

            }

        }
    };

    public void onDisconnect()
    {
        myClientTask.cancel(true);
        Log.w("MSG","Async Task cancelled!");
    }

    public class MyClientTask extends AsyncTask<Void, String, Void> {

        String dstAddress;
        int dstPort;
        String response ="";
        String msgToServer;

        MyClientTask(String addr, int port, String msgTo) {
            dstAddress = addr;
            dstPort = port;
            msgToServer = msgTo;
            Log.w("MSG","Entering async task");
        }


        @Override
        protected Void doInBackground(Void... arg0) {


          //  DataOutputStream dataOutputStream = null;
//            DataInputStream dataInputStream = null;

            try {
                socket = new Socket(dstAddress, dstPort);
                socketStatus = true;
            }
            catch (UnknownHostException e)
            {
                // TODO Auto-generated catch block
                e.printStackTrace();
                response = "UnknownHostException: " + e.toString();
                socketStatus = false;
            }
            catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
                response = "IOException: " + e.toString();
            }


            Log.w("MSG","Inside while loop for retrieving data");

            while(!isCancelled() && socketStatus) {
                try {
                    StringBuilder msg = new StringBuilder();
                    int input;
                    BufferedReader br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    while( (input = br.read())!=-1)
                    {
                        if((char)input =='*')
                            break;
                        else
                        msg.append((char)input);
                    }
                    response = msg.toString();

                    if (!response.isEmpty()) {
                        publishProgress(response);
                        Log.w("Data", response);
                    }


                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            try {
                Log.w("MSG","Stopping async task");
                socketStatus = false;
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
                socketStatus = true;
            }
            return null;
        }


        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            textResponse.setText("\nWeight:"+values[0]+" Kg");
          //  Toast.makeText(MainActivity.this,"Server:"+values[0],Toast.LENGTH_LONG).show();
            SingleToast.show(MainActivity.this,"Weight:"+values[0]+ "Kg",Toast.LENGTH_LONG);
            Log.w("MSG","Updating with msg");
        }

        @Override
        protected void onPostExecute(Void result) {
            Log.w("MSG","On postExecute method..");
            textResponse.setText(response);
            super.onPostExecute(result);
        }

    }

    public class sendESP extends AsyncTask<Void, Void, Void> {

        String message;

        sendESP(String msg){
         message = msg;
        }

        @Override
        protected Void doInBackground(Void...params) {

            try {
                Log.w("MSG","Sending message:"+message);
                BufferedWriter toEsp = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
                toEsp.write(message);
                toEsp.flush();
                publishProgress();
               // toEsp.newLine();
            } catch (IOException e) {
                e.printStackTrace();
                Log.w("MSG","Sending failed!");
            }


            return null;
        }

        @Override
        protected void onProgressUpdate(Void... values) {
            super.onProgressUpdate(values);
            Toast.makeText(MainActivity.this,"Message Sent!",Toast.LENGTH_SHORT).show();
        }
    }

}