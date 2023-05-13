#pragma once
#include <map>
#include <stack>
#include <string>
#include <iostream>
#include <vector>
#include "codegen.hpp"

//Cast a integer, or a floating-point number, or a pointer to i1 integer.
//Return NULL if failed.
//This function is very useful when generating a condition value for "if", "while", "for" statements.
llvm::Value* Cast2I1(llvm::Value* Value) {
	if (Value->getType() == GlobalBuilder.getInt1Ty())
		return Value;
	else if (Value->getType()->isIntegerTy())
		return GlobalBuilder.CreateICmpNE(Value, llvm::ConstantInt::get((llvm::IntegerType*)Value->getType(), 0, true));
	else if (Value->getType()->isFloatingPointTy())
		return GlobalBuilder.CreateFCmpONE(Value, llvm::ConstantFP::get(Value->getType(), 0.0));
	else if (Value->getType()->isPointerTy())
		return GlobalBuilder.CreateICmpNE(GlobalBuilder.CreatePtrToInt(Value, GlobalBuilder.getInt64Ty()), GlobalBuilder.getInt64(0));
	else {
		throw std::logic_error("Cannot cast to bool type.");
		return NULL;
	}
}

//Type casting
//Supported:
//1. Int -> Int, Float, Pointer(val = 0)
//2. Float -> Int, Float
//3. Pointer -> Int, Pointer (same basetype)
//Other types are not supported, and will return NULL.
llvm::Value* TypeCasting(ast::MyValue MyVal, llvm::Type* Type, CodeGenerator& Gen) {
	llvm::Value* Value = MyVal.Value;
	if (Value->getType() == Type) {
		return Value;
	}
	else if (Type == GlobalBuilder.getInt1Ty()) {	//Int1 (bool) is special.
		return Cast2I1(Value);
	}
	else if (Value->getType()->isIntegerTy() && Type->isIntegerTy()) {
		return GlobalBuilder.CreateIntCast(Value, Type, !Value->getType()->isIntegerTy(1));
	}
	else if (Value->getType()->isIntegerTy() && Type->isFloatingPointTy()) {
		return Value->getType()->isIntegerTy(1) ?
			GlobalBuilder.CreateUIToFP(Value, Type) : GlobalBuilder.CreateSIToFP(Value, Type);
	}
	else if (Value->getType()->isIntegerTy() && Type->isPointerTy()) {
		if (!MyVal.IsZeroConstant) {
			throw std::logic_error("Type cast failed: pointer <- integer, integer can only be constant null (or 0)");
			return NULL;
		}
		return GlobalBuilder.CreateIntToPtr(Value, Type);
	}
	else if (Value->getType()->isFloatingPointTy() && Type->isIntegerTy()) {
		return GlobalBuilder.CreateFPToSI(Value, Type);
	}
	else if (Value->getType()->isFloatingPointTy() && Type->isFloatingPointTy()) {
		return GlobalBuilder.CreateFPCast(Value, Type);
	}
	else if (Value->getType()->isPointerTy() && Type->isIntegerTy()) {
		return GlobalBuilder.CreatePtrToInt(Value, Type);
	}
	else if (Value->getType()->isPointerTy() && Type->isPointerTy()) {
		return GlobalBuilder.CreatePointerCast(Value, Type);
	}
	else {
		//error msg
		throw std::logic_error("Type cast failed");
		return NULL;
	}
}

//Upgrade the type, given another type.
//1. int1
//2. int8
//3. int16
//4. int32
//5. int64
//6. float
//7. double
//Return NULL if failed.
//For example,
//	TypeUpgrading(int16, int32) -> int32
//	TypeUpgrading(int32, double) -> double
//	TypeUpgrading(int64, float) -> float
llvm::Value* TypeUpgrading(llvm::Value* Value, llvm::Type* Type) {
	if (Value->getType()->isIntegerTy() && Type->isIntegerTy()) {
		size_t Bit1 = ((llvm::IntegerType*)Value->getType())->getBitWidth();
		size_t Bit2 = ((llvm::IntegerType*)Type)->getBitWidth();
		if (Bit1 < Bit2)
			return GlobalBuilder.CreateIntCast(Value, Type, Bit1 != 1);
		else return Value;
	}
	else if (Value->getType()->isFloatingPointTy() && Type->isFloatingPointTy()) {
		if (Value->getType()->isFloatTy() && Type->isDoubleTy())
			return GlobalBuilder.CreateFPCast(Value, Type);
		else return Value;
	}
	else if (Value->getType()->isIntegerTy() && Type->isFloatingPointTy()) {
		return Value->getType()->isIntegerTy(1) ?
			GlobalBuilder.CreateUIToFP(Value, Type) : GlobalBuilder.CreateSIToFP(Value, Type);
	}
	else if (Value->getType()->isFloatingPointTy() && Type->isIntegerTy()) {
		return Value;
	}
	else return NULL;
}

