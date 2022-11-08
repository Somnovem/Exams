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
using System.Windows.Shapes;

namespace ExamWPF
{
    /// <summary>
    /// Interaction logic for Searching.xaml
    /// </summary>
    public partial class Searching : Window
    {
        public Searching()
        {
            InitializeComponent();
        }

        private void Btn_Exit(object sender, RoutedEventArgs e)
        {
            Window window = new MainWindow();
            window.Show();
            this.Close();
        }
        /// <summary>
        /// Get possible translations for the input word
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Search(object sender, RoutedEventArgs e)
        {
            string searching = box1.Text;
            block1.Text = Programm.SearchTranslations(searching);
        }
    }
}
