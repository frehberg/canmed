#!/usr/bin/ruby

require '../env' 
require 'test/unit/assertions'

include SysTest
 
command = "#{CANMED_EXEC} -d udp:port=7777"

puts "spawning #{command}"

child=Process.spawn(command)

sleep 2
reachable_wait('udp', 7777, 4)

term_wait_kill(child, timeout=4)
