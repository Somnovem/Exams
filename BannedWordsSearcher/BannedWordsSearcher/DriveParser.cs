using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Timers;
using System.Threading;
using Timer = System.Timers.Timer;

namespace BannedWordsSearcher
{

    public enum ParserState
    {
        RUNNING,
        SUSPENDED,
        ABORTED
    }

    internal class DriveParser
    {
        public string name;
        private string drivePath;
        private string destinationPath;
        private Dictionary<string, int> bannedWordsCounter;
        private List<string> bannedWords;
        private ManualResetEvent threadStabilizer;
        public ManualResetEvent startAwaiter;
        private Timer timer;
        public ParserState State;
        private int counter;
        public string Report { get; private set; }

        public delegate void SignalFilesToCheckDelegate(string name,int num);
        public event SignalFilesToCheckDelegate SignalFilesToCheck;
        public delegate void GetLastTransferredDelegate(Dictionary<string, int> bannedWordsCounter, int fileCount, bool parserFinished = false,DriveParser parser = null);
        public event GetLastTransferredDelegate GetLastTransferred;
        public DriveParser(string name,string drivePath,List<string> bannedWords, string destinationPath)
        {
            this.name = name;
            this.drivePath = drivePath;
            this.bannedWords = bannedWords;
            this.destinationPath = destinationPath;
            threadStabilizer = new ManualResetEvent(false);
            startAwaiter = new ManualResetEvent(false);
            Report = "";
            timer = new Timer(100);
            timer.AutoReset = true;
            timer.Elapsed += Timer_Elapsed;
            State = ParserState.RUNNING;
        }

        public static IEnumerable<string> GetFiles(string path, string searchPattern, SearchOption searchOption)
        {
            List<string> foldersToProcess = new List<string>(){path};

            while (foldersToProcess.Count > 0)
            {
                string folder = foldersToProcess[0];
                foldersToProcess.RemoveAt(0);

                if (searchOption.HasFlag(SearchOption.AllDirectories))
                {
                    
                    try
                    {
                        var subfolders = Directory.GetDirectories(folder);
                        foldersToProcess.AddRange(subfolders);
                    }
                    catch 
                    {
                        
                    }
                }

                var files = new List<string>();
                try
                {
                    files = Directory.GetFiles(folder, searchPattern, SearchOption.TopDirectoryOnly).ToList();
                }
                catch
                {
                   
                }
                foreach (var file in files)
                {
                    yield return file;
                }
            }
        }
        private bool bannedWordEncountered;

        public void StartSearching()
        {
                string[] filesToCheck = GetFiles(drivePath, "*.txt", SearchOption.AllDirectories).ToArray();
                SignalFilesToCheck?.Invoke(this.name,filesToCheck.Length);
                startAwaiter.WaitOne();
                bannedWordsCounter = new Dictionary<string, int>();
                timer.Start();
                Dictionary<string, int> localBannedWordsEncountered = new Dictionary<string, int>();
                counter = 0;
                foreach (var file in filesToCheck)
                {
                    if (State == ParserState.RUNNING)
                    {
                        string[] lines = null;
                        try
                        {
                            lines = File.ReadAllLines(file);
                        }
                        catch
                        {
                            //Access was denied, or file is being used by another process
                            counter++;
                            continue;
                        }
                        string redactedText = "";
                        bannedWordEncountered = false;
                        string tempLine;
                        localBannedWordsEncountered.Clear();
                        foreach (string line in lines)
                        {
                            tempLine = line;
                            string[] words = line.Split(" .,!?;".ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                            if (words.Length > 0)
                            {
                                foreach (var word in bannedWords)
                                {
                                    if (words.Contains(word))
                                    {
                                        if (bannedWordsCounter.ContainsKey(word))
                                            bannedWordsCounter[word]++;
                                        else
                                            bannedWordsCounter.Add(word, 1);

                                        if (localBannedWordsEncountered.ContainsKey(word))
                                            localBannedWordsEncountered[word]++;
                                        else
                                            localBannedWordsEncountered.Add(word, 1);

                                        if (!bannedWordEncountered) bannedWordEncountered = true;
                                        tempLine = tempLine.Replace(word, "*******");
                                    }
                                }
                                if (bannedWordEncountered) redactedText += tempLine + '\n';
                            }
                        }
                        string destinationFilePath = "";
                        if (bannedWordEncountered)
                        {
                            FileInfo info = new FileInfo(file);
                            string fileNameRedacted = info.Name.Split('.')[0] + "Redacted" + info.Extension;
                            destinationFilePath = Path.Combine(destinationPath, fileNameRedacted);
                            try
                            {
                                File.Copy(file, Path.Combine(destinationPath, info.Name), true);
                                File.WriteAllText(destinationFilePath, redactedText);
                            }
                            catch
                            {
                                //Access was denied, or file is being used by another process
                            }
                        }
                        Report += $"Read file: {file}\n";
                        Report += $"Original file size: {new FileInfo(file).Length} bytes\n";
                        foreach (var item in localBannedWordsEncountered.Keys)
                        {
                            Report += $"Encountered {item} -> {localBannedWordsEncountered[item]} times\n";
                        }
                        if (!string.IsNullOrEmpty(destinationFilePath)) Report += $"File size after changes: {new FileInfo(destinationFilePath).Length} bytes\n";
                        Report += '\n';
                        counter++;
                        threadStabilizer.WaitOne();
                        threadStabilizer.Set();
                    }
                    else if (State == ParserState.SUSPENDED) Thread.Sleep(200);
                    else break;
                }
                timer.Stop();
                GetLastTransferred?.Invoke(bannedWordsCounter,counter,true,this);
        }

        private void Timer_Elapsed(object? sender, ElapsedEventArgs e)
        {
                timer.Stop();
                threadStabilizer.Reset();
                GetLastTransferred?.Invoke(bannedWordsCounter,counter);
                foreach (var key in bannedWordsCounter.Keys)
                {
                    bannedWordsCounter[key] = 0;
                }
                counter = 0;
                threadStabilizer.Set();
                timer.Start();
        }
    }
}
