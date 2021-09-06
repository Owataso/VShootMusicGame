using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TitleState : BaseState
{
    ButtonTitleStart buttonTitleStart;

	// Use this for initialization
	void Start()
    {
        buttonTitleStart = transform.Find("StartButton").GetComponent<ButtonTitleStart>();
        buttonTitleStart.Appear(1);
	}
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    // タイトルスタートボタンを撃ったらタイトル終了
    public override bool isEnd() { return buttonTitleStart.isClosed; }
}
