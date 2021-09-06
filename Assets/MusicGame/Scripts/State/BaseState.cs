using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class BaseState : MonoBehaviour
{
    public virtual void Enable() { gameObject.SetActive(true); }
    public virtual void Disable() { gameObject.SetActive(false); }
    public virtual bool isEnd() { return false; }
}
