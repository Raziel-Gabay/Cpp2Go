using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Diagnostics;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Collections;
using System.Reflection.Emit;
using System.IO;
using static System.Net.Mime.MediaTypeNames;
using System.Xml.Linq;

namespace Cpp2Go_Visual_Interface
{
    /// <summary>
    /// Interaction logic for TranspilerWindow.xaml
    /// </summary>
    public partial class TranspilerWindow : Window
    {
        public TranspilerWindow()
        {
            InitializeComponent();
        }

        private void TranslateClick(object sender, RoutedEventArgs e)
        {
            SaveToFile(CppCode.Text);
            codeTranslator();
        }
        private void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox textBox = (TextBox)sender;
            if (textBox.Text == "Enter your text here...")
            {
                textBox.Text = string.Empty;
            }
        }

        private void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            TextBox textBox = (TextBox)sender;
            if (string.IsNullOrWhiteSpace(textBox.Text))
            {
                textBox.Text = "Enter your text here...";
            }
        }
        private void SaveToFile(string text)
        {
            string filePath = @"cppCode.txt";
            try
            {
                if (text != "Enter your text here..." || !string.IsNullOrWhiteSpace(text))
                {
                    using (StreamWriter writer = new StreamWriter(filePath))
                    {
                        writer.Write(text);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error saving text to file: {ex.Message}");
            }
        }
        private void codeTranslator()
        {
            // Path to the executable file
            string exePath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Cpp2Go.exe");

            // Create a new process start info
            ProcessStartInfo startInfo = new ProcessStartInfo
            {
                FileName = exePath,
                UseShellExecute = false, // Set to false to redirect the standard output
                RedirectStandardOutput = true,  // Set to true to capture the standard output
                RedirectStandardError = true
            };

            // Start the process
            Process process = Process.Start(startInfo);

            // Read the standard output
            string output = process.StandardOutput.ReadToEnd();
            string error = process.StandardError.ReadToEnd();

            // Wait for the process to exit
            process.WaitForExit();

            if (error.Count() > 0)
            {
                GoCode.Text = error;
                GoCode.Foreground = Brushes.Red;
            }
            else
            {
                GoCode.Text = output;
                GoCode.Foreground = Brushes.White;
            }
        }

        private void Back(object sender, RoutedEventArgs e)
        {
            MainWindow objMainWindow = new MainWindow();
            this.Visibility = Visibility.Hidden;
            objMainWindow.Show();
        }
    }
}
