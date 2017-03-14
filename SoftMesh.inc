#ifndef(SOFTMESH_INC_VAR)
#declare SOFTMESH_INC_VAR = 1;

#include "DiskArray.inc"    
#include "Array.inc"
#include "IO.inc"
#include "transforms.inc"
#include "strings.inc"


//-----------------------------------------------------------------------------------------MESH CREATION AND BASIC MANIPULATION                       
#macro SoftMesh_CreateSoftMesh(Name, VertexArray, TriangleArray, VertexCount, TriangleCount, Rotation, RotationAnchor)
    //Name.vertex.1, Name.vertex.2 etc
    //Name.triangles.1 Name.triangles.2 etc
    //LEs triangle vont par groupe de 3
    //face number c'est 1 face tous les 3 triangle value   
    #local VertArrName = concat(Name, ".vertex");
    #local TriArrName = concat(Name, ".triangles");
     
    DiskArray_CreateDiskArray(VertArrName)
    DiskArray_CreateDiskArray(TriArrName)
    setAttr(Name, "reformRate", 1)//how fast does it go back to its original form
    setAttr(Name, "pushMultiplierByStep", 1)//kinda rigidty but different
    setAttr(Name, "bouncyness", 0)//how much velocity is conserved when a rigidbody bounces against this surface
    setAttr(Name, "maxDistanceFromBase", 1)//kinda rigidity
    setAttr(Name, "multiplyAdjacentPointVelOnBounce", 0.8)//how good does the material transfert "contact wave" or "push wave" but inverted, the higher, the less wave are tranfered IE the material deforms a lot                                  
    
    #for(I, 0, VertexCount-1)
        DiskArray_Add(VertArrName, VertexArray[I])
    #end                                   
    #for(I, 0, TriangleCount-1)
        DiskArray_Add(TriArrName, TriangleArray[I])
    #end
    
    SoftMesh_RotateMesh(Name, Rotation, RotationAnchor)
    
    #local Center = SoftMesh_GetMeshCenter(Name);
    #for(I, 0, VertexCount-1)
        #local VertVarName = DiskArray_GetVarName(VertArrName, I)
        #local PointPos = SoftMesh_GetPointPos(Name, I);
        #local Offset = PointPos - Center;
        
        setAttr(VertVarName, "offsetFromOriginAtStart", Offset)
        setAttr(VertVarName, "velocity", <0,0,0>) 
    #end                                     
#end 

#macro SoftMesh_SetBouncyness(Name, Val)
    setAttr(Name, "bouncyness", Val)
#end

#macro SoftMesh_SetReformRate(Name, Val)
    setAttr(Name, "reformRate", Val)
#end

#macro SoftMesh_SetDeformRate(Name, Val)
    setAttr(Name, "pushMultiplierByStep", Val)
#end

#macro SoftMesh_MaxDistanceFromBase(Name, Val)
    setAttr(Name, "maxDistanceFromBase", Val)
#end 

#macro SoftMesh_GetVertCount(Name)
    #local VertArrName = concat(Name, ".vertex");
    #local VertCount = DiskArray_GetCount(VertArrName);
    VertCount;
#end

#macro SoftMesh_GetTriangleCount(Name)
    #local TriArrName = concat(Name, ".triangles");
    #local TriCount = DiskArray_GetCount(TriArrName);
    TriCount;
#end

#macro SoftMesh_GetPointPos(Name, PointIndex)
    #local VertArrName = concat(Name, ".vertex");
    #local Return = DiskArray_GetVect(VertArrName, PointIndex);
    Return;
#end

#macro SoftMesh_SetPointPos(Name, PointIndex, Pos)
    #local VertArrName = concat(Name, ".vertex");
    DiskArray_Set(VertArrName, PointIndex, Pos)
#end

#macro SoftMesh_SetPointVelocity(Name, PointIndex, Vel)
    #local VertArrName = concat(Name, ".vertex");
    #local VectVarName = DiskArray_GetVarName(VertArrName, PointIndex)
    setAttr(VectVarName, "velocity", Vel)
#end

#macro SoftMesh_GetPointVelocity(Name, PointIndex)
    #local VertArrName = concat(Name, ".vertex");
    #local VectVarName = DiskArray_GetVarName(VertArrName, PointIndex)
    #local Return = getFloatAttr(VectVarName, "velocity")
    Return;
