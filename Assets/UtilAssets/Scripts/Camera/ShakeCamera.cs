using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShakeCamera : MonoBehaviour
{
    [SerializeField]
    int shakeFrame;

    [SerializeField]
    float shakePower;

    int frame;

    // Use this for initialization
    void Start()
    {
        frame = 0;
    }

    // Update is called once per frame
    void Update()
    {
        if (frame > 0)
        {
            frame--;

            Vector2 shakedPos = /*transform.position*/ Vector2.zero;
            float power = shakePower * (frame / (float)shakeFrame);
            //Debug.Log(power);
            float x = ((Random.value - 0.5f) * 2) * power, y = ((Random.value - 0.5f) * 2) * power;
            shakedPos.x += x;
            shakedPos.y += y;
            //Debug.Log("x:" + x + "y:" + y);
            //Debug.Log("sx:" + shakedPos.x + "y:" + shakedPos.y);
            transform.position = new Vector3(shakedPos.x, shakedPos.y, transform.position.z);
        }
    }

    public void Set()
    {
        frame = shakeFrame;
    }
}
