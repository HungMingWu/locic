#ifndef LOCIC_CODEGEN_HPP
#define LOCIC_CODEGEN_HPP

#include <cstddef>
#include <memory>
#include <string>

#include <locic/BuildOptions.hpp>
#include <locic/Debug.hpp>
#include <locic/SEM.hpp>

namespace locic {

	namespace CodeGen {
	
		class Module;
		
		class CodeGenerator {
			public:
				CodeGenerator(const std::string& moduleName, Debug::Module& debugModule, const BuildOptions& buildOptions);
				~CodeGenerator();
				
				Module& module();
				
				void applyOptimisations(size_t optLevel);
				
				void genNamespace(SEM::Namespace* nameSpace);
				
				void writeToFile(const std::string& fileName);
				
				void dumpToFile(const std::string& fileName);
				
				void dump();
				
			private:
				std::unique_ptr<Module> module_;
				
		};
		
	}
	
}

#endif
