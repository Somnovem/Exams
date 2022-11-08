using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
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
    /// Interaction logic for Removing.xaml
    /// </summary>
    public partial class Removing : Window
    {
        public Removing()
        {
            InitializeComponent();
            listbox1.ItemsSource = Programm.keys;
            btn_remove.IsEnabled = false;
        }
        private void Btn_Exit(object sender, RoutedEventArgs e)
        {
            Window window = new MainWindow();
            window.Show();
            this.Close();
        }
        /// <summary>
        /// Asks the user if the user wants do delete the dictionary and does so if needed
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Remove(object sender, RoutedEventArgs e)
        {
            var d = MessageBox.Show("Do you want to remove the dictionary?", "Caution", MessageBoxButton.YesNo, MessageBoxImage.Warning);
            if (d == MessageBoxResult.Yes)
            {
                Programm.RemoveDictionary(listbox1.SelectedItem as string);
                btn_remove.IsEnabled = false;
            }
        }
        /// <summary>
        /// Enables the button to remove a selected dictionary
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void listbox_SelectedIndexChanged(object sender, SelectionChangedEventArgs e)
        {
            btn_remove.IsEnabled = true;
        }
    }
}
