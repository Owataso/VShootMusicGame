using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonSelectMusics : ButtonTargetSelect
{
    GameMain gameMain;  // 選曲渡すよう
    SelectMusicWindowManager selectMusicWindowManager;
    public int musicID;

    Vector3 orgPosition;
    public Vector3 selectMovePosition;
    float orgAngle;
    public float selectMoveAngle;
    float selectMoveTime;
    readonly float selectMoveEndTime = 0.25f;

    public bool noMusicChart { private get; set; }  // 譜面なしフラグ

    // 一斉に暗くさせる用
    CanvasGroup canvasGroup;

    enum State
    {
        None,
        Select,
        SelectBack
    }
    State state;

	// Use this for initialization
	protected override void Awake()
    {
        base.Awake();
        // ★rootGameMain
        gameMain = transform.root.GetComponent<GameMain>();
        canvasGroup = GetComponent<CanvasGroup>();

        selectMusicWindowManager = transform.parent.GetComponent<SelectMusicWindowManager>();
        // イベント登録
        eventAction.AddListener(DecideMusic);

        state = State.None;
    }

    protected override void Update()
    {
        base.Update();

        switch (state)
        {
            case State.Select:
                if (selectMoveTime < selectMoveEndTime)
                {
                    selectMoveTime = Mathf.Min(selectMoveTime + Time.deltaTime, selectMoveEndTime);
                    var t = selectMoveTime / selectMoveEndTime;
                    //cashTransform.localPosition = oulMath.LerpVector(orgPosition, selectMovePosition, t);
                    cashTransform.localEulerAngles = new Vector3(0, Mathf.Lerp(orgAngle, selectMoveAngle, t));
                }
                cashTransform.localPosition = oulMath.LerpVector(cashTransform.localPosition, selectMovePosition, 0.25f);
                //cashTransform.localEulerAngles = new Vector3(0, Mathf.Lerp(cashTransform.localEulerAngles.y, selectMoveAngle, 0.25f));
                break;
            case State.SelectBack:
                {
                    selectMoveTime = Mathf.Min(selectMoveTime + Time.deltaTime, selectMoveEndTime);
                    var t = selectMoveTime / selectMoveEndTime;
                    cashTransform.localPosition = oulMath.LerpVector(selectMovePosition, orgPosition, t);
                    cashTransform.localEulerAngles = new Vector3(0, Mathf.Lerp(selectMoveAngle, orgAngle, t));
                    if (t >= 0.9999999f) state = State.None;
                }
                break;
        }

    }

    void DecideMusic()
    {
        Select();
        gameMain.DecideMusic(musicID);
    }

    public void Enable()
    {
        if (state == State.Select)
        {
            state = State.SelectBack;
            selectMoveTime = 0;
        }

        // 曲データなかったら判定なし
        if (noMusicChart) return;
        base.SetEnable(true);
        canvasGroup.alpha = 1.0f;
    }

    public void Disable()
    {
        base.SetEnable(false);
        if (state != State.Select)
        {
            canvasGroup.alpha = 0.05f;
        }
    }

    public void SetPosition(Vector3 position)
    {
        cashTransform.localPosition = position;
        orgPosition = position;
    }

    public void SetAngle(float angle)
    {
        cashTransform.localEulerAngles = new Vector3(0, angle, 0);
        orgAngle = angle;
    }

    public void Select()
    {
        state = State.Select;
        selectMoveTime = 0;
    }
}
