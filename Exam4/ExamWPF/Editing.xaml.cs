using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
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
    /// Interaction logic for Editing.xaml
    /// </summary>
    public partial class Editing : Window
    {
        static string wordOld = "";

        public Editing()
        {
            InitializeComponent();
            WordChooser.ItemsSource = Programm.dictionaries[Programm.keys.IndexOf(Words.dict)].GetValues().Key;
            WordEditor.IsEnabled = false;
            TranslEditor.IsEnabled = false;
        }

        private void Btn_Exit(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        /// <summary>
        /// Initiates the editing of the chosen word
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void WordChosen(object sender, SelectionChangedEventArgs e)
        {
            WordEditor.IsEnabled = true;
            TranslEditor.IsEnabled = true;
            WordEditor.Text = WordChooser.SelectedItem as string;
            wordOld = WordEditor.Text;
            TranslEditor.Text = "";
            foreach (var item in Programm.dictionaries[Programm.keys.IndexOf(Words.dict)].dictionary[wordOld])
            {
                TranslEditor.Text += item + ", ";
            }
        }
        /// <summary>
        /// Confirmes the changes made and saves them to the list
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Confirm(object sender, RoutedEventArgs e)
        {
            if (WordEditor.Text == "")
            {
                MessageBox.Show("The word cant be empty", "Caution", MessageBoxButton.OK, MessageBoxImage.Stop);
                return;
            }
            var temp = TranslEditor.Text.Split(',', StringSplitOptions.RemoveEmptyEntries).ToList();
            if (temp.Count == 0)
            {
                MessageBox.Show("Can't leave the translation list empty", "Caution", MessageBoxButton.OK, MessageBoxImage.Stop);
                return;
            }
            if (temp.Last() == " ") temp.RemoveAt(temp.Count - 1);
            if (wordOld != WordEditor.Text)
            {
                Programm.dictionaries[Programm.keys.IndexOf(Words.dict)].dictionary.Remove(wordOld);

                Programm.dictionaries[Programm.keys.IndexOf(Words.dict)].dictionary.Add(WordEditor.Text,temp);
            }
            else
            {
                Programm.dictionaries[Programm.keys.IndexOf(Words.dict)].dictionary[wordOld] = temp;
            }

            WordEditor.Text = "";
            TranslEditor.Text = "";
            WordEditor.IsEnabled = false;
            TranslEditor.IsEnabled = false;

        }
    }
}
