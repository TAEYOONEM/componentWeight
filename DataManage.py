import pandas as pd
import sys

class Database() : 
    def __init__(self) :                
        self.raw_table = pd.DataFrame(
            {
                'Tem and Hum': [],
                'N': []
            }
        )
    
    # Temperature and Humidity Update
    def thUpdate(self,data) :                    
        try :
            self.data_series = pd.Series(
                                { 
                                    'Tem and Hum' : data,
                                    'N': 0
                                }
                )            
            self.raw_table = self.raw_table.append(self.data_series,ignore_index=True) ## List 랑 다름
            print(self.raw_table)
        except :
            print("TH update error")
            sys.exit()

    # Data
    def dataUpdate(self,list) :
        try :
            for i in list :                
                i  = float(i)
                self.data_series = pd.Series(
                                { 
                                    'Tem and Hum' : 0,
                                    'N': i
                                }
                )   
                self.raw_table = self.raw_table.append(self.data_series,ignore_index=True) ## List 랑 다름
            print(self.raw_table)                         
        except :
            print("Data update error")
            sys.exit()

    def tableExel(self) :
        self.raw_table.to_excel('database.xlsx')

