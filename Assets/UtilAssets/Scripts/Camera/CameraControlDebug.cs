using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraControlDebug : MonoBehaviour
{
    [Range(0.01f, 0.1f)]
    public float translateSpeed = 0.05f;// 移動速度

    [Range(1.0f, 5.7f)]
    public float rotateSpeed = 3.0f;   // 回転速度

    [Range(0.1f, 1)]
    public float lerp = 0.33f;          // 補間度合

    Vector3 newPosition;
    float newYaw, newPitch;

    Gun gun;

	// Use this for initialization
	void Start()
    {
        //// VRカメラ作動してたら無効にする
        //if(GetComponent<SteamVR_Camera>().enabled)
        //{
        //    enabled = false;
        //    return;
        //}

        //Debug.Log("キテルグマ");
        newPosition = transform.position;
        newYaw = newPitch = 0;

        var gunTransform = transform.Find("Adjust/Gun");
        if (gunTransform) gun = gunTransform.GetComponent<Gun>();
	}
	
	// Update is called once per frame
	void Update ()
    {
        // キー入力で制御

        // Translate
        var forward = new Vector3(Mathf.Sin(transform.localEulerAngles.y * Mathf.Deg2Rad), 0, Mathf.Cos(transform.localEulerAngles.y * Mathf.Deg2Rad));
        if (Input.GetKey(KeyCode.W)) newPosition += forward * translateSpeed;
        if (Input.GetKey(KeyCode.S)) newPosition -= forward * translateSpeed;
        if (Input.GetKey(KeyCode.A)) newPosition -= transform.right * translateSpeed;
        if (Input.GetKey(KeyCode.D)) newPosition += transform.right * translateSpeed;
        if (Input.GetKey(KeyCode.Q)) newPosition.y -= translateSpeed;
        if (Input.GetKey(KeyCode.E)) newPosition.y += translateSpeed;
        if (newPosition.y < 0) newPosition.y = 0;
        transform.localPosition = newPosition * lerp + transform.localPosition * (1 - lerp);

        // Rotate
        //if (Input.GetKey(KeyCode.UpArrow)) transform.Rotate(-rotateSpeed, 0, 0);
        //if (Input.GetKey(KeyCode.DownArrow)) transform.Rotate(rotateSpeed, 0, 0);
        if (Input.GetKey(KeyCode.UpArrow))  newPitch -= rotateSpeed;
        if (Input.GetKey(KeyCode.DownArrow)) newPitch += rotateSpeed;
        if (Input.GetKey(KeyCode.LeftArrow)) newYaw -= rotateSpeed;
        if (Input.GetKey(KeyCode.RightArrow)) newYaw += rotateSpeed;
        newPitch = Mathf.Clamp(newPitch, -85, 85);
        var newAngle = transform.localEulerAngles;
        newAngle.x = Mathf.LerpAngle(transform.localEulerAngles.x, newPitch, lerp);
        newAngle.y = Mathf.LerpAngle(transform.localEulerAngles.y, newYaw, lerp);
        transform.localEulerAngles = newAngle;

        // 銃のオンオフ
        if (gun)
        {
            if (Input.GetKeyDown(KeyCode.Space)) gun.gameObject.SetActive(!gun.gameObject.activeInHierarchy);

            // 銃のトリガー
            if (gun.gameObject.activeInHierarchy)
            {
                if (Input.GetKeyDown(KeyCode.Return)) gun.Trigger();
                else if (Input.GetKey(KeyCode.Return)) gun.TriggerHold();
                else if (Input.GetKeyUp(KeyCode.Return)) gun.TriggerUp();
            }
        }
    }
}
