namespace TopClock
{
    partial class ChildForm
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
            this.components = new System.ComponentModel.Container();
            this.TimeLable = new System.Windows.Forms.Label();
            this.oneMinTimer = new System.Windows.Forms.Timer(this.components);
            this.closeBtn = new System.Windows.Forms.PictureBox();
            this.minBtn = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.closeBtn)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.minBtn)).BeginInit();
            this.SuspendLayout();
            // 
            // TimeLable
            // 
            this.TimeLable.AutoSize = true;
            this.TimeLable.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.TimeLable.Dock = System.Windows.Forms.DockStyle.Left;
            this.TimeLable.ForeColor = System.Drawing.Color.White;
            this.TimeLable.Location = new System.Drawing.Point(0, 0);
            this.TimeLable.Margin = new System.Windows.Forms.Padding(0);
            this.TimeLable.Name = "TimeLable";
            this.TimeLable.Size = new System.Drawing.Size(64, 28);
            this.TimeLable.TabIndex = 0;
            this.TimeLable.Text = "time";
            // 
            // oneMinTimer
            // 
            this.oneMinTimer.Enabled = true;
            this.oneMinTimer.Interval = 60000;
            this.oneMinTimer.Tick += new System.EventHandler(this.oneMinTimer_Tick);
            // 
            // closeBtn
            // 
            this.closeBtn.Image = global::TopClock.Properties.Resources.close;
            this.closeBtn.Location = new System.Drawing.Point(64, 0);
            this.closeBtn.Margin = new System.Windows.Forms.Padding(0);
            this.closeBtn.Name = "closeBtn";
            this.closeBtn.Size = new System.Drawing.Size(5, 5);
            this.closeBtn.TabIndex = 1;
            this.closeBtn.TabStop = false;
            this.closeBtn.Click += new System.EventHandler(this.closeBtn_Click);
            // 
            // minBtn
            // 
            this.minBtn.Image = global::TopClock.Properties.Resources.minimized;
            this.minBtn.Location = new System.Drawing.Point(0, 0);
            this.minBtn.Margin = new System.Windows.Forms.Padding(0);
            this.minBtn.Name = "minBtn";
            this.minBtn.Size = new System.Drawing.Size(5, 5);
            this.minBtn.TabIndex = 2;
            this.minBtn.TabStop = false;
            this.minBtn.Click += new System.EventHandler(this.minBtn_Click);
            // 
            // ChildForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(13F, 28F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.ClientSize = new System.Drawing.Size(116, 28);
            this.Controls.Add(this.minBtn);
            this.Controls.Add(this.closeBtn);
            this.Controls.Add(this.TimeLable);
            this.Font = new System.Drawing.Font("Consolas", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(7);
            this.Name = "ChildForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Form";
            this.TopMost = true;
            this.TransparencyKey = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.Load += new System.EventHandler(this.TopClockForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.closeBtn)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.minBtn)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label TimeLable;
        private System.Windows.Forms.PictureBox closeBtn;
        private System.Windows.Forms.PictureBox minBtn;
        public System.Windows.Forms.Timer oneMinTimer;
    }
}

