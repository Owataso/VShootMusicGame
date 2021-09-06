using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BaseNavigator : MonoBehaviour
{
    public enum ReactionType
    {
        BadResult,
        GoodResult,
        PerfectResult,
        IsApproaching
    }

    protected Transform cashTransform;
    protected Transform targetPlayer;
    protected Animator animator;
    protected bool a = false;

	// Use this for initialization
	protected virtual void Start ()
    {
        cashTransform = transform;
        targetPlayer = Camera.main.transform;
        animator = GetComponent<Animator>();
	}
	
	// Update is called once per frame
	protected virtual void Update ()
    { 

	}

    public virtual void Reaction(ReactionType type)
    {
        animator.Play(type.ToString());
    }
}