#end

#macro SoftMesh_RotateMesh(Name, Rotation, Anchor)
    #local VertArrName = concat(Name, ".vertex");
    #local VertCount = DiskArray_GetCount(VertArrName);
    
    #for(I, 0, VertCount-1)
        #local V = DiskArray_GetVect(VertArrName, I);
        #local V = vtransform(V, Rotate_Around_Trans(Rotation, Anchor));
        DiskArray_Set(VertArrName, I, V)
    #end
#end

#macro SoftMesh_ScaleMesh(Name, Scale, Anchor)
    #local VertArrName = concat(Name, ".vertex");
    #local VertCount = DiskArray_GetCount(VertArrName);
    
    #for(I, 0, VertCount-1)
        #local V = DiskArray_GetVect(VertArrName, I);  
        
        #local OffsetVect = V - Anchor;
        #local OffsetVect = <OffsetVect.x*Scale.x, OffsetVect.y*Scale.y, OffsetVect.z*Scale.z>;
        #local V = Anchor + OffsetVect; 
        
        DiskArray_Set(VertArrName, I, V)
    #end
#end

#macro SoftMesh_MoveMeshUniformly(Name, Move)
    #local VertArrName = concat(Name, ".vertex");
    #local VertCount = DiskArray_GetCount(VertArrName);
    
    #for(I,0,VertCount-1)
        #local CurrentPos = DiskArray_GetVect(VertArrName, I);
        #local NewPos = CurrentPos + Move;
        DiskArray_Set(VertArrName, I, NewPos)
    #end
#end

#macro SoftMesh_AddToVelocityUniformly(Name, Force)
    #local VertArrName = concat(Name, ".vertex");
    #local VertCount = DiskArray_GetCount(VertArrName);
    
    #for(I,0,VertCount-1)
        #local VectVarName = DiskArray_GetVarName(VertArrName, I)
        #local CurrentVel = getVectAttr(VectVarName, "velocity");
        setAttr(VectVarName, "velocity", CurrentVel + Force)
    #end
#end 

//-----------------------------------------------------------------------------------------MESH DISPLAY
#macro SoftMesh_DrawVerts(Name)
    #local VertArrName = concat(Name, ".vertex");
    #local VertCount = DiskArray_GetCount(VertArrName);
    
    #for(I, 0, VertCount-1)  
        #local V = DiskArray_GetVect(VertArrName, I);
        sphere{V, .06 pigment{Blue}}
    #end
#end

#macro SoftMesh_DrawTriangles(Name)
    #local TriArrName = concat(Name, ".triangles");
    #local TriCount = DiskArray_GetCount(TriArrName);
    
    #for(I, 0, TriCount/3 -1)
        #local Vs = SoftMesh_GetFaceVerts(Name, I);
        
        sphere_sweep{
            linear_spline,
            4,
            Vs[0], 0.03
            Vs[1], 0.03
            Vs[2], 0.03
            Vs[0], 0.03
            pigment{Blue}
        }
    #end
#end

#macro SoftMesh_DrawMeshCenter(Name)
    #local Center = SoftMesh_GetMeshCenter(Name);
    sphere{Center, 0.1 pigment{Red}}
#end

#macro SoftMesh_DrawFaceNormals(Name)
    #local TriArrName = concat(Name, ".triangles");
    #local TriCount = DiskArray_GetCount(TriArrName);
    
    #for(I, 0, TriCount/3 -1)
        #local FaceNormal = SoftMesh_GetFaceNormal(Name, I);
        #local FaceCenter = SoftMesh_GetFaceCenter(Name, I);
        SoftMesh_DrawVector(FaceCenter, FaceNormal)
    #end
#end

#macro SoftMesh_DrawFaceNormal(Name, FaceIndex)
    #local Normal = SoftMesh_GetFaceNormal(Name, FaceIndex); 
    #local Center = SoftMesh_GetFaceCenter(Name, FaceIndex);
    SoftMesh_DrawVector(Center, Normal)
#end

#macro SoftMesh_DrawDebugMesh(Name)
    SoftMesh_DrawVerts(Name)
    SoftMesh_DrawTriangles(Name)
    //SoftMesh_DrawFaceNormals(Name)
    SoftMesh_DrawMeshCenter(Name)
    SoftMesh_DrawVertsVelocity(Name)
