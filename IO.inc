#ifndef(IO_INC_VAR)
#declare IO_INC_VAR = 1;                  
                  
#macro ReadVect(StrFilePath)
	#fopen mFile StrFilePath read
	#local Result = <0, 0, 0>;
	#read (mFile, Result)
	#fclose mFile
	Result;
#end

#macro ReadFloat(StrFilePath)
	#fopen mFile StrFilePath read
	#local Result = 0;
	#read (mFile, Result)
	#fclose mFile
	Result; 
#end 

#macro ReadStr(StrFilePath)
	#fopen mFile StrFilePath read
	#local Result = "";
	#read (mFile, Result)
	#fclose mFile
	Result;
#end

#macro ReadVar(StrFilePath)
    #fopen mFile StrFilePath read
    #read(mFile, Result)
    #fclose mFile
    Result
#end

#macro StoreVar(StrFilePath, VarToStore)
	#fopen mFile StrFilePath write
	#write (mFile, VarToStore)
	#fclose mFile
#end

#macro setAttr(StrVarName, StrAttrName, Val)
    StoreVar(concat(StrVarName, ".", StrAttrName), Val)
#end                                                   

#macro getStrAttr(StrVarName, StrAttrName)
    ReadStr(concat(StrVarName, ".", StrAttrName))
#end                                             

#macro getVectAttr(StrVarName, StrAttrName)
    ReadVect(concat(StrVarName, ".", StrAttrName))
#end                                              

#macro getFloatAttr(StrVarName, StrAttrName)
    ReadFloat(concat(StrVarName, ".", StrAttrName))
#end

#macro WriteArray(Path, Arr)
    #local Len = dimension_size(Arr, 1);
    #fopen mFile Path write
    #for(I, 0, Len-1)
        #write(mFile, Arr[I])
        #if(I != Len-1)
            #write(mFile, ",")
        #end 
    #end
    #fclose mFile
#end

#macro WriteArrayWCount(Path, Arr, Len)
    #fopen mFile Path write
    #for(I, 0, Len-1)
        #write(mFile, Arr[I])
        #if(I != Len-1)
            #write(mFile, ",")
        #end 
    #end
    #fclose mFile
#end

#macro ReadArray(Path, Len)
    #local ArrOut = array[Len] 
    #fopen mFile Path read
    #for(I, 0, Len-1)
        #read (mFile, Reading)
        #local ArrOut[I] = Reading;                            
    #end
    ArrOut;
    #fclose mFile
#end 

#macro WriteObj(Path, ArrObj)             
    #local Len = dimension_size(ArrObj, 1);
    #fopen mFile Path write
    #for(I, 0, Len-1)        
        #local ToWrite = ArrObj[I][0];
        #write(mFile, ToWrite)
        #if(I != Len-1)
            #write(mFile, ",")
        #end           
    #end
    #fclose mFile
#end

#macro ReadObj(Path, ArrOut)
    #local Len = dimension_size(ArrOut, 1);
    #fopen mFile Path read
    #for(I, 0, Len-1)
        #read (mFile, Reading)
        #local ArrOut[I] = array[1]{Reading}                            
    #end
    #fclose mFile
#end 
/*
  Example to use the last 2 methods 
WriteObj("testFile", array[3]{array[1]{1}, array[1]{<4, 6, 8>}, array[1]{"\"salut\""}}) 
#declare mArr = array[3];
ReadObj("testFile", mArr)

#debug concat("\n", str(mArr[0][0], 0, -1), "\n")
#debug concat("\n", vstr(3, mArr[1][0], "/", 0, -1), "\n")
#debug concat("\n", mArr[2][0], "\n")  

*/      

#end //end IO.inc