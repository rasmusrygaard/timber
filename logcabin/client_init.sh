cd ~
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
else
  cd timber
  git pull
  cd ..
fi
cp timber/logcabin/WriteInts.cc logcabin/Examples/WriteInts.cc
cp timber/logcabin/SConscript logcabin/Examples/SConscript
cd logcabin
git submodule update --init
scons
