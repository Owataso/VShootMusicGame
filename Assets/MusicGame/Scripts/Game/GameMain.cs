using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameMain : MonoBehaviour
{
    public enum State
    {
        Title,
        TransitionUp,
        SelectMusic,
        Ready,
        Playing,
        Result,
        TransitionDown
    }
    public State state;

    //MusicInfoVRShoot musicInfo;

    public TitleState titleState;

    GameObject transitionObject;                // 遷移エレベーター
    public SelectMusicState selectMusicState;   // 選曲画面
    public ReadyState readyState;
    public PlayingState playingState;           // 総じて演奏部分のオブジェクトを持っている
    public ResultState resultState;             // 総じてリザルト部分のオブジェクトを持っている

    BaseState statePointer;

    MusicData selectMusicData;

    public int selectMusicID;
    public Difficulty selectDifficulty;

    // Use this for initialization
    void Start ()
    {
        // 曲データロード
        MusicDataBase.LoadMusics();

        transitionObject = transform.Find("Transition").gameObject;

        //selectMusicData = MusicDataBase.musicDatas[0];
        //selectDifficulty = Difficulty.Hyper;
        //selectMusicData.difficulty = Difficulty.Hyper;
        //selectMusicData.musicName =
        //    //"ShadowWorld"
        //    //"残酷な天使のテーゼ"
        //    "UnleashSpell"
        //    ;
        //selectMusicData.level = 31;

        //musicInfo = OMT_VRShoot.LoadOMT(Application.dataPath + "/MusicGame/Resources/Musics/" + selectMusicData.musicName + "/" + selectMusicData.difficulty.ToString() + ".omt");

        // 初期ステート設定
        switch (state)
        {
            case State.SelectMusic:
                StartMusicSelect();
                break;

            case State.Playing:
                StartPlaying();
                break;
        }

        // チュートリアルならメインを非アクティブ
        //if (state != State.Playing) playingState.gameObject.SetActive(false);

        //Restart();
    }
	
	// Update is called once per frame
	void Update ()
    {
        if(statePointer.isEnd())
        {
            statePointer.Disable();

            switch (state)
            {
                case State.Title:
                    break;

                case State.SelectMusic:
                    break;

                case State.Playing:
                    {
                        // プレイ→リザルト
                        // リザルトデータ保存
                        ResultData resultData = playingState.scoreManager.resultData;
                        bool isFullCombo = (resultData.maxCombo == playingState.GetNumNote());
                        resultState.SetResultData(resultData, selectMusicData, selectDifficulty, isFullCombo);
                        statePointer = resultState;
                        state = State.Result;
                    }
                    break;

                case State.Result:
                    // イベント関数を呼んで変更する
                    break;
            }

            statePointer.Enable();

        }

        switch (state)
        {
            case State.Title:
                if(titleState.isEnd())
                {
                    titleState.gameObject.SetActive(false);
                    state = State.TransitionUp;
                }
                break;

            case State.Result:
                // イベント関数を呼んで変更する
                break;

            case State.TransitionUp:
                // エレベーター上昇
                //transitionObject.transform.Translate(0, Time.deltaTime * -5, 0);
                transitionObject.GetComponent<Rigidbody>().AddForce(0, -100, 0);
                //// 回転も入れる
                //transitionObject.transform.Rotate(0, Time.deltaTime * 10, 0);

                // 昇りきったら
                if(transitionObject.transform.position.y < -40)
                {
                    state = State.Ready;

                    // エレベーター(背景と地面)を消してレディー用のオブジェクトを表示
                    transitionObject.SetActive(false);
                    readyState.Enable();
                }
                break;
        }


	}

    // 終了時に自動で呼び出す
    void OnApplicationQuit()
    {
        // 曲の記録のセーブ
        MusicDataBase.SaveRecord();
    }

    public void StartPlaying()
    {
        // 現在のステートを終了
        if(statePointer) statePointer.Disable();

        // 曲プレイステートへ
        state = State.Playing;
        statePointer = playingState;
        statePointer.Enable();

        // 曲情報セット
        selectMusicData = MusicDataBase.musicDatas[selectMusicID];
        playingState.SetMusic(selectMusicData, selectDifficulty);
    }

    public void StartMusicSelect()
    {
        // 現在のステートを終了
        if (statePointer) statePointer.Disable();

        // 選曲画面ステートへ
        state = State.Playing;
        statePointer = selectMusicState;
        statePointer.Enable();

        selectMusicState.ChangeDifficulty(selectDifficulty);
    }

    public void ChangeDifficulty(Difficulty dif)
    {
        selectDifficulty = dif;
        selectMusicState.ChangeDifficulty(selectDifficulty);
    }

    public void DecideMusic(int id)
    {
        selectMusicID = id;
        selectMusicState.DecideMusic(id);
    }

    public void DecideOK()
    {
        selectMusicState.DecideOK();

        // メイン開始
        StartPlaying();
    }
    public void DecideCancel()
    {
        selectMusicState.DecideCancel();
    }

    //public void DecideMusic(int id)
    //{
    //    selectMusicState.DecideMusic(no, selectDifficulty);
    //}

    //public void ChangeDifficultyNormal()
    //{
    //    ChangeDifficulty(Difficulty.Normal);
    //}

    //public void ChangeDifficultyHyper()
    //{
    //    ChangeDifficulty(Difficulty.Hyper);
    //}

    //public void ChangeDifficultyExpert()
    //{
    //    ChangeDifficulty(Difficulty.Expert);
    //}

    //public void ChangeDifficultyMaster()
    //{
    //    ChangeDifficulty(Difficulty.Master);
    //}
}
