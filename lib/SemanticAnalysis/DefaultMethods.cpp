#include <stdexcept>
#include <vector>

#include <locic/Constant.hpp>
#include <locic/Name.hpp>
#include <locic/SEM.hpp>

#include <locic/SemanticAnalysis/Context.hpp>
#include <locic/SemanticAnalysis/DefaultMethods.hpp>
#include <locic/SemanticAnalysis/Exception.hpp>
#include <locic/SemanticAnalysis/Lval.hpp>
#include <locic/SemanticAnalysis/Ref.hpp>
#include <locic/SemanticAnalysis/TypeProperties.hpp>

namespace locic {

	namespace SemanticAnalysis {
	
		SEM::Function* CreateDefaultConstructorDecl(Context& context, SEM::TypeInstance* typeInstance) {
			const bool isVarArg = false;
			const bool isMethod = true;
			const bool isStatic = true;
			const bool isConst = false;
			
			// Default constructor only moves, and since moves never
			// throw the constructor never throws.
			const bool isNoExcept = true;
			
			const auto constructTypes = typeInstance->constructTypes();
			
			std::vector<SEM::Var*> argVars;
			for (const auto constructType: constructTypes) {
				const bool isLvalConst = false;
				const auto lvalType = makeValueLvalType(context, isLvalConst, constructType);
				argVars.push_back(SEM::Var::Basic(constructType, lvalType));
			}
			
			const auto functionType = SEM::Type::Function(isVarArg, isNoExcept, typeInstance->selfType(), constructTypes);
			return SEM::Function::Decl(isMethod, isStatic, isConst, functionType, typeInstance->name() + "create", argVars, typeInstance->moduleScope());
		}
		
		SEM::Function* CreateDefaultImplicitCopyDecl(SEM::TypeInstance* typeInstance) {
			const bool isVarArg = false;
			const bool isMethod = true;
			const bool isStatic = false;
			const bool isConst = true;
			
			// Default copy constructor may throw since it
			// may call child copy constructors that throw.
			const bool isNoExcept = false;
			
			const auto functionType = SEM::Type::Function(isVarArg, isNoExcept, typeInstance->selfType(), {});
			return SEM::Function::Decl(isMethod, isStatic, isConst, functionType, typeInstance->name() + "implicitcopy", {}, typeInstance->moduleScope());
		}
		
		SEM::Function* CreateDefaultCompareDecl(Context& context, SEM::TypeInstance* typeInstance) {
			const bool isVarArg = false;
			const bool isMethod = true;
			const bool isStatic = false;
			const bool isConst = true;
			
			// Default compare method may throw since it
			// may call child compare methods that throw.
			const bool isNoExcept = false;
			
			const auto selfType = typeInstance->selfType();
			const auto intType = getBuiltInType(context.scopeStack(), "int_t")->selfType();
			const auto functionType = SEM::Type::Function(isVarArg, isNoExcept, intType, { selfType });
			const auto operandVar = SEM::Var::Basic(selfType, selfType);
			return SEM::Function::Decl(isMethod, isStatic, isConst, functionType, typeInstance->name() + "compare", { operandVar }, typeInstance->moduleScope());
		}
		
		SEM::Function* CreateDefaultMethodDecl(Context& context, SEM::TypeInstance* typeInstance, bool isStatic, const Name& name, const Debug::SourceLocation& location) {
			const auto canonicalName = CanonicalizeMethodName(name.last());
			if (canonicalName == "create") {
				if (!isStatic) {
					throw ErrorException(makeString("Default method '%s' must be static at position %s.",
						name.toString().c_str(), location.toString().c_str()));
				}
				
				return CreateDefaultConstructorDecl(context, typeInstance);
			} else if (canonicalName == "implicitcopy") {
				if (isStatic) {
					throw ErrorException(makeString("Default method '%s' must be non-static at position %s.",
						name.toString().c_str(), location.toString().c_str()));
				}
				
				return CreateDefaultImplicitCopyDecl(typeInstance);
			} else if (canonicalName == "compare") {
				if (isStatic) {
					throw ErrorException(makeString("Default method '%s' must be non-static at position %s.",
						name.toString().c_str(), location.toString().c_str()));
				}
				
				return CreateDefaultCompareDecl(context, typeInstance);
			}
			
			throw ErrorException(makeString("Unknown default method '%s' at position %s.",
				name.toString().c_str(), location.toString().c_str()));
		}
		
