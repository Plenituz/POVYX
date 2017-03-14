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

#end //end IO.inc