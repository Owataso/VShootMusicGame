using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotateAnim : MonoBehaviour
{
    [Range(-15, 15)]
    public float rotateSpeed = 0;

    public bool playOnAwake = true;

    /*
    public float appearTime = 0;        // 出現までにかかる時間

    public float startAlpha = 0, endAlpha = 1;
    public float startScale = 0, endScale = 1;
    */


	// Use this for initialization
	/*protected override void Awake()
    {
        base.Awake();

        if (playOnAwake) Action();
	}
    */

    // Update is called once per frame
    /*protected override */void Update()
    {
        /*
        // アクションフラグがたっていないと返す
        if (!ActionCheck()) return;
        */

        // 回転処理(Z軸)
        transform.Rotate(0, 0, rotateSpeed);

        /*
        // エンドフレームまで来たら終わる
        if (currentTime >= appearTime) return;

        if ((currentTime += Time.deltaTime) >= appearTime)
        {
            currentTime = appearTime;
        }

        float rate = currentTime / appearTime;

        // 拡大率更新
        SetScale(Mathf.Lerp(startScale, endScale, rate));

        // α値更新
        SetAlpha(Mathf.Lerp(startAlpha, endAlpha, rate));
        */
    }

    /*
    public override void Action(float delay = 0)
    {
        base.Action(delay);

        if(appearTime == 0)
        {
            SetScale(endScale);
            SetAlpha(endAlpha);
        }
        else
        {
            SetScale(startScale);
            SetAlpha(startAlpha);
        }
    }
    */
}
