#ifndef(PRIMITIVE_INc_VAR)
#declare PRIMITIVE_INc_VAR = 1;
    
#macro Primitive_CreateSphere(Name, Position, Radius)
    setAttr(Name, "type", "\"Sphere\"")
    setAttr(Name, "radius", Radius)
    setAttr(Name, "pos", Position)
#end 

#macro Primitive_CreatePlane(Name, Normal, Offset)  
    #local Normal = Normal / vlength(Normal);
    setAttr(Name, "type", "\"Plane\"")
    setAttr(Name, "normal", Normal)
    setAttr(Name, "offset", Offset)
#end   
    

#end