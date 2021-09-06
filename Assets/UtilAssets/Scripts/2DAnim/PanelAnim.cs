using System.Collections;
using System.Collections.Generic;
#if UNITY_EDITOR
using UnityEditor;
#endif
using UnityEngine;

public class PanelAnim : MonoBehaviour
{
    Transform cashTransform;
    Transform cameraTransform;
    float timer = 0;
    void Awake()
    {
        cashTransform = transform;
        cameraTransform = Camera.main.transform;

        // 再生位置初期化
        //GetComponent<Animator>().StartPlayback();
    }

    protected virtual void Update()
    {
        // ビルボード処理
        Vector3 p = cameraTransform.position;
        p.y = cashTransform.position.y;
        cashTransform.LookAt(p);
        // 逆になってたので無理やり修正
        cashTransform.Rotate(0, 180, 0);

        // 時間設定型だったら
        //if (isTimerSet)
        if (timer > 0)
        {
            // 設定した時間に到達したら削除
            if ((timer -= Time.deltaTime) <= 0)
            {
                //Destroy(gameObject);
                gameObject.SetActive(false);
            }
            //Debug.Log("effect:" + timer);
        }

        else
        {
            // アニメーション再生が最後までいったら削除
            if (GetComponent<Animator>().GetCurrentAnimatorStateInfo(0).normalizedTime >= 1)
            {
                //Destroy(gameObject);
                gameObject.SetActive(false);
            }
        }
    }

    // 時間設定
    public void SetTimer(float timer){ this.timer = timer; }

    public virtual void Action(Vector3 position)
    {
        cashTransform.localPosition = position;
        timer = 0;
    }
}
