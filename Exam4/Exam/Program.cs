using Menu;
using System.Collections;
using System.Data;
using System.Net;
using System.Net.Http.Headers;
using System.Xml;
namespace Exam
{
    enum Languages
    { UKRAINIAN = 0, ENGLISH, RUSSIAN, FRENCH, ITALIAN, SPANISH, GERMAN, POLISH }
    //Ukrainian has a value of zero, but is still the top 1.

    //Holds global methods
    static class GlobalMethods
    {
        public static int BuildCenteredMenu(List<string> list)
        {
            return Menu.ConsoleMenu.SelectVertical(HPosition.Center, VPosition.Center, HorizontalAlignment.Center, list);
        }
        public static int BuildCenteredMenu(string[] array)
        {
            return Menu.ConsoleMenu.SelectVertical(HPosition.Center, VPosition.Center, HorizontalAlignment.Center, array);
        }
        public static void ShowErrorMessage(string er)
        {
            Console.Clear();
            Console.SetCursorPosition(15, 15);
            Console.WriteLine(er);
            Console.Read();
            Console.ReadLine();
        }
        public static bool GetYesOrNo(string s)
        {
            Console.SetCursorPosition(25, 10);
            Console.WriteLine(s);
            return (Menu.ConsoleMenu.SelectVertical(HPosition.Center, VPosition.Center, HorizontalAlignment.Center, "Yes", "No") == 1) ? false : true;
        }
    }


    //Data Class that is the dictionary itself
    class MyDictionary : IEnumerable
    {
        Dictionary<string, List<string>> dictionary = new();
        public string ChooseWord()
        {
            if (dictionary.Count < 2) return "";
            return dictionary.ElementAt(Menu.ConsoleMenu.SelectVertical(HPosition.Center, VPosition.Center, HorizontalAlignment.Center, dictionary.Keys.ToArray())).Key;
        }
        public string ChooseTranslation(string word)
        {
            if (dictionary[word].Count < 1) return "";
            return dictionary[word].ElementAt(Menu.ConsoleMenu.SelectVertical(Menu.HPosition.Center, Menu.VPosition.Center, Menu.HorizontalAlignment.Center, dictionary[word]));
        }
        public void AddWord(string newWord)
        {
            dictionary.Add(newWord, new List<string>());
            AddTranslation(newWord);
        }
        public void RemoveWord()
        {
            if (dictionary.Count == 0)
            {
                Console.WriteLine("The dictionary is empty.There is nothing to remove.");
                Console.Read();
                return;
            }
            //choosing a word to remove
            string word = ChooseWord();
            if (word == "")
            {
                GlobalMethods.ShowErrorMessage("Not enough words in the dictionary");
                return;
            }
            dictionary.Remove(word);
        }
        public void Clear()
        {
            if (dictionary.Count != 0 && GlobalMethods.GetYesOrNo("Do you want to clear the dictionary?")) dictionary.Clear();
        }
        public void AddTranslation(string word)
        {
            Console.Clear();
            Console.SetCursorPosition(20, 10);
            if (dictionary[word].Count !=0)Console.Write("Other ");
            Console.Write($"translation of the word {word}: ");
            string newTranslation = Console.ReadLine();
            if (dictionary[word].Contains(newTranslation))
            {
                return;
            }
            dictionary[word].Add(newTranslation);
        }
        public void RemoveTranslation() 
        {
            string word = ChooseWord();
            if (word == "")
            {
                GlobalMethods.ShowErrorMessage("Not enough words in the dictionary");
                return;
            }
            if (dictionary[word].Count == 1)
            {
                Console.Clear();
                Console.WriteLine("There is 1 element left. You cannot delete it.");
                Console.Read();
                return;
            }
            string translation = ChooseTranslation(word);
            dictionary[word].Remove(translation);
        }
        public void ChangeWord()
        {
        string wordToChange = ChooseWord();
            if (wordToChange == "")
            {
                GlobalMethods.ShowErrorMessage("Not enough words in the dictionary");
                return;
            }
            Console.SetCursorPosition(0, 0);
            Console.WriteLine("Choose word to change");
            var Transltaions = dictionary[wordToChange];
            Console.Clear();
            dictionary.Remove(wordToChange);
            Console.WriteLine($"The word {wordToChange} will be replaced with : ");
            string newWord= Console.ReadLine();
            dictionary[newWord] = Transltaions;
        }
        public void ChangeTranslation()
        {
            Console.Clear();
            string word = ChooseWord();
            if (word == "")
            {
                GlobalMethods.ShowErrorMessage("Not enough word in the dictionary");
                return;
            }
            Console.Clear();
            string translation = ChooseTranslation(word);
            if (word == "")
            {
                GlobalMethods.ShowErrorMessage("Not enough translations of the word");
                return;
            }
            Console.Clear();
            Console.SetCursorPosition(15, 15);
            Console.Write("New transltaion:");
            dictionary[word][dictionary[word].LastIndexOf(translation)]  = Console.ReadLine();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return ((IEnumerable)dictionary).GetEnumerator();
        }
        public void Save(StreamWriter sw)
        {
            foreach (string word in dictionary.Keys.ToArray())
            {
                sw.WriteLine(word);
                foreach (string translation in dictionary[word])
                {
                    sw.Write($"{translation},");
                }
                sw.Write('\n');
            }
        }
        public void Load(StreamReader sr)
        {
            while (!sr.EndOfStream)
            {
                string word = sr.ReadLine();
                List<string> translations = sr.ReadLine().Split(',', StringSplitOptions.RemoveEmptyEntries).ToList();
                dictionary.Add(word, translations);
            }
        }
        public void PrintOut()
        {
            var temp = dictionary.Keys.ToArray();
            if (temp.Count() == 0)
            {
                GlobalMethods.ShowErrorMessage("Dictionary is empty");
                return;
            }
            Console.Clear();
            Console.WriteLine("Words\t--\t--\t--\t--\tTranslations");
            int pos = 1;
            foreach (var item in temp)
            {
                Console.SetCursorPosition(0, pos);
                Console.Write($"{item}");
                Console.SetCursorPosition(40, pos++);
                foreach (var words in dictionary[item])
                {
                    Console.Write($"{words},");
                }
                Console.WriteLine();
            }
        }
        public List<string> TryGetTranslations(string word)
        {
            if (dictionary.ContainsKey(word))
            {
                return dictionary[word];
            }
            return null;
        }
    }


