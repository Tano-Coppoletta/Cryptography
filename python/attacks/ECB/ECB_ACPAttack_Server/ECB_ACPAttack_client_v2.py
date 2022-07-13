import os
from math import ceil

from Crypto.Cipher import AES

os.environ['PWNLIB_NOTERM'] = 'True'
os.environ['PWNLIB_SILENT'] = 'True'


from pwn import *
from myconfig import HOST, PORT

if __name__ == '__main__':
    #server = remote(HOST,PORT)
    #message = b"A"*10
    #server.send(message)

    #ciphertext = server.recv(1024)

    #server.close()

    #print(ciphertext.hex())
    #print(len(ciphertext))

    #message = """Here is the msg:{0} - and the sec:{1}""".format(input0, ecb_oracle_secret)
    prefix = b'Here is the msg '
    messaggio =b'A' * 5

    print(len(prefix))
    print(len(messaggio))


    for guess in string.printable:
        msg = messaggio + guess.encode() + b'to cipher:'+ messaggio

        #print(msg[:16] +b'-----'+ msg[16:32] +b'-----'+msg[32:48])
        #print(guess.encode())
        server = remote(HOST,PORT)

        server.send(msg)
        ciphertext=server.recv(1024)
        if guess.encode() == b'H':
            print(ciphertext[16:32] + b'----------'+ciphertext[32:48])

        if (ciphertext[16:32] == ciphertext[32:48]):
            print("found " + guess)
            break

