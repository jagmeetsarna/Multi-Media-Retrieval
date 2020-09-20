import os
import csv



dir_path=r"C:\Users\Jagmeet's PC\Desktop\Multimedia retrieval\labeledDb\Sample_LabeledDB"
#filename=r"C:\Users\Jagmeet's PC\Desktop\Multimedia retrieval\labeledDb\LabeledDB_new\LabeledDB_new\Airplane\61.off"

folders = os.walk(dir_path)

shape_class=[] 


fold_struct={}

for folder in folders:
    
    shape_class.append(os.path.basename(folder[0]))
    
    fold_struct[shape_class[-1]] = folder[2]
    #print(shape_class)
    #filename=r"C:\Users\Jagmeet's PC\Desktop\Multimedia retrieval\labeledDb\LabeledDB_new\LabeledDB_new\{}\61.off".format(shape_class)

for key in fold_struct:

    for value in fold_struct[key]:
        filename = dir_path +"\\{}\\{}".format(key,value)
       # print(filename)
        if("off" in filename.split('\\')[-1]):
            
            f=open(filename,"r")
            lines=f.readlines()
            column_1=[]
            column_2=[]
            column_3=[]
            for x in lines[2:]:
                x=x.replace("\n","")
                column_1.append(x.split(' ')[0])
                column_2.append(x.split(' ')[1])
                column_3.append(x.split(' ')[2])
            f.close()
            #shape
            shape = int(column_1[-1])
            shape_type=""
            if shape == 3:
                shape_type= "Triangle"
            else:
                shape_type= "Quadilateral"
                
            faces = lines[1].split(' ')[1]
            vertices = lines[1].split(' ')[0]
            #x-axis    
            x_axis=[]
            
            for i in column_1[2:]:
                if("." in i):
                    x_axis.append(float(i))
                
                    
                    
            max_x_axis =  max(x_axis)
            min_x_axis =  min(x_axis)
            
            #y-axis    
            y_axis=[]
            
            for i in column_2[2:]:
                if("." not in i):
                    break
                y_axis.append(float(i))
                    
            max_y_axis =  max(y_axis)
            min_y_axis =  min(y_axis)
            
            #z-axis    
            z_axis=[]
            
            for i in column_3[2:]:
                if("." not in i):
                    break
                z_axis.append(float(i))
                    
            max_z_axis =  max(z_axis)
            min_z_axis =  min(z_axis)
            
            vol =(max_x_axis -  min_x_axis)*(max_y_axis-min_y_axis)*(max_z_axis-min_z_axis)
            row_list = [key,filename.split('\\')[-1],shape_type,vertices,faces,max_x_axis,min_x_axis,max_y_axis,min_y_axis,max_z_axis,min_z_axis,vol]
            with open('filter.csv', 'a',newline='') as file:
                writer = csv.writer(file)
                writer.writerow(row_list)
            
       


        
        
    
    


