cd ~
if ! which protoc >/dev/null; then
  wget https://protobuf.googlecode.com/files/protobuf-2.5.0.tar.bz2
  tar -xvjf protobuf-2.5.0.tar.bz2
  cd protobuf-2.5.0
  ./configure --prefix=/usr
  make
  make check
  sudo make install
fi
cd ~
if [ -d "logcabin" ]; then
  git clone https://github.com/logcabin/logcabin.git
  cd logcabin
  git submodule update --init
  sudo apt-get install -y scons protobuf-compiler libcrypto++
  scons
fi
