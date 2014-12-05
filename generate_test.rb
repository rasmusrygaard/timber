# Usage:
# ruby generate_test.rb [partition size] [slow network] [duration] [pause]
#   [partition size]: 0/1/2 - The number of nodes to partition every PAUSE seconds
#   [slow network?]: 'true'/'false' - Should we slow down the network?
#   [duration]: [1..] - The duration of the script in seconds
#   [pause]: [1..] - The number of seconds to pause between partitions

NODES = (1..5).to_a

raise ArgumentError, 'usage: generate_test.rb [partition size] [slow network?] [duration] [pause]' if ARGV.size != 4
partition_size, slow_network, duration, pause = ARGV
partition_size = partition_size.to_i
duration = duration.to_i
pause = pause.to_i
raise ArgumentError, "Invalid partition size #{ partition_size }" unless (0..2).include? partition_size
slow_network = slow_network[0] == 't'

puts 'heal_cluster'
puts 'slow' if slow_network
if partition_size > 0
  (duration / pause).times do
    puts "snubnodes #{ NODES.sample(partition_size).join(' ') }"
    puts "sleep #{ pause }"
  end
  puts 'heal_cluster'
else
  puts "sleep #{ duration }"
end
puts 'heal' if slow_network

puts 'exit'