//Upgrade two types at the same time.
//1. int1
//2. int8
//3. int16
//4. int32
//5. int64
//6. float
//7. double
//Return false if failed.
//For example,
//	TypeUpgrading(int16, int32) -> int32
//	TypeUpgrading(int32, double) -> double
//	TypeUpgrading(int64, float) -> float
bool TypeUpgrading(llvm::Value*& Value1, llvm::Value*& Value2) {
	if (Value1->getType()->isIntegerTy() && Value2->getType()->isIntegerTy()) {
		size_t Bit1 = ((llvm::IntegerType*)Value1->getType())->getBitWidth();
		size_t Bit2 = ((llvm::IntegerType*)Value2->getType())->getBitWidth();
		if (Bit1 < Bit2)
			Value1 = GlobalBuilder.CreateIntCast(Value1, Value2->getType(), Bit1 != 1);
		else if (Bit1 > Bit2)
			Value2 = GlobalBuilder.CreateIntCast(Value2, Value1->getType(), Bit2 != 1);
		return true;
	}
	else if (Value1->getType()->isFloatingPointTy() && Value2->getType()->isFloatingPointTy()) {
		if (Value1->getType()->isFloatTy() && Value2->getType()->isDoubleTy())
			Value1 = GlobalBuilder.CreateFPCast(Value1, GlobalBuilder.getDoubleTy());
		else if (Value1->getType()->isDoubleTy() && Value2->getType()->isFloatTy())
			Value2 = GlobalBuilder.CreateFPCast(Value2, GlobalBuilder.getDoubleTy());
		return true;
	}
	else if (Value1->getType()->isIntegerTy() && Value2->getType()->isFloatingPointTy()) {
		Value1 = Value1->getType()->isIntegerTy(1) ?
			GlobalBuilder.CreateUIToFP(Value1, Value2->getType()) : GlobalBuilder.CreateSIToFP(Value1, Value2->getType());
		return true;
	}
	else if (Value1->getType()->isFloatingPointTy() && Value2->getType()->isIntegerTy()) {
		Value2 = Value2->getType()->isIntegerTy(1) ?
			GlobalBuilder.CreateUIToFP(Value2, Value1->getType()) : GlobalBuilder.CreateSIToFP(Value2, Value1->getType());
		return true;
	}
	else return false;
}

//Create an alloca instruction in the entry block of the current function,
//wherever the variable declaration is.
//This function is useful when the variable declaration is not in the entry block of the function,
//e.g. in the body of a loop.
//In this case, this function can help prevent stack overflow.
llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* Func, std::string VarName, llvm::Type* VarType) {
	llvm::IRBuilder<> TmpB(&Func->getEntryBlock(), Func->getEntryBlock().begin());
	return TmpB.CreateAlloca(VarType, 0, VarName);
}

//Create an equal-comparison instruction. This function will automatically do type casting
//if the two input values are not of the same type.
llvm::Value* CreateCmpEQ(llvm::Value* LeftOp, llvm::Value* RightOp) {
	//Arithmatic compare
	if (TypeUpgrading(LeftOp, RightOp)) {
		if (LeftOp->getType()->isIntegerTy())
			return GlobalBuilder.CreateICmpEQ(LeftOp, RightOp);
		else
			return GlobalBuilder.CreateFCmpOEQ(LeftOp, RightOp);
	}
	//Pointer compare
	if (LeftOp->getType()->isPointerTy() && LeftOp->getType() == RightOp->getType()) {
		return GlobalBuilder.CreateICmpEQ(
			GlobalBuilder.CreatePtrToInt(LeftOp, GlobalBuilder.getInt64Ty()),
			GlobalBuilder.CreatePtrToInt(RightOp, GlobalBuilder.getInt64Ty())
		);
	}
	else if (LeftOp->getType()->isPointerTy() && RightOp->getType()->isIntegerTy()) {
		return GlobalBuilder.CreateICmpEQ(
			GlobalBuilder.CreatePtrToInt(LeftOp, GlobalBuilder.getInt64Ty()),
			TypeUpgrading(RightOp, GlobalBuilder.getInt64Ty())
		);
	}
	else if (LeftOp->getType()->isIntegerTy() && RightOp->getType()->isPointerTy()) {
		return GlobalBuilder.CreateICmpEQ(
			TypeUpgrading(LeftOp, GlobalBuilder.getInt64Ty()),
			GlobalBuilder.CreatePtrToInt(RightOp, GlobalBuilder.getInt64Ty())
		);
	}
	throw std::domain_error("Comparison \"==\" using unsupported type combination.");
	return NULL;
}

