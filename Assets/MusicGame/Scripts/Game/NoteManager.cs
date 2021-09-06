using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NoteManager : MonoBehaviour
{
    /********************************************/
    //	流れてくるノーツの情報
    /********************************************/
    public class PlayNoteInfo
    {
        public NoteDataVRShoot noteData;    // ノーツデータ
        public Note noteObject;             // ゲームオブジェクト
        public int position;                // 位置
        public bool erase;                  // 役目を終えたか
        public ushort soflanNo;             // 自分がどのソフランの位置か
    };

    public GameObject targetMark;
    Transform[] targetMarkers = new Transform[2];

    public List<PlayNoteInfo> activeNoteList { get; private set; }  // 判定とる時に、前面が優先度つくように


    PlayNoteInfo[] notes;               // ノーツリスト
    PlayCursor playCursor;              // 曲の再生時間取得用
    int createCursor;                   // ノーツ創るカーソル
    ObjectPoller objectPoller;          // プーリング用
    JudgeManager judgeManger;           // 判定くん
    NoteBulletManager bulletManager;    // ノーツ弾生成用

    Constant constant;           // 定数用

    // Use this for initialization
    void Awake()
    {
        // ターゲットマーク
        targetMarkers[0] = Instantiate(targetMark, transform).transform;
        targetMarkers[0].gameObject.SetActive(false);
        targetMarkers[1] = Instantiate(targetMark, transform).transform;
        targetMarkers[1].gameObject.SetActive(false);

        activeNoteList = new List<PlayNoteInfo>();
        playCursor = transform.parent.Find("PlayCursor").GetComponent<PlayCursor>();
        judgeManger = transform.parent.Find("JudgeManager").GetComponent<JudgeManager>();
        bulletManager = transform.Find("NoteBulletManager").GetComponent<NoteBulletManager>();
        createCursor = 0;
        objectPoller = GetComponent<ObjectPoller>();
        constant = GameObject.Find("Util").GetComponent<Constant>();
	}
	
	// Update is called once per frame
	void FixedUpdate ()
    {
        // 再生時間取得
        var currentPosition = playCursor.GetCurrentPosition();

        // ノーツ出現チェック
        for (int i = createCursor; i < notes.Length; i++)
        {
            // CNエンドはCN開始に入れているのでスルー
            if ((notes[i].noteData.byNoteType & (byte)NoteType.EndCN) != 0) continue;

            notes[i].position = currentPosition - notes[i].noteData.iEventTime;

            // ノーツ出現チェック
            if (notes[i].position > -constant.msAppearNoteTime)
            {
                var obj = CreateNoteObject(notes[i].noteData);

                // スクリプトをくっつける
                notes[i].noteObject = obj.GetComponent<Note>();

                // CN開始ノーツなら
                if (notes[i].noteData.isNoteType(NoteType.StartCN))
                {
                    var CNEndIndex = notes[i].noteData.wCNEndIndex;
                    var CNObject = CreateNoteObject(notes[CNEndIndex].noteData);
                    // CNエンドにスクリプトをくっつける
                    notes[CNEndIndex].noteObject = CNObject.GetComponent<Note>();
                    notes[CNEndIndex].noteObject.Action(notes[CNEndIndex].noteData.byNoteType, null);
                    // CN開始にCNエンドの実体を渡してやる
                    notes[i].noteObject.CNEndNote = notes[CNEndIndex].noteObject;
                }

                // ノーツ弾生成
                NoteBullet bullet = bulletManager.GetNoteBulletObject((NoteType)notes[i].noteData.byNoteType).GetComponent<NoteBullet>();
                // (CN終了ノーツの処理より後に書く理由はCN開始のActionの時点でCNEndNoteに実体が入っててほしかったから)
                notes[i].noteObject.Action(notes[i].noteData.byNoteType, bullet);

                activeNoteList.Add(notes[i]);

                //// いれたノーツがリストの初回なら、ターゲットマークを出す
                //if(activeNoteList.Count == 1)
                //{
                //    targetMarkers[0].gameObject.SetActive(true);
                //    targetMarkers[0].SetParent(notes[i].noteObject.cashTransform, false);
                //}
                //else if(activeNoteList.Count == 2)
                //{
                //    if (activeNoteList[0].position == activeNoteList[1].position)
                //    {
                //        targetMarkers[1].gameObject.SetActive(true);
                //        targetMarkers[1].SetParent(notes[i].noteObject.cashTransform, false);
                //    }
                //}

		        // 生成チェックスタート地点更新
                createCursor = i + 1;

                continue;
            }

            // 毎回全部チェックしてたらコストかかるので、うにゃ
            break;
        }

        // アクティブなノーツの更新
        for (int i = 0; i < activeNoteList.Count; i++)
        {
            // イベントポジション更新
            int newPosition = currentPosition - activeNoteList[i].noteData.iEventTime;
            //note.position = currentPosition - activeNoteList[i].noteData.iEventTime;
            activeNoteList[i].position = newPosition;
            activeNoteList[i].noteObject.eventPosition = newPosition;
            // CNなら
            if(activeNoteList[i].noteData.isNoteType(NoteType.StartCN))
            {
                // CN終了を計算してやる
                var CNEndIndex = activeNoteList[i].noteData.wCNEndIndex;
                activeNoteList[i].noteObject.CNEndNote.eventPosition = currentPosition - notes[CNEndIndex].noteData.iEventTime;
            }

            // DJAUTO
            if (constant.autoPlay)
            {
                if (newPosition > 0/* && !activeNoteList[i].noteObject.isCNHold*/)
                {
                    JudgeNote(JudgeType.JustCool, i, activeNoteList[i].noteObject.cashTransform.localPosition - transform.forward * 0.75f);

                    continue;
                }
            }
            else if(activeNoteList[i].noteObject.isCNHold)
            {
                if (newPosition > 0/* && !activeNoteList[i].noteObject.isCNHold*/)
                {
                    JudgeNote(JudgeType.JustCool, i, activeNoteList[i].noteObject.cashTransform.localPosition - transform.forward * 0.75f);

                    continue;
                }
            }

            // 見逃しpoor処理
            if(newPosition > constant.badWidth)
            {
                // BAD判定
                LostNote(i, activeNoteList[i].noteObject.cashTransform.localPosition - transform.forward * 0.75f);
            }
        }
	}

    GameObject CreateNoteObject(NoteDataVRShoot noteData)
    {
        // ノーツセット
        var obj = objectPoller.GetPoolObject();
        Debug.Assert(obj, "ノート生成:プール数足りてない");

        // 座標セット
        var radius = constant.noteDist;
        var radian = noteData.fDegree * Mathf.Deg2Rad;

        Vector3 pos;
        pos.x = Mathf.Sin(radian) * radius;
        pos.y = constant.offsetY + noteData.fRateY * constant.verticalWidth;
        pos.z = Mathf.Cos(radian) * radius;
        obj.transform.localPosition = pos;

        //Debug.Log("ノーツ生成: x" + pos.x + ", y" + pos.y + ", z" + pos.z);

        return obj;
    }

    public void Set(MusicInfoVRShoot info)
    {
        // カーソル初期化
        createCursor = 0;

        // アクティブノーツリスト初期化
        foreach (PlayNoteInfo activeNote in activeNoteList) activeNote.noteObject.gameObject.SetActive(false);
        activeNoteList.Clear();

        // ノーツ領域確保
        notes = new PlayNoteInfo[info.omsInfo.wNumNotes];

        // ノーツ情報をリストに入れていく
        for (ushort i = 0; i < info.omsInfo.wNumNotes; i++)
        {
            notes[i] = new PlayNoteInfo();
            notes[i].erase = false;
            notes[i].noteData = info.noteDatas[i];
	        notes[i].noteObject = null;
            notes[i].position = -114514;
        }
    }

    public Note Judge(Vector3 position, Vector3 direction)
    {
        //Debug.Log("キテルグマ");

        // リターンするCNノートのオブジェクト
        Note retCNNote = null;

        // 今出現しているノーツから検索
        for(int i = 0; i < activeNoteList.Count; i++)
        {
            // 判定との距離(再生時間的な意味で)
            int dist = Mathf.Abs(activeNoteList[i].position);

            // Badより速く押してたら判定しなくてもいい
            if (dist > constant.badWidth)
            {
                //Debug.Log("キテナイグマ" + dist);
                break;
            }

            // レーザーと衝突していたら
            Vector3 intersectionPoint = Vector3.zero;
            if (oulMath.LineVsCircle(activeNoteList[i].noteObject.GetComponent<SphereCollider>(),position,direction, out intersectionPoint))
            {
                // CN開始ノーツならそのオブジェクトを返してやる
                if (activeNoteList[i].noteData.isNoteType(NoteType.StartCN))
                {
                    retCNNote = activeNoteList[i].noteObject;
                }

                if (dist < constant.justCoolWidth) JudgeNote(JudgeType.JustCool, i, intersectionPoint);
                else if (dist < constant.coolWidth) JudgeNote(JudgeType.Cool, i, intersectionPoint);
                else if (dist < constant.niceWidth) JudgeNote(JudgeType.Nice, i, intersectionPoint);
                else if (dist <= constant.badWidth) JudgeNote(JudgeType.Bad, i, intersectionPoint);

                // 判定終わったので抜ける
                break;
            }
        }

        return retCNNote;
    }

    void JudgeNote(JudgeType type, int activeNoteIndex, Vector3 position)
    {
        // 判定
        judgeManger.Judge(type, position);

        // CN開始ノーツなら
        if (activeNoteList[activeNoteIndex].noteData.isNoteType(NoteType.StartCN))
        {
            // CNしてなかったらまだ消さない
            if (!activeNoteList[activeNoteIndex].noteObject.isCNHold)
            {
                var CNEndIndex = activeNoteList[activeNoteIndex].noteData.wCNEndIndex;
                // ★イベント時間をCN終端ノーツのイベント時間にする
                activeNoteList[activeNoteIndex].noteData.iEventTime = notes[CNEndIndex].noteData.iEventTime;
                // 再生時間取得
                var currentPosition = playCursor.GetCurrentPosition();
                activeNoteList[activeNoteIndex].noteObject.eventPosition = currentPosition - activeNoteList[activeNoteIndex].noteData.iEventTime;
                //Debug.Log(currentPosition + "," + notes[activeNoteIndex].noteData.iEventTime);
                // CNスタート関数
                activeNoteList[activeNoteIndex].noteObject.CNStartHold();
                return;
            }
            // CN終了オブジェクトさんを非表示に
            activeNoteList[activeNoteIndex].noteObject.isCNHold = false;
            activeNoteList[activeNoteIndex].noteObject.CNEndNote.gameObject.SetActive(false);
            activeNoteList[activeNoteIndex].noteObject.CNEndNote = null;
        }

	    // ノーツ描画オブジェクトさんを非表示に
        activeNoteList[activeNoteIndex].noteObject.noteBullet.gameObject.SetActive(false);
        activeNoteList[activeNoteIndex].noteObject.gameObject.SetActive(false);
        activeNoteList[activeNoteIndex].noteObject = null;

        // リストから消す
        activeNoteList.Remove(activeNoteList[activeNoteIndex]);

        // ターゲットマーカー位置更新
        //UpdateTargetMarker();
    }

    public void JudgeCNNote(Note CNNote)
    {
        for(int i=0;i<activeNoteList.Count;i++)
        {
            if(activeNoteList[i].noteObject == CNNote)
            {
                // 判定との距離(再生時間的な意味で)
                int dist = Mathf.Abs(activeNoteList[i].position);

                var notePosition = CNNote.transform.position;
                if (dist < constant.justCoolWidth) JudgeNote(JudgeType.JustCool, i, notePosition);
                else if (dist < constant.coolWidth) JudgeNote(JudgeType.Cool, i, notePosition);
                else if (dist < constant.niceWidth) JudgeNote(JudgeType.Nice, i, notePosition);
                else JudgeNote(JudgeType.Bad, i, notePosition);
                break;
            }
        }
    }

    void LostNote(int activeNoteIndex, Vector3 position)
    {
        // 判定
        judgeManger.Lost(position);

        // CN開始ノーツなら
        if (activeNoteList[activeNoteIndex].noteData.isNoteType(NoteType.StartCN))
        {
            // CN終了オブジェクトさんを非表示に
            activeNoteList[activeNoteIndex].noteObject.CNEndNote.gameObject.SetActive(false);
            activeNoteList[activeNoteIndex].noteObject.CNEndNote = null;
        }

        // ノーツ描画オブジェクトさんを非表示に
        activeNoteList[activeNoteIndex].noteObject.gameObject.SetActive(false);
        activeNoteList[activeNoteIndex].noteObject = null;

        // リストから消す
        activeNoteList.Remove(activeNoteList[activeNoteIndex]);

        // ターゲットマーカー位置更新
        //UpdateTargetMarker();
    }

    void UpdateTargetMarker()
    {
        if (activeNoteList.Count > 1)
        {
            targetMarkers[0].SetParent(activeNoteList[0].noteObject.cashTransform, false);

            // 同時押し
            if (activeNoteList[0].position == activeNoteList[1].position)
            {
                targetMarkers[1].gameObject.SetActive(true);
                targetMarkers[1].SetParent(activeNoteList[1].noteObject.cashTransform, false);
            }
            else targetMarkers[1].gameObject.SetActive(false);
        }
        else if (activeNoteList.Count == 1)
        {
            targetMarkers[0].SetParent(activeNoteList[0].noteObject.cashTransform, false);
        }
        else
        {
            targetMarkers[0].gameObject.SetActive(false);
            targetMarkers[1].gameObject.SetActive(false);
        }
    }

    public int GetNumNote() { return notes.Length; }
}
