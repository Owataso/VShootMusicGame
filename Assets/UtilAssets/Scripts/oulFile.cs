public static class oulFile
{
    static public string[] EnumDirectory(string sDirectory)
    {
        //string cdir = Directory.GetCurrentDirectory();
        //string[] files = Directory.GetFiles(cdir);
        //string[] dirs = Directory.GetDirectories(cdir);

        // 変数sDirectory以下のサブフォルダをすべて取得する
        // ワイルドカード"*"は、すべてのフォルダを意味する
        string[] ret = System.IO.Directory.GetDirectories(sDirectory, "*", System.IO.SearchOption.AllDirectories);

        // ディレクトリパスまで入っているので、フォルダー名だけにする
        for (int i = 0; i < ret.Length; i++) ret[i] = System.IO.Path.GetFileName(ret[i]);

        return ret;
    }

    static public byte[] ReadAllBytes(string path)
    {
        try
        {
            byte[] buf;
            // バイナリオープン
            using (System.IO.FileStream file = new System.IO.FileStream(path, System.IO.FileMode.Open, System.IO.FileAccess.Read))
            using (System.IO.BinaryReader reader = new System.IO.BinaryReader(file))
            {
                buf = reader.ReadBytes((int)reader.BaseStream.Length);
            }

            return buf;
        }

        catch (System.Exception e)
        {
            ExceptionMessage.Message("file error", e);
        }

        return null;
    }
}
