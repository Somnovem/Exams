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
    /// Interaction logic for Adding_word.xaml
    /// </summary>
    public partial class Adding_word : Window
    {
        public Adding_word()
        {
            InitializeComponent();
        }
        /// <summary>
        /// Gets the input and adds it to the list
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Confirm(object sender, RoutedEventArgs e)
        {
            string word = Word.Text;
            string transl = Translat.Text;
            if (word != "" &&  transl!= "")
            {
                if (!transl.EndsWith(','))
                {
                    transl += ',';
                }
                if (!Programm.dictionaries[Programm.keys.IndexOf(Words.dict)].dictionary.ContainsKey(word))
                {
                    Programm.dictionaries[Programm.keys.IndexOf(Words.dict)].dictionary.Add(word, transl.Split(',', StringSplitOptions.RemoveEmptyEntries).ToList());
                    this.Close();
                }
            }
        }
    }
}
