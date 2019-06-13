using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class test1 : MonoBehaviour {
    GameObject Joint0;
    GameObject Joint1;
    GameObject Joint2;
    GameObject Joint3;
    GameObject Joint4;
    GameObject Joint5;
    GameObject Joint6;

    int i = 0;


    // Use this for initialization
    void Start () {
        Joint0 = GameObject.Find("NCrobot/Joint0");
        Joint1 = GameObject.Find("NCrobot/Joint0/Joint1");
        Joint2 = GameObject.Find("NCrobot/Joint0/Joint1/Joint2");
        Joint3 = GameObject.Find("NCrobot/Joint0/Joint1/Joint2/Joint3");
        Joint4 = GameObject.Find("NCrobot/Joint0/Joint1/Joint2/Joint3/Joint4");
        Joint5 = GameObject.Find("NCrobot/Joint0/Joint1/Joint2/Joint3/Joint4/Joint5");
        Joint6 = GameObject.Find("NCrobot/Joint0/Joint1/Joint2/Joint3/Joint4/Joint5/Joint6");

    }
	
	// Update is called once per frame
	void FixedUpdate () {
        i = i + 1;
        //Joint0.transform.rotation = Quaternion.Euler(0, 90 - i, 0);
        //Joint1.transform.localEulerAngles = new Vector3(i, -90, 0);
        //Joint1.transform.rotation = Quaternion.Euler(i, -90, 0);
        //Joint2.transform.rotation = Quaternion.Euler(0, -180 + i, 0);
        //Joint3.transform.rotation = Quaternion.Euler(-90 + i, 180, 0);
        //Joint4.transform.rotation = Quaternion.Euler(90 + i, 90, 90);
        //Joint4.transform.localEulerAngles = new Vector3(90+i, 90, 90);
        //Joint5.transform.rotation = Quaternion.Euler(i, 0, 0);
        Joint6.transform.rotation = Quaternion.Euler(0, i, 0);
    }
}
