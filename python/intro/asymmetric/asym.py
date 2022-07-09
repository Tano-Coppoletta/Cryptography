from Crypto.Util.number import getPrime

n_length = 1024

p1 = getPrime(n_length)
p2 = getPrime(n_length)
print("p1= " +str(p1))
print("p2= " + str(p2))

n = p1*p2
print("n= "+ str(n))

phi = (p1-1)*(p2-1)

#define the public exponent
e = 65537

from math import gcd

#greatest common divisor
#e and phi should not have a gcd different from 1
g = gcd(e,phi)
print(g)
if g!=1:
    raise ValueError

d = pow(e, -1, phi)
print("d= " + str(d))

#RSA key
public_rsa_key = (e, n)
private_rsa_key = (d, n)

#encryption
msg = b'this is the message to encrypt'
#rsa requires that the plaintext is a number
msg_int = int.from_bytes(msg, byteorder='big') #big means big endian
print("msg=+ " + str(msg_int))

#if the int representation of the msg is greater than n trigger an error
if msg_int > n-1:
    raise ValueError

C = pow(msg_int , e, n)
print("Ciphertext = " + str(C))

D = pow(C, d, n)
print("Decryption = " + str(D))

msg_dec = D.to_bytes(n_length, byteorder='big')
print("msg" + str(msg_dec))
print(msg.decode())
