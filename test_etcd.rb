require 'etcd'
require 'set'

config = open('timber.config').readlines.map(&:rstrip)
private_ips = config.map { |line| line.split[2] }

clients = private_ips.map { |ip| Etcd.client(host: ip, port: 4001, read_timeout: 1) }
start = Time.now
clients[0].set('/result', value: [0].to_s)
acked = [0]
DURATION = 80 # second
clients.cycle do |client|
  while (Time.now - start) < DURATION
    res = client.get('/result')
    arr = JSON.parse(res.value)
    to_write = arr.last + 1
    begin
      puts "#{ Time.now } WRITE host=#{ client.host } value=#{ to_write }"
      client.set('/result', value: (arr + [to_write]).to_s)
      puts "#{ Time.now } ACK host=#{ client.host } value=#{ to_write }"
      acked << to_write
    rescue Net::ReadTimeout
      puts "#{ Time.now } TIMEOUT host=#{ client.host } value=#{ to_write }"
      break # Break out of the while loop to move to the next client
    rescue Etcd::TestFailed
      puts "#{ Time.now } REJECT host=#{ client.host } value=#{ to_write }"
      break # Break out of the while loop to move to the next client
    end
  end
  break if (Time.now - start) >= DURATION
end

arr = JSON.parse(client.get('/result').value)
puts "Elements in result: #{ arr.size }"
expected_size = acked.size
puts "Expected size: #{ expected_size }"
puts "Writes Dropped: #{ 100 * (1 - (arr.uniq.size / expected_size.to_f)) } %"
puts "Ghost Writes: #{ 100 * ((arr.size - Set.new(arr).size) / expected_size.to_f) } %"
