using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectMusicBGMManager : MonoBehaviour
{
    public CustomizeManager customize; // カスタマイズ(選曲BGM用)
    float playTime;   // 選曲BGM保存用

    AudioSource audioSource;    // サウンドプレイヤー
    AuditionData auditionData;  // 試聴用データ

    readonly float fadeSpeed = 0.02f;   // フェード速度

    float fadeRate;     // フェード用
    float moveFadeRate; // フェード移動用
    bool isAudioPlayed;      // 再生フラグ

	// Use this for initialization
	void Awake()
    {
        audioSource = GetComponent<AudioSource>();
        playTime = 0;
        fadeRate = 0;
        moveFadeRate = 0;
        isAudioPlayed = false;
	}

    // Update is called once per frame
    void Update()
    {
        // フェードイン
        if (moveFadeRate > 0)
        {
            // フェードしきったら終了
            if ((fadeRate += moveFadeRate) > 1)
            {
                moveFadeRate = 0;
                fadeRate = 1;
            }
            // ボリューム設定
            audioSource.volume = fadeRate;
        }

        // フェードアウト
        else if (moveFadeRate < 0)
        {
            // フェードしきったら終了
            if ((fadeRate += moveFadeRate) < 0)
            {
                moveFadeRate = 0;
                fadeRate = 0;

                // ループ
                audioSource.time = auditionData.startSecond;
                moveFadeRate = fadeSpeed;
            }
            // ボリューム設定
            audioSource.volume = fadeRate;
        }

        // フェードしていない状態
        else if(isAudioPlayed)
        {
            // 曲終了判定
            if (audioSource.time >= auditionData.endSecond)
            {
                // フェードアウト
                moveFadeRate = -fadeSpeed;
            }
        }
    }

    public void PlayAudition(AuditionData data)
    {
        // 再生カーソル保存
        playTime = audioSource.time;

        auditionData = data;

        // 音源設定
        audioSource.clip = auditionData.audioClip;

        // BGM再生
        audioSource.time = 0;
        audioSource.Play();

        // 再生カーソル設定
        audioSource.time = auditionData.startSecond;

        // フェードインなので最初は音量0
        audioSource.volume = fadeRate = 0;
        moveFadeRate = fadeSpeed;

        // 再生フラグON
        isAudioPlayed = true;
    }

    public void PlaySelectMusicBGM(bool isFirst)
    {
        if (isFirst) playTime = 0;

        // 音源設定
        audioSource.clip = customize.GetSelectBGMAudioClip();

        // BGM再生
        audioSource.time = 0;
        audioSource.Play();

        // 再生カーソル設定
        audioSource.time = playTime;

        // 選曲画面突入時はいきなり流す
        if (isFirst)
        {
            audioSource.volume = fadeRate = 1;
            moveFadeRate = 0;
        }
        // 試聴から帰ってきたときはフェードイン
        else
        {
            audioSource.volume = fadeRate = 0;
            moveFadeRate = fadeSpeed * 4;
        }

        // 再生フラグOFF
        isAudioPlayed = false;
    }
}
