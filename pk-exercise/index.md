--- 
layout: default
title: "GPG Hands-on: The Expedition of the Thousand"
---

# GPG Hands-on: The Expedition of the Thousand

You are Camillo Benso (Count of Cavour):
- [your private key](benso.prk)
- [your public key](benso.puk)

You receive the following three messages apparently from Gen. Garibaldi:

* [Message 1](sailing-off-1.txt.asc)
* [Message 2](sailing-off-2.txt.asc)
* [Message 3](sailing-off-3.txt.asc)

You realize you need Gen. Garibaldi's public key.

You ask your agents to find it.

After some time an agent comes back to you with [Garibaldi's public key #1](garibaldi1.puk).

Question 1: Assuming this is Garibaldi's public key, where the expedition will sail from?
  
A few hours later two agents bring to you two additional keys:
- [Garibaldi's public key #2](garibaldi2.puk)
- [Garibaldi's public key #3](garibaldi3.puk)

At this point you start being suspicious and ask the agents to provide evidence that these keys belong to Gen. Garibaldi.


One of the agent comes back some time later with the following sequence of exadecimal characters:
```
10DB 9869 9AAE 281F 47F6  F140 5688 E7EA 9DCE F4E6
```
claiming to be the fingerprint of Garibaldi's public key.

Question 2: Assuming this is the fingerprint of Garibaldi's public key, where the expedition will sail from?

You are still unconvinced and then decide to give a closer look to the three public keys brought to you by the agents.

A closer look to [Garibaldi's public key #3](garibaldi3.puk) reveals that...

You then ask you agents to find Mazzini's public key. After some time they bring the following two keys:
- [Mazzini's Public Key #1](mazzini1.puk)
- [Mazzini's Public Key #2](mazzini2.puk)

Luckily you met Mazzini last week, you pull his business card from the pocket and you find his public key is printed on it:
```
3B57 70D9 9571 5C09 8552  C368 BB34 9BD6 C17C C964
```

You trust Mazzini to properly validate the keys that he signs.

Question 3: Where the expedition will sail from?
