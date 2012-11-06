using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SipicWindows
{
    class AssemblyLine
    {
        private string           text;
        private AssemblyLineType type;
        private int              lineNumber;
        private string           directive;
        private int              addr;
        
        public AssemblyLine(string readLine, int line)
        {
            this.text = readLine;
            this.lineNumber = line;

            type = GetAsmType(text);

            if (type == AssemblyLineType.Code) {
                string addr_string = text.Substring(0, 4);
                directive = text.Substring(7);
                addr = int.Parse(addr_string, System.Globalization.NumberStyles.HexNumber);
            }
        }
    
        public static AssemblyLineType GetAsmType(string text)
        {
            int comaIdx = text.IndexOf(':');

            if (comaIdx == 4)
            {
                return AssemblyLineType.Code;
            }
            else {
                return AssemblyLineType.Comment;
            }
        }


        public int Addr              { get { return addr; } }
        public string Text           { get { return text; } }
        public AssemblyLineType Type { get { return type; } }
        public int Line              { get { return lineNumber; } }
        
    }


    public enum AssemblyLineType{
        Comment,
        Code
    }
}
