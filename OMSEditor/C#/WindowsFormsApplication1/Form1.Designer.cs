namespace WindowsFormsApplication1
{
    partial class Form1
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.Window_Cpp = new System.Windows.Forms.Panel();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.Button_MIDIImport = new System.Windows.Forms.Button();
            this.Button_OGGLoad = new System.Windows.Forms.Button();
            this.ListView_Project = new System.Windows.Forms.ListView();
            this.Button_LoadProject = new System.Windows.Forms.Button();
            this.Button_NewProject = new System.Windows.Forms.Button();
            this.TextBox_NewProject = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.Label_ProjectName = new System.Windows.Forms.Label();
            this.Numeric_Shift = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.Button_PlayMusic = new System.Windows.Forms.Button();
            this.Button_StopMusic = new System.Windows.Forms.Button();
            this.Button_SaveProject = new System.Windows.Forms.Button();
            this.Button_DeleteProject = new System.Windows.Forms.Button();
            this.ListView_EventData = new System.Windows.Forms.ListView();
            this.Button_StartEdit = new System.Windows.Forms.Button();
            this.Button_EndEdit = new System.Windows.Forms.Button();
            this.RadioButton0_BaseEP = new System.Windows.Forms.RadioButton();
            this.RadioButton0_VRShoot = new System.Windows.Forms.RadioButton();
            this.GroupBox_Humen = new System.Windows.Forms.GroupBox();
            this.GroupBox_Project = new System.Windows.Forms.GroupBox();
            this.Label_MaxDegree = new System.Windows.Forms.Label();
            this.Label_MaxDegreeVal = new System.Windows.Forms.Label();
            this.TrackBar_MaxDegree = new System.Windows.Forms.TrackBar();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.RadioButton1_LinePlot = new System.Windows.Forms.RadioButton();
            this.RadioButton1_XScale = new System.Windows.Forms.RadioButton();
            this.RadioButton1_Translate = new System.Windows.Forms.RadioButton();
            this.Button_ReverseY = new System.Windows.Forms.Button();
            this.Button_ReverseX = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.Numeric_Shift)).BeginInit();
            this.GroupBox_Humen.SuspendLayout();
            this.GroupBox_Project.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TrackBar_MaxDegree)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // Window_Cpp
            // 
            this.Window_Cpp.Location = new System.Drawing.Point(16, 16);
            this.Window_Cpp.Margin = new System.Windows.Forms.Padding(4);
            this.Window_Cpp.Name = "Window_Cpp";
            this.Window_Cpp.Size = new System.Drawing.Size(640, 360);
            this.Window_Cpp.TabIndex = 3;
            this.Window_Cpp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Window_Cpp_MouseDown);
            this.Window_Cpp.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Window_Cpp_MouseMove);
            this.Window_Cpp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Window_Cpp_MouseUp);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 10;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // Button_MIDIImport
            // 
            this.Button_MIDIImport.Location = new System.Drawing.Point(40, 24);
            this.Button_MIDIImport.Margin = new System.Windows.Forms.Padding(4);
            this.Button_MIDIImport.Name = "Button_MIDIImport";
            this.Button_MIDIImport.Size = new System.Drawing.Size(128, 32);
            this.Button_MIDIImport.TabIndex = 0;
            this.Button_MIDIImport.Text = "MIDI_Load";
            this.Button_MIDIImport.UseVisualStyleBackColor = true;
            this.Button_MIDIImport.Click += new System.EventHandler(this.MIDIImportButton_Click);
            // 
            // Button_OGGLoad
            // 
            this.Button_OGGLoad.Location = new System.Drawing.Point(40, 61);
            this.Button_OGGLoad.Margin = new System.Windows.Forms.Padding(4);
            this.Button_OGGLoad.Name = "Button_OGGLoad";
            this.Button_OGGLoad.Size = new System.Drawing.Size(128, 32);
            this.Button_OGGLoad.TabIndex = 2;
            this.Button_OGGLoad.Text = "OGG_Load";
            this.Button_OGGLoad.UseVisualStyleBackColor = true;
            this.Button_OGGLoad.Click += new System.EventHandler(this.Button_OGGLoad_Click);
            // 
            // ListView_Project
            // 
            this.ListView_Project.Location = new System.Drawing.Point(17, 36);
            this.ListView_Project.MultiSelect = false;
            this.ListView_Project.Name = "ListView_Project";
            this.ListView_Project.Size = new System.Drawing.Size(148, 360);
            this.ListView_Project.TabIndex = 1;
            this.ListView_Project.UseCompatibleStateImageBehavior = false;
            this.ListView_Project.View = System.Windows.Forms.View.Details;
            // 
            // Button_LoadProject
            // 
            this.Button_LoadProject.Location = new System.Drawing.Point(172, 181);
            this.Button_LoadProject.Margin = new System.Windows.Forms.Padding(4);
            this.Button_LoadProject.Name = "Button_LoadProject";
            this.Button_LoadProject.Size = new System.Drawing.Size(143, 40);
            this.Button_LoadProject.TabIndex = 5;
            this.Button_LoadProject.Text = "選択プロジェクト読込";
            this.Button_LoadProject.UseVisualStyleBackColor = true;
            this.Button_LoadProject.Click += new System.EventHandler(this.ProjectLoadButton_Click);
            // 
            // Button_NewProject
            // 
            this.Button_NewProject.Location = new System.Drawing.Point(172, 97);
            this.Button_NewProject.Margin = new System.Windows.Forms.Padding(4);
            this.Button_NewProject.Name = "Button_NewProject";
            this.Button_NewProject.Size = new System.Drawing.Size(143, 40);
            this.Button_NewProject.TabIndex = 4;
            this.Button_NewProject.Text = "新規プロジェクト作成";
            this.Button_NewProject.UseVisualStyleBackColor = true;
            this.Button_NewProject.Click += new System.EventHandler(this.NewProjectButton_Click);
            // 
            // TextBox_NewProject
            // 
            this.TextBox_NewProject.Location = new System.Drawing.Point(172, 68);
            this.TextBox_NewProject.Name = "TextBox_NewProject";
            this.TextBox_NewProject.Size = new System.Drawing.Size(143, 22);
            this.TextBox_NewProject.TabIndex = 6;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.label1.Location = new System.Drawing.Point(168, 45);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(155, 20);
            this.label1.TabIndex = 7;
            this.label1.Text = "新規プロジェクト名";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("MS UI Gothic", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.label2.Location = new System.Drawing.Point(664, 281);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(264, 34);
            this.label2.TabIndex = 8;
            this.label2.Text = "編集プロジェクト名";
            // 
            // Label_ProjectName
            // 
            this.Label_ProjectName.AutoSize = true;
            this.Label_ProjectName.Font = new System.Drawing.Font("MS UI Gothic", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.Label_ProjectName.ForeColor = System.Drawing.SystemColors.Highlight;
            this.Label_ProjectName.Location = new System.Drawing.Point(678, 332);
            this.Label_ProjectName.Name = "Label_ProjectName";
            this.Label_ProjectName.Size = new System.Drawing.Size(69, 34);
            this.Label_ProjectName.TabIndex = 9;
            this.Label_ProjectName.Text = "なし";
            // 
            // Numeric_Shift
            // 
            this.Numeric_Shift.Increment = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.Numeric_Shift.Location = new System.Drawing.Point(684, 641);
            this.Numeric_Shift.Maximum = new decimal(new int[] {
            32767,
            0,
            0,
            0});
            this.Numeric_Shift.Minimum = new decimal(new int[] {
            32768,
            0,
            0,
            -2147483648});
            this.Numeric_Shift.Name = "Numeric_Shift";
            this.Numeric_Shift.Size = new System.Drawing.Size(120, 22);
            this.Numeric_Shift.TabIndex = 11;
            this.Numeric_Shift.ValueChanged += new System.EventHandler(this.Numeric_Shift_ValueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.label4.Location = new System.Drawing.Point(681, 621);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(133, 15);
            this.label4.TabIndex = 12;
            this.label4.Text = "譜面のずれ調整(ms)";
            // 
            // Button_PlayMusic
            // 
            this.Button_PlayMusic.Location = new System.Drawing.Point(40, 22);
            this.Button_PlayMusic.Margin = new System.Windows.Forms.Padding(4);
            this.Button_PlayMusic.Name = "Button_PlayMusic";
            this.Button_PlayMusic.Size = new System.Drawing.Size(128, 32);
            this.Button_PlayMusic.TabIndex = 13;
            this.Button_PlayMusic.Text = "譜面の再生";
            this.Button_PlayMusic.UseVisualStyleBackColor = true;
            this.Button_PlayMusic.Click += new System.EventHandler(this.Button_PlayMusic_Click);
            // 
            // Button_StopMusic
            // 
            this.Button_StopMusic.Location = new System.Drawing.Point(40, 61);
            this.Button_StopMusic.Margin = new System.Windows.Forms.Padding(4);
            this.Button_StopMusic.Name = "Button_StopMusic";
            this.Button_StopMusic.Size = new System.Drawing.Size(128, 32);
            this.Button_StopMusic.TabIndex = 14;
            this.Button_StopMusic.Text = "停止";
            this.Button_StopMusic.UseVisualStyleBackColor = true;
            this.Button_StopMusic.Click += new System.EventHandler(this.Button_StopMusic_Click);
            // 
            // Button_SaveProject
            // 
            this.Button_SaveProject.Location = new System.Drawing.Point(172, 335);
            this.Button_SaveProject.Margin = new System.Windows.Forms.Padding(4);
            this.Button_SaveProject.Name = "Button_SaveProject";
            this.Button_SaveProject.Size = new System.Drawing.Size(143, 40);
            this.Button_SaveProject.TabIndex = 15;
            this.Button_SaveProject.Text = "プロジェクト保存";
            this.Button_SaveProject.UseVisualStyleBackColor = true;
            this.Button_SaveProject.Click += new System.EventHandler(this.Button_SaveProject_Click);
            // 
            // Button_DeleteProject
            // 
            this.Button_DeleteProject.Location = new System.Drawing.Point(172, 229);
            this.Button_DeleteProject.Margin = new System.Windows.Forms.Padding(4);
            this.Button_DeleteProject.Name = "Button_DeleteProject";
            this.Button_DeleteProject.Size = new System.Drawing.Size(143, 40);
            this.Button_DeleteProject.TabIndex = 16;
            this.Button_DeleteProject.Text = "選択プロジェクト削除";
            this.Button_DeleteProject.UseVisualStyleBackColor = true;
            this.Button_DeleteProject.Click += new System.EventHandler(this.Button_DeleteProject_Click);
            // 
            // ListView_EventData
            // 
            this.ListView_EventData.FullRowSelect = true;
            this.ListView_EventData.Location = new System.Drawing.Point(19, 400);
            this.ListView_EventData.Name = "ListView_EventData";
            this.ListView_EventData.Size = new System.Drawing.Size(640, 263);
            this.ListView_EventData.TabIndex = 17;
            this.ListView_EventData.UseCompatibleStateImageBehavior = false;
            this.ListView_EventData.View = System.Windows.Forms.View.Details;
            this.ListView_EventData.SelectedIndexChanged += new System.EventHandler(this.ListView_EventData_SelectedIndexChanged);
            this.ListView_EventData.Click += new System.EventHandler(this.ListView_EventData_Click);
            this.ListView_EventData.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ListView_EventData_KeyDown);
            // 
            // Button_StartEdit
            // 
            this.Button_StartEdit.Location = new System.Drawing.Point(39, 22);
            this.Button_StartEdit.Margin = new System.Windows.Forms.Padding(4);
            this.Button_StartEdit.Name = "Button_StartEdit";
            this.Button_StartEdit.Size = new System.Drawing.Size(128, 32);
            this.Button_StartEdit.TabIndex = 18;
            this.Button_StartEdit.Text = "譜面エディット開始";
            this.Button_StartEdit.UseVisualStyleBackColor = true;
            this.Button_StartEdit.Click += new System.EventHandler(this.Button_StartEdit_Click);
            // 
            // Button_EndEdit
            // 
            this.Button_EndEdit.Location = new System.Drawing.Point(39, 61);
            this.Button_EndEdit.Margin = new System.Windows.Forms.Padding(4);
            this.Button_EndEdit.Name = "Button_EndEdit";
            this.Button_EndEdit.Size = new System.Drawing.Size(128, 32);
            this.Button_EndEdit.TabIndex = 19;
            this.Button_EndEdit.Text = "譜面エディット終了";
            this.Button_EndEdit.UseVisualStyleBackColor = true;
            this.Button_EndEdit.Click += new System.EventHandler(this.Button_EndEdit_Click);
            // 
            // RadioButton0_BaseEP
            // 
            this.RadioButton0_BaseEP.AutoSize = true;
            this.RadioButton0_BaseEP.Location = new System.Drawing.Point(25, 33);
            this.RadioButton0_BaseEP.Name = "RadioButton0_BaseEP";
            this.RadioButton0_BaseEP.Size = new System.Drawing.Size(105, 19);
            this.RadioButton0_BaseEP.TabIndex = 20;
            this.RadioButton0_BaseEP.TabStop = true;
            this.RadioButton0_BaseEP.Text = "ベース用(EP)";
            this.RadioButton0_BaseEP.UseVisualStyleBackColor = true;
            this.RadioButton0_BaseEP.Click += new System.EventHandler(this.RadioButton_BaseEP_Click);
            // 
            // RadioButton0_VRShoot
            // 
            this.RadioButton0_VRShoot.AutoSize = true;
            this.RadioButton0_VRShoot.Location = new System.Drawing.Point(25, 56);
            this.RadioButton0_VRShoot.Name = "RadioButton0_VRShoot";
            this.RadioButton0_VRShoot.Size = new System.Drawing.Size(122, 19);
            this.RadioButton0_VRShoot.TabIndex = 21;
            this.RadioButton0_VRShoot.TabStop = true;
            this.RadioButton0_VRShoot.Text = "VRシューティング";
            this.RadioButton0_VRShoot.UseVisualStyleBackColor = true;
            this.RadioButton0_VRShoot.Click += new System.EventHandler(this.RadioButton_VRShoot_Click);
            // 
            // GroupBox_Humen
            // 
            this.GroupBox_Humen.Controls.Add(this.RadioButton0_BaseEP);
            this.GroupBox_Humen.Controls.Add(this.RadioButton0_VRShoot);
            this.GroupBox_Humen.Location = new System.Drawing.Point(1050, 442);
            this.GroupBox_Humen.Name = "GroupBox_Humen";
            this.GroupBox_Humen.Size = new System.Drawing.Size(200, 100);
            this.GroupBox_Humen.TabIndex = 22;
            this.GroupBox_Humen.TabStop = false;
            this.GroupBox_Humen.Text = "譜面";
            // 
            // GroupBox_Project
            // 
            this.GroupBox_Project.Controls.Add(this.ListView_Project);
            this.GroupBox_Project.Controls.Add(this.Button_NewProject);
            this.GroupBox_Project.Controls.Add(this.Button_LoadProject);
            this.GroupBox_Project.Controls.Add(this.TextBox_NewProject);
            this.GroupBox_Project.Controls.Add(this.label1);
            this.GroupBox_Project.Controls.Add(this.Button_SaveProject);
            this.GroupBox_Project.Controls.Add(this.Button_DeleteProject);
            this.GroupBox_Project.Location = new System.Drawing.Point(917, 12);
            this.GroupBox_Project.Name = "GroupBox_Project";
            this.GroupBox_Project.Size = new System.Drawing.Size(333, 412);
            this.GroupBox_Project.TabIndex = 29;
            this.GroupBox_Project.TabStop = false;
            this.GroupBox_Project.Text = "プロジェクト";
            // 
            // Label_MaxDegree
            // 
            this.Label_MaxDegree.AutoSize = true;
            this.Label_MaxDegree.Font = new System.Drawing.Font("MS UI Gothic", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.Label_MaxDegree.Location = new System.Drawing.Point(689, 400);
            this.Label_MaxDegree.Name = "Label_MaxDegree";
            this.Label_MaxDegree.Size = new System.Drawing.Size(177, 17);
            this.Label_MaxDegree.TabIndex = 27;
            this.Label_MaxDegree.Text = "MaxDegree(90,180,360)";
            // 
            // Label_MaxDegreeVal
            // 
            this.Label_MaxDegreeVal.AutoSize = true;
            this.Label_MaxDegreeVal.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.Label_MaxDegreeVal.Location = new System.Drawing.Point(748, 442);
            this.Label_MaxDegreeVal.Name = "Label_MaxDegreeVal";
            this.Label_MaxDegreeVal.Size = new System.Drawing.Size(20, 20);
            this.Label_MaxDegreeVal.TabIndex = 28;
            this.Label_MaxDegreeVal.Text = "0";
            // 
            // TrackBar_MaxDegree
            // 
            this.TrackBar_MaxDegree.LargeChange = 1;
            this.TrackBar_MaxDegree.Location = new System.Drawing.Point(665, 416);
            this.TrackBar_MaxDegree.Maximum = 2;
            this.TrackBar_MaxDegree.Name = "TrackBar_MaxDegree";
            this.TrackBar_MaxDegree.Size = new System.Drawing.Size(174, 56);
            this.TrackBar_MaxDegree.TabIndex = 23;
            this.TrackBar_MaxDegree.TickFrequency = 3;
            this.TrackBar_MaxDegree.Scroll += new System.EventHandler(this.TrackBar_MaxDegree_Scroll);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.Button_StartEdit);
            this.groupBox1.Controls.Add(this.Button_EndEdit);
            this.groupBox1.Location = new System.Drawing.Point(1050, 563);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(200, 100);
            this.groupBox1.TabIndex = 30;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "エディット";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.Button_StopMusic);
            this.groupBox2.Controls.Add(this.Button_PlayMusic);
            this.groupBox2.Location = new System.Drawing.Point(844, 563);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(200, 100);
            this.groupBox2.TabIndex = 31;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "テスト再生";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.Button_MIDIImport);
            this.groupBox3.Controls.Add(this.Button_OGGLoad);
            this.groupBox3.Location = new System.Drawing.Point(844, 442);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(200, 100);
            this.groupBox3.TabIndex = 32;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "譜面元と音源";
            // 
            // RadioButton1_LinePlot
            // 
            this.RadioButton1_LinePlot.AutoSize = true;
            this.RadioButton1_LinePlot.Checked = true;
            this.RadioButton1_LinePlot.Location = new System.Drawing.Point(684, 479);
            this.RadioButton1_LinePlot.Name = "RadioButton1_LinePlot";
            this.RadioButton1_LinePlot.Size = new System.Drawing.Size(102, 19);
            this.RadioButton1_LinePlot.TabIndex = 33;
            this.RadioButton1_LinePlot.TabStop = true;
            this.RadioButton1_LinePlot.Text = "ラインプロット";
            this.RadioButton1_LinePlot.UseVisualStyleBackColor = true;
            this.RadioButton1_LinePlot.CheckedChanged += new System.EventHandler(this.RadioButton1_LinePlot_CheckedChanged);
            // 
            // RadioButton1_XScale
            // 
            this.RadioButton1_XScale.AutoSize = true;
            this.RadioButton1_XScale.Location = new System.Drawing.Point(684, 504);
            this.RadioButton1_XScale.Name = "RadioButton1_XScale";
            this.RadioButton1_XScale.Size = new System.Drawing.Size(84, 19);
            this.RadioButton1_XScale.TabIndex = 34;
            this.RadioButton1_XScale.Text = "Xスケール";
            this.RadioButton1_XScale.UseVisualStyleBackColor = true;
            this.RadioButton1_XScale.CheckedChanged += new System.EventHandler(this.RadioButton1_XScale_CheckedChanged);
            // 
            // RadioButton1_Translate
            // 
            this.RadioButton1_Translate.AutoSize = true;
            this.RadioButton1_Translate.Location = new System.Drawing.Point(684, 529);
            this.RadioButton1_Translate.Name = "RadioButton1_Translate";
            this.RadioButton1_Translate.Size = new System.Drawing.Size(88, 19);
            this.RadioButton1_Translate.TabIndex = 35;
            this.RadioButton1_Translate.Text = "平行移動";
            this.RadioButton1_Translate.UseVisualStyleBackColor = true;
            this.RadioButton1_Translate.CheckedChanged += new System.EventHandler(this.RadioButton1_Translate_CheckedChanged);
            // 
            // Button_ReverseY
            // 
            this.Button_ReverseY.Location = new System.Drawing.Point(752, 563);
            this.Button_ReverseY.Margin = new System.Windows.Forms.Padding(4);
            this.Button_ReverseY.Name = "Button_ReverseY";
            this.Button_ReverseY.Size = new System.Drawing.Size(77, 36);
            this.Button_ReverseY.TabIndex = 36;
            this.Button_ReverseY.Text = "Y反転";
            this.Button_ReverseY.UseVisualStyleBackColor = true;
            this.Button_ReverseY.Click += new System.EventHandler(this.Button_ReverseY_Click);
            // 
            // Button_ReverseX
            // 
            this.Button_ReverseX.Location = new System.Drawing.Point(670, 563);
            this.Button_ReverseX.Margin = new System.Windows.Forms.Padding(4);
            this.Button_ReverseX.Name = "Button_ReverseX";
            this.Button_ReverseX.Size = new System.Drawing.Size(77, 36);
            this.Button_ReverseX.TabIndex = 37;
            this.Button_ReverseX.Text = "X反転";
            this.Button_ReverseX.UseVisualStyleBackColor = true;
            this.Button_ReverseX.Click += new System.EventHandler(this.Button_ReverseX_Click);
            // 
            // Form1
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(200)))), ((int)(((byte)(170)))));
            this.ClientSize = new System.Drawing.Size(1262, 675);
            this.Controls.Add(this.Button_ReverseX);
            this.Controls.Add(this.Button_ReverseY);
            this.Controls.Add(this.RadioButton1_Translate);
            this.Controls.Add(this.RadioButton1_XScale);
            this.Controls.Add(this.RadioButton1_LinePlot);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.GroupBox_Project);
            this.Controls.Add(this.Label_MaxDegreeVal);
            this.Controls.Add(this.Label_MaxDegree);
            this.Controls.Add(this.TrackBar_MaxDegree);
            this.Controls.Add(this.GroupBox_Humen);
            this.Controls.Add(this.ListView_EventData);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.Numeric_Shift);
            this.Controls.Add(this.Label_ProjectName);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.Window_Cpp);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form1";
            this.Text = "OMSEditor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.Form1_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.Form1_DragEnter);
            ((System.ComponentModel.ISupportInitialize)(this.Numeric_Shift)).EndInit();
            this.GroupBox_Humen.ResumeLayout(false);
            this.GroupBox_Humen.PerformLayout();
            this.GroupBox_Project.ResumeLayout(false);
            this.GroupBox_Project.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TrackBar_MaxDegree)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel Window_Cpp;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Button Button_MIDIImport;
        private System.Windows.Forms.Button Button_OGGLoad;
        private System.Windows.Forms.ListView ListView_Project;
        private System.Windows.Forms.Button Button_LoadProject;
        private System.Windows.Forms.Button Button_NewProject;
        private System.Windows.Forms.TextBox TextBox_NewProject;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label Label_ProjectName;
        private System.Windows.Forms.NumericUpDown Numeric_Shift;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button Button_PlayMusic;
        private System.Windows.Forms.Button Button_StopMusic;
        private System.Windows.Forms.Button Button_SaveProject;
        private System.Windows.Forms.Button Button_DeleteProject;
        private System.Windows.Forms.ListView ListView_EventData;
        private System.Windows.Forms.Button Button_StartEdit;
        private System.Windows.Forms.Button Button_EndEdit;
        private System.Windows.Forms.RadioButton RadioButton0_BaseEP;
        private System.Windows.Forms.RadioButton RadioButton0_VRShoot;
        private System.Windows.Forms.GroupBox GroupBox_Humen;
        private System.Windows.Forms.GroupBox GroupBox_Project;
        private System.Windows.Forms.Label Label_MaxDegree;
        private System.Windows.Forms.Label Label_MaxDegreeVal;
        private System.Windows.Forms.TrackBar TrackBar_MaxDegree;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.RadioButton RadioButton1_LinePlot;
        private System.Windows.Forms.RadioButton RadioButton1_XScale;
        private System.Windows.Forms.RadioButton RadioButton1_Translate;
        private System.Windows.Forms.Button Button_ReverseY;
        private System.Windows.Forms.Button Button_ReverseX;
    }
}