    //Manages the list of dictionaries
    static class Programm
    {
    static Dictionary<string, MyDictionary> dictionaryList = new();
        static void AddDictionary()
        {
            Console.Clear();
            List<string> languages = new();
            foreach (var item in Enum.GetValues(typeof(Languages)))
            {
                languages.Add(item.ToString());
            }
            Console.SetCursorPosition(30, 8);
            Console.WriteLine("From: ");
            int from = Menu.ConsoleMenu.SelectVertical(HPosition.Center, VPosition.Center, HorizontalAlignment.Center,languages);
            Console.Clear();
            Console.SetCursorPosition(30, 8);
            Console.WriteLine("To: ");
            int to = Menu.ConsoleMenu.SelectVertical(HPosition.Center, VPosition.Center, HorizontalAlignment.Center, languages);
            Console.Clear();
            if (from == to)
            {
                Console.SetCursorPosition(25, 10);
                Console.WriteLine("There is no point in translating a language into itself.Returning to menu.");
                Console.Read();
                return;
            }
            if (!dictionaryList.TryAdd($"{(Languages)from}-{(Languages)to}", new MyDictionary()))
            {
                GlobalMethods.ShowErrorMessage("Such dictionary already exists");
                return;
            }
        }
        static void RemoveDictionary()
        {
        Console.Clear();
        List<string> Dictionaries = dictionaryList.Keys.ToList();
        if (Dictionaries.Count == 0)
            {
                GlobalMethods.ShowErrorMessage("No dictionaries");
                return;
            }
        Dictionaries.Add("Exit");
        int c = Menu.ConsoleMenu.SelectVertical(HPosition.Center, VPosition.Center, HorizontalAlignment.Center, Dictionaries);
        Console.Clear();
        if (c == Dictionaries.Count - 1)
        {
            return;
        }
        dictionaryList.Remove(Dictionaries.ElementAt(c));
        }
        static void ChangeDictionary()
        {
            Console.Clear();
            List<string> dictionaries = dictionaryList.Keys.ToList();
            if (dictionaries.Count == 0)
            {
                GlobalMethods.ShowErrorMessage("No dictionaries to change");
                return;
            }
            Console.SetCursorPosition(10, 5);
            Console.WriteLine("Dictionary to work with: ");
            MyDictionary workingDictionary = dictionaryList[dictionaries.ElementAt(GlobalMethods.BuildCenteredMenu(dictionaries))];
            Console.Clear();
            List<string> options = new List<string> {"Add word","Remove Word","Change word","Add translation","Remove translation","Change Translation","Clear Dictionary","View words in the dictionary","Exit"};
            while (true)
            {
                Console.Clear();
                int c = GlobalMethods.BuildCenteredMenu(options);
                Console.Clear();
                if (c == options.Count - 1)
                {
                    return;
                }
                switch (c)
                {
                    case 0:
                        Console.SetCursorPosition(20, 10);
                        Console.Write("Word to add: ");
                        string temp = Console.ReadLine();
                        workingDictionary.AddWord(temp);
                        break;
                    case 1:
                        workingDictionary.RemoveWord();
                        break;
                    case 2:
                        workingDictionary.ChangeWord();
                        break;
                    case 3:
                        workingDictionary.AddTranslation(workingDictionary.ChooseWord());
                        break;
                    case 4:
                        workingDictionary.RemoveTranslation();
                        break;
                    case 5:
                        workingDictionary.ChangeTranslation();
                        break;
                    case 6:
                        workingDictionary.Clear();
                        break;
                    case 7:
                        Console.Clear();
                        workingDictionary.PrintOut();
                        Console.Read();
                        break;
                    default:
                        break;
                }
            }
        }
        static void Load()
        {
            if (!(Directory.Exists("Dictionaries")))
            {
                GlobalMethods.ShowErrorMessage("Directory was not found");
                return;
            }
            List<string> files= Directory.EnumerateFiles("Dictionaries\\").ToList();
            if (files.Count == 0)
            {
                GlobalMethods.ShowErrorMessage("Directory was empty");
                return;
            }
            if(dictionaryList.Count != 0)dictionaryList.Clear();
            foreach (var dict in files)
            {
                MyDictionary newDict = new();
                using (Stream stream = File.Open(dict, FileMode.Open))
                {
                    using (StreamReader sr = new StreamReader(stream))
                    {
                        newDict.Load(sr);
                    }
                }
                string temp = dict.Substring(dict.LastIndexOf('\\') + 1);
                dictionaryList.Add(temp.Substring(0,temp.Length - 4), newDict);
            }
        }
        static void Save()
        {
            if (dictionaryList.Count == 0)
            {
                GlobalMethods.ShowErrorMessage("List of dictionaries is empty.");
                return;
            }
            Directory.CreateDirectory("Dictionaries");
            foreach (string dict in dictionaryList.Keys.ToArray())
            {
                Console.WriteLine(dict);
                using (Stream stream = File.Create($"Dictionaries\\{dict}.txt"))
                {
                    using (StreamWriter sw = new StreamWriter(stream))
                    {
                        dictionaryList[dict].Save(sw);
                    }
                }
            }
        }
        static void SearchTranslations()
        {
            var list = dictionaryList.Keys.ToList();
            if (list.Count == 0)
            {
                GlobalMethods.ShowErrorMessage("List of dictionaries is empty.");
                return;
            }
            Console.Clear();
            Console.SetCursorPosition(20, 10);
            Console.Write("The word to search translations for: ");
            string word = Console.ReadLine();
            Console.Clear();
            bool wasFound = false;
            for (int i = 0; i < list.Count; i++)
            {
                var translations = dictionaryList[list[i]].TryGetTranslations(word);
                if (translations != null)
                {
                    Console.Write($"Translations for word {word} in {list[i]}: ");
                    foreach (var item in translations)
                    {
                        Console.Write($"{item}, ");
                    }
                    Console.WriteLine();
                    wasFound = true;
                }
            }
            if (!wasFound)
            {
                GlobalMethods.ShowErrorMessage("No such word was found");
                return;
            }
            Console.Read();
        }
        static void SingleWord()
        {
            var list = dictionaryList.Keys.ToList();
            if (list.Count == 0)
            {
                GlobalMethods.ShowErrorMessage("List of dictionaries is empty.");
                return;
            }
            Console.Clear();
            Console.SetCursorPosition(20, 10);
            Console.Write("The word to search for: ");
            string word = Console.ReadLine();
            Console.Clear();
            string res = "";
            for (int i = 0; i < list.Count; i++)
            {
                var translations = dictionaryList[list[i]].TryGetTranslations(word);
                if (translations != null)
                {
                    res += $"Translations for word {word} in {list[i]}: ";
                    foreach (var item in translations)
                    {
                        res+=$"{item}, ";
                    }
                    res += "\n";
                }
            }
            if (res == "")
            {
                GlobalMethods.ShowErrorMessage("No such word was found");
                return;
            }
            if (!Directory.Exists("Singles"))
            {
                Directory.CreateDirectory("Singles");
            }
            using (Stream stream = File.Create($"Singles\\{word}.txt"))
            {
                using (StreamWriter sw = new StreamWriter(stream))
                {
                    sw.Write(res);
                }
            }
        }
        static public void Main()
        {
            try
            {
                string[] options = new string[] { "Add a dictionary", "Remove a dictionary", "Work with a dictionary","Search for translation","Load from files", "Save to files","Get a single word to a unique file","Exit" };
                while (true)
                {
                    Console.Clear();
                    int c = Menu.ConsoleMenu.SelectVertical(HPosition.Left,VPosition.Center,HorizontalAlignment.Right,options);
                    switch (c)
                    {
                        case 0:
                            AddDictionary();
                            break;
                        case 1:
                            RemoveDictionary();
                            break;
                        case 2:
                            ChangeDictionary();
                            break;
                        case 3:
                            SearchTranslations();
                            break;
                        case 4:
                            Load();
                            break;
                        case 5:
                            Save();
                            break;
                        case 6:
                            SingleWord();
                            break;
                        case 7:
                            //Exiting
                            return;
                        default:
                            break;
                    }
                }
            }
            finally
            {
                Console.Clear();
                Console.SetCursorPosition(20, 20);
                Console.WriteLine("Exiting the programm...Goodbye!");
                Console.SetCursorPosition(10, 21);
            }
        }
    }
}