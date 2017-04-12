#ifndef(DISK_ARRAY_INc_VAR)
#declare DISK_ARRAY_INc_VAR = 1; 

#include "IO.inc"


/*  
    This is an ordered list but it creates one file per object 

  You can store without specifying type, therefore
  you can have an array with different types inside 
  each index.
  But you have to know the type when you read the file.
*/
#macro DiskArray_CreateDiskArray(Name)
    setAttr(Name, "count", 0)
#end

#macro DiskArray_GetCount(Name)
    #local Count = getFloatAttr(Name, "count");
    Count;
#end

#macro DiskArray_GetVarName(Name, Index)
    #local VarName = concat(Name, ".diskarray.", str(Index, 0,0));
    VarName;
#end

#macro DiskArray_Add(Name, Val)
    #local Count = getFloatAttr(Name, "count");
    #local VarName = concat(Name, ".diskarray.", str(Count, 0,0));
    
    StoreVar(VarName, Val)//store the value    
    setAttr(Name, "count", Count+1)//update the count
#end 

#macro DiskArray_Set(Name, Index, Val)
    #local VarName = concat(Name, ".diskarray.", str(Index, 0, 0));
    StoreVar(VarName, Val)
#end

#macro DiskArray_GetFloat(Name, Index)
    #local Count = getFloatAttr(Name, "count");
    #if(Index > Count)
        #error "Index out of bound, bitch"
    #end
    
    #local VarName = concat(Name, ".diskarray.", str(Index, 0,0));
    #local Return = ReadFloat(VarName);
    Return;
#end

#macro DiskArray_GetVect(Name, Index)
    #local Count = getFloatAttr(Name, "count");
    #if(Index > Count)
        #error "Index out of bound, bitch"
    #end
    
    #local VarName = concat(Name, ".diskarray.", str(Index, 0,0));
    #local Return = ReadVect(VarName);
    Return;
#end

#macro DiskArray_GetStr(Name, Index)
    #local Count = getFloatAttr(Name, "count");
    #if(Index > Count)
        #error "(diskarray) Index out of bound, bitch"
    #end
    
    #local VarName = concat(Name, ".diskarray.", str(Index, 0,0));
    #local Return = ReadStr(VarName)
    Return;
#end

#end // end DiskArray.inc