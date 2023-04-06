using System;
using System.Collections.Generic;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.IO;
using Ookii.Dialogs.Wpf;
using Microsoft.Win32;
using System.Diagnostics;
using System.Linq;

namespace BannedWordsSearcher
{
    public partial class MainWindow : Window
    {

        private MediaPlayer musicPlayer;
        private List<DriveParser> runningParsers;


        private List<string> parsersToAwait;
        private List<string> bannedWords;
        private Dictionary<string, int> encounteredBannedWords;
        private int filesToSearch;
        private int filesSearched;


        private string parsersResult;
        private string destinationPath;
        private Thread reportingThread;
        private bool wasStartedFromConsole;


        public MainWindow(bool fromConsole = false,string destinationPath = null, string[] bannedWordsArray = null)
        {
            Process proc = Process.GetCurrentProcess();
            int count = Process.GetProcesses().Where(p =>
                p.ProcessName == proc.ProcessName).Count();
            if (count > 1)
            {
                MessageBox.Show("Already an instance is running...");
                Environment.Exit(0);
            }
            wasStartedFromConsole = fromConsole;
            if (wasStartedFromConsole) this.Visibility = Visibility.Hidden;
            InitializeComponent();
            if (wasStartedFromConsole) 
            {
                this.destinationPath = destinationPath;
                bannedWords = bannedWordsArray.ToList();
                btnStart_Click(null, null);
            }
            else
            {
                bannedWords = new List<string>();
                musicPlayer = new MediaPlayer();
                string musicPath = @"..\..\..\Music\backgroundMusic.wav";
                musicPlayer.Open(new Uri(musicPath, UriKind.RelativeOrAbsolute));
                musicPlayer.Volume = 0.05;
                musicPlayer.MediaEnded += Repeater;
                string[] drives = Directory.GetLogicalDrives();
                foreach (var item in drives)
                {
                    CheckBox cb = new CheckBox();
                    cb.Content = item;
                    lvDrives.Items.Add(cb);
                }
            }


            parsersResult = "";

        }
        private void UpdateCounter()
        {

            try
            {
                this.Dispatcher.Invoke(() =>
                {
                    string[] messages = new string[] { $"Files processed: {filesSearched}/{filesToSearch}", "All search finished!" };
                    if (wasStartedFromConsole)
                    {
                        Console.WriteLine(messages[0]);
                    }
                    else
                    {
                        lblProcess.Content = messages[0];
                        double progress = (double)filesSearched / filesToSearch;
                        mainProgressBar.Value = progress * 100;
                    }

                    if (runningParsers.Count == 0 && parsersToAwait.Count == 0)
                    {
                        if (wasStartedFromConsole)
                        {
                            Console.ForegroundColor = ConsoleColor.Red;
                            Console.WriteLine(messages[1]);
                            Console.ForegroundColor = ConsoleColor.White;
                        }
                        else MessageBox.Show(messages[1],"Result",MessageBoxButton.OK,MessageBoxImage.Information);
                        btnStop_Click(null, null);
                        if (wasStartedFromConsole)
                        {
                            Console.WriteLine("Press Enter to exit...");
                            Console.ReadLine();
                            Environment.Exit(0);
                        }
                    }
                });
            }
            catch (Exception)
            {
                // Task was canceled
            }

        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (runningParsers != null) FinishSearching();
            if (reportingThread != null) reportingThread.Join();
        }

        #region Music Control

        private bool musicMuted = false;
        private bool musicPaused = true;

        private static void ChangeButtonImage(Button btn, string path) 
        {
            var img = new BitmapImage(new Uri(path, UriKind.RelativeOrAbsolute));
            Image temp = new Image();
            temp.Source = img;
            btn.Content = temp;
        }

        private void Mute_Click(object sender, RoutedEventArgs e)
        {
            if (musicMuted)
            {
                musicPlayer.Volume = 0.1;
                ChangeButtonImage(btnMute, @"..\..\..\Images\mute.ico");
            }
            else
            {
                musicPlayer.Volume = 0;
                ChangeButtonImage(btnMute, @"..\..\..\Images\unmute.ico");
            }
            musicMuted = !musicMuted;
        }

        private void Pause_Click(object sender, RoutedEventArgs e)
        {
            if (musicPaused)
            {
                musicPlayer.Play();
                ChangeButtonImage(btnPause, @"..\..\..\Images\pause.ico");
            }
            else
            {
                musicPlayer.Pause();
                ChangeButtonImage(btnPause, @"..\..\..\Images\play.ico");
            }
            musicPaused = !musicPaused;
        }

