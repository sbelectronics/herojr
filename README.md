Hero Jr Project Repo

I bought a hero jr with no electronics.

This is me making my own electronics.


Running things:

Configuration
   powerboard
      echo > /etc/hero/nullspeechboard
   sensorboard
      echo > /etc/hero/proxypowerboard

On both the sensorboard and the powerboard,
    cd projects/pi/herojr/python
    python ./herocontrol -r diags

On the remote board,
    cd projects/pi/remote
    python ./remote.py -H pi-heromotor -H pi-herosensor