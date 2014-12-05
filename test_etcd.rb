require 'etcd'
require 'set'

config = open('timber.config').readlines.map(&:rstrip)
private_ips = config.map { |line| line.split[2] }

clients = private_ips.map { |ip| puts ip; Etcd.client(host: ip, port: 4001, read_timeout: 1, allow_redirect: true) }
clients.each_with_index do |client, i|
  begin
    if client.leader[client.host]
      client.set('/result', value: [0].to_s)
    end
  rescue Exception => e
    print e
  end
end
sleep 2
start = Time.now
acked = [0]
to_write = 1
DURATION = 305 # second
clients.cycle do |client|
  while (Time.now - start) < DURATION
    res = client.get('/result')
    arr = JSON.parse(res.value)
    begin
      puts "#{ Time.now } WRITE host=#{ client.host } value=#{ to_write }"
      client.set('/result', value: (arr + [to_write]).to_s)
      puts "#{ Time.now } ACK host=#{ client.host } value=#{ to_write }"
      acked << to_write
      to_write += 1
    rescue Net::HTTPFatalError
      puts "#{ Time.now } PARTITIONED host=#{ client.host } value=#{ to_write }"
      break # Break out of the while loop to move to the next client
    rescue Net::ReadTimeout
      puts "#{ Time.now } TIMEOUT host=#{ client.host } value=#{ to_write }"
      break # Break out of the while loop to move to the next client
    rescue Net::HTTPRetriableError
      puts "#{ Time.now } REJECT host=#{ client.host } value=#{ to_write }"
      break # Break out of the while loop to move to the next client
    end
  end
  break if (Time.now - start) >= DURATION
end

result_str = nil
clients.each do |client|
  if client.leader[client.host]
    result_str = client.get('/result').value
  end
end
result = JSON.parse(result_str)
puts "|A|: #{ acked.size }"
puts "|R|: #{ result.size }"
puts "S: #{ (result - acked).join(', ') }"
puts "|S|: #{ (result - acked).size }"
puts "D: #{ (acked - result).join(', ') }"
puts "|D|: #{ (acked - result).size }"
puts "|D|/|A|: #{ (acked - result).size / acked.size.to_f }"
puts "|S|/|A|: #{ (result - acked).size / acked.size.to_f }"
