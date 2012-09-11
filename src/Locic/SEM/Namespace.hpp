#ifndef LOCIC_SEM_NAMESPACE_HPP
#define LOCIC_SEM_NAMESPACE_HPP

#include <string>
#include <Locic/Map.hpp>
#include <Locic/SEM/Function.hpp>
#include <Locic/SEM/TypeInstance.hpp>

namespace SEM{

	struct Namespace;

	struct NamespaceNode{
		enum TypeEnum{
			FUNCTION = 0,
			NAMESPACE,
			TYPEINSTANCE
		} typeEnum;
		
		union{
			Function * function;
			Namespace * nameSpace;
			TypeInstance * typeInstance;
		};
		
		inline NamespaceNode(TypeEnum e)
			: typeEnum(e){ }
		
		inline static NamespaceNode * Function(Function * function){
			NamespaceNode * node = new NamespaceNode(FUNCTION);
			node->function = function;
			return node;
		}
		
		inline static NamespaceNode * Namespace(Namespace * nameSpace){
			NamespaceNode * node = new NamespaceNode(NAMESPACE);
			node->nameSpace = nameSpace;
			return node;
		}
		
		inline static NamespaceNode * TypeInstance(TypeInstance * typeInstance){
			NamespaceNode * node = new NamespaceNode(TYPEINSTANCE);
			node->typeInstance = typeInstance;
			return node;
		}
		
		inline struct Function * getFunction(){
			return (typeEnum == FUNCTION) ? function : NULL;
		}
		
		inline struct Namespace * getNamespace(){
			return (typeEnum == NAMESPACE) ? nameSpace : NULL;
		}
		
		inline struct TypeInstance * getTypeInstance(){
			return (typeEnum == TYPEINSTANCE) ? typeInstance : NULL;
		}
	};
	
	struct Namespace{
		std::string name;
		Locic::StringMap<NamespaceNode *> children;
		
		inline Namespace(const std::string& n)
			: name(n){ }
	};

}

#endif