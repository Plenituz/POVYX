#ifndef(COLLISION_INC_VAR)
#declare COLLISION_INC_VAR = 1;
#include "strings.inc"     
    
#macro Collision_Collide(Primitive1, Primitive2)
    #local Type1 = getStrAttr(Primitive1, "type")
    #local Type2 = getStrAttr(Primitive2, "type")
    #local ContactName = "None"
    #local Call = concat("#declare DoICollide = Collision_", Type1, "_", Type2, "(\"", Primitive1,"\",\"", Primitive2, "\", ContactName);");
    Parse_String(Call)
    
    #local Return = 0; 
    #if(DoICollide)
        #local Return = 1;       
    #end
    Return;
    
    //func name convention : Collision_Type1_Type2(Name1, Name2, OutContactName)
#end                                                            

#macro Collision_Sphere_Sphere(Name, Other, OutContactName)
    #local PosMe = getVectAttr(Name, "pos");
    #local PosOther = getVectAttr(Other, "pos");
    #local RadMe = getFloatAttr(Name, "radius");
    #local RadOther = getFloatAttr(Other, "radius");
    #local RadSum = RadMe + RadOther;
    #local Diff = PosMe - PosOther;
    #local DiffLength = vlength(Diff);
            
    #declare OutContactName = "None"
    #local Return = 0;
    #if(DiffLength <= RadSum)
        #local Return = 1;
        
        #local ContactNormal = Diff/DiffLength; //povray takes care of division by 0, in another language you have to check for 0 lenght vector before
        #local ContactPoint = PosMe + Diff*0.5;
        #local ContactPene = RadMe + RadOther - DiffLength;
        Contact_CreateContact("contact")
        Contact_AddContactPoint("contact", ContactPoint)
        Contact_AddContactNormal("contact", ContactNormal)
        Contact_AddContactPenetration("contact", ContactPene)
        #declare OutContactName = "contact"
    #end              
    Return;
#end 

#macro Collision_Sphere_Plane(Name1, Name2, OutContactName)
   // Collision_Plane_Sphere(Name1, Name2, OutContactName)
#end

#macro Collision_Plane_Sphere(Name1, Name2, OutContactName)                                                        
    /*#local Type1 = getStrAttr(Name1, "type")
    #if(Type1 = "Plane")
        #local PlanePoint = getVectAttr(Name1, "offset")
        #local PlaneNormal = getVectAttr(Name1, "normal")
        #local SpherePos = getVectAttr(Name2, "pos")
        #local SphereRad = getFloatAttr(Name2, "radius")
    #else
        #local PlanePoint = getVectAttr(Name2, "offset")
        #local PlaneNormal = getVectAttr(Name2, "normal")
        #local SpherePos = getVectAttr(Name1, "pos")
        #local SphereRad = getFloatAttr(Name1, "radius")
    #end
    
    #local ToPoint = PlanePoint - SpherePos;
    #local U = PlaneNormal * vdot(ToPoint, PlaneNormal);
    #local ULength = vlength(U);
    //U is the vector going from the center of the sphere to the plane by
    //the smalles distance
    #declare OutContactName = "None"
    #local Return = 0;              
    #if(ULength <= SphereRad)
        #local Return = 1;
        #local ContactPoint = SpherePos - PlaneNormal * (ULength + SphereRad);
        #local ContactNormal = PlaneNormal;
        #local ContactPene = -ULength;
        Contact_CreateContact("contact")
        Contact_AddContactPoint("contact", ContactPoint)
        Contact_AddContactNormal("contact", ContactNormal)
        Contact_AddContactPenetration("contact", ContactPene)
        #declare OutContactName = "contact"
    #end
    Return; */                                                 
#end