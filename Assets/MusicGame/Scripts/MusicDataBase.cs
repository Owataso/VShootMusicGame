using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using UnityEngine;

public static class MusicDataBase
{
    public static MusicData[] musicDatas;
    public static bool saveFlag = false;

    public static AuditionData GetAuditionData(int id)
    {
        // 曲データ読み込んでいないなら読み込む
        if(!musicDatas[id].auditionData.audioClip)
        {
#if CUSTOM_MUSIC
            musicDatas[id].auditionData.audioClip = WavLoader.LoadWAV("AudioClip_" + musicDatas[id].folderName, path + "/test.wav", false);
#else
            musicDatas[id].auditionData.audioClip = Resources.Load("Musics/" + musicDatas[id].folderName + "/Music1") as AudioClip;
#endif
        }
        return musicDatas[id].auditionData;
    }

    public static void LoadMusics()
    {
        // フォルダー列挙
        string[] subFolders = oulFile.EnumDirectory(Application.dataPath + "/Musics");

        // 曲の分の配列確保
        musicDatas = new MusicData[subFolders.Length];

        for (int i = 0; i < subFolders.Length; i++)
        {
            var folderName = subFolders[i];

            musicDatas[i] = new MusicData();

            // フォルダー名=保存
            musicDatas[i].folderName = folderName;

            // パス
            string path = Application.dataPath + "/Musics/"+ musicDatas[i].folderName;

            // テキスト情報読み込み
            {
                //Debug.Log(path + "/info.txt");

                var loader = new TextLoader();
                loader.LoadText(path + "/info.txt");

                // 曲名、アーティスト名
                musicDatas[i].musicName = loader.ReadLine();
                musicDatas[i].artistName = loader.ReadLine();

                // レベル
                foreach (int dif in System.Enum.GetValues(typeof(Difficulty)))
                {
                    musicDatas[i].difficulDatas[dif].level = loader.ReadInt();
                }
                // ジャンル
                musicDatas[i].genre = (Genre)loader.ReadInt();

                // BPM
                musicDatas[i].minBPM = loader.ReadFloat();
                musicDatas[i].maxBPM = loader.ReadFloat();

                // 試聴する秒
                musicDatas[i].auditionData.startSecond = loader.ReadFloat();
                musicDatas[i].auditionData.endSecond = loader.ReadFloat();
            }

            // プレイ記録読み込み
            {
                // バイナリオープン
                using (FileStream file = new FileStream(path + "/record.bin", FileMode.Open, FileAccess.Read))
                using (BinaryReader reader = new BinaryReader(file))
                {
                    foreach (int dif in System.Enum.GetValues(typeof(Difficulty)))
                    {
                        musicDatas[i].difficulDatas[dif].record.hiScore = reader.ReadInt32();      // ハイスコア
                        musicDatas[i].difficulDatas[dif].record.byClearLamp = reader.ReadByte();   // クリアランプ
                        musicDatas[i].difficulDatas[dif].record.maxCombp = reader.ReadUInt16();    // 最大コンボ数
                        musicDatas[i].difficulDatas[dif].record.playCount = reader.ReadUInt16();   // プレイ回数
                    }
                }
            }

            // ジャケット画像読み込み
            var texture = PngLoader.LoadPNG(path + "/jacket.png");
            if (texture) musicDatas[i].jacketSprite = Sprite.Create(texture, new Rect(0, 0, texture.width, texture.height), new Vector2(0.5f, 0.5f));

            // 音源ファイル読み込み(取得時に読み込むことにした)

        }
    }

    static public void SaveRecord()
    {
        if (!saveFlag) return;
        //Debug.Log("キテルグマ");

        for (int i = 0; i < musicDatas.Length; i++)
        {
            // パス
            var path = Application.dataPath + "/Musics/" + musicDatas[i].folderName;

            // バイナリオープン
            using (FileStream file = new FileStream(path + "/record.bin", FileMode.Open, FileAccess.Write))
            using (BinaryWriter writer = new BinaryWriter(file))
            {
                foreach (int dif in System.Enum.GetValues(typeof(Difficulty)))
                {
                    var record = musicDatas[i].difficulDatas[dif].record;
                    writer.Write(BitConverter.GetBytes(record.hiScore), 0, 4);
                    writer.Write(BitConverter.GetBytes(record.byClearLamp), 0, 1);
                    writer.Write(BitConverter.GetBytes(record.maxCombp), 0, 2);
                    writer.Write(BitConverter.GetBytes(record.playCount), 0, 2);
                }
            }
        }
    }
}
