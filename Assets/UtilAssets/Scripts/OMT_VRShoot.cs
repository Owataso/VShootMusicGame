using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class OMSInfo
{
    public byte byPlayMusicNo;     // フォルダの中のどの番号の曲を再生するか
    public ushort wNumNotes;       // ノーツ数
    public ushort wNumSoflans;     // ソフラン回数
    public ushort wNumBeats;       // 拍子変更回数
    public short sDivision;        // 分能値
    public short sShift;           // shift値
}

public class NoteDataVRShoot
{
    public int iEventTime;     // イベント時間
    public ushort wSoflanNo;   // 自分が何番目のソフランの番号か(ソフラン依存の座標にするときに必要)
    public byte byNoteType;    // ノーツタイプを格納(上のenumを格納する)	// char型にしたのは、前の使ってた情報(tone)を使いまわせるからという無理やり
    public float fDegree;      // 中央からの角度 0 ～ 360(正面が0)
    public float fRateY;       // Y座標(0～1)
    public ushort wCNEndIndex; // CN終了のノーツ(EPだとレーンで判断していたが、このゲームだとこれがいることになりそう)

    public bool isNoteType(NoteType noteType) { return ((byNoteType & (byte)noteType) != 0); }
}

// 速度変化
public class SoflanData
{
    public int iEventTime;         // イベント時間
    public float fBPM;             // BPM値(小数点込)
    public float fTick;            // tick値(60 / BPM / 分能値 * 1000)
}

// 拍子
public class BeatData
{
    public int iEventTime;     // イベント時間
    public byte byNumerator;   // 分子
    public byte byDenominator; // 分母
}


public class MusicInfoVRShoot
{
    public NoteDataVRShoot[] noteDatas;  // ノーツイベント(可変長)
    public OMSInfo omsInfo;              // OMS情報
    public SoflanData[] soflanDatas;     // BPM変化イベント(可変長)
    public BeatData[] beatDatas;         // 拍子変更イベント(可変長)

}



public static class OMT_VRShoot
{
    const int END_OF_NOTE = -1;

    public static MusicInfoVRShoot LoadOMT(string fileName)
    {
        MusicInfoVRShoot ret = new MusicInfoVRShoot();

        // テキストオープン
        using (FileStream file = new FileStream(fileName, FileMode.Open))
        using (StreamReader reader = new StreamReader(file))
        {
            string line;
            string[] words;
            char[] delimiterChars = { ' ' };

            /* ヘッダチャンク侵入 */
            line = reader.ReadLine();
            if (line != "OMhd")
            {
                Debug.LogWarning("OMhdでエラー");
                return null;
            }

            // 譜面タイプチェック
            line = reader.ReadLine();
            if (line != "VR_SHOOT")
            {
                Debug.LogWarning("VR_SHOOTでエラー");
                return null;
            }

            // OMSフォーマット読み込み
            line = reader.ReadLine();
            // 空白区切り
            words = line.Split(delimiterChars);
            if (words.Length < 5)
            {
                Debug.LogWarning("OMS_Formatでエラー");
                return null;
            }
            ret.omsInfo = new OMSInfo();
            ret.omsInfo.byPlayMusicNo = byte.Parse(words[0]);
            ret.omsInfo.wNumSoflans = ushort.Parse(words[1]);
            ret.omsInfo.wNumBeats = ushort.Parse(words[2]);
            ret.omsInfo.sDivision = short.Parse(words[3]);
            ret.omsInfo.sShift = short.Parse(words[4]);

            /* データチャンク侵入 */
            line = reader.ReadLine();
            if (line != "OMdt")
            {
                Debug.LogWarning("OMdtでエラー");
                return null;
            }

            // チャンクチェック
            line = reader.ReadLine();
            if (line != "NOTE:")
            {
                Debug.LogWarning("NOTE:でエラー");
                return null;
            }

            // ノーツ読み込み
            {
                List<NoteDataVRShoot> list = new List<NoteDataVRShoot>();

                // 終端コードまで無限ループ
                while (true)
                {
                    NoteDataVRShoot append = new NoteDataVRShoot();

                    line = reader.ReadLine();
                    // 空白区切り
                    words = line.Split(delimiterChars);

                    append.iEventTime = int.Parse(words[0]);

                    // ★ノーツ終端コード(-1)
                    if (append.iEventTime == END_OF_NOTE)
                    {
                        // 領域確保
                        ret.omsInfo.wNumNotes = (ushort)list.Count;
                        ret.noteDatas = new NoteDataVRShoot[ret.omsInfo.wNumNotes];

                        // データコピー
                        for (ushort i = 0; i < list.Count; i++)
                        {
                            ret.noteDatas[i] = list[i];
                        }

                        // ループ抜ける
                        break;
                    }

                    if (words.Length < 6)
                    {
                        Debug.LogWarning("ノーツ読み込みループでエラー。空白数");
                        return null;
                    }
                    append.wSoflanNo = ushort.Parse(words[1]);
                    append.byNoteType = byte.Parse(words[2]);
                    append.fDegree = float.Parse(words[3]);
                    append.fRateY = float.Parse(words[4]);
                    append.wCNEndIndex = ushort.Parse(words[5]);

                    // リスト追加
                    list.Add(append);
                }
            }

            // チャンクチェック
            line = reader.ReadLine();
            if (line != "SOFLAN:")
            {
                Debug.LogWarning("SOFLAN:でエラー");
                return null;
            }

            // BPM変更情報
            ret.soflanDatas = new SoflanData[ret.omsInfo.wNumSoflans];
            for (ushort i = 0; i < ret.omsInfo.wNumSoflans; i++)
            {
                line = reader.ReadLine();
                // 空白区切り
                words = line.Split(delimiterChars);

                ret.soflanDatas[i] = new SoflanData();
                ret.soflanDatas[i].iEventTime = int.Parse(words[0]);
                ret.soflanDatas[i].fBPM = float.Parse(words[1]);
                ret.soflanDatas[i].fTick = float.Parse(words[2]);
            }

            // チャンクチェック
            line = reader.ReadLine();
            if (line != "BEAT:")
            {
                Debug.LogWarning("BEAT:でエラー");
                return null;
            }

            // 拍子情報
            ret.beatDatas = new BeatData[ret.omsInfo.wNumBeats];
            for (ushort i = 0; i < ret.omsInfo.wNumBeats; i++)
            {
                line = reader.ReadLine();
                // 空白区切り
                words = line.Split(delimiterChars);

                ret.beatDatas[i] = new BeatData();
                ret.beatDatas[i].iEventTime = int.Parse(words[0]);
                ret.beatDatas[i].byNumerator = byte.Parse(words[1]);
                ret.beatDatas[i].byDenominator = byte.Parse(words[2]);
            }
        }

        return ret;
    }

