/* hellovm.c - a kind of "hello world" for libjit
 *
 * Written by Norbert Bollow <nb@SoftwareEconomics.biz>
 *
 * This program is a JIT for the "Hello VM" bytecode language, which
 * has just one (string) register, and three opcodes: A) load string
 * constant into register, B) output contents of register, C) exit program.
 *
 * Compile with: gcc hellovm.c -ljit -o hellovm
 *
 * A valid "Hello VM" bytecode language program, suitable as input file for
 * this JIT, can be generated by the following perl script
 *
 * #!/usr/bin/perl
 * open TEST, ">test";
 * $bytecode="AHello World!\n\x00BC";
 * print TEST "HelloVM\x00", pack('i',length($bytecode)), $bytecode;
 *
 * A slightly less trivial example can be generated e.g. with
 * $bytecode="AHello, \x00BBAWorld!\n\x00BC";
 *
 * The contents of this file are in the Public Domain.
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <jit/jit.h>
char* readbytes (int, int);
static void hellovm_output (char *);
static void hellovm_exit ();
static void out_of_memory ();

int main (int argc, char *argv[]){
	int fd;
	char *buf;
	int len;
	jit_context_t context;
	const jit_type_t hellovm_output_arg_type = jit_type_void_ptr;
	jit_type_t hellovm_signature_main;
	jit_type_t hellovm_signature_exit, hellovm_signature_output;
	jit_type_t hellovm_type_string;
	jit_value_t hellovm_reg;
	jit_value_t tmp;
	char *str;
	jit_function_t hellovm_main;
	int pos;
	int willexit = 0;
	int retval;

	if (argc != 2) {
		printf("Usage: hellovm program\n");
		return 99;
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		perror(argv[1]);
		return 99;
	}

	/* read filetype identification string and length field */
	buf = readbytes(fd, 12);

	/* check filetype identification string */
	if (jit_strcmp("HelloVM", buf)!=0) {
		fprintf(stderr, "%s is not in HelloVM format.\n", argv[1]);
		return 99;
	}

	/* check length field and read bytecode data */
	len = *((int*) (buf+8));
	if (len<0) {
		fprintf(stderr, "%s: Invalid length field.\n", argv[1]);
		return 99;
	}
	free(buf);
	buf = readbytes(fd, len);
	close(fd);

	/* Now the fun begins :) */
	jit_init();
	context = jit_context_create();
	jit_context_build_start(context);
	/* Build signatures for output and exit functions */
	hellovm_signature_output = jit_type_create_signature
					   (jit_abi_cdecl, jit_type_void,
					   (jit_type_t*) &hellovm_output_arg_type, 1, 0);
	if (!hellovm_signature_output) {
		out_of_memory();
	}
	hellovm_signature_exit = jit_type_create_signature
					 (jit_abi_cdecl, jit_type_void, NULL, 0, 0);
	if (!hellovm_signature_exit) {
		out_of_memory();
	}
	/* There is always a single function with signature: int main() */
	hellovm_signature_main = jit_type_create_signature
					 (jit_abi_cdecl, jit_type_int, NULL, 0, 0);
	if (!hellovm_signature_main) {
		out_of_memory();
	}
	hellovm_main = jit_function_create(context, hellovm_signature_main);
	if (!hellovm_main) {
		out_of_memory();
	}
	/* The HelloVM has a single register holding a NUL-terminated string */
	hellovm_type_string = jit_type_create_pointer(jit_type_sys_char, 1);
	if (!hellovm_type_string) {
		out_of_memory();
	}
	hellovm_reg = jit_value_create(hellovm_main, hellovm_type_string);
	if (!hellovm_reg) {
		out_of_memory();
	}
	/* Initialize the string register with "" */
	tmp = jit_value_create_nint_constant
		      (hellovm_main, hellovm_type_string, (int) "");
	if (!tmp) {
		out_of_memory();
	}
	jit_insn_store(hellovm_main, hellovm_reg, tmp);
	/* Now JIT the supplied bytecodes */
	pos = 0;
	while (pos<len) {
		switch (buf[pos]) {
		    case 'A':
		    {
			    /* load string constant into hellovm_reg */
			    pos++;
			    str = jit_strndup(buf+pos, len-pos);
			    if (!str && buf[pos] && len-pos) {
				    out_of_memory();
			    }
			    tmp = jit_value_create_nint_constant
					  (hellovm_main, hellovm_type_string,
					  (int) str);
			    if (!tmp) {
				    out_of_memory();
			    }
			    jit_insn_store(hellovm_main, hellovm_reg, tmp);
			    pos += jit_strlen(str)+1;
		    }
		    break;
		    case 'B':
		    {
			    /* output contents of hellovm_reg */
			    pos++;
			    jit_insn_call_native
				    (hellovm_main, "hellovm_output",
				    (void *) hellovm_output,
				    hellovm_signature_output,
				    &hellovm_reg, 1,
				    JIT_CALL_NOTHROW);
		    }
		    break;
		    case 'C':
		    {
			    /* exit with exit value 0 */
			    pos++;
			    jit_insn_call_native
				    (hellovm_main, "hellovm_exit",
				    (void *) hellovm_exit,
				    hellovm_signature_exit,
				    NULL, 0,
				    JIT_CALL_NOTHROW|JIT_CALL_NORETURN);
			    willexit = 1;
		    }
		    break;
		    default:
		    {
			    fprintf(stderr, "Error: Illegal opcode\n");
			    return 99;
		    }
		}
	}
	if (willexit) {
		if (!jit_function_compile(hellovm_main)) {
			fprintf(stderr, "JIT compilation error\n");
			return 99;
		}
		jit_context_build_end(context);
		free(buf);
		if (!jit_function_apply(hellovm_main, NULL, &retval)) {
			fprintf(stderr, "Exception during execution\n");
			return 99;
		}
		/* not reached with the current instruction set */
		return retval;
	}else {
		jit_function_abandon(hellovm_main);
		fprintf(stderr, "Error: program without exit opcode\n");
		return 99;
	}
}

static void hellovm_output (char *value){
	fputs(value, stdout);
}

static void hellovm_nulput (char *value){
	putchar(0);
}

static void hellovm_exit (){
	exit(0);
}

static void out_of_memory (){
	fprintf(stderr, "Out of memory.\n");
	exit(99);
}

/* This function will not return unless the specified number of bytes
 * has been read successfully */
char* readbytes (int file, int bytestoread){
	char *buffer;
	int pos = 0;
	int bytesread;

	if ((buffer = jit_malloc(bytestoread)) == NULL) {
		perror("memory allocation error in readbytes()");
		exit(99);
	}

	while (pos<bytestoread) {
		bytesread = read(file, buffer+pos, bytestoread-pos);
		if (bytesread==-1) {
			perror("read error in readbytes()");
			exit(99);
		}
		if (bytesread==0) {
			fprintf(stderr, "unexpected eof in readbytes()\n");
			exit(99);
		}
		pos += bytesread;
	}
	return buffer;
}
