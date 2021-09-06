using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectMusicState : BaseState
{
    public SelectMusicBGMManager BGMManager;
    public SelectMusicWindowManager windowManager;
    public SelectMusicInfoWindow infoWindow;
    public BaseButton decideOK, decideCancel;

    public BaseButton[] diffucultyWindows = new BaseButton[Enum.GetValues(typeof(Difficulty)).Length];

    readonly float selectedDifficultyScale = 1, noSelectedDifficultyScale = 0.75f;

    MusicData selectMusicData = null;

    Difficulty difficulty;

    // Use this for initialization
    void Start ()
    {

	}
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    public override void Enable()
    {
        gameObject.SetActive(true);

        // 選択肢ボタン消す
        decideOK.gameObject.SetActive(false);
        decideCancel.gameObject.SetActive(false);
        // 選曲情報ウィンドウ消す
        infoWindow.gameObject.SetActive(false);
        // 曲ウィンドウ再選択
        windowManager.DecideCancel();
        // BGM流す
        BGMManager.PlaySelectMusicBGM(true);

        windowManager.Action();
        windowManager.UpdateMusicDatas(difficulty);

        foreach(BaseButton button in diffucultyWindows)
        {
            button.Appear();
        }
    }

    public override void Disable()
    {
        gameObject.SetActive(false);
    }

    public override bool isEnd()
    {
        return false;
    }

    public void ChangeDifficulty(Difficulty dif)
    {
        difficulty = dif;
        windowManager.ChangeDifficulty(dif);
        if (selectMusicData != null) infoWindow.Set(selectMusicData, dif);

        // 難易度枠の大きさ変更
        foreach (Difficulty i in Enum.GetValues(typeof(Difficulty)))
        {
            float scale = (dif == i) ? selectedDifficultyScale : noSelectedDifficultyScale;
            diffucultyWindows[(int)i].transform.localScale = new Vector3(scale, scale);
        }
    }

    public void DecideMusic(int id)
    {
        //Debug.Log("キテルグマ");
        selectMusicData = MusicDataBase.musicDatas[id];

        // 選曲ウィンドウ管理さんに報告(選曲ウィンドウの銃判定無効化)
        windowManager.DecideMusic(id);

        // 試聴流す
        BGMManager.PlayAudition(MusicDataBase.GetAuditionData(id));

        // 情報ウィンドウ表示
        infoWindow.gameObject.SetActive(true);
        infoWindow.Set(selectMusicData, difficulty);

        // 選択肢ボタン表示
        decideOK.gameObject.SetActive(true);
        decideOK.Appear();
        decideCancel.gameObject.SetActive(true);
        decideCancel.Appear();
    }

    public void DecideOK()
    {
        selectMusicData = null;
    }

    public void DecideCancel()
    {
        selectMusicData = null;
        // 曲ウィンドウ再選択
        windowManager.DecideCancel();
        // 情報ウィンドウ閉じる
        infoWindow.gameObject.SetActive(false);
        // 選択肢ボタン閉じる
        decideOK.Close();
        decideCancel.Close();
        // 選曲画面BGM
        BGMManager.PlaySelectMusicBGM(false);
    }
}
