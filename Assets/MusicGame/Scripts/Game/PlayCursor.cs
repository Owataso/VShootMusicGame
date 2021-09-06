using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayCursor : MonoBehaviour
{
    AudioSource audioSource;

    public float currentBPM { get; private set; }        // 現在のBPM

    int currentPlayTime;     // 現在の再生カーソル(デルタタイムの計測で使う)
    int currentBeatMSecond;  // 現在の()
    int beatMSecond;		 // 何ミリ秒毎に拍を打つか

    void Awake()
    {
        audioSource = GetComponent<AudioSource>();
        currentBPM = 60;
    }

    void Update()
    {
        if (!isPlay()) return;

        var currentPosition = GetCurrentPosition();
        var deltaTime = currentPosition - currentPlayTime;

        // もしかしたら終端かもしれないので止める
        if (deltaTime < 0)
        {
            return;
        }
        currentPlayTime = currentPosition;
        currentBeatMSecond += deltaTime;
    }

    public void Play()
    {
        // 時間初期化
        currentBeatMSecond = currentPlayTime = 0;

        //oulAudio.PlayBGM("Main", false);
        audioSource.Play();
    }

    public int GetCurrentPosition()
    {
        //return oulAudio.GetMSecondBGM();
        return (int)(audioSource.time * 1000);
    }

    public void Set(/*string fileName*/ AudioClip clip)
    {
        //oulAudio.LoadBGM("Main", fileName);
        audioSource.clip = clip;
    }

    public bool isPlay()
    {
        //return oulAudio.isPlayBGM();
        return audioSource.isPlaying;
    }

    // BPMセット(ソフランさんから呼び出す)
    public void SetBPM(float bpm)
    {
        // BPM
        currentBPM = bpm;

        // 現在の拍計測ミリ秒を初期化
        currentBeatMSecond = 0;

        // BPMの値から、何ミリ秒に1拍か計算する
        beatMSecond = (int)(1000 * (60 / bpm));
    }

    // 拍の割合
    public float GetPercent()
    {
        while (currentBeatMSecond > beatMSecond) currentBeatMSecond -= beatMSecond;
        return 1 - (float)currentBeatMSecond / beatMSecond;
    }
}
