import subprocess
import boto.ec2 as ec2
import time
import os
import socket

def create_connection(awsKey=os.environ.get('AWS_KEY'),
                      awsSecret=os.environ.get('AWS_SECRET')):
    return ec2.connect_to_region("us-east-1",
                                 aws_access_key_id=awsKey,
                                 aws_secret_access_key=awsSecret)

def instance_public_ips(conn, ids):
    launched_instances = conn.get_only_instances(instance_ids=ids)
    return [ inst.ip_address for inst in launched_instances ]

def instance_private_ips(conn, ids):
    launched_instances = conn.get_only_instances(instance_ids=ids)
    return [ inst.private_ip_address for inst in launched_instances ]

def launch_instances(conn, n):
    reservation = conn.run_instances('ami-84c259ec',
                                     min_count=n,
                                     max_count=n,
                                     key_name='224WKeys',
                                     instance_type="t2.micro",
                                     subnet_id='subnet-191d4231',
                                     security_group_ids=['sg-721a0417'] ) # 'timber'
    try:
        instances = reservation.instances
        ids = [ inst.id for inst in instances ]

        while True:
            instance_statuses = conn.get_all_instance_status(instance_ids=ids)
            states = [instance.state_name for instance in instance_statuses]
            done = all(status == 'running' for status in states) and len(states) is n
            if done:
                break
            time.sleep(5)
        return ids
    except Exception as e:
        print 'ooops, terminating instances', e
        ids = [ inst.id for inst in reservation.instances ]
        conn.terminate_instances(ids)
        return []

def test_conn(ip, tries=10):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    while tries > 0:
        try:
            s.connect((address, 22))
            s.close()
        except:
            tries -= 1
            print "Port 22 unavailable, waiting 5 seconds and trying again"
            time.sleep(5)

def run_script(conn, instance_ids, scriptfile):
    ips = instance_public_ips(conn, instance_ids)
    copy_cmds = ["/usr/bin/ssh -i 224WKeys.pem -o StrictHostKeyChecking=no ubuntu@%s bash -s" % ip for ip in ips]

    for ip in ips:
        test_conn(ip)
    processes = []
    for cmd in copy_cmds:
        print cmd

        processes.append(subprocess.Popen(cmd.split(' '), stdin=open(scriptfile)))

    for process in processes:
        process.wait()

def send_conf(ip, name="timber.config"):
    cmd = "/usr/bin/scp -i 224WKeys.pem " + name + " ubuntu@" + ip + ":~/timber.config"
    proc = subprocess.Popen(cmd)
    proc.wait()

if not os.path.isfile('client.config'):
    print "Create connection"
    conn = create_connection()
    print "Launch instance"
    ids = launch_instances(conn, 1)
    print 'Got machines', ' '.join(ids)
    f = open('client.config', 'w')
    info = zip(ids, instance_public_ips(conn, ids), instance_private_ips(conn, ids))
    for id, public_ip, private_ip in info:
        f.write(id + '\t' + public_ip + '\t' + private_ip + '\n')
    print 'Running script client_init.sh'
    run_script(conn, ids, 'logcabin/client_init.sh')
    (_,ip,_) = info[0]
    send_conf(ip)
else:
    print "Client instance already exists (in client.config), use that or kill it with kill_ec2.py"