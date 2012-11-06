using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;

namespace SipicWindows
{
    public partial class Form1 : Form
    {
        [DllImport(@"C:\Users\alexis01.micrium01\Projects\Sipic\Sipic.vs2012\Debug\Sipic.dll", EntryPoint = "init_sipic")]
        public static extern void init();

        [DllImport(@"C:\Users\alexis01.micrium01\Projects\Sipic\Sipic.vs2012\Debug\Sipic.dll", EntryPoint = "Sim_Step")]
        public static extern void sim_step();

        [DllImport(@"C:\Users\alexis01.micrium01\Projects\Sipic\Sipic.vs2012\Debug\Sipic.dll", EntryPoint = "Sim_GetValueFromDataMem")]
        public static extern UInt16 Sim_GetValueFromDataMem(UInt16 addr);

        //unsigned int  __declspec(dllexport) __stdcall Sim_GetOPCFromProgMem(unsigned int addr)
        [DllImport(@"C:\Users\alexis01.micrium01\Projects\Sipic\Sipic.vs2012\Debug\Sipic.dll", EntryPoint = "Sim_GetOPCFromProgMem")]
        public static extern UInt32 Sim_GetOPCFromProgMem(UInt32 addr);

        const UInt16 CORE_SR_C    = 0x0001;
        const UInt16 CORE_SR_Z    = 0x0002;
        const UInt16 CORE_SR_OV   = 0x0004;
        const UInt16 CORE_SR_N    = 0x0008;
        const UInt16 CORE_SR_RA   = 0x0010;
        const UInt16 CORE_SR_IPL0 = 0x0020;
        const UInt16 CORE_SR_IPL1 = 0x0040;
        const UInt16 CORE_SR_IPL2 = 0x0080;
        const UInt16 CORE_SR_DC   = 0x0100;
        const UInt16 CORE_SR_DA   = 0x0200;
        const UInt16 CORE_SR_SAB  = 0x0400;
        const UInt16 CORE_SR_OAB  = 0x0800;
        const UInt16 CORE_SR_SB   = 0x1000;
        const UInt16 CORE_SR_SA   = 0x2000;
        const UInt16 CORE_SR_OB   = 0x4000;
        const UInt16 CORE_SR_OA   = 0x8000;


        MemoryBlock testBlock1;
        Thread runThread;
        AssemblyLine[] assemblyCode;
        private Dictionary<int, AssemblyLine> addrMap = new Dictionary<int, AssemblyLine>();
        bool exception = false;


        delegate void SetIntCallback(int val);


        public Form1()
        {
            InitializeComponent();
            init();

           
            InitMemoryTable();

            RefreshMemoryTable(false);

            InitCodeTable();
#if false
            for (int i = 0; i < 100; i++)
            {
                sim_step();
                testBlock1.Update();
            }
#endif


        }

        public void SetPCInvoke(int val)
        {
            if (this.numPC.InvokeRequired) {
                SetIntCallback d = new SetIntCallback(SetPCInvoke);
                try {
                    this.BeginInvoke(d, new object[] { val });
                } catch { }
            } else {
                numPC.Value = val;
            }
        }

        public void SetOPCInvoke(int val)
        {
            if (this.numOPC.InvokeRequired)
            {
                SetIntCallback d = new SetIntCallback(SetOPCInvoke);
                try
                {
                    this.BeginInvoke(d, new object[] { val });
                }
                catch { }
            }
            else
            {
                numOPC.Value = val;
            }
        }

