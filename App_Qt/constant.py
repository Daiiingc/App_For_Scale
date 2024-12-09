from dataclasses import dataclass, field
from enum import Enum,auto

from array import array

# tạo mảng phần tử
arr_in = array('B',[])

# Định nghĩa các hằng số của một bản tin

# Hằng số của các phần tử trong bản tin

START_BYTE                   =   0xAA
DEFAULT_LENGTH               =   4      # StartFrame 1 + TypeMessage 1 + Sensor 1 + LengthData 1
WEIGHT_DATA_LENGTH           =   4
CHECKSUM_LENGTH				 =	 2

WEIGTH_CALIB_ASK_LENGTH      =   2        
WEIGTH_CALIB_ANWSER_LENGTH   =   6        
WEIGTH_HOLD_ASK_LENGTH       =   2        				
WEIGTH_HOLD_ANWSER_LENGTH    =   6        
WEIGTH_UNHOLD_ASK_LENGTH     =   2        
WEIGTH_UNHOLD_ANWSER_LENGTH  =   6        
WEIGTH_ASK_LENGTH            =   2        
WEIGTH_ANWSER_LENGTH         =   6   

# Hằng số của type message
class Type_Msg_E(Enum):
    WEIGTH_CALIB_ASK        = 1         # không chứa data về weight	
    WEIGTH_CALIB_ANWSER     = auto()    # chứa data về weigth ngay sau thời điểm bị calibrated!    
    WEIGTH_HOLD_ASK         = auto()    # không chứa data về weight		
    WEIGTH_HOLD_ANWSER      = auto()    # chứa data về: weight tại thời điểm yêu cầu
    WEIGTH_UNHOLD_ASK       = auto()    # không chứa data về weight		
    WEIGTH_UNHOLD_ANWSER    = auto()    # chứa data về: weight sau khi yêu cầu
    WEIGTH_ASK              = auto()    # không chứa data về weight		
    WEIGTH_ANWSER           = auto()    # chứa data về weigth tại thời điểm yêu cầu					

class Sensor_Type_E(Enum):
    LOADCELL = 0x10

class Sensor_Order_E(Enum):
    SENSOR_1 = 0x01
    SENSOR_2 = auto()
    SENSOR_3 = auto()
    SENSOR_4 = auto()
    SENSOR_5 = auto()

class Sensor_E(Enum):
    FORCE = Sensor_Type_E.LOADCELL.value | Sensor_Order_E.SENSOR_1.value

# Struct chứa dữ liệu của bản tin
# @dataclass
class Frame_Msg_T:
    Start_Frame: int = 0
    Type_Message: int = 0
    Sensor: int = 0 
    Length_Data: int = 0    # LengthData = Length( Data[] + CheckFrame )
    Data: int = field(default = arr_in)  # Data[] = WeightData + TextData
    Check_Frame: int = 0