        private void Repeater(object? sender, EventArgs e) 
        {
            musicPlayer.Position = TimeSpan.Zero;
            musicPlayer.Play();
        }
        #endregion

        #region Thread Control
        private void btnStart_Click(object sender, RoutedEventArgs e)
        {
            if (wasStartedFromConsole)
            {
                List<string> drivesToCheck = Directory.GetLogicalDrives().ToList();
                encounteredBannedWords = new Dictionary<string, int>();
                runningParsers = new List<DriveParser>();
                parsersToAwait = new List<string>();
                filesSearched = 0;
                filesToSearch = 0;
                int count = 0;
                foreach (var drive in drivesToCheck)
                {
                    string parserToAwaitName = $"parser{(string)drivesToCheck[count++]}";
                    parsersToAwait.Add(parserToAwaitName);
                    DriveParser parser = new DriveParser(parserToAwaitName, drive, bannedWords, destinationPath);
                    parser.SignalFilesToCheck += Parser_SignalFilesToCheck;
                    parser.GetLastTransferred += Parser_GetLastTransferred;
                    Thread parsingThread = new Thread(parser.StartSearching);
                    parsingThread.Start();
                    runningParsers.Add(parser);
                }
                Console.WriteLine("0/0");
            }
            else
            {
                if (string.IsNullOrEmpty(destinationPath))
                {
                    MessageBox.Show("Destination path wasn't chosen!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
                if (bannedWords.Count == 0)
                {
                    MessageBox.Show("List of banned words is empty!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
                List<string> drivesToCheck = new List<string>();
                foreach (CheckBox item in lvDrives.Items)
                {
                    if (item.IsChecked == true) drivesToCheck.Add((string)item.Content);
                }
                if (drivesToCheck.Count == 0)
                {
                    MessageBox.Show("You didn't select any drives to check", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
                encounteredBannedWords = new Dictionary<string, int>();
                runningParsers = new List<DriveParser>();
                parsersToAwait = new List<string>();


                filesSearched = 0;
                filesToSearch = 0;
                lblProcess.Content = $"Files processed: {filesSearched}/{filesToSearch}";
                mainProgressBar.Value = 0;
                edState.Text = "Counting and processing files...";
                lbTop.Items.Clear();


                int count = 0;
                foreach (var drive in drivesToCheck)
                {
                    string parserToAwaitName = $"parser{drivesToCheck[count++]}";
                    parsersToAwait.Add(parserToAwaitName);
                    DriveParser parser = new DriveParser(parserToAwaitName, drive, bannedWords, destinationPath);
                    parser.SignalFilesToCheck += Parser_SignalFilesToCheck;
                    parser.GetLastTransferred += Parser_GetLastTransferred;
                    Thread parsingThread = new Thread(parser.StartSearching);
                    parsingThread.Start();
                    runningParsers.Add(parser);
                }
                btnStart.IsEnabled = false;
                btnStop.IsEnabled = true;
                btnFreeze.IsEnabled = true;
            }

        }

        private void Parser_GetLastTransferred(Dictionary<string, int> bannedWordsCounter,int fileCount,bool parserFinished, DriveParser parser)
        {
            foreach (var item in bannedWordsCounter.Keys)
            {
                int timesEncountered = bannedWordsCounter[item];

                if (encounteredBannedWords.ContainsKey(item))
                    encounteredBannedWords[item] += timesEncountered;
                else
                    encounteredBannedWords.Add(item, timesEncountered);
            }
            filesSearched += fileCount;
            if (parserFinished)
            {
                string message = $"Parser on drive {parser.name.Remove(0, 6)} ended his work";
                if(wasStartedFromConsole)
                    Console.WriteLine(message);
                else
                    MessageBox.Show(message, "Result", MessageBoxButton.OK, MessageBoxImage.Information);
                parsersResult += parser.Report;
                runningParsers.Remove(parser);
            }
            UpdateCounter();
        }

        private void Parser_SignalFilesToCheck(string name,int num)
        {
            filesToSearch += num;
            runningParsers.Where(x=>x.name == name).ToArray()[0].startAwaiter.Set();
            parsersToAwait.Remove(name);
            UpdateCounter();
        }

        private void btnFreeze_Click(object sender, RoutedEventArgs e)
        {
            foreach (DriveParser parser in runningParsers)
            {
                parser.State = ParserState.SUSPENDED;
            }
            btnStop.IsEnabled = false;
            btnFreeze.IsEnabled = false;
            btnResume.IsEnabled = true;
            edState.Text = "Process frozen";
        }

        private void btnStop_Click(object sender, RoutedEventArgs e)
        {
            FinishSearching();
            if (!wasStartedFromConsole)
            {
                btnStop.IsEnabled = false;
                btnResume.IsEnabled = false;
                btnFreeze.IsEnabled = false;
                btnStart.IsEnabled = true;
                edState.Text = "Not started";
            }
        }

        private void FinishSearching()
        {
            foreach (DriveParser parser in runningParsers)
            {
                parser.State = ParserState.ABORTED;
            }
            runningParsers.Clear();
            List<int> appearences = encounteredBannedWords.Select(x => x.Value).ToList();
            appearences.Sort();
            appearences.Reverse();
            List<int> top10List = new List<int>();
            for (int i = 0; i < appearences.Count && i < 10; i++)
            {
                top10List.Add(appearences[i]);
            }
            HashSet<string> copyHolder = new HashSet<string>();
            List<string> wordsInTop10 = new List<string>();
            for (int i = 0; i < top10List.Count; i++)
            {
                int num = top10List[i];
                foreach (var item in encounteredBannedWords.Keys)
                {
                    if (encounteredBannedWords[item] == num)
                    {
                        string message = $"{item} -> {num}";
                        if (copyHolder.Add(message))
                        {
                            lbTop.Items.Add($"{i+1}. {message}");
                            wordsInTop10.Add($"{i + 1}. {message}");
                        }
                    }
                }
            }
            ReportWriter reporter = new ReportWriter(wordsInTop10, parsersResult, encounteredBannedWords, filesSearched,destinationPath);
            reportingThread = new Thread(reporter.WriteReport);
            reportingThread.Start();
            if (wasStartedFromConsole)
            {
                reportingThread.Join();
                Console.WriteLine("Report written");
            }
        }

        private void btnResume_Click(object sender, RoutedEventArgs e)
        {
            foreach (DriveParser parser in runningParsers)
            {
                parser.State = ParserState.RUNNING;
            }
            btnStop.IsEnabled = true;
            btnResume.IsEnabled = false;
            btnFreeze.IsEnabled = true;
            edState.Text = "Counting and processing files...";
        }

        private void btnChooseDestination_Click(object sender, RoutedEventArgs e)
        {
            var folderBrowser = new VistaFolderBrowserDialog();
            if (folderBrowser.ShowDialog() != true) return;
            destinationPath = folderBrowser.SelectedPath;
            lblDestination.Text = destinationPath;
        }

        #endregion

        #region Banned Words

        private void UpdateBannedWordsList() 
        {
            lbBannedWords.Items.Clear();
            for (int i = 0; i < bannedWords.Count; i++)
            {
               lbBannedWords.Items.Add(bannedWords[i]);
            }
        }

        private void btnLoadFromFile_Click(object sender, RoutedEventArgs e)
        {
            var bannedWordsPath = new OpenFileDialog();
            bannedWordsPath.Filter = "Txt files|*.txt";
            if (bannedWordsPath.ShowDialog() != true) return;
            bannedWords.Clear();
            HashSet<string> words = new HashSet<string>();
            foreach (var item in File.ReadAllLines(bannedWordsPath.FileName))
            {
                words.Add(item);
            }
            foreach (var item in words)
            {
                bannedWords.Add(item);
            }
            UpdateBannedWordsList();
        }

        private void btnAddBannedWord_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(edBannedWord.Text)) 
            {
                MessageBox.Show("Enter the banned word, to add it");
                return;
            }
            if (bannedWords.Contains(edBannedWord.Text)) 
            {
                MessageBox.Show("This banned word is already listed");
                return;
            }
            bannedWords.Add(edBannedWord.Text);
            UpdateBannedWordsList();
            edBannedWord.Text = "";
            edBannedWord.Focus();
        }

        private void lbBannedWords_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (lbBannedWords.SelectedItem == null) return;
            bannedWords.Remove((string)lbBannedWords.SelectedItem);
            UpdateBannedWordsList();
        }

        #endregion
    }
}
