#ifndef(MESH_INC_VAR)
#declare MESH_INC_VAR = 1; 

#include "IO.inc"
#include "colors.inc"
#include "transforms.inc"
#include "Array.inc"

//getFace[i]
//getFacecloseToPoint(point)
//^ fait le dot product de chaque face et trouve le plus petit
//renderMesh
//render normals
//render vertexs    
//getfacenormal(i)
//pushOutside(point)  

//TODO : getPointDeepness : donne a quel point le point est profond dans le collider
//permet de chopper le point le plus profond entre deux collider et pousser celui la



#declare PUSH_REVERSE_RESOLUTION = 0.01;

//BASICS----------------------------------------------------------------
#macro Mesh_CreateMesh(Name, VertexArray, VertCount)  
//Name.array.I.p1, Name.array.I.p2 Name.array.I.p3 Name.array.I+1.p1 ...
    #local Current = 0;
    setAttr(Name, "vertCount", VertCount)
    #for(I, 0, VertCount-1)
        #if(I = Current)
            #local AttrName = concat(Name, ".array");
            #local Index = str(I/3, 0, 0);
            #local FullName = concat(AttrName, ".", Index, ".");
            #local P1 = VertexArray[I];
            #local P2 = VertexArray[I+1];
            #local P3 = VertexArray[I+2];
            #local Current = I+3;
            
            StoreVar(concat(FullName, "p1"), P1)
            StoreVar(concat(FullName, "p2"), P2)
            StoreVar(concat(FullName, "p3"), P3)
        #end 
    #end
#end 

#macro Mesh_MoveMesh(Name, Move)
    #local VertCount = getFloatAttr(Name, "vertCount");
    #for(I, 0, VertCount/3 -1)
        #local VarName = concat(Name, ".array.", str(I, 0, 0), ".");
        #local P1 = ReadVect(concat(VarName, "p1"));
        #local P2 = ReadVect(concat(VarName, "p2"));
        #local P3 = ReadVect(concat(VarName, "p3"));
        
        #local P1 = P1 + Move;
        #local P2 = P2 + Move;
        #local P3 = P3 + Move;
        
        StoreVar(concat(VarName, "p1"), P1)
        StoreVar(concat(VarName, "p2"), P2)
        StoreVar(concat(VarName, "p3"), P3)
    #end
#end

#macro Mesh_RotateMesh(Name, Rotation, Anchor)
    #local VertCount = getFloatAttr(Name, "vertCount");
    #for(I, 0, VertCount/3 -1)
        #local VarName = concat(Name, ".array.", str(I, 0, 0), ".");
        #local P1 = ReadVect(concat(VarName, "p1"));
        #local P2 = ReadVect(concat(VarName, "p2"));
        #local P3 = ReadVect(concat(VarName, "p3"));
        
        #local P1 = vtransform(P1,Rotate_Around_Trans(Rotation, Anchor));
        #local P2 = vtransform(P2,Rotate_Around_Trans(Rotation, Anchor));
        #local P3 = vtransform(P3,Rotate_Around_Trans(Rotation, Anchor));
        
        StoreVar(concat(VarName, "p1"), P1)
        StoreVar(concat(VarName, "p2"), P2)
        StoreVar(concat(VarName, "p3"), P3)
    #end
#end 

#macro Mesh_GetVertex(Name, Index)
    #local VertCount = getFloatAttr(Name, "vertCount");
    #if(Index > VertCount)
        #error "(Mesh)vertex out of bound, bitch";
    #end 
    
    #local FaceIndex = floor(Index/3);
    #local VertInFace = Index - (FaceIndex * 3) + 1;
    #local VarName = concat(Name, ".array.", 
        str(FaceIndex, 0,0), ".p", str(VertInFace, 0, 0));
    #local VertPos = ReadVect(VarName);
    VertPos;    
#end

#macro Mesh_GetVertCount(Name)
    #local VertCount = getFloatAttr(Name, "vertCount");
    VertCount;
#end

#macro Mesh_GetFaceNormal(Name, Index)
    #local VarName = concat(Name, ".array.", str(Index, 0, 0), ".");
    #local P1 = ReadVect(concat(VarName, "p1"));
    #local P2 = ReadVect(concat(VarName, "p2"));
    #local P3 = ReadVect(concat(VarName, "p3"));
    
    #local UpVect = vcross(P2-P1, P3-P1); 
    #local UpVect = UpVect/vlength(UpVect);
    UpVect;     
#end

