#include "Interpreter.h"

using namespace std;

int main(int argc, char* argv[]){
	ifstream read;
	read.open(argv[1]);

	ofstream write;
	write.open(argv[2]);

	Scanner scanner;
	bool success = scanner.scan(read);

	if (success){
		Parser parser(scanner.getTokens());
		success = parser.parse();
		if (success){
			Interpreter interpreter;
			interpreter.interpret(parser.getDataLog(), write);
		}
	}

	read.close();
	write.close();

	return 0;
}