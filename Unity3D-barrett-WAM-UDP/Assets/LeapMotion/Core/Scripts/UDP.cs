using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System.Net.Sockets;
using System.Text;
using System.Net;
using System.Threading;
using System;

public class UDP : MonoBehaviour {
	GameObject Joint0;
	GameObject Joint1;
	GameObject Joint2;
	GameObject Joint3;
	GameObject Joint4;
	GameObject Joint5;
	GameObject Joint6;


	string recvstr;
	string UDPclientIP;
	UdpClient udpclient;
	IPEndPoint endpoint;
	Socket socket;
	EndPoint ep;

	int recvlen=0;
	string[] data = new string[7]; //存放关节角
	double[] joint=new double[7];

	Thread receivedata;

    private System.Object thislock = new System.Object();

    void InitSocket()
	{
		endpoint = new IPEndPoint (IPAddress.Parse (UDPclientIP), 8866);
		socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
		socket.Bind (endpoint);
		ep = (EndPoint)endpoint;
		print("等待连接");
		//专门用于接收数据的线程
		receivedata = new Thread (new ThreadStart (receive));
		receivedata.Start ();
	}

	void receive()
	{
		while(true)
		{
			byte[] recvbuf=new byte[100];
			recvlen=socket.ReceiveFrom(recvbuf, ref ep);
			recvstr=Encoding.UTF8.GetString(recvbuf,0,recvlen);
			Debug.Log("收到的信息 " + recvstr);
            //从接收的数据提取出关节角度
            // 
            lock (thislock) {
                data = recvstr.Split(',');
            //    for (int i = 0; i < 7; i++)
            //{
            //    joint[i] = System.Convert.ToDouble(data[i]) * 57.29578;
            //}
            }
            
        }
	}

    void socketquit()
	{
		//关闭线程
		if (receivedata != null) 
		{
			receivedata.Interrupt ();
			receivedata.Abort ();
		}
		//关闭socker
		if(socket!=null)
		{
			socket.Close ();
			Debug.LogWarning ("断开连接");
		}
	}

    // Use this for initialization
    void Start () {
		joint [0] = 0;
		joint [1] = 0;
		joint [2] = 0;
		joint [3] = 0;
		joint [4] = 0;
		joint [5] = 0;
		joint [6] = 0;
		Joint0 = GameObject.Find ("NCrobot/Joint0");
		Joint1 = GameObject.Find ("NCrobot/Joint0/Joint1");
		Joint2 = GameObject.Find ("NCrobot/Joint0/Joint1/Joint2");
		Joint3 = GameObject.Find ("NCrobot/Joint0/Joint1/Joint2/Joint3");
		Joint4 = GameObject.Find ("NCrobot/Joint0/Joint1/Joint2/Joint3/Joint4");
		Joint5 = GameObject.Find ("NCrobot/Joint0/Joint1/Joint2/Joint3/Joint4/Joint5");
		Joint6 = GameObject.Find ("NCrobot/Joint0/Joint1/Joint2/Joint3/Joint4/Joint5/Joint6");

		UDPclientIP = "127.0.0.1";
		UDPclientIP = UDPclientIP.Trim();       
		InitSocket();
	}

	// Update is called once per frame
	void Update () {
        //Joint0.transform.rotation = Quaternion.Euler(0, 90 - (float)joint[0], 0);
        //Joint1.transform.rotation = Quaternion.Euler((float)joint[1], -90, 0);
        //Joint2.transform.rotation = Quaternion.Euler(0, -180 + (float)joint[2], 0);
        //Joint3.transform.rotation = Quaternion.Euler(-90 + (float)joint[3], 180, 0);
        //Joint4.transform.rotation = Quaternion.Euler(90 + (float)joint[4], 90, 90);
        //Joint5.transform.rotation = Quaternion.Euler((float)joint[5], 0, 0);
        //Joint6.transform.rotation = Quaternion.Euler(0, (float)joint[6], 0);

        // lock()
        // System.Threading.Interlocked
        lock (thislock) {
            for (int i = 0; i < 7; i++)
            {
                joint[i] = System.Convert.ToDouble(data[i]) * 57.29578;
            }
        }
        // freelock()
        Joint0.transform.localEulerAngles = new Vector3(0, 90 - (float)joint[0], 0);
        Joint1.transform.localEulerAngles = new Vector3((float)joint[1], -90, 0);
        Joint2.transform.localEulerAngles = new Vector3(0, -180 + (float)joint[2], 0);
        Joint3.transform.localEulerAngles = new Vector3(-90 + (float)joint[3], 180, 0);
        Joint4.transform.localEulerAngles = new Vector3(90 + (float)joint[4], 90, 90);
        Joint5.transform.localEulerAngles = new Vector3((float)joint[5], 0, 0);
        Joint6.transform.localEulerAngles = new Vector3(0, (float)joint[6], 0);
    }

    void SocketQuit()
	{
		//关闭线程
		if (receivedata != null)
		{
			receivedata.Interrupt();
			receivedata.Abort();
		}
		//最后关闭socket
		if (socket != null)
			socket.Close();
		Debug.LogWarning("断开连接");
	}
		

	void OnApplicationQuit()
	{
		SocketQuit();
	}

}
