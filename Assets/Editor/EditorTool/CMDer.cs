using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EditorTool
{
    public class CMDer
    {
        public static string RunCMD(string path,string cmd)
        {
            Process process = new Process();
            process.StartInfo.FileName = "cmd.exe";
            process.StartInfo.CreateNoWindow=true;

            process.StartInfo.UseShellExecute=false;
            process.StartInfo.RedirectStandardInput = true;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.RedirectStandardError = true;

           // process.StartInfo.StandardErrorEncoding = System.Text.Encoding.UTF8;
           // process.StartInfo.StandardOutputEncoding = System.Text.Encoding.UTF8;

            process.StartInfo.WorkingDirectory = path;
            process.Start();
            process.StandardInput.WriteLine(cmd);
            process.StandardInput.AutoFlush = true;
            process.StandardInput.WriteLine("exit");

            var output=process.StandardOutput.ReadToEnd();
            process.WaitForExit();
            process.Close();
            return output;

        }


        public static bool Run(string path, string cmd)
        {
            Process process = new Process();

            try
            {
                process.StartInfo.FileName = @"cmd.exe";
                process.StartInfo.CreateNoWindow = true;

                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardInput = true;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;

                // process.StartInfo.StandardErrorEncoding = System.Text.Encoding.UTF8;
                // process.StartInfo.StandardOutputEncoding = System.Text.Encoding.UTF8;

                process.StartInfo.WorkingDirectory = path;
                process.Start();
                process.StandardInput.WriteLine(cmd);
                process.StandardInput.AutoFlush = true;
                process.StandardInput.WriteLine("exit");

                var output = process.StandardOutput.ReadToEnd();
                process.WaitForExit();
                var error = process.StandardError.ReadToEnd();
                if (process.ExitCode != 0 || error != "")
                {
                    return false;
                }
            }
            finally
            {
                process.Close();
            }
         
            return true;

        }
    }
}