		bool HasDefaultImplicitCopy(SEM::TypeInstance* typeInstance) {
			if (typeInstance->isUnionDatatype()) {
				for (auto variantTypeInstance: typeInstance->variants()) {
					if (!HasDefaultImplicitCopy(variantTypeInstance)) {
						return false;
					}
				}
				return true;
			} else {
				for (auto var: typeInstance->variables()) {
					if (!supportsImplicitCopy(var->constructType())) {
						return false;
					}
				}
				return true;
			}
		}
		
		bool HasDefaultCompare(SEM::TypeInstance* typeInstance) {
			if (typeInstance->isUnionDatatype()) {
				for (auto variantTypeInstance: typeInstance->variants()) {
					if (!supportsCompare(variantTypeInstance->selfType())) {
						return false;
					}
				}
				return true;
			} else {
				for (auto var: typeInstance->variables()) {
					if (!supportsCompare(var->constructType())) {
						return false;
					}
				}
				return true;
			}
		}
		
		void CreateDefaultConstructor(SEM::TypeInstance* typeInstance, SEM::Function* function, const Debug::SourceLocation& location) {
			const auto functionScope = new SEM::Scope();
			
			assert(!typeInstance->isUnionDatatype());
			
			std::vector<SEM::Value*> constructValues;
			for (const auto argVar: function->parameters()) {
				const auto argVarValue = SEM::Value::LocalVar(argVar);
				constructValues.push_back(CallValue(GetMethod(argVarValue, "move", location), {}, location));
			}
			
			const auto internalConstructedValue = SEM::Value::InternalConstruct(typeInstance, constructValues);
			functionScope->statements().push_back(SEM::Statement::Return(internalConstructedValue));
			
			function->setScope(functionScope);
		}
		
		void CreateDefaultImplicitCopy(SEM::TypeInstance* typeInstance, SEM::Function* function, const Debug::SourceLocation& location) {
			const auto selfType = typeInstance->selfType();
			const auto selfValue = SEM::Value::Self(SEM::Type::Reference(selfType)->createRefType(selfType));
			
			const auto functionScope = new SEM::Scope();
			
			if (typeInstance->isUnionDatatype()) {
				std::vector<SEM::SwitchCase*> switchCases;
				for (const auto variantTypeInstance: typeInstance->variants()) {
					const auto variantType = variantTypeInstance->selfType();
					const auto caseVar = SEM::Var::Basic(variantType, variantType);
					const auto caseVarValue = SEM::Value::LocalVar(caseVar);
					
					const auto caseScope = new SEM::Scope();
					const auto copyResult = CallValue(GetMethod(caseVarValue, "implicitcopy", location), {}, location);
					const auto copyResultCast = SEM::Value::Cast(selfType, copyResult);
					caseScope->statements().push_back(SEM::Statement::Return(copyResultCast));
					
					switchCases.push_back(new SEM::SwitchCase(caseVar, caseScope));
				}
				functionScope->statements().push_back(SEM::Statement::Switch(derefAll(selfValue), switchCases));
			} else {
				std::vector<SEM::Value*> copyValues;
				
				for (const auto memberVar: typeInstance->variables()) {
					const auto selfMember = SEM::Value::MemberAccess(derefValue(selfValue), memberVar);
					const auto copyResult = CallValue(GetMethod(selfMember, "implicitcopy", location), {}, location);
					copyValues.push_back(copyResult);
				}
				
				const auto internalConstructedValue = SEM::Value::InternalConstruct(typeInstance, copyValues);
				functionScope->statements().push_back(SEM::Statement::Return(internalConstructedValue));
			}
			
			function->setScope(functionScope);
		}
		
