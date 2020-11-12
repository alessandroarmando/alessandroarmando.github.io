docker build --tag gpg:1.0 .
docker run --hostname hey -v `pwd`/ImportedKeys:/home/armando/ImportedKeys -it gpg:1.0 bash
