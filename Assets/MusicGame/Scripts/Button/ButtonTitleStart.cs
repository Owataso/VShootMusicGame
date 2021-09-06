using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonTitleStart : BaseButton
{
    public override void HitAction()
    {
        base.HitAction();

        Close(0.5f);
    }
}
