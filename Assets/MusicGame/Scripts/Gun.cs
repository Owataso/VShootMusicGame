using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Gun : MonoBehaviour
{
    LineRenderer laser;     // レーザー描画用
    Transform laserStart;   // レーザーの根元
    Transform laserVec;     // レーザーの向きベクトル
    Vector3 laserDir;
    Transform muzzle;       // 銃口(3Dサウンドとかエフェクトの座標とか用)
    AudioSource shotSound;  // ショット音

    public GameMain gameMain;
    public NoteManager noteManager; // ノーツ(判定用)
    public PlayCursor playCursor;   // 連射用

    RaycastHit raycastHit;

    GameObject shotImpact;
    GameObject shotRipple;
    public BulletManager bulletManager;    // 弾(判定とは一切関係ない)管理

    Note cnNote;     // CN中のノートオブジェクト

    BaseButton rayHitButton;    // レーザーが当たってるボタン

    Transform cashTransform;

    float rapidFireTime;

    // Use this for initialization
    void Start()
    {
        cashTransform = transform;

        // レーザー
        laser = GetComponent<LineRenderer>();
        laser.startColor = new Color(1, 0, 0);
        laser.endColor = new Color(1, 0, 0);
        laserStart = cashTransform.Find("LaserStart");
        laserVec = cashTransform.Find("LaserVec");

        // 銃口
        muzzle = cashTransform.Find("Muzzle");
        shotSound = muzzle.GetComponent<AudioSource>();
        // カスタマイズからガンショットのClipを取得
        shotSound.clip = GameObject.Find("Game").GetComponent<CustomizeManager>().GetGunShotAudioClip();

        // 銃のエフェクト
        shotImpact = cashTransform.Find("ShotImpact").gameObject;
        shotRipple = cashTransform.Find("ShotRipple").gameObject;

        // 銃弾管理さん
        //bulletManager = GameObject.Find("BulletManager").GetComponent<BulletManager>();

        // ノート(判定用)
        //noteManager = GameObject.Find("Game/Playing/NoteManager").GetComponent<NoteManager>();

        cnNote = null;

        //Debug.Log("銃キテルグマ");
    }

    // Update is called once per frame
    void Update ()
    {
        // レーザーベクトル
        laserDir = LaserDir();

        // レーザの根元座標更新
        laser.SetPosition(0, laserStart.position);

        // レーザー壁判定処理
        // レーザーがオブジェクトにヒットしていたら
        if (Physics.Raycast(cashTransform.position, laserDir, out raycastHit))
        {
            // レーザーの終端をを交点に
            laser.SetPosition(1, raycastHit.point);

            // ボタン判定(ボタンじゃなかったらnull)
            BaseButton button = raycastHit.transform.GetComponent<BaseButton>();
            // ヒットの入った瞬間と抜けた瞬間の処理
            if (button)
            {
                if (!rayHitButton)
                {
                    // 稼働してなかったら当たってない扱いにする
                    if (!button.OnRayHitEnter()) button = null;
                }
                else if (rayHitButton.name != button.name)
                {
                    // 稼働してなかったら当たってない扱いにする
                    if (!button.OnRayHitEnter()) button = null;
                    rayHitButton.OnRayHitExit();
                }
            }
            else if (rayHitButton) rayHitButton.OnRayHitExit();

            rayHitButton = button;
        }
        else
        {
            // レーザーの終端を直線状に
            laser.SetPosition(1, laserDir * 100);

            // 忘れたからうまく動かなかった元
            if (rayHitButton) rayHitButton.OnRayHitExit();
            rayHitButton = null;
        }
    }

    public void Trigger()
    {
        // エフェクトと音再生
        ShotSoundEffect();

        // 弾発射(判定とは関係ない)
        bulletManager.AddBullet(muzzle.position, laserDir);

        // レイキャストがヒットしてなかったら出ていけぇ！！
        if (!raycastHit.collider) return;

        // ボタン処理
        if (rayHitButton)
        {
            rayHitButton.HitAction();
            rayHitButton = null;
            return;
        }

        // CN連射時間リセット
        rapidFireTime = 0;

        // ノート判定
        if(gameMain.state == GameMain.State.Playing)cnNote = noteManager.Judge(laserStart.position, LaserDir());
    }

    public bool TriggerHold()
    {
        // CN中なら連射処理
        if(cnNote)
        {
            // CN終了処理
            if (!cnNote.isCNHold)
            {
                cnNote = null;
                return false;
            }

            //Debug.Log(playCursor.currentBPM);

            // BPMから連射間隔を計算
            float rapidInterval = 60 / playCursor.currentBPM;
            // 16分間隔
            rapidInterval *= 0.25f;

            if ((rapidFireTime += Time.deltaTime) >= rapidInterval)
            {
                // エフェクトと音再生
                ShotSoundEffect();
                // 弾発射(ホーミング)
                bulletManager.AddHomingBullet(muzzle.position, laserDir, cnNote.transform);
                // 時間
                rapidFireTime -= rapidInterval;

                return true;
            }
        }
        return false;
    }

    public void TriggerUp()
    {
        // CN中なら
        if (cnNote)
        {
            noteManager.JudgeCNNote(cnNote);
            cnNote = null;
        }
    }

    void ShotSoundEffect()
    {
        // ショット音再生
        shotSound.PlayOneShot(shotSound.clip);

        // ショットエフェクト
        shotImpact.SetActive(true);
        shotImpact.GetComponent<oul3DAnimBase>().Action();
        shotRipple.SetActive(true);
        shotRipple.GetComponent<oul3DAnimBase>().Action();
    }

    Vector3 LaserDir() { return (laserVec.position - laserStart.position).normalized; }
}
