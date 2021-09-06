using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEngine;

public static class WavLoader
{
    public static readonly float b2f8Bit = 1.0f / Mathf.Pow(2, 7);
    public static readonly float b2f16Bit = 1.0f / Mathf.Pow(2, 15);

    public static AudioClip LoadWAV(string clipName, string path, bool isStream)
    {
        try
        {
            byte[] buf;
            int readIndex = 0;

            buf = oulFile.ReadAllBytes(path);

            /* RIFFチャンク侵入 */
            if (Encoding.ASCII.GetString(buf, readIndex, 4) != "RIFF")
            {
                ExceptionMessage.MessageBox("WAVファイルエラー", "RIFFチャンク侵入失敗");
                return null;
            }
            readIndex += 4;

            // 以降のファイルサイズ(読み飛ばし)
            readIndex += 4;

            // RIFFの種類がWAVEであることを表すWAVEヘッダ
            if (Encoding.ASCII.GetString(buf, readIndex, 4) != "WAVE")
            {
                ExceptionMessage.MessageBox("WAVファイルエラー", "WAVヘッダじゃない");
                return null;
            }
            readIndex += 4;

            ushort channels;          // チャンネル
            int lengthSamples;        // Createの引数用
            int samplePerSecond;      // サンプルレート(UnityではFrequencyという名前で扱っている)
            ushort bitsPerSample;     // サンプルあたりのビット数(ここではbyte→float関数の分岐で使っている)

            // fmtチャンクまで読み飛ばし
            while (true)
            {
                if (Encoding.ASCII.GetString(buf, readIndex++, 1) == "f") break;
            }
            // ゴリ
            if (Encoding.ASCII.GetString(buf, readIndex, 3) != "mt ")
            {
                ExceptionMessage.MessageBox("WAVファイルエラー", "fmt チャンク侵入失敗");
                return null;
            }
            readIndex += 3;

            /* fmtチャンク侵入 */

            // fmtチャンクのバイト数
            //uint fmtChunkSize = BitConverter.ToUInt32(buf, readIndex);
            readIndex += 4;

            // フォーマットID
            readIndex += 2;

            // チャンネル数
            channels = BitConverter.ToUInt16(buf, readIndex);
            readIndex += 2;

            // サンプリングレート
            samplePerSecond = BitConverter.ToInt32(buf, readIndex);
            readIndex += 4;

            // データ速度(Byte/sec)
            readIndex += 4;

            // ブロックサイズ
            readIndex += 2;

            // サンプルあたりのビット数(bit/sample)ビットは8 or 16
            bitsPerSample = BitConverter.ToUInt16(buf, readIndex);
            readIndex += 2;

            // dataチャンクまで読み飛ばし
            while (true)
            {
                if (Encoding.ASCII.GetString(buf, readIndex++, 1) == "d") break;
            }
            // ゴリ
            if (Encoding.ASCII.GetString(buf, readIndex, 3) != "ata")
            {
                ExceptionMessage.MessageBox("WAVファイルエラー", "dataチャンク侵入失敗");
                return null;
            }
            readIndex += 3;

            /* dataチャンク侵入 */

            int dataSize = BitConverter.ToInt32(buf, readIndex);
            readIndex += 4;

            // byte→floatに変換するためのラムダ式
            Func<float> ConvertByteToFloat;

            // 8bitならば符号無しunsigned(0 ～ 255, 無音は 128)
            if (bitsPerSample == 8)
            {
                // 8ビットサンプルの場合のfloat変換関数
                ConvertByteToFloat = () =>
                {
                    // 1 / 128
                    return ((int)buf[readIndex] - 0x80) * b2f8Bit;
                };
            }
            // 16bit ならば符号付き signed(-32768 ～ +32767, 無音は 0)
            else if (bitsPerSample == 16)
            {
                // 16ビットサンプルの場合のfloat変換関数
                ConvertByteToFloat = () =>
                {
                    // 1 / 32768
                    return System.BitConverter.ToInt16(buf, readIndex) * b2f16Bit;
                };
            }
            else
            {
                ExceptionMessage.MessageBox("WAVEファイルエラー", "サンプルおかしい");
                return null;
            }

            // 不正解ルート
            // データサイズをそのままぶち込むと超悲惨なことになります(実行するたびにメモリがガンガン消費してUnityが落ちる)
            //float[] floatBuf = new float[dataSize];
            //for (int i = 0; i < dataSize; i++)
            //{
            //    floatBuf[i] = ConvertByteToFloat();
            //    if (floatBuf[i] < 0 || floatBuf[i] > 1) Debug.Log(floatBuf[i]);
            //    readIndex++;
            //}

            const int rate = 8;

            // 正解ルート
            // データサンプル数
            lengthSamples = dataSize / (bitsPerSample / 8) / channels / rate;

            float[] floatBuf = new float[lengthSamples * channels];
            int stepByte = bitsPerSample / 8;

            // 変換ラムダ式を使ってデータfloat化
            for (int i = 0; i < lengthSamples * channels; i++)
            {
                floatBuf[i] = ConvertByteToFloat();
                readIndex += stepByte * rate;
            }

            // フォーマット情報からクリップ作成
            AudioClip clip = AudioClip.Create(clipName, lengthSamples, channels, samplePerSecond / rate, isStream);

            // 波形データセット
            clip.SetData(floatBuf, 0);

            return clip;
        }

        catch (System.Exception e)
        {
            ExceptionMessage.Message("wav load error", e);
        }

        return null;
    }