#end

#macro SoftMesh_DrawAsMesh(Name)
    #local MeshStr = SoftMesh_GetMeshString(Name)
    Parse_String(MeshStr)
#end

#macro SoftMesh_GetMeshString(Name)
    #local MeshStr = "mesh{"
    #local TriCount = SoftMesh_GetTriangleCount(Name);
    //#debug concat("\ntricount=", str(TriCount, 0,0), "\n")
    #for(I, 0, TriCount/3 -1)
        #local ThisTriangleStr = "triangle{"
        #local TrianglePoints = SoftMesh_GetFaceVerts(Name, I);
        #local P1Str = concat("<", vstr(3, TrianglePoints[0], ",", 0, -1), ">");
        #local P2Str = concat("<", vstr(3, TrianglePoints[1], ",", 0, -1), ">");
        #local P3Str = concat("<", vstr(3, TrianglePoints[2], ",", 0, -1), ">");
        #local ThisTriangleStr = concat(ThisTriangleStr, P1Str, ",", P2Str, ",", P3Str, "}");
        #local MeshStr = concat(MeshStr, ThisTriangleStr);
    #end                                                        
    #local MeshStr = concat(MeshStr, " pigment{Red}}");
    MeshStr
#end

#macro SoftMesh_GlowPoint(Name, PointIndex)
    #local VertArrName = concat(Name, ".vertex");
    #local Pos = DiskArray_GetVect(VertArrName, PointIndex);
    sphere{Pos, 0.1 pigment{Red}}
#end

#macro SoftMesh_DrawVector(Point, Vector)
    sphere_sweep{
        linear_spline,
        2,
        Point, .03
        Point + Vector, .03
        pigment{Green}
    }                 
    sphere{Point + Vector, .07 pigment{Green}}
#end

#macro SoftMesh_DrawVertsVelocity(Name)
    #local VertArrName = concat(Name, ".vertex");
    #local VertCount = DiskArray_GetCount(VertArrName);
    
    #for(I,0,VertCount-1)
        #local VectVarName = DiskArray_GetVarName(VertArrName, I)
        #local CurrentVel = getVectAttr(VectVarName, "velocity");
        #local CurrentPos = SoftMesh_GetPointPos(Name, I);
        SoftMesh_DrawVector(CurrentPos, CurrentVel)
    #end
#end 

//-----------------------------------------------------------------------------------------MESH SOFTBODY PHYSICS 

#macro SoftMesh_Update(Name, Framerate, AirDrag)
    //apply velocity and torque
    #local VertArrName = concat(Name, ".vertex");
    #local VertCount = DiskArray_GetCount(VertArrName);
    #local MeshCenter = SoftMesh_GetMeshCenter(Name);
    #local ReformRate = getFloatAttr(Name, "reformRate");
    
    #for(I, 0, VertCount-1)
        #local VertVarName = DiskArray_GetVarName(VertArrName, I)
        #local OldPos = SoftMesh_GetPointPos(Name, I);
        #local PointVelocity = getVectAttr(VertVarName, "velocity");
        #local OffsetAtStart = getVectAttr(VertVarName, "offsetFromOriginAtStart");
        #local AimPosition = MeshCenter + OffsetAtStart;
        #local ReformVelocity = AimPosition - OldPos;
            
        
        #local PointVelocity = PointVelocity + ReformVelocity*ReformRate;
        //air drag
        #local PointVelocity = PointVelocity*(1-AirDrag);
        //apply velocity
        #local NewPos = OldPos + PointVelocity*(1/Framerate);
                
        setAttr(VertVarName, "velocity", PointVelocity)
        SoftMesh_SetPointPos(Name, I, NewPos)        
    #end
#end

#macro SoftMesh_PushPoint(Name, PointIndex, Force)                           
    //#local OldPos = SoftMesh_GetPointPos(Name, PointIndex);
    //SoftMesh_SetPointPos(Name, PointIndex, OldPos + Force)
    #local AlreadyPushed = array[1]{-1};
    #local ReduceForcePerStep = getFloatAttr(Name, "pushMultiplierByStep");
    //SoftMesh_SetPointVelocity(Name, PointIndex, Force*ForceCarry)
    SoftMesh_SpreadPush(Name, PointIndex, Force, AlreadyPushed, ReduceForcePerStep)
