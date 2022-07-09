import json

from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
import base64

if __name__ == '__main__':
    header = b'This only needs authentication'
    payload = b'this also need confidentiality'

    key = get_random_bytes(AES.key_size[2])

    cipher = AES.new(key, AES.MODE_GCM)

    cipher.update(header)
    ciphertext, tag = cipher.encrypt_and_digest(payload)

    json_keys = ['nonce', 'header', 'ciphertext', 'tag']
    json_values = [cipher.nonce, header, ciphertext, tag]

    json_b64_values = [base64.b64encode(x).decode() for x in json_values]

    json_obj = json.dumps(dict(zip(json_keys, json_b64_values)))

    print(json_obj)

    #### at the verifier

    b64_obj= json.loads(json_obj)
    json_keys= ['nonce', 'header', 'ciphertext', 'tag']

    jv = {k:base64.b64decode(b64_obj[k]) for k in json_keys}

    cipher_reveiver = AES.new(key, AES.MODE_GCM, nonce = jv['nonce'])

    cipher_reveiver.update(jv['header'])
    try:
        cipher_reveiver.decrypt_and_verify(jv['ciphertext'], jv['tag'])
        print("Everything is ok...")
    except (ValueError, KeyError):
        print("Errors occurred with the tag")