# httpspam.cpp
A DDOS tool for spamming GET or POST requests written in C++
```
Usage: ./httpspam.exe [-m method] [-t count] -u target_url

  Options:

    -m            Method(s) to use. GET | POST | GETPOST
    -t            Number of threads to use. Default: 12
    -u            URL to attack

Examples:

./httpspam.exe -u localhost

./httpspam.exe -m POST -t 48 -u localhost
```
