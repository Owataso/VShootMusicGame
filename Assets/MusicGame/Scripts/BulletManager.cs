using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BulletManager : MonoBehaviour
{
    ObjectPoller objectPoller;

    class BulletData
    {
        public Transform transform;
        public Transform target;
        public Vector3[] bezierPoint = new Vector3[3];
        public float time;
    }
    readonly float bezierTime = 0.15f;

    List<BulletData> bulletList = new List<BulletData>();

	// Use this for initialization
	void Start ()
    {
        objectPoller = GetComponent<ObjectPoller>();
	}
	
	// Update is called once per frame
	void Update ()
    {
		for(int i=0;i<bulletList.Count;i++)
        {

            var bulletTransform = bulletList[i].transform;
            bool endFlag = false;

            // ホーミングフラグ
            if(bulletList[i].target)
            {
                if((bulletList[i].time += Time.deltaTime) >bezierTime)
                {
                    bulletList[i].time = bezierTime;
                    endFlag = true;
                }
                // ベジエで座標制御
                bulletList[i].bezierPoint[2] = bulletList[i].target.localPosition;
                bulletTransform.localPosition = oulMath.Bezier(bulletList[i].bezierPoint, bulletList[i].time / bezierTime);
            }

            else 
            {   // 向いてる方向に前進
                bulletTransform.Translate(0, Time.deltaTime * 250, 0);
                // 弾消去判定(画面外)
                if (bulletTransform.position.sqrMagnitude > 250 * 250) endFlag = true;
            }

            // 非稼働
            if (endFlag)
            {
                bulletList[i].transform.gameObject.SetActive(false);

                // リストから消す
                bulletList.Remove(bulletList[i]);
            }

        }
	}

    public void AddBullet(Vector3 position, Vector3 forward)
    {
        // オブジェクトアクティブ化
        var data = new BulletData();
        var obj = objectPoller.GetPoolObject();

        if (obj)
        {
            data.transform = obj.transform;

            // 位置と回転を初期化
            data.transform.localPosition = position;
            data.transform.LookAt(position + forward);
            var angle = data.transform.localEulerAngles;
            angle.x += 90;
            data.transform.localEulerAngles = angle;

            // ★この関数を呼ばないと線が続きからになってしまう！
            obj.GetComponent<TrailRenderer>().Clear();

            //Debug.Log(obj.transform.forward);

            // リストに追加
            bulletList.Add(data);
        }
    }

    public void AddHomingBullet(Vector3 position, Vector3 forward, Transform target)
    {
        // オブジェクトアクティブ化
        var data = new BulletData();
        var obj = objectPoller.GetPoolObject();

        if (obj)
        {
            data.transform = obj.transform;

            // 位置と回転を初期化
            data.transform.localPosition = position;
            //data.transform.LookAt(position + forward);

            // 目標設定
            data.target = target;
            // ベジエ設定
            data.bezierPoint[0] = position;
            data.bezierPoint[1] = position + forward * 2;
            data.bezierPoint[2] = target.localPosition;

            // ★この関数を呼ばないと線が続きからになってしまう！
            obj.GetComponent<TrailRenderer>().Clear();

            //Debug.Log(obj.transform.forward);

            // リストに追加
            bulletList.Add(data);
        }
    }
}
