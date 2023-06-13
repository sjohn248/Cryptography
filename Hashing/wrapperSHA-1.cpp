#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <fstream>
#include <ctime>
#include <map>
#include <set>


using namespace std;

class sha1Algo{
    public:
        string plainText = "";
        string hash = "";
        string myHash(string input);

        string getplainText(){ 
            return this->plainText;
        }
        string getHash(){
            return this->hash;
        }

        void setHash(){
            this->hash = myHash(plainText);
            return;
        }
};

//converts sha1Algo hash output to certain bit lengths
class wrapper {
    public:
        int bits = 0;
        string plainText = ""; 
        wrapper(string input, string hash, int bits){
            m_sha1Algo.plainText = input;
            m_sha1Algo.hash = hash;
            this->bits = bits;
        }
        
        string truncateHash(){
            string hash = m_sha1Algo.getHash();
            //cout << hash;
            //number of chars to take from has value. 4 bits per hex char
            int numChars = 0;
            int shiftCount = 0;
            int multiple = 4;

            //Get number of hex characters to keep and how bytes to shift
            if (this->bits %4 == 0){
                numChars = this->bits/4;
            }
            else{
                numChars = floor(this->bits/4);
                numChars++;
                shiftCount = this->bits % multiple;
                shiftCount = this->bits + multiple - shiftCount;
                shiftCount = shiftCount - this->bits;

            }
            
            char truncated[numChars+1];
            for (int i = 0; i < numChars; i++){
                truncated[i] = hash[i];
            }
            truncated[numChars] = '\0';

            int val = strtol(truncated, 0, 16);
            val = val >> shiftCount;

            stringstream stream;
            stream << setw(8) << setfill('0') << hex << val;
            string result( stream.str() );
            return(result);
        }
        
    private:
        sha1Algo m_sha1Algo;
};
//left rotate bits by a certain number of rotations
unsigned int leftRot(unsigned int bits, unsigned int rotations){
    return (bits << rotations) | (bits >> (32 - rotations));
}

