using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum NoteType
{
    Normal = 1 << 0,    // CNなし(通常ノーツ)
    StartCN = 1 << 1,   // CN開始
    EndCN = 1 << 2,     // CN終了
    Chords = 1 << 3,    // 同時押し
}

public enum JudgeType
{
    JustCool,   // ピカグレ
    Cool,       // グレ
    Nice,       // グド
    Bad,        // バド
    Miss,       // poor
}

public enum Difficulty
{
    Normal, // N
    Hyper,  // 灰
    Expert, // 穴
    Master, // 黒穴
}

//------------------------------------------------------
//	スコア
//------------------------------------------------------
public enum ScoreRank
{
    RAAA, AAA, AA, A, B, C, D, None
}


//------------------------------------------------------
//	クリアランプ
//------------------------------------------------------
public enum ClearLamp
{
    NoPlay,	    // 未プレイ
	Failed,		// 未クリア
	Clear,		// ノマゲ
	FullCombo,	// フル婚
};

// ジャンル
public enum Genre
{
    Original,   // オリジナル
    Game,       // ゲーム音楽
    Classic,    // クラシック
    Vovaloid,   // ボカロ
    Pops,       // アニメとか版権の類
    Other,      // その他
}

public class Constant : MonoBehaviour
{
    [Range(750, 2000)]
    public int msAppearNoteTime = 1500;  // イベントタイムの差がこの時間を下回ったら出現する

    [Range(5, 20)]
    public float noteDist = 15.0f;        // 中央からのノーツの距離

    [Range(5, 10)]
    public float verticalWidth = 5f;   // 上下幅

    [Range(0.5f, 2.5f)]
    public float offsetY = 1;            // 最少のY(offsetY + rateY * verticalWidth)になる

    [Range(0, 2000)]
    public ushort vibratePower = 1000;  // 振動の強さ

    [Range(0, 1)]
    public float seVolume = 1.0f;

    public Sprite[] rankSprites = new Sprite[System.Enum.GetValues(typeof(ScoreRank)).Length];
    public Sprite[] clearSprites = new Sprite[System.Enum.GetValues(typeof(ClearLamp)).Length];
    public Sprite[] clearLampSprites = new Sprite[System.Enum.GetValues(typeof(ClearLamp)).Length];

    public int justCoolWidth = 40;
    public int coolWidth = 80;
    public int niceWidth = 150;
    public int badWidth = 300;

    public bool triggerDown = false;   // トリガーを強く引かないとアクションしない

    public bool autoPlay = false;      // DJAUTOさん

    public static readonly int maxScore = 100000;
    public static readonly Color[] difficultyColors = { new Color(0, 1, 0, 1), new Color(1, 1, 0, 1), new Color(1, 0, 0, 1), new Color(1, 0, 1, 1) };

    public static ScoreRank ComputeRank(int score)
    {
        if (score == 0) return ScoreRank.None;
        if (score >= 95000) return ScoreRank.RAAA;
        if (score >= 90000) return ScoreRank.AAA;
        if (score >= 80000) return ScoreRank.AA;
        if (score >= 70000) return ScoreRank.A;
        if (score >= 60000) return ScoreRank.B;
        if (score >= 50000) return ScoreRank.C;
        return ScoreRank.D;
    }

    public Sprite GetRankSprite(int score) { return rankSprites[(int)ComputeRank(score)]; }
    public Sprite GetClearSprite(ClearLamp lamp) { return clearSprites[(int)lamp]; }
    public Sprite GetClearLampSprite(ClearLamp lamp) { return clearLampSprites[(int)lamp]; }
}


// 試聴データ
public class AuditionData
{
    public AudioClip audioClip;  // 音源
    public float startSecond;    // フェードインが始まる秒
    public float endSecond;      // フェードインが終わる秒
}

// 曲の記録情報
public struct MusicRecord
{
    public int hiScore;         // ハイスコア
    public byte byClearLamp;    // クリアランプ
    public ushort maxCombp;     // 最大コンボ数
    public ushort playCount;    // プレイ回数
}

// 難易度ごとに持つ情報
public struct MusicDifficulData
{
    public int level;           // レベル
    public int playMusicNumber; // 再生する曲の番号(にえんて)
    public MusicRecord record;  // プレイヤー記録
}


// 曲の情報
public class MusicData
{
    public AuditionData auditionData = new AuditionData();  // 試聴データ
    public string folderName;           // データ入ってるフォルダ名
    public string musicName;           // 曲名
    public string artistName;           // アーティスト名
    public Sprite jacketSprite;        // ジャケット
    //public AudioClip audioClip;        // 曲の音源データ
    public float minBPM, maxBPM;       // BPM
    public Genre genre;                // ジャンル
    public MusicDifficulData[] difficulDatas = new MusicDifficulData[System.Enum.GetValues(typeof(Difficulty)).Length];   // 難易度ごとの情報
}

public class ResultData
{
    public int score;
    public int maxCombo;
    public int[] judgeCounts = new int[System.Enum.GetValues(typeof(JudgeType)).Length];
    public void Clear()
    {
        score = maxCombo = 0;
        System.Array.Clear(judgeCounts, 0, judgeCounts.Length);
    }
}
