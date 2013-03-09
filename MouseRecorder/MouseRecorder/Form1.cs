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
    public partial class Form1 : Form
    {
        private delegate int HookProc(int nCode, int wParam, IntPtr lParam); 
        private const int WH_MOUSE_LL       = 14;
        private const int WH_KEYBOARD_LL = 13;
        private const int WM_LBUTTONDOWN = 0x201;
        private const int WM_RBUTTONDOWN = 0x204;
        private const string FILE_PATH = @"E://mousePostion.txt";

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

        static bool isHookSet;
        static int hHook;
        FileStream sFile;
        StreamReader sReader;
        StreamWriter sWriter;
        Bitmap bitmap;

        public Form1()
        {
            InitializeComponent();
            isHookSet = false;
            hHook = 0;
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
            if (btn_gen.Text == "生成")
            {
                genImage();
                btn_gen.Text = "保存";
            }
            else if (btn_gen.Text == "保存")
            {
                saveIamge();
            }
            else
            {
                // do nothing
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

        private void genImage()
        {
            try
            {
                sFile = new FileStream(FILE_PATH, FileMode.Open);
                sReader = new StreamReader(sFile);
            }
            catch (Exception e)
            {

            }

            imageForm form = new imageForm();
            form.Show();
            form.getPanel().CreateGraphics();

            bitmap = new Bitmap(form.getPanel().Width, form.getPanel().Height);
            
            string line;
            Point point = new Point();
            Graphics g = Graphics.FromImage(bitmap);
            g.FillRectangle(new SolidBrush(Color.White), 0, 0, form.getPanel().Width, form.getPanel().Height);
            while ((line = sReader.ReadLine()) != null)
            {
                string[] sArray=line.Split(' ');
                point.X = int.Parse(sArray[0]);
                point.Y = int.Parse(sArray[1]);
                bitmap.SetPixel(point.X >> 1, point.Y >> 1, Color.Red);
            }
            g = form.getPanel().CreateGraphics();
            g.DrawImage(bitmap, 0, 0);
        }

        private void saveIamge()
        {
            bitmap.Save(@"E://save.bmp");
        }
    }
}
