#ifndef(SOFTMESHCOLLISION_INC_VAR)
#declare SOFTMESHCOLLISION_INC_VAR = 1; 

#include "SoftMesh.inc"
#include "IO.inc"        
#include "BoxCollider.inc"

//if point inside
//get face closest to point


#macro SoftMeshCollision_IsPointInsideMesh(Name, Point)
    #local TriArrName = concat(Name, ".triangles");
    #local TriCount = DiskArray_GetCount(TriArrName); 
    
    #local Break = 0;
    #for(I, 0, TriCount/3 -1)
        #local IsBelowThisFace = SoftMeshCollision_IsPointBelowFace(Name, I, Point);
        #if(!IsBelowThisFace)
            #local Break = 1;
            #break
        #end                 
    #end          
    #if(!Break)
        #local Return = 1;
    #else
        #local Return = 0;
    #end
    Return;
#end

#macro SoftMeshCollision_IsPointBelowFace(Name, FaceIndex, Point)
    #local Normal = SoftMesh_GetFaceNormal(Name, FaceIndex);
    #local FacePoints = SoftMesh_GetFaceVerts(Name, FaceIndex);
    #local FacePoint =  FacePoints[0];
    #local DirVect = Point - FacePoint;
    #local DotProduct = vdot(Normal, DirVect);
    
    #if(DotProduct <= 0)
        #local Return = 1;
    #else
        #local Return = 0;
    #end                  
    Return;
#end 

#macro SoftMeshCollision_GetFaceClosestToPoint(Name, Point)
    //trouve le dot product le plus bas ? en Abs()
    
    //faire une liste de face qui contiennent potentiellement ce point
    //en faisant un is below mais avec la face genre 2 point + la normal modifier
    //puis parcourir cette liste et  trouver la fac qui a un dot product avec le 
    //point le plus faible en math.abs()
     #local PotentialFaces = array[1]{-1};
     #local TriArrName = concat(Name, ".triangles");
     #local TriCount = DiskArray_GetCount(TriArrName);
          
     #for(I, 0, TriCount/3 - 1)
         #local Bool = SoftMeshCollision_IsPointPotentiallyInsideFace(Name, I, Point);
         #if(Bool)
             AddToArray(PotentialFaces, I)
         #end
     #end                                 
     
     //on a une liste de face potentiellement etant la plus proche
     //maintenant on la parcours et on garde seulement la face qui donne
     //le abs(dot product) le plus bas
     
     //on commence par l'index 1 parceque le 0 c'est -1 (necessaire pour facilité
     //l'expansion de l'array 
     
     #local Return = -1;
     #if(dimension_size(PotentialFaces, 1) > 1)       
         #local LowestDotProduct = 2;
         #local LowestFace = PotentialFaces[1];
         #for(I, 1, dimension_size(PotentialFaces, 1)-1)
            #local Normal = SoftMesh_GetFaceNormal(Name, PotentialFaces[I]); 
            #local FacePoints = SoftMesh_GetFaceVerts(Name, PotentialFaces[I]);
            #local FacePoint = FacePoints[0];
            
            #local DirVect = Point - FacePoint;
            #local DotProduct = vdot(Normal, DirVect);
            #local DotProduct = abs(DotProduct);
            
            #if(DotProduct < LowestDotProduct)
                #local LowestDotProduct = DotProduct;
                #local LowestFace = PotentialFaces[I];
            #end                                      
         #end
         #local Return = LowestFace;                                         
     #end
     //#debug concat("\nclosest face=", str(Return,0,0), "\n")
     Return;
#end

#macro SoftMeshCollision_IsPointPotentiallyInsideFace(Name, FaceIndex, Point)
    //P1 P1-NORMAL P2
    //P1 P1+Normal P3
    //P2 P2-Normal P3                                
    #local FacePoints = SoftMesh_GetFaceVerts(Name, FaceIndex);
    #local P1 = FacePoints[0];
    #local P2 = FacePoints[1];
    #local P3 = FacePoints[2];
    #local Normal = SoftMesh_GetFaceNormal(Name, FaceIndex);
    
    #local Res1 = SoftMeshCollision_IsPointBelowThreePoint(Point, P1, P1-Normal, P2);
    #local Res2 = SoftMeshCollision_IsPointBelowThreePoint(Point, P1, P1+Normal, P3);
    #local Res3 = SoftMeshCollision_IsPointBelowThreePoint(Point, P2, P2-Normal, P3);
    #local Result = Res1+Res2+Res3;
    
    #if(Result = 3)
        #local Return = 1;
    #else
        #local Return = 0;
    #end                  
    Return;  
#end 

#macro SoftMeshCollision_IsPointBelowThreePoint(Point, P1, P2, P3)
    #local Normal = vcross(P2-P1, P3-P1);
    #local DirVect = Point - P1;         
    #local DotProduct = vdot(Normal, DirVect);
    //Mesh_DrawVector(P1, Normal)
    #if(DotProduct <= 0)
        #local Return = 1;
    #else
        #local Return = 0;
    #end                  
    Return;
#end

