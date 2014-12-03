require 'etcd'
require 'set'

config = open('timber.config').readlines.map(&:rstrip)
private_ips = config.map { |line| line.split[2] }

clients = private_ips.map { |ip| Etcd.client(host: ip, port: 4001, allow_redirect: true) }
count = 0
clients.cycle do |client|
  while true
    begin
      arr = JSON.parse(client.get('/result').value)
    rescue Etcd::KeyNotFound
      arr = []
    end
    arr << count
    begin
      Timeout::timeout(2) {
        puts "#{ Time.now } WRITE host=#{ client.host } value=#{ count }"
        client.set('/result', value: arr.to_s)
        puts "#{ Time.now } ACK host=#{ client.host } value=#{ count }"
      }
    rescue Timeout::Error
      puts "#{ Time.now } TIMEOUT host=#{ client.host } value=#{ count }"
      break # Break out of the while loop to move to the next client
    end
    count += 1
    if count > 200
      break
    end
  end
end

arr = JSON.parse(client.get('/result').value)
puts "Elements in result: #{ arr.size }"
expected_size = arr.max + 1
puts "Expected size: #{ expected_size }"
puts "Writes Droppped: #{ 100 * (1 - (arr.size / expected_size.to_f)) } %"
puts "Ghost Writes: #{ 100 * ((arr.size - Set.new(arr).size) / expected_size.to_f) } %"
