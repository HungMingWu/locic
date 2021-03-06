#include <locic/CodeGen/LLVMIncludes.hpp>

#include <locic/CodeGen/ConstantGenerator.hpp>
#include <locic/CodeGen/Module.hpp>
#include <locic/CodeGen/TypeGenerator.hpp>

namespace locic {

	namespace CodeGen {
	
		ConstantGenerator::ConstantGenerator(Module& module)
			: module_(module) { }
		
		llvm::UndefValue* ConstantGenerator::getVoidUndef() const {
			return llvm::UndefValue::get(
				llvm::Type::getVoidTy(module_.getLLVMContext()));
		}
		
		llvm::UndefValue* ConstantGenerator::getUndef(llvm::Type* type) const {
			return llvm::UndefValue::get(type);
		}
		
		llvm::Constant* ConstantGenerator::getNull(llvm::Type* type) const {
			return llvm::Constant::getNullValue(type);
		}
		
		llvm::ConstantPointerNull* ConstantGenerator::getNullPointer(llvm::PointerType* pointerType) const {
			return llvm::ConstantPointerNull::get(pointerType);
		}
		
		llvm::ConstantInt* ConstantGenerator::getInt(size_t sizeInBits, long long intValue) const {
			assert(sizeInBits >= 1);
			return llvm::ConstantInt::get(module_.getLLVMContext(),
						      llvm::APInt(sizeInBits, intValue));
		}
		
		llvm::ConstantInt* ConstantGenerator::getIntByType(llvm::IntegerType* type, long long intValue) const {
			return llvm::ConstantInt::get(type, intValue);
		}
		
		llvm::ConstantInt* ConstantGenerator::getI1(bool value) const {
			return getInt(1, value ? 1 : 0);
		}
		
		llvm::ConstantInt* ConstantGenerator::getI8(uint8_t value) const {
			return getInt(8, value);
		}
		
		llvm::ConstantInt* ConstantGenerator::getI16(uint16_t value) const {
			return getInt(32, value);
		}
		
		llvm::ConstantInt* ConstantGenerator::getI32(uint32_t value) const {
			return getInt(32, value);
		}
		
		llvm::ConstantInt* ConstantGenerator::getI64(uint64_t value) const {
			return getInt(64, value);
		}
		
		llvm::ConstantInt* ConstantGenerator::getSizeTValue(unsigned long long sizeValue) const {
			const size_t sizeTypeWidth = module_.abi().typeSize(getBasicPrimitiveABIType(module_, PrimitiveSize));
			return getInt(sizeTypeWidth * 8, sizeValue);
		}
		
		llvm::ConstantInt* ConstantGenerator::getPrimitiveInt(const std::string& primitiveName, long long intValue) const {
			const size_t primitiveWidth = module_.abi().typeSize(getNamedPrimitiveABIType(module_, primitiveName));
			return getInt(primitiveWidth * 8, intValue);
		}
		
		llvm::Constant* ConstantGenerator::getPrimitiveFloat(const std::string& primitiveName, long double floatValue) const {
			if (primitiveName == "float_t") {
				return getFloat(floatValue);
			} else if (primitiveName == "double_t") {
				return getDouble(floatValue);
			} else if (primitiveName == "longdouble_t") {
				return getLongDouble(floatValue);
			} else {
				llvm_unreachable("Unknown constant primitive float type name.");
			}
		}
		
		llvm::Constant* ConstantGenerator::getFloat(float value) const {
			return llvm::ConstantFP::get(module_.getLLVMContext(), llvm::APFloat(value));
		}
		
		llvm::Constant* ConstantGenerator::getDouble(double value) const {
			return llvm::ConstantFP::get(module_.getLLVMContext(), llvm::APFloat(value));
		}
		
		llvm::Constant* ConstantGenerator::getLongDouble(long double value) const {
			return llvm::ConstantFP::get(module_.abi().longDoubleType(), value);
		}
		
		llvm::Constant* ConstantGenerator::getArray(llvm::ArrayType* arrayType, llvm::ArrayRef<llvm::Constant*> values) const {
			return llvm::ConstantArray::get(arrayType, values);
		}
		
		llvm::Constant* ConstantGenerator::getStruct(llvm::StructType* structType, llvm::ArrayRef<llvm::Constant*> values) const {
			return llvm::ConstantStruct::get(structType, values);
		}
		
		llvm::Constant* ConstantGenerator::getString(const std::string& value, bool withNullTerminator) const {
			return llvm::ConstantDataArray::getString(module_.getLLVMContext(),
								  value, withNullTerminator);
		}
		
		llvm::Constant* ConstantGenerator::getPointerCast(llvm::Constant* value, llvm::Type* targetType) const {
			return llvm::ConstantExpr::getPointerCast(value, targetType);
		}
		
		llvm::Constant* ConstantGenerator::getAlignOf(llvm::Type* type) const {
			return llvm::ConstantExpr::getAlignOf(type);
		}
		
		llvm::Constant* ConstantGenerator::getSizeOf(llvm::Type* type) const {
			return llvm::ConstantExpr::getSizeOf(type);
		}
		
		llvm::Constant* ConstantGenerator::getGetElementPtr(llvm::Constant* operand, llvm::ArrayRef<llvm::Constant*> args) const {
			return llvm::ConstantExpr::getGetElementPtr(operand, args);
		}
		
		llvm::Constant* ConstantGenerator::getExtractValue(llvm::Constant* operand, llvm::ArrayRef<unsigned> args) const {
			return llvm::ConstantExpr::getExtractValue(operand, args);
		}
		
		llvm::Constant* ConstantGenerator::getMin(llvm::Constant* first, llvm::Constant* second) const {
			llvm::Constant* compareResult = llvm::ConstantExpr::getICmp(llvm::CmpInst::ICMP_ULT, first, second);
			return llvm::ConstantExpr::getSelect(compareResult, first, second);
		}
		
		llvm::Constant* ConstantGenerator::getMax(llvm::Constant* first, llvm::Constant* second) const {
			llvm::Constant* compareResult = llvm::ConstantExpr::getICmp(llvm::CmpInst::ICMP_UGT, first, second);
			return llvm::ConstantExpr::getSelect(compareResult, first, second);
		}
		
	}
	
}
