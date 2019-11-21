--- 
layout: default
title: "GPG Hands-on: The Expedition of the Thousand"
---

# GPG Hands-on: [The Expedition of the Thousand](https://en.wikipedia.org/wiki/Expedition_of_the_Thousand)

You are Camillo Benso (Count of Cavour):
- [your private key](benso.prk) (Passphrase: "pippo")
- [your public key](benso.puk)

You receive the following three messages apparently from Gen. Garibaldi:

* [Message 1](sailing-off-1.gpg)
* [Message 2](sailing-off-2.gpg)
* [Message 3](sailing-off-3.gpg)

You realize you need Gen. Garibaldi's public key.

You ask your agents to find it.

After some time an agent comes back to you with
- [Garibaldi's public key certificate #1](garibaldi1.puk) (KeyID: 10DB98699AAE281F47F6F1405688E7EA9DCEF4E6).

Question 1: Assuming this is Garibaldi's public key, where the expedition will sail from?
  
A few hours later two agents bring to you two additional keys:
- [Garibaldi's public key #2](garibaldi2.puk) (KeyID: 10DB98699AAE281F47F6F1405688E7EA9DCEF4E6)
- [Garibaldi's public key #3](garibaldi3.puk) (KeyID: 5E11681C77285F1E8062B36CAB846700F26773AE)

At this point you start being suspicious and ask the agents to provide evidence that these keys belong to Gen. Garibaldi.


One of the agent comes back some time later with
```
10DB 9869 9AAE 281F 47F6  F140 5688 E7EA 9DCE F4E6
```
claiming to be the fingerprint of the actual Garibaldi's public key.

Question 2: Assuming this is the fingerprint of Garibaldi's public key, where the expedition will sail from?

You are still unconvinced and thus decide to give a closer look to the three public keys brought to you by the agents.

By inspecting [Garibaldi's public key #3](garibaldi3.puk) you notice that...

You then ask your agents to find Mazzini's public key. After some time they bring the following two keys:
- [Mazzini's Public Key #1](mazzini1.puk) (KeyID: 3B5770D995715C098552C368BB349BD6C17CC964)
- [Mazzini's Public Key #2](mazzini2.puk) (KeyID: 178B5921E9447070E436585D6D82B01D17FD1FD3)

Luckily you met Mazzini last week, you pull his business card from the pocket and you find his public key is printed on it:
```
178B 5921 E944 7070 E436  585D 6D82 B01D 17FD 1FD3
```

You trust Mazzini to properly validate the keys that he signs.

Question 3: Where the expedition will sail from?

[Answers](answers)
