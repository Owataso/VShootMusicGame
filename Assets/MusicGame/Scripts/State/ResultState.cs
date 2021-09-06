using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ResultState : BaseState
{
    public RectTransform infoWindow, judgeWindow, scoreWindow;

    public Text musicName, difficulty, levelNumber, maxComboNumber, justCoolNumber, coolNumber, niceNumber, badNumber, missNumber, scoreNumber;
    public Image jacketImage, rankImage;
    public BaseButton retryButton, nextButton;

    ResultData resultData;
    //Constant constant;

    bool isNewRecord;

    void Awake()
    {
        //constant = GameObject.Find("Util").GetComponent<Constant>();
    }

    public void SetResultData(ResultData resultData, MusicData selectMusicData, Difficulty selectDifficulty, bool isFullCombo)
    {
        var constant = GameObject.Find("Util").GetComponent<Constant>();

        this.resultData = resultData;
        infoWindow.gameObject.SetActive(true);
        judgeWindow.gameObject.SetActive(true);
        scoreWindow.gameObject.SetActive(true);
        retryButton.gameObject.SetActive(true);
        nextButton.gameObject.SetActive(true);

        // テキスト
        musicName.text = selectMusicData.musicName;
        difficulty.color = Constant.difficultyColors[(int)selectDifficulty];
        difficulty.text = selectDifficulty.ToString();
        levelNumber.text = selectMusicData.difficulDatas[(int)selectDifficulty].level.ToString();
        maxComboNumber.text = resultData.maxCombo.ToString();
        justCoolNumber.text = resultData.judgeCounts[(int)JudgeType.JustCool].ToString();
        coolNumber.text = resultData.judgeCounts[(int)JudgeType.Cool].ToString();
        niceNumber.text = resultData.judgeCounts[(int)JudgeType.Nice].ToString();
        badNumber.text = resultData.judgeCounts[(int)JudgeType.Bad].ToString();
        missNumber.text = resultData.judgeCounts[(int)JudgeType.Miss].ToString();
        scoreNumber.text = resultData.score.ToString();
        jacketImage.sprite = selectMusicData.jacketSprite;
        rankImage.sprite = constant.GetRankSprite(resultData.score);

        // ハイスコア更新処理(Autoなら無論スルー)
        if (constant.autoPlay) return;

        var record = selectMusicData.difficulDatas[(int)selectDifficulty].record;
        var isClear = (resultData.score >= 50000);

        // スコア更新
        if (record.hiScore < resultData.score)
        {
            selectMusicData.difficulDatas[(int)selectDifficulty].record.hiScore = resultData.score;
            isNewRecord = MusicDataBase.saveFlag = true;
        }
        else isNewRecord = false;

        // クリアしてたらクリアランプ更新
        if (isClear)
        {
            // フルコン判定
            if (isFullCombo)
            {
                if (record.byClearLamp != (byte)ClearLamp.FullCombo)
                {
                    selectMusicData.difficulDatas[(int)selectDifficulty].record.byClearLamp = (byte)ClearLamp.FullCombo;
                }
            }
            else// if (ModeMgr->GetMode() != PLAY_MODE::GRADE)    // 段位ならクリアランプ更新はなし
            {
                // 前のランプよりランク高かったら更新
                if (record.byClearLamp < (byte)ClearLamp.Clear)
                {
                    selectMusicData.difficulDatas[(int)selectDifficulty].record.byClearLamp = (byte)ClearLamp.Clear;
                }
            }
        }
        else if (record.byClearLamp == (byte)ClearLamp.NoPlay)
        {
            selectMusicData.difficulDatas[(int)selectDifficulty].record.byClearLamp = (byte)ClearLamp.Failed;
        }

        // プレイカウント加算
        selectMusicData.difficulDatas[(int)selectDifficulty].record.playCount++;
        // セーブフラグON
        MusicDataBase.saveFlag = true;
    }

    public override void Enable()
    {
        base.Enable();

        // ボタン
        retryButton.Appear();
        nextButton.Appear();
    }

}
