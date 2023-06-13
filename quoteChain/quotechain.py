import random
import hashlib


def driver(text, hash, blockNum):
    textByte = text.encode('ASCII')
    while(1):
        if blockNum == 1:
            hashByte = hash.to_bytes((hash.bit_length()+7)//8, 'big')
        else:
            hashByte = hash.to_bytes(32, 'big')

        nonce = random.getrandbits(32)
        nonceBytes = nonce.to_bytes((nonce.bit_length()+7)//8, 'big')
        appended = hashByte + nonceBytes + textByte
        hashAppended = hashlib.sha256(appended).hexdigest()
        hashAppended = int(hashAppended,16)

        if(hashAppended.bit_length() <= 246):
            print(nonce)
            print(str(hex(hashAppended)))
            break



text = input("Enter your quote: ")
hash = input("Enter your hash: ")
hash = int(hash, 16)
driver(text, hash, 1)

text = input("Enter your new quote: ")
blockOneHash = input("Enter Block1 hash: ")
blockOneHash = int(blockOneHash, 16)
driver(text, blockOneHash, 2)

text = input("Enter your new quote: ")
blockTwoHash = input("Enter Block2 hash: ")
blockTwoHash = int(blockTwoHash, 16)
driver(text, blockTwoHash, 3)

