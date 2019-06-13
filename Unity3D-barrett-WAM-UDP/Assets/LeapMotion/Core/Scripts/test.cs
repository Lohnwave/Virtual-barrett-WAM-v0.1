using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Leap;
using Leap.Unity;

public class test : MonoBehaviour {
	GameObject hand;
	// Use this for initialization
	void Start () {
		hand = GameObject.Find ("leaphands/Attachment Hands/Attachment Hand (Right)/Palm");

	}
	
	// Update is called once per frame
	void FixedUpdate () {
		
		Vector3 palmposition = hand.transform.position;
		Quaternion palmdirection = hand.transform.rotation;
		this.GetComponent<Transform>().position = palmposition;
		this.GetComponent<Transform>().rotation = palmdirection;
	}
}