#macro Mesh_GetFaceCenter(Name, Index)
    #local VarName = concat(Name, ".array.", str(Index, 0, 0), ".");
    #local P1 = ReadVect(concat(VarName, "p1"));
    #local P2 = ReadVect(concat(VarName, "p2"));
    #local P3 = ReadVect(concat(VarName, "p3"));
    
    #local Return = (P1 + P2 + P3)/3.0;
    Return;
#end

#macro Mesh_GetMeshCenter(Name) 
    #local Addition = <0,0,0>;
    #local VertCount = getFloatAttr(Name, "vertCount");
    #for(I, 0, VertCount/3 -1)
        #local VarName = concat(Name, ".array.", str(I, 0, 0), ".");
        #local P1 = ReadVect(concat(VarName, "p1"));
        #local P2 = ReadVect(concat(VarName, "p2"));
        #local P3 = ReadVect(concat(VarName, "p3"));
        
        #local Addition = Addition + P1 + P2 + P3;        
    #end
    #local Addition = Addition / VertCount;
    Addition; 
#end 

#macro Mesh_GetFaceClosestToPoint(Name, Point)
    //trouve le dot product le plus bas ? en Abs()
    
    //faire une liste de face qui contiennent potentiellement ce point
    //en faisant un is below mais avec la face genre 2 point + la normal modifier
    //puis parcourir cette liste et  trouver la fac qui a un dot product avec le 
    //point le plus faible en math.abs()
     #local PotentialFaces = array[1]{-1};
     #local VertCount = getFloatAttr(Name, "vertCount");
          
     #for(I, 0, VertCount/3 - 1)
         #local Bool = Mesh_IsPointPotentiallyInsideFace(Name, I, Point);
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
            #local Normal = Mesh_GetFaceNormal(Name, PotentialFaces[I]);
            #local FacePoint = ReadVect(concat(Name, ".array.", str(PotentialFaces[I], 0, 0), ".p1"));
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
     Return;
#end 

#macro Mesh_IsPointPotentiallyInsideFace(Name, Index, Point)
    //P1 P1-NORMAL P2
    //P1 P1+Normal P3
    //P2 P2-Normal P3                                
    #local VarName = concat(Name, ".array.", str(Index, 0, 0), ".");
    #local P1 = ReadVect(concat(VarName, "p1"));
    #local P2 = ReadVect(concat(VarName, "p2"));
    #local P3 = ReadVect(concat(VarName, "p3"));
    #local Normal = Mesh_GetFaceNormal(Name, Index);
    
    #local Res1 = Mesh_IsPointBelowThreePoint(Point, P1, P1-Normal, P2);
    #local Res2 = Mesh_IsPointBelowThreePoint(Point, P1, P1+Normal, P3);
    #local Res3 = Mesh_IsPointBelowThreePoint(Point, P2, P2-Normal, P3);
    #local Result = Res1+Res2+Res3;
    
    #if(Result = 3)
        #local Return = 1;
    #else
        #local Return = 0;
    #end                  
    Return;  
#end 

#macro Mesh_IsPointBelowThreePoint(Point, P1, P2, P3)
    #local Normal = vcross(P2-P1, P3-P1);
    #local DirVect = Point - P1;         
    #local DotProduct = vdot(Normal, DirVect);
    //Mesh_DrawVector(P1, Normal)
    #if(DotProduct < 0 | DotProduct = 0)
        #local Return = 1;
    #else
        #local Return = 0;
    #end                  
    Return;
#end

//COLLISION----------------------------------------------------------
#macro Mesh_IsPointBelowFace(Name, Index, Point)
    #local Normal = Mesh_GetFaceNormal(Name, Index);
    #local FacePoint = ReadVect(concat(Name, ".array.", str(Index, 0, 0), ".p1"));
    #local DirVect = Point - FacePoint;
    #local DotProduct = vdot(Normal, DirVect);
    
    #if(DotProduct < 0 | DotProduct = 0)
        #local Return = 1;
    #else
        #local Return = 0;
    #end                  
    Return;
#end 

#macro Mesh_IsPointInsideMesh(Name, Point)
    #local VertCount = getFloatAttr(Name, "vertCount");
    #local Break = 0;
    #for(I, 0, VertCount/3 -1)
        #local IsBelowThisFace = Mesh_IsPointBelowFace(Name, I, Point);
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

#macro Mesh_IsMeshCollidingWithMe(Name, OtherMesh)
    //check every point of other mesh to see if any is inside me
    #local OtherMeshCount = Mesh_GetVertCount(OtherMesh);
    #local AnyInside = 0;
    #for(I, 0, OtherMeshCount - 1)                          
        #local Vert = Mesh_GetVertex(OtherMesh, I);
        #local ThisOneInside = Mesh_IsPointInsideMesh(Name, Vert);
        #if(ThisOneInside)
            #local AnyInside = 1;
            #break
        #end 
    #end          
    #if(AnyInside)
        #local Return = 1;            
    #else
        #local Return = 0;
    #end                 
    Return;
