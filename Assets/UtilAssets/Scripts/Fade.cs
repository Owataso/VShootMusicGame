using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Fade : MonoBehaviour
{
    //===============================================
    //	定数
    //===============================================
    public enum Type
    {
        In,
        Out,

        WhiteIn,
        WhiteOut
    }

    enum FadeState
    {
        NoFade,     // セットすらされていない
        ActiveFade, // フェード中
        EndFade,    // フェードが終わったとき   
    }

    //===============================================
    //	メンバ変数
    //===============================================
    float currentTime = 0, endTime = 0;
    float startAlpha = 0, endAlpha = 1;
    Color fadeColor = new Color(0, 0, 0, 0);
    FadeState fadeState = FadeState.NoFade;
    Type fadeType = Type.In;
    Image image;

	// Use this for initialization
	void Start ()
    {
        image = GetComponent<Image>();
	}
	
	// Update is called once per frame
	void Update ()
    {
        // フェード中なら
        if (fadeState == FadeState.ActiveFade)
        {
            var deltaTime = (Time.timeScale == 0) ? 0.01666f : Time.deltaTime;
            var rate = (currentTime += deltaTime) / endTime;
            fadeColor.a = startAlpha * (1 - rate) + endAlpha * rate;

            // フェードイン
            if (fadeType == Type.In || fadeType == Type.WhiteIn)
            {
                // αを減らしていく
                //fadeColor.a = 1 - rate;

                // 目標フェードまで到達したら終了
                if (fadeColor.a <= endAlpha)
                {
                    fadeColor.a = endAlpha;
                    fadeState = FadeState.EndFade;
                }
            }

            // フェードアウト
            else //if(fadeType == FadeType.Out || fadeType == FadeType.WhiteOut)
            {
                // αを増やしていく
                //fadeColor.a = rate;

                // 目標フェードまで到達したら終了
                if (fadeColor.a >= endAlpha)
                {
                    fadeColor.a = endAlpha;
                    fadeState = FadeState.EndFade;
                }
            }

            // カラー設定
            image.color = fadeColor;
        }
    }

    //===============================================
    //	初期化(作る理由はフェードステートをNoFadeにしたい為)
    //===============================================
    public void reset() // Resetだとオーバーライドしてしまう
    {
        endTime = 0;
        fadeColor = new Color(0, 0, 0, 0);
        fadeState = FadeState.NoFade;
        fadeType = Type.In;
    }

    //===============================================
    //	フェードセッティング
    //===============================================
    public void Set(Type type, float time)
    {
        currentTime = 0;
        endTime = time;
        fadeType = type;
        fadeState = FadeState.ActiveFade;

        switch (type)
        {
            case Type.In:
                fadeColor.r = fadeColor.g = fadeColor.b = 0;
                startAlpha = 1;
                endAlpha = 0;
                break;

            case Type.Out:
                fadeColor.r = fadeColor.g = fadeColor.b = 0;
                startAlpha = 0;
                endAlpha = 1;
                break;

            case Type.WhiteIn:
                fadeColor.r = fadeColor.g = fadeColor.b = 1;
                startAlpha = 1;
                endAlpha = 0;
                break;

            case Type.WhiteOut:
                fadeColor.r = fadeColor.g = fadeColor.b = 1;
                startAlpha = 0;
                endAlpha = 1;
                break;
        }

        fadeColor.a = startAlpha;
    }
    public void Set(Type type, float time, float startAlpha, float endAlpha)
    {
        currentTime = 0;
        endTime = time;
        fadeType = type;
        this.startAlpha = startAlpha;
        this.endAlpha = endAlpha;
        fadeState = FadeState.ActiveFade;

        switch (type)
        {
            case Type.In:
                fadeColor = new Color(0, 0, 0, startAlpha);
                break;

            case Type.Out:
                fadeColor = new Color(0, 0, 0, startAlpha);
                break;

            case Type.WhiteIn:
                fadeColor = new Color(1, 1, 1, startAlpha);
                break;

            case Type.WhiteOut:
                fadeColor = new Color(1, 1, 1, startAlpha);
                break;
        }
    }
    public void Set(Type type, float time, float r, float g, float b)
    {
        currentTime = 0;
        endTime = time;
        fadeType = type;
        fadeState = FadeState.ActiveFade;

        fadeColor.r = r;
        fadeColor.g = g;
        fadeColor.b = b;

        switch (type)
        {
            case Type.In:
            case Type.WhiteIn:
                startAlpha = 1;
                endAlpha = 0;
                break;

            case Type.Out:
            case Type.WhiteOut:
                startAlpha = 0;
                endAlpha = 1;
                break;
        }

        fadeColor.a = startAlpha;
    }
}