string sha1Algo::myHash(string input){
    unsigned int h0 = 0x67452301;
    unsigned int h1 = 0xEFCDAB89;
    unsigned int h2 = 0x98BADCFE;
    unsigned int h3 = 0x10325476;
    unsigned int h4 = 0xC3D2E1F0;
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
//Generates a random string of numbers, uppercase letters, and lowercase letters of size len
string createRandomString(const int len) {
    static const char chars[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string randomStr;
    randomStr.reserve(len);

    for (int i = 0; i < len; ++i) {
        randomStr += chars[rand() % (sizeof(chars) - 1)];
    }
    return randomStr;
}
//Conducts a collision attack by hashing random strings and adding them to a unique set. Once a hash that cannot be added to the set is found, a collision has occured. 
void colAttack(){
    sha1Algo hash;
    map<string,string>::iterator it;
    map<string, string> colAttack;
    set<string> randomStrings;
    string text = "";
    int attempts = 0;
    double average = 0;

    srand((unsigned)time(NULL) * getpid());

    ofstream file;
    file.open("CollisionResults.txt");
    int bitSizes[8] = {8, 10, 12, 14, 16, 17, 18, 19};
    for (int j = 0; j < 8; j++){
        file << "BITS:" << bitSizes[j] << endl;
        for(int i = 0; i < 50; i++){
            file << "ROUND:" << i+1 << endl;
            //reset map to empty
            colAttack = map<string, string>();
            randomStrings.clear();
            while(1){
                //Don't want empty strings
                int length = (rand() % 30) + 5;
                text = createRandomString(length);

                //if random string is unique
                if (get<1>(randomStrings.insert(text)) == true ){
                    attempts++;
                    hash.plainText = text;
                    hash.setHash();
                    wrapper wrap(hash.getplainText(), hash.getHash(), bitSizes[j]);
                    string hashed = wrap.truncateHash();
                    it = colAttack.find(hashed);

                    //check for collision
                    if (it != colAttack.end()){
                        file <<  "ATTEMPTS:" << attempts << endl;
                        break;
                    }
                    colAttack.insert(pair<string, string> (hashed, text));
                }
            }
            attempts = 0;
        }
    }
    file << "END\n";
    file.close();
}
//Conducts a 400 preimage attacks on specific bit lengths by hashing a random string, then comparing new random string hashes until one matches the original.
void preImageAttack(){
    sha1Algo hash;
    set<string> randomStrings;

    string text = "";
    int attempts = 0;
    double average = 0;
    srand((unsigned)time(NULL) * getpid());

    ofstream file;
    file.open("PreImageResults.txt");
    int bitSizes[8] = {8, 10, 12, 14, 16, 17, 18, 19};
    for (int j = 0; j < 8; j++){
        file << "BITS:" << bitSizes[j] << endl;
        for(int i = 0; i < 50; i++){
            file << "ROUND:" << i+1 << endl;
            //create first random hash value
            int length = (rand() % 30) + 5;
            string text1 = createRandomString(length);
            hash.plainText = text1;
            hash.setHash();
            wrapper wrap(hash.getplainText(), hash.getHash(), bitSizes[j]);
            string hash1 = wrap.truncateHash();
            randomStrings.insert(text1);
            
            while(1){
                //Create random string that is at least 5 characters long
                length = (rand() % 30) + 5;
                string text2 = createRandomString(length);

                //if random string is unique (i.e. not the same as first string created.)
                if (get<1>(randomStrings.insert(text2)) == true ){
                    attempts++;
                    //random hash to test against first hash
                    hash.plainText = text2;
                    hash.setHash();
                    wrapper wrap(hash.getplainText(), hash.getHash(), bitSizes[j]);
                    string hash2 = wrap.truncateHash();
                    
                    //check for collision
                    if (hash2 == hash1){
                        file <<  "ATTEMPTS:" << attempts << endl;      
                        break;
                    }
                }
            
            }
            randomStrings.clear();
            attempts = 0;
        }
    }
    file << "END\n";
    file.close();
}
int main()
{
    int input = 0;
    cout << "-----MODE OPTIONS-----\n"
         << "1: hash a plaintext string.\n"
         << "2: Perform a collision attack.\n"
         << "3: Perform a pre-image attack.\n"
         << "4: Perform both pre-image and collision attack.\n"
         << "ENTER NUMBER TO PERFORM ONE.\n";

    cin >> input;

    clock_t start, end;
    double msecs;

    if (input == 1){
        sha1Algo hash;
        string text = "";
        cout << "ENTER TEXT TO HASH: ";
        getline(cin, text);
        getline(cin,text);
        hash.plainText = text;
        hash.setHash();
        cout << hash.getHash() << endl;
    }
    else if (input == 2){
        cout << "Performing collision attack on bit lengths of 8, 10, 12, 14, 16, 17, 18, 19....\n";
        start = clock();
        colAttack();
        end = clock();
        msecs = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "Created new file, CollisionResults.txt.\n";
        cout << "Finished program in: " << msecs <<  " seconds\n"; 
    }
    else if (input == 3){
        cout << "Performing pre-image attack on bit lengths of 8, 10, 12, 14, 16, 17, 18, and 19....\n";
        start = clock();
        preImageAttack();
        end = clock();
        msecs = ((double) (end - start)) * 1000 / CLOCKS_PER_SEC;
        cout << "Created new file, PreImageResults.txt.\n";
        msecs = msecs/1000;
        cout << "Finished program in: " << msecs << "\n"; 
    }
    else if (input == 4){
        cout << "Performing pre-image and collision attacks on bit lengths of 8, 10, 12, 14, 16, 17, 18, and 19....\n";
        start = clock();
        preImageAttack();
        colAttack();
        end = clock();
        msecs = ((double) (end - start)) * 1000 / CLOCKS_PER_SEC;
        cout << "Created new files, PreImageResults.txt and CollisionResults.txt\n";
        msecs = msecs/1000;
        int minutes = msecs/60;
        cout << "Finished program in: " << minutes << " minutes." << "\n"; 
    }
    else{
        cout << "INVALID INPUT... CLOSING PROGRAM.\n";
        return 0;
    }
    return 0;
    
}