using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonMusicStart : BaseButton
{
    GameMain gameMain;

	// Use this for initialization
	protected override void Awake()
    {
        base.Awake();

        gameMain = GameObject.Find("Game").GetComponent<GameMain>();
    }
	
    public override void HitAction()
    {
        base.HitAction();

        // 曲スタート
        gameMain.StartPlaying();
    }
}
