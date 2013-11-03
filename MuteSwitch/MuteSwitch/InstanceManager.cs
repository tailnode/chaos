using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace MuteSwitch
{
    class InstanceManager
    {
        public static bool started()
        {
            Process current = Process.GetCurrentProcess();
            Process[] processes = Process.GetProcessesByName(current.ProcessName);

            foreach (Process process in processes)
            {
                if (current.Id != process.Id &&
                    current.MainModule.FileName == process.MainModule.FileName)
                {
                    return true;
                }
            }

            return false;
        }
    }
}
