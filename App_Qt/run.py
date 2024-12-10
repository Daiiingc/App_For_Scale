from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5.QtCore import Qt, QObject, QThread, pyqtSignal
from login_handle import LOGIN_HANDLE
from main_handle import MAIN_HANDLE
import serial.tools.list_ports
import serial
import message
import constant
import convert
import array as arr
import fsm_array_receive 
import time

port = ""
# xác định cổng đầu vào của USART để thực hiện truyền nhận
cp=serial.tools.list_ports.comports()
for p in cp:
    if(p.serial_number != None):
        port = p.name

ser = serial.Serial(port, baudrate=9600)

arr_out = arr.array('B',[])                 # tạo mảng đầu ra của Create Frame
fsm_arr_out = arr.array('B',[])             # tạo mảng đầu ra sau khi FSM
struct_out = constant.Frame_Msg_T()



# Gửi bản tin từ Python đến STM32
def Send_Msg_To_Stm(arr):
    ser.write(arr)
    
# Nhận bản tin cân nặng có được từ STM32
def Receive_Data_From_Stm():
    value = ser.read(1)
    data = ord(value)
    return data
    
# Tạo bản tin Create để gửi từ Qt
def Create_Msg_To_Send(type_msg):
        if len(arr_out) != 0:
            del arr_out[:]
        
        sensor_send = struct_out.Sensor
        struct_in = constant.Frame_Msg_T()
        message.Message_Create_FrameStruct(struct_in,type_msg,sensor_send)   
        data_out_len = message.Message_Create_Frame_Array(struct_in, arr_out)
        
        print("Ask:")
        message.Print_Array(arr_out,data_out_len)

# Tạo mảng fsm khi nhận được bytes từ STM         
def Fsm_Arr_From_Receive():
    
    if len(fsm_arr_out) != 0:
        del fsm_arr_out[:]
    
    while 1:
        datain = Receive_Data_From_Stm()
        if(fsm_array_receive.Fsm_Test_Array_Receive(datain,fsm_arr_out) != 0): 
            break
       
        # print(fsm_arr_out)
        
    # length_arr_fsm = len(fsm_arr_out)
    # print("FSM:")


# Tách mảng FSM để lưu trữ trong struct đầu ra
def Detect_Frame_From_Fsm():
    struct_out_len = message.Message_Detect_Frame(fsm_arr_out, struct_out)
    # print("Detect:")
    # Print_Struct(struct_out, struct_out_len)

class Worker(QObject):
    finished = pyqtSignal()
    progress = pyqtSignal(int)
    
    def Run(self):
        """Long-running task."""
        while True:
            Fsm_Arr_From_Receive()
            Detect_Frame_From_Fsm()
            self.progress.emit(struct_out.Type_Message)
            time.sleep(0.1)
            

# Chuong trinh chay app hien thi cua can
class My_Ui(QMainWindow):
    def __init__(self, parent = None):
        super().__init__(parent)

        self.loginUI = QMainWindow()
        self.loginHandle = LOGIN_HANDLE(self.loginUI)
        self.loginHandle.Button_Start.clicked.connect(self.Run_Long_Task)
        
        self.loginUI.show()
        self.mainUI = QMainWindow()
        self.mainHandle = MAIN_HANDLE(self.mainUI)
        
        self.mainHandle.list_action.itemDoubleClicked.connect(self.Button_Click)

        self.mainHandle.label_weigth.setText('0')
        self.mainHandle.label_unit.setText("g")
    
    # Viết tin của bản tin answer được nhận
    def Print_Action_Text(self, text):
        self.mainHandle.label_action.setText(text)
    
    # In kết quả cân nặng
    def Print_Weight(self,num):
        number = round(num, 3)
        if(number == 0):
            self.mainHandle.label_weigth.setText('0')
            self.mainHandle.label_unit.setText("")

        elif (number > 0.000 and number < 1.000):
            number *= 1000
            self.mainHandle.label_weigth.setText(str(int(number)))
            self.mainHandle.label_unit.setText("g")
        elif (number > -1.000 and number < 0.000):
            number *= 1000
            self.mainHandle.label_weigth.setText(str(int(number)))
            self.mainHandle.label_unit.setText("g")
        elif (number >= 1.000 or number <= -1.000):
            self.mainHandle.label_weigth.setText(str(number))
            self.mainHandle.label_unit.setText("kg")
        else:
            self.mainHandle.label_weigth.setText(str(number))
            self.mainHandle.label_unit.setText("kg")

    def Button_Click(self,item):
        
        action = "{}".format(item.text())
            
        if(action == "Calibrating Action"):
            print("yes")
            Create_Msg_To_Send(constant.Type_Msg_E.WEIGTH_CALIB_ASK.value)
            Send_Msg_To_Stm(arr_out)
            self.Print_Action_Text("Calib Action")

        elif(action == "Hold Action"):
            Create_Msg_To_Send(constant.Type_Msg_E.WEIGTH_HOLD_ASK.value)
            Send_Msg_To_Stm(arr_out)
            self.Print_Action_Text("Hold Action")
            
        elif(action == "Unhold Action"):
            Create_Msg_To_Send(constant.Type_Msg_E.WEIGTH_UNHOLD_ASK.value)
            Send_Msg_To_Stm(arr_out)
            self.Print_Action_Text("Unhold Action")
            
        elif(action == "Show Weight Action"):
            Create_Msg_To_Send(constant.Type_Msg_E.WEIGTH_ASK.value)
            Send_Msg_To_Stm(arr_out)
            self.Print_Action_Text("Show Action")
    
    #Quá trình chọn bản tin và thực hiện việc truyền nhận
    def Main_Process(self, message):
        match(message):
            case constant.Type_Msg_E.WEIGTH_CALIB_ANWSER.value:
                weigth_value = convert.Convert_Bytes_To_Float(struct_out.Data[:4])

            case constant.Type_Msg_E.WEIGTH_HOLD_ANWSER.value:
                weigth_value = convert.Convert_Bytes_To_Float(struct_out.Data[:4])

            case constant.Type_Msg_E.WEIGTH_UNHOLD_ANWSER.value:
                weigth_value = convert.Convert_Bytes_To_Float(struct_out.Data[:4])

            case constant.Type_Msg_E.WEIGTH_ANWSER.value:
                weigth_value = convert.Convert_Bytes_To_Float(struct_out.Data[:4])
        
        self.Print_Weight(weigth_value)

        sensor_type = convert.Check_Sensor(struct_out.Sensor)
        match(sensor_type):
            case constant.Sensor_Order_E.SENSOR_1.value:
                self.mainHandle.label_device.setText("LOADCELL")
            case constant.Sensor_Order_E.SENSOR_2.value:
                self.mainHandle.label_device.setText("LOADCELL 2")
            case constant.Sensor_Order_E.SENSOR_3.value:
                pass
            case constant.Sensor_Order_E.SENSOR_4.value:
                pass

    def Run_Long_Task(self):
        self.loginUI.hide()
        
        self.thread = QThread()
        self.worker = Worker()
        self.worker.moveToThread(self.thread)
        self.thread.started.connect(self.worker.Run)
        self.worker.finished.connect(self.thread.quit)
        self.worker.finished.connect(self.worker.deleteLater)
        self.thread.finished.connect(self.thread.deleteLater)
        self.worker.progress.connect(self.Main_Process)

        self.thread.start()
        self.mainUI.showMaximized()
    
    
if __name__ == "__main__":
    app = QApplication([])
    Run_UI = My_Ui()
    app.exec_()
