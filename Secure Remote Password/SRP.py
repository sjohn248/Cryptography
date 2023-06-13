import random
import hashlib
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

def modExponentiation(base, exponent, divisor):
    remainder = 1
    while exponent > 0:
        #if least significant bit is 1
        if exponent & 1 == 1:
            remainder = (remainder*base)%divisor
        base = (base*base)%divisor
        exponent >>=1
    return remainder


a = random.getrandbits(1024)
print(f"A: {a}")
g =5
p = 233000556327543348946447470779219175150430130236907257523476085501968599658761371268535640963004707302492862642690597042148035540759198167263992070601617519279204228564031769469422146187139698860509698350226540759311033166697559129871348428777658832731699421786638279199926610332604408923157248859637890960407
pubKey = modExponentiation(g, a, p)
print(f"PUBKEY: {pubKey}")

netID = "sjohn248"

iterations = 1000
password = input("ENTER PASSWORD: ")

passwordBytes = password.encode('ASCII')
salt = input("ENTER SALT: ")
salt = int(salt,16)
saltBytes = salt.to_bytes((salt.bit_length() + 7) // 8, 'big')
B = int(input("ENTER B: "))
gPowera = int(input("ENTER g^a: "))

#Part 2
x = hashlib.sha256(saltBytes + passwordBytes).digest()
for i in range (iterations - 2):
    x = hashlib.sha256(x).digest()

x = hashlib.sha256(x).hexdigest()
print(f"X: {int(x, 16)}")


pBytes = p.to_bytes((p.bit_length() + 7) // 8, 'big')
gBytes = g.to_bytes((g.bit_length() + 7) // 8, 'big')

k = hashlib.sha256(pBytes + gBytes).hexdigest()

print(f"K: {int(k,16)}")

gPowerb = B - (int(k,16) * (modExponentiation(g, int(x,16), p)))
gPowerb = gPowerb % p
print(f"GPOWERB: {gPowerb}")


gPoweraBytes = gPowera.to_bytes((gPowera.bit_length() + 7) // 8, 'big')
gPowerbBytes = gPowerb.to_bytes((gPowerb.bit_length() + 7) // 8, 'big')

u = hashlib.sha256(gPoweraBytes + gPowerbBytes).hexdigest()
print(f"U: {int(u,16)}")

exponent = a + (int(u,16) * int(x,16))
sharedKey = modExponentiation(gPowerb, exponent, p)
print(f"SHAREDKEY: {sharedKey}")

#Part 3
hashP = hashlib.sha256(pBytes).hexdigest()
hashG = hashlib.sha256(gBytes).hexdigest()

firstXor = int(hashP,16) ^ int(hashG,16)
firstXorBytes = firstXor.to_bytes((firstXor.bit_length() + 7) // 8, 'big')

netIDBytes = netID.encode('ASCII')
hashNetID = hashlib.sha256(netIDBytes).hexdigest()
hashNetID = int(hashNetID,16)
hashNetID = hashNetID.to_bytes((hashNetID.bit_length() + 7) // 8, 'big')

sharedKeyBytes = sharedKey.to_bytes((sharedKey.bit_length() + 7) // 8, 'big')

M1 = hashlib.sha256(firstXorBytes + hashNetID + saltBytes + gPoweraBytes + gPowerbBytes + sharedKeyBytes).hexdigest()
print(f"M1: {M1}")

M1Bytes = int(M1,16)
M1Bytes = M1Bytes.to_bytes((M1Bytes.bit_length() + 7) // 8, 'big')
#Part 4
M2 = hashlib.sha256(gPoweraBytes + M1Bytes + sharedKeyBytes).hexdigest()
print(f"M2: {M2}")



