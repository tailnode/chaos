namespace MouseRecorder
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.btn_start_stop = new System.Windows.Forms.Button();
            this.btn_gen = new System.Windows.Forms.Button();
            this.btn_setup = new System.Windows.Forms.Button();
            this.btn_save = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btn_start_stop
            // 
            this.btn_start_stop.Location = new System.Drawing.Point(12, 12);
            this.btn_start_stop.Name = "btn_start_stop";
            this.btn_start_stop.Size = new System.Drawing.Size(75, 23);
            this.btn_start_stop.TabIndex = 0;
            this.btn_start_stop.Text = "开始";
            this.btn_start_stop.UseVisualStyleBackColor = true;
            this.btn_start_stop.Click += new System.EventHandler(this.btnStartStop_Click);
            // 
            // btn_gen
            // 
            this.btn_gen.Location = new System.Drawing.Point(110, 12);
            this.btn_gen.Name = "btn_gen";
            this.btn_gen.Size = new System.Drawing.Size(75, 23);
            this.btn_gen.TabIndex = 1;
            this.btn_gen.Text = "生成";
            this.btn_gen.UseVisualStyleBackColor = true;
            this.btn_gen.Click += new System.EventHandler(this.btnGen_Click);
            // 
            // btn_setup
            // 
            this.btn_setup.Location = new System.Drawing.Point(12, 41);
            this.btn_setup.Name = "btn_setup";
            this.btn_setup.Size = new System.Drawing.Size(75, 23);
            this.btn_setup.TabIndex = 2;
            this.btn_setup.Text = "设置";
            this.btn_setup.UseVisualStyleBackColor = true;
            this.btn_setup.Click += new System.EventHandler(this.btn_setup_Click);
            // 
            // btn_save
            // 
            this.btn_save.Enabled = false;
            this.btn_save.Location = new System.Drawing.Point(110, 41);
            this.btn_save.Name = "btn_save";
            this.btn_save.Size = new System.Drawing.Size(75, 23);
            this.btn_save.TabIndex = 3;
            this.btn_save.Text = "保存";
            this.btn_save.UseVisualStyleBackColor = true;
            this.btn_save.Click += new System.EventHandler(this.btn_save_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(196, 75);
            this.Controls.Add(this.btn_save);
            this.Controls.Add(this.btn_setup);
            this.Controls.Add(this.btn_gen);
            this.Controls.Add(this.btn_start_stop);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "MouseRecorder";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btn_start_stop;
        private System.Windows.Forms.Button btn_gen;
        private System.Windows.Forms.Button btn_setup;
        private System.Windows.Forms.Button btn_save;
    }
}

