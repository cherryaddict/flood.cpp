# httpspam.cpp
A DDOS tool for spamming GET and/or POST requests written in C++
```
Usage: ./httpspam.exe [-m method] [-t count] -u target_url

  Options:

    -m            [ GET | POST | GETPOST ] Default: GET
    -t            Number of threads to use. Default: 12
    -u            URL to attack

  Examples:

    ./httpspam.exe -u localhost

    ./httpspam.exe -m POST -t 48 -u localhost
```
