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

def checkPrime(p, a):
    exp = p - 1
    #divide by 2 and exp is even. If odd/2 is a decimal and we dont want that
    while not exp & 1:
        exp >>= 1

    #if positive one or negative one, it is probably prime
    x = modExponentiation(a, exp, p)    
    if x == 1 or x == p-1:
        #probably prime
        return True

    while exp != p - 1:
        x = (x * x) % p 
        exp = exp * 2

        if (x == 1):
            return False;
        elif (x == p - 1):
            return True; 
    
    #For sure composite    
    return False

def millerRabin(p, k):
    #loops the number of times to test
    for i in range(k):
        a = random.randint(2, p-2)
        if not checkPrime(p, a):
            return False
    return(True)    


#generates a single prime number of nBit length
def primeGen(nBits):
    num = random.getrandbits(nBits)
    num = num | (1 << 0)
    num = num | (1 << nBits-1) 
    while(1):
        if (num %2 != 0):  
            if (millerRabin(num, 3) == True): 
                return (num)
        num +=2  

#generates a strong prime of nBit length
def strongPrimeGen(nBits):
    count = 0
    while(1):
        num = primeGen(nBits)
        strong = (num - 1) >> 1
        print(f'Testing Prime: {count}')
        if(millerRabin(strong, 3) == True):
            break
        count+=1
    return(num)

def gcd(a, b):
  while b != 0:
    a, b = b, a % b
  return a

def extEuclidean(a, b):
    x2, x = 1, 0 
    y2, y = 0, 1
    while(b):
        q = a // b
        x, x2 = x2 - q*x, x
        y, y2 = y2 - q*y, y
        a, b = b, a % b
    return(y2)

def getParams(e):
    while(1):
        p = primeGen(1024)
        q = primeGen(1024)
        n = p*q

        phi = (p-1)*(q-1)
        if(gcd(phi, e) == 1):
            break
    return(p, q, n, phi)

def diffieHellman():
    print("Do you want to generate a new strong prime(press 1) or use default(press 2)?")
    mode = input()
    if (mode == "1"):
        print("Generating Strong Prime...")
        p = strongPrimeGen(1024)
        print(f"Your Strong Prime is: {p}")
    elif(mode == "2"):
        p = 162406240459070511790522193884671263202672311225361496498818196588732313721334286548866846578666790823130576411051805445766332206846195323170645247827585891191146261006972350594967460037788709589999328145086343379744093838159633594297281031496856929318152654200263291195588311657357880023267416385490237441427
        print(f"Your Strong Prime is: {p}")
    else:
        print("Invalid input. Closing...")
        exit()
    a = random.getrandbits(1024)
    print(f"Your \"a\" value is: {a}")
    g = 5
    pubKey = modExponentiation(g, a, p)
    print(f"Your pubKey, \"g^a\", is: {pubKey}")
    
    gPowerb = input("Paste g^b from server: ")
    sharedKey = modExponentiation(int(gPowerb), a, p)
    sharedKeytoPrint = sharedKey
    sharedKey = (sharedKey).to_bytes((sharedKey.bit_length() + 7) //8, byteorder='big', signed=False)
    
    cipherText = input("Paste Ciphertext from server: ")
    cipherText = bytes.fromhex(cipherText)
    
    key = hashlib.sha256(sharedKey).hexdigest()[:32]
    
    key = bytes.fromhex(key)
    
    iv = input("Paste iv from server: ")
    iv = bytes.fromhex(iv)

    print(f"Your shared key, \"g^a^b\" is: {sharedKeytoPrint}")
    cipher = AES.new(key, AES.MODE_CBC, iv)
    pt = unpad(cipher.decrypt(cipherText), AES.block_size)
    pt = pt.decode('ASCII')
    print(f"Your Decrypted Message is: {pt}")

def rsa():
    e = 65537
    print("Generating RSA parameters...")
    p, q, n, phi = getParams(e)
    print(f"First Prime: {p}\n")
    print(f"Second Prime: {q}\n")
    print(f"RSA Modulus: {n}\n")
    print(f"phi(n): {phi}\n")
    d = extEuclidean(phi, e)%phi
    print(f"Private Exponent(d): {d}")
    plaintext = input("Plaintext to Encrypt: ")
    plaintext = int.from_bytes(plaintext.encode(), 'big')

    cipherText = modExponentiation(plaintext, e, n)
    print(f"Encrypted Value is: {cipherText}")

    #decrypt
    cipherText = input("Value to Decrypt: ")
    pt = hex(modExponentiation(int(cipherText), d, n))
    pt = bytes.fromhex(pt[2:])
    print(pt.decode('ASCII'))



print("Please Select Diffie-Hellman(press 1) or RSA(press 2): ")
mode = input()
if (mode == "1"):
    diffieHellman()
elif(mode == "2"):
    rsa()
else:
    print("Invalid input. Closing...")
    exit()

