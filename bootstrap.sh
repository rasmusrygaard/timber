sudo apt-get -y update
sudo apt-get install -y git build-essential emacs curl wget iptables emacs automake pkg-config
git clone https://github.com/rasmusrygaard/timber.git
cd timber
make
chmod +x logcabin/*.sh
server/server
