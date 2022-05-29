# flood.cpp
A DDOS tool for spamming GET and/or POST requests written in C++
```
Compile: g++ flood.cpp -pthread -lcurl -o flood

Usage: ./flood [-m method] [-t count] -u target_url

  Options:

    -m            [ GET | POST | GETPOST ] Default: GET
    -t            Number of threads to use. Default: 12
    -u            URL to attack

  Examples:

    ./flood -u localhost

    ./flood -m POST -t 48 -u localhost
```
