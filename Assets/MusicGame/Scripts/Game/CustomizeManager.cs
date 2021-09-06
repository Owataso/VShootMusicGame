using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CustomizeManager : MonoBehaviour
{
    public enum GunShotSE
    {
        SF,
        Impact
    }
    public GunShotSE selectGunshotSE;

    public enum JudgeSE
    {
        Default,
        Pop
    }
    public JudgeSE selectJudgeSE;

    public enum SelectBGM
    {
        Default
    }
    public SelectBGM selectMusicBGM;

    AudioClip[] audioClipsGunShot = new AudioClip[System.Enum.GetValues(typeof(GunShotSE)).Length];
    AudioClip[] audioClipsJudgeCool = new AudioClip[System.Enum.GetValues(typeof(JudgeSE)).Length];
    AudioClip[] audioClipsJudgeNice = new AudioClip[System.Enum.GetValues(typeof(JudgeSE)).Length];
    AudioClip[] audioClipsJudgeBad = new AudioClip[System.Enum.GetValues(typeof(JudgeSE)).Length];
    AudioClip[] audioClipsSelectBGM = new AudioClip[System.Enum.GetValues(typeof(SelectBGM)).Length];

    // Use this for initialization
    void Awake()
    {
        // 銃のショット音の読み込み
        foreach (GunShotSE i in System.Enum.GetValues(typeof(GunShotSE)))
        {
            audioClipsGunShot[(int)i] = (AudioClip)Resources.Load("Customizes/SE/Shot/" + i.ToString() + "/shot");
        }

        // 判定音の読み込み
        foreach (JudgeSE i in System.Enum.GetValues(typeof(JudgeSE)))
        {
            audioClipsJudgeCool[(int)i] = (AudioClip)Resources.Load("Customizes/SE/Judge/" + i.ToString() + "/cool");
            audioClipsJudgeNice[(int)i] = (AudioClip)Resources.Load("Customizes/SE/Judge/" + i.ToString() + "/nice");
            audioClipsJudgeBad[(int)i] = (AudioClip)Resources.Load("Customizes/SE/Judge/" + i.ToString() + "/bad");
        }

        // セレクト画面BGM読み込み
        foreach (SelectBGM i in System.Enum.GetValues(typeof(SelectBGM)))
        {
            audioClipsSelectBGM[(int)i] = (AudioClip)Resources.Load("Customizes/SelectBGM/" + i.ToString() + "/music");
        }
        //Debug.Log("カスタマイズキテルグマ");
    }

    void Start()
    {
        // 判定SE設定
        oulAudio.SetAudioClip(oulAudio.seID.Cool, GetJudgeCoolAudioClipCool());
        oulAudio.SetAudioClip(oulAudio.seID.Nice, GetJudgeNiceAudioClip());
        oulAudio.SetAudioClip(oulAudio.seID.Bad, GetJudgeBadAudioClip());
    }

	
	// Update is called once per frame
	void Update ()
    {
		
	}

    public AudioClip GetGunShotAudioClip(){ return audioClipsGunShot[(int)selectGunshotSE]; }

    public AudioClip GetJudgeCoolAudioClipCool() { return audioClipsJudgeCool[(int)selectJudgeSE]; }
    public AudioClip GetJudgeNiceAudioClip() { return audioClipsJudgeNice[(int)selectJudgeSE]; }
    public AudioClip GetJudgeBadAudioClip() { return audioClipsJudgeBad[(int)selectJudgeSE]; }
    public AudioClip GetSelectBGMAudioClip() { return audioClipsSelectBGM[(int)selectMusicBGM]; }
}
