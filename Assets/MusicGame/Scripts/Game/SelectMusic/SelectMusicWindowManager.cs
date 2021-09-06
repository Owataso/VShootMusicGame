using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SelectMusicWindowManager : MonoBehaviour
{
    public GameObject windowPrefab;

    public int divideAround = 7;
    public float radius = 6;
    public float centerY = 1.5f;

    bool isStart = false;

    Transform cashTransform;
    SelectMusicState selectMusicState;
    MusicData[] musicDatas;
    ButtonSelectMusics[] windows;

    Constant constant;

    // Use this for initialization
    void Start()
    {
        if (isStart) return;

        constant = GameObject.Find("Util").GetComponent<Constant>();

        cashTransform = transform;
        selectMusicState = cashTransform.parent.GetComponent<SelectMusicState>();
        musicDatas = MusicDataBase.musicDatas;
        windows = new ButtonSelectMusics[musicDatas.Length];

        // 曲のデータをもとにウィンドウを作成
        //foreach(MusicData data in datas)
        for (int i = 0; i < musicDatas.Length; i++)
        {
            var obj = Instantiate(windowPrefab, cashTransform).GetComponent<ButtonSelectMusics>();

            obj.musicID = i;

            // 座標と向きを設定
            Vector3 position = new Vector3();
            float rad = i / (float)divideAround;    // 0～1
            rad *= Mathf.PI * 2;
            position.x = Mathf.Sin(rad) * radius;
            position.y = centerY;
            position.z = Mathf.Cos(rad) * radius;
            obj.SetPosition(position);
            obj.SetAngle(rad * Mathf.Rad2Deg);


            //UnityEngine.Events.UnityAction action = 
            // TOD
            //button.eventAction.AddListener(cashTransform.root.GetComponent<GameMain>().StartMusicSelect);
            obj.Appear();

            windows[i] = obj;
        }

        isStart = true;
	}
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    public void Action()
    {
        // 出現
        foreach (ButtonSelectMusics window in windows)
        {
            window.Appear();
        }
    }

    public void ChangeDifficulty(Difficulty dif)
    {
        for (int i = 0; i < windows.Length; i++)
        {
            var data = musicDatas[i].difficulDatas[(int)dif];
            windows[i].cashTransform.Find("ClearLamp").GetComponent<Image>().sprite = constant.GetClearLampSprite((ClearLamp)musicDatas[i].difficulDatas[(int)dif].record.byClearLamp);
            windows[i].cashTransform.Find("DifficultFrame").GetComponent<Image>().color = Constant.difficultyColors[(int)dif];
            windows[i].cashTransform.Find("LvNumber").GetComponent<Text>().text = data.level.ToString();
            windows[i].cashTransform.Find("RankImage").GetComponent<Image>().sprite = constant.GetRankSprite(data.record.hiScore);

            // 曲データがない場合、選択を無効にする
            if (data.level == 0)
            {
                windows[i].noMusicChart = true;
                windows[i].Disable();
            }
            else
            {
                windows[i].noMusicChart = false;
                windows[i].Enable();
            }
        }
    }

    public void DecideMusic(int id)
    {
        // 曲選ぶやつ当たらないように無効化する
        for (int i = 0; i < windows.Length; i++)
        {
            windows[i].Disable();
        }
    }

    public void DecideCancel()
    {
        if (!isStart) Start();

        // 曲選ぶやつ有効化する
        foreach (ButtonSelectMusics window in windows)
        {
            window.Enable();
        }
    }

    public void UpdateMusicDatas(Difficulty dif)
    {
        for (int i = 0; i < windows.Length; i++)
        {
            var trans = windows[i].cashTransform;

            trans.Find("ClearLamp").GetComponent<Image>().sprite = constant.GetClearLampSprite((ClearLamp)musicDatas[i].difficulDatas[(int)dif].record.byClearLamp);
            trans.Find("MusicName").GetComponent<Text>().text = musicDatas[i].musicName;
            trans.Find("ArtistName").GetComponent<Text>().text = musicDatas[i].artistName;
            trans.Find("JacketImage").GetComponent<Image>().sprite = musicDatas[i].jacketSprite;
            trans.Find("DifficultFrame").GetComponent<Image>().color = Constant.difficultyColors[(int)dif];
            trans.Find("LvNumber").GetComponent<Text>().text = musicDatas[i].difficulDatas[(int)dif].level.ToString();
            trans.Find("RankImage").GetComponent<Image>().sprite = constant.GetRankSprite(musicDatas[i].difficulDatas[(int)dif].record.hiScore);
        }
    }
}
