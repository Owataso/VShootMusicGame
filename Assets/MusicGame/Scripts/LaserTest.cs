using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LaserTest : MonoBehaviour
{
    LineRenderer laser;

    // Use this for initialization
    void Start ()
    {
        laser = GetComponent<LineRenderer>();
    }
	
	// Update is called once per frame
	void Update ()
    {
        //RaycastHit hit;
        //if (Physics.Raycast(transform.position, transform.forward, out hit))
        //{
        //    laser.SetPosition(1, hit.point);
        //}
        //else
        //{
            laser.SetPosition(1, transform.forward * 100);
        //}

        var noteManager = GameObject.Find("GameMain/NoteManager").GetComponent<NoteManager>();
        for(int i=0;i<noteManager.activeNoteList.Count;i++)
        {
            var intersectPoint = Vector3.zero;
            if(oulMath.LineVsCircle(noteManager.activeNoteList[i].noteObject.GetComponent<SphereCollider>(), transform.position, transform.forward, out intersectPoint))
            {
                laser.SetPosition(1, intersectPoint);
                //Debug.Log("キテルグマ");
                break;
            }
        }
    }
}
