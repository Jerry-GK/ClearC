#pragma once
#include <map>
#include <stack>
#include <string>
#include <iostream>
#include <vector>
#include "codegen.hpp"

//Cast a integer, or a floating-point number, or a pointer to bool
llvm::Value* CastToBool(llvm::Value* Value) {
	if (Value->getType() == GlobalBuilder.getInt1Ty())
		return Value;
	else if (Value->getType()->isIntegerTy())
		return GlobalBuilder.CreateICmpNE(Value, llvm::ConstantInt::get((llvm::IntegerType*)Value->getType(), 0, true));
	else if (Value->getType()->isFloatingPointTy())
		return GlobalBuilder.CreateFCmpONE(Value, llvm::ConstantFP::get(Value->getType(), 0.0));
	else if (Value->getType()->isPointerTy())
		return GlobalBuilder.CreateICmpNE(GlobalBuilder.CreatePtrToInt(Value, GlobalBuilder.getInt64Ty()), GlobalBuilder.getInt64(0));
	else {
		throw std::logic_error("Cannot cast to bool type");
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
		return CastToBool(Value);
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

//Upgrade two types to the same type.
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

llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* Func, std::string VarName, llvm::Type* VarType) {
	llvm::IRBuilder<> TmpB(&Func->getEntryBlock(), Func->getEntryBlock().begin());
	return TmpB.CreateAlloca(VarType, 0, VarName);
}

llvm::Value* CreateCmpEQ(llvm::Value* LeftOp, llvm::Value* RightOp) {
	if (TypeUpgrading(LeftOp, RightOp)) {
		if (LeftOp->getType()->isIntegerTy())
			return GlobalBuilder.CreateICmpEQ(LeftOp, RightOp);
		else
			return GlobalBuilder.CreateFCmpOEQ(LeftOp, RightOp);
	}
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
	throw std::logic_error("Comparison \"==\" between uncomparable types");
	return NULL;
}

llvm::BranchInst* TerminateBlockByBr(llvm::BasicBlock* BB) {
	//If not terminated, jump to the target block
	if (!GlobalBuilder.GetInsertBlock()->getTerminator())
		return GlobalBuilder.CreateBr(BB);
	else
		return NULL;
}

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
	throw std::logic_error("Addition between uncompatible types");
	return NULL;
}

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
	throw std::logic_error("Subtraction between uncompatible types");
	return NULL;
}

llvm::Value* CreateMul(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
		if (LeftOp.Value->getType()->isIntegerTy())
			return GlobalBuilder.CreateMul(LeftOp.Value, RightOp.Value);
		else
			return GlobalBuilder.CreateFMul(LeftOp.Value, RightOp.Value);
	}
	else {
		throw std::logic_error("Multiplication operator \"*\" can only be applied to integers or floating-point numbers");
		return NULL;
	}
}

llvm::Value* CreateDiv(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (TypeUpgrading(LeftOp.Value, RightOp.Value)) {
		if (LeftOp.Value->getType()->isIntegerTy())
			return GlobalBuilder.CreateSDiv(LeftOp.Value, RightOp.Value);
		else
			return GlobalBuilder.CreateFDiv(LeftOp.Value, RightOp.Value);
	}
	else {
		throw std::logic_error("Division operator \"/\" can only be applied to integers or floating-point numbers");
		return NULL;
	}
}

llvm::Value* CreateMod(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::logic_error("Modulo operator \"%\" can be applied between 2 integers");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateSRem(LeftOp.Value, RightOp.Value);
}

llvm::Value* CreateShl(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::logic_error("Left shifting operator \"<<\" can be applied between 2 integers");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateShl(LeftOp.Value, RightOp.Value);
}

llvm::Value* CreateShr(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::logic_error("Left shifting operator \"<<\" can be applied between 2 integers");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateAShr(LeftOp.Value, RightOp.Value);
}

llvm::Value* CreateBitwiseAND(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::logic_error("Bitwise AND operator \"&\" can be applied between 2 integers");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateAnd(LeftOp.Value, RightOp.Value);
}

llvm::Value* CreateBitwiseOR(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::logic_error("Bitwise OR operator \"|\" can be applied between 2 integers");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateOr(LeftOp.Value, RightOp.Value);
}

llvm::Value* CreateBitwiseXOR(ast::MyValue LeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (!(LeftOp.Value->getType()->isIntegerTy() && RightOp.Value->getType()->isIntegerTy())) {
		throw std::logic_error("Bitwise XOR operator \"^\" can be applied between 2 integers");
		return NULL;
	}
	TypeUpgrading(LeftOp.Value, RightOp.Value);
	return GlobalBuilder.CreateXor(LeftOp.Value, RightOp.Value);
}

//Create an assignment. This function will automatically do type casting
llvm::Value* CreateAssignment(ast::MyValue pLeftOp, ast::MyValue RightOp, CodeGenerator& Generator) {
	if (pLeftOp.Value->getType()->getNonOpaquePointerElementType()->isArrayTy())
	{
		throw std::logic_error("Array type (const ptr) variable cannot be assigned");
		return NULL;
	}
	if (pLeftOp.IsInnerConstPointer) {
		throw std::logic_error("Const variable cannot be assigned");
		return NULL;
	}

	//not allow ptr<const> assign to ptr<>
	if (pLeftOp.Value->getType()->getNonOpaquePointerElementType()->isPointerTy() && !pLeftOp.IsPointingToInnerConst &&
		RightOp.Value->getType()->isPointerTy() && RightOp.IsInnerConstPointer) {
		throw std::logic_error("Inner-const pointer cannot assign to a non-inner-const pointer");
		return NULL;
	}

	RightOp.Value = TypeCasting(RightOp, pLeftOp.Value->getType()->getNonOpaquePointerElementType(), Generator);
	if (RightOp.Value == NULL) {
		throw std::logic_error("Assignment with values that cannot be cast to the target type");
		return NULL;
	}
	GlobalBuilder.CreateStore(RightOp.Value, pLeftOp.Value);
	return pLeftOp.Value;
}

llvm::Value* CreateLoad(llvm::Value* pLeftOp, CodeGenerator& Generator) {
	if (pLeftOp->getType()->getNonOpaquePointerElementType()->isArrayTy())
		return GlobalBuilder.CreatePointerCast(pLeftOp, pLeftOp->getType()->getNonOpaquePointerElementType()->getArrayElementType()->getPointerTo());
	else
		return GlobalBuilder.CreateLoad(pLeftOp->getType()->getNonOpaquePointerElementType(), pLeftOp);
}