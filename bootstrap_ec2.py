import subprocess
import boto.ec2 as ec2
import os

def create_connection(awsKey=os.environ.get('AWS_KEY'),
                      awsSecret=os.environ.get('AWS_SECRET')):
    return ec2.connect_to_region("us-east-1",
                                 aws_access_key_id=awsKey,
                                 aws_secret_access_key=awsSecret)

def instance_public_ips(conn, ids):
    launched_instances = conn.get_only_instances(instance_ids=ids)
    return [ inst.ip_address for inst in launched_instances ]

def run_script(conn, instance_ids, scriptfile):
    ips = instance_public_ips(conn, instance_ids)

    copy_cmds = ["/usr/bin/ssh -i 224WKeys.pem -o StrictHostKeyChecking=no ubuntu@" + ip + " bash -s" for ip in ips]
    processes = [subprocess.Popen(cmd.split(' '), stdin=open('bootstrap.sh')) for cmd in copy_cmds]
    for p in processes:
        print 'Waiting for process', p
        p.wait()

if not os.path.isfile('timber.config'):
    print 'No instances running'
    exit()

f = open('timber.config')
conn = create_connection()
ids = [line.split('\t')[0] for line in f]
print 'Running script bootstrap.sh'
run_script(conn, ids, 'bootstrap.sh')
