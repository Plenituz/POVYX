#ifndef(BOXCOLLIDER_INC_VAR)
#declare BOXCOLLIDER_INC_VAR = 1;         

#include "IO.inc"
      
//Point 1 has to be bottom left and point 2 top right
#macro BoxCollider_CreateBoxCollider(Name, Point1, Point2)
    setAttr(Name, "p1", Point1)
    setAttr(Name, "p2", Point2)
    
#end

#macro BoxCollider_IsPointInside(Name, Point)
    #local P1 = getVectAttr(Name, "p1");
    #local P2 = getVectAttr(Name, "p2");
                                       
    #local Result = 0;
    #if(Point.x >= P1.x & Point.y >= P1.y)
        #if(Point.z >= P1.z & Point.x <= P2.x)
            #if(Point.y <= P2.y & Point.z <= P2.z)
                #local Result = 1;
            #end        
        #end
    #end
    Result;
#end

#macro BoxCollider_IntersectWithOtherBox(Name, Other)
    #local mP1 = getVectAttr(Name, "p1");
    #local mP2 = getVectAttr(Name, "p2");
    #local tP1 = getVectAttr(Other, "p1");
    #local tP2 = getVectAttr(Other, "p2");
    
    #local Return = 0;
    #if(mP1.x <= tP2.x & mP2.x >= tP1.x)
        #if(mP1.y <= tP2.y & mP2.y >= tP1.y)
            #if(mP1.z <= tP2.z & mP2.z >= tP1.z)
                #local Return = 1;
            #end
        #end
    #end
    Return;
#end

#macro BoxCollider_Draw(Name, Col)
    #local P1 = getVectAttr(Name, "p1");
    #local P2 = getVectAttr(Name, "p2"); 
    
    box{P1, P2 pigment{Col} }
#end
                                
#end // end BoxCollider.inc