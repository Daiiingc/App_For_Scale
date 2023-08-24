
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtCore import QObject, QTimer,pyqtSignal, QThread
from login_handle import LOGIN_HANDLE
from main_handle import MAIN_HANDLE
import threading
import serial
import message
import constant
import convert
import array as arr
import fsm_array_receive 


# xác định cổng đầu vào của USART để thực hiện truyền nhận
ser = serial.Serial("COM5", baudrate=9600)

arr_out = arr.array('B',[])                 # tạo mảng đầu ra của Create Frame
fsm_arr_out = arr.array('B',[])             # tạo mảng đầu ra sau khi FSM

count = 0
flag = 1

# Gửi bản tin từ Python đến STM32
def Send_Msg_To_Stm(arr):
    ser.write(arr)
    
# Nhận bản tin cân nặng có được từ STM32
def Receive_Data_From_Stm():
    global count
    value = ser.read(1)
    data = ord(value)
    
     
    return data
    
# Tạo bản tin Create để gửi từ Qt
def Create_Msg_To_Send(type_msg):
        if len(arr_out) != 0:
            del arr_out[:]
            
        struct_in = constant.Frame_Msg(0x55, type_msg)
        data_out_len = message.Message_Create_Frame(struct_in, arr_out)
        print("Ask:")
        Print_Array(arr_out,data_out_len)

# Tạo mảng fsm khi nhận được bytes từ STM         
def Fsm_Arr_From_Receive():
    
    if len(fsm_arr_out) != 0:
        del fsm_arr_out[:]
    
    
    while 1:
        datain = Receive_Data_From_Stm()
        print(hex(datain), end= ' ')
        if(fsm_array_receive.Fsm_Test_Array_Receive(datain,fsm_arr_out) != 0): 
            break
        
        # print(fsm_arr_out)
        
    # length_arr_fsm = len(fsm_arr_out)
    # print("FSM:")


# Tách mảng FSM để lưu trữ trong struct đầu ra
def Detect_Frame_From_Fsm(struct_out):
    struct_out_len = message.Message_Detect_Frame(fsm_arr_out, struct_out)
    # print("Detect:")
    # Print_Struct(struct_out, struct_out_len)

# in mảng
def Print_Array(arr,arr_len):
    
    for i in range(arr_len):
        print(hex(arr[i]), end=' ')
    print('\n')
        
# in các thành phần có trong struct
def Print_Struct(struct, len_data):

    print("Start Frame:", hex(struct.Start_Frame))
    print("Type_Message:", hex(struct.Type_Message))
    print("Length_Data:", hex(struct.Length_Data))
    print("Length_Text", hex(struct.Length_Text))
    print("Data:", end= ' ')
    for i in range(len_data):
        print(hex(struct.Data[i]), end=' ')
    
    print('\n')
    print("Check_Frame:", hex(struct.Check_Frame))

# class USART_Thread(QThread):
#     data_received = pyqtSignal(str)
    
#     def __init__(self):
#         super(USART_Thread, self).__init__()
#         # self.port = uart_port
#         # self.baudrate = uart_baudrate
#         self.uart = None

#     def Run(self):
#         try:
#             self.ser = serial.Serial(self.port, self.baudrate)
#         while True:
#             Receive_Data_From_Stm()

        


# Chuong trinh chay app hien thi cua can
class My_Ui():
    def __init__(self):
        
        super().__init__()
        self.loginUI = QMainWindow()
        self.loginHandle = LOGIN_HANDLE(self.loginUI)
        self.loginHandle.Button_Start.clicked.connect(lambda: self.LoadLoginForm())
        self.loginUI.show()
        
        self.mainUI = QMainWindow()
        self.mainHandle = MAIN_HANDLE(self.mainUI)
        
        self.mainHandle.list_action.itemDoubleClicked.connect(self.Button_Click)
        

        # self.timer = QTimer()
        # self.timer.timeout.connect(self.Button_Click)
        # # # self.timer.timeout.connect(self.Button_Click)
        
        

    def LoadLoginForm(self):
        self.loginUI.hide()
        self.mainUI.show()
        
        # self.timer.start(500)
        
        # self.timer.start(500)

    # Viết tin của bản tin answer được nhận
    def Print_Action_Text(self, text):
        self.mainHandle.Action_Show()
        self.mainHandle.label_action.setText(text)

    # In kết quả cân nặng
    def Print_Weight(self,num):
        self.mainHandle.Number_Show()
        if(num == 0):
            int_num = int(num)
            self.mainHandle.label_number.setText(str(int_num))
        elif (num > 0 and num < 1):
            int_num = round(num*1000) 
            self.mainHandle.label_number.setText(str(int_num) + " Gram")
        elif (num > -1 and num < 0):
            int_num = round(num*1000) 
            self.mainHandle.label_number.setText(str(int_num) + " Gram")
        elif (num >= 1 or num <= -1):
            float_num = round(num,3)
            self.mainHandle.label_number.setText(str(float_num) + " Kilogram")
        else:
            float_num = round(num,3)
            self.mainHandle.label_number.setText(str(float_num) + " Kilogram")

    # Quá trình truyền nhận bản tin và in kết quả
    def Process_Message(self):
       
        struct_out = constant.Frame_Msg(0,0)
        Detect_Frame_From_Fsm(struct_out)

        weigth_value = convert.Convert_Bytes_To_Float(struct_out.Data[:4])
        text = convert.Convert_Bytes_To_Text(struct_out.Data[4:])
        print(text)
        
        self.Print_Action_Text(text)
        self.Print_Weight(weigth_value)

        

    def Button_Click(self,item):
        
        
        # self.timer.start(500)
        action = "{}".format(item.text())
            
        if(action == "Calibrating Action"):       
            Create_Msg_To_Send(constant.Type_Msg.WEIGTH_CALIB_ASK.value)
            Send_Msg_To_Stm(arr_out)
                
        elif(action == "Hold Action"):
            Create_Msg_To_Send(constant.Type_Msg.WEIGTH_HOLD_ASK.value)
            Send_Msg_To_Stm(arr_out)
            

        elif(action == "Unhold Action"):
            Create_Msg_To_Send(constant.Type_Msg.WEIGTH_UNHOLD_ASK.value)
            Send_Msg_To_Stm(arr_out)
                
        elif(action ==  "Show Weight Action"):
            Create_Msg_To_Send(constant.Type_Msg.WEIGTH_ASK.value)
            Send_Msg_To_Stm(arr_out)
        
        
        
        
        
        # print(action)
        
        
    
    #Quá trình chọn bản tin và thực hiện việc truyền nhận
    def Main_Process(self):
            
            Fsm_Arr_From_Receive()
            
            if(fsm_array_receive.Fsm_Get_DataReady_Flag()):
                self.Process_Message()
                fsm_array_receive.Fsm_Clear_DataReady_Flag()
                  
        # self.timer.singleShot(500,self.Main_Process)
  
    

    # t1 = threading.Thread(target=Main_Process)
    # t1.start()




