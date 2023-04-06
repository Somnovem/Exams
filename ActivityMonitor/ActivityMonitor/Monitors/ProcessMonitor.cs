using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows;

namespace ActivityMonitor
{
    internal class ProcessMonitor
    {
        private List<string> existingProcesses;
        private string[] bannedProcessNames;
        private bool closeRequested;
        public delegate void ProcessStartedDelegate(Process[] processes);
        public event ProcessStartedDelegate BannedProcessStarted;
        public event ProcessStartedDelegate NewProcessesStarted;
        public ProcessMonitor(string[] bannedProcessNames)
        {
            this.bannedProcessNames = bannedProcessNames;
            existingProcesses = new List<string>();
            closeRequested = false;
            Process[] startingProcesses = Process.GetProcesses();
            for (int i = 0; i < startingProcesses.Length; i++)
            {
                existingProcesses.Add(startingProcesses[0].ProcessName);
            }
        }
        public void StartMonitoring()
        {
            bool checkBanned = bannedProcessNames.Length > 0;
            while (!closeRequested)
            {
                Process[] newProcesses = Process.GetProcesses().Where(x => existingProcesses.Contains(x.ProcessName) == false).ToArray();
                if (newProcesses.Length != 0) NewProcessesStarted?.Invoke(newProcesses);
                if (checkBanned)
                {
                    Process[] banning = newProcesses.Where(x => bannedProcessNames.Contains(x.ProcessName)).ToArray();
                    if (banning.Length != 0)
                    {
                        BannedProcessStarted?.Invoke(banning);
                        for (int i = 0; i < banning.Length; i++)
                        {
                            banning[i].CloseMainWindow();
                            banning[i].Close();
                        }
                    }
                }
                Thread.Sleep(3000);
            }
        }
        public void StartMonitoringAsync() => Task.Run(StartMonitoring);

        public void Close() { closeRequested = true; }
    }
}
