using System;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Windows.Forms;

namespace ActivityMonitor
{

    class GlobalKeyboardHook
    {
        #region DLLs

        [DllImport("user32.dll")]
        private static extern IntPtr SetWindowsHookEx(int hookType, KeyboardProc callback, IntPtr hMod, uint dwThreadId);

        [DllImport("user32.dll")]
        private static extern bool UnhookWindowsHookEx(IntPtr hhk);

        [DllImport("user32.dll")]
        private static extern IntPtr CallNextHookEx(IntPtr hhk, int nCode, IntPtr wParam, IntPtr lParam);

        [DllImport("kernel32.dll")]
        private static extern IntPtr GetModuleHandle(string lpModuleName);

        private const int WH_KEYBOARD_LL = 13;
        private const int WM_KEYDOWN = 0x0100;

        #endregion

        private delegate IntPtr KeyboardProc(int nCode, IntPtr wParam, IntPtr lParam);

        public event EventHandler<Keys> KeyDown;

        private IntPtr KeyboardHookId = IntPtr.Zero;

        private KeyboardProc KeyboardProcDelegate = null;

        public GlobalKeyboardHook()
        {
            InstallHook();
        }

        // Destructor
        ~GlobalKeyboardHook()
        {
            Dispose(disposing:false);
        }

        private void InstallHook()
        {
            IntPtr hModule = GetModuleHandle(Process.GetCurrentProcess().MainModule.ModuleName);

            KeyboardProcDelegate = new KeyboardProc(KeyboardProcCallback);
            KeyboardHookId = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProcDelegate, hModule, 0);

            if (KeyboardHookId == IntPtr.Zero)
            {
                throw new Exception("Failed to install keyboard hook");
            }
        }

        private void UninstallHook()
        {
            if (KeyboardHookId != IntPtr.Zero)
            {
                UnhookWindowsHookEx(KeyboardHookId);
                KeyboardHookId = IntPtr.Zero;
            }

            if (KeyboardProcDelegate != null)
            {
                KeyboardProcDelegate = null;
            }
        }

        private IntPtr KeyboardProcCallback(int nCode, IntPtr wParam, IntPtr lParam)
        {
            if (nCode >= 0 && wParam == (IntPtr)WM_KEYDOWN)
            {
                int keyCode = Marshal.ReadInt32(lParam);

                KeyDown?.Invoke(this, (Keys)keyCode);
            }

            return CallNextHookEx(IntPtr.Zero, nCode, wParam, lParam);
        }


        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }


        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {

            }
            UninstallHook();
        }
    }
}

