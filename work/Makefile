CXX = g++
CXXFLAGS = -I /usr/include/cryptopp 
LDFLAGS = -L /urs/include/cryptopp -l cryptopp

all: canfd crypto hash hmac code

canfd: canfd_recv canfd_send

fresh: fresh_in fresh_out

canfd_send: canfd_send.cpp canfd_send_main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@.out $(LDFLAGS)

canfd_recv: canfd_recv.cpp canfd_recv_main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@.out $(LDFLAGS)

crypto: crypto_intro.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@.out $(LDFLAGS)

hash: canfd_recv.cpp canfd_send.cpp hash_frame.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@.out $(LDFLAGS)

hmac: canfd_recv.cpp canfd_send.cpp hmac_frame.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@.out $(LDFLAGS)

fresh_out: canfd_send.cpp fresh_frame_out.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@.out $(LDFLAGS)

fresh_in: canfd_recv.cpp fresh_frame_in.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@.out $(LDFLAGS)

clean:
	rm -f canfd crypto hash hmac
