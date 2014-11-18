import boto.ec2 as ec2

def create_connection(awsKey=os.environ.get('AWS_KEY'),
                      awsSecret=os.environ.get('AWS_SECRET')):
    return ec2.connect_to_region("us-east-1",
                                 aws_access_key_id=awsKey,
                                 aws_secret_access_key=awsSecret)

def terminate_instances(conn, ids):
    conn.terminate_instances(ids)

f = open('timber.config')
conn = create_connection()
ids = [line.split('\t')[0] for line in f]

terminate_instances(conn, ids)
