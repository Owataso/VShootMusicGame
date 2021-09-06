using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spell : MonoBehaviour
{
    [Range(-1, 1)]
    public float speed = 0.5f;

    [Range(0, 1)]
    public float alpha = 1;

	// Use this for initialization
	void Start ()
    {
        GetComponent<MeshRenderer>().material.color = new Color(1, 1, 1, alpha);
	}
	
	// Update is called once per frame
	void Update ()
    {
        transform.Rotate(0, speed, 0);
	}
}
