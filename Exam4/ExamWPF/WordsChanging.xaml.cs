using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.IO;
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
    /// Interaction logic for Words.xaml
    /// </summary>
    /// 

    class Wording
    {
        public string Word { get; set; }
        public string Translations { get; set; }
    }
    public partial class Words : Window
    {
       /// <summary>
       /// refills the content of the grid from the given dictionary
       /// </summary>
       /// <param name="dictionary">dictionary for words to be pulled from</param>
        public void LoadBox(string dictionary)
        {
            ObservableCollection<Wording> list = new ObservableCollection<Wording>();
            if (Programm.dictionaries.Count == 0)
            {
                grid1.ItemsSource = new List<MyDictionary>();
                return;
            }
            var temp = Programm.dictionaries[Programm.keys.IndexOf(dictionary)].GetValues();
            list.Clear();
            foreach (var item in temp.Key)
            {
                list.Add(new Wording() { Word = item });
            }
            for (int j = 0; j < temp.Value.Count; j++)
            {
                list.ElementAt(j).Translations = temp.Value.ElementAt(j);
            }
            if (list.Count == 0)
            {
                grid1.ItemsSource = new List<MyDictionary>();
                return;
            }
            grid1.ItemsSource = list.Take(list.Count);
                grid1.Columns[0].Width = new DataGridLength(20, DataGridLengthUnitType.Star);
                grid1.Columns[1].Width = new DataGridLength(80, DataGridLengthUnitType.Star);
            grid1.VerticalScrollBarVisibility = ScrollBarVisibility.Visible;
        }
        public Words()
        {
            InitializeComponent();
            box1.ItemsSource = Programm.keys;
            grid1.CanUserAddRows = true;
            grid1.CanUserDeleteRows = true;
        }      
        public static string dict = "";

        private void Dict_Selected(object sender, SelectionChangedEventArgs e)
        {
            LoadBox(box1.SelectedItem as string);
            dict = box1.SelectedItem as string;
        }
        /// <summary>
        /// Exiting back to the main menu
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Exit(object sender, RoutedEventArgs e)
        {
            Window window = new MainWindow();
            window.Show();
            this.Close();
        }
        /// <summary>
        /// Removing the selected item from the grid
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Remove(object sender, RoutedEventArgs e)
        {
            if (box1.SelectedItem == null)
            {
                return;
            }
            Window window = new Removing_word();
            window.Show();
            LoadBox(dict);
        }
        /// <summary>
        /// Opening a window for the user to add a word
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Add(object sender, RoutedEventArgs e)
        {
            if (box1.SelectedItem != null)
            {
                Window window = new Adding_word();
                window.Show();
            }
        }
        /// <summary>
        /// Reload the grid when the user returns to the main window of editing
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Focus_Gained(object sender, RoutedEventArgs e)
        {
            if (box1.SelectedItem != null)
            {
                LoadBox(dict);
            }
           
        }
        /// <summary>
        /// Open the window for the user to change properties of the contained words
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Change(object sender, RoutedEventArgs e)
        {
            if (box1.SelectedItem !=null)
            {
                Window window = new Editing();
                window.Show();
            }

        }
    }
}
