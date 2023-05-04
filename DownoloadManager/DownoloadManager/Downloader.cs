using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DownoloadManager
{
    internal class Downloader 
    {
        private string url;
        private string savePath;
        private int numThreads;
        private int aliveThreadsCount;
        private List<DownloadThread> threads;

        public event Action NoFileFound;
        public delegate void FileSizeCountedDelegate(long size);
        public event FileSizeCountedDelegate FileSizeCounted;

        public event Action DownloadFinished;

        public Downloader(string url, string savePath, int numThreads)
        {
            this.url = url;
            this.savePath = savePath;
            this.numThreads = numThreads;
            this.threads = new List<DownloadThread>();
        }

        public void StartDownload()
        {
            aliveThreadsCount = numThreads;
            HttpWebRequest request = null;
            HttpWebResponse response = null;
            try
            {
                request = (HttpWebRequest)WebRequest.Create(url);
                response = (HttpWebResponse)request.GetResponse();
            }
            catch 
            {
                NoFileFound?.Invoke();
                return;
            }
            long fileSize = response.ContentLength;
            response.Close();
            FileSizeCounted?.Invoke(fileSize);
            long blockSize = fileSize / numThreads;
            for (int i = 0; i < numThreads; i++)
            {
                long startIndex = i * blockSize;
                long endIndex = (i + 1) * blockSize - 1;

                if (i == numThreads - 1)
                {
                    endIndex = fileSize - 1;
                }

                DownloadThread downloadThread = new DownloadThread(url ,startIndex, endIndex);
                downloadThread.WorkFinished += DownloadThread_WorkFinished;
                threads.Add(downloadThread);
                Thread thread = new Thread(new ThreadStart(downloadThread.StartDownload));
                thread.Start();
            }
        }

        private void DownloadThread_WorkFinished()
        {
            Interlocked.Decrement(ref aliveThreadsCount);
            if(aliveThreadsCount == 0)Finish();
        }

        private void Finish()
        {
            List<byte> fullFile = new List<byte>();
            for (int i = 0; i < threads.Count; i++)
            {
                fullFile.AddRange(threads[i].data);
            }
            File.WriteAllBytes(savePath, fullFile.ToArray());
            DownloadFinished?.Invoke();
        }

        public void PauseDownload()
        {
            foreach (DownloadThread thread in threads)
            {
                thread.PauseDownload();
            }
        }

        public void ResumeDownload()
        {
            foreach (DownloadThread thread in threads)
            {
                thread.ResumeDownload();
            }
        }

        public void AbortDownload()
        {
            foreach (DownloadThread thread in threads)
            {
                thread.AbortDownload();
            }
        }

        private class DownloadThread
        {
            private string url;
            private long startIndex;
            private long endIndex;
            private bool isPaused;
            private bool isAborted;
            public List<byte> data;

            public event Action WorkFinished;

            public DownloadThread(string url, long startIndex, long endIndex)
            {
                this.url = url;
                this.startIndex = startIndex;
                this.endIndex = endIndex;
                this.isPaused = false;
                this.isAborted = false;
                data = new List<byte>();
            }

            public void StartDownload()
            {
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
                request.AddRange(startIndex, endIndex);
                HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                Stream stream = response.GetResponseStream();
                byte[] buffer = new byte[4096];
                int bytesRead;
                while ((bytesRead = stream.Read(buffer, 0, buffer.Length)) > 0)
                {
                    while (isPaused)
                    {
                        if (isAborted)
                        {
                            stream.Close();
                            return;
                        }

                        Thread.Sleep(100);
                    }
                    data.AddRange(buffer.Take(bytesRead));

                    if (isAborted)
                    {
                        stream.Close();
                        return;
                    }
                }

                stream.Close();
                WorkFinished?.Invoke();
            }
            public void PauseDownload()
            {
                isPaused = true;
            }
            public void ResumeDownload()
            {
                isPaused = false;
            }
            public void AbortDownload()
            {
                isAborted = true;
            }
        }
    }

}
