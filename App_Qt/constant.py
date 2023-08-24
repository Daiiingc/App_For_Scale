from dataclasses import dataclass, field
from enum import Enum,auto

from array import array

# tạo mảng phần tử
arr_in = array('B',[])

# Định nghĩa các hằng số của một bản tin

# Hằng số của các phần tử trong bản tin

START_BYTE                  = 0x55
DEFAULT_LENGTH              = 4 # StartFrame 1 + TypeMessage 1 + LengthData 1 + LengthText 1
WEIGHT_DATA_LENGTH          = 4

# đoạn tin xuất hiện của từng chúc năng
CALIB_ANWSER_TEXT           = "Calibrated!"
HOLD_ANWSER_TEXT            = "Holding!" 
UNHOLD_ANWSER_TEXT          = "Stopped holding!"
WEIGHT_ANWSER_TEXT          = "Weight:"

# Độ dài của các đoạn tin
CALIB_ANWSER_TEXT_LENGTH    = len(CALIB_ANWSER_TEXT)
HOLD_ANWSER_TEXT_LENGTH     = len(HOLD_ANWSER_TEXT)
UNHOLD_ANWSER_TEXT_LENGTH   = len(UNHOLD_ANWSER_TEXT)
WEIGHT_ANWSER_TEXT_LENGTH   = len(WEIGHT_ANWSER_TEXT)
#define CALIB_ASK_TEXT_LENGTH           
#define HOLD_ASK_TEXT_LENGTH
#define UNHOLD_ASK_TEXT_LENGTH
#define WEIGHT_ASK_TEXT_LENGTH

WEIGTH_CALIB_ASK_LENGTH     = 2                                   # CheckFrame 2 
WEIGTH_CALIB_ANWSER_LENGTH  = 4 + CALIB_ANWSER_TEXT_LENGTH + 2    # Data: weight 4 + text 11 + CheckFrame 2 */
WEIGTH_HOLD_ASK_LENGTH      = 2                                   # CheckFrame 2				
WEIGTH_HOLD_ANWSER_LENGTH   = 4 + HOLD_ANWSER_TEXT_LENGTH  + 2    # Data: weight 4 + text 9 + CheckFrame 2*/
WEIGTH_UNHOLD_ASK_LENGTH    = 2                                   # CheckFrame 2
WEIGTH_UNHOLD_ANWSER_LENGTH = 4 + UNHOLD_ANWSER_TEXT_LENGTH + 2   # Data: weight 4 + text 16 + CheckFrame 2 */
WEIGTH_ASK_LENGTH           = 2                                   # CheckFrame 2
WEIGTH_ANWSER_LENGTH        = 4 + WEIGHT_ANWSER_TEXT_LENGTH + 2   # Data: weight 4 + text 8 + CheckFrame 2 */   

# Hằng số của type message
class Type_Msg(Enum):
    WEIGTH_CALIB_ASK        = 1         # không chứa data về weight		55 01 02 00 41 48
    WEIGTH_CALIB_ANWSER     = auto()    # chứa data về weigth ngay sau thời điểm bị calib, text: calibrated!    
    WEIGTH_HOLD_ASK         = auto()    # không chứa data về weight		55 03 02 00 E0 88
    WEIGTH_HOLD_ANWSER      = auto()    # chứa data về: weight tại thời điểm yêu cầu, text: "holding!"
    WEIGTH_UNHOLD_ASK       = auto()    # không chứa data về weight		55 05 02 00 00 89
    WEIGTH_UNHOLD_ANWSER    = auto()    # chứa data về: weight sau khi yêu cầu, text: "stopped holding!"
    WEIGTH_ASK              = auto()    # không chứa data về weight		55 07 02 00 A1 49
    WEIGTH_ANWSER           = auto()    # chứa data về weigth tại thời điểm yêu cầu, text: "weight:"					

# Struct chứa dữ liệu của bản tin
@dataclass
class Frame_Msg:
    Start_Frame: int
    Type_Message: int
    Length_Data: int = 0    # LengthData = Length( Data[] + CheckFrame )
    Length_Text: int = 0
    Data: int = field(default = arr_in)  # Data[] = WeightData + TextData
    Check_Frame: int = 0








