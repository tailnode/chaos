using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace TopClock
{
    public partial class ChildForm : Form
    {
        private DateTime time;

        public ChildForm()
        {
            InitializeComponent();
            updateTime();
        }

        private void TopClockForm_Load(object sender, EventArgs e)
        {

        }

        private void oneMinTimer_Tick(object sender, EventArgs e)
        {
            updateTime();
        }

        // 更新显示的时间
        private void updateTime()
        {
            time = DateTime.Now;
            TimeLable.Text = time.Hour.ToString() + ":" + time.Minute.ToString();
        }

        private void closeBtn_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