		void CreateDefaultCompare(Context& context, SEM::TypeInstance* typeInstance, SEM::Function* function, const Debug::SourceLocation& location) {
			const auto selfType = typeInstance->selfType();
			const auto selfValue = SEM::Value::Self(SEM::Type::Reference(selfType)->createRefType(selfType));
			
			const auto intType = getBuiltInType(context.scopeStack(), "int_t")->selfType();
			
			const auto operandVar = function->parameters().at(0);
			const auto operandValue = SEM::Value::LocalVar(operandVar);
			
			const auto functionScope = new SEM::Scope();
			
			if (typeInstance->isUnionDatatype()) {
				std::vector<SEM::SwitchCase*> switchCases;
				for (size_t i = 0; i < typeInstance->variants().size(); i++) {
					const auto variantTypeInstance = typeInstance->variants().at(i);
					const auto variantType = variantTypeInstance->selfType();
					const auto caseVar = SEM::Var::Basic(variantType, variantType);
					const auto caseVarValue = SEM::Value::LocalVar(caseVar);
					
					const auto caseScope = new SEM::Scope();
					
					std::vector<SEM::SwitchCase*> subSwitchCases;
					for (size_t j = 0; j < typeInstance->variants().size(); j++) {
						const auto subVariantTypeInstance = typeInstance->variants().at(j);
						const auto subVariantType = subVariantTypeInstance->selfType();
						const auto subCaseVar = SEM::Var::Basic(subVariantType, subVariantType);
						const auto subCaseVarValue = SEM::Value::LocalVar(subCaseVar);
						
						const auto subCaseScope = new SEM::Scope();
						const auto plusOneConstant = SEM::Value::Constant(Constant::Integer(1), intType);
						if (i < j) {
							const auto minusOneConstant = CallValue(GetMethod(plusOneConstant, "minus", location), {}, location);
							subCaseScope->statements().push_back(SEM::Statement::Return(minusOneConstant));
						} else if (i > j) {
							subCaseScope->statements().push_back(SEM::Statement::Return(plusOneConstant));
						} else {
							// TODO: Either there needs to be an implicit copy here,
							// or 'compare' should accept its argument by reference.
							const auto compareResult = CallValue(GetMethod(caseVarValue, "compare", location), { subCaseVarValue }, location);
							subCaseScope->statements().push_back(SEM::Statement::Return(compareResult));
						}
						
						subSwitchCases.push_back(new SEM::SwitchCase(subCaseVar, subCaseScope));
					}
					
					caseScope->statements().push_back(SEM::Statement::Switch(derefAll(operandValue), subSwitchCases));
					
					switchCases.push_back(new SEM::SwitchCase(caseVar, caseScope));
				}
				
				functionScope->statements().push_back(SEM::Statement::Switch(derefAll(selfValue), switchCases));
			} else {
				auto currentScope = functionScope;
				
				for (const auto memberVar: typeInstance->variables()) {
					const auto selfMember = SEM::Value::MemberAccess(derefValue(selfValue), memberVar);
					const auto operandMember = SEM::Value::MemberAccess(derefValue(operandValue), memberVar);
					const auto compareResult = CallValue(GetMethod(selfMember, "compare", location), { operandMember }, location);
					const auto isZero = CallValue(GetMethod(compareResult, "iszero", location), {}, location);
					
					const auto ifTrueScope = new SEM::Scope();
					const auto ifFalseScope = new SEM::Scope();
					
					const auto ifStatement = SEM::Statement::If({ new SEM::IfClause(isZero, ifTrueScope) }, ifFalseScope);
					ifFalseScope->statements().push_back(SEM::Statement::Return(compareResult));
					currentScope->statements().push_back(ifStatement);
					
					currentScope = ifTrueScope;
				}
				
				const auto zeroConstant = SEM::Value::Constant(Constant::Integer(0), intType);
				currentScope->statements().push_back(SEM::Statement::Return(zeroConstant));
			}
			
			function->setScope(functionScope);
		}
		
		void CreateDefaultMethod(Context& context, SEM::TypeInstance* typeInstance, SEM::Function* function, const Debug::SourceLocation& location) {
			assert(function->isDeclaration());
			
			const auto& name = function->name();
			const auto canonicalName = CanonicalizeMethodName(name.last());
			if (canonicalName == "create") {
				assert(!typeInstance->isException());
				CreateDefaultConstructor(typeInstance, function, location);
			} else if (canonicalName == "implicitcopy") {
				if (!HasDefaultImplicitCopy(typeInstance)) {
					throw ErrorException(makeString("Default method '%s' cannot be generated because member types do not support it, at position %s.",
						name.toString().c_str(), location.toString().c_str()));
				}
				
				CreateDefaultImplicitCopy(typeInstance, function, location);
			} else if (canonicalName == "compare") {
				if (!HasDefaultCompare(typeInstance)) {
					throw ErrorException(makeString("Default method '%s' cannot be generated because member types do not support it, at position %s.",
						name.toString().c_str(), location.toString().c_str()));
				}
				
				CreateDefaultCompare(context, typeInstance, function, location);
			} else {
				throw std::runtime_error(makeString("Unknown default method '%s'.", name.toString().c_str()));
			}
		}
		
	}
	
}

