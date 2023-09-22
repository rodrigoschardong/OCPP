# OCPP
Exploring and validating OCPP protocol

----
You can read the [python documentation](https://github.com/mobilityhouse/ocpp) for more information.

Installation
------------

Install websockets on python:

```bash
   $ sudo apt-get update
   $ sudo apt-get install libwebsockets-dev
```

Quick start
-----------

- Create a config.json file from config.example.json 
- Fill config.json file with your personal information
- Run a server script 
```
   $ sh build_and_run_central_system.sh
```
- Run a client script
```
   $ sh build_and_run_charge_point.sh
```


