using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NoteBullet : MonoBehaviour
{
    //TrailRenderer trail;
    Transform cashTransform;
    Vector3[] bezierPositions = new Vector3[] { new Vector3(0, 0, 30), Vector3.zero, Vector3.zero };
    public float t { private get; set; }

    readonly float curvePower = 10;

    static float adjustX = 1.5f;

    void Awake()
    {
        cashTransform = transform;
        //trail = GetComponent<TrailRenderer>();
        t = 114514;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if (t <= 1)
        {
            // 終端は常にノーツの位置にする
            bezierPositions[2] = cashTransform.parent.localPosition;

            var newPosition = oulMath.Bezier(bezierPositions, t);
            var move = newPosition - cashTransform.position;
            cashTransform.LookAt(cashTransform.position + move);
            cashTransform.position = newPosition;
        }
    }

    public void Action()
    {
        t = 0;

        // 初期座標セット
        cashTransform.position = bezierPositions[0];

        // 軌跡リセット
        //trail.Clear();

        // 終端を撃つポジションにする
        bezierPositions[2] = cashTransform.parent.localPosition;

        // 中間
        var center = (bezierPositions[0] + bezierPositions[2]) / 2;
        if(bezierPositions[2].x == 0)
        {
            center.x = adjustX;
            adjustX = -adjustX;
        }
        center.x *= curvePower;
        center.y = 15;
        bezierPositions[1] = center;

        //Debug.Log(bezierPositions[0].x + "," + bezierPositions[1].x + "," + bezierPositions[2].x);
    }
}