//Create an unconditional branch if the current block doesn't have a terminator.
//This function is safer than GlobalBuilder.CreateBr(llvm::BasicBlock* BB),
//because if the current block already has a terminator, it does nothing.
//For example, when generating if-statement, we create three blocks: ThenBB, ElseBB, MergeBB.
//At the end of ThenBB and ElseBB, an unconditional branch to MergeBB needs to be created respectively,
//UNLESS ThenBB or ElseBB is already terminated.
//e.g.
//	if (i) break;
//	else continue;
llvm::BranchInst* TerminateBlockByBr(llvm::BasicBlock* BB) {
	//If not terminated, jump to the target block
	if (!GlobalBuilder.GetInsertBlock()->getTerminator())
		return GlobalBuilder.CreateBr(BB);
	else
		return NULL;
}

//Create an addition instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int + Int -> Int			(TypeUpgrading)
//2. Int + FP -> FP				(TypeUpgrading)
//3. Int + Pointer -> Pointer
//4. FP + Int -> FP				(TypeUpgrading)
//2. FP + FP -> FP				(TypeUpgrading)
//3. Pointer + Int -> Pointer
llvm::Value* CreateAdd(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
		if (LeftOp.Value->getType()->isIntegerTy())
			return GlobalBuilder.CreateAdd(LeftOp.Value, RightOp.Value);
		else
			return GlobalBuilder.CreateFAdd(LeftOp.Value, RightOp.Value);
	}
	if (LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isPointerTy()) {
		auto TMP = LeftOp;
		LeftOp = RightOp;
		RightOp = TMP;
	}
	if (LeftOp.Value->getType()->isPointerTy() && RightOp.Value->getType()->isIntegerTy()) {
		return GlobalBuilder.CreateGEP(LeftOp.Value->getType()->getNonOpaquePointerElementType(), LeftOp.Value, RightOp.Value);
	}
	throw std::logic_error("Addition using unsupported type combination.");
	return NULL;
}

//Create a subtraction instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int - Int -> Int			(TypeUpgrading)
//2. Int - FP -> FP				(TypeUpgrading)
//3. FP - Int -> FP				(TypeUpgrading)
//4. FP - FP -> FP				(TypeUpgrading)
//5. Pointer - Int -> Pointer
//6. Pointer - Pointer -> Int64
llvm::Value* CreateSub(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
		if (LeftOp.Value->getType()->isIntegerTy())
			return GlobalBuilder.CreateSub(LeftOp.Value, RightOp.Value);
		else
			return GlobalBuilder.CreateFSub(LeftOp.Value, RightOp.Value);
	}
	if (LeftOp.Value->getType()->isPointerTy() && RightOp.Value->getType()->isIntegerTy()) {
		return GlobalBuilder.CreateGEP(LeftOp.Value->getType()->getNonOpaquePointerElementType(), LeftOp.Value, GlobalBuilder.CreateNeg(RightOp.Value));
	}
	if (LeftOp.Value->getType()->isPointerTy() && LeftOp.Value->getType() == RightOp.Value->getType())
		return GlobalBuilder.CreatePtrDiff(LeftOp.Value->getType()->getNonOpaquePointerElementType(), LeftOp.Value, RightOp.Value);
	throw std::logic_error("Subtraction using unsupported type combination.");
	return NULL;
}

//Create a multiplication instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int * Int -> Int			(TypeUpgrading)
//2. Int * FP -> FP				(TypeUpgrading)
//3. FP * Int -> FP				(TypeUpgrading)
//4. FP * FP -> FP				(TypeUpgrading)
llvm::Value* CreateMul(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
		if (LeftOp.Value->getType()->isIntegerTy())
			return GlobalBuilder.CreateMul(LeftOp.Value, RightOp.Value);
		else
			return GlobalBuilder.CreateFMul(LeftOp.Value, RightOp.Value);
	}
	else {
		throw std::logic_error("Multiplication operator \"*\" must only be applied to integers or floating-point numbers.");
		return NULL;
	}
}

