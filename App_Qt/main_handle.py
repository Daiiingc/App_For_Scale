from main import Ui_MainWindow
from PyQt5.QtGui import QFont


class MAIN_HANDLE(Ui_MainWindow):
    
    def __init__(self,mainwindow):
        self.setupUi(mainwindow)

    def list_add(self):
        self.list_action.addItem(" ")
        self.list_action.setCurrentRow(4)

    def Action_Show(self):
        
        self.label_action.setFont(QFont('Arial',40))
        
        
    
    def Number_Show(self):
        
        self.label_number.setFont(QFont('Arial',40))

    #---------------------