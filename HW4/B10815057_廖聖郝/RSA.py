import random
import base64
import sys

def fast_power_mod(base, exponent, mod_num): #sqare and multiply algorithm
    bins = bin(exponent)
    result = 1
    for index in range(0,len(bins)):
        result = (result * result) % mod_num
        if bins[index] == '1':
            result = (result * base) % mod_num
    return result

def mod_inverse(a, m):
    m0 = m
    y = 0
    x = 1 
    if (m == 1):
        return 0 
    while (a > 1):
        q = a // m
        t = m
        m = a % m
        a = t
        t = y
        y = x - q * y
        x = t
    if (x < 0):
        x = x + m0 
    return x

def gcd(a,b):
    if b <= 1 or b >= a:
        return -1

    while b != 0:
        a, b = b, a % b
    return a
def is_coprime(a, b):
    if a >= b:
        return gcd(a, b) == 1
    else:
        return gcd(b, a) == 1
    

def miller_rabin(check_num):   
    if check_num == 2 or check_num == 3:
        return True    
    if check_num == 1 or check_num < 0 or check_num % 2 == 0:
        return False
    k = 10
    r = 0
    s = check_num-1
    while(s%2==0):
        r += 1
        s //=2
    for i in range(k):
        a = random.randrange(2,check_num-1)
        x = pow(a,s,check_num)
        if(x==1 or x == check_num-1):
            continue
        for j in range(r-1):
            x = pow(x,2,check_num)
            if(x==check_num-1):
                break
        else:
            return False
    return True

def prime_gen(bit_count):
    result = random.getrandbits(bit_count)
    if result % 2 == 0:
        result += 1
    while miller_rabin(result) == False:
        result += 2
    return result

def key_gen(bit_count):
    p = prime_gen(int(bit_count))
    q = prime_gen(int(bit_count))
    n = p * q
    phi_n = (p-1)*(q-1)
    public_key = random.randint(2,phi_n)
    if public_key % 2 == 0:
        public_key += 1
    while not is_coprime(public_key,phi_n):
        public_key += 2
    private_key = mod_inverse(public_key,phi_n)

    print("p = {}".format(p))
    print("q = {}".format(q))
    print("n = {}".format(n))
    print("phi = {}".format(phi_n))
    print("e = {}".format(public_key))
    print("d = {}".format(private_key))
    return

def RSA_encrypt(plain_num,n,public_key):
    cipher_num = fast_power_mod(plain_num,public_key,n)
    return cipher_num

def RSA_decrypt(cipher_num,n,private_key):
    plain_num = fast_power_mod(cipher_num,private_key,n)
    return plain_num

def RSA_CRT_decrypt(cipher_num,p,q,private_key):
    dp = private_key % (p-1)
    dq = private_key % (q-1)
    q_inv = mod_inverse(q,p)
    m1 = fast_power_mod(cipher_num,dp,p)
    m2 = fast_power_mod(cipher_num,dq,q)
    h = q_inv * (m1-m2) % p
    result = m2 + h * q
    return int(result)

def str2num(input):
    m_bytes = input.encode('utf-8')
    m_num = int.from_bytes(m_bytes, 'little')
    return m_num

def num2str(input):
    m_bytes = input.to_bytes((input.bit_length() + 7) // 8, 'little')
    m_str = m_bytes.decode('utf-8')
    return m_str

def encrypt(plain_text,n,public_key):
    plain_num = str2num(plain_text)
    cipher_num = RSA_encrypt(plain_num,n,public_key)
    cipher_base64 = base64.b64encode(str(cipher_num).encode('ascii'))
    cipher_text = cipher_base64.decode('ascii')
    return cipher_text

def decrypt(cipher_text,n,private_key):
    cipher_num = int(base64.b64decode(cipher_text).decode('ascii'))
    plain_num = RSA_decrypt(cipher_num,n,private_key)
    plain_text = num2str(plain_num)
    return plain_text

def CRT_decrypt(cipher_text,p,q,private_key):
    cipher_num = int(base64.b64decode(cipher_text).decode('ascii'))
    plain_num = RSA_CRT_decrypt(cipher_num,p,q,private_key)
    plain_text = num2str(plain_num)
    return plain_text

def main():
    argc = len(sys.argv)
    try:
        if argc < 2:
            raise IndexError
        if sys.argv[1] == '-i':
            key_gen(1024)
        elif sys.argv[1] == '-e':
            if argc != 5:
                raise IndexError
            cipher_text = encrypt(sys.argv[2],int(sys.argv[3]),int(sys.argv[4]))
            print(cipher_text)
        elif sys.argv[1] == '-d':
            if argc != 5:
                raise IndexError            
            plain_text = decrypt(sys.argv[2],int(sys.argv[3]),int(sys.argv[4]))
            print(plain_text)
        elif sys.argv[1] == '-CRT':
            if argc != 6:
                raise IndexError
            plain_text = CRT_decrypt(sys.argv[2],int(sys.argv[3]),int(sys.argv[4]),int(sys.argv[5]))
            print(plain_text)
    except IndexError:
        print('argv error!')
    return

if __name__ == "__main__":
    main()