using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AlphaAnim : oul2DAnimBase
{
    [Range(0, 2)]
    public float endTime = 0.5f;

    [Range(0, 1)]
    public float startAlpha = 1;

    [Range(0, 1)]
    public float endAlpha = 0;

    public bool useMiddle = false;

    [Range(0, 2)]
    public float middleTime = 0.25f;

    [Range(0, 1)]
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

        // フレーム更新
        // エンドフレームまで来たら終わる
        if ((currentTime += Time.deltaTime) >= endTime)
        {
            actionFlag = false;
            endFlag = true;

            gameObject.SetActive(false);
        }

        // アルファ処理
        float rate;
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
                alpha = Mathf.Lerp(middleAlpha, endAlpha, rate);
            }
        }
        else
        {
            rate = currentTime / endTime;
            alpha = Mathf.Lerp(startAlpha, endAlpha, rate);
        }

        SetAlpha(alpha);
    }

    public override void Action(float delay = 0f)
    {
        base.Action(delay);

        SetAlpha(0);

        if (useMiddle)
        {
            // 応急処置
            if (middleTime >= endTime) middleTime = endTime - 0.01f;
        }
    }
}
