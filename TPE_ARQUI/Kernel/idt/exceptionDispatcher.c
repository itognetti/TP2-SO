#include <videoDriver.h>
#include <registers.h>
#include <naiveConsole.h>
#include <colors.h>

#define ZERO_EXCEPTION 0
#define INVALID_OPERATION_CODE 6

static void zeroDivision();
static void invalidOperationCode();

void exceptionDispatcher(uint64_t exception, uint64_t *rsp) {
	switch(exception){
		case ZERO_EXCEPTION:
			zeroDivision();
			break;
		case INVALID_OPERATION_CODE:
			invalidOperationCode();
			break;
		}
	printRegistersState(rsp);
}

static void zeroDivision() {
	println("Error - divided by 0 Exception", RED);
}

static void invalidOperationCode(){
	println("Error - invalid operation code Exception", RED);
}
