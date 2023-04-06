using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Forms;
using Application = System.Windows.Application;
using MessageBox = System.Windows.Forms.MessageBox;
namespace BannedWordsSearcher
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            string[] args = e.Args;
            if (args.Length == 0)
            {
                var window = new MainWindow();

                window.ShowDialog();
            }
            else if(args.Length == 1 && args[0] == "-c")
            {
                AllocConsole();
                Console.Title = "Banned words";
                Console.Write("Destination path: ");
                string destinationPath = Console.ReadLine();
                if (!Directory.Exists(destinationPath))
                {
                    Console.WriteLine("Such directory doesn't exist");
                    return;
                }
                Console.Write("Banned words(separated by spaces): ");
                string[] bannedWords = Console.ReadLine().Split(" ".ToCharArray(),StringSplitOptions.RemoveEmptyEntries);
                var window = new MainWindow(true,destinationPath,bannedWords);
                window.ShowDialog();
            }
        }

        [DllImport("Kernel32.dll")]
        static extern void AllocConsole();
    }
}
