using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NaviNazenara : BaseNavigator {

	// Use this for initialization
	protected override void Start ()
    {
        base.Start();
	}

    // Update is called once per frame
    protected override void Update()
    {
        base.Update();

        // プレイヤーのほうに向く
        var targetPosition = targetPlayer.position;
        targetPosition.y = cashTransform.localPosition.y;
        cashTransform.LookAt(targetPosition);

        if (!a)
        {
            a = true;
            Reaction(ReactionType.IsApproaching);
        }
    }

    public override void Reaction(ReactionType type)
    {
        base.Reaction(type);
    }
}
