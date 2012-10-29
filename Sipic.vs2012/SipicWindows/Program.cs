using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace SipicWindows
{
    static class Program
    {

        [DllImport(@"C:\Users\alexis01.micrium01\Projects\Sipic\Sipic.vs2012\Debug\Sipic.dll", EntryPoint = "init")]
        public static extern void init();

        [DllImport(@"C:\Users\alexis01.micrium01\Projects\Sipic\Sipic.vs2012\Debug\Sipic.dll", EntryPoint = "run")]
        public static extern void run();

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
#if true
            init();
            run();
#endif
            
            Application.Run(new Form1());
        }
    }

#if false
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct SIM
    {
        public long ulClientRC;
        public long ulClientReas;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 3)]
        public string HostRC;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 5)]
        public string HostReas;

    }
#endif
}
