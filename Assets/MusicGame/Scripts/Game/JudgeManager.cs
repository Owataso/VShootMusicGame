using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class JudgeManager : MonoBehaviour
{
    //public GameObject explosion;

    ParticleManager particleManager;
    PanelEffectManager panelManager;
    NumberEffectManager numberEffectManager;
    ScoreManager scoreManager;

    FullComboEffect fullCombo;

    // Use this for initialization
    void Awake()
    {
        var util = GameObject.Find("Util").transform;

        particleManager = util.Find("ParticleManager").GetComponent<ParticleManager>();
        panelManager = util.Find("PanelEffectManager").GetComponent<PanelEffectManager>();
        numberEffectManager = util.Find("NumberEffectManager").GetComponent<NumberEffectManager>();
        scoreManager = transform.parent.Find("ScoreManager").GetComponent<ScoreManager>();

        fullCombo = transform.parent.Find("FullComboEffect").GetComponent<FullComboEffect>();
    }
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    public void Judge(JudgeType type, Vector3 position)
    {
        //Debug.Log("キテルグマ");

        // 判定カウント
        scoreManager.AddJudgeCount(type);

        // フルコン判定
        if (scoreManager.isFullCombo())
        {
            // フルコンエフェクト発動
            fullCombo.gameObject.SetActive(true);
            fullCombo.Action();

            // SE再生
            oulAudio.PlaySE("FullCombo");
        }

        switch (type)
        {
            case JudgeType.JustCool:
                oulAudio.PlaySE(oulAudio.seID.Cool);
                particleManager.AddEffect(ParticleManager.EffectType.JustCool, position);
                // ヒットエフェクト
                panelManager.AddEffect(PanelEffectManager.EffectType.Hit, position);
                // ナンバーエフェクト(コンボ)
                numberEffectManager.AddNumber(position, scoreManager.combo, NumberEffectManager.ColorType.White, NumberEffectManager.NumberEffectType.Jump);
                //var obj = Instantiate(explosion, transform);
                //obj.transform.position = position;
                break;

            case JudgeType.Cool:
                oulAudio.PlaySE(oulAudio.seID.Cool);
                particleManager.AddEffect(ParticleManager.EffectType.Cool, position);
                // ヒットエフェクト
                panelManager.AddEffect(PanelEffectManager.EffectType.Hit, position);
                // ナンバーエフェクト(コンボ)
                numberEffectManager.AddNumber(position, scoreManager.combo, NumberEffectManager.ColorType.White, NumberEffectManager.NumberEffectType.Jump);
                break;

            case JudgeType.Nice:
                oulAudio.PlaySE(oulAudio.seID.Nice);
                particleManager.AddEffect(ParticleManager.EffectType.Nice, position);
                break;

            case JudgeType.Bad:
                oulAudio.PlaySE(oulAudio.seID.Bad);
                particleManager.AddEffect(ParticleManager.EffectType.Bad, position);
                break;

            case JudgeType.Miss:
                particleManager.AddEffect(ParticleManager.EffectType.Miss, position);
                break;
        }


        // 判定カウント後にスコア計算
        //score = ComputeScore();

        //Debug.Log(score);
    }

    public void Lost(Vector3 position)
    {
        particleManager.AddEffect(ParticleManager.EffectType.Miss, position);

        // 判定カウント
        scoreManager.AddJudgeCount(JudgeType.Miss);
    }

    public void Reset()
    {
        fullCombo.gameObject.SetActive(false);
    }
}
