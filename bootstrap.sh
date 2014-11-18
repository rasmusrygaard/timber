sudo apt-get -y update
sudo apt-get install -y git build-essential emacs curl wget iptables emacs automake pkg-config
git clone https://github.com/rasmusrygaard/timber.git
iptables -A INPUT -p tcp --dport 6160 -j ACCEPT
cd timber
make
chmod +x logcabin/*.sh
server/server
