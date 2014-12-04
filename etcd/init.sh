rm -rf *etcd*
curl -L  https://github.com/coreos/etcd/releases/download/v0.4.6/etcd-v0.4.6-linux-amd64.tar.gz -o etcd-v0.4.6-linux-amd64.tar.gz
tar xzvf etcd-v0.4.6-linux-amd64.tar.gz
export ETCD_HTTP_READ_TIMEOUT=1.0
export ETCD_HTTP_WRITE_TIMEOUT=1.0
mv etcd-v0.4.6-linux-amd64.tar.gz etcd
