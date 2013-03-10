using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Reflection;
using System.IO;

namespace MouseRecorder
{
    public partial class MainForm : Form
    {
        private delegate int HookProc(int nCode, int wParam, IntPtr lParam);

        #region 常量、枚举、结构体定义

        private const int WH_MOUSE_LL       = 14;
        private const int WH_KEYBOARD_LL    = 13;
        private const int WM_LBUTTONDOWN    = 0x201;
        private const int WM_RBUTTONDOWN    = 0x204;
        private const string FILE_PATH =    @"./mousePostion.txt";

        [StructLayout(LayoutKind.Sequential)]
        private class POINT
        {
            public int x;
            public int y;
        }

        [StructLayout(LayoutKind.Sequential)]
        private class MouseLLHookStruct
        {
            public POINT pt;
            public int mouseData;
            public int flags;
            public int time;
            public int dwExtraInfo;
        }

        #endregion

        #region 变量定义

        static bool isHookSet;
        static int hHook;
        static FileStream sFile;
        static StreamReader sReader;
        static StreamWriter sWriter;
        static Bitmap bitmap;
        static ImageForm imageForm;

        #endregion

        #region 引入DLL

        // 引入DLL
        [DllImport("user32.dll", CharSet = CharSet.Auto,
           CallingConvention = CallingConvention.StdCall, SetLastError = true)]
        private static extern int SetWindowsHookEx(
            int idHook,
            HookProc lpfn,
            IntPtr hMod,
            int dwThreadId);

        [DllImport("user32.dll", CharSet = CharSet.Auto,
                    CallingConvention = CallingConvention.StdCall, SetLastError = true)]
        private static extern int UnhookWindowsHookEx(int idHook);

        [DllImport("user32.dll", CharSet = CharSet.Auto,
                     CallingConvention = CallingConvention.StdCall)]
        private static extern int CallNextHookEx(
            int idHook,
            int nCode,
            int wParam,
            IntPtr lParam);

        #endregion


        public MainForm()
        {
            InitializeComponent();
            init();
        }

        private void btnStartStop_Click(object sender, EventArgs e)
        {
            if (false == isHookSet)
            {
                hHook = SetWindowsHookEx(
                    WH_MOUSE_LL,
                    MouseHookProc,
                    Marshal.GetHINSTANCE(
                        Assembly.GetExecutingAssembly().GetModules()[0]),
                    0);
                if (0 == hHook)
                {
                    int reCode = Marshal.GetLastWin32Error();
                    Console.WriteLine("error code = {0}", reCode);
                    return;
                }
                sFile = new FileStream(FILE_PATH, FileMode.Append);
                sWriter= new StreamWriter(sFile);

                isHookSet = true;
                btn_start_stop.Text = "停止";
            }
            else
            {
                UnhookWindowsHookEx(hHook);
                sWriter.Close();
                sFile.Close();
                isHookSet = false;
                btn_start_stop.Text = "开始";
            }
        }

        private void btnGen_Click(object sender, EventArgs e)
        {
            if (true == showImage())
            {
                this.btn_save.Enabled = true;
            }
        }

        private int MouseHookProc(int nCode, int wParam, IntPtr lParam)
        {
            MouseLLHookStruct mouseHookStruct = (MouseLLHookStruct)Marshal.PtrToStructure(lParam, typeof(MouseLLHookStruct));
            if (WM_LBUTTONDOWN == wParam || WM_RBUTTONDOWN == wParam)
            {
                sWriter.WriteLine("{0} {1}", mouseHookStruct.pt.x, mouseHookStruct.pt.y);
                sWriter.Flush();
            }
            
            return CallNextHookEx(hHook, nCode, wParam, lParam);
        }

        // 打开新窗口显示图片
        private bool showImage()
        {
            try
            {
                sFile = new FileStream(FILE_PATH, FileMode.Open);
                sReader = new StreamReader(sFile);
            }
            catch (System.IO.FileNotFoundException e)
            {
                MessageBox.Show("请先点击开始按钮进行数据记录", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            catch
            {
                MessageBox.Show("数据打开错误,如果正在进行记录请先点击停止按钮", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            // ImageForm窗口被释放后重新new
            if (imageForm.IsDisposed)
            {
                imageForm = new ImageForm();
            }

            // 使生成的窗口在主窗口右侧
            imageForm.Show();
            imageForm.Location = new Point(this.Location.X + this.Size.Width, this.Location.Y);
            
            string line;
            Point point = new Point();

            bitmap = new Bitmap(imageForm.getPanel().Width, imageForm.getPanel().Height);
            Graphics g = Graphics.FromImage(bitmap);
            g.FillRectangle(new SolidBrush(Color.White), 0, 0, imageForm.getPanel().Width, imageForm.getPanel().Height);
            while ((line = sReader.ReadLine()) != null)
            {
                string[] sArray=line.Split(' ');
                point.X = int.Parse(sArray[0]);
                point.Y = int.Parse(sArray[1]);
                bitmap.SetPixel(point.X >> 1, point.Y >> 1, Color.Red);
            }
            g = imageForm.getPanel().CreateGraphics();
            g.DrawImage(bitmap, 0, 0);
            sReader.Close();
            sFile.Close();
            return true;
        }

        // 保存为图片
        private void saveImage()
        {
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.ShowDialog();
            if (dialog.FileName.Length != 0)
            {
                bitmap.Save(dialog.FileName);
            }
        }

        private void init()
        {
            imageForm = new ImageForm();
        }

        private void btn_setup_Click(object sender, EventArgs e)
        {

        }

        private void btn_save_Click(object sender, EventArgs e)
        {
            saveImage();
        }
    }
}
