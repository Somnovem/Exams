using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace ActivityMonitor
{
    internal class KeyMonitor
    {
        private GlobalKeyboardHook keyboardHook;
        public delegate void KeyPressedDelegaate(string key);
        public event KeyPressedDelegaate KeyPressed;

        public delegate void BannedWordEnteredDelegaate(string word);
        public event BannedWordEnteredDelegaate BannedWordEntered;

        private string[] bannedWords;
        private bool noBannedWords = false;
        private StringBuilder wordBuilder;

        public KeyMonitor(string[] bannedWords)
        {
            if (bannedWords.Length == 0) noBannedWords = true;
            else
            {
                this.bannedWords = new string[bannedWords.Length];
                for (int i = 0; i < bannedWords.Length; i++)
                {
                    this.bannedWords[i] = bannedWords[i].ToUpper();
                }
            }
        }
        public void StartMonitoring()
        {
            keyboardHook = new GlobalKeyboardHook();
            keyboardHook.KeyDown += KeyboardHook_KeyDown;
            if (!noBannedWords) wordBuilder = new StringBuilder();
        }

        private void KeyboardHook_KeyDown(object? sender, Keys e)
        {
            string keyPressed = e.ToString();
            if (!noBannedWords)
            {
                if (keyPressed.Length != 1)
                {
                    if (bannedWords.Contains(wordBuilder.ToString())) BannedWordEntered?.Invoke(wordBuilder.ToString());
                    wordBuilder.Clear();
                }
                else wordBuilder.Append(keyPressed);
            }
            KeyPressed?.Invoke(keyPressed);
        }

        public void Close()
        {
            keyboardHook.Dispose();
        }
    }
}
