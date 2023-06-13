from itertools import product


REPLACE = { letter: str(index) for index, letter in enumerate('oizeasgtb') }
def Leet2Combos(word):
    possibles = []
    for l in word.lower():
        ll = REPLACE.get(l, l)
        possibles.append( (l,) if ll == l else (l, ll) )
    return [ ''.join(t) for t in product(*possibles) ]


def Leet2(word):
    letters = [ REPLACE.get(l, l) for l in word.lower() ]
    return ''.join(letters)

print(REPLACE)

with open('list.txt', 'r') as f:
    for line in f:
        for word in line.split():
            combos = Leet2Combos(word)
            for combo in combos:
                   with open('output.txt', 'a') as file: 
                        file.write(combo)
                        file.write('\n')
                        print(combo)
print (Leet2Combos('zit'))
print (Leet2Combos('red'))