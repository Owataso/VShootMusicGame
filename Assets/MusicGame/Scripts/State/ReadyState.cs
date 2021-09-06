using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ReadyState : BaseState
{
    ButtonDescription[] buttonDescriptions = new ButtonDescription[2];
    ButtonMusicStart buttonMusicStart;

	// Use this for initialization
	void Awake()
    {
        buttonDescriptions[0] = transform.Find("Button_Description0").GetComponent<ButtonDescription>();
        buttonDescriptions[1] = transform.Find("Button_Description1").GetComponent<ButtonDescription>();
        buttonMusicStart = transform.Find("StartButton").GetComponent<ButtonMusicStart>();
    }
	
    public override void Enable()
    {
        base.Enable();

        buttonDescriptions[0].Appear(0);
        buttonDescriptions[1].Appear(0.1f);
        buttonMusicStart.Appear(0.2f);
    }

    public override void Disable()
    {
        base.Disable();

        buttonDescriptions[0].Close(0.2f);
        buttonDescriptions[1].Close(0.1f);
        buttonMusicStart.Close(0);
    }
}