//Create a division instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int / Int -> Int			(TypeUpgrading)
//2. Int / FP -> FP				(TypeUpgrading)
//3. FP / Int -> FP				(TypeUpgrading)
//4. FP / FP -> FP				(TypeUpgrading)
llvm::Value* CreateDiv(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
		if (LeftOp.Value->getType()->isIntegerTy())
			return GlobalBuilder.CreateSDiv(LeftOp.Value, RightOp.Value);
		else
			return GlobalBuilder.CreateFDiv(LeftOp.Value, RightOp.Value);
	}
	else {
		throw std::logic_error("Division operator \"/\" must only be applied to integers or floating-point numbers.");
		return NULL;
	}
}

//Create a modulo instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int % Int -> Int			(TypeUpgrading)
llvm::Value* CreateMod(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::domain_error("Modulo operator \"%\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateSRem(LeftOp.Value, RightOp.Value);
}

//Create a shl instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int << Int -> Int			(TypeUpgrading)
llvm::Value* CreateShl(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::domain_error("Left shifting operator \"<<\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateShl(LeftOp.Value, RightOp.Value);
}

//Create a shr instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int >> Int -> Int			(TypeUpgrading)
llvm::Value* CreateShr(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::domain_error("Left shifting operator \"<<\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateAShr(LeftOp.Value, RightOp.Value);
}

//Create a bitwise AND instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int & Int -> Int			(TypeUpgrading)
llvm::Value* CreateBitwiseAND(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::domain_error("Bitwise AND operator \"&\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateAnd(LeftOp.Value, RightOp.Value);
}

//Create a bitwise OR instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int | Int -> Int			(TypeUpgrading)
llvm::Value* CreateBitwiseOR(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::domain_error("Bitwise OR operator \"|\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateOr(LeftOp.Value, RightOp.Value);
}

//Create a bitwise XOR instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int ^ Int -> Int			(TypeUpgrading)
llvm::Value* CreateBitwiseXOR(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::domain_error("Bitwise XOR operator \"^\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateXor(LeftOp.Value, RightOp.Value);
}

//Create an assignment. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int = FP
//2. Int = Int
//3. Int = Pointer
//4. FP = Int
//5. FP = FP
//6. Pointer = Int
//7. Pointer = Pointer
//8. Exactly the same type assignment
//The "pLeftOp" argument should be a pointer pointing to the variable (the left-value in C)
llvm::Value* CreateAssignment(ast::MyValue pLeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (pLeftOp.Value->getType()->getNonOpaquePointerElementType()->isArrayTy())
	{
		throw std::domain_error("Array type (const ptr) variable cannot be assigned.");
		return NULL;
	}
	if (pLeftOp.IsInnerConstPointer) {
		throw std::domain_error("Const variable cannot be assigned.");
		return NULL;
	}

	//not allow ptr<const> assign to ptr<>
	if (pLeftOp.Value->getType()->getNonOpaquePointerElementType()->isPointerTy() && !pLeftOp.IsPointingToInnerConst &&
		RightOp.Value->getType()->isPointerTy() && RightOp.IsInnerConstPointer) {
		throw std::domain_error("Inner-const pointer cannot assign to a non-inner-const pointer.");
		return NULL;
	}

	RightOp.Value = TypeCasting(RightOp, pLeftOp.Value->getType()->getNonOpaquePointerElementType(), Generator);
	if (RightOp.Value == NULL) {
		throw std::domain_error("Assignment with values that cannot be cast to the target type.");
		return NULL;
	}
	GlobalBuilder.CreateStore(RightOp.Value, pLeftOp.Value);
	return pLeftOp.Value;
}

//Create a load instruction.
//This is different to GlobalBuilder.CreateLoad.
//If the argument is a pointer to an array, this function will
//return a pointer to its first element, instead of loading an array.
//This compiles with the C standard. For example:
//int a[10];
//int * b = a;	//When used as a right value, "a" is an integer pointer instead of an array. 
llvm::Value* CreateLoad(llvm::Value* pLeftOp, CodeGenerator& Generator) {
	//For array types, return the pointer to its first element
	if (pLeftOp->getType()->getNonOpaquePointerElementType()->isArrayTy())
		return GlobalBuilder.CreatePointerCast(pLeftOp, pLeftOp->getType()->getNonOpaquePointerElementType()->getArrayElementType()->getPointerTo());
	else
		return GlobalBuilder.CreateLoad(pLeftOp->getType()->getNonOpaquePointerElementType(), pLeftOp);
}