using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SoflanManager : MonoBehaviour
{
    SoflanData[] soflans;
    int soflanStep;

    PlayCursor playCursor;              // 曲の再生時間取得用

    Constant constant;           // 定数用

    // Use this for initialization
    void Awake()
    {
        playCursor = transform.parent.Find("PlayCursor").GetComponent<PlayCursor>();
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        // 再生時間取得
        var currentPosition = playCursor.GetCurrentPosition();

        if (soflanStep < soflans.Length - 1)
        {
            int position = (int)((currentPosition - soflans[soflanStep + 1].iEventTime));
            if (position >= 0)
            {
                // カーソルを次に
                soflanStep++;

                playCursor.SetBPM(soflans[soflanStep].fBPM);
            }
        }
    }

    public void Set(MusicInfoVRShoot info)
    {
        // カーソル初期化
        soflanStep = 0;

        // ソフラン領域確保
        soflans = new SoflanData[info.omsInfo.wNumSoflans];

        // ソフラン情報をリストに入れていく
        for (ushort i = 0; i < info.omsInfo.wNumSoflans; i++)
        {
            soflans[i] = new SoflanData();
            soflans[i].iEventTime = info.soflanDatas[i].iEventTime;
            soflans[i].fBPM = info.soflanDatas[i].fBPM;
            soflans[i].fTick = info.soflanDatas[i].fTick;
        }
    }
}
