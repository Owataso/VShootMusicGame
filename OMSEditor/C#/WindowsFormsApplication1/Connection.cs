using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication1
{
    class Connection
    {
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr NewSceneMain();
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DeleteSceneMain(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Init(IntPtr p, IntPtr hwnd);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Update(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Render(IntPtr p);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int LoadMIDI(IntPtr p, string sFileName);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int LoadProject(IntPtr p, string sFileName);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int LoadProjectByOMS(IntPtr p, string sFileName);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SaveProject(IntPtr p);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void InportOGG(IntPtr p, string sFileName);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetOMSOpenType(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int isMusicOpend(IntPtr p);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern short GetShift(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetShift(IntPtr p, short iShift);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int PlayMusic(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void StopMusic(IntPtr p);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int StartEdit(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void EndEdit(IntPtr p);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetCurrentPosition(IntPtr p, ulong dwCurrentPosition);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetState(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern ushort GetNumNote(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetNoteData(IntPtr p, StringBuilder outBuf, uint bufSize, ushort noteID, ushort DataType);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetNoteData(IntPtr p, string value, ushort noteID, ushort DataType);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void BaseToVRShooting(IntPtr p);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SelectEditNote(IntPtr p, string indicesString);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern ushort GetEditNoteIndex(IntPtr p);   
          
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void MouseLeftDown(IntPtr p, int x, int y); 
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void MouseLeftDrag(IntPtr p, int x, int y);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void MouseRightDown(IntPtr p, int x, int y);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void MouseRightUp(IntPtr p, int x, int y);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void MouseRightDrag(IntPtr p, int x, int y);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetMaxDegree(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetMaxDegree(IntPtr p, int degree);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetDragFlagVRShoot(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetDragFlagVRShoot(IntPtr p, int dragFlag);

        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void CopyNote(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PasteNote(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ReverseX(IntPtr p);
        [DllImport("./DirectX.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ReverseY(IntPtr p);

        private IntPtr scene = IntPtr.Zero;

        public Connection()
        {
        }

        public void New()
        {
            scene = NewSceneMain();
        }

        public void Initialize(IntPtr hwnd)
        {
            Init(scene, hwnd);
        }

        public void Delete()
        {
            DeleteSceneMain(scene);
        }

        public void SceneUpdate()
        {
            Update(scene);
        }

        public void SceneRender()
        {
            Render(scene);
        }

        public int LoadMIDI(string sFileName)
        {
            return LoadMIDI(scene, sFileName);
        }

        public int LoadProject(string sFileName)
        {
            return LoadProject(scene, sFileName);
        }
        public int LoadProjectByOMS(string sFileName)
        {
            return LoadProjectByOMS(scene, sFileName);
        }
        public void SaveProject()
        {
            SaveProject(scene);
        }

        public void InportOGG(string sFileName)
        {
            InportOGG(scene, sFileName);
        }

        public int GetOMSOpenType()
        {
            return GetOMSOpenType(scene);
        }
        public int isMusicOpend()
        {
            return isMusicOpend(scene);
        }

        public short GetShift()
        {
            return GetShift(scene);
        }
        public void SetShift(short sShift)
        {
            SetShift(scene, sShift);
        }

        public int PlayMusic()
        {
            return PlayMusic(scene);
        }
        public void StopMusic()
        {
            StopMusic(scene);
        }

        public int StartEdit()
        {
            return StartEdit(scene);
        }
        public void EndEdit()
        {
            EndEdit(scene);
        }

        public void SetCurrentPosition(ulong dwCurrentPosition)
        {
            SetCurrentPosition(scene, dwCurrentPosition);
        }

        public int GetState()
        {
            return GetState(scene);
        }
        public ushort GetNumNote()
        {
            return GetNumNote(scene);
        }
        public string GetNoteData(ushort noteID, ushort DataType)
        {
            StringBuilder sb = new StringBuilder(256);
            GetNoteData(scene, sb, (uint)sb.Capacity, noteID, DataType);
            return sb.ToString();

            //return "なし";

            //const int NUM = 256;
            //char[] arr = new char[NUM];

            //// アンマネージ配列のメモリを確保
            //IntPtr ptr = Marshal.AllocCoTaskMem(Marshal.SizeOf(typeof(char)) * NUM);

            //// 引数でポインタを渡す
            //GetNoteData(scene, ptr, NUM, noteID, DataType);

            //// マネージ配列へコピー
            //Marshal.Copy(ptr, arr, 0, NUM);

            //// アンマネージ配列のメモリを解放
            //Marshal.FreeCoTaskMem(ptr);

            //return arr.ToString();
        }
        public void SetNoteData(string value, ushort noteID, ushort DataType)
        {
            SetNoteData(scene, value, noteID, DataType);
        }

        public void BaseToVRShooting()
        {
            BaseToVRShooting(scene);
        }

        public void SelectEditNote(string indicesString)
        {
            SelectEditNote(scene, indicesString);
        }
        public ushort GetEditNoteIndex()
        {
            return GetEditNoteIndex(scene);
        }

        public void MouseLeftDown(int x, int y)
        {
            MouseLeftDown(scene, x, y);
        }
        public void MouseLeftDrag(int x, int y)
        {
            MouseLeftDrag(scene, x, y);
        }
        public void MouseRightDown(int x, int y)
        {
            MouseRightDown(scene, x, y);
        }
        public void MouseRightUp(int x, int y)
        {
            MouseRightUp(scene, x, y);
        }
        public void MouseRightDrag(int x, int y)
        {
            MouseRightDrag(scene, x, y);
        }

        public int GetMaxDegree()
        {
            return GetMaxDegree(scene);
        }
        public void SetMaxDegree(int degree)
        {
            SetMaxDegree(scene, degree);
        }

        public int GetDragFlagVRShoot()
        {
            return GetDragFlagVRShoot(scene);
        }
        public void SetDragFlagVRShoot(int dragFlag)
        {
            SetDragFlagVRShoot(scene, dragFlag);
        }

        public void CopyNote()
        {
            CopyNote(scene);
        }
        public void PasteNote()
        {
            PasteNote(scene);
        }
        public void ReverseX()
        {
            ReverseX(scene);
        }
        public void ReverseY()
        {
            ReverseY(scene);
        }

    }
}