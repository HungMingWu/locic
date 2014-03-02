#ifndef LOCIC_CODEGEN_GENTYPE_HPP
#define LOCIC_CODEGEN_GENTYPE_HPP

#include <locic/SEM.hpp>

#include <locic/CodeGen/Module.hpp>

namespace locic {

	namespace CodeGen {
		
		llvm::FunctionType* genFunctionType(Module& module, SEM::Type* type, llvm::Type* contextPointerType = NULL);
		
		llvm::Type* getTypeInstancePointer(Module& module, SEM::TypeInstance* typeInstance,
			const std::vector<SEM::Type*>& templateArguments);
	
		llvm::Type* genType(Module& module, SEM::Type* type);
		
		llvm::DIType genDebugType(Module& module, SEM::Type* unresolvedType);
		
	}
	
}

#endif
