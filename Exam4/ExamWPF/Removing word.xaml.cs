using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
    /// Interaction logic for Removing_word.xaml
    /// </summary>
    public partial class Removing_word : Window
    {
        static ObservableCollection<string> words = new ObservableCollection<string>(Programm.dictionaries[Programm.keys.IndexOf(Words.dict)].dictionary.Keys.ToList());
        public Removing_word()
        {
            InitializeComponent();
            remove.IsEnabled = false;
            listbox1.ItemsSource = words;
        }

        private void Item_Selected(object sender, SelectionChangedEventArgs e)
        {
            remove.IsEnabled = true;
        }

        private void Btn_Exit(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void Btn_Remove(object sender, RoutedEventArgs e)
        {
            Programm.dictionaries[Programm.keys.IndexOf(Words.dict)].dictionary.Remove(listbox1.SelectedItem as string);
            words.Remove(listbox1.SelectedItem as string);
            remove.IsEnabled = false;
        }
    }
}