#macro SoftMeshCollision_GetVectorToExitMesh(Name, Point, OutNormal)
//http://math.stackexchange.com/questions/100439/determine-where-a-vector-will-intersect-a-plane
    #local O = <Point.x, Point.y, Point.z>;
    #local ClosestFaceIndex = SoftMeshCollision_GetFaceClosestToPoint(Name, Point);
    #local Normal = SoftMesh_GetFaceNormal(Name, ClosestFaceIndex);
    #local FacePoints = SoftMesh_GetFaceVerts(Name, ClosestFaceIndex);
    #local A = FacePoints[0];
    
    #local K = Normal.x*(O.x - A.x) + Normal.y*(O.y - A.y) + Normal.z*(O.z - A.z);
    #local Below = (Normal.x*Normal.x) + (Normal.y*Normal.y) + (Normal.z*Normal.z);
    #local T = -K/Below;
    
    #local IntersectionPoint = <O.x + Normal.x*T, O.y + Normal.y*T, O.z + Normal.z*T>;
    
    #local Return = IntersectionPoint - O;
    #declare OutNormal = Normal;
    Return;       
#end

/*#macro SoftMeshCollision_GetVectorToExitMesh(Name, Point, Step)
    #local OriginalPoint = <Point.x, Point.y, Point.z>;
    #local ClosestFaceIndex = SoftMeshCollision_GetFaceClosestToPoint(Name, Point);
    #local Normal = SoftMesh_GetFaceNormal(Name, ClosestFaceIndex);
    #local IsInside = SoftMeshCollision_IsPointInsideMesh(Name, Point);
    
    #while(IsInside)
        #local Point = Point + Normal*Step;
        #local IsInside = SoftMeshCollision_IsPointInsideMesh(Name, Point);
    #end
    //the point is outside the mesh now
    #local Return = Point - OriginalPoint;
    Return;    
#end*/ 

#macro SoftMeshCollision_CollideWith(Name, OtherSoftMesh)
    //pour chaque point de Name check si il est a l'interrieur de OtherSoftMesh
    //si c'est le cas pousser le point de la quantité necessaire pour sortir de OtherSoftMesh 
    #local VertArrName = concat(Name, ".vertex");
    #local VertCount = DiskArray_GetCount(VertArrName);
    #local ColliderBouncyness = getFloatAttr(OtherSoftMesh, "bouncyness");
    
    #for(I, 0, VertCount-1)                                   
        #local Point = DiskArray_GetVect(VertArrName, I);
        #local IsInside = SoftMeshCollision_IsPointInsideMesh(OtherSoftMesh, Point);
        #if(IsInside)                                                                       
            #local Normal = <0,0,0>;
            #local ExitVector = SoftMeshCollision_GetVectorToExitMesh(OtherSoftMesh, Point, Normal);
            SoftMesh_PushPoint(Name, I, ExitVector)
            //SoftMesh_PushPoint(Name, I, Normal*ColliderBouncyness)
            //the velocity has to bleed to adacent points 
            SoftMesh_SetPointVelocity(Name, I, <0,0,0>)
            SoftMesh_SpreadVelocity(Name, I, Normal*ColliderBouncyness)
            SoftMesh_SpreadReduceVelocity(Name, I)
            SoftMesh_SpreadTorque(Name, I, Normal)
            //TODO BOUNCE
        #end
    #end
#end

#macro SoftMeshCollision_BuildBoxCollider(Name)
    #local VertCount = SoftMesh_GetVertCount(Name);
    #local P1 = SoftMesh_GetPointPos(Name, 0);
    #local SmallestX = P1.x;
    #local SmallestY = P1.y;
    #local SmallestZ = P1.z;
    #local BiggestX = P1.x;
    #local BiggestY = P1.y;
    #local BiggestZ = P1.z;
    
    #for(I, 0, VertCount-1)
        #local CurrentPoint = SoftMesh_GetPointPos(Name, I);
        #if(CurrentPoint.x < SmallestX)
            #local SmallestX = CurrentPoint.x;
        #end                                  
        #if(CurrentPoint.y < SmallestY)
            #local SmallestY = CurrentPoint.y;
        #end                                  
        #if(CurrentPoint.z < SmallestZ)
            #local SmallestZ = CurrentPoint.z;
        #end                                  
        
        #if(CurrentPoint.x > BiggestX)
            #local BiggestX = CurrentPoint.x;
        #end
        #if(CurrentPoint.y > BiggestY)
            #local BiggestY = CurrentPoint.y;
        #end
        #if(CurrentPoint.z > BiggestZ)
            #local BiggestZ = CurrentPoint.z;
        #end
    #end 
    #local P1 = <SmallestX, SmallestY, SmallestZ>;
    #local P2 = <BiggestX, BiggestY, BiggestZ>;
    #local CollName = concat(Name, "_BoxCollider");
    
    BoxCollider_CreateBoxCollider(CollName, P1, P2)
    setAttr(Name, "boxCollider", concat("\"", CollName, "\""))
#end

#macro SoftMeshCollision_DoMeshsBoxCollidersIntersect(Name1, Name2)
    #local Coll1 = getStrAttr(Name1, "boxCollider")
    #local Coll2 = getStrAttr(Name2, "boxCollider")
    
    #local DoCollide = BoxCollider_IntersectWithOtherBox(Coll1, Coll2);
    DoCollide;
#end 

#macro SoftMeshCollision_DrawBoxCollider(Name, Col)
    #local CollName = getStrAttr(Name, "boxCollider")
    BoxCollider_Draw(CollName, Col)
#end

#end// end SoftMeshCollision.inc