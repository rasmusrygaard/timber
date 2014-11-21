sudo dpkg --configure -a
sudo apt-get -y update
sudo apt-get install -y git build-essential emacs curl wget iptables emacs automake pkg-config bison
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
cd ~
echo "Starting server"
nohup sudo timber/server/server &> server.log &
echo "Done"