#end

#macro Mesh_GetPushMeshOutsideOfMeVect(Name, OtherMesh)
    //this assumes OtherMesh is inside Name mesh 
    #local BiggestVect = <0, 0, 0>;
    #local BiggestDist = 0;
    
    #local OtherMeshCount = Mesh_GetVertCount(OtherMesh);
    #for(I, 0, OtherMeshCount - 1)                          
        #local Vert = Mesh_GetVertex(OtherMesh, I);
        #local Inside = Mesh_IsPointInsideMesh(Name, Vert);
        
        #if(Inside)
            #local PushVect = Mesh_GetPushOutsideVect(Name, Vert);
            #local PushLen = vlength(PushVect);
            #if(PushLen > BiggestDist)
                #local BiggestDist = PushLen;
                #local BiggestVect = PushVect;
            #end
        #end        
    #end
    //Biggest vect is the vect of push the furtherest inside of me
    #debug concat("\npushing In Mesh Pushmesh=", vstr(3, BiggestVect, "/", 0, -1), "\n")
    BiggestVect;
#end

#macro Mesh_GetPushOutsideVect(Name, Point) 
    #local OriginalPoint = <Point.x, Point.y, Point.z>;
    #debug concat("\noriginal=", vstr(3, OriginalPoint, "/", 0, -1), "\n")
    //copy the point
    #local NewPoint = Mesh_PushPointOutside(Name, Point);
    #local Return = NewPoint - OriginalPoint;
    #debug concat("\nNew=", vstr(3, NewPoint, "/", 0, -1), "\n")
    #debug concat("\ndiff=", vstr(3, Return, "/", 0, -1), "\n")
    Return;
#end

#macro Mesh_PushPointOutside(Name, Point)
    #local Return = Point;
    #local IsInside = Mesh_IsPointInsideMesh(Name, Point);
    #if(IsInside)
        #declare ClosestFaceIndex = Mesh_GetFaceClosestToPoint(Name, Point);
        #if(ClosestFaceIndex > -1)                                                
            #local Normal = Mesh_GetFaceNormal(Name, ClosestFaceIndex);
            #local Return = 
            Mesh_PushPointOutsideAlongNormal(Name, Point, Normal, PUSH_REVERSE_RESOLUTION);
        #end 
    #end 
    Return;
#end                        

#macro Mesh_PushPointOutsideAlongNormal(Name, Point, Normal, ReverseResolution)
    #local IsInside = Mesh_IsPointInsideMesh(Name, Point);
    #local Return = Point;
    
    #while(IsInside)
        #local Return = Return + Normal*ReverseResolution;
        #local IsInside = Mesh_IsPointInsideMesh(Name, Return);
    #end                                                     
    Return;
#end
 
 
 //VISUAL------------------------------------------------------------
#macro Mesh_RenderVertex(Name)
    #local VertCount = getFloatAttr(Name, "vertCount");
    #local Current = 0;
    
    #for(I, 0, VertCount-1)
        #if(I = Current)
            #local VarName = concat(Name, ".array.", str(I/3, 0, 0), ".");
            #local P1 = ReadVect(concat(VarName, "p1"));
            #local P2 = ReadVect(concat(VarName, "p2"));
            #local P3 = ReadVect(concat(VarName, "p3"));
            #local Current = I+3;
            
            sphere_sweep{
                linear_spline,
                4,
                P1, .03
                P2, .03
                P3, .03
                P1, .03
                pigment{Blue}
            }
            sphere{P1, .06 pigment{Blue}}
            sphere{P2, .06 pigment{Blue}}
            sphere{P3, .06 pigment{Blue}}
        #end
    #end
#end       

#macro Mesh_DrawFaceNormal(Name, Index)
    #local Center = Mesh_GetFaceCenter(Name, Index);
    #local Normal = Mesh_GetFaceNormal(Name, Index);
    Mesh_DrawVector(Center, Normal)
#end

#macro Mesh_DrawAllFaceNormal(Name)
    #local VertCount = getFloatAttr(Name, "vertCount");
    #for(I, 0, VertCount/3 - 1)
        Mesh_DrawFaceNormal(Name, I)
    #end
#end

#macro Mesh_DrawVector(Point, Vector)
    sphere_sweep{
        linear_spline,
        2,
        Point, .03
        Point + Vector, .03
        pigment{Green}
    }                 
    sphere{Point + Vector, .07 pigment{Green}}
#end


#end //end Mesh.inc