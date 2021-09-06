using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Note : MonoBehaviour
{
    public Transform cashTransform;
    Transform cameraTransform;
   
    public int eventPosition { private get; set; }
    Constant constant;

    public Note CNEndNote;      // CNのゴールノーツの実体(主にCN開始くんが持っている)
    public bool isCNHold;       // CNおさえているかどうか
    Vector3 CNStartPosition;    // CN開始時の座標
    int CNEndEventPosition;     // ホールド時のCN終了のイベントポジション
    LineRenderer lineRenderer;  // CN用の線(プロットはきつそう)

    byte noteType;

    public SpriteRenderer outTarget, middleTarget, inTarget;
    public Sprite normalNoteSprite, chordsSprite, cnSprite;

    public NoteBullet noteBullet;

	// Use this for initialization
	void Awake()
    {
        cashTransform = transform;
        cameraTransform = Camera.main.transform;
        constant = GameObject.Find("Util").GetComponent<Constant>();
        //noteBullet = cashTransform.Find("NoteBullet").GetComponent<NoteBullet>();

        lineRenderer = GetComponent<LineRenderer>();
    }
	
	// Update is called once per frame
	void FixedUpdate ()
    {
        // ビルボード処理
        oulMath.Billboard(ref cashTransform);

        // タイプに応じて更新を分岐
        if ((noteType & (byte)NoteType.StartCN) != 0) CNStartNoteUpdate();
        else if ((noteType & (byte)NoteType.EndCN) != 0) CNEndNoteUpdate();
        else NormalNoteUpdate();
    }

    public void Action(byte byNoteType, NoteBullet bullet)
    {
        // ノーツ弾取得して、子供にする
        noteBullet = bullet;
        noteBullet.transform.parent = cashTransform;

        noteType = byNoteType;

        outTarget.color = new Color(1, 1, 1, 0);
        middleTarget.transform.localScale = new Vector3(3, 3, 3);
        inTarget.color = new Color(1, 1, 1, 0);

        // 同時押しノーツだったら
        if ((noteType & (byte)NoteType.Chords) != 0)
        {
            outTarget.sprite = cnSprite;
        }
        //else chordsSprite.gameObject.SetActive(false);

        // ロングノーツだったら
        if ((noteType & (byte)NoteType.StartCN) != 0)
        {
            lineRenderer.enabled = true;
            // 線の始点は自分の座標
            CNStartPosition = cashTransform.localPosition;
            lineRenderer.SetPosition(0, CNStartPosition);
            // 線の終点はCN終わりの座標
            lineRenderer.SetPosition(1, CNEndNote.cashTransform.localPosition);
            // ロングノーツスプライト
            outTarget.sprite = cnSprite;
            // 弾
            noteBullet.Action();
        }
        else if ((noteType & (byte)NoteType.EndCN) != 0)
        {
            outTarget.sprite = cnSprite;
            lineRenderer.enabled = false;
        }
        else
        {
            outTarget.sprite = normalNoteSprite;
            lineRenderer.enabled = false;

            // 弾
            noteBullet.Action();
        }

        isCNHold = false;
    }

    void NormalNoteUpdate()
    {
        // 割合を出す
        var clampNotePosition = Mathf.Min(eventPosition, 0); //
        var rate0 = Mathf.Abs((float)clampNotePosition / constant.msAppearNoteTime);
        var rate1 = Mathf.Min(rate0, 1);
        var rate2 = Mathf.Min(Mathf.Abs((float)eventPosition / constant.msAppearNoteTime), 1);

        // 弾
        noteBullet.t = 1 - rate0;

        // 透明度設定
        var col = middleTarget.color;
        col.a = 1 - rate2;
        col.a *= col.a;
        col.a = Mathf.Min(1.5f * col.a, 1);
        middleTarget.color = col;

        // 拡大率設定(1なら元の大きさ)
        var scale = 0.4f * (1 - rate1) + 2.0f * rate1;
        middleTarget.transform.localScale = new Vector3(scale, scale, scale);

        // ノーツの透明度を上げていく
        if (outTarget.color.a < 1)
        {
            // rateが0.2の時点で完全に出現
            outTarget.color = new Color(1, 1, 1, 1 - rate1);
            inTarget.color = new Color(1, 1, 1, 1 - rate1);
        }
    }

    public void CNStartHold()
    {
        isCNHold = true;

        CNEndEventPosition = eventPosition;
    }

    void CNStartNoteUpdate()
    {
        // ホールド中
        if(isCNHold)
        {
            // 今のCN終了と開始時のCN終了の時間の差で割合を出す
            var rate = eventPosition / (float)CNEndEventPosition;
            // 開始と終了の座標を割合で移動する
            var newPosition = CNStartPosition * rate + CNEndNote.cashTransform.localPosition * (1 - rate);
            // 座標更新
            cashTransform.localPosition = newPosition;
            lineRenderer.SetPosition(0, newPosition);
            // 弾
            noteBullet.t = 1;
            //Debug.Log(eventPosition + "," + CNEndEventPosition);
            CNEndNote.eventPosition = eventPosition;
        }

        // ホールドじゃない
        else
        {
            NormalNoteUpdate();
        }
    }

    void CNEndNoteUpdate()
    {
        NormalNoteUpdate();
    }
}
