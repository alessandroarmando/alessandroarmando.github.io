--- 
layout: default
title: "GPG Hands-on: The Expedition of the Thousand"
---

# GPG Hands-on: [The Expedition of the Thousand](https://en.wikipedia.org/wiki/Expedition_of_the_Thousand)

You are Camillo Benso (Count of Cavour):
- [your private key](benso.prk) (Passphrase: "BagnaCauda")
- [your public key](benso.puk)

Note: After importing the private key you must tell GPG that you trust these keys.  After all, since here you are impersonating the Count of Cavour, these are your keys. Use the `gpg --edit-key` command and set the trust for this key to `ultimate`.

You receive the following three messages apparently from Gen. Garibaldi:

* [Message 1](sailing-off-1.gpg)
* [Message 2](sailing-off-2.gpg)
* [Message 3](sailing-off-3.gpg)

You realize you need Gen. Garibaldi's public key.

You ask your agents to find it.

After some time an agent comes back to you with
- [Garibaldi's public key #1](garibaldi1.puk) (KeyID: 975EE7ED69CB835DA1B73029CA71A50EC83BEDDB).

Question 1: Assuming this is Garibaldi's public key, where the expedition will sail from?
  
A few hours later two agents bring to you the following additional keys:
- [Garibaldi's public key #2](garibaldi2.puk) (KeyID: ED8B6D50AF8AA4542CC05D07B25AAD1106246D76)
- [Garibaldi's public key #3](garibaldi3.puk) (KeyID: CE8013BB8CC9139CB750A8D9CE844F12BC581CCF)
- [Mazzini's Public Key #1](mazzini1.puk) (KeyID: EB99FAB1C91F30BD35CBE9FD20422A281CDA255C)
- [Mazzini's Public Key #2](mazzini2.puk) (KeyID: 260F498B4E3F72EF9AE273518B7C2EF6CFEBA5A0)

You import them into you key ring.

At this point you start being suspicious and ask the agents to provide evidence that these keys belong to Gen. Garibaldi.

Some time later an agent comes back with
```
ED8B6D50AF8AA4542CC05D07B25AAD1106246D76
```
claiming to be the fingerprint of the actual Garibaldi's public key.

Question 2: Assuming this is the fingerprint of Garibaldi's public key, where the expedition will sail from?

You are still unconvinced and thus decide to give a closer look to the public keys brought to you by the agents.

You recall you met Garibaldi last week. You pull his business card from the pocket and you find his public key fingerprint printed on it:
```
260F498B4E3F72EF9AE273518B7C2EF6CFEBA5A0
```

You trust Mazzini to properly validate the keys that he signs.

Question 3: Where the expedition will sail from?

[Answers](answers)
