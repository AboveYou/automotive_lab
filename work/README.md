# Structure

1. intro into CAN-FD
1.1 send a frame on the can bus and see it with can-utils
1.2 receive a frame with another program and display the content
1.2 watch the communication on the bus in Wireshark

2. intro into Crypto++
2.1 how to generate hashes#include <cassert>
2.2 how to generate symmetric keys
2.3 how to use the keys to generate HMACs

3. hashing
3.1 hash the payload of a frame bevore sending and append the hash to the payload
3.2 write logic to verify the hash on the receiving side
3.3 is there a problem with this approach?

4. HMACs
4.1 generate a symmetric key and store it on the sender and receiver (key exchange is not part of this practise)
4.2 use an HMAC to hash the payload and append it bevore sending
4.3 adapt the logic on the receiving side to verifry the integrity unsing the HMAC
4.4 does this eliminate all kind of attacks?

5. fressness
5.2 implement a counter a the sending and receiving side and increase it with every package send/recevied from a specific ECU (source address)
5.3 bevore sending add the fressness counter to the payload and create an HMAC
5.3 on the reveicing side chek the HMAC and the fresness values in the payload
5.4 can you see a problem by using this method?
