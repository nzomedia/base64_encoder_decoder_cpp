#ifndef BASE64_H
#define BASE64_H
#include <string>

//Test if a caracter is a base64 caracter:
bool is_base64_char(const unsigned char c);
//Test if a string only consists of base64 caracters:
bool are_base64_chars(const std::string str);

//Return the value associated to a base64 character.
unsigned char get_base64_char_value(const unsigned char b);

//Encode a string in base64 format:
std::string base64_encode(const std::string);
//Decode a base64 encoded string:
std::string base64_decode(std::string);

std::string base64_encode_24bits(const std::string word);
   
#endif
