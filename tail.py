import os
import sys

index = int(sys.argv[1])
info = [line.rstrip() for line in open('timber.config')][index - 1]

id, public_ip, private_ip = info.split('\t')
print("/usr/bin/ssh -i 224WKeys.pem -o StrictHostKeyChecking=no ubuntu@%s tail -f server.log" % public_ip)
os.system("/usr/bin/ssh -i 224WKeys.pem -o StrictHostKeyChecking=no ubuntu@%s 'tail -f server.log'" % public_ip)
