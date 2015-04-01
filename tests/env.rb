


PROJECT_ROOT="~/src/esb/canmed/"
TESTS_DIR="#{PROJECT_ROOT}/tests"
BIN_DIR="#{PROJECT_ROOT}/build/src"

CANMED_EXEC="#{BIN_DIR}/canmed"

require 'test/unit/assertions'


module SysTest

 	include Test::Unit::Assertions
 
	def term_wait_kill(childproc, timeout=10, retcode=0)
		Process.kill('TERM', childproc)
		wait_kill(childproc, timeout, retcode, signal=Signal.list['TERM'])	
	end
	
	def wait_kill(childproc, timeout=10, retcode=0, signal=nil)
		def check_alive(childproc, _retcode=0, _signal=nil) 
			begin
				pid, exitstat = Process.waitpid2(childproc, Process::WNOHANG)
				## puts "...... signal  #{pid}/#{exitstat}"
				if pid==nil
					## puts "...... running"
					## process not terminated yet 
					##
					## continue, sleep a second and raise exception 
					## if timeout expired
					return true
				elsif pid>0 and exitstat.success? and _retcode==0
					## process terminated with success
					## puts "...... terminated with success #{exitstat}"
					return false 
				elsif pid>0 and exitstat.signaled?  and exitstat.termsig == _signal
					## process terminated with SIGTERM, which is Ok
					## succeeded - terminate the loop
					## puts "...... terminated with signal #{exitstat}"
					return false
				elsif pid>0 and exitstat.signaled? 
					## process terminated with unexpted signal
					## puts "...... terminated with signal #{exitstat}/#{exitstat.termsig}/#{_signal}"
					assert(false, 'process terminated with error signal')

				elsif pid>0 and exitstat.exited?
					## process terminated with error code
					## puts "...... terminated with error code #{exitstat}"
					assert(false, 'process exited with error')		
				end
			rescue  SystemCallError => e
				## puts "...... caught exception #{e}/#{e.errno}"
				
				if e.errno == 10
					## not a child process, but still alive
					## puts "...... process still alive"
					return true
				else
					## here we can not tell, if the process did exist at all in the past 
					## and if it just terminated or with what kind of exitstate.
					## therefor we just return that the process is no longer alive
					
					## puts "...... process no longer alive"
					return false
				end
			end
		
		end
		
		
		for loop in 1..timeout
			puts ".... terminated? #{childproc}"
			alive = check_alive(childproc, retcode, signal)
			if not alive
				break
			else
				## continue loop			
				if loop==timeout 
				 	Process.kill("KILL", childproc);
				 	for forceloop in 1..5
				 		surviving = check_alive(childproc, retcode, Signal.list['KILL'])
				 		 
				 		if surviving==false
				 			break
				 		end
				 	end
				 	puts 'ERROR, timeout expired, process did not terminate within time'
				 	assert(false, 'process timeout reached')
				end
				
				## sleep one second
				sleep 1
			end		
			
 		end
	end


	def reachable_wait(protocol, port, timeout=10)
		single_entry_pattern = Regexp.new(
		/^\s*\d+:\s+(.{8}):(.{4})\s+(.{8}):(.{4})\s+(.{2})/
		)

		tcp_states = {
		'00' => 'UNKNOWN', # Bad state ... Impossible to achieve ...
		'FF' => 'UNKNOWN', # Bad state ... Impossible to achieve ...
		'01' => 'ESTABLISHED',
		'02' => 'SYN_SENT',
		'03' => 'SYN_RECV',
		'04' => 'FIN_WAIT1',
		'05' => 'FIN_WAIT2',
		'06' => 'TIME_WAIT',
		'07' => 'CLOSE',
		'08' => 'CLOSE_WAIT',
		'09' => 'LAST_ACK',
		'0A' => 'LISTEN',
		'0B' => 'CLOSING'
		}

		for ntimes in 1..timeout
			found = false
			## puts ".. waiting for port #{port}"
			File.open('/proc/net/' + protocol).each do |i|
			  i = i.strip
			  if match = i.match(single_entry_pattern)
				local_IP = match[1].to_i(16)
				local_IP = [local_IP].pack("N").unpack("C4").reverse.join('.')
				local_port = match[2].to_i(16)
				remote_IP = match[3].to_i(16)
				remote_IP = [remote_IP].pack("N").unpack("C4").reverse.join('.')
				remote_port = match[4].to_i(16)
				connection_state = match[5]
				connection_state = tcp_states[connection_state]
		
				## puts ".... #{local_IP}:#{local_port} " + "#{remote_IP}:#{remote_port} #{connection_state}"
				if local_port == port
					puts ".... endpoint reachable #{port}"
					found = true
					break
				end
			  end
			end
			if found
				break;
			end
			
			if ntimes==timeout
			   puts 'ERROR, timeout expired, endpoint not available'
			   assert(false, 'endpoint not available')
			end
		
			sleep 1
		end

	end
	
end


