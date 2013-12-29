#ifndef LOCIC_PARSER_DEFAULTPARSER_HPP
#define LOCIC_PARSER_DEFAULTPARSER_HPP

#include <cstdio>
#include <string>
#include <locic/AST.hpp>
#include <locic/Parser/Context.hpp>

namespace locic{

	namespace Parser{

		class DefaultParser{
			public:
				DefaultParser(AST::NamespaceList& rootNamespaceList, FILE * file, const std::string& fileName);
				~DefaultParser();
				
				bool parseFile();
				
				std::vector<Error> getErrors();
				
			private:
				FILE * file_;
				void * lexer_;
				Context context_;
				
		};
		
	}
	
}

#endif