    //public static void WriteOMT(string fileName, MusicInfoVRShoot info)
    //{
    //    // テキストオープン
    //    using (FileStream file = new FileStream(fileName, FileMode.Open))
    //    using (StreamWriter writer = new StreamWriter(file))
    //    {
    //        string line;
    //        string[] words;
    //        char[] delimiterChars = { ' ' };

    //        /* ヘッダチャンク作成 */
    //        writer.WriteLine("OMhd");
    //        // 譜面タイプ
    //        writer.WriteLine("VR_SHOOT");
    //        // OMSフォーマット
    //        writer.WriteLine(
    //            info.omsInfo.byPlayMusicNo + " " +
    //            info.omsInfo.wNumSoflans + " " +
    //            info.omsInfo.wNumBeats + " " +
    //            info.omsInfo.sDivision + " " +
    //            info.omsInfo.sShift);
    //        /* データチャンク作成 */
    //        writer.WriteLine("OMdt");
    //        // ノートチャンク
    //        writer.WriteLine("NOTE:");

    //        // ノーツ書き出し
    //        {
    //            List<NoteDataVRShoot> list = new List<NoteDataVRShoot>();

    //            // 終端コードまで無限ループ
    //            while (true)
    //            {
    //                NoteDataVRShoot append = new NoteDataVRShoot();

    //                line = reader.ReadLine();
    //                // 空白区切り
    //                words = line.Split(delimiterChars);

    //                append.iEventTime = int.Parse(words[0]);

    //                // ★ノーツ終端コード(-1)
    //                if (append.iEventTime == END_OF_NOTE)
    //                {
    //                    // 領域確保
    //                    ret.omsInfo.wNumNotes = (ushort)list.Count;
    //                    ret.noteDatas = new NoteDataVRShoot[ret.omsInfo.wNumNotes];

    //                    // データコピー
    //                    for (ushort i = 0; i < list.Count; i++)
    //                    {
    //                        ret.noteDatas[i] = list[i];
    //                    }

    //                    // ループ抜ける
    //                    break;
    //                }

    //                if (words.Length < 6)
    //                {
    //                    Debug.LogWarning("ノーツ読み込みループでエラー。空白数");
    //                    return null;
    //                }
    //                append.wSoflanNo = ushort.Parse(words[1]);
    //                append.byNoteType = byte.Parse(words[2]);
    //                append.fDegree = float.Parse(words[3]);
    //                append.fRateY = float.Parse(words[4]);
    //                append.wCNEndIndex = ushort.Parse(words[5]);

    //                // リスト追加
    //                list.Add(append);
    //            }
    //        }

    //        // チャンクチェック
    //        line = reader.ReadLine();
    //        if (line != "SOFLAN:")
    //        {
    //            Debug.LogWarning("SOFLAN:でエラー");
    //            return null;
    //        }

    //        // BPM変更情報
    //        ret.soflanDatas = new SoflanData[ret.omsInfo.wNumSoflans];
    //        for (ushort i = 0; i < ret.omsInfo.wNumSoflans; i++)
    //        {
    //            line = reader.ReadLine();
    //            // 空白区切り
    //            words = line.Split(delimiterChars);

    //            ret.soflanDatas[i] = new SoflanData();
    //            ret.soflanDatas[i].iEventTime = int.Parse(words[0]);
    //            ret.soflanDatas[i].fBPM = float.Parse(words[1]);
    //            ret.soflanDatas[i].fTick = float.Parse(words[2]);
    //        }

    //        // チャンクチェック
    //        line = reader.ReadLine();
    //        if (line != "BEAT:")
    //        {
    //            Debug.LogWarning("BEAT:でエラー");
    //            return null;
    //        }

    //        // 拍子情報
    //        ret.beatDatas = new BeatData[ret.omsInfo.wNumBeats];
    //        for (ushort i = 0; i < ret.omsInfo.wNumBeats; i++)
    //        {
    //            line = reader.ReadLine();
    //            // 空白区切り
    //            words = line.Split(delimiterChars);

    //            ret.beatDatas[i] = new BeatData();
    //            ret.beatDatas[i].iEventTime = int.Parse(words[0]);
    //            ret.beatDatas[i].byNumerator = byte.Parse(words[1]);
    //            ret.beatDatas[i].byDenominator = byte.Parse(words[2]);
    //        }
    //    }

    //    return ret;
    //}
}
