# Automotice Securtiy

* 
* - hashed (SHA246)
* A common way
* 
* - symmetric keys (AES)
* Cause we want to prove the authentication of the ECU we need to sign a CAN frame somehow.
* Signignig can be done with symmetric keys, cuase they are smal in size which ich benefical for low memory ECUs.
* 
* - hash message authentication codes (HMACs)
* A common way to prove the authenticy of data is to use