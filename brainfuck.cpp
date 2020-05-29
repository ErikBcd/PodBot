#include <stdio.h>
#include <string.h>
#include <iostream>
#include <regex>
#include <vector>

// TODO: Better comments, clean up this mess (aka merge these massive switches into a single thing)
using namespace std;


class Brainfuck
{
    
    public:
        static vector<int> cells;
        static int ptr;
	
	/* Interprets a given Brainfuck sequence and returns a string containing all the prints */
        static string interpret(string input) {
	    // We only need the brainfuck related commands from the input string
            regex br("[^<>+\-.\,\\[\\]]");
            input = regex_replace(input, br, "");
	    // Initialize the cells
            for (int i = 0; i < input.length(); i++) {
                cells.push_back(0);
            }

            Brainfuck::ptr = 0;

            string out = "";
	    /* 
	     * The main interpreter loop:
	     * Go through each bf operation and perform it 
	     * on the cells and the output string.
	     */
            for (int i = 0; input[i] != '\0'; i++) {

                switch(input[i]) {
                	case '+':
				// Increment the value of the current cell
				Brainfuck::cells[Brainfuck::ptr]++;
				break;
			case '-':
				// Decrement the value of the current cell
				Brainfuck::cells[Brainfuck::ptr]--;
    				break;
		    	case '<':
				// Move to the cell left of the current cell
			    	Brainfuck::ptr--;
    				break;
	    		case '>':
				// Move to the cell right of the current cell
		    		Brainfuck::ptr++;
			    	break;
    			case '.':
				// Add the value of the current cell 
				// as a character to the output string
	    			out += (char) Brainfuck::cells[Brainfuck::ptr];
		    		break;
    			case ',':
				// Wait for user input
				// TODO: Implement this..... somehow.
	    			return "Currently not supported! Working on it!";
                	case '[':
				// The start of a loop.
                    		{
                        		int eoL = getEndOfLoop(input, i);
                        		if (eoL == -1) {
                            		return string("Error! Loop at Position `%d` is never closed!", i);
                       	 	}
                        	out += Brainfuck::loop(input, eoL, i);
                        	i = eoL;
                        	break;
                    }
                }
            }
            Brainfuck::cells.clear();
            Brainfuck::ptr = 0;
            return out;
        }
    private:
        
	/* 
	 * Implementation of the brainfuck loops 
	 * Will loop throgh the given code inside the boundaries defined by
	 * loopStop and loopStart until the value of the current cell is 0 
	 * and the end of the loop is reached.
	 */
        static string loop(string index, const int loopStop, const int loopStart) {
            string out = "";
            int i = loopStart +1;

            while (true) {
                if (i == loopStop && cells[ptr] == 0) {
                    return out;
                }
                switch(index[i]) {
                case '+':
                    Brainfuck::cells[Brainfuck::ptr]++;
                    break;
                case '-':
                    Brainfuck::cells[Brainfuck::ptr]--;
                    break;
                case '<':
                    Brainfuck::ptr--;
                    break;
                case '>':
                    Brainfuck::ptr++;
                    break;
                case '.':
                    out += (char) Brainfuck::cells[Brainfuck::ptr];
                    break;
                case ',':
                    return "`,` operator currently not supported! Working on it!";
                case '[':
                    {
                        int eoL = getEndOfLoop(index, i);
                        if (eoL == -1) {
                            return string("Error! Loop at Position `%d` is never closed!", i);
                        }
                        out += loop(index, eoL, i);
                        i = eoL;
                        break;
                    }
                case ']':
                    if (i == loopStop) {
                        if (cells[ptr] == 0) {
                            return out;
                        } else {
                            i = loopStart;
                        }
                    }
                    break;
                }
                i++;
            }
            return out;
        }
	
	/* 
	 * Searches for the end of the current loop and returns the index of it.
	 */
        static int getEndOfLoop(string code, int index) {
            int i = index + 1;
            int depth = 0;
            while (true) {
                if (code[i] == '[') {
                    depth++;
                } else if (code[i] == ']') {
                    if (depth == 0) {
                        return i;
                    } else {
                        depth--;
                    }
                } else if (code[i] == '\0') {
                    return -1;
                }
                i++;
            }
        }
};

vector<int> Brainfuck::cells;
int::Brainfuck::ptr = 0;
