import boto.ec2 as ec2
import os
import sys

amiImgId = ""
sshStr = "-i 224WKeys.pem ubuntu@"

def launchInstances(n, awsKey, awsSecret):
	conn = ec2.connect_to_region("us-west-2", awsKey, awsSecret)
	conn.run_instances(amiImgId, min_count=n, max_count=n, key_name='CS341Keys', 
		instance_type="t1.micro", security_groups=['rasmus-master'])
	instances = conn.get_all_instances()
	ips = [ inst.ip_address for inst in instances ]
	cmds = ["scp -i 224WKeys.pem logcabin/init.sh ubuntu@" + ip + ":~" for ip in ips]
	

awsKey = os.environ.get('AWS_KEY')
awsSecret = os.environ.get('AWS_SECRET')

numInstances = int(sys.argv[1])




