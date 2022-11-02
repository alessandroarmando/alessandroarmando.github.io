--- 
title: "Hands-on Guide to Cryptography"
---

# ASCII Encoding
![ASCII Table](USASCII_code_chart.png "ASCII Table")

Examples:

- '+' = (2B)16 = (0010 1011)2
- 'a' = (61)16 = (0110 0001)2
- 'A' = (41)16 = (0100 0001)2

*Properties*
- If x is (the ASCII code of) a lower case letter, then x+' ' is the (the ASCII code of) the corresponding upper case letter.  For instance, 'a'+' ' = 'A', 'b'+' ' = 'B', ...
- Dually, if x is (the ASCII code of) an upper case letter, then x+' ' is the (the ASCII code of) the corresponding lower case letter.  For instance, 'A'+' ' = 'a', 'B'+' ' = 'b', ...
- The xor of two alfanumeric characters is a special character with high probability.

# XOR Analysis

One-time pad (Vernam Cipher) is an unbreakable type of encryption (unconditional security) but it requires the use of a *single-use* pre-shared key that is not smaller than the message being encrypted.

Reusing the key is recipe to disaster.  Let's see why (and how) the plaintext and the key can be recovered if the key is reused.

* [xortool.py](https://github.com/hellman/xortool), (semi)automated xor analysis
* [MTP](https://github.com/CameronLonsdale/MTP) interactive cryptanalysis