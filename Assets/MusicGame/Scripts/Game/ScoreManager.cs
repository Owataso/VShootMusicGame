using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ScoreManager : MonoBehaviour
{
    Constant constant;
    public Number scoreNumber;
    //Text scoreText, comboText;
    //Text scoreTextWorld, comboTextWorld;

    //public int score { get; private set; }
    //public int combo { get; private set; }
    //public int maxCombo { get; private set; }
    //public int[] judgeCounts { get; private set; }

    public ResultData resultData { get; private set; }
    public int combo;

    int numNote;

    // Use this for initialization
    void Awake()
    {
        var util = GameObject.Find("Util").transform;

        constant = util.GetComponent<Constant>();

        //scoreText = GameObject.Find("Canvas/ScoreText").GetComponent<Text>();
        //comboText = GameObject.Find("Canvas/ComboText").GetComponent<Text>();
        //scoreTextWorld = GameObject.Find("WorldCanvas/ScoreText").GetComponent<Text>();
        //comboTextWorld = GameObject.Find("WorldCanvas/ComboText").GetComponent<Text>();

        resultData = new ResultData();
    }
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    public void AddCombo()
    {

    }
    public void ZeroCombo()
    {
        combo = 0;
    }

    public void AddJudgeCount(JudgeType judgeType)
    {
        // 判定に応じて
        if(judgeType == JudgeType.Bad || judgeType == JudgeType.Miss)
        {
            // コンボ終了
            combo = 0;
        }
        else
        {
            // コンボ継続
            if (++combo > resultData.maxCombo)
            {
                resultData.maxCombo = combo;
            }
        }

        // 判定カウント
        resultData.judgeCounts[(int)judgeType]++;

        // スコア計算
        resultData.score = ComputeScore();
        // 数字更新
        UpdateScoreNumber();
    }

    public void Set(int numNote)
    {
        resultData.Clear();
        combo = 0;
        this.numNote = numNote;
        UpdateScoreNumber();
    }

    void UpdateScoreNumber()
    {
        // 数字画像更新
        scoreNumber.SetNumber(resultData.score);
        //scoreText.text = "Score  : " + score;
        //scoreTextWorld.text = "Score  : " + score;
    }

    int ComputeScore()
    {
        //Debug.Log(judgeCounts[(int)JudgeType.Great]+"," + judgeCounts[(int)JudgeType.Good] + "," + numNote + "," + constant.MaxScore);

        // ポップンラピストリア式スコア計算
        return (int)((
            resultData.judgeCounts[(int)JudgeType.JustCool] * 1 + 
            resultData.judgeCounts[(int)JudgeType.Cool] * 0.7f +
            resultData.judgeCounts[(int)JudgeType.Nice] * 0.4f)
            / numNote * Constant.maxScore);
    }

    public bool isFullCombo() { return (combo == numNote); }
}