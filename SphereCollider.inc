#ifndef(SPHERECOLLIDER_INC_VAR)
#declare SPHERECOLLIDER_INC_VAR = 1; 

#include "IO.inc"

#macro SphereCollider_CreateCollider(Name, Radius, Position)
    setAttr(Name, "type", "\"Sphere\"")
    setAttr(Name, "radius", Radius)
    setAttr(Name, "pos", Position)    
#end                              

#macro SphereCollider_IsCollidingWith(Name, Other)
    #local Type = getStrAttr(Other, "type")
           
    #local Return = 0;
    #if(Type == "SphereCollider")
         #local PosMe = getVectAttr(Name, "pos");
         #local PosOther = getVectAttr(Other, "pos");
         #local RadMe = getFloatAttr(Name, "radius");
         #local RadOther = getFloatAttr(Other, "radius");
         #local RadSum = RadMe + RadOther;
         #local Diff = PosMe - PosOther;
         #local Diff = vlength(Diff);
         
         #if(Diff <= RadSum)
             #local Return = 1;
         #end
    #else
        //other collider types
    #end   
    
    Return;
#end

#end