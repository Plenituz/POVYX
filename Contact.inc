#ifndef(CONTACT_INC_VAR)
#declare CONTACT_INC_VAR = 1;
       
#include "DiskArray.inc"    
    
#macro Contact_CreateContact(Name)
    #local ContactPointVarName = concat(Name, ".contactPoints")
    #local ContactNormalVarName = concat(Name, ".contactNormals")
    #local ContactPenetrationVarName = concat(Name, ".penetrations")
    DiskArray_CreateDiskArray(ContactPointVarName)
    DiskArray_CreateDiskArray(ContactNormalVarName)
    DiskArray_CreateDiskArray(ContactPenetrationVarName) 
#end                                                    

#macro Contact_GetContactPoint(Name, Index)
    #local ContactPointVarName = concat(Name, ".contactPoints")
    #local Vect = DiskArray_GetVect(ContactPointVarName, Index)
    Vect;
#end     

#macro Contact_AddContactPoint(Name, Point)
    #local ContactPointVarName = concat(Name, ".contactPoints")
    DiskArray_Add(ContactPointVarName, Point)
#end                                         

#macro Contact_GetContactPointCount(Name)
    #local ContactPointVarName = concat(Name, ".contactPoints")
    #local Count = DiskArray_GetCount(ContactPointVarName)
    Count;
#end      

#macro Contact_GetContactNormal(Name, Index)
    #local ContactNormalVarName = concat(Name, ".contactNormals")
    #local Vect = DiskArray_GetVect(ContactNormalVarName, Index)
    Vect;
#end     

#macro Contact_AddContactNormal(Name, Normal)
    #local ContactNormalVarName = concat(Name, ".contactNormals")
    DiskArray_Add(ContactNormalVarName, Normal)
#end
 
#macro Contact_GetContactPenetration(Name, Index)
    #local ContactPenetrationVarName = concat(Name, ".penetrations")
    #local Vect = DiskArray_GetFloat(ContactPenetrationVarName, Index)
    Vect;
#end     

#macro Contact_AddContactPenetration(Name, Val)
    #local ContactPenetrationVarName = concat(Name, ".penetrations")
    DiskArray_Add(ContactPenetrationVarName, Val)
#end
                                         

//contact point count 
//add contact point

#end // end Contact.inc