#include <stdlib.h>
#include <stdio.h>

#include <Locic/List.h>
#include <Locic/AST/ClassDecl.h>
#include <Locic/AST/ClassDef.h>
#include <Locic/AST/File.h>

AST_File * AST_MakeFile(){
	AST_File * file = malloc(sizeof(AST_File));
	file->functionDeclarations = Locic_List_Alloc();
	file->functionDefinitions = Locic_List_Alloc();
	file->classDeclarations = Locic_List_Alloc();
	file->classDefinitions = Locic_List_Alloc();
	return file;
}

AST_File * AST_FileAddFunctionDecl(AST_File * file, AST_FunctionDecl * functionDecl){
	file->functionDeclarations = Locic_List_Append(file->functionDeclarations, functionDecl);
	return file;
}

AST_File * AST_FileAddFunctionDef(AST_File * file, AST_FunctionDef * functionDef){
	file->functionDefinitions = Locic_List_Append(file->functionDefinitions, functionDef);
	return file;
}

AST_File * AST_FileAddClassDecl(AST_File * file, AST_ClassDecl * classDecl){
	file->classDeclarations = Locic_List_Append(file->classDeclarations, classDecl);
	return file;
}

AST_File * AST_FileAddClassDef(AST_File * file, AST_ClassDef * classDef){
	file->classDefinitions = Locic_List_Append(file->classDefinitions, classDef);
	return file;
}

void AST_PrintFile(AST_File * file){
	Locic_ListElement * element;
	printf("----Class Declarations:\n");
	for(element = Locic_List_Begin(file->classDeclarations); element != Locic_List_End(file->classDeclarations); element = element->next){
		AST_ClassDecl * decl = (AST_ClassDecl *) element->data;
		AST_PrintClassDecl(decl);
	}
	printf("\n----Class Definitions:\n");
	for(element = Locic_List_Begin(file->classDefinitions); element != Locic_List_End(file->classDefinitions); element = element->next){
		AST_ClassDef * def = (AST_ClassDef *) element->data;
		AST_PrintClassDef(def);
	}
}

