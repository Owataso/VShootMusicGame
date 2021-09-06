using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class oul2DAnimBase : MonoBehaviour
{
    protected bool actionFlag;    // アクションフラグ	 	 
    protected bool endFlag;    // 終了フラグ
    protected float delayTime;    // ディレイ
    protected SpriteRenderer sprite;

    protected float currentTime;

    protected Transform cashTransform;

    // Use this for initialization
    protected virtual void Awake()
    {
        actionFlag = false;
        endFlag = false;
        delayTime = 0;
        currentTime = 0;
        sprite = GetComponent<SpriteRenderer>();
        if (!sprite) Debug.LogWarning("oul2DAnim: スプライトレンダラ―がない");

        cashTransform = transform;
    }

    // Update is called once per frame
    protected abstract void Update();

    public virtual void Action(float delay)
    {
        //Debug.Log("キテルグマ");

        currentTime = 0;
        delayTime = delay;
        actionFlag = true; /* 実行フラグOn */
        endFlag = false; // エンドフラグ
    }

    public void Stop() { actionFlag = false; }

    protected bool ActionCheck()
    {
        // アクションフラグがたっていないと返す
        if (!actionFlag) return false;

        //Debug.Log("キテルグマ1");

        // ?
        // ディレイタイマーが0になるまで通さない
        if (delayTime > 0)
        {
            delayTime -= Time.deltaTime;
            return false;
        }

        //Debug.Log("キテルグマ2");

        return true; //成功
    }

    protected void SetScale(float scale) { transform.localScale = new Vector3(scale, scale, scale); }
    protected void SetAlpha(float alpha)
    {
        var newColor = sprite.color;
        newColor.a = alpha;
        sprite.color = newColor;
    }
}