if __name__ == "__main__":
    app = QApplication([])
    Run_UI = My_Ui()
    app.exec_()


# import sys
# from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QPushButton
# from PyQt5.QtCore import pyqtSignal, pyqtSlot, QThread
# import serial


# class UARTThread(QThread):
#     dataReceived = pyqtSignal(str)

#     def __init__(self, uart_port, uart_baudrate):
#         super(UARTThread, self).__init__()
#         self.uart_port = uart_port
#         self.uart_baudrate = uart_baudrate
#         self.uart = None

#     def run(self):
#         try:
#             self.uart = serial.Serial(self.uart_port, self.uart_baudrate)
#             while True:
#                 data = self.uart.readline().strip().decode()
#                 self.dataReceived.emit(data)
#         except serial.SerialException:
#             pass

#     def stop(self):
#         if self.uart:
#             self.uart.close()


# class MainWindow(QMainWindow):
#     def __init__(self):
#         super(MainWindow, self).__init__()
#         self.setWindowTitle("Weight Display")
#         self.setGeometry(100, 100, 300, 200)

#         self.weight_label = QLabel("Weight: ", self)
#         self.weight_label.setGeometry(20, 20, 200, 30)

#         self.calib_button = QPushButton("Calib", self)
#         self.calib_button.setGeometry(20, 70, 60, 30)
#         self.calib_button.clicked.connect(self.sendCalibCommand)

#         self.hold_button = QPushButton("Hold", self)
#         self.hold_button.setGeometry(90, 70, 60, 30)
#         self.hold_button.clicked.connect(self.sendHoldCommand)

#         self.unhold_button = QPushButton("Unhold", self)
#         self.unhold_button.setGeometry(160, 70, 60, 30)
#         self.unhold_button.clicked.connect(self.sendUnholdCommand)

#         self.show_weight_button = QPushButton("Show Weight", self)
#         self.show_weight_button.setGeometry(230, 70, 80, 30)
#         self.show_weight_button.clicked.connect(self.sendShowWeightCommand)

#         self.uart_thread = UARTThread('/dev/ttyUSB0', 9600)
#         self.uart_thread.dataReceived.connect(self.updateWeightLabel)

#     def sendCommand(self, command):
#         try:
#             with serial.Serial('/dev/ttyUSB0', 9600) as uart:
#                 uart.write(command.encode())
#         except serial.SerialException:
#             pass

#     @pyqtSlot()
#     def sendCalibCommand(self):
#         self.sendCommand("CALIB")

#     @pyqtSlot()
#     def sendHoldCommand(self):
#         self.sendCommand("HOLD")

#     @pyqtSlot()
#     def sendUnholdCommand(self):
#         self.sendCommand("UNHOLD")

#     @pyqtSlot()
#     def sendShowWeightCommand(self):
#         self.sendCommand("SHOW")

#     @pyqtSlot(str)
#     def updateWeightLabel(self, data):
#         self.weight_label.setText("Weight: " + data)

#     def closeEvent(self, event):
#         self.uart_thread.stop()
#         self.uart_thread.wait()
#         event.accept()


# if __name__ == "__main__":
#     app = QApplication(sys.argv)
#     window = MainWindow()
#     window.show()
#     window.uart_thread.start()
#     sys.exit(app.exec_())
