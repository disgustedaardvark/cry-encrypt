#
# Utility script for returning a given ciphertext hex bytes xor some string
#   (therefore, what [hashkey] string was used to encrypt it)
#   -> For testing for vulnerabilities in encryption process.
#


def main():
    # input one hex string and one UTF-8 string
    # convert both to bytes
    # xor them
    # output in hex
    cipher_text = input("Cipher-text in hex: ").lower().strip()
    cipher_text = cipher_text.replace(" ", "")
    print(cipher_text)
    xor_string = input("String to XOR with (in UTF8): ")
    print(xor_string)
    xor_bytes = xor_string

    # convert from hex to bytes
    # first add leading zeros, two per byte
    cipher_text = cipher_text.zfill(len(xor_string) * 2)
    # then turn each hex pair into a byte -> utf8 char -> final string
    cipher_text = [chr(int(cipher_text[i:i+2], 16)) for i in range(len(cipher_text))]
    print("Converted to utf8 byte string:", ''.join(cipher_text))

    # bitwise xor each char
    xor_ints = [ord(cipher_text[i]) ^ ord(xor_bytes[i]) for i in range(len(xor_bytes))]
    xor_hex = [hex(byte)[2:] for byte in xor_ints]

    print("Xor produced:", ''.join(xor_hex))

'''


def main():
    cipher_text = input("Cipher-text in hex: ")
    xor_string = input("Xor with: ")

    is_hex = input("Was that key in hex? [y/n] ").upper().strip()
    if is_hex == 'Y':
        # convert from hex to bytes
        # first add leading zeros, two per byte
        xor_string = xor_string.zfill(len(cipher_text) * 2)
        print("Converted to full hex:", xor_string)
        # then convert to ascii chars instead
        for i in range(0, len(xor_string), 2):
            to_ascii += chr(int(xor_string[i:i+2], 16))
        print("Converted to ascii:", to_ascii)
    elif len(cipher_text) != len(xor_string):
        # if both are ascii strings, they need to be the same length
        print("Must be the same byte length")
        return

    # split into chars
    cipher_bytes = [ord(char) for char in cipher_text]
    xor_string_bytes = [ord(char) for char in xor_string]

    # bitwise xor each char
    xor_ints = [cipher_bytes[i] ^ xor_string_bytes[i] for i in range(len(cipher_bytes))]
    # output in ascii and hex
    output_ascii = ''.join([chr(i) for i in xor_ints])
    output_hex = ''.join([hex(i).zfill(2)[2:] for i in xor_ints]) # make sure each hex byte is 2 digits, without 0x
    print("Xor result (ascii): " + output_ascii)
    print("Xor result (hex): " + output_hex)

'''
if __name__=='__main__':
    main()
