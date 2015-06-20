#include <iostream>
#include "mybase64.h"
#include <cstdlib>
#include <unistd.h>  //For getopt(): this function handles command line arguments.

using namespace std;

///////////////////////////////////////////////////
//		Base64 encoder/decoder by Youssouf
//
//		contact: nzomedia2@gmail.com
//
///////////////// Last edit: june 2015 ///////
//
////////////////////////////////////////////////////

extern char *optarg;  //Used with getopt() to get an option's value.
extern int opterr;

void printUsage(char *programName){
	cerr << "Encode/decode Base64\nErreur: Usage " << programName << " [-e|-d] <Chaine de caracteres>\n Options:\n -e: Encoder en base64.\n -d: Decoder une chaine codée en base64.\n";
}

int main(int argc, char **argv){

    string src;  //Encoding/decoding functions use a string as input parameter.
    opterr = 0; // To prevent getopt() from printing its error message.
    char cli_option = getopt(argc, argv, "e:d:");

    switch(cli_option){
	case 'e':{ //encode
		src = optarg;
    		string res = base64_encode(src);
    		cout << res << endl;
		break;
	}
	case 'd':{ //decode
		src = optarg;
    		string res = base64_decode(src);
    		cout << res << endl;
		break;
	}
	default:{ //If we dont know the option or there were no option.
		printUsage(argv[0]);
		exit(2);
	}
	};
    return 0;
    }
