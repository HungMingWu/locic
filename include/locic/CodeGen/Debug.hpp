#ifndef LOCIC_CODEGEN_DEBUG_HPP
#define LOCIC_CODEGEN_DEBUG_HPP

#include <string>

#include <locic/CodeGen/LLVMIncludes.hpp>
#include <locic/Name.hpp>
#include <locic/SEM.hpp>

namespace locic {

	namespace CodeGen {
		
		struct DebugCompileUnit {
			std::string compilerName;
			std::string directoryName;
			std::string fileName;
			std::string flags;
		};
		
		class Module;
		
		class DebugBuilder {
			public:
				DebugBuilder(Module& module);
				~DebugBuilder();
				
				void finalize();
				
				llvm::DICompileUnit createCompileUnit(const DebugCompileUnit& compileUnit);
				
				llvm::DICompileUnit compileUnit() const;
				
				llvm::DIFile createFile(const std::string& fileName, const std::string& directory);
				
				llvm::DISubprogram convertFunction(llvm::DIFile file, unsigned int lineNumber, bool isDefinition, const Name& name, llvm::Function* function);
				
				llvm::DIVariable convertVar(SEM::Var* var, bool isParam);
				
			private:
				Module& module_;
				llvm::DIBuilder builder_;
			
		};
		
	}
	
}

#endif
