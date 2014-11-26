#include <assert.h>

#include <stdexcept>
#include <string>

#include <locic/AST.hpp>
#include <locic/Debug.hpp>
#include <locic/SEM.hpp>

#include <locic/SemanticAnalysis/Context.hpp>
#include <locic/SemanticAnalysis/ConvertType.hpp>
#include <locic/SemanticAnalysis/Exception.hpp>
#include <locic/SemanticAnalysis/NameSearch.hpp>
#include <locic/SemanticAnalysis/Template.hpp>

namespace locic {

	namespace SemanticAnalysis {
		
		void ConvertRequireExpr(Context& context, const AST::Node<AST::RequireExpr>& astRequireExprNode, SEM::TemplateRequireMap& requireMap) {
			const auto& location = astRequireExprNode.location();
			
			switch (astRequireExprNode->kind()) {
				case AST::RequireExpr::BRACKET: {
					ConvertRequireExpr(context, astRequireExprNode->bracketExpr(), requireMap);
					break;
				}
				case AST::RequireExpr::TYPESPEC: {
					const auto& typeSpecName = astRequireExprNode->typeSpecName();
					const auto& typeSpecType = astRequireExprNode->typeSpecType();
					
					const auto searchResult = performSearch(context, Name::Relative() + typeSpecName);
					if (!searchResult.isTemplateVar()) {
						throw ErrorException(makeString("Failed to find template var '%s' "
							"in require expression, at position %s.",
							typeSpecName.c_str(),
							location.toString().c_str()));
					}
					
					const auto semTemplateVar = searchResult.templateVar();
					const auto semSpecType = ConvertType(context, typeSpecType);
					
					addTypeToRequirement(context, requireMap.at(semTemplateVar), semSpecType);
					break;
				}
				case AST::RequireExpr::AND: {
					ConvertRequireExpr(context, astRequireExprNode->andLeft(), requireMap);
					ConvertRequireExpr(context, astRequireExprNode->andRight(), requireMap);
					break;
				}
			}
		}
		
		void ConvertRequireSpecifier(Context& context, const AST::Node<AST::RequireSpecifier>& astRequireSpecifierNode, SEM::TemplateRequireMap& requireMap) {
			switch (astRequireSpecifierNode->kind()) {
				case AST::RequireSpecifier::NONE:
					return;
				case AST::RequireSpecifier::EXPR:
				{
					ConvertRequireExpr(context, astRequireSpecifierNode->expr(), requireMap);
					return;
				}
			}
			
			throw std::logic_error("Unknown AST RequireSpecifier kind.");
		}
		
	}
	
}

