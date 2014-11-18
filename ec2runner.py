import boto.ec2 as ec2
import os
import time
import sys


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
    reservation = conn.run_instances('ami-9eaa1cf6',
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




if not os.path.isfile('timber.config'):
    count = int(sys.argv[1])
    print 'Create connection'
    conn = create_connection()
    print 'Launching instances'
    ids = launch_instances(conn, count)
    print 'Got machines', ' '.join(ids)
    f = open('timber.config', 'w')
    info = zip(ids, instance_public_ips(conn, ids), instance_private_ips(conn, ids))
    for id, public_ip, private_ip in info:
        f.write(id + '\t' + public_ip + '\t' + private_ip + '\n')



else:
    print 'Instances already exist, terminate them with kill_ec2.py'
