#include <string>
#include <vector>

#include <locic/String.hpp>

#include <locic/AST/Function.hpp>
#include <locic/AST/Node.hpp>
#include <locic/AST/TemplateTypeVar.hpp>
#include <locic/AST/TypeInstance.hpp>
#include <locic/AST/TypeVar.hpp>

namespace locic {

	namespace AST {
	
		TypeInstance::TypeInstance(TypeEnum e, const std::string& n,
								   const Node<TypeVarList>& v, const Node<FunctionList>& f)
			: typeEnum(e), name(n), templateVariables(makeDefaultNode<TemplateTypeVarList>()),
			  variables(v), functions(f) { }
			  
		TypeInstance* TypeInstance::Primitive(const std::string& name, const Node<FunctionList>& functions) {
			return new TypeInstance(PRIMITIVE, name, makeDefaultNode<TypeVarList>(), functions);
		}
		
		TypeInstance* TypeInstance::ClassDecl(const std::string& name, const Node<FunctionList>& functions) {
			return new TypeInstance(CLASSDECL, name, makeDefaultNode<TypeVarList>(), functions);
		}
		
		TypeInstance* TypeInstance::ClassDef(const std::string& name, const Node<TypeVarList>& variables, const Node<FunctionList>& functions) {
			return new TypeInstance(CLASSDEF, name, variables, functions);
		}
		
		TypeInstance* TypeInstance::Datatype(const std::string& name, const Node<TypeVarList>& variables) {
			return new TypeInstance(DATATYPE, name, variables, makeDefaultNode<FunctionList>());
		}
		
		TypeInstance* TypeInstance::Interface(const std::string& name, const Node<FunctionList>& functions) {
			return new TypeInstance(INTERFACE, name, makeDefaultNode<TypeVarList>(), functions);
		}
		
		TypeInstance* TypeInstance::Struct(const std::string& name, const Node<TypeVarList>& variables) {
			return new TypeInstance(STRUCT, name, variables, makeDefaultNode<FunctionList>());
		}
		
		std::string TypeInstance::toString() const {
			std::string s = makeString("TypeInstance[name = %s](", name.c_str());
			
			{
				s += "TemplateVariableList(";
				
				bool isFirst = true;
				
				for (auto node : *templateVariables) {
					if (!isFirst) {
						s += ", ";
					}
					
					isFirst = false;
					s += node.toString();
				}
				
				s += ")";
			}
			
			s += ", ";
			
			{
				s += "MemberVariableList(";
				
				bool isFirst = true;
				
				for (auto node : *variables) {
					if (!isFirst) {
						s += ", ";
					}
					
					isFirst = false;
					s += node.toString();
				}
				
				s += ")";
			}
			
			s += ", ";
			
			{
				s += "FunctionList(";
				
				bool isFirst = true;
				
				for (auto node : *functions) {
					if (!isFirst) {
						s += ", ";
					}
					
					isFirst = false;
					s += node.toString();
				}
				
				s += ")";
			}
			
			s += ")";
			return s;
		}
		
	}
	
}

