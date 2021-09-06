using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bezier : MonoBehaviour
{
    float currentTime, endTime;
    Vector3[] pointArray;
    public bool isActive, isEnd;

	// Use this for initialization
	void Start ()
    {
        currentTime = endTime = 0;
        isActive = isEnd = false;
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (!isActive) return;

        if(currentTime > endTime)
        {
            currentTime = endTime;
        }

        if(currentTime <= endTime)
        {
            var computedPosision = oulMath.Bezier(pointArray, currentTime / (float)endTime);
            transform.position = computedPosision;

            if (currentTime == endTime)
            {
                isEnd = true;
                isActive = false;
            }
            else currentTime += Time.deltaTime;
        }
	}

    public void Set(Vector3[] pointArray, float endTime)
    {
        //Debug.Log("あ");
        isEnd = false;
        isActive = true;
        currentTime = 0;
        this.pointArray = pointArray;
        this.endTime = endTime;
    }
}
