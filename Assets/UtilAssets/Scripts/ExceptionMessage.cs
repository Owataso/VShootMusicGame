public static class ExceptionMessage
{
    public static void Message(string errorStr, System.Exception e)
    {
#if UNITY_EDITOR
        UnityEditor.EditorUtility.DisplayDialog(errorStr, e.GetType().FullName + "\r\nan exception was thrown.", "OK");
#endif
    }

    public static void MessageBox(string errorStr, string Message)
    {
#if UNITY_EDITOR
        UnityEditor.EditorUtility.DisplayDialog(errorStr, Message, "OK");
#endif
    }

}