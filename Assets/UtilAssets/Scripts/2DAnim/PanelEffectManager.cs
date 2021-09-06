#if UNITY_EDITOR
using UnityEditor;
#endif
using UnityEngine;

public class PanelEffectManager : MonoBehaviour
{
    public enum EffectType
    {
	    Hit,
    }

    ObjectPoller objectPoller;
    
	// Use this for initialization
	void Start ()
    {
        objectPoller = GetComponent<ObjectPoller>();
        objectPoller.poolDatas = new ObjectPoolData[System.Enum.GetValues(typeof(EffectType)).Length];

        // Resouceからプレハブをロード&プール元のオブジェクト設定
        LoadEffectsEnumString();

        // メモリ確保
        objectPoller.poolDatas[(int)EffectType.Hit].poolObjects = new GameObject[12];
        objectPoller.Pool();
    }
	

    // enumの文字列でファイル名指定(楽で直打ち無しだが、不意のエラーが怖い)
    void LoadEffectsEnumString()
    {
        foreach (int i in System.Enum.GetValues(typeof(EffectType)))
        {
            var fileName = "Prefabs/PanelEffects/" + ((EffectType)i).ToString();
            objectPoller.poolDatas[i].originalObject = (GameObject)Resources.Load(fileName);
#if UNITY_EDITOR
            if (!objectPoller.poolDatas[i].originalObject)
            {
                EditorUtility.DisplayDialog("エラー", fileName + "\r\n対応するエフェクトがResouces/Prefabs/PenelEffetsに入っていない", "OK");
                continue;
            }
#endif
            // if文を忘れるとそれはそれはひどいことになった(PanelAnimが実行した回数だけくっつけられてた)
            //if(objectPoller.poolDatas[i].originalObject.GetComponent<PanelAnim>() == null) objectPoller.poolDatas[i].originalObject.AddComponent<PanelAnim>();
        }
    }

    // 直打ちで全部読み込む(フルパス直打ち指定はマジックナンバー扱いらしい)
    //void LoadEffectsWritePath()
    //{
    //
    //}


    public void AddEffect(EffectType type, Vector3 position, bool isLoop=false, float loopTime=0)
    {
        //var obj = Instantiate(effectObjects[(int)type], transform);
        var obj = objectPoller.GetPoolObject((int)type);
        if (!obj) return;
        obj.GetComponent<PanelAnim>().Action(position);
        if (isLoop) obj.GetComponent<PanelAnim>().SetTimer(loopTime);
    }
}
