sudo dpkg --configure -a
sudo apt-get -y update
sudo apt-get install -y git build-essential emacs curl wget iptables emacs automake pkg-config
if [ ! -d 'timber' ]
then
    git clone https://github.com/rasmusrygaard/timber.git
else
    cd timber
    git pull origin master
    cd -
fi
sudo iptables -A INPUT -p tcp --dport 6160 -j ACCEPT
cd timber
make
chmod +x logcabin/*.sh
echo "Starting server"
server/server > server.log &
echo "Done"
