using UnityEngine;
using System.Collections;

public class Singleton<T> where T : class, new()
{
    static T m_Instance = null;
    public static T Instance { get { if (m_Instance == null) { m_Instance = new T(); } return m_Instance; } }
}