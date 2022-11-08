using System;
using System.Collections.Generic;
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
using System.Windows.Controls.Primitives;
namespace ExamWPF
{
    /// <summary>
    /// Interaction logic for Adding.xaml
    /// </summary>
    public partial class Adding : Window
    {
        public Adding()
        {
            InitializeComponent();
            box1.ItemsSource = typeof(Languages).GetEnumNames();
            box2.ItemsSource = typeof(Languages).GetEnumNames();
        }
        /// <summary>
        /// Gets the input and adds it to the list
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Accept(object sender, RoutedEventArgs e)
        {
            int res = Programm.AddDictionary(box1.SelectedValue as string, box2.SelectedValue as string);
            if (res == 0)
            {
                MessageBox.Show("Cannot translate a language to itself", "Caution", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
            else if (res == -1)
            {
                MessageBox.Show("Such dictionary already exists", "Caution", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
            else
            {
                MessageBox.Show("Succesfully added", "Caution", MessageBoxButton.OK, MessageBoxImage.Information);
                Window window = new MainWindow();
                window.Show();
                this.Close();
            }
        }

        private void Btn_Exit(object sender, RoutedEventArgs e)
        {
            Window window = new MainWindow();
            window.Show();
            this.Close();
        }
    }
}
