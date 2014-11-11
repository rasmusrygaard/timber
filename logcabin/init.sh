git clone https://github.com/logcabin/logcabin.git
cd logcabin
git submodule update --init
sudo apt-get install -y scons protobuf-compiler libcrypto++
wget https://protobuf.googlecode.com/files/protobuf-2.5.0.tar.bz2
tar -xvjf protobuf-2.5.0.tar.bz2
cd protobuf-2.5.0.tar.bz2
./configure --prefix=/usr
make
make check
sudo make install
cd ..
scons
