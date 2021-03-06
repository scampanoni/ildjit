#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// My headers
#include <ir_virtual_machine_backend.h>
#include <IRtoBCMaterializer.h>
// End

namespace llvm {

/// isMaterializable - True if GV can be materialized from whatever backing
/// store this GVMaterializer uses and has not been materialized yet.
bool IRtoBCMaterializer::isMaterializable (const GlobalValue *GV) const {
	const Function *f;
	f	= (const Function *)GV;
	assert(f != NULL);
	return f->empty();
}

/// Materialize - make sure the given GlobalValue is fully read.  On success,
/// return the success code. Otherwise, return a code describing the error.
///
error_code IRtoBCMaterializer::Materialize(GlobalValue *GV) {
	Function 	*f;
	IRVM_internal_t *llvmRoots;
	ir_method_t	*irMethod;
	void		*ilMethod;

	/* Fetch LLVM's top-level handles	*/
	llvmRoots = (IRVM_internal_t *) this->irVM->data;
	assert(llvmRoots != NULL);

	/* Fetch the LLVM function		*/
	f	= (Function *)GV;
	assert(f != NULL);

	/* Check if we have already generated the bitcode	*/
	if (!f->empty()){

		return error_code::success();
	}

	/* Fetch the IR method			*/
	irMethod	= (ir_method_t *)xanHashTable_lookup(llvmRoots->irMethods, f);
	if (irMethod == NULL){

		/* Ignore function stubs created by LLVM in the course of translating to machine code.	*
		 * These stubs have no corresponding IR methods.					*/
		return error_code::success();
	}
	assert(irMethod != NULL);

	/* Fetch the input language method	*/
	ilMethod	= xanHashTable_lookup(llvmRoots->ilMethods, irMethod);
	assert(ilMethod != NULL);

	/* Generate the LLVM bitcode		*/
	this->irVM->recompilerFunction(ilMethod, NULL, NULL);

	return error_code::success();
}

}
