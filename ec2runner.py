import boto.ec2 as ec2
import os
import time
import sys

def create_connection(awsKey=os.environ.get('AWS_KEY'),
                      awsSecret=os.environ.get('AWS_SECRET')):
    return ec2.connect_to_region("us-east-1",
                                 aws_access_key_id=awsKey,
                                 aws_secret_access_key=awsSecret)

def instance_ips(conn, ids):
    launched_instances = conn.get_only_instances(instance_ids=ids)
    return [ inst.ip_address for inst in launched_instances ]


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
            done = any(status == 'running' for status in states) and len(states) is n
            if done:
                break
            time.sleep(5)
        return ids
    except Exception as e:
        print 'ooops, terminating instances', e
        ids = [ inst.id for inst in reservation.instances ]
        conn.terminate_instances(ids)
        return []

def run_script(conn, instance_ids, scriptfile):
    ips = instance_ips(conn, instance_ids)

    copy_cmds = ["ssh -i 224WKeys.pem -o StrictHostKeyChecking=no ubuntu@" + ip + " sudo bash -s < " + scriptfile for ip in ips]
    for cmd in copy_cmds:
        print cmd

print 'Create connection'
conn = create_connection()
print 'Launching instances'
ids = launch_instances(conn, 1)
print 'Got machines', ' '.join(ids)
print 'Running script bootstrap.sh'
run_script(conn, ids, 'bootstap.sh')
