using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NoteBulletManager : MonoBehaviour
{
    public enum NoteBulletType
    {
        Normal,
        Chords,
        CN
    }

    ObjectPoller objectPoller;

	// Use this for initialization
	void Start ()
    {
        objectPoller = GetComponent<ObjectPoller>();
	}

    public GameObject GetNoteBulletObject(NoteType type)
    {
        switch (type)
        {
            case NoteType.Normal:
                return objectPoller.GetPoolObject((int)NoteBulletType.Normal);

            case NoteType.Chords:
                return objectPoller.GetPoolObject((int)NoteBulletType.Chords);

            case NoteType.StartCN:
                return objectPoller.GetPoolObject((int)NoteBulletType.CN);
        }

        return null;
    }
}
