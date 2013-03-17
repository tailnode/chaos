namespace TopClock
{
    partial class ParentForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.topmost = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // topmost
            // 
            this.topmost.Enabled = true;
            this.topmost.Interval = 1000;
            this.topmost.Tick += new System.EventHandler(this.topmost_Tick);
            // 
            // ParentForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(75, 28);
            this.ControlBox = false;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "ParentForm";
            this.Opacity = 0.01;
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "ChildForm";
            this.TopMost = true;
            this.Load += new System.EventHandler(this.ParentForm_Load);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ParentForm_MouseDown);
            this.LocationChanged += new System.EventHandler(this.ParentForm_LocationChanged);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Timer topmost;
    }
}