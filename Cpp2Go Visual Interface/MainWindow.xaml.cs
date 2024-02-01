using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Cpp2Go_Visual_Interface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void OpenTranspilerWindow(object sender, RoutedEventArgs e)
        {
            TranspilerWindow objTranspilerWindow = new TranspilerWindow();
            this.Visibility = Visibility.Hidden;
            objTranspilerWindow.Show();
        }
    }
}