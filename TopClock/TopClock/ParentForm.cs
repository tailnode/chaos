using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace TopClock
{
    public partial class ParentForm : Form
    {
        [DllImport("user32.dll")]
        public static extern bool ReleaseCapture();
        [DllImport("user32.dll")]
        public static extern bool SendMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);
        public const int WM_SYSCOMMAND = 0x0112;
        public const int SC_MOVE = 0xF010;
        public const int HTCAPTION = 0x0002;

        ChildForm childForm;
        public ParentForm()
        {
            InitializeComponent();
            this.childForm = new ChildForm(this);
            this.childForm.Owner = this;
            this.childForm.Dock = DockStyle.Fill;
            this.childForm.Show();
            this.childForm.BringToFront();
        }

        private void ParentForm_Load(object sender, EventArgs e)
        {

        }

        private void moveForm()
        {
            ReleaseCapture();
            SendMessage(this.Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }

        private void ParentForm_MouseDown(object sender, MouseEventArgs e)
        {
            moveForm();
        }

        private void ParentForm_LocationChanged(object sender, EventArgs e)
        {
            childForm.Location = this.Location;
        }

        private void topmost_Tick(object sender, EventArgs e)
        {
            this.TopMost = true;
        }

        private void notifyIcon_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                childForm.getTimer().Enabled = true;
                topmost.Enabled = true;
                childForm.updateTime();
                this.WindowState = FormWindowState.Normal;
            }
        }

        public Timer getTimer()
        {
            return topmost;
        }

        private void menu_exit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void menu_Opened(object sender, EventArgs e)
        {
            topmost.Enabled = false;
        }

        private void menu_Closed(object sender, ToolStripDropDownClosedEventArgs e)
        {
            if (this.WindowState != FormWindowState.Minimized)
            {
                topmost.Enabled = true;
            }
        }
    }
}
