using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FullComboEffect : MonoBehaviour
{
    //const int numRipple = 6;
    //RippleAnim[] rippleObjects = new RippleAnim[numRipple];

    //AlphaAnim flashCircle;

    AlphaAnim targetIn, targetOut, cyberCircle;

    MoveInOutAnim fullStr;  // Fullの文字
    MoveInOutAnim comboStr; // Comboの文字

    const int numSatelliteRing = 3;
    UVScroll[] satelliteRings = new UVScroll[numSatelliteRing];

    public bool isAction { get; private set; }  // アクションフラグ
    float currentTime;                          // フルコンエフェクト時間(終了判定に使う)
    readonly float endTime = 4;                 // フルコンエフェクト終了時間

    ParticleSystem[] particleSystems;

    // Use this for initialization
    void Awake()
    {
        //for (int i = 0; i < numRipple; i++) rippleObjects[i] = transform.Find("Ripple" + i).GetComponent<RippleAnim>();
        for (int i = 0; i < numSatelliteRing; i++) satelliteRings[i] = transform.Find("SatelliteRing" + i).GetComponent<UVScroll>();

        //flashCircle = transform.Find("FlashCircle").GetComponent<AlphaAnim>();
        targetIn = transform.Find("TargetIn1").GetComponent<AlphaAnim>();
        targetOut = transform.Find("TargetOut1").GetComponent<AlphaAnim>();
        cyberCircle = transform.Find("CyberCircle0").GetComponent<AlphaAnim>();
        fullStr = transform.Find("FullCombo0").GetComponent<MoveInOutAnim>();
        comboStr = transform.Find("FullCombo1").GetComponent<MoveInOutAnim>();
        //rippleObjects[0] = transform.Find("Ripple0").GetComponent<RippleAnim>();
        //rippleObjects[1] = transform.Find("Ripple1").GetComponent<RippleAnim>();
        //rippleObjects[2] = transform.Find("Ripple2").GetComponent<RippleAnim>();
        //rippleObjects[3] = transform.Find("Ripple2").GetComponent<RippleAnim>();

        // パーティクルシステム初期化
        particleSystems = transform.Find("Particles").GetComponentsInChildren<ParticleSystem>();
        foreach (ParticleSystem system in particleSystems)
        {
            system.Clear();
        }

        isAction = false;
    }

    // Update is called once per frame
    void Update()
    {
        //if(Input.GetKeyDown(KeyCode.Return))
        //{
        //    Action();
        //}

        // フルコン発動してたら
        if (!isAction) return;

        // 時間計測
        if (currentTime < endTime) currentTime += Time.deltaTime;
    }

    public void Action(/*Vector3? pos = null*/)
    {
        // アクションフラグ
        isAction = true;
        currentTime = 0;

        //Vector3 position = pos ?? Vector3.zero;
        //transform.position = position;

        //Debug.Log(position);

        //for (int i = 0; i < numRipple; i++)
        //{
        //    rippleObjects[i].gameObject.SetActive(true);
        //    //rippleObjects[i].Action();
        //}
        for (int i = 0; i < numSatelliteRing; i++)
        {
            satelliteRings[i].gameObject.SetActive(true);
        }

        //flashCircle.gameObject.SetActive(true);
        targetIn.gameObject.SetActive(true);
        targetOut.gameObject.SetActive(true);
        cyberCircle.gameObject.SetActive(true);
        fullStr.gameObject.SetActive(true);
        comboStr.gameObject.SetActive(true);

        //rippleObjects[0].Action();
        //rippleObjects[1].Action();
        //rippleObjects[2].Action();
        //rippleObjects[3].Action(0.12f);
        //rippleObjects[4].Action(0.20f);
        //rippleObjects[5].Action(0.28f);

        //flashCircle.Action();

        targetIn.Action(0.45f);
        targetOut.Action(0.45f);
        cyberCircle.Action(0.45f);

        fullStr.Action(0.5f);
        comboStr.Action(0.5f);

        satelliteRings[0].Action(0.08f);
        satelliteRings[1].Action(0.08f);
        satelliteRings[2].Action(0.08f);

        // パーティクルシステム稼働
        foreach (ParticleSystem system in particleSystems)
        {
            system.Clear();
            system.Play();
        }
    }

    public void Reset()
    {
        isAction = false;
    }

    // フルコン発動していてなおかつ終了していたら
    public bool isEnd() { return (isAction && currentTime >= endTime); }
}
