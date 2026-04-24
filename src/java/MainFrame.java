import javax.swing.*;

public class MainFrame extends JFrame {
        
        MainFrame() {
                setTitle("Planar Graph Engine");
                setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                setResizable(false);
                setSize(900, 900);
                setVisible(true);
var iconUrl = MainFrame.class.getResource("/icon.png");
if (iconUrl == null) {
    System.out.println("Nie znaleziono ikony!");
} else {
    setIconImage(new ImageIcon(iconUrl).getImage());
}
        }        
}
