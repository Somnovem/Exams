using System;
using System.Collections.Generic;
using System.IO;

namespace BannedWordsSearcher
{
    internal class ReportWriter
    {
        private List<string> top10;
        private Dictionary<string, int> totalEncountered;
        private int totalFilesScanned;
        private string destinationPath;
        string parsersReport;
        public ReportWriter(List<string> top10, string parsersReport, 
                            Dictionary<string, int> totalEncountered, int totalFilesScanned, string destinationPath)
        {
            this.top10 = top10;
            this.parsersReport = parsersReport;
            this.totalEncountered = totalEncountered;
            this.totalFilesScanned = totalFilesScanned;
            this.destinationPath = destinationPath;
        }
        public void WriteReport()
        {
            using (StreamWriter writer = new StreamWriter(Path.Combine(destinationPath, "Report.txt")))
            {
                writer.WriteLine($"Action performed: {DateTime.Now}");
                writer.WriteLine($"Total files scanned: {totalFilesScanned}");
                writer.WriteLine("Top 10:");
                foreach (var item in top10)
                {
                    writer.WriteLine(item);
                }
                writer.WriteLine("-------------------------------------");
                writer.WriteLine("Total encountered:");
                foreach (var item in totalEncountered.Keys)
                {
                    writer.WriteLine($"{item} -> {totalEncountered[item]}");
                }
                writer.WriteLine("-------------------------------------");
                writer.WriteLine(parsersReport);
            }
        }
    }
}
