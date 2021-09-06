using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RippleAnim : oul2DAnimBase
{
    public float endTime = 0.5f;

    public float startScale = 1f;

    public float endScale = 2f;

    public float startAlpha = 1;

    public bool useMiddle = false;

    public float middleTime = 0.25f;

    public float middleAlpha = 1;

    // Use this for initialization
    protected override void Awake()
    {
        base.Awake();
	}
	
	// Update is called once per frame
	protected override void Update()
    {
        // アクションフラグがたっていないと返す
        if (!ActionCheck()) return;

        //Debug.Log("キテルグマ");

        // フレーム更新
        // エンドフレームまで来たら終わる
        if ((currentTime += Time.deltaTime) >= endTime)
        {
            actionFlag = false;
            endFlag = true;

            gameObject.SetActive(false);
        }

        // 拡大率更新
        float rate = currentTime / endTime;
        SetScale(Mathf.Lerp(startScale, endScale, rate));

        // アルファ処理
        float alpha;

        if (useMiddle)
        {
            // 中間フレーム以前
            if (currentTime < middleTime)
            {
                rate = currentTime / middleTime;
                alpha = Mathf.Lerp(startAlpha, middleAlpha, rate);
            }
            // 中間フレーム以降
            else
            {
                rate = (currentTime - middleTime) / (endTime - middleTime);
                alpha = (1 - rate) * middleAlpha;
            }
        }
        else alpha = (1f - rate) * startAlpha;

        SetAlpha(alpha);


    }

    public override void Action(float delay = 0f)
    {
        base.Action(delay);

        // 初期化
        SetScale(startScale);

        // ディレイ抜けるまで透明
        //SetAlpha(startAlpha);
        SetAlpha(0);

        if(useMiddle)
        {
            // 応急処置
            if (middleTime >= endTime) middleTime = endTime - 0.01f;
        }
    }
}