#end

#macro SoftMesh_SpreadPush(Name, PointIndex, Force, AlreadyPushed, ReduceBy)
    #local OldPos = SoftMesh_GetPointPos(Name, PointIndex);
    SoftMesh_SetPointPos(Name, PointIndex, OldPos + Force)  //push point
    
    AddToArray(AlreadyPushed, PointIndex) //update array preventing infinite loops
    #declare Force = Force * ReduceBy; //reduce force  
                                       
    //pass force along                                       
    #local ConnectedPointIndexes = SoftMesh_GetConnectedVertsIndexes(Name, PointIndex);
    #local Size = dimension_size(ConnectedPointIndexes, 1);
    #for(I, 0, Size-1)
        #local IsPushed = ArrayContainsFloat(AlreadyPushed, ConnectedPointIndexes[I]);
        #if(!IsPushed)
            SoftMesh_SpreadPush(Name, ConnectedPointIndexes[I], Force, AlreadyPushed, ReduceBy)
        #end
    #end 
    //TODO prendre en compte : plus le point est poussé plus il est dur de le pousser
    //TODO scatter des points automatiquement sur la surface de la mesh ?  
    //TODO chaque point dois avoir de la velocity et du torque qui reste apres chaque push
#end

#macro SoftMesh_SpreadVelocity(Name, PointIndex, Vel)
    #local AlreadyVeled = array[1]{-1};
    #local ReduceForcePerStep = getFloatAttr(Name, "pushMultiplierByStep");
    SoftMesh_SpreadSpreadVelocity(Name, PointIndex, Vel, AlreadyVeled, ReduceForcePerStep)
#end

#macro SoftMesh_SpreadSpreadVelocity(Name, PointIndex, Force, AlreadyVeled, ReduceBy)
    #local OldVel = SoftMesh_GetPointVelocity(Name, PointIndex);
    SoftMesh_SetPointVelocity(Name, PointIndex, OldVel + Force)  //push point
    
    AddToArray(AlreadyVeled, PointIndex) //update array preventing infinite loops
    #declare Force = Force * ReduceBy; //reduce force  
                                       
    //pass force along                                       
    #local ConnectedPointIndexes = SoftMesh_GetConnectedVertsIndexes(Name, PointIndex);
    #local Size = dimension_size(ConnectedPointIndexes, 1);
    #for(I, 0, Size-1)
        #local IsPushed = ArrayContainsFloat(AlreadyVeled, ConnectedPointIndexes[I]);
        #if(!IsPushed)
            SoftMesh_SpreadSpreadVelocity(Name, ConnectedPointIndexes[I], Force, AlreadyVeled, ReduceBy)
        #end
    #end 
#end 

#macro SoftMesh_SpreadReduceVelocity(Name, PointIndex)
    #local AlreadyVeled = array[1]{-1};
    #local By = getFloatAttr(Name, "multiplyAdjacentPointVelOnBounce");
    #local ReduceForcePerStep = getFloatAttr(Name, "pushMultiplierByStep");
    SoftMesh_SpreadSpreadReduceVelocity(Name, PointIndex, By, AlreadyVeled, ReduceForcePerStep)
#end

#macro SoftMesh_SpreadSpreadReduceVelocity(Name, PointIndex, By, AlreadyVeled, ReduceBy)
    #local OldVel = SoftMesh_GetPointVelocity(Name, PointIndex);
    SoftMesh_SetPointVelocity(Name, PointIndex, OldVel*By)  //push point
    
    AddToArray(AlreadyVeled, PointIndex) //update array preventing infinite loops
    #declare By = By * ReduceBy; //reduce force  
                                       
    //pass force along                                       
    #local ConnectedPointIndexes = SoftMesh_GetConnectedVertsIndexes(Name, PointIndex);
    #local Size = dimension_size(ConnectedPointIndexes, 1);
    #for(I, 0, Size-1)
        #local IsPushed = ArrayContainsFloat(AlreadyVeled, ConnectedPointIndexes[I]);
        #if(!IsPushed)
            SoftMesh_SpreadSpreadReduceVelocity(Name, ConnectedPointIndexes[I], By, AlreadyVeled, ReduceBy)
        #end
    #end 
