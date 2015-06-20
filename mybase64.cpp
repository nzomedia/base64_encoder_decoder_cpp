#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>
#include "mybase64.h"
#include <algorithm>


///////////////////////////////////////////////////
//              Base64 encoder/decoder by Youssouf
//
//              contact: nzomedia2@gmail.com
//
///////////////// Last edit: june 2015 ////////////
//	You're free to use it, if there are errors 
//      or something can be optimised please let me know.
////////////////////////////////////////////////////




std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode_24bits(const std::string word){
    std::string result;
    unsigned char c;
    if(word.length() != 3)
        return "";
    //1st 6bits
    c = word[0] >>2;
    result += base64_chars[c];
    
    //2nd
    c = word[0] <<6;
    c = c >> 2;
    c = c | word[1] >>4;
    result += base64_chars[c];
    

    //3rd
    c = word[1] <<4;
    c = c >> 2;
    c = c | word[2] >>6;
    result += base64_chars[c];

    //4th
    c = word[2] << 2;
    c = c >>2;
    result += base64_chars[c];

    return result;
}

bool is_base64_char(const unsigned char c){
    if(isalnum(c) || c == '=' || c == '/')
        return true;
    else
        return false;
}

bool are_base64_chars(const std::string str){
    int index = 0;
    for(; index < str.length(); ++index){
        if(!is_base64_char(str[index]))
            return false;
    }
    return true;
}

std::string base64_encode(const std::string src){
    std::string result;
    std::string mot; //group of 24bits (3octets)
    
    int src_len = src.length();
    int nb_24bit_word = src_len/3; //remains src_len%3 caracteres.
    int reste = src_len%3;

    int nb_mot = 0;
    auto begin = src.begin();
    auto end = src.begin() + 3;
    
    unsigned char c;
    unsigned int t;
    while(nb_mot < nb_24bit_word){
        mot = std::string(begin, end);
        
        //1
        c = mot[0] >>2;
        result += base64_chars[c];

        //2
        c = mot[0] <<6;
        c = c >> 2;
        c = c | mot[1] >>4;
        result += base64_chars[c];


        //3
        c = mot[1] <<4;
        c = c >> 2;
        c = c | mot[2] >>6;
        result += base64_chars[c];

        //4
        c = mot[2] << 2;
        c = c >>2;
        result += base64_chars[c];
        
        nb_mot++;
        begin = src.begin() + (nb_mot*3);
        end = src.begin() + (nb_mot*3 +3);
    }
    //on ajoute des padding au nombre de caracteres restant
    //pour en faire un paquet de 24bits (3octets):
    mot = std::string(begin, end);
    if(reste == 1){
        mot += "\0"; //on ajoute 8bits mais on utilisera que 4.
        
        //1
        c = mot[0] >>2;
        
        result += base64_chars[c];

        //2:
        c = (mot[0] << 6); //!!On ne doit pas enchainer les bitshifting
        c = c >> 2; //le resultat attendu ne sera pas celui obtenu meme avec des ().
        result += base64_chars[c];
        result.append("==");        
    }
    else if(reste == 2){
        mot += "\0"; //on utilisera les 2bits.
        //1
        c = mot[0] >>2;
        result += base64_chars[c];

        //2
        c = mot[0] <<6;
        c = c >> 2;
        c = c | mot[1] >>4;
        result += base64_chars[c];


        //3
        c = mot[1] <<4;
        c = c >> 2;
        c = c | mot[2] >>6;
        result += base64_chars[c];
        
        result += '=';
        
    }
    return result;
}


unsigned char get_base64_char_value(const unsigned char b){
    //Return the value associated to a base64 character.
    int i;
    for(i = 0; i < base64_chars.length(); ++i){
        if(b == base64_chars[i])
            return i;
    }
    return -1;
}


std::string base64_decode(std::string b64_str){

//We need to remove \n caracters:
b64_str.erase(remove(b64_str.begin(), b64_str.end(), '\n'), b64_str.end());

    if(b64_str.length() % 4 != 0)
        return "Incorrect base64 string length";
    
    std::string result;   
    std::string mot;

    auto begin = b64_str.begin();
    auto end = b64_str.begin()+4;
    int nb_mot = b64_str.length()/4;
    unsigned char c;
    unsigned char byte1, byte2, byte3, byte4;
    int test;
    for(int n = 0; n < nb_mot; ++n){
        mot = std::string(begin, end);
        if(!are_base64_chars(mot))
            return "Not-base64-char found";
        
        byte1 = get_base64_char_value(mot[0]);
        byte2 = get_base64_char_value(mot[1]) & 48;
        c = (byte1<<2) | (byte2 >> 4);
        result += c;
        
        byte1 = get_base64_char_value(mot[1]) << 4;
        if(mot[2] == '=') //on ignore le caractere de padding.
            byte2 = 0;
        else
            byte2 = get_base64_char_value(mot[2]) & 60;
        c = byte1 | (byte2 >> 2);
        result += c;
        
        if(mot[2] == '=') //this means that there's two '=' chars at the end,
            result += '\0'; //we ignore them.
        else{
            byte1 = get_base64_char_value(mot[2]) << 6;
            if(mot[3] == '=')
                byte2 = 0;
            else
                byte2 = get_base64_char_value(mot[3]);
            c = byte1 | byte2;
            result += c;
        }

        begin = end;
        end += 4;
    }
    
    return result;
}
