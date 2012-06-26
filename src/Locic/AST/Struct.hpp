#ifndef LOCIC_AST_STRUCT_HPP
#define LOCIC_AST_STRUCT_HPP

#include <list>
#include <string>
#include <Locic/AST/Var.hpp>

namespace AST {

	struct Struct {
		std::string name;
		std::list<TypeVar *> variables;
		
		inline Struct(const std::string& n, const std::list<TypeVar *>& v)
			: name(n), variables(v) { }
	};
	
}

#endif