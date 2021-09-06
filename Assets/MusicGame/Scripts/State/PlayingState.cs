using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayingState : BaseState
{
    public NoteManager noteManager;
    public SoflanManager soflanManager;
    public JudgeManager judgeManager;
    public ScoreManager scoreManager;
    public PlayCursor playCursor;
    public FullComboEffect fullCombo;


    public void SetMusic(MusicData selectMusicData, Difficulty difficulty)
    {
        string path;
        path = Application.dataPath + "/Musics/";
        MusicInfoVRShoot musicInfo = OMT_VRShoot.LoadOMT(path + selectMusicData.folderName + "/" + difficulty.ToString() + ".omt");

        noteManager.Set(musicInfo);
        soflanManager.Set(musicInfo);
        judgeManager.Reset();
        scoreManager.Set(musicInfo.omsInfo.wNumNotes);
        fullCombo.Reset();
        playCursor.Set(selectMusicData.auditionData.audioClip);
        playCursor.Play();
    }

    public override void Enable()
    {
        gameObject.SetActive(true);
    }

    public override bool isEnd()
    {
        // 曲終わっててもフルコンまで待つ
        if(fullCombo.isAction)
        {
            return fullCombo.isEnd();
        }
        return (!playCursor.isPlay());
    }

    public int GetNumNote() { return noteManager.GetNumNote(); }
}
