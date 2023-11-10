docker build -t gpg:1.0 .

docker run --hostname hey -v `pwd`/ImportedKeys:/home/armando/ImportedKeys -it gpg:1.0 bash

docker run --hostname hey -v `pwd`/pk-exercise:/home/armando/ImportedKeys -it gpg:1.0 bash
