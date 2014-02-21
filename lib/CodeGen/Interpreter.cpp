#include <locic/CodeGen/LLVMIncludes.hpp>

#include <stdexcept>
#include <string>
#include <vector>

#include <locic/CodeGen/Interpreter.hpp>
#include <locic/CodeGen/Module.hpp>

namespace locic {

	namespace CodeGen {
		
		Interpreter::Interpreter(Module& module)
			: module_(module),
			executionEngine_(NULL) {
				llvm::TargetOptions targetOptions;
				targetOptions.JITExceptionHandling = true;
				
				llvm::EngineBuilder engineBuilder(module.getLLVMModulePtr());
				
				engineBuilder.setEngineKind(llvm::EngineKind::JIT);
				engineBuilder.setTargetOptions(targetOptions);
				engineBuilder.setUseMCJIT(true);
				
				std::string errorString;
				executionEngine_ = engineBuilder.setErrorStr(&errorString).create();
				if (executionEngine_ == NULL) {
					throw std::runtime_error(std::string("Interpreter failed: Couldn't create execution engine with error: ") + errorString);
				}
			}
		
		Interpreter::~Interpreter() { }
		
		void* Interpreter::getFunctionPointer(const std::string& functionName) {
			llvm::Function* function = module_.getLLVMModule().getFunction(functionName);
			if (function == NULL) {
				throw std::runtime_error(std::string("Interpreter failed: No function '") + functionName + std::string("' exists in module."));
			}
			return executionEngine_->getPointerToFunction(function);
		}
		
		int Interpreter::runAsMain(const std::string& functionName, const std::vector<std::string>& args) {
			llvm::Function* function = module_.getLLVMModule().getFunction(functionName);
			if (function == NULL) {
				throw std::runtime_error(std::string("Interpreter failed: No function '") + functionName + std::string("' exists in module."));
			}
			return executionEngine_->runFunctionAsMain(function, args, NULL);
		}
		
	}
	
}

