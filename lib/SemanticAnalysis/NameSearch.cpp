#include <assert.h>

#include <locic/Name.hpp>
#include <locic/SEM.hpp>

#include <locic/SemanticAnalysis/NameSearch.hpp>
#include <locic/SemanticAnalysis/ScopeStack.hpp>
#include <locic/SemanticAnalysis/SearchResult.hpp>

namespace locic {

	namespace SemanticAnalysis {
	
		SearchResult performFunctionSearch(SEM::Function* function, const Name& name, size_t pos) {
			const auto size = name.size() - pos;
			
			if (size == 0) return SearchResult::Function(function);
			
			return SearchResult::None();
		}
		
		SearchResult performTypeAliasSearch(SEM::TypeAlias* typeAlias, const Name& name, size_t pos) {
			const auto size = name.size() - pos;
			
			if (size == 0) return SearchResult::TypeAlias(typeAlias);
			
			return SearchResult::None();
		}
		
		SearchResult performTypeInstanceSearch(SEM::TypeInstance* typeInstance, const Name& name, size_t pos) {
			const auto size = name.size() - pos;
			
			if (size == 0) return SearchResult::TypeInstance(typeInstance);
			
			const auto canonicalName = CanonicalizeMethodName(name.at(pos));
			const auto iterator = typeInstance->functions().find(canonicalName);
			if (iterator != typeInstance->functions().end()) {
				const auto function = iterator->second;
				if (function->isStaticMethod()) {
					return performFunctionSearch(function, name, pos + 1);
				}
			}
			
			return SearchResult::None();
		}
		
		SearchResult performNamespaceSearch(SEM::Namespace* nameSpace, const Name& name, size_t pos) {
			const auto size = name.size() - pos;
			
			if (size == 0) return SearchResult::None();
			
			const auto iterator = nameSpace->items().find(name.at(pos));
			if (iterator != nameSpace->items().end()) {
				const auto& item = iterator->second;
				if (item.isFunction()) {
					return performFunctionSearch(item.function(), name, pos + 1);
				} else if (item.isNamespace()) {
					return performNamespaceSearch(item.nameSpace(), name, pos + 1);
				} else if (item.isTypeAlias()) {
					return performTypeAliasSearch(item.typeAlias(), name, pos + 1);
				} else if (item.isTypeInstance()) {
					return performTypeInstanceSearch(item.typeInstance(), name, pos + 1);
				}
			}
			
			return SearchResult::None();
		}
		
		SearchResult performInnerFunctionSearch(SEM::Function* function, const Name& name) {
			if (name.size() != 1 || name.isAbsolute()) return SearchResult::None();
			
			// Search template variables.
			{
				const auto iterator = function->namedTemplateVariables().find(name.at(0));
				if (iterator != function->namedTemplateVariables().end()) {
					return SearchResult::TemplateVar(iterator->second);
				}
			}
			
			// Search parameter variables.
			{
				const auto iterator = function->namedVariables().find(name.at(0));
				if (iterator != function->namedVariables().end()) {
					return SearchResult::Var(iterator->second);
				}
			}
			
			return SearchResult::None();
		}
		
		SearchResult performInnerTypeAliasSearch(SEM::TypeAlias* typeAlias, const Name& name) {
			if (name.size() != 1 || name.isAbsolute()) return SearchResult::None();
			
			const auto iterator = typeAlias->namedTemplateVariables().find(name.at(0));
			if (iterator != typeAlias->namedTemplateVariables().end()) {
				return SearchResult::TemplateVar(iterator->second);
			}
			
			return SearchResult::None();
		}
		
		SearchResult performInnerTypeInstanceSearch(SEM::TypeInstance* typeInstance, const Name& name) {
			if (name.size() != 1 || name.isAbsolute()) return SearchResult::None();
			
			const auto iterator = typeInstance->namedTemplateVariables().find(name.at(0));
			if (iterator != typeInstance->namedTemplateVariables().end()) {
				return SearchResult::TemplateVar(iterator->second);
			}
			
			return SearchResult::None();
		}
		
		SearchResult performInnerScopeSearch(SEM::Scope* scope, const Name& name) {
			if (name.size() != 1 || name.isAbsolute()) return SearchResult::None();
			
			const auto iterator = scope->namedVariables().find(name.at(0));
			if (iterator != scope->namedVariables().end()) {
				return SearchResult::Var(iterator->second);
			}
			
			return SearchResult::None();
		}
		
		SearchResult performInnerSwitchCaseSearch(SEM::SwitchCase* switchCase, const Name& name) {
			if (name.size() != 1 || name.isAbsolute()) return SearchResult::None();
			
			const auto iterator = switchCase->namedVariables().find(name.at(0));
			if (iterator != switchCase->namedVariables().end()) {
				return SearchResult::Var(iterator->second);
			}
			
			return SearchResult::None();
		}
		
		SearchResult performInnerCatchClauseSearch(SEM::CatchClause* catchClause, const Name& name) {
			if (name.size() != 1 || name.isAbsolute()) return SearchResult::None();
			
			const auto iterator = catchClause->namedVariables().find(name.at(0));
			if (iterator != catchClause->namedVariables().end()) {
				return SearchResult::Var(iterator->second);
			}
			
			return SearchResult::None();
		}
		
		SearchResult performInnerSearch(const ScopeElement& element, const Name& name) {
			if (element.isNamespace()) {
				return performNamespaceSearch(element.nameSpace(), name, 0);
			} else if (element.isFunction()) {
				return performInnerFunctionSearch(element.function(), name);
			} else if (element.isTypeAlias()) {
				return performInnerTypeAliasSearch(element.typeAlias(), name);
			} else if (element.isTypeInstance()) {
				return performInnerTypeInstanceSearch(element.typeInstance(), name);
			} else if (element.isScope()) {
				return performInnerScopeSearch(element.scope(), name);
			} else if (element.isSwitchCase()) {
				return performInnerSwitchCaseSearch(element.switchCase(), name);
			} else if (element.isCatchClause()) {
				return performInnerCatchClauseSearch(element.catchClause(), name);
			} else {
				return SearchResult::None();
			}
		}
		
		SearchResult performSearch(Context& context, const Name& name) {
			assert(!name.empty());
			
			const auto& scopeStack = context.scopeStack();
			
			const size_t startPosition = name.isAbsolute() ? scopeStack.size() - 1 : 0;
			for (size_t i = startPosition; i < scopeStack.size(); i++) {
				const auto pos = scopeStack.size() - i - 1;
				const auto result = performInnerSearch(scopeStack.at(pos), name);
				if (!result.isNone()) return result;
			}
			
			return SearchResult::None();
		}
		
	}
	
}

