from myconfig import HOST, PORT
import os
os.environ['PWNLIB_NOTERM'] = 'True'
os.environ['PWNLIB_SILENT'] = 'True'

from Crypto.Cipher import AES
from Crypto.Util.Padding import pad

from pwn import *

if __name__ == '__main__':

    server = remote(HOST,PORT)

    username = b'a'*6
    original_cookie = pad(b'username='+username+b',admin=false',AES.block_size)
    print(original_cookie)
    new_cookie = pad(b'username='+username+b',admin=true',AES.block_size)
    print(new_cookie)

    server.send(username)
    encrypted_cookie = server.recv(1024)

    edt_encry_cookie=bytearray(encrypted_cookie)



    for i in range(AES.block_size):
        mask =new_cookie[i+AES.block_size] ^ original_cookie[i+AES.block_size]
        edt_encry_cookie[i] = mask ^ edt_encry_cookie[i]


    server.send(edt_encry_cookie)
    ans = server.recv(1024)
    print(ans)
