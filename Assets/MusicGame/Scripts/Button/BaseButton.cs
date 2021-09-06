using UnityEngine;

public class BaseButton : MonoBehaviour
{
    //public bool uGUI;

    public Transform cashTransform;
    BoxCollider collider;
    float currentTime, delayTime;
    float orgScale, startScale, endScale;
    bool isRayHit, isEnable;
    public bool isAction { get; private set; }
    public bool isClosed { get; private set; }

    public float scaleEndTime = 0.25f;

    // eventをつけると外部からセットはできるが、関数実行はこのクラスじゃないとできないようになる
    //public event Action eventAction;

    // Unity版の超便利関数ポインタ
    public UnityEngine.Events.UnityEvent eventAction = new UnityEngine.Events.UnityEvent();

    protected virtual void Awake()
    {
        delayTime = 0;
        cashTransform = transform;
        collider = GetComponent<BoxCollider>();
        orgScale = endScale = cashTransform.localScale.y;
        currentTime = scaleEndTime;
        cashTransform.localScale = new Vector3(cashTransform.localScale.x, 0);

        isClosed = false;
        isAction = isRayHit = isEnable = false;
    }

    protected virtual void Update()
    {
        // ディレイ処理
        if (delayTime > 0)
        {
            delayTime = Mathf.Max(delayTime - Time.deltaTime, 0);
            return;
        }

        if (currentTime < scaleEndTime)
        {
            currentTime += Time.deltaTime;

            // 終了判定
            if (currentTime >= scaleEndTime)
            {
                currentTime = scaleEndTime;
                if (endScale == 0)
                {
                    isClosed = true;
                    // 非表示
                    gameObject.SetActive(false);

                    isEnable = false;
                }
                else isEnable = true;
            }

            float rate = currentTime / scaleEndTime;
            float newScale = Mathf.Lerp(startScale, endScale, rate);
            cashTransform.localScale = new Vector3(cashTransform.localScale.x, newScale);

            if (newScale < 0) Debug.Log("おしおき");
        }
    }

    // 撃った時の挙動
    public virtual void HitAction()
    {
        // 稼働してなかったら無視
        if (!isEnable) return;

        isAction = true;

        // イベント関数ポインタ実行
        if (eventAction != null) eventAction.Invoke();
    }

    // レーザーがヒットした挙動
    public virtual bool OnRayHitEnter()
    {
        if (!isEnable) return false;

        // 音
        oulAudio.PlaySE("Cursor0");

        // ヒットフラグ
        isRayHit = true;

        return true;
        //Debug.Log("キテルグマ");
    }

    // レーザーがボタンから外れた時の挙動
    public virtual void OnRayHitExit()
    {
        // ヒットフラグオフ
        isRayHit = false;
    }

    // 出現時の挙動
    public void Appear(float delay = 0)
    {
        delayTime = delay;
        currentTime = 0;
        startScale = 0;
        endScale = orgScale;

        isClosed = false;
        isAction = false;
    }
    public void Close(float delay = 0)
    {
        delayTime = delay;
        currentTime = 0;
        startScale = orgScale;
        endScale = 0;

        isClosed = false;
        isAction = false;
    }

    public virtual void SetEnable(bool enable)
    {
        isEnable = enable;
        collider.enabled = enable;
    }
}
