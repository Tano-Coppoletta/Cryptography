from Crypto.Cipher import ChaCha20
from Crypto.Random import get_random_bytes

if __name__ == '__main__':
    plaintext = b'This is the message to encrypt but the attacker knows there is a specific sequence of numbers 12345'
    #attacker knows that b'1' is in a specific position
    index = plaintext.index(b'1')
    print(index)

    key = get_random_bytes(32)
    nonce = get_random_bytes(12)
    cipher = ChaCha20.new(key= key, nonce= nonce)
    ciphertext= cipher.encrypt(plaintext)

    #attacker knows, ciphertext, index
    new_value = b'9'
    #the attacker wants to change a specific byte with the value b'9'
    new_int = ord(new_value) #this extract the ASCII code of 9

    mask = ord(b'1') ^ new_int #the mask is a xor between 1 and 9

    edt_ciphertext = bytearray(ciphertext)

    edt_ciphertext[index] = ciphertext[index] ^ mask

    #edt_ciphertext is received by the receiver
    #the receiver receives the ciphertext changed by the Man in the middle
    cipher_dec= ChaCha20.new(key= key, nonce= nonce)
    decrypted_text = cipher_dec.decrypt(edt_ciphertext)
    print(decrypted_text)