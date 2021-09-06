using UnityEngine;

public class ParticleManager : MonoBehaviour
{
    public enum EffectType
    {
        JustCool,
        Cool,
        Nice,
        Bad,
        Miss
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
        objectPoller.poolDatas[(int)EffectType.JustCool].poolObjects = new GameObject[12];
        objectPoller.poolDatas[(int)EffectType.Cool].poolObjects = new GameObject[12];
        objectPoller.poolDatas[(int)EffectType.Nice].poolObjects = new GameObject[12];
        objectPoller.poolDatas[(int)EffectType.Bad].poolObjects = new GameObject[12];
        objectPoller.poolDatas[(int)EffectType.Miss].poolObjects = new GameObject[12];

        objectPoller.Pool();
    }

    // enumの文字列でファイル名指定(楽で直打ち無しだが、不意のエラーが怖い)
    void LoadEffectsEnumString()
    {
        foreach (int i in System.Enum.GetValues(typeof(EffectType)))
        {
            var fileName = "Prefabs/ParticleEffects/" + ((EffectType)i).ToString();
            objectPoller.poolDatas[i].originalObject = (GameObject)Resources.Load(fileName);
#if UNITY_EDITOR
            if (!objectPoller.poolDatas[i].originalObject)
            {
                UnityEditor.EditorUtility.DisplayDialog("エラー", fileName + "\r\n対応するエフェクトがResouces/Prefabs/ParticleEffetsに入っていない", "OK");
                continue;
            }
#endif
        }
    }

    public void AddEffect(EffectType type, Vector3 position)
    {
        //var obj = Instantiate(effectObjects[(int)type], transform);
        var obj = objectPoller.GetPoolObject((int)type);
        if (!obj) return;
        obj.GetComponent<ParticleEffect>().Action(position);
    }
}
