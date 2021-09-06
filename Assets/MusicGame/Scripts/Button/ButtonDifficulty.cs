using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonDifficulty : ButtonWhiteSelect
{
    GameMain gameMain;
    public Difficulty difficulty;

	// Use this for initialization
	protected override void Awake()
    {
        base.Awake();
        // ★rootGameMain
        gameMain = transform.root.GetComponent<GameMain>();
        // イベント登録
        eventAction.AddListener(ChangeDifficulty);
	}
	
	// Update is called once per frame
	protected override void Update ()
    {
        base.Update();
	}

    void ChangeDifficulty()
    {
        gameMain.ChangeDifficulty(difficulty);
    }
}