        public void SetCodeLineInvoke(int val)
        {
            if (this.codeGridView.InvokeRequired)
            {
                SetIntCallback d = new SetIntCallback(SetCodeLineInvoke);
                try
                {
                    this.BeginInvoke(d, new object[] { val });
                }
                catch { }
            }
            else
            {
                int lastDisplayedScrollLine = codeGridView.FirstDisplayedScrollingRowIndex;
                codeGridView.FirstDisplayedScrollingRowIndex = val;

                codeGridView.Rows[lastDisplayedScrollLine].Cells[0].Style.BackColor = Color.White;
                codeGridView.Rows[lastDisplayedScrollLine].Cells[0].Style.ForeColor = Color.Black;

                codeGridView.Rows[val].Cells[0].Style.BackColor = Color.Black;
                codeGridView.Rows[val].Cells[0].Style.ForeColor = Color.Yellow;

            }
        }

        private void InitMemoryTable()
        {
            testBlock1 = new MemoryBlock(0x0000, 0x0C00);

            for (int i = 0; i < testBlock1.Word_Size; i++)
            {
                this.memoryView1.Rows.Add(testBlock1.GetRowFromOffset(i));
                this.memoryView2.Rows.Add(testBlock1.GetRowFromOffset(i));
            }

            testBlock1.Update(false);
        }

        private void LowLight(DataGridViewRow dgvr)
        {
            DataGridViewCellCollection dgvcc = dgvr.Cells;

            foreach (DataGridViewCell dgvc in dgvcc)
            {
                dgvc.Style.ForeColor = Color.Black;
                dgvc.Style.BackColor = Color.White;
            }
        }

        private void HighLight(DataGridViewRow dgvr)
        {
            DataGridViewCellCollection dgvcc = dgvr.Cells;

            foreach (DataGridViewCell dgvc in dgvcc)
            {
                dgvc.Style.ForeColor = Color.Green;
                dgvc.Style.BackColor = Color.Black;
            }
        }

        private void RefreshMemoryTable(bool silent)
        {
            if (!silent)
            {
                ushort stackPtr = testBlock1.GetMemoryAtAddr(30);

                for (int i = 0; i < testBlock1.Word_Size; i++)
                {
                    if (testBlock1.MemoryHasChangedAtOffset(i) || !silent)
                    {
                        DataGridViewCellCollection dgvrc = this.memoryView1.Rows[i].Cells;
                        dgvrc[1].Value = ((testBlock1.GetRowFromOffset(i))[1]);

                        dgvrc = this.memoryView2.Rows[i].Cells;
                        dgvrc[1].Value = ((testBlock1.GetRowFromOffset(i))[1]);
                    }

                    LowLight(this.memoryView1.Rows[i]);
                    LowLight(this.memoryView2.Rows[i]);
                }


                HighLight(this.memoryView1.Rows[stackPtr / 2]);
                HighLight(this.memoryView2.Rows[stackPtr / 2]);
            }
        }

        private int GetPC()
        {
            Int32 PCL = testBlock1.GetMemoryAtAddr(46);
            Int32 PCH = (testBlock1.GetMemoryAtAddr(48) & 0x00FF) << 16;
            Int32 PC = (PCH | PCL);

            return PC;
        }

        private int GetOPC()
        {
             int PC  = GetPC();
             uint OPC = Sim_GetOPCFromProgMem((uint)PC);

            return (int)OPC;
        }

        private void UpdatePC(bool silent)
        {
            if (!silent)
            {
                SetPCInvoke(GetPC());
            }
        }

        private void UpdateOPC(bool silent)
        {
            if (!silent)
            {
                SetOPCInvoke(GetOPC());
            }
        }

        private void IteratePC(bool silent)
        {
            testBlock1.Flush(silent);
            sim_step();
            SynchonizeUI(silent);
        }

