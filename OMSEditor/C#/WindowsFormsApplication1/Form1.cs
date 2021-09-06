using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        //+------------------------------------------------
        //  定数
        //+------------------------------------------------
        enum OMS_RESULT
        {
            OK,                 // 成功
            FAILED_OPEN,        // ファイルオープン失敗
            FAILED_READ_MIDI,   // MIDI正常に読み込まれなかった
            FAILED_READ_OMS		// OMS正常に読み込まれなかった
        }
        enum OMS_TYPE
        {
            NO_LOAD = -1,   // ロードしてない
            BASE,           // どの譜面にでもできるように汎用性を持たせたもの
            VR_SHOOT,		// VRシューティング用
        };

        enum DRAG_FLAG
        {
            LINE_PLOT,  // ラインのプロット
            X_SCALE,    // 横スケール
            TRANSLATE	// 平行移動
        };

        enum EP_NOTE_DATA_TYPE
        {
            EVENT_TIME, // イベント時間
            NOTE,       // ノーツタイプ
            LANE,       // レーン番号
        };

        enum VRSHOOT_NOTE_DATA_TYPE
        {
            EVENT_TIME,     // イベント時間
            NOTE,           // ノーツタイプ
            DEGREE,         // 中心からの角度(実質x座標)
            RATE_Y,         // Y座標(0～1)
            CN_END_INDEX,   // CN先のノーツのインデックス
        };

        const int FALSE = 0, TRUE = 1;

        //+------------------------------------------------
        //  変数
        //+------------------------------------------------
        bool m_bSaveFlag;                   // セーブフラグ
        bool windowLeftClickFlag;           // 画面外でクリック→画面内で離したときに呼ばれないようにする用
        bool windowRightClickFlag;          // 画面外でクリック→画面内で離したときに呼ばれないようにする用
        string m_sProjectName;              // 開いているプロジェクト名
        Connection cppScene;                // C++のシーン
        UsefulFunctions myFunction;         // 自分用に関数まとめたやつ

        //+------------------------------------------------
        //  初期化
        //+------------------------------------------------
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            cppScene = new Connection();
            cppScene.New();
            cppScene.Initialize(this.Window_Cpp.Handle);

            m_bSaveFlag = false;
            windowLeftClickFlag = windowRightClickFlag = false;
            m_sProjectName = "";

            // 自分用関数
            myFunction = new UsefulFunctions();

            // ホイールイベントの追加
            // ※マウスホイールのイベントのみ、プロパティで設定出来ないので、自分で関数を作って追加してやる必要がある。
            // 下記の引数につっこんでるのは、自分で作ったマウスホイールイベントの関数。
            this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.MyMouseWheelEvent);

            //カラムの設定（1列）
            ListView_Project.Columns.Add("ProjectList");
            foreach (ColumnHeader ch in ListView_Project.Columns)
            {
                ch.Width = 250;
            }

            // フォルダー列挙
            string[] l_sSubFolders = myFunction.EnumDirectory("Projects");
            foreach (string sProjectName in l_sSubFolders)
            {
                ListView_Project.Items.Add(new ListViewItem(myFunction.MakeFileName(sProjectName)));
            }

            ListView_EventData.Columns.Add("Index");
            ListView_EventData.Columns.Add("EventTime(ms)");
            ListView_EventData.Columns.Add("NoteType");
            ListView_EventData.Columns.Add("Info0");
            ListView_EventData.Columns.Add("Info1");
            ListView_EventData.Columns.Add("Info2");
            foreach (ColumnHeader ch in ListView_EventData.Columns)
            {
                ch.Width = 120;
            }

            // スライダー非表示
            SetVRShootTrackBarVisible(false);
        }

        //========================================================================================================================================
        //  フォームが閉じられる「前」に呼ばれるイベント
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_bSaveFlag)
            {
                switch (MessageBox.Show("プロジェクトは変更されています。保存しますか？", "", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question))
                {
                    case DialogResult.Yes:
                        SaveProject();
                        break;

                    case DialogResult.Cancel:
                        // FormClosingEventArgs の Cancel プロパティを true に設定します。
                        e.Cancel = true;
                        return;
                }
            }

            // C++側の開放処理
            cppScene.Delete();
        }

        Point GetCppSceneMousePosition()
        {
            //フォーム上の座標でマウスポインタの位置を取得する
            //画面座標でマウスポインタの位置を取得する
            Point sp = Cursor.Position;
            //画面座標をクライアント座標に変換する
            Point cp = this.PointToClient(sp);

            // 画面に合わせる
            cp.X -= Window_Cpp.Location.X;
            cp.Y -= Window_Cpp.Location.Y;

            cp.X *= 1280 / Window_Cpp.Size.Width;
            cp.Y *= 720 / Window_Cpp.Size.Height;

            return cp;
        }

        //*****************************************************************
        //      フレーム更新処理
        //*****************************************************************

        private void timer1_Tick(object sender, EventArgs e)
        {
            cppScene.SceneUpdate();
            cppScene.SceneRender();
        }

        void OMS_ResutMessage(int iResult)
        {
            string l_sMessage = "";
            switch ((OMS_RESULT)iResult)
            {
                case OMS_RESULT.FAILED_OPEN:
                    //l_sMessage = "ファイルオープンに失敗！";
                    break;

                case OMS_RESULT.FAILED_READ_MIDI:
                    l_sMessage = "MIDIファイルの解析に失敗！\ndominoで書きだしてどうぞ";
                    break;

                case OMS_RESULT.FAILED_READ_OMS:
                    l_sMessage = "OMS or OMTファイルの解析に失敗！";
                    break;
            }

            // 結果がOKじゃなかったらメッセージを出す
            if (l_sMessage != "") MessageBox.Show(l_sMessage, "エラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        bool LoadMIDI(string sFileName)
        {
            // すでに譜面を読み込んでいたら
            if ((OMS_TYPE)cppScene.GetOMSOpenType() != OMS_TYPE.NO_LOAD)
            {
                // 上書きしますかと聞いて、やっぱいいや(Cancel)だったら抜ける
                if (MessageBox.Show("すでに譜面ファイルを読み込んでいます。\r\n上書きしますか？", "", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.Cancel)
                    return false;
            }

            int l_iResult = cppScene.LoadMIDI(sFileName);
            if (l_iResult != (int)OMS_RESULT.OK)
            {
                // エラーメッセージを出す
                OMS_ResutMessage(l_iResult);
                return false;
            }

            var openOMS = (OMS_TYPE)cppScene.GetOMSOpenType();
            switch (openOMS)
            {
                case OMS_TYPE.BASE:
                    RadioButton0_BaseEP.Checked = true;
                    break;
                case OMS_TYPE.VR_SHOOT:
                    RadioButton0_VRShoot.Checked = true;
                    break;
            }

            return true;
        }

        void InportOGG(string sFileName)
        {
            // すでに曲を開いていたら
            if (cppScene.isMusicOpend() == TRUE)
            {
                // 上書きしますかと聞いて、やっぱいいや(Cancel)だったら抜ける
                if (MessageBox.Show("すでに曲ファイルが読み込まれています。\r\n上書きしますか？", "", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.Cancel)
                    return;
            }
            cppScene.InportOGG(sFileName);
        }

        void LoadProject(string sProjectName)
        {
            m_sProjectName = sProjectName;

            string l_sDirectoryPath = "Projects/" + sProjectName;

            // 開いたフラグON
            Label_ProjectName.Text = m_sProjectName;

            // C++側のデータにプロジェクトをロードさせる(OMS読み込み失敗してたらなんかメッセージだす)
            OMS_ResutMessage(cppScene.LoadProject(l_sDirectoryPath));

            OpenOMSCheck();
        }

        void LoadProjectByOMS(string sFileName)
        {
            m_sProjectName = sFileName;

            // 開いたフラグON
            Label_ProjectName.Text = myFunction.MakeFileName(m_sProjectName);

            // C++側のデータにプロジェクトをロードさせる(OMS読み込み失敗してたらなんかメッセージだす)
            OMS_ResutMessage(cppScene.LoadProjectByOMS(sFileName));

            OpenOMSCheck();
        }

        void OpenOMSCheck()
        {
            // Shift値
            var openOMS = (OMS_TYPE)cppScene.GetOMSOpenType();
            switch (openOMS)
            {
                case OMS_TYPE.BASE:
                    RadioButton0_BaseEP.Checked = true;
                    break;
                case OMS_TYPE.VR_SHOOT:
                    RadioButton0_VRShoot.Checked = true;
                    break;
            }

            if (openOMS != OMS_TYPE.NO_LOAD)
            {
                Numeric_Shift.Value = cppScene.GetShift();
            }
        }

        void SaveProject()
        {
            // C++側のデータにプロジェクトをセーブさせる
            cppScene.SaveProject();

            // フラグ解除
            m_bSaveFlag = false;
        }

        bool CheckProjectOpen()
        {
            // プロジェクトが読み込まれていないときの処理
            if(m_sProjectName == "")
            {
                MessageBox.Show("プロジェクトが読み込まれていません。\r\n新規プロジェクトを作成するか既存プロジェクトを開いてください", "エラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            // 正常にプロジェクトが読み込まれている
            return true;
        }

        bool CheckListViewSelect()
        {
            // リストビュー項目が選択されていないときの処理
            if (ListView_Project.SelectedItems.Count <= 0)
            {
                MessageBox.Show("リストビューでプロジェクトを選択してください", "エラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return true;
        }

        void SetVRShootTrackBarVisible(bool visible)
        {
            TrackBar_MaxDegree.Visible =
            Label_MaxDegree.Visible =
            Label_MaxDegreeVal.Visible =
            RadioButton1_LinePlot.Visible =
            RadioButton1_XScale.Visible =
            RadioButton1_Translate.Visible =
            Button_ReverseX.Visible = 
            Button_ReverseY.Visible = 
            //TrackBar_RateY.Visible =
            //Label_RateY.Visible =
            //Label_RateYVal.Visible =
            visible;
        }

        void SetEditVisible(bool visible)
        {

        }

        //========================================================================================================================================
        //  「新規プロジェクト」ボタンクリックイベント
        void NewProjectButton_Click(object sender, EventArgs e)
        {
            string sProjectName = TextBox_NewProject.Text;
            // 入力していない場合
            if (sProjectName == "")
            {
                MessageBox.Show("新規プロジェクト名を入力してください", "エラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // かぶってないかチェック
            // 選択されているリストを取得しリストビューから削除する
            foreach (ListViewItem Item in ListView_Project.SelectedItems)
            {
                if (Item.Text == sProjectName)
                {
                    MessageBox.Show("プロジェクト名が被ってるから別のプロジェクト名にしてね", "エラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }

            // リストに追加
            ListView_Project.Items.Add(new ListViewItem(sProjectName));

            // フォルダ作成
            myFunction.CreateDirectory("Projects/" + sProjectName);

            // プロジェクトロード
            LoadProject(sProjectName);
        }

        //========================================================================================================================================
        //  「プロジェクトからロード」ボタンクリックイベント
        void ProjectLoadButton_Click(object sender, EventArgs e)
        {
            // プロジェクト選択してなかったら出ていけぇ！！
            if (!CheckListViewSelect()) return;

            ListViewItem l_ListViewItem = new ListViewItem();

            // 選択アイテムを格納
            l_ListViewItem = ListView_Project.SelectedItems[0];

            // プロジェクトロード
            LoadProject(l_ListViewItem.Text);
        }

        //========================================================================================================================================
        //  「プロジェクト保存」ボタンクリックイベント
        void Button_SaveProject_Click(object sender, EventArgs e)
        {
            // プロジェクト開いてなかったら出ていけぇ！！
            if (!CheckProjectOpen()) return;

            // プロジェクト保存
            SaveProject();
        }

        //  「プロジェクト削除」ボタンクリックイベント
        void Button_DeleteProject_Click(object sender, EventArgs e)
        {
            // プロジェクト選択してなかったら出ていけぇ！！
            if (!CheckListViewSelect()) return;

            ListViewItem l_ListViewItem = new ListViewItem();

            // 選択アイテムを格納
            l_ListViewItem = ListView_Project.SelectedItems[0];

            switch (MessageBox.Show("プロジェクト [" + l_ListViewItem.Text + "] を削除します。\r\nよろしいですか？", "", MessageBoxButtons.OKCancel, MessageBoxIcon.Question))
            {
                case DialogResult.OK:
                    // フォルダ削除
                    myFunction.DeleteDirectory("Projects/" + l_ListViewItem.Text);
                    // リストビュー削除
                    ListView_Project.Items.Remove(l_ListViewItem);
                    break;
            }
    
        }

        //========================================================================================================================================
        //  「保存」ボタンクリックイベント
        void SaveButton_Click(object sender, EventArgs e)
        {
            // セーブファイルダイアログ作成
            SaveFileDialog save = new SaveFileDialog();

            // フィルター(ここで保存するファイルの形式を決める)
            save.Filter = "PNG File (*.png)|*.png";

            // OK以外ならリターン
            if (save.ShowDialog() != DialogResult.OK) return;

            /*save.FileName;*/ // この変数名にファイルパスが入ってる！！
        }

        //========================================================================================================================================
        //  「開く」ボタンクリックイベント
        void LoadButton_Click(object sender, EventArgs e)
        {

            // オープンファイルダイアログ作成
            OpenFileDialog open = new OpenFileDialog();

            // フィルター(ここで開くファイルを絞り込む)
            open.Filter = "PNG File (*.png)|*.png";

            // OK以外ならリターン
            if (open.ShowDialog() != DialogResult.OK) return;

            // クラスのリストに格納
            //AddClassList(open.FileName);

        }

        //========================================================================================================================================
        //  Drag&Drop「範囲内に入る」
        void Form1_DragEnter(object sender, DragEventArgs e)
        {
            //コントロール内にドラッグされたとき実行される
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                //ドラッグされたデータ形式を調べ、ファイルのときはコピーとする
                e.Effect = DragDropEffects.Copy;
            else
                //ファイル以外は受け付けない
                e.Effect = DragDropEffects.None;
        }

        //========================================================================================================================================
        //  Drag&Drop「範囲内に入ってファイルが放り投げられたとき」
        void Form1_DragDrop(object sender, DragEventArgs e)
        {
            // ドラッグ＆ドロップされたファイルパスを取得
            string[] PathList = (string[])e.Data.GetData(DataFormats.FileDrop);

            //// ファイルを列挙(複数のファイルがドロップされたのを想定して)。もちろん1個だけでも大丈夫
            //for (int i = 0; i < PathList.Length; i++)
            //{
            //    AddClassList(PathList[i]);
            //}

            // ドラッグ＆ドロップされたファイルの拡張子取得
            string l_sExtention = myFunction.MakeFilenameExtension(PathList[0]);
            switch (l_sExtention)
            {
                case "mid":/* MIDIファイル */
                           // プロジェクト開いてなかったら出ていけぇ！！
                    if (!CheckProjectOpen()) return;
                    if (!LoadMIDI(PathList[0])) return;
                    m_bSaveFlag = true;
                    break;

                case "ogg":/* oggファイル */
                           // プロジェクト開いてなかったら出ていけぇ！！
                    if (!CheckProjectOpen()) return;
                    InportOGG(PathList[0]);
                    m_bSaveFlag = true;
                    break;

                case "omt":/* 独自テキスト譜面ファイル */
                    LoadProjectByOMS(PathList[0]);
                    break;

                default:
                    MessageBox.Show("意図されないファイル形式がドラッグ&ドロップされました", "エラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
            }

        }

        //========================================================================================================================================
        //  MIDI読み込みボタン
        void MIDIImportButton_Click(object sender, EventArgs e)
        {
            // プロジェクト開いてなかったら出ていけぇ！！
            if (!CheckProjectOpen()) return;

            // オープンファイルダイアログ作成
            OpenFileDialog open = new OpenFileDialog();

            // フィルター(ここで開くファイルを絞り込む)
            open.Filter =
                "Standard MIDI File (*.mid)|*.mid";

            // OK以外ならリターン
            if (open.ShowDialog() != DialogResult.OK) return;

            // MIDIファイルオープン&失敗してたらリターン
            if (!LoadMIDI(open.FileName)) return;

            m_bSaveFlag = true;
        }

        //========================================================================================================================================
        //  OGG読み込みボタン
        void Button_OGGLoad_Click(object sender, EventArgs e)
        {
            // プロジェクト開いてなかったら出ていけぇ！！
            if (!CheckProjectOpen()) return;

            // オープンファイルダイアログ作成
            OpenFileDialog open = new OpenFileDialog();

            // フィルター(ここで開くファイルを絞り込む)
            open.Filter =
                "Ogg Vorbis File (*.ogg)|*.ogg";

            // OK以外ならリターン
            if (open.ShowDialog() != DialogResult.OK) return;

            // OGGインポート
            cppScene.InportOGG(open.FileName);

            m_bSaveFlag = true;
        }

        /* 自分で作ったマウスホイールイベント */
        void MyMouseWheelEvent(object sender, MouseEventArgs e)
        {
            // [↑]
            //if (e.Delta > 0)    scene.UpWheelEvent();

            // [↓]
            //else                scene.DownWheelEvent();
        }

        //========================================================================================================================================
        //  Shift値変更
        void Numeric_Shift_ValueChanged(object sender, EventArgs e)
        {
            // プロジェクト開いてなかったら出ていけぇ！！
            if (!CheckProjectOpen()) return;

            cppScene.SetShift(Decimal.ToInt16(Numeric_Shift.Value));

            //m_bSaveFlag = true;
        }

        void Button_PlayMusic_Click(object sender, EventArgs e)
        {
            // プロジェクト開いてなかったら出ていけぇ！！
            if (!CheckProjectOpen()) return;

            if (cppScene.PlayMusic() == FALSE)
            {
                MessageBox.Show("曲と譜面の両方をセットしてください(もしくはすでに再生中)", "エラー", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        void Button_StartEdit_Click(object sender, EventArgs e)
        {
            // プロジェクト開いてなかったら出ていけぇ！！
            if (!CheckProjectOpen()) return;

            // c++のエディット開始(例外なら抜ける)
            if (cppScene.StartEdit() == FALSE) return;

            var numNote = cppScene.GetNumNote();
            for (ushort i = 0; i < numNote; i++)
            {
                string[] row = new string[6];

                // インデックス
                row[0] = i.ToString();


                for (ushort j = 0; j < 5; j++)
                {
                    row[j + 1] = cppScene.GetNoteData(i, j);
                }
                ListView_EventData.Items.Add(new ListViewItem(row));
            }
        }

        void Button_EndEdit_Click(object sender, EventArgs e)
        {
            // プロジェクト開いてなかったら出ていけぇ！！
            if (!CheckProjectOpen()) return;

            // c++のエディット終了
            cppScene.EndEdit();

            // リストビュークリア(Clear関数だと、Column情報も消えてしまうので不採用)
            foreach (ListViewItem item in ListView_EventData.Items)
            {
                ListView_EventData.Items.Remove(item);
            }

            // スライダー非表示
            if((OMS_TYPE)cppScene.GetOMSOpenType() == OMS_TYPE.VR_SHOOT)
            {
                SetVRShootTrackBarVisible(false);
            }
        }

        void RadioButton_BaseEP_Click(object sender, EventArgs e)
        {
            // プロジェクト開いてなかったら
            if (!CheckProjectOpen())
            {
                // チェックを外す
                RadioButton0_BaseEP.Checked = false;
                return;
            }
        }

        void RadioButton_VRShoot_Click(object sender, EventArgs e)
        {
            // プロジェクト開いてなかったら
            if (!CheckProjectOpen())
            {
                // チェックを外す
                RadioButton0_VRShoot.Checked = false;
                return;
            }

            if((OMS_TYPE)cppScene.GetOMSOpenType() == OMS_TYPE.BASE)
            {
                switch (MessageBox.Show("VRシューティング譜面に切り替えます。実行しますか？", "", MessageBoxButtons.OKCancel, MessageBoxIcon.Question))
                {
                    case DialogResult.OK:
                        // 変換
                        cppScene.BaseToVRShooting();
                        m_bSaveFlag = true;
                        break;

                    case DialogResult.Cancel:
                        // チェックを外す
                        RadioButton0_VRShoot.Checked = false;
                        return;
                }
            }
        }

        void ListView_EventData_Click(object sender, EventArgs e)
        {
            // 複数選択のみ対応
            var numIndices = ListView_EventData.SelectedIndices.Count;
            if (numIndices <= 1) return;

            string indicesString = "";
            foreach (int index in ListView_EventData.SelectedIndices)
            {
                indicesString += index + ",";
            }
            cppScene.SelectEditNote(indicesString);
        }

        void ListView_EventData_SelectedIndexChanged(object sender, EventArgs e)
        {
            // 2回呼びもしくは複数選択なら抜ける
            var numIndices = ListView_EventData.SelectedIndices.Count;
            if (numIndices == 0 || numIndices > 1) return;

            ushort selectIndex = (ushort)ListView_EventData.SelectedIndices[0];
            string indicesString = selectIndex + ",";
            cppScene.SelectEditNote(indicesString);

            // スライダーの設定
            if ((OMS_TYPE)cppScene.GetOMSOpenType() == OMS_TYPE.VR_SHOOT)
            {
                // スライダー非表示
                SetVRShootTrackBarVisible(true);

                // ラベル変更
                // 角度
                //string stringValue = cppScene.GetNoteData(selectIndex, (ushort)VRSHOOT_NOTE_DATA_TYPE.DEGREE);
                //float floatValue = float.Parse(stringValue);
                //TrackBar_Degree.Value = (int)(floatValue * 100);
                //Label_DegreeVal.Text = floatValue.ToString();

                // トラックバー
                int degree = cppScene.GetMaxDegree();
                int trackValue = 0;
                for (int i = degree; i != 90; i /= 2) trackValue++;
                TrackBar_MaxDegree.Value = trackValue;
                Label_MaxDegreeVal.Text = degree.ToString();

                //// Yレート
                //stringValue = cppScene.GetNoteData(selectIndex, (ushort)VRSHOOT_NOTE_DATA_TYPE.RATE_Y);
                //floatValue = float.Parse(stringValue);
                //TrackBar_RateY.Value = (int)(floatValue * 100);
                //Label_RateYVal.Text = floatValue.ToString();
            }
        }

        //private void TrackBar_RateY_Scroll(object sender, EventArgs e)
        //{
        //    ushort selectIndex = cppScene.GetEditNoteIndex();

        //    float floatValue = TrackBar_RateY.Value * 0.01f;
        //    string stringValue = floatValue.ToString();

        //    // cppにデータ設定
        //    cppScene.SetNoteData(stringValue, selectIndex, (ushort)VRSHOOT_NOTE_DATA_TYPE.RATE_Y);

        //    // リストビューの値設定
        //    ListView_EventData.Items[selectIndex].SubItems[1 + (int)VRSHOOT_NOTE_DATA_TYPE.RATE_Y].Text = stringValue;

        //    // ラベル文字設定
        //    Label_RateYVal.Text = stringValue;
        //}

        //private void TrackBar_Degree_Scroll(object sender, EventArgs e)
        //{
        //    ushort selectIndex = cppScene.GetEditNoteIndex();

        //    float floatValue = TrackBar_Degree.Value * 0.01f;
        //    string stringValue = floatValue.ToString();

        //    // cppにデータ設定
        //    cppScene.SetNoteData(stringValue, selectIndex, (ushort)VRSHOOT_NOTE_DATA_TYPE.DEGREE);

        //    // リストビューの値設定
        //    ListView_EventData.Items[selectIndex].SubItems[1 + (int)VRSHOOT_NOTE_DATA_TYPE.DEGREE].Text = stringValue;

        //    // ラベル文字設定
        //    Label_MaxDegreeVal.Text = stringValue;
        //}

        void TrackBar_MaxDegree_Scroll(object sender, EventArgs e)
        {
            //ushort selectIndex = cppScene.GetEditNoteIndex();

            //float floatValue = TrackBar_Degree.Value * 0.01f;
            //string stringValue = floatValue.ToString();

            int degree = 90;
            for (int i = 0; i < TrackBar_MaxDegree.Value; i++) degree *= 2; // 0→90,1→180,2→360

            // cppにデータ設定
            cppScene.SetMaxDegree(degree);

            // リストビューの値設定
            //ListView_EventData.Items[selectIndex].SubItems[1 + (int)VRSHOOT_NOTE_DATA_TYPE.DEGREE].Text = stringValue;

            // ラベル文字設定
            Label_MaxDegreeVal.Text = degree.ToString();
        }

        void Window_Cpp_MouseDown(object sender, MouseEventArgs e)
        {
            if ((MouseButtons & MouseButtons.Middle) == MouseButtons.Middle)
            {
                //Console.WriteLine("マウスの中央ボタンが押されています。");
                return;
            }

            if((MouseButtons & MouseButtons.Left) == MouseButtons.Left)
            {
                windowLeftClickFlag = true;

                // cppウィンドウ上でのマウス座標を取得
                Point cp = GetCppSceneMousePosition();
                // cppに設定
                cppScene.MouseLeftDown(cp.X, cp.Y);
                return;
            }

            if ((MouseButtons & MouseButtons.Right) == MouseButtons.Right)
            {
                windowRightClickFlag = true;

                // cppウィンドウ上でのマウス座標を取得
                Point cp = GetCppSceneMousePosition();
                // cppに設定
                cppScene.MouseRightDown(cp.X, cp.Y);
                return;
            }
        }

        void Window_Cpp_MouseUp(object sender, MouseEventArgs e)
        {
            if (windowLeftClickFlag)
            {
                if ((MouseButtons & MouseButtons.Left) != MouseButtons.Left)
                {
                    windowLeftClickFlag = false;
                    return;
                }
            }
            if (windowRightClickFlag)
            {
                if ((MouseButtons & MouseButtons.Right) != MouseButtons.Right)
                {
                    windowRightClickFlag = false;
                    // cppウィンドウ上でのマウス座標を取得
                    Point cp = GetCppSceneMousePosition();
                    // cppに設定
                    cppScene.MouseRightUp(cp.X, cp.Y);
                    return;
                }
            }
        }

        void Window_Cpp_MouseMove(object sender, MouseEventArgs e)
        {
            if (windowLeftClickFlag)
            {
                // 左クリック離してたら抜ける
                if ((MouseButtons & MouseButtons.Left) != MouseButtons.Left)
                {
                    windowLeftClickFlag = false;
                    return;
                }

                // cppウィンドウ上でのマウス座標を取得
                Point cp = GetCppSceneMousePosition();
                // cppに設定
                cppScene.MouseLeftDrag(cp.X, cp.Y);
            }

            if (windowRightClickFlag)
            {
                // 左クリック離してたら抜ける
                if ((MouseButtons & MouseButtons.Right) != MouseButtons.Right)
                {
                    windowRightClickFlag = false;
                    return;
                }

                // cppウィンドウ上でのマウス座標を取得
                Point cp = GetCppSceneMousePosition();
                // cppに設定
                cppScene.MouseRightDrag(cp.X, cp.Y);
            }
        }

        void RadioButton1_LinePlot_CheckedChanged(object sender, EventArgs e)
        {
            cppScene.SetDragFlagVRShoot((int)DRAG_FLAG.LINE_PLOT);
        }

        void RadioButton1_XScale_CheckedChanged(object sender, EventArgs e)
        {
            cppScene.SetDragFlagVRShoot((int)DRAG_FLAG.X_SCALE);
        }

        void RadioButton1_Translate_CheckedChanged(object sender, EventArgs e)
        {
            cppScene.SetDragFlagVRShoot((int)DRAG_FLAG.TRANSLATE);
        }

        private void ListView_EventData_KeyDown(object sender, KeyEventArgs e)
        {
            // Ctrl + C
            if (e.KeyData == (Keys.Control | Keys.C)) cppScene.CopyNote();
            else if (e.KeyData == (Keys.Control | Keys.V)) cppScene.PasteNote(); 
        }

        private void Button_ReverseX_Click(object sender, EventArgs e)
        {
            cppScene.ReverseX();
        }

        private void Button_ReverseY_Click(object sender, EventArgs e)
        {
            cppScene.ReverseY();
        }

        void Button_StopMusic_Click(object sender, EventArgs e)
        {
            // プロジェクト開いてなかったら出ていけぇ！！
            if (!CheckProjectOpen()) return;

            cppScene.StopMusic();
        }
    }
}