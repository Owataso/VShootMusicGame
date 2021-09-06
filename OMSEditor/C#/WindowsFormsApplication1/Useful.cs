using System;
using System.IO;

namespace WindowsFormsApplication1
{
    public class UsefulFunctions
    {
        //========================================================================================================================================
        //  引数のファイル名から拡張子だけを取り出して返す関数  e.g.(filename:"hoge.txt" → return "txt")
        public string MakeFilenameExtension(string filename)
        {
            string ret = "";

            // string→char[]
            char[] work = filename.ToCharArray();

            // 最後の.まで
            int DotPos = work.Length - 1;
            for (; work[DotPos] != '.'; DotPos--);

            // .以降を格納
            for (int i= DotPos + 1; i < work.Length; i++)
            {
                ret += work[i];
            }

            return ret;
        }

        //========================================================================================================================================
        //  引数のファイル名からファイルパスだけを取り出して返す関数  e.g.(filename:"C:\\Users\\folder\\hoge.mp3" → return "C:\\Users\\folder\\")
        public string MakeFilepath(string filename)
        {
            string ret = "";

            // string→char[]
            char[] work = filename.ToCharArray();
            int thrash_pos = work.Length - 1;    // 最後の\\の座標を探す

            // 始点から最後のスラッシュまで距離を作る
            while (!work[thrash_pos].Equals('\\'))
            {
                thrash_pos--;
            }

            // その距離分までの文字を入れていく
            for (int i = 0; i < thrash_pos; i++)
            {
                ret += work[i];
            }
            return ret;
        }

        //========================================================================================================================================
        //  引数のファイルパスからファイル名だけを取り出して返す関数  e.g.(filename:"C:\\Users\\folder\\hoge.mp3" → return "hoge.mp3")
        public string MakeFileName(string filename)
        {
            string ret = "";

            // string→char[]
            char[] work = filename.ToCharArray();
            int thrash_pos = work.Length - 1;    // 最後の\\の座標を探す

            // 始点から最後のスラッシュまで距離を作る
            while (!work[thrash_pos - 1].Equals('\\')) // @はquoted string といい、\関連のめんどくさいのをなくしてくれる
            {
                thrash_pos--;
            }

            // その距離分までの文字を入れていく
            for (int i = thrash_pos; i < work.Length; i++)
            {
                ret += work[i];
            }
            return ret;
        }

        //========================================================================================================================================
        //  引数のファイル名の拡張子変えて渡す(.は要らない) "...hoge.mp3", "wav"と入れる
        public string ChangeFilenameExtention(string filename, string next_filename_extention)
        {
            // string→char[]
            char[] char_filename = filename.ToCharArray();
            int dot_pos = char_filename.Length - 1;

            // .まで距離を作る
            while (!char_filename[dot_pos - 1].Equals('.'))
            {
                dot_pos--;
            }

            // 拡張子だけを取り除いたファイル名
            char[] work = new char[dot_pos];
            Array.Copy(char_filename, 0, work, 0, dot_pos);

            // に引数の拡張子にくっつける
            char[] mergedArray = new char[work.Length + next_filename_extention.ToCharArray().Length];
            work.CopyTo(mergedArray, 0);    // 入れ物の最初から(0)
            next_filename_extention.ToCharArray().CopyTo(mergedArray, work.Length);

            return new string(mergedArray);
        }

        //========================================================================================================================================
        //  絶対パス→相対パス
        public string Absolute_to_Relative(string absolute_path)
        {
            string basePath = Directory.GetCurrentDirectory();

            //"%"を"%25"に変換しておく（デコード対策）
            basePath = basePath.Replace("%", "%25");
            absolute_path = absolute_path.Replace("%", "%25");

            //相対パスを取得する
            Uri u1 = new Uri(basePath);
            Uri u2 = new Uri(absolute_path);
            Uri relativeUri = u1.MakeRelativeUri(u2);
            string relativePath = relativeUri.ToString();

            //URLデコードする（エンコード対策）
            relativePath = Uri.UnescapeDataString(relativePath);

            //"%25"を"%"に戻す
            relativePath = relativePath.Replace("%25", "%");

            char[] work = relativePath.ToCharArray();
            relativePath = "";

            int thrash_pos = 0;
            while (!work[thrash_pos].Equals('/'))
            {
                thrash_pos++;
            }
            for (int i = thrash_pos + 1; i < work.Length; i++)
            {
                relativePath += work[i];
            }

            return relativePath;
        }

