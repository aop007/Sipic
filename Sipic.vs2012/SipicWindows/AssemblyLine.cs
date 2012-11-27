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
        
        public AssemblyLine(string readLine, int line, int asm_type)
        {
            this.text = readLine;
            this.lineNumber = line;

            type = GetAsmType(text, asm_type);

            if (type == AssemblyLineType.Code) {
                if (asm_type == 0)
                {
                    string addr_string = text.Substring(0, 4);
                    directive = text.Substring(7);
                    addr = int.Parse(addr_string, System.Globalization.NumberStyles.HexNumber);
                }
                else {
                    string addr_string = text.Substring(2, 4);
                    directive = text.Substring(19);
                    try {
                        addr = int.Parse(addr_string, System.Globalization.NumberStyles.HexNumber);
                    } catch { }
                }
            }
        }

        public static AssemblyLineType GetAsmType(string text, int asm_type)
        {
            if (asm_type == 0)
            {
                int comaIdx = text.IndexOf(':');

                if (comaIdx == 4)
                {
                    return AssemblyLineType.Code;
                }
                else
                {
                    return AssemblyLineType.Comment;
                }
            }
            else if (asm_type == 1)
            {
                if (text.Length <= 1) {
                    return AssemblyLineType.Ignore;
                }
                else if ((text[0] == ' ') && (text[1] == ' '))
                {
                    return AssemblyLineType.Code;
                }
                else
                {
                    return AssemblyLineType.Comment;
                }
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
        Code,
        Ignore
    }
}
