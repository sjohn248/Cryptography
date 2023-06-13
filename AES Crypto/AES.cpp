#include <iostream>
#include <iomanip>
using namespace std;

// Rcon[] is 1-based, so the first entry is just a place holder
const unsigned int rCon[] = {0x00000000,
           0x01000000, 0x02000000, 0x04000000, 0x08000000,
           0x10000000, 0x20000000, 0x40000000, 0x80000000,
           0x1B000000, 0x36000000, 0x6C000000, 0xD8000000,
           0xAB000000, 0x4D000000, 0x9A000000, 0x2F000000,
           0x5E000000, 0xBC000000, 0x63000000, 0xC6000000,
           0x97000000, 0x35000000, 0x6A000000, 0xD4000000,
           0xB3000000, 0x7D000000, 0xFA000000, 0xEF000000,
           0xC5000000, 0x91000000, 0x39000000, 0x72000000,
           0xE4000000, 0xD3000000, 0xBD000000, 0x61000000,
           0xC2000000, 0x9F000000, 0x25000000, 0x4A000000,
           0x94000000, 0x33000000, 0x66000000, 0xCC000000,
           0x83000000, 0x1D000000, 0x3A000000, 0x74000000,
           0xE8000000, 0xCB000000, 0x8D000000};

const unsigned int sBox[16][16] = {
    { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
    { 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
    { 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
    { 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
    { 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
    { 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
    { 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
    { 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
    { 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
    { 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
    { 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
    { 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
    { 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
    { 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
    { 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
    { 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
  };

const unsigned int invSbox[16][16] = {
    { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb } ,
    { 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb } ,
    { 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e } ,
    { 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25 } ,
    { 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92 } ,
    { 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84 } ,
    { 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06 } ,
    { 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b } ,
    { 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73 } ,
    { 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e } ,
    { 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b } ,
    { 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4 } ,
    { 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f } ,
    { 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef } ,
    { 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61 } ,
    { 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d }
    };

/*uint8_t state[4][4] =  { {0x19,0xa0,0x9a,0xe9},
                         {0x3d,0xf4,0xc6,0xf8},
                         {0xe3,0xe2,0x8d,0x48},
                         {0xbe,0x2b,0x2a,0x08}};
*/
uint8_t fixedPoly[4][4] = { {0x2,0x3,0x1,0x1},
                            {0x1,0x2,0x3,0x1},
                            {0x1,0x1,0x2,0x3},
                            {0x3,0x1,0x1,0x2}};

uint8_t invFixedPoly[4][4] = {  {0x0e, 0x0b, 0x0d, 0x09},
                                {0x09, 0x0e, 0x0b, 0x0d},
                                {0x0d, 0x09, 0x0e, 0x0b},
                                {0x0b, 0x0d, 0x09, 0x0e}};

uint8_t key1[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};


             
uint8_t key2[24] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};

uint8_t key3[32] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
//uint8_t in[16]  = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
//                    0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };


uint8_t in[16]  = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                   0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

uint8_t out[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

unsigned int ffAdd(unsigned int a, unsigned int b){
    //^ is the XOR operator in C++
    unsigned int result = a^b;
    return result;
}
unsigned int xTime(unsigned int a){
    unsigned char result = a<<1;
    //AND with 0x80(128) to see if first bit is a 1 or a 0. If the first bit is 1 then we XOR with 0x1b.   
    if (a & 0x80){
        result = result^(0x1b);
    }
    return result;
}
unsigned int ffMultiply(unsigned int a, unsigned int b){
    unsigned int sum = 0;
    //unsigned int bitCheck = a;

    for (int i = 0; i < 8; i++){
        //check if least significant bit is a 1 or 0, if 1 then new sum = sum XOR b.
        if (a & 0x1){
            sum = sum^(b);
        }
        b = xTime(b);
        //shift bits to the right so least significant bit changes
        //bitCheck = bitCheck >> 1;
        a = a >> 1;
    }
    return sum;
}
uint32_t subWord(uint32_t a){
    unsigned int parts[4];
    unsigned int sBoxValue[4];
    parts[0] = (a & 0xFF);
    parts[1] = (a>>8 & 0xFF);
    parts[2] = (a>>16 & 0xFF);
    parts[3] = (a>>24 & 0xFF);

    //finds Sbox value based on 4 bits
    for (int i = 0; i < 4; i++){
        unsigned int halfbyte1 = parts[i] & 0xF;
        unsigned int halfbyte2 = parts[i] >>4 &0xF;
        sBoxValue[i] = sBox[halfbyte2][halfbyte1];
    }
    uint32_t newValue = 0;
    //builds new value based on the 4 sBox values found
    for(int i = 3; i >= 0; i--){
        newValue = (newValue << 8) | sBoxValue[i];
    }
    return newValue;
}
unsigned int rotWord(unsigned int a){
    //get the highest word by itself
    unsigned int a1 = a & 0xFF000000;
    //shift the word to the end
    a1 = a1 >> 24;
    //clear out 8 bits and append a1 onto a
    uint32_t newWord = (a << 8) | a1;
    return newWord;
}

uint8_t* subBytes(uint8_t (&a)[4][4]){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            unsigned int part1 = (a[i][j]  & 0xF);
            unsigned int part2 = (a[i][j] >> 4 & 0xFF);
            a[i][j] = sBox[part2][part1];
        }
    }
    return reinterpret_cast<uint8_t *>(a);
}

uint8_t* invSubBytes(uint8_t (&a)[4][4]){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            unsigned int part1 = (a[i][j]  & 0xF);
            unsigned int part2 = (a[i][j] >> 4 & 0xFF);
            a[i][j] = invSbox[part2][part1];
        }
    }
    return reinterpret_cast<uint8_t *>(a);
}

uint8_t* shiftRows(uint8_t (&a)[4][4]){
    uint8_t tempRow[4];

    for(int i = 0; i < 4; i++){
        //copy the row being shifted
        for (int j = 0; j < 4; j++){
                tempRow[j] = a[i][j];
        }
        //second row shifts 1
        if (i == 1){
            a[1][0] = tempRow[1];
            a[1][1] = tempRow[2];
            a[1][2] = tempRow[3];
            a[1][3] = tempRow[0];
        }
        //third row shifts 2
        if (i == 2){
            a[2][0] = tempRow[2];
            a[2][1] = tempRow[3];
            a[2][2] = tempRow[0];
            a[2][3] = tempRow[1];  
        }
        //fourth row shifts 3
        if (i == 3){
            a[3][0] = tempRow[3];
            a[3][1] = tempRow[0];
            a[3][2] = tempRow[1];
            a[3][3] = tempRow[2];   
        }
    }
    return reinterpret_cast<uint8_t *>(a);
}

uint8_t* invShiftRows(uint8_t (&a)[4][4]){
    uint8_t tempRow[4];

    for(int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
                tempRow[j] = a[i][j];
        }
        if (i == 1){
            a[1][0] = tempRow[3];
            a[1][1] = tempRow[0];
            a[1][2] = tempRow[1];
            a[1][3] = tempRow[2];
        }
        if (i == 2){
            a[2][0] = tempRow[2];
            a[2][1] = tempRow[3];
            a[2][2] = tempRow[0];
            a[2][3] = tempRow[1];  
        }
        if (i == 3){
            a[3][0] = tempRow[1];
            a[3][1] = tempRow[2];
            a[3][2] = tempRow[3];
            a[3][3] = tempRow[0];   
        }
    }
    return reinterpret_cast<uint8_t *>(a);
}

uint8_t* mixColumns(uint8_t (&a)[4][4]){
    uint8_t temp[4][4];
    for (int i = 0; i < 4; i++){
        //make a copy of state 
        for(int j = 0; j < 4; j++){
            temp[i][j] = a[i][j];
        }
    }
    //perform math as listed in specifications
    for (int i = 0; i <4; i++){
        temp[0][i] = (ffMultiply(fixedPoly[0][0], a[0][i]) ^ (ffMultiply(fixedPoly[0][1], a[1][i]) ^ a[2][i] ^ a[3][i]));
        temp[1][i] = a[0][i] ^ (ffMultiply(fixedPoly[0][0], a[1][i])) ^ (ffMultiply(fixedPoly[0][1], a[2][i])) ^ a[3][i];
        temp[2][i] = a[0][i] ^ a[1][i] ^ (ffMultiply(fixedPoly[0][0], a[2][i]) ^ (ffMultiply(fixedPoly[0][1], a[3][i])));
        temp[3][i] = (ffMultiply(fixedPoly[0][1], a[0][i]) ^a[1][i] ^a[2][i] ^ (ffMultiply(fixedPoly[0][0], a[3][i])));
    }

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            a[i][j] = temp[i][j];
        }
    }
    return reinterpret_cast<uint8_t *>(a);
}

uint8_t* invMixColumns(uint8_t (&a)[4][4]){
    uint8_t temp[4][4];
    for (int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            temp[i][j] = a[i][j];
        }
    }
    for (int i = 0; i <4; i++){
        temp[0][i] = (ffMultiply(invFixedPoly[0][0], a[0][i]) ^ (ffMultiply(invFixedPoly[0][1], a[1][i]) ^ (ffMultiply(invFixedPoly[0][2], a[2][i]) ^ (ffMultiply(invFixedPoly[0][3], a[3][i])))));
        temp[1][i] = (ffMultiply(invFixedPoly[1][0], a[0][i]) ^ (ffMultiply(invFixedPoly[0][0], a[1][i]) ^ (ffMultiply(invFixedPoly[0][1], a[2][i]) ^ (ffMultiply(invFixedPoly[0][2], a[3][i]))))); 
        temp[2][i] = (ffMultiply(invFixedPoly[0][2], a[0][i]) ^ (ffMultiply(invFixedPoly[1][0], a[1][i]) ^ (ffMultiply(invFixedPoly[0][0], a[2][i]) ^ (ffMultiply(invFixedPoly[0][1], a[3][i])))));
        temp[3][i] = (ffMultiply(invFixedPoly[0][1], a[0][i]) ^ (ffMultiply(invFixedPoly[0][2], a[1][i]) ^ (ffMultiply(invFixedPoly[0][3], a[2][i]) ^ (ffMultiply(invFixedPoly[0][0], a[3][i])))));
    }

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            a[i][j] = temp[i][j];
        }
    }
    return reinterpret_cast<uint8_t *>(a);
}

uint32_t word(uint32_t part1, uint32_t part2, uint32_t part3, uint32_t part4){
    uint32_t newWord;
    newWord = newWord << 8 | part1;
    newWord = newWord<< 8 | part2;
    newWord = newWord << 8 | part3; 
    newWord = newWord << 8 | part4;
    return(newWord);
}
uint32_t* keyExpansion(uint8_t *key, uint32_t *expanded, int Nk, int mode){
    uint32_t temp = 0;

    int i = 0;
    while (i < Nk){
        expanded[i] = word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);
        i +=1;
    }
    i = Nk;
    while (i < mode){
        temp = expanded[i-1];
        if (i%Nk == 0){
            temp = subWord(rotWord(temp)) ^ rCon[i/Nk];
        }
        else if (Nk > 6 && i%Nk == 4){
            temp = subWord(temp);
        }
        expanded[i] = expanded[i-Nk] ^ temp;
        i = i + 1;
    }
    return reinterpret_cast<uint32_t *>(expanded);
}

void addRoundKey(uint8_t (&state)[4][4], uint32_t(&expanded)[60], int Nb, int round, int pRound, string type){
    uint8_t temp[4];
    cout  <<  "round[" <<  setw(2) << setfill(' ') <<  right << dec << pRound <<  "]." <<  setw(10) << left << type;
    for (int i = 0; i < 4; i++){
        temp[3] = expanded[round*Nb+i] & 0xFF;
        temp[2] = expanded[round*Nb+i] >> 8 & 0xFF;
        temp[1] = expanded[round*Nb+i] >> 16 & 0xFF;
        temp[0] = expanded[round*Nb+i] >> 24 & 0xFF;
        for (int j = 0; j < 4; j++){
            cout  << right << hex << setfill('0') << setw(2) << unsigned(temp[j]);
            state[j][i] = state[j][i] ^ temp[j];
        }
        
    }
    cout << "\n";
}

uint8_t* buildState(uint8_t(&input)[16], uint8_t(&state)[4][4]){
    int k = 0;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            state[j][i] = input[k];
            k++;
        }
    }

     return reinterpret_cast<uint8_t *>(state);
}

uint8_t* buildOutput(uint8_t(&output)[16], uint8_t(&state)[4][4]){
    int k = 0;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            output[k] = state[j][i];
            k++;
        }
    }
    return reinterpret_cast<uint8_t *>(state);
}
void printState(int round, uint8_t state[4][4], string type){

    cout << "round[" << setw(2) << setfill(' ') << right << dec << round << "]."  << left << setw(10) << type;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            cout  <<  right << setw(2) << hex <<  setfill('0')  << unsigned(state[j][i]);
        }
    }
    cout << "\n";
}
void printInandOut(uint8_t data[16]){
    for (int i = 0; i < 16; i++){
        cout <<  setfill('0') <<  setw(2) << hex  << unsigned(data[i]);
    }
}
void cipher(uint8_t(&input)[16], uint8_t(&output)[16], uint32_t(&word)[60], int Nk, int Nr){
    uint8_t state[4][4];
    buildState(input, state);

    
    int rCounter = 0;
    cout << "round[ " << rCounter << "].input"  << "     ";
    printInandOut(input);
    cout << "\n";
    addRoundKey(state, word, 4, rCounter, rCounter, "k_sch");

    
 
    for (int round = 1; round < Nr; round++){ 
        rCounter++;
        printState(rCounter, state, "start");
        subBytes(state);
        printState(round, state, "s_box");
        shiftRows(state);
        printState(round, state, "s_row");
        mixColumns(state);
        printState(round, state, "m_col");
        addRoundKey(state, word, 4, round, round, "k_sch");
    }

    rCounter++;
    printState(rCounter, state, "start");
    subBytes(state);
    printState(rCounter, state, "s_box");
    shiftRows(state);
    printState(rCounter, state, "s_row");
    addRoundKey(state, word, 4, rCounter, rCounter, "k_sch");

    buildOutput(output, state);
    cout << "round[" << dec << rCounter << "].output"  << "    ";
    printInandOut(output);
}
void invCipher(uint8_t(&input)[16], uint8_t(&output)[16], uint32_t(&word)[60], int Nk, int Nr){
    uint8_t state[4][4];
    buildState(input, state);

    int pRound = 0;
    cout << "round[ " << pRound << "].iinput"  << "    ";
    printInandOut(input);
    cout << "\n";
    int rCounter = Nr-1;
    addRoundKey(state, word, 4, Nr, pRound, "ik_sch");

    for (int round = Nr-1; round >= 1; round--){  
        pRound++;
        printState(pRound, state, "istart");
        invShiftRows(state);
        printState(pRound, state, "is_row");
        invSubBytes(state);
        printState(pRound, state, "is_box");
        addRoundKey(state, word, 4, round, pRound, "ik_sch");
        printState(pRound, state, "ik_add");
        invMixColumns(state);
        rCounter--;
    }

    pRound++; 
    printState(pRound, state, "istart");
    invShiftRows(state);
    printState(pRound, state, "is_row");
    invSubBytes(state);
    printState(pRound, state, "is_box");
    addRoundKey(state, word, 4, rCounter, pRound, "ik_sch");
    buildOutput(output, state);
    cout << "round[" << dec << pRound << "].ioutput"  << "   ";
    printInandOut(output);
}
int main(){

    //AES-128 (Nk = 4, Nr = 10)
    //w size is equal to  Nb*(Nr+1), so AES-128 = 44, AES-192 = 52, AES-256 = 60. 10 rounds for 128, 12 rounds for 192, 14 rounds for 256
    cout << "C.1   AES-128 (Nk=4, Nr=10)\n\n"
         << "PLAINTEXT:          00112233445566778899aabbccddeeff\n"
         << "KEY:                000102030405060708090a0b0c0d0e0f\n\n"
         << "CIPHER (ENCRYPT):\n";
    
    uint32_t w[60];
    keyExpansion(key1 , w, 4, 44);
    cipher(in, out, w, 4, 10);
    cout << "\n\nINVERSE CIPHER (DECRYPT):\n";
    invCipher(out, out, w, 4, 10);

    cout << "\n\nC.2   AES-192 (Nk=6, Nr=12)\n\n"
         << "PLAINTEXT:          00112233445566778899aabbccddeeff\n"
         << "KEY:                000102030405060708090a0b0c0d0e0f1011121314151617\n\n"
         << "CIPHER (ENCRYPT):\n";

    uint32_t w2[60];
    keyExpansion(key2 , w2, 6, 52);
    cipher(in, out, w2, 6, 12);
    cout << "\n\nINVERSE CIPHER (DECRYPT):\n";
    invCipher(out, out, w2, 6, 12);


    cout << "\n\nC.3   AES-256 (Nk=8, Nr=14)\n\n"
         << "PLAINTEXT:          00112233445566778899aabbccddeeff\n"
         << "KEY:                000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f\n\n"
         << "CIPHER (ENCRYPT):\n";

    uint32_t w3[60];
    keyExpansion(key3 , w3, 8, 60);
    cipher(in, out, w3, 8, 14);
    cout << "\n\nINVERSE CIPHER (DECRYPT):\n";
    invCipher(out, out, w3, 8, 14);
   
    return 0;
}