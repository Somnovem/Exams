using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Ookii.Dialogs.Wpf;

namespace ActivityMonitor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private List<string> bannedWords;
        private List<string> bannedProcesses;
        private string destinationPath;
        private KeyMonitor keyMonitor;
        private ProcessMonitor processMonitor;
        private EventLogger eventLogger;
        public MainWindow()
        {
            bannedWords = new List<string>();
            bannedProcesses = new List<string>();
            this.StateChanged += MainWindow_StateChanged;
            InitializeComponent();
        }

        private void btnAddBannedWord_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(edBannedWord.Text))
            {
                MessageBox.Show("No banned word to add", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            if (bannedWords.Contains(edBannedWord.Text))
            {
                MessageBox.Show("Such banned word was already added", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            bannedWords.Add(edBannedWord.Text);
            edBannedWord.Text = "";
            UpdateListBox(lbBannedWords,bannedWords);
        }

        private void btnAddBannedProcess_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(edBannedProcesses.Text))
            {
                MessageBox.Show("No banned processname to add", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            if (bannedWords.Contains(edBannedProcesses.Text))
            {
                MessageBox.Show("Such banned processname was already added", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            bannedProcesses.Add(edBannedProcesses.Text);
            edBannedProcesses.Text = "";
            UpdateListBox(lbBannedProcesses, bannedProcesses);
        }

        private void btnStartMonitoring_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(destinationPath))
            {
                MessageBox.Show("No destination path was chosen", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            lbPressedKeys.Items.Clear();
            lbEnteredBannedWords.Items.Clear();
            lbStartedBannedProcesses.Items.Clear();


            eventLogger = new EventLogger(destinationPath);
            keyMonitor = new KeyMonitor(bannedWords.ToArray());
            processMonitor = new ProcessMonitor(bannedProcesses.ToArray());

            keyMonitor.KeyPressed += KeyMonitor_KeyPressed;
            keyMonitor.BannedWordEntered += KeyMonitor_BannedWordEntered;
            

            processMonitor.NewProcessesStarted += ProcessMonitor_NewProcessesStarted;
            if (bannedProcesses.Count != 0)
            {
                processMonitor.BannedProcessStarted += ProcessMonitor_BannedProcessStarted;
            }

            processMonitor.StartMonitoringAsync();
            keyMonitor.StartMonitoring();
            StateManager(wasStarted:true);
        }

        private void btnStopMonitoring_Click(object sender, RoutedEventArgs e)
        {
            StopWork();
            keyMonitor = null;
            processMonitor = null;
            eventLogger = null;
            StateManager(wasStarted: false);
        }

        private void btnSelectDestination_Click(object sender, RoutedEventArgs e)
        {
            var folderBrowser = new VistaFolderBrowserDialog();
            if (folderBrowser.ShowDialog() != true) return;
            destinationPath = folderBrowser.SelectedPath;
            edDestinationPath.Text = destinationPath;
        }

        private void lbBannedWords_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (lbBannedWords.SelectedItem == null) return;
            bannedWords.Remove((string)lbBannedWords.SelectedItem);
            UpdateListBox(lbBannedWords, bannedWords);
        }

        private void lbBannedProcesses_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (lbBannedProcesses.SelectedItem == null) return;
            bannedProcesses.Remove((string)lbBannedProcesses.SelectedItem);
            UpdateListBox(lbBannedProcesses, bannedProcesses);
        }

        #region Window-support functions

        private void UpdateListBox(ListBox lb, List<string> items)
        {
            lb.Items.Clear();
            for (int i = 0; i < items.Count; i++)
            {
                lb.Items.Add(items[i]);
            }

        }

        private void StateManager(bool wasStarted)
        {
            btnStartMonitoring.IsEnabled = !wasStarted;
            btnStopMonitoring.IsEnabled = wasStarted;
        }

        private void StopWork()
        {
            keyMonitor?.Close();
            processMonitor?.Close();
            eventLogger?.Dispose();
        }

        #endregion

        #region Window events

        private void MainWindow_StateChanged(object? sender, System.EventArgs e)
        {
            if (WindowState == WindowState.Minimized)
            {
                ShowInTaskbar = false;
                var notifyIcon = new System.Windows.Forms.NotifyIcon();
                notifyIcon.Icon = new System.Drawing.Icon(@"..\..\..\..\Images\icon.ico");
                notifyIcon.Visible = true;
                notifyIcon.DoubleClick += (s, args) =>
                {
                    WindowState = WindowState.Normal;
                    ShowInTaskbar = true;
                    this.Activate();
                    notifyIcon.Visible = false;
                };
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            StopWork();
        }

        #endregion

        #region Handling events

        private void ProcessMonitor_NewProcessesStarted(System.Diagnostics.Process[] processes)
        {
            List<string> newProcesses = new List<string>();
            for (int i = 0; i < processes.Length; i++)
            {
                try
                {
                    newProcesses.Add($"New process '{processes[i].ProcessName}' started at {processes[i].StartTime}");
                }
                catch
                {
                    //Access denied by Win
                }
            }
            eventLogger.LogNewProcesses(newProcesses);
        }

        private void ProcessMonitor_BannedProcessStarted(System.Diagnostics.Process[] processes)
        {

            string[] temp = new string[processes.Length];
            for (int i = 0; i < processes.Length; i++)
            {
                temp[i] = processes[i].ProcessName;
            }

            this.Dispatcher.Invoke(() =>
            {
                for (int i = 0; i < temp.Length; i++)
                {
                    lbStartedBannedProcesses.Items.Add(temp[i]);
                }
            });


            eventLogger.LogBannedProcess(temp);
        }

        private void KeyMonitor_BannedWordEntered(string word)
        {
            this.Dispatcher.Invoke(() => { lbEnteredBannedWords.Items.Add(word); });
            eventLogger.LogBannedWord(word);
        }

        private void KeyMonitor_KeyPressed(string key)
        {
            this.Dispatcher.Invoke(() => { lbPressedKeys.Items.Add(key); });
            eventLogger.LogKey(key);
        }

        #endregion
    }
}