#end

//-----------------------------------------------------------------------------------------GET MESH INFOS
#macro SoftMesh_GetFaceVerts(Name, FaceIndex)
    #local TriArrName = concat(Name, ".triangles");
    #local VertArrName = concat(Name, ".vertex"); 
    #local TriCount = DiskArray_GetCount(TriArrName);
    
    #if(FaceIndex >= TriCount/3)
        #error "Face index out of bounds"
    #end
    #local VertIndex1 = DiskArray_GetFloat(TriArrName, FaceIndex*3);
    #local VertIndex2 = DiskArray_GetFloat(TriArrName, FaceIndex*3+1);
    #local VertIndex3 = DiskArray_GetFloat(TriArrName, FaceIndex*3+2);
    #local V1 = DiskArray_GetVect(VertArrName, VertIndex1);
    #local V2 = DiskArray_GetVect(VertArrName, VertIndex2);
    #local V3 = DiskArray_GetVect(VertArrName, VertIndex3);
    #local Return = array[3]{V1, V2, V3};
    Return
#end

#macro SoftMesh_GetFaceNormal(Name, FaceIndex)
     #local FaceVerts = SoftMesh_GetFaceVerts(Name, FaceIndex);
     #local Normal = vcross(FaceVerts[1] - FaceVerts[0], FaceVerts[2] - FaceVerts[0]);
     #local Normal = Normal/vlength(Normal);
     Normal;    
#end

#macro SoftMesh_GetFaceCenter(Name, FaceIndex)
     #local FaceVerts = SoftMesh_GetFaceVerts(Name, FaceIndex);
     #local Sum = FaceVerts[0] + FaceVerts[1] + FaceVerts[2];
     #local Return = Sum/3.0;
     Return;    
#end

#macro SoftMesh_GetMeshCenter(Name)
    #local VertArrName = concat(Name, ".vertex");
    #local VertCount = DiskArray_GetCount(VertArrName);
    #local Sum = 0;
    
    #for(I, 0, VertCount-1)
        #local V = DiskArray_GetVect(VertArrName, I);
        #local Sum = Sum + V;
    #end                     
    #local Sum = Sum/VertCount;
    Sum;
#end 

#macro SoftMesh_GetConnectedVertsIndexes(Name, VertIndex)
    //parcours triangles et a chaque fois qu'un des triangles a VertIndex dedans, ajouter 
    //les deux autres vertex du triangle a l'array Return, si l'array ne contient pas deja ce nombre
    #local TriArrName = concat(Name, ".triangles");
    #local TriCount = DiskArray_GetCount(TriArrName); 
    #local ReturnArr = array[1]{-1};
    
    #for(I, 0, TriCount/3 -1)
        #local VertIndex1 = DiskArray_GetFloat(TriArrName, I*3);
        #local VertIndex2 = DiskArray_GetFloat(TriArrName, I*3+1);
        #local VertIndex3 = DiskArray_GetFloat(TriArrName, I*3+2);
        //#debug concat("\ntri(", str(I/3, 0, 0), ")=[", str(VertIndex1, 0,0), ", ", str(VertIndex2, 0, 0), ", ", str(VertIndex3, 0, 0), "]\n")
        #local TmpArr = array[3]{VertIndex1, VertIndex2, VertIndex3};
        #local IsMyVertIndexHere = ArrayContainsFloat(TmpArr, VertIndex);
        
        #if(IsMyVertIndexHere)
            //le triangle qu'on regarde contient le vert qu'on cherche
            #for(K, 0, 2)
                //parcours la TmpArr
                #local AlreadyThere = ArrayContainsFloat(ReturnArr, TmpArr[K]);
                #if(!AlreadyThere & TmpArr[K] != VertIndex)
                    AddToArray(ReturnArr, TmpArr[K])
                #end
            #end    
        #end
    #end
    #local ReturnArr = RemoveFromArray(ReturnArr, 0)
    ReturnArr
    /*#local Size = dimension_size(ReturnArr, 1);
    #for(I, 1, Size-1)
        #debug concat("\nconnected:", str(ReturnArr[I], 0, 0), "\n")
    #end */
#end                      
                       
                       

#end //end SoftMesh.inc