        private void UpdateSRBar(bool silent)
        {
            if (!silent) {
                List<string> statusSRList = new List<string>();
                UInt16 SR = Sim_GetValueFromDataMem(0x42);
                

                if ((SR & CORE_SR_OA) == CORE_SR_OA) { statusSRList.Add("OA"); }
                if ((SR & CORE_SR_OB) == CORE_SR_OB) { statusSRList.Add("OB"); }
                if ((SR & CORE_SR_SA) == CORE_SR_SA) { statusSRList.Add("SA"); }
                if ((SR & CORE_SR_SB) == CORE_SR_SB) { statusSRList.Add("SB"); }
                if ((SR & CORE_SR_OAB) == CORE_SR_OAB) { statusSRList.Add("OAB"); }
                if ((SR & CORE_SR_SAB) == CORE_SR_SAB) { statusSRList.Add("SAB"); }
                if ((SR & CORE_SR_DA) == CORE_SR_DA) { statusSRList.Add("DA"); }
                if ((SR & CORE_SR_DC) == CORE_SR_DC) { statusSRList.Add("DC"); }
                if ((SR & CORE_SR_IPL2) == CORE_SR_IPL2) { statusSRList.Add("IPL2"); }
                if ((SR & CORE_SR_IPL1) == CORE_SR_IPL1) { statusSRList.Add("IPL1"); }
                if ((SR & CORE_SR_IPL0) == CORE_SR_IPL0) { statusSRList.Add("IPL0"); }
                if ((SR & CORE_SR_RA) == CORE_SR_RA) { statusSRList.Add("RA"); }
                if ((SR & CORE_SR_N) == CORE_SR_N) { statusSRList.Add("N"); }
                if ((SR & CORE_SR_OV) == CORE_SR_OV) { statusSRList.Add("OV"); }
                if ((SR & CORE_SR_Z) == CORE_SR_Z) { statusSRList.Add("Z"); }
                if ((SR & CORE_SR_C) == CORE_SR_C) { statusSRList.Add("C"); }

                string[] statusSRArray = statusSRList.ToArray();
                string statusSR = string.Join(" | ", statusSRArray);

                StatusRegisterLabel.Text = statusSR;
            }
        }
        private void SynchonizeUI(bool silent)
        {
            testBlock1.Update(silent);
            RefreshMemoryTable(silent);
            UpdatePC(silent);
            UpdateOPC(silent);
            UpdateCodeView(silent);
            UpdateSRBar(silent);
        }

        private void UpdateCodeView(bool silent)
        {
            if (!silent)
            {
                AssemblyLine al = null;
                try
                {
                    al = addrMap[GetPC()];
                }
                catch
                {
                    Console.WriteLine("Addr " + GetPC() + " is Unavailable");
                    //exception = true;
                }
                if (al != null)
                {
                    SetCodeLineInvoke(al.Line);
                }
                else
                {
                    SetCodeLineInvoke(0);
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            IteratePC(false);
        }

        private void Run_Click(object sender, EventArgs e)
        {
            if (this.Run.Text == "Run")
            {
                exception     = false;
                this.Run.Text = "Pause";
                runThread = new Thread(new ThreadStart(RunPC));
                runThread.Start();
            }
            else {
                exception = true;
                runThread.Join();
                this.Run.Text = "Run";
            }
        }

        private bool BreakPointNotMet()
        { 
            if ((this.checkBox1.Checked) && (GetPC() == BPupdown.Value)) {
                return false;
            }

            if (this.condCheck.Checked) {
                UInt32 addr  = (UInt32)this.condAddr.Value;
                UInt16 mask  = (UInt16)this.condMask.Value;
                UInt16 value = (UInt16)this.condValue.Value;
                UInt16 memory = (UInt16)Sim_GetValueFromDataMem((UInt16)addr);

                if ((memory & mask) == (value & mask)) {
                    return false;
                }

            }

            return true;
        }

        private void RunPC()
        {
            while (BreakPointNotMet() && (exception == false)) {
                IteratePC(true);
                //Thread.Sleep(1);
            }
            SynchonizeUI(false);
        }

        private void InitCodeTable()
        { 
            StreamReader sr = new StreamReader(@"C:\Users\alexis01.micrium01\Projects\Sipic\Sipic\InputFiles\main.lst");
            string readLine = sr.ReadLine();
            List<AssemblyLine> assemblyFile = new List<AssemblyLine>();
            int idx = 0;


            this.codeGridView.ReadOnly = true;
            this.codeGridView.RowHeadersVisible = false;
            this.codeGridView.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.codeGridView.ShowCellErrors = false;
            this.codeGridView.ShowCellToolTips = false;
            this.codeGridView.ShowEditingIcon = false;

            DataGridViewTextBoxColumn dgvtbc = new DataGridViewTextBoxColumn();
            dgvtbc.SortMode = DataGridViewColumnSortMode.NotSortable;
            dgvtbc.DefaultCellStyle.Font = new Font("Courier New", 9);

            codeGridView.Columns.Add(dgvtbc);
            

            while(readLine != null) {
                AssemblyLine al = new AssemblyLine(readLine, idx);

                if (al.Type == AssemblyLineType.Code) {
                    addrMap.Add(al.Addr, al);
                }

                assemblyFile.Add(al);
                
               
                codeGridView.Rows.Add(new object[] { al.Text });
                


                readLine = sr.ReadLine();
                idx++;
            }

            for (int i = 0; i < codeGridView.Rows.Count; i++)
            {
                codeGridView.Rows[i].Cells[0].Style.Font = new Font(dgvtbc.DefaultCellStyle.Font, FontStyle.Regular);
                codeGridView.Rows[i].Height = 18;
                codeGridView.Rows[i].Resizable = DataGridViewTriState.False;
            }

            dgvtbc.AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCells;
            codeGridView.CellBorderStyle = DataGridViewCellBorderStyle.None;

            assemblyCode = assemblyFile.ToArray();
        }
    }
}

public class MemoryBlock
{
    [DllImport(@"C:\Users\alexis01.micrium01\Projects\Sipic\Sipic.vs2012\Debug\Sipic.dll", EntryPoint = "init")]
    public static extern void init();

