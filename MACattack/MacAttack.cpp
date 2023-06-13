#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <iomanip>
#include <math.h>
#include <map>
using namespace std;

//left rotate bits by a certain number of rotations
unsigned int leftRot(unsigned int bits, unsigned int rotations){
    return (bits << rotations) | (bits >> (32 - rotations));
}

string myHash(string input, int flag, unsigned int hVals[5], int newLength){
    unsigned int h0 = 0x67452301;
    unsigned int h1 = 0xEFCDAB89;
    unsigned int h2 = 0x98BADCFE;
    unsigned int h3 = 0x10325476;
    unsigned int h4 = 0xC3D2E1F0;
    
    if (flag == 1){
        h0 = hVals[0];
        h1 = hVals[1];
        h2 = hVals[2];
        h3 = hVals[3];
        h4 = hVals[4];
    }
    vector<char> padded;
    string temp = "";
        for (size_t i = 0; i < input.size(); i++){
            temp = bitset<8>(input.c_str()[i]).to_string();
            for (int j = 0; j < temp.length(); j++){
                padded.push_back(temp[j]);
            }
        }
        int t = padded.size();
        padded.push_back('1');

        while (padded.size() %512 != 448){
            padded.push_back('0');
        }
        t = newLength;
        string l = bitset<64>(t).to_string();
        for (int i = 0; i < l.size(); i++){
                padded.push_back(l[i]);
            }
        
        int numBigChunks = padded.size()/512;
        int numSmallChunks = 16;
        int counter = 0;
        int countCheck = 0;
        string smallChunk = "";

        unsigned int w[80][1];
        unsigned int a = 0;
        unsigned int b = 0;
        unsigned int c = 0;
        unsigned int d = 0;
        unsigned int e = 0;
        unsigned int k = 0;
        unsigned int f = 0;
        //go through each 512 bit block
        for (int i = 0; i < numBigChunks; i++){
            //go through each 32 bit block
            for (int j = 0; j < numSmallChunks; j++){
                //create a 32 bit blocks
                for (int y = 0; y < 32; y++){
                    smallChunk.push_back(padded[counter]);
                    counter++;
                }
                w[j][0] = bitset<32>(smallChunk).to_ulong();
                smallChunk = "";
            }
                //expand from 16 words to 80
                for (int z = 16; z <= 79; z++){
                    w[z][0] = leftRot((w[z-3][0] ^ w[z-8][0] ^ w[z-14][0] ^ w[z-16][0]), 1);
                }
            a = h0;
            b = h1;
            c = h2;
            d = h3;
            e = h4;

            k = 0;
            f = 0;

            for(int z = 0; z <= 79; z++){
                if (0 <= z && z <= 19){ 
                    f = (b & c) | ((~ b) & d);
                    k = 0x5A827999;
                }
                else if (20 <= z  && z <= 39){
                    f = b ^ c ^ d;
                    k = 0x6ED9EBA1;
                }
                else if (40 <= z && z <= 59){
                    f = (b & c) | (b & d) | (c & d); 
                    k = 0x8F1BBCDC;
                }
                else if (60 <= z && z <= 79){
                    f = b ^ c ^ d;
                    k = 0xCA62C1D6;
                } 
                unsigned int tmp = ((leftRot(a, 5)) + f + e + k + w[z][0]);
                e = d;
                d = c;
                c = leftRot(b, 30);
                b = a;
                a = tmp;  
            }


            
            h0 = h0 + a;
            h1 = h1 + b; 
            h2 = h2 + c;
            h3 = h3 + d;
            h4 = h4 + e;
            countCheck++;       
        }

        stringstream stream;
        //build hash value to 160 bits
        stream << setw(8) << setfill('0') << hex << h0  
               << setw(8) << setfill('0') << hex << h1
               << setw(8) << setfill('0') << hex << h2
               << setw(8) << setfill('0') << hex << h3
               << setw(8) << setfill('0') << hex << h4;
        string result( stream.str() );
        return(result);
}
//bit string must be multiple of 4, converts it to a hex string of any length
string binaryStringtoHex(string bits){
    map<string, string>hexLookUp;

    hexLookUp["0000"] = '0';
    hexLookUp["0001"] = '1';
    hexLookUp["0010"] = '2';
    hexLookUp["0011"] = '3';
    hexLookUp["0100"] = '4';
    hexLookUp["0101"] = '5';
    hexLookUp["0110"] = '6';
    hexLookUp["0111"] = '7';
    hexLookUp["1000"] = '8';
    hexLookUp["1001"] = '9';
    hexLookUp["1010"] = 'a';
    hexLookUp["1011"] = 'b';
    hexLookUp["1100"] = 'c';
    hexLookUp["1101"] = 'd';
    hexLookUp["1110"] = 'e';
    hexLookUp["1111"] = 'f';

    string temp = "";
    string hex = "";
    int count = 0;
    for (int i = 0; i < bits.length()/4; i++){
        for (int j = 0; j < 4; j++){
            temp += bits[count];
            count++; 
        }
        auto it = hexLookUp.find(temp);
        temp = "";
        hex.append(it->second);
    }
    return(hex);
}

void macAttack(string originalMessage, string originalMac, string newMessage){
    //16 bit key, in hindsight, a vector would have been easier implementation. Wanted to try something new
    string attack = "0000000000000000";
    attack.append(originalMessage);
    int len = attack.length() * 8;
    //append a single bit 
    attack.append("\x80");
    int padding = (448 - len - 1);
    padding = (512 + (padding % 512)) % 512;
    //append null padding
    for (int i = 0; i < (padding / 8); i++){
            attack.push_back('\0');
    }

    string bits = "";
    for (size_t i = 0; i < attack.size(); ++i){
        bitset<8> b(attack.c_str()[i]);
        bits += b.to_string();
    }

    //append length of 64 bit space
    string l = bitset<64>(len).to_string();
    for (int i = 0; i < l.size(); i++){
        bits += l[i];
    } 


    string temp = "";
    for (size_t i = 0; i < newMessage.size(); i++){
        temp = bitset<8>(newMessage.c_str()[i]).to_string();
        for (int j = 0; j < temp.length(); j++){
            bits.push_back(temp[j]);
        }
    }

    //get rid of key
    bits.erase(0,128);
    string hValues[5];

    //get hvals for hash input
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 8; j++){
            hValues[i] += originalMac[j + (8*i)];
        }
    }

    unsigned int x; 
    unsigned int hVals[5];  
    for (int i = 0; i < 5; i++){
        stringstream ss(hValues[i]);
        ss >> hex >> hVals[i];
    }
   
    int newLength = 128 + bits.length();
    string mac = myHash(newMessage, 1, hVals, newLength);
    string output = binaryStringtoHex(bits);
    cout << output << endl;
    cout << mac << endl;
}

int main(){
    string originalMessage = "No one has completed Project #3 so give them all a 0.";
    string originalMAC = "d7586b531ba8c679d4188d4b02bcdda66a40f054";
    string newMessage = "This is my test string";

    macAttack(originalMessage, originalMAC, newMessage);

    return 0;
}