using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace DownoloadManager
{


    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Downloader downloader;
        private MainViewModel viewModel;
        private string destinationPath;
        private SaveFileDialog saveFileDialog;
        private ArgumentException argumentHandler;
        public MainWindow()
        {
            InitializeComponent();
            argumentHandler = new ArgumentException();
            DataContext = new MainViewModel();
            viewModel = (MainViewModel)DataContext;
            saveFileDialog = new SaveFileDialog();
            saveFileDialog.InitialDirectory = "C:\\";
            saveFileDialog.Title = "Store and save files as...";
            destinationPath = "";
        }

        #region Support Functions

        private void ShowError(string message)
        {
            MessageBox.Show(message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
        }

        private void InvokeAction(Action a)
        {
            try
            {
                this.Dispatcher.Invoke(a);
            }
            catch
            {
                //The task was canceled
            }
        }

        private void OpenFileItem(object selectedItem)
        {
            FileItem file = selectedItem as FileItem;
            if (file.Status != FileItemStatus.DOWNLOADED)
            {
                ShowError("File is not yet downloaded or was aborted");
                return;
            }
            try
            {
                ProcessStartInfo startInfo = new ProcessStartInfo(file.FullPath);
                startInfo.UseShellExecute = true;
                Process.Start(startInfo);
            }
            catch
            {
                ShowError("Requested file was deleted");
                viewModel.FileItems.Remove(file);
                lbSearched.Items.Remove(file);
                return;
            }
        }

        #endregion

        #region Downloader Events

        private void Downloader_NoFileFound()
        {
            ShowError("No such file found");
            InvokeAction(() =>
            {
                viewModel.FileItems[0].Status = FileItemStatus.ABORTED;
                btnDownload.IsEnabled = true;
            });
        }

        private void Downloader_FileSizeCounted(long size)
        {
            viewModel.FileItems[0].Size = $"{size} bytes";
        }

        private void Downloader_DownloadFinished()
        {
            InvokeAction(() =>
            {
                viewModel.FileItems[0].Status = FileItemStatus.DOWNLOADED;
                btnDownload.IsEnabled = true;
                btnFreeze.IsEnabled = false;
                btnAbort.IsEnabled = false;
            });
            MessageBox.Show("Download finished", "Success", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        #endregion

        #region Click Events

        private void btnAbort_Click(object sender, RoutedEventArgs e)
        {
            downloader.AbortDownload();
            InvokeAction(() =>
            {
                viewModel.FileItems[0].Status = FileItemStatus.ABORTED;
                btnDownload.IsEnabled = true;
                btnFreeze.IsEnabled = false;
                btnAbort.IsEnabled = false;
                btnResume.IsEnabled = false;
            });
        }

        private void btnFreeze_Click(object sender, RoutedEventArgs e)
        {
            downloader.PauseDownload();
            InvokeAction(() =>
            {
                viewModel.FileItems[0].Status = FileItemStatus.FROZEN;
                btnFreeze.IsEnabled = false;
                btnResume.IsEnabled = true;
            });
        }

        private void btnResume_Click(object sender, RoutedEventArgs e)
        {
            downloader.ResumeDownload();
            InvokeAction(() =>
            {
                viewModel.FileItems[0].Status = FileItemStatus.DOWNLOADING;
                btnFreeze.IsEnabled = true;
                btnResume.IsEnabled = false;
            });
        }

        private void btnOpenFile_Click(object sender, RoutedEventArgs e)
        {
            if (lbFiles.SelectedItem == null) return;
            OpenFileItem(lbFiles.SelectedItem);
        }

        private void btnRenameFile_Click(object sender, RoutedEventArgs e)
        {
            if (lbFiles.SelectedItem == null) return;
            FileItem file = lbFiles.SelectedItem as FileItem;
            if (file.Status != FileItemStatus.DOWNLOADED)
            {
                ShowError("File is not yet downloaded or was aborted");
                return;
            }
            SaveFileDialog renameDialog = new SaveFileDialog();
            FileInfo info = new FileInfo(file.FullPath);
            renameDialog.Title = "Choose the path where to store the renamed file...";
            renameDialog.FileName = file.FileName;
            if (renameDialog.ShowDialog() != true) return;
            if (info.FullName.Equals(renameDialog.FileName))
            {
                return;
            }
            File.Copy(info.FullName, renameDialog.FileName);
            file.FullPath = renameDialog.FileName;
            file.FileName = renameDialog.SafeFileName;
        }

        private void btnMoveFile_Click(object sender, RoutedEventArgs e)
        {
            if (lbFiles.SelectedItem == null) return;
            FileItem file = lbFiles.SelectedItem as FileItem;
            if (file.Status != FileItemStatus.DOWNLOADED)
            {
                ShowError("File is not yet downloaded or was aborted");
                return;
            }
            SaveFileDialog moveDialog = new SaveFileDialog();
            FileInfo info = new FileInfo(file.FullPath);
            moveDialog.InitialDirectory = info.DirectoryName;
            moveDialog.Title = "Choose the new path of the file...";
            if (moveDialog.ShowDialog() != true) return;
            File.Move(file.FullPath, moveDialog.FileName);
            file.FullPath = moveDialog.FileName;
            file.FileName = moveDialog.SafeFileName;
        }

        private void btnDeleteFile_Click(object sender, RoutedEventArgs e)
        {
            if (lbFiles.SelectedItem == null) return;
            FileItem file = lbFiles.SelectedItem as FileItem;
            if (file.Status != FileItemStatus.DOWNLOADED)
            {
                ShowError("File is not yet downloaded or was aborted");
                return;
            }
            MessageBoxResult dialogResult = MessageBox.Show("Do you really want to delete this file?", "Attention", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (dialogResult == MessageBoxResult.Yes)
            {
                File.Delete(file.FullPath);
                viewModel.FileItems.Remove(file);
            }
        }

        private void btnChangeDestination_Click(object sender, RoutedEventArgs e)
        {
            if (saveFileDialog.ShowDialog() != true) return;
            destinationPath = saveFileDialog.FileName;
            edDestination.Text = destinationPath;
        }

        private void btnDownload_Click(object sender, RoutedEventArgs e)
        {
            btnDownload.IsEnabled = false;
            string tag;
            int threadCount;
            try
            {
                if (string.IsNullOrEmpty(edDownloadUrl.Text))
                {
                    ShowError("No url provided");
                    throw argumentHandler;
                }
                if (string.IsNullOrEmpty(destinationPath))
                {
                    ShowError("No destination path chosen");
                    throw argumentHandler;
                }

                threadCount = 0;
                if (!int.TryParse(edNumThreads.Text, out threadCount) || threadCount < 1)
                {
                    ShowError("Input thread-count is not a natural number");
                    throw argumentHandler;
                }

                tag = edTag.Text;
                if (!string.IsNullOrEmpty(tag))
                {
                    if (tag.Length < 4 || tag.Length > 20)
                    {
                        ShowError("Tag must be 4-20 characters long");
                        throw argumentHandler;
                    }
                }
            }
            catch
            {
                btnDownload.IsEnabled = true;
                return;
            }
            for (int i = 0; i < viewModel.FileItems.Count; i++)
            {
                if (viewModel.FileItems[i].FullPath.Equals(destinationPath))
                {
                    viewModel.FileItems.RemoveAt(i); //already a file at this path, so remove the item
                    return;
                }
            }
            downloader = new Downloader(edDownloadUrl.Text, destinationPath, threadCount);
            downloader.DownloadFinished += Downloader_DownloadFinished;
            downloader.FileSizeCounted += Downloader_FileSizeCounted;
            downloader.NoFileFound += Downloader_NoFileFound;
            viewModel.FileItems.Insert(0, new FileItem()
            {
                FileName = saveFileDialog.SafeFileName,
                Status = FileItemStatus.DOWNLOADING,
                Tag = tag,
                FullPath = destinationPath
            });
            downloader.StartDownload();
            btnAbort.IsEnabled = true;
            btnFreeze.IsEnabled = true;
        }

        private void btnSearchTag_Click(object sender, RoutedEventArgs e)
        {
            lbSearched.Items.Clear();
            string searchedTag = edSearchTag.Text;
            FileItem[] matches = viewModel.FileItems.Where(x => x.Tag.Equals(searchedTag)).ToArray();
            for (int i = 0; i < matches.Length; i++)
            {
                lbSearched.Items.Add(matches[i]);
            }
        }

        private void lbSearched_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (lbSearched.SelectedItem == null) return;
            OpenFileItem(lbSearched.SelectedItem);
        }

        #endregion
    }
}
