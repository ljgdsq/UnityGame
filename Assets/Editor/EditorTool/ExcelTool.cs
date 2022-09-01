using System.Collections.Generic;
using System.IO;
using NPOI.HSSF.UserModel;
using UnityEditor;
using UnityEngine;

namespace EditorTool
{
    public class ExcelTool
    {
        [MenuItem("Tools/ReadExcel")]
        public static void ReadExcel()
        {
            var file = Path.Combine(Application.dataPath, "Editor/Sheet/Dialog.xls");  
            using (FileStream fileStream = new FileStream(file, FileMode.Open))

            {
                HSSFWorkbook workbook = new HSSFWorkbook(fileStream);
                var sheet = workbook.GetSheetAt(0);
                var titleLine = sheet.GetRow(0);//comment
                var typeLine = sheet.GetRow(1);//type
                //  var line3 = sheet.GetRow(2);//


                Dictionary<string, Dictionary<string, string>> LanguageTasble = new Dictionary<string, Dictionary<string, string>>();


                List<(string, int)> localIndex = new List<(string, int)>();


                //cells 0 is key
                int startLine = 1;
                int startCol = 2;

                int i = startLine;
                for (; i < titleLine.Cells.Count; i++)
                {
                    var local = titleLine.Cells[i].StringCellValue;
                    if (!string.IsNullOrEmpty(local))
                    {
                        localIndex.Add((local, i));
                        LanguageTasble.Add(local, new Dictionary<string, string>());
                    }
                }

                i = startCol;
                for (; i <= sheet.LastRowNum; i++)
                {
                    var row = sheet.GetRow(i);
                    var key = row.Cells[0].StringCellValue;

                    foreach (var localPair in localIndex)
                    {
                        var localText = row.Cells[localPair.Item2].StringCellValue;
                        LanguageTasble[localPair.Item1].Add(key, localText);

                    }
                }

                workbook.Close();
            }
        }
    }
}