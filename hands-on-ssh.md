--- 
layout: default
title: "How to establis an SSH connection"
---

## Establishing a SSH connection with a host

To connect to a host (pgpeduc.csec.dibris.unige.it for our hands-on session) you must use a ssh client on you PC.  If you have a linux PC, then you can type the following command on a terminal:

```
> ssh -l <USERNAME GOES HERE> pgpeduc.csec.dibris.unige.it
The authenticity of host 'pgpeduc.csec.dibris.unige.it (130.251.1.200)' can't be established.
ECDSA key fingerprint is SHA256:h8W/YOP24Va+wl1a67aZ+uErm04kjPictC2w5yqFgpA.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added 'pgpeduc.csec.dibris.unige.it,130.251.1.200' (ECDSA) to the list of known hosts.
armando@pgpeduc.csec.dibris.unige.it's password: <PASSWORD GOES HERE>
Welcome to Ubuntu 16.04.3 LTS (GNU/Linux 4.4.0-97-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

0 packages can be updated.
0 updates are security updates.


*** System restart required ***
Last login: Fri Oct 13 18:43:43 2017 from 2.34.21.85
>
```

The first time you use the ssh to connect to a given host you are asked to conferm its authenticity.  Why?

By typing "yes" **you** assure the ssh client that it is talking to the right host, i.e. it is not victim of a man-in-the-middle attack.  The ssh client will store the pulic key and it will be associated it with that address from here on.
If you log out (by typing "exit") and re-lauch the ssh client command you won't be asked to confirm the public key again:
```
> ssh -l <USERNAME GOES HERE> pgpeduc.csec.dibris.unige.it
armando@pgpeduc.csec.dibris.unige.it's password: <PASSWORD GOES HERE>
Welcome to Ubuntu 16.04.3 LTS (GNU/Linux 4.4.0-97-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

0 packages can be updated.
0 updates are security updates.


*** System restart required ***
Last login: Fri Oct 13 18:43:43 2017 from 2.34.21.85
>
```

