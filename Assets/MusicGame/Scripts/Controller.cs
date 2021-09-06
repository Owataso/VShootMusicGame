using UnityEngine;
using System.Collections;
using System;
using UnityEngine.UI;

public class Controller : MonoBehaviour
{
    SteamVR_TrackedObject trackedObject;
    SteamVR_Controller.Device device;

    Gun gun;                // 銃
    Constant constant;      // 定数

    ushort vibratePower;

    // Use this for initialization
    void Start()
    {
        trackedObject = GetComponent<SteamVR_TrackedObject>();
        device = SteamVR_Controller.Input((int)trackedObject.index);

        // 銃
        gun = transform.Find("Gun").GetComponent<Gun>();

        // 定数
        constant = GameObject.Find("Util").GetComponent<Constant>();

        vibratePower = 0;
    }

    // Update is called once per frame
    void Update()
    {
        if(vibratePower > 0)
        {
            device.TriggerHapticPulse(vibratePower);
            if((vibratePower -= 100) < 0) vibratePower = 0;
        }

        bool isTriggered = false;
        bool isTriggerHold = false;

        // トリガーを深く引かないと判定しない設定の場合
        if (constant.triggerDown)
        {
            // 引いた瞬間
            if (device.GetPressDown(SteamVR_Controller.ButtonMask.Trigger))
            {
                //Debug.Log("トリガーを深く引いた");
                isTriggered = true;

                // 振動設定
                if(constant.vibratePower > 0)vibratePower = constant.vibratePower;
            }
            // 引きっぱ
            else if (device.GetPress(SteamVR_Controller.ButtonMask.Trigger))
            {
                isTriggerHold = true;
            }
        }

        // トリガーを浅く引いても判定する設定の場合
        else
        {
            // 引いた瞬間
            if (device.GetTouchDown(SteamVR_Controller.ButtonMask.Trigger))
            {
                //Debug.Log("トリガーを浅く引いた");
                isTriggered = true;

                // 振動設定
                if (constant.vibratePower > 0) vibratePower = constant.vibratePower;
            }
            // 引きっぱ
            else if (device.GetPress(SteamVR_Controller.ButtonMask.Trigger))
            {
                isTriggerHold = true;
            }
        }

        // 銃のトリガーイベント
        if (isTriggered) gun.Trigger();
        else if (isTriggerHold) if(gun.TriggerHold()) if (constant.vibratePower > 0) vibratePower = constant.vibratePower;

        //if (device.GetPressUp(SteamVR_Controller.ButtonMask.Trigger))
        //{
        //    Debug.Log("深いトリガー離した");
        //}


        // トリガー離し判定
        if (device.GetTouchUp(SteamVR_Controller.ButtonMask.Trigger))
        {
            //Debug.Log("浅いトリガー離した");
            gun.TriggerUp();
        }

        //if (device.GetPressDown(SteamVR_Controller.ButtonMask.Touchpad))
        //{
        //    Debug.Log("タッチパッドをクリックした");
        //}
        //if (device.GetPress(SteamVR_Controller.ButtonMask.Touchpad))
        //{
        //    Debug.Log("タッチパッドをクリックしている");
        //}
        //if (device.GetPressUp(SteamVR_Controller.ButtonMask.Touchpad))
        //{
        //    Debug.Log("タッチパッドをクリックして離した");
        //}
        //if (device.GetTouchDown(SteamVR_Controller.ButtonMask.Touchpad))
        //{
        //    Debug.Log("タッチパッドに触った");
        //}
        //if (device.GetTouchUp(SteamVR_Controller.ButtonMask.Touchpad))
        //{
        //    Debug.Log("タッチパッドを離した");
        //}
        //if (device.GetPressDown(SteamVR_Controller.ButtonMask.ApplicationMenu))
        //{
        //    Debug.Log("メニューボタンをクリックした");
        //    laser.enabled = !laser.enabled;
        //}
        //if (device.GetTouch(SteamVR_Controller.ButtonMask.Trigger))
        //{
        //    Debug.Log("トリガーを浅く引いている");
        //}
        //if (device.GetTouch(SteamVR_Controller.ButtonMask.Touchpad))
        //{
        //    Debug.Log("タッチパッドに触っている");
        //}
    }
}
