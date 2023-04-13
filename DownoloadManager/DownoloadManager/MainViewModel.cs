using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DownoloadManager
{
    internal class MainViewModel
    {
        public ObservableCollection<FileItem> FileItems { get; set; }
        public MainViewModel()
        {
            FileItems = new ObservableCollection<FileItem>();
        }
    }
}
