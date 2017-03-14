#ifndef(ARRAY_INC_VAR)
#declare ARRAY_INC_VAR = 1;

#macro ExpandArray(Array, N)
	#local Size = dimension_size(Array, 1);
	#local NewSize = Size + N;
	#local NewArray = array[NewSize];
	#for (I, 0, Size-1)
		#ifdef (Array[I])
		#local NewArray[I] = Array[I];
		#end
	#end
	#declare Array = NewArray;
#end

#macro AddToArray(Array, Val)
	#local Nb = dimension_size(Array, 1);
	ExpandArray(Array, 1)
	#declare Array[Nb] = Val;
#end 

#macro ArrayContainsFloat(Array, Val)
    #local Size = dimension_size(Array, 1);
    #local Return = 0;
    #for(I, 0, Size-1)
        #if(Array[I] = Val)
            #local Return = 1;
            #break
        #end
    #end          
    Return;
#end

#macro RemoveFromArray(Array, Index)
    #local Size = dimension_size(Array, 1);
    #local Return = array[Size-1];
    
    #for(I, 0, Index-1)
       #local Return[I] = Array[I];                
    #end                     
    #for(I, Index+1, Size-1)
        #local Return[I-1] = Array[I];
    #end                       
    Return;  
    //WARNING : you can't put a ";" after calling this method, go figure why 
#end

#end //end Array.inc 