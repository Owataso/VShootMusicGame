using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ParticleEffect : MonoBehaviour
{

    ParticleSystem[] particleSystems;
    public bool isAction { get; private set; }  // アクションフラグ
    float currentTime;                          // エフェクト時間(終了判定に使う)
    public float endTime = 2;                   // エフェクト終了時間

    Transform cashTransform;

    // Use this for initialization
    void Awake()
    {
        cashTransform = transform;
        isAction = false;

        // パーティクルシステム初期化
        particleSystems = GetComponentsInChildren<ParticleSystem>();
        foreach (ParticleSystem system in particleSystems)
        {
            system.Clear();
        }
    }

    // Update is called once per frame
    void Update()
    {
#if UNITY_EDITOR
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            Action(cashTransform.localPosition);
        }
#endif

        // 発動してたら
        if (!isAction) return;

        // 時間計測
        if (currentTime < endTime)
        {
            if((currentTime += Time.deltaTime) >= endTime)
            {
                isAction = false;
                //gameObject.SetActive(false);
            }
        }
    }

    public void Action(Vector3 position)
    {
        cashTransform.localPosition = position;

        // アクションフラグ
        isAction = true;
        currentTime = 0;

        // パーティクルシステム稼働
        foreach (ParticleSystem system in particleSystems)
        {
            system.Clear();
            system.Play();
        }
    }
}