    public static AudioClip LoadOGG(string clipName, string path)
    {
        /*
         Byte order: Little-endian Offset   Length   Contents
        [ 0      4 bytes  "OggS"
          4      1 byte   Stream structure version (0x00)
          5      1 byte   Packet flag:
                            bit 0:  true if page continued
                            bit 1:  true if first page
                            bit 2:  true if last page
                            bit 3..7: reserved
          6      8 bytes  The end pcm sample position (64bit integer)
         14      4 bytes  Stream serial number
         18      4 bytes  Page number
         22      4 bytes  Check sum
         26      1 byte   Number of segments(s)
         27     (s)bytes  Sengment table
         27+(s) (b)bytes  Body (b := header[27] + header[27+1] + ... + header[27+s-1])
        ]

         */
        try
        {
            byte[] buf;
            int readIndex = 0;

            buf = oulFile.ReadAllBytes(path);

            /* RIFFチャンク侵入 */
            if (Encoding.ASCII.GetString(buf, readIndex, 4) != "RIFF")
            {
                ExceptionMessage.MessageBox("WAVファイルエラー", "RIFFチャンク侵入失敗");
                return null;
            }
            readIndex += 4;

            // 以降のファイルサイズ(読み飛ばし)
            readIndex += 4;

            // RIFFの種類がWAVEであることを表すWAVEヘッダ
            if (Encoding.ASCII.GetString(buf, readIndex, 4) != "WAVE")
            {
                ExceptionMessage.MessageBox("WAVファイルエラー", "WAVヘッダじゃない");
                return null;
            }
            readIndex += 4;

            ushort channels;          // チャンネル
            int lengthSamples;        // Createの引数用
            int samplePerSecond;      // サンプルレート(UnityではFrequencyという名前で扱っている)
            ushort bitsPerSample;     // サンプルあたりのビット数(ここではbyte→float関数の分岐で使っている)

            // fmtチャンクまで読み飛ばし
            while (true)
            {
                if (Encoding.ASCII.GetString(buf, readIndex++, 1) != "f") break;
            }
            // ゴリ
            if (Encoding.ASCII.GetString(buf, readIndex, 3) != "mt ")
            {
                ExceptionMessage.MessageBox("WAVファイルエラー", "fmt チャンク侵入失敗");
                return null;
            }
            readIndex += 3;

            /* fmtチャンク侵入 */

            // fmtチャンクのバイト数
            //uint fmtChunkSize = BitConverter.ToUInt32(buf, readIndex);
            readIndex += 4;

            // フォーマットID
            readIndex += 2;

            // チャンネル数
            channels = BitConverter.ToUInt16(buf, readIndex);
            readIndex += 2;

            // サンプリングレート
            samplePerSecond = BitConverter.ToInt32(buf, readIndex);
            readIndex += 4;

            // データ速度(Byte/sec)
            readIndex += 4;

            // ブロックサイズ
            readIndex += 2;

            // サンプルあたりのビット数(bit/sample)ビットは8 or 16
            bitsPerSample = BitConverter.ToUInt16(buf, readIndex);
            readIndex += 2;

            // dataチャンクまで読み飛ばし
            while (true)
            {
                if (Encoding.ASCII.GetString(buf, readIndex++, 1) != "d") break;
            }
            // ゴリ
            if (Encoding.ASCII.GetString(buf, readIndex, 3) != "ata")
            {
                ExceptionMessage.MessageBox("WAVファイルエラー", "dataチャンク侵入失敗");
                return null;
            }
            readIndex += 3;

            /* dataチャンク侵入 */

            int dataSize = BitConverter.ToInt32(buf, readIndex);
            readIndex += 4;

            // byte→floatに変換するためのラムダ式
            Func<float> ConvertByteToFloat;

            // 8bitならば符号無しunsigned(0 ～ 255, 無音は 128)
            if (bitsPerSample == 8)
            {
                // 8ビットサンプルの場合のfloat変換関数
                ConvertByteToFloat = () =>
                {
                    // 1 / 128
                    return ((int)buf[readIndex] - 0x80) * (1.0f / Mathf.Pow(2, 7));
                };
            }
            // 16bit ならば符号付き signed(-32768 ～ +32767, 無音は 0)
            else if (bitsPerSample == 16)
            {
                // 16ビットサンプルの場合のfloat変換関数
                ConvertByteToFloat = () =>
                {
                    // 1 / 32768
                    short sampleData = System.BitConverter.ToInt16(buf, readIndex);
                    return sampleData * (1.0f / Mathf.Pow(2, 15));
                };
            }
            else
            {
                ExceptionMessage.MessageBox("WAVEファイルエラー", "サンプルおかしい");
                return null;
            }

            // 変換ラムダ式を使ってデータfloat化
            float[] floatBuf = new float[dataSize];
            for (int i = 0; i < dataSize; i++)
            {
                floatBuf[i] = ConvertByteToFloat();
                readIndex++;
            }

            lengthSamples = dataSize / (bitsPerSample / 8) / channels;

            // フォーマット情報からクリップ作成
            AudioClip clip = AudioClip.Create(clipName, 1, channels, samplePerSecond, true);

            // 波形データセット
            clip.SetData(floatBuf, 0);

            return clip;
        }

        catch (System.Exception e)
        {
            ExceptionMessage.Message("wav load error", e);
        }

        return null;
    }
}
