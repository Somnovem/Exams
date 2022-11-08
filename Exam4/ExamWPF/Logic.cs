using Menu;
using System;
using System.Data;
using System.IO;
using System.Net;
using System.Net.Http.Headers;
using System.Windows.Controls;
using System.Windows;
using System.Xml;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Navigation;
using System.Reflection.Metadata.Ecma335;
using System.Collections.Specialized;
using System.Collections.ObjectModel;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;

namespace ExamWPF
{
    /// <summary>
    /// enum of accepted languages
    /// </summary>
    public enum Languages
    { UKRAINIAN = 0, ENGLISH, RUSSIAN, FRENCH, ITALIAN, SPANISH, GERMAN, POLISH }
    //Ukrainian has a value of zero, but is still the top 1.

    //Data Class that is the dictionary itself
    public class MyDictionary : IEnumerable
    {
        public Dictionary<string, List<string>> dictionary = new();
        IEnumerator IEnumerable.GetEnumerator()
        {
            return ((IEnumerable)dictionary).GetEnumerator();
        }
        /// <summary>
        /// Performs the save of this dictionary to the stream
        /// </summary>
        /// <param name="sw">stream to which to save</param>
        public void Save(StreamWriter sw)
        {
            foreach (string word in dictionary.Keys.ToArray())
            {
                sw.Write(word);
                sw.Write(":");
                foreach (string translation in dictionary[word])
                {
                    sw.Write($"{translation},");
                }
                sw.Write('\n');
            }
        }
        /// <summary>
        /// Performs the load of this dictionary from the stream
        /// </summary>
        /// <param name="sw">stream from which to load</param>
        public void Load(StreamReader sr)
        {
            while (!sr.EndOfStream)
            {
                string temp = sr.ReadLine();
                string word = temp.Split(':', StringSplitOptions.RemoveEmptyEntries)[0];
                List<string> translations = temp.Split(':', StringSplitOptions.RemoveEmptyEntries)[1].Split(',', StringSplitOptions.RemoveEmptyEntries).ToList();
                dictionary.Add(word, translations);
            }
        }
        /// <summary>
        /// Seek translations of the specified word
        /// </summary>
        /// <param name="word">the word to search for</param>
        /// <returns>list of translations</returns>
        public List<string> TryGetTranslations(string word)
        {
            if (dictionary.ContainsKey(word))
            {
                return dictionary[word];
            }
            return null;
        }
        public List<string> GetTranslations()
        {
            return dictionary.Values.ElementAt(0);
        }
        /// <summary>
        /// Return pair of lists of the words and the translations for them
        /// </summary>
        /// <returns>pair of list<string></returns>
        public KeyValuePair<List<string>, List<string>> GetValues()
        {
            List<string> words = new();
            List<string> translations = new();
            foreach (var item in dictionary.Keys)
            {
                words.Add(item);
                string translation = "";
                foreach (var trln in dictionary[item])
                {
                    translation += trln + ", ";
                }
                translations.Add(translation);
            }
            return new KeyValuePair<List<string>, List<string>>(words,translations);
        }
    }


    //Manages the list of dictionaries

    static public class Programm
    {
        public static ObservableCollection<string> keys = new ObservableCollection<string>();
        public static ObservableCollection<MyDictionary> dictionaries = new ObservableCollection<MyDictionary>();
        public static int AddDictionary(string from,string to)
        {
            if (from.CompareTo(to) == 0)
            {
                return 0;
            }
            string working = $"{from}-{to}";
            if (keys.Contains(working))
            {
                return -1;
            }
            keys.Add(working);
            dictionaries.Add(new MyDictionary());
            return 1;
        }
        public static void RemoveDictionary(string dict)
        {
            int pos = keys.IndexOf(dict);
            keys.Remove(dict);
            dictionaries.RemoveAt(pos);
        }
        /// <summary>
        /// Initiates the loading from the folder
        /// </summary>
        public static void Load()
        {
            if (!(Directory.Exists("Dictionaries")))
            {
                
                return;
            }
            List<string> files = Directory.EnumerateFiles("Dictionaries\\").ToList();
            if (files.Count == 0)
            {
                return;
            }
            if (keys.Count != 0) { keys.Clear(); dictionaries.Clear(); }
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
                keys.Add(temp.Substring(0, temp.Length - 4));
                dictionaries.Add(newDict);
            }
        }
        /// <summary>
        /// Initiates the saving to the folder of each dictionary and removes the deleted ones
        /// </summary>
        public static void Save()
        {
            if (keys.Count == 0)
            {
                return;
            }
            Directory.CreateDirectory("Dictionaries");
            for (int i = 0; i < keys.Count; i++)
            {
                using (Stream stream = File.Create($"Dictionaries\\{keys[i]}.txt"))
                {
                    using (StreamWriter sw = new StreamWriter(stream))
                    {
                        dictionaries[i].Save(sw);
                    }
                }
            }
            var directories = Directory.GetFiles("Dictionaries");
            for (int i = 0; i < directories.Length; i++)
            {
                directories[i] = directories[i].Substring(directories[i].LastIndexOf('\\') + 1, directories[i].LastIndexOf('.') - (directories[i].LastIndexOf('\\') + 1));
            }
            foreach (var item in directories)
            {
                if (!keys.Contains(item))
                {
                    File.Delete($"Dictionaries\\{item}.txt");
                }
            }
        }
        /// <summary>
        /// Returns all posible translations found in all the dictionaries
        /// </summary>
        /// <param name="word">the word to search for</param>
        /// <returns>string with sections per each dictionary</returns>
        public static string SearchTranslations(string word)
        {
            string res = "";
            if (keys.Count == 0)
            {
                return "";
            }
            for (int i = 0; i < keys.Count; i++)
            {
                res += keys[i] + ": ";
                var translations = dictionaries[i].TryGetTranslations(word);
                if (translations != null)
                {
                    foreach (var item in translations)
                    {
                        res += $"{item}, ";
                    }
                    res += "\n";
                }
            }
            return res;
        }
    }
}
