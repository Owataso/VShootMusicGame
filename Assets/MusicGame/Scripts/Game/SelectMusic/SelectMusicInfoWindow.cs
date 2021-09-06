using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectMusicInfoWindow : MonoBehaviour
{
    public SpriteRenderer clearSprite;
    public SpriteRenderer rankSprite;
    public Number scoreNumber;
    public Number playCountNumber;
    public Number minBPMNumber, maxBPMNumber;

    Constant constant;
    public void Awake()
    {
        constant = GameObject.Find("Util").GetComponent<Constant>();
    }

    public void Set(MusicData data, Difficulty dif)
    {

        var record = data.difficulDatas[(int)dif].record;
        clearSprite.sprite = constant.GetClearSprite((ClearLamp)record.byClearLamp);
        rankSprite.sprite = constant.GetRankSprite(record.hiScore);
        scoreNumber.SetNumber(record.hiScore);
        playCountNumber.SetNumber(record.playCount);
        minBPMNumber.SetNumber(data.minBPM, 1);
        maxBPMNumber.SetNumber(data.maxBPM, 1);
    }
}
