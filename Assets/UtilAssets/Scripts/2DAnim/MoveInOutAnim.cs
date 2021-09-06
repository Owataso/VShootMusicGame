using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveInOutAnim : oul2DAnimBase
{
    [Range(-5, 5)]
    public float moveInOut = 1;            // インとアウトそれぞれの移動距離

    [Range(0.05f, 1)]
    public float timeInOut = 0.25f;    // インとアウトそれぞれに要する時間

    [Range(0, 0.5f)]
    public float moveMiddle = 0.25f;           // 中間地点時の移動距離

    [Range(0.25f, 5)]
    public float timeMiddle = 2.5f;    // 中間地点時に要する時間

    Vector3 /*orgPosition, */startPosition, middleStartPosition, middleEndPosition, endPosition;         // 元の位置保存用

    enum State
    {
        FadeIn, Middle, FadeOut, End
    }
    State state;

	// Use this for initialization
	protected override void Awake ()
    {
        base.Awake();

        cashTransform = transform;

        /*orgPosition = */
        startPosition = middleStartPosition = middleEndPosition = endPosition = cashTransform.localPosition;

        Vector3 move = cashTransform.right * moveInOut;

        // 開始と終了座標
        startPosition += -move;
        endPosition += move;

        // 中間の開始と終了

        // まず真ん中に
        middleStartPosition = middleEndPosition = (startPosition + endPosition) * 0.5f;

        // そこから移動する
        var add = moveInOut / Mathf.Abs(moveInOut) * moveMiddle;
        middleStartPosition += cashTransform.right * -add;
        middleEndPosition += cashTransform.right * add;

        state = State.FadeIn;
	}
	
	// Update is called once per frame
	protected override void Update ()
    {
        // アクションフラグがたっていないと返す
        if (!ActionCheck()) return;

        var newPosition = cashTransform.localPosition;

        switch (state)
        {
            case State.FadeIn:
                // 座標更新処理
                newPosition = Vector3.Lerp(startPosition, middleStartPosition, currentTime / timeInOut);

                // アルファ値更新処理
                SetAlpha(currentTime / timeInOut);

                // イン終了の時間まで経過したら中間ステートへ
                if ((currentTime += Time.deltaTime) >= timeInOut)
                {
                    currentTime = 0;
                    state = State.Middle;
                }
                break;

            case State.Middle:
                // 座標更新処理
                newPosition = Vector3.Lerp(middleStartPosition, middleEndPosition, currentTime / timeMiddle);

                // 中間終了の時間まで経過したらフェードアウトステートへ
                if ((currentTime += Time.deltaTime) >= timeMiddle)
                {
                    currentTime = 0;
                    state = State.FadeOut;
                }
                break;

            case State.FadeOut:
                // 座標更新処理
                newPosition = Vector3.Lerp(middleEndPosition, endPosition, currentTime / timeInOut);

                // アルファ値更新処理
                SetAlpha(1 - (currentTime / timeInOut));

                // 終了の時間まで経過したら終了
                if ((currentTime += Time.deltaTime) >= timeInOut)
                {
                    currentTime = 0;
                    state = State.End;

                    gameObject.SetActive(false);
                }
                break;
        }

        cashTransform.localPosition = newPosition;
    }

    public override void Action(float delay)
    {
        state = State.FadeIn;

        // 座標を開始位置に
        cashTransform.localPosition = startPosition;

        // 完全に透明な状態から
        SetAlpha(0);

        base.Action(delay);
    }
}
