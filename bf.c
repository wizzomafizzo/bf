#include <stdio.h>
#include <assert.h>

/*
  A Brainfuck program has an implicit byte pointer, called "the pointer", which
  is free to move around within an array of 30000 bytes, initially all set to
  zero. The pointer itself is initialized to point to the beginning of this
  array.

  The Brainfuck programming language consists of eight commands, each of which
  is represented as a single character.

  > 	Increment the pointer.
  < 	Decrement the pointer.
  + 	Increment the byte at the pointer.
  - 	Decrement the byte at the pointer.
  . 	Output the byte at the pointer.
  , 	Input a byte and store it in the byte at the pointer.
  [ 	Jump forward past the matching ] if the byte at the pointer is zero.
  ] 	Jump backward to the matching [ unless the byte at the pointer is zero.
*/

#define MEM_LENGTH 30000
#define LOOP_DEPTH 100

char *test_1 = // prints "Hello World!\n"
	"++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++."
	">>.<-.<.+++.------.--------.>>+.>++.";

char *test_2 = // prints "9 8 7 6 5 4 3 2 1 0 \n"
	"++++++++++++++++++++++++++++++++[>+>+<<-]>>+++++++++++++++++++++++++<<"
	"++++++++++[>>.-<.<-]++++++++++.";

char *test_3 = ",+.>,-.>,++.<<.>.>.>+++[.-]>++++++++++.";

int run(char *program)
{
	// pointer to current instruction in program
	unsigned int pp = 0;

	// block of memory for program to use
	unsigned char mem[MEM_LENGTH] = {0};
	// pointer to current byte in memory
	unsigned int mp = 0;

	// storage for active loops
	// when a loop begins, the address of the start of the loop in program
	// is stored and used to reset pp when the loop block ends. it acts as
	// a stack of start pointers to support nested loops
	unsigned int loop[LOOP_DEPTH] = {0};
	// keeps track of current loop level
	unsigned int lp = 0;

	// loop through instructions until end of program
	while (program[pp] != '\0') {
		// printf("I:%c *P:%d M:%d *M:%d L:%d *L:%d\n",
		//        program[pp], pp, mem[mp], mp, loop[lp], lp);

		switch (program[pp]) {
		case '>':
			// increment pointer
			assert(mp < (MEM_LENGTH - 1));
			mp++;
			break;
		case '<':
			// decrement pointer
			assert(mp > 0);
			mp--;
			break;
		case '+':
			// increment value at pointer
			mem[mp]++;
			break;
		case '-':
			// decrement value at pointer
			mem[mp]--;
			break;
		case '.':
			// print character from byte at pointer
			putchar(mem[mp]);
			break;
		case ',':
			// read character to byte at pointer
			mem[mp] = getchar();
			break;
		case '[':
			// start new loop if value at pointer > 0
			if (mem[mp]) {
				// push start to loop stack and proceed
				assert(lp < (LOOP_DEPTH - 1));
				loop[lp] = pp;
				lp++;
			} else {
				// skip past loop
				while (program[pp] != ']' &&
				       program[pp] != '\0') {
					pp++;
				}
				assert(program[pp] != '\0');
				pp++;
			}
			break;
		case ']':
			// end current loop if value at pointer == 0
			assert(lp > 0);
			if (mem[mp]) {
				// reset to start of loop
				pp = loop[lp - 1];
			} else {
				// pop loop stack and proceed
				loop[lp] = 0;
				lp--;
			}
			break;
		default:
			// unknown instructions are ignored
			break;
		}

		// next instruction!
		pp++;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	run(test_1);
	run(test_2);
	run(test_3);

	return 0;
}
