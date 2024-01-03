from serial import Serial 
from numpy.core.fromnumeric import mean, std
import sys
import time

class SerialCom(Serial) :
    def __init__(self,prt) :
        super().__init__(port = prt , baudrate = 9600 )        
        
        if self.readable() == True :
            print(prt, "serial on")

    def readData(self) :
        x = self.readline()
        x = x.decode()
        x = x.rstrip('\r\n')  
        return x

    def writeData(self,char) :
        self.write(char.encode())

class Protocol(SerialCom) :
    def __init__(self) :
        super().__init__()

        self.protocolChar = ''
        self.cnt = 0
        
        self.sample = 0
        self.sample2 =0
        self.sample3 =0
        self.sample4 =0

        self.sampleList = []
        self.sampleList2 = []
        self.sampleList3 = []
        self.sampleList4 = []
        
        self.tem = 0
        self.hum =0
        self.data = 0
        self.dataList = []
        self.errorList = []

    # def selectPort(self,p) :
    #     p = clicksdl;a  
    #     super().__init__(p)


    def getSamplelist(self) :                      
        try:
            self.sampleList = []
            self.sampleList2 = []
            self.sampleList3 = []
            
            self.writeData("m")
            while True :
                self.sample = self.readData()                    
                if self.sample == "e" :
                    self.sample2 = mean(self.sampleList2)
                    self.sample3 = mean(self.sampleList3)
                    # self.sample4 = mean(self.sampleList4)
                    break
                else  :
                    self.sample = float(self.sample)
                    self.sampleList.append(self.sample)
                    if self.cnt == 0 :
                        self.sampleList2.append(self.sample)
                        self.cnt += 1 
                    elif self.cnt == 1 : 
                        self.sampleList3.append(self.sample)
                        self.cnt = 0
                    # elif self.cnt == 2 : 
                    #     self.sampleList4.append(self.sample)
                    #     self.cnt = 0 
            print(self.sample2)
            print(self.sample3)
            # print(self.sample4)                
            print("serial end")
        except :        
            print("Sample serial error")
            sys.exit() 

    def getSample(self) :                      
        try:
            self.sampleList = []
            self.writeData("m")
            while True :
                self.sample = self.readData()                    
                if self.sample == "e" :
                    break
                else  :
                    self.sample = float(self.sample)
                    self.sampleList.append(self.sample)    
            print(self.sampleList)                
            print("serial end")
        except :        
            print("Sample serial error")
            sys.exit() 

    def calculate(self) :         
        self.Min = min(self.sampleList)
        self.Max = max(self.sampleList)
        self.Avg = mean(self.sampleList)
        self.Standard = std(self.sampleList)
        self.calFlag = True
        print("Min:",self.Min)
        print("Max:",self.Max)
        print("Avg:",self.Avg)
        print("Standard:",self.Standard)

    def boundary(self) : 
        if self.calFlag == True :
            self.infimum = self.Avg - self.Standard
            self.supremum = self.Avg + self.Standard
            self.bounFlag =  True
            print("infimum:",self.infimum)
            print("supremum:",self.supremum)
        else :
            print("please calculate first")

    def errorCheck(self,x) :   
        if self.bounFlag == True : 
            if (x < self.infimum) or (x > self.supremum) :
                try : 
                    self.errorFlag = True
                    print("error")
                except :
                    print("error check error")                
            else :
                print("ok")
        else :
            print("please boundary first")

    # Temperature and humidity
    def getTH(self) :            
        try :                  
            self.writeData("t")        
            self.tem = self.readData()
            self.hum = self.readData()
            self.tem = float(self.tem)
            self.hum = float(self.hum)
            print("Temperature:",self.tem)
            print("Humidity:",self.hum)
        except :
            print("getTH error")
            sys.exit()

    # Weight
    def getData(self) :                    
        try:
            self.dataList = []
            self.errorList = []
            self.errorFlag = False

            self.writeData("d")
            while True :
                self.data = self.readData()
                #end check
                if self.data == "e" :
                    break
                else :
                    self.data = float(self.data)
                    self.dataList.append(self.data)
            print(self.dataList)

            # index 문제해결하기 
            for i in range(len(self.dataList)) :
                self.errorCheck(self.dataList[i])
                if self.errorFlag == True :
                    self.errorList.append(i)
                    # self.writeData("x")
                    self.errorFlag = False
            
            print(self.errorList)                     
            print(self.dataList)

            self.errorList.reverse()

            for i in self.errorList :
                del self.dataList[i]                

                if i == 0 :
                    print("0 error")
                elif i == 1 : 
                    print("1 error")
                elif i == 2 : 
                    print("2 error")          
                elif i == 3 : 
                    print("3 error")

            print(self.dataList)    
            
            self.writeData("e")
            print("end")
        except :
            print("getWeight error")
            sys.exit() 
    
    # def sampleQ(self) :
    #     try :
    #         self.char = input("Typing a:",)
    #         self.writeData(self.char)
    #         while True :
    #             self.data = self.readData()
    #             if self.data == "e" :
    #                 print("read sample done")
    #                 break
    #     except :
    #         print("sampleQ error")


