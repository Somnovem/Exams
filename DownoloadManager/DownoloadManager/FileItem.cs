using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace DownoloadManager
{
    internal class FileItem : INotifyPropertyChanged
    {
        private string _filename;
        public string FileName
        {
            get { return _filename; }
            set
            {
                if (_filename != value)
                {
                    _filename = value;
                    OnPropertyChanged(nameof(FileName));
                }
            }
        }

        private FileItemStatus _status;
        public FileItemStatus Status
        {
            get { return _status; }
            set
            {
                if (_status != value)
                {
                    _status = value;
                    OnPropertyChanged(nameof(Status));
                }
            }
        }

        private string _size;
        public string Size
        {
            get { return _size; }
            set
            {
                if (_size != value)
                {
                    _size = value;
                    OnPropertyChanged(nameof(Size));
                }
            }
        }

        private string _tag;
        public string Tag
        {
            get { return _tag; }
            set
            {
                if (_tag != value)
                {
                    _tag = value;
                    OnPropertyChanged(nameof(Tag));
                }
            }
        }

        private string _fullpath;
        public string FullPath
        {
            get { return _fullpath; }
            set
            {
                if (_fullpath != value)
                {
                    _fullpath = value;
                    OnPropertyChanged(nameof(FullPath));
                }
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;
        protected void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
    public enum FileItemStatus 
    {
        DOWNLOADING,
        DOWNLOADED,
        FROZEN,
        ABORTED
    }
}
