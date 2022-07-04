import base64

from Crypto.Hash import SHA3_256
from Crypto.Hash import HMAC
from Crypto.Random import get_random_bytes

import json

if __name__ == '__main__':

    msg=b'This is the message used in input'

    secret = get_random_bytes(32)

    hmac_generator = HMAC.new(secret, digestmod= SHA3_256)

    hmac_generator.update(msg)


    print(hmac_generator.hexdigest())
    #store in a json object
    obj= json.dumps({'message': msg.decode(), 'MAC': base64.b64encode(hmac_generator.digest()).decode()})

    #---------receive the json object
    b64_obj = json.loads(obj)
    hmac_verifier = HMAC.new(secret, digestmod = SHA3_256)

    hmac_verifier.update(b64_obj['message'].encode())
    mac = bytearray(base64.b64decode(b64_obj['MAC'].encode()))
    #mac[0]=0 with this we generate the exception
    try:
        hmac_verifier.verify(mac)
        print("the message is authentic")
    except ValueError:
        print("Wrong message or secret")