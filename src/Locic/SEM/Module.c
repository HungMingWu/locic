#include <stdlib.h>
#include <stdio.h>
#include <Locic/AST/ClassDecl.h>
#include <Locic/AST/ClassDef.h>
#include <Locic/AST/File.h>
#include <Locic/List.h>

AST_File * AST_MakeFile(){
	AST_File * file = malloc(sizeof(AST_File));
	file->functionDeclarations = AST_ListCreate();
	file->functionDefinitions = AST_ListCreate();
	file->classDeclarations = AST_ListCreate();
	file->classDefinitions = AST_ListCreate();
	return file;
}

AST_File * AST_FileAddFunctionDecl(AST_File * file, AST_FunctionDecl * functionDecl){
	file->functionDeclarations = AST_ListAppend(file->functionDeclarations, functionDecl);
	return file;
}

AST_File * AST_FileAddFunctionDef(AST_File * file, AST_FunctionDef * functionDef){
	file->functionDefinitions = AST_ListAppend(file->functionDefinitions, functionDef);
	return file;
}

AST_File * AST_FileAddClassDecl(AST_File * file, AST_ClassDecl * classDecl){
	file->classDeclarations = AST_ListAppend(file->classDeclarations, classDecl);
	return file;
}

AST_File * AST_FileAddClassDef(AST_File * file, AST_ClassDef * classDef){
	file->classDefinitions = AST_ListAppend(file->classDefinitions, classDef);
	return file;
}

void AST_PrintFile(AST_File * file){
	AST_ListElement * element;
	printf("----Class Declarations:\n");
	for(element = AST_ListBegin(file->classDeclarations); element != AST_ListEnd(file->classDeclarations); element = element->next){
		AST_ClassDecl * decl = (AST_ClassDecl *) element->data;
		AST_PrintClassDecl(decl);
	}
	printf("\n----Class Definitions:\n");
	for(element = AST_ListBegin(file->classDefinitions); element != AST_ListEnd(file->classDefinitions); element = element->next){
		AST_ClassDef * def = (AST_ClassDef *) element->data;
		AST_PrintClassDef(def);
	}
}
