using System;
using System.Collections.Generic;
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
namespace ExamWPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        
        public MainWindow()
        {
            InitializeComponent();
        }
        /// <summary>
        /// Open a window to add a dictionary
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_AddDict(object sender, RoutedEventArgs e)
        {
            Window wnd = new Adding();
            wnd.Show();
            this.Close();
        }
        /// <summary>
        /// Open a window to remove a dictionary
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_RemoveDict(object sender, RoutedEventArgs e)
        {
            if (Programm.keys.Count !=0)
            {
                Window window = new Removing();
                window.Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("No dictionaries", "Caution", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }
        /// <summary>
        /// Performs the save and exit
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Exit(object sender, RoutedEventArgs e)
        {
            Programm.Save();
            this.Close();
        }
        /// <summary>
        /// Open a window to search a translations for an input
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Search(object sender, RoutedEventArgs e)
        {
            if (Programm.keys.Count != 0)
            {
                Window window = new Searching();
                window.Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("No dictionaries", "Caution", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }
        /// <summary>
        /// Open a window to change a dictionary
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Words(object sender, RoutedEventArgs e)
        {
            if (Programm.keys.Count != 0)
            {
                Window window = new Words();
                window.Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("No dictionaries", "Caution", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }
    }
}