        public byte[] BinaryRead(string file_path)
        {
            //return File.ReadAllBytes(file_path);  // 楽だけど、膨大な量のデータが怖い

            FileStream fs = new FileStream(file_path, FileMode.Open, FileAccess.Read);

            int fileSize = (int)fs.Length; // ファイルのサイズ
            byte[] buf = new byte[fileSize]; // データ格納用配列

            int readSize; // Readメソッドで読み込んだバイト数
            int remain = fileSize; // 読み込むべき残りのバイト数
            int bufPos = 0; // データ格納用配列内の追加位置

            while (remain > 0)
            {
                // 1024Bytesずつ読み込む
                readSize = fs.Read(buf, bufPos, Math.Min(1024, remain));

                bufPos += readSize;
                remain -= readSize;
            }
            fs.Dispose();

            return buf;
        }

        public UInt32 BinaryWrite(BinaryWriter bw, byte[] write_data, UInt32 data_pos)
        {
            int fileSize = (int)write_data.Length; // ファイルのサイズ

            int remain = fileSize; // 書き込むべき残りのバイト数
            int writeDataPos = 0;  // 書き込むデータ自体の位置(ちびちび分けて書き出すため)

            while (remain > 0)
            {
                // 書き込み開始位置の指定(いちいち指定してやらないといけない)
                bw.Seek((int)data_pos, SeekOrigin.Begin);

                // 1024Bytesずつ書き込む(メモリパンクさせないように)
                int writeSize = Math.Min(1024, remain);
                bw.Write(write_data, writeDataPos, writeSize);

                writeDataPos += writeSize;
                remain -= writeSize;

                data_pos += (UInt32)writeSize;
            }

            // これだと全部一気にやるので膨大なデータ量の書き出しが怖い
            //bw.Seek((int)data_pos, SeekOrigin.Begin);    // 開始位置からのデータ
            //bw.Write(write_data);
            //
            //data_pos += (UInt32)write_data.Length;

            return data_pos;
        }

        public string[] EnumDirectory(string sDirectory)
        {
            // 変数sDirectory以下のサブフォルダをすべて取得する
            // ワイルドカード"*"は、すべてのフォルダを意味する
            return System.IO.Directory.GetDirectories(sDirectory, "*", System.IO.SearchOption.AllDirectories);
        }

        //========================================================================================================================================
        //  ディレクトリ(フォルダ)作成  "DATA/Sub"でSubフォルダ作成
        public void CreateDirectory(string sDirectory)
        {
            //フォルダ"C:\TEST\SUB"を作成する
            //"C:\TEST"フォルダが存在しなくても"C:\TEST\SUB"が作成される
            //"C:\TEST\SUB"が存在していると、IOExceptionが発生
            //アクセス許可が無いと、UnauthorizedAccessExceptionが発生
            System.IO.DirectoryInfo l_DirectoryInfo = System.IO.Directory.CreateDirectory(sDirectory);
        }

        public void DeleteDirectory(string sDirectory)
        {
            //フォルダ"C:\TEST"を削除する
            //第2項をTrueにすると、"C:\TEST"を根こそぎ（サブフォルダ、ファイルも）削除する
            //"C:\TEST"に読み取り専用ファイルがあると、UnauthorizedAccessExceptionが発生
            //"C:\TEST"が存在しないと、DirectoryNotFoundExceptionが発生
            System.IO.Directory.Delete(sDirectory, true);
        }
    }
}
