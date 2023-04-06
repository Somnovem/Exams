using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ActivityMonitor
{
    internal class EventLogger : IDisposable
    {
        private StreamWriter writer;
        private string bannedWordsInfoPath;
        private string bannedProcessesInfoPath;
        private string newProcessesInfoPath;
        public EventLogger(string destinationPath)
        {
            bannedWordsInfoPath = Path.Combine(destinationPath, "BannedWords.txt");
            bannedProcessesInfoPath = Path.Combine(destinationPath, "BannedProcesses.txt");
            newProcessesInfoPath = Path.Combine(destinationPath, "ProcessMonitoringResult.txt");
            writer = new StreamWriter(Path.Combine(destinationPath, "MonitoringKeysResult.txt"));
            File.Create(bannedWordsInfoPath).Dispose();
            File.Create(bannedProcessesInfoPath).Dispose();
            File.Create(newProcessesInfoPath).Dispose();
        }
        public void LogKey(string key)
        {
            writer.WriteLine($"Pressed key: {key}");
        }
        public void LogBannedProcess(string[] processes)
        {
            using (StreamWriter writerBanned = new StreamWriter(bannedProcessesInfoPath, true))
            {
                for (int i = 0; i < processes.Length; i++)
                {
                    writerBanned.WriteLine($"Banned process was started: {processes[i]}");
                }
            }
        }
        public void LogBannedWord(string word)
        {
            using (StreamWriter writerBanned = new StreamWriter(bannedWordsInfoPath, true))
            {
                writerBanned.WriteLine($"Banned word was entered: {word}");
            }
        }
        public void LogNewProcesses(List<string> processInfo) 
        {
            using (StreamWriter writerProcesses = new StreamWriter(newProcessesInfoPath, true))
            {
                for (int i = 0; i < processInfo.Count; i++)
                {
                    writerProcesses.WriteLine(processInfo[i]);
                }
            }
        }

        public void Dispose()
        {
            writer.Dispose();
        }
    }
}
