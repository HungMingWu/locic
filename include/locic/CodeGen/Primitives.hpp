#ifndef LOCIC_CODEGEN_PRIMITIVES_HPP
#define LOCIC_CODEGEN_PRIMITIVES_HPP

#include <llvm-abi/Type.hpp>

#include <locic/CodeGen/LLVMIncludes.hpp>

#include <locic/SEM.hpp>
#include <locic/CodeGen/TargetInfo.hpp>

namespace locic {

	namespace CodeGen {
		
		enum PrimitiveKind {
			PrimitiveVoid,
			PrimitiveNull,
			PrimitiveBool,
			PrimitiveUnichar,
			PrimitiveCompareResult,
			
			PrimitiveInt8,
			PrimitiveUInt8,
			PrimitiveInt16,
			PrimitiveUInt16,
			PrimitiveInt32,
			PrimitiveUInt32,
			PrimitiveInt64,
			PrimitiveUInt64,
			
			PrimitiveByte,
			PrimitiveUByte,
			PrimitiveShort,
			PrimitiveUShort,
			PrimitiveInt,
			PrimitiveUInt,
			PrimitiveLong,
			PrimitiveULong,
			PrimitiveLongLong,
			PrimitiveULongLong,
			
			PrimitiveSize,
			PrimitiveSSize,
			
			PrimitivePtrDiff,
			
			PrimitiveFloat,
			PrimitiveDouble,
			PrimitiveLongDouble,
			PrimitiveRef,
			PrimitivePtr,
			PrimitivePtrLval,
			PrimitiveValueLval,
			PrimitiveMemberLval,
			PrimitiveTypename
		};
		
		class Function;
		class Module;
		
		bool isSignedIntegerType(Module& module, SEM::Type* type);
		
		bool isUnsignedIntegerType(Module& module, SEM::Type* type);
		
		void createPrimitiveAlignOf(Module& module, SEM::Type* type, llvm::Function& llvmFunction);
		
		void createPrimitiveSizeOf(Module& module, SEM::Type* type, llvm::Function& llvmFunction);
		
		void createPrimitiveMethod(Module& module, SEM::TypeInstance* typeInstance, SEM::Function* function, llvm::Function& llvmFunction);
		
		void createPrimitiveDestructor(Module& module, SEM::TypeInstance* typeInstance, llvm::Function& llvmFunction);
		
		void genPrimitiveDestructorCall(Function& function, SEM::Type* type, llvm::Value* value);
		
		llvm::Value* genTrivialPrimitiveFunctionCall(Function& function, SEM::Type* type, SEM::Function* semFunction, llvm::ArrayRef<SEM::Type*> templateArgs, llvm::ArrayRef<std::pair<llvm::Value*, bool>> args);
		
		void genStorePrimitiveLval(Function& functionGenerator, llvm::Value* value, llvm::Value* var, SEM::Type* varType);
		
		llvm::Value* genPrimitiveAlignMask(Function& function, SEM::Type* type);
		
		llvm::Value* genPrimitiveSizeOf(Function& function, SEM::Type* type);
		
		llvm::Type* getPrimitiveType(Module& module, SEM::Type* type);
		
		llvm::Type* getNamedPrimitiveType(Module& module, const std::string& name);
		
		llvm::Type* getBasicPrimitiveType(Module& module, PrimitiveKind kind, const std::string& name);
		
		llvm_abi::Type* getPrimitiveABIType(Module& module, SEM::Type* type);
		
		bool primitiveTypeHasDestructor(Module& module, SEM::Type* type);
		
		bool primitiveTypeInstanceHasDestructor(Module& module, SEM::TypeInstance* typeInstance);
		
		bool isPrimitiveTypeSizeAlwaysKnown(Module& module, SEM::Type* type);
		
		bool isPrimitiveTypeSizeKnownInThisModule(Module& module, SEM::Type* type);
		
		bool needsLivenessIndicator(Module& module, SEM::Type* type);
		
	}
	
}

#endif
