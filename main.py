from DataManage import *
from SerialCom import *

# def main() :   
#      testControl  = Protocol()
#      testData = Database()
#      try :          
#           while True :
#                a = testControl.readData()
#                if a == 'g' :
#                     break
#           # testControl.getSamplelist()
#           testControl.getSamplelist()
#           testData.dataUpdate(testControl.sampleList)
          
#           testControl.writeData('n')
#      except :
#           print("error")

# if __name__ == '__main__' :
#      main()

def main() :   
     testControl  = SerialCom("COM5")
     testControl2 = SerialCom('COM10') 

     sampleList = []
     try :           
          while True :
               a = testControl2.readData()
               if a == 'g' :
                    break
          testControl2.writeData('m') # motor on
          while True :
               a = testControl2.readData()
               if a == 'e' :
                    break # motor done               
          testControl.writeData("s")  # scale on
          while True : 
               sample = testControl.readData()                    
               if sample == "e" :
                    break
               else  :
                    sample = float(sample)
                    sampleList.append(sample)    
          

          print("serial end")
          testControl2.writeData('r') # reverse on
          while True :
               a = testControl2.readData()
               if a == 'e' :
                    break # reverse done          
     except  KeyboardInterrupt :
          print("stop")

if __name__ == '__main__' :
     main()