using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace MuteSwitch
{
    public partial class Form1 : Form
    {
        enum MuteState { MUTE_UNKNOW, MUTE_ON, MUTE_OFF};

        MuteState state;
        [DllImport("MuteDLL.dll")]
        static extern void muteOn();

        [DllImport("MuteDLL.dll")]
        static extern void muteOff();

        public Form1()
        {
            InitializeComponent();
            state = MuteState.MUTE_UNKNOW;
        }

        private void muteSwicth_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (state != MuteState.MUTE_ON)
                {
                    muteOn();
                    state = MuteState.MUTE_ON;
                    updateNotifyIcon();
                }
            }
            else if (e.Button == MouseButtons.Right)
            {
                if (state != MuteState.MUTE_OFF)
                {
                    muteOff();
                    state = MuteState.MUTE_OFF;
                    updateNotifyIcon();
                }
            }
            else if (e.Button == MouseButtons.Middle)
            {
                Application.Exit();
            }


        }

        private void updateNotifyIcon()
        {
            if (MuteState.MUTE_OFF == state)
            {
                muteSwicth.Icon = Properties.Resources.muteOff;
                muteSwicth.Text = "耳机声音：开";
            }
            else if (MuteState.MUTE_ON == state)
            {
                muteSwicth.Icon = Properties.Resources.muteOn;
                muteSwicth.Text = "耳机声音：关";
            }
            else
            {
                muteSwicth.Icon = Properties.Resources.muteUnknow;
                muteSwicth.Text = "耳机声音：未知";
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        protected override void SetVisibleCore(bool value)
        {
            base.SetVisibleCore(false);
        }
    }
}
