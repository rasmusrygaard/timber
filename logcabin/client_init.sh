cd ~
sudo apt-get -y update
sudo apt-get install -y git wget build-essential
if ! which protoc >/dev/null; then
  wget -N https://protobuf.googlecode.com/files/protobuf-2.5.0.tar.bz2 &> protobuf_wget.log
  tar -xvjf protobuf-2.5.0.tar.bz2
  cd protobuf-2.5.0
  ./configure --prefix=/usr
  make
  make check
  sudo make install
fi
cd ~
if [ ! -d "timber" ]; then
  git clone https://github.com/rasmusrygaard/timber.git &> timber_clone.log
fi
if [ ! -d "logcabin" ]; then
  git clone https://github.com/logcabin/logcabin.git &> logcabin_clone.log
  cp timber/logcabin/WriteInts.cc logcabin/Examples/WriteInts.cc
  cp timber/logcabin/SConscript logcabin/Examples/SConscript
  cd logcabin
  git submodule update --init
  sudo apt-get install -y scons protobuf-compiler libcrypto++
  scons
fi