    [DllImport(@"C:\Users\alexis01.micrium01\Projects\Sipic\Sipic.vs2012\Debug\Sipic.dll", EntryPoint = "Sim_Step")]
    public static extern void sim_step();

    [DllImport(@"C:\Users\alexis01.micrium01\Projects\Sipic\Sipic.vs2012\Debug\Sipic.dll", EntryPoint = "Sim_GetValueFromDataMem")]
    public static extern ushort Sim_GetValueFromDataMem(ushort addr);


    private int start;
    private int word_size;
    UInt16[] mem;
    UInt16[] mem_shadow;

    public MemoryBlock(int start, int byte_size)
    {
        this.start = start;
        this.word_size = byte_size / 2;

        this.mem        = new ushort[word_size];
        this.mem_shadow = new ushort[word_size];
    }

    public void Update(bool silent)
    {
        if (silent)
        {
            mem[46/2] = Sim_GetValueFromDataMem((UInt16)(46 + start));
            mem[48/2] = Sim_GetValueFromDataMem((UInt16)(48 + start));
        }
        else
        {
            for (int i = 0; i < word_size; i++)
            {
                mem[i] = Sim_GetValueFromDataMem((UInt16)(i * 2 + start));
            }
        }
    }

    public void Flush(bool silent)
    {
        if (!silent)
        {
            for (int i = 0; i < word_size; i++)
            {
                mem[i] = mem_shadow[i];
            }
        }
    }

    public int Word_Size
    {
        get { return word_size; }
    }

    public object[] GetRowFromOffset(int offset)
    {
        List<object> row = new List<object>();
        string addrHex  = (offset * 2 + start).ToString("X");
        string valueHex = (mem[offset]).ToString("X");

        row.Add(addrHex);
        row.Add(valueHex);

        return (row.ToArray());
    }

    public bool MemoryHasChangedAtOffset(int i)
    {
        return (mem[i] != mem_shadow[i]);
    }

    public UInt16 GetMemoryAtAddr(int addr)
    {
        return (mem[addr / 2]);
    }

    public UInt16 GetShadowAtAddr(int addr)
    {
        return (mem_shadow[addr / 2]);
    }

}
