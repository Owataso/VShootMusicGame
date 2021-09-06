using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TargetMark : MonoBehaviour
{
    public Transform targetInner, targetOuter;
    public float innerRotate = 0.8f, outerRotate = -1.2f;

	// Update is called once per frame
	void Update ()
    {
        targetOuter.Rotate(0, 0, -1.2f);
	    targetInner.Rotate(0, 0, 0.8f);	
	}
}
