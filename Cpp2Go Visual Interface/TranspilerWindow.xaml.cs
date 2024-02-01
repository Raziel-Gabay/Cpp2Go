using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

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

    }
}
