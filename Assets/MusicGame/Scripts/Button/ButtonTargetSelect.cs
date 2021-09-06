using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonTargetSelect : BaseButton
{
    public TargetMark targetMark;

	// Use this for initialization
	void Start()
    {
        targetMark.gameObject.SetActive(false);
	}

    public override bool OnRayHitEnter()
    {
        if (base.OnRayHitEnter())
        {
            // ターゲットマーク表示
            targetMark.gameObject.SetActive(true);
            return true;
        }
        return false;
    }

    public override void OnRayHitExit()
    {
        base.OnRayHitExit();

        // ターゲットマーク非表示
        targetMark.gameObject.SetActive(false);
    }

    public override void SetEnable(bool enable)
    {
        base.SetEnable(enable);

        if(!enable) targetMark.gameObject.SetActive(false);
    }
